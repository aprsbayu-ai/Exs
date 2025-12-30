#pragma once
#ifndef CYBERGUARDIAN_MEMORYMANAGER_INTERNAL_H
#define CYBERGUARDIAN_MEMORYMANAGER_INTERNAL_H

#include <cstddef>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <string>
#include <vector>

namespace CyberGuardian {
namespace Internal {
namespace Common {

struct AllocationInfo_Exs {
    void* ptr_Exs;
    size_t size_Exs;
    std::string file_Exs;
    int line_Exs;
    std::string type_Exs;
    std::chrono::steady_clock::time_point timestamp_Exs;
};

class MemoryManager_Exs {
private:
    static MemoryManager_Exs* instance_Exs;
    std::unordered_map<void*, AllocationInfo_Exs> allocations_Exs;
    std::mutex mutex_Exs;
    size_t totalAllocated_Exs;
    size_t peakAllocated_Exs;
    bool trackAllocations_Exs;
    
    MemoryManager_Exs();
    ~MemoryManager_Exs();
    
public:
    static MemoryManager_Exs* GetInstance_Exs();
    static void DestroyInstance_Exs();
    
    void* Allocate_Exs(size_t size, const char* file = __FILE__, int line = __LINE__);
    void* AllocateAligned_Exs(size_t size, size_t alignment, const char* file = __FILE__, int line = __LINE__);
    void Deallocate_Exs(void* ptr);
    void DeallocateAligned_Exs(void* ptr);
    
    template<typename T, typename... Args>
    T* New_Exs(Args&&... args);
    
    template<typename T>
    void Delete_Exs(T* ptr);
    
    size_t GetTotalAllocated_Exs() const;
    size_t GetPeakAllocated_Exs() const;
    size_t GetAllocationCount_Exs() const;
    
    void EnableTracking_Exs(bool enable);
    void DumpLeaks_Exs(const std::string& filename = "memory_leaks.log");
    void ResetStats_Exs();
    
private:
    void RegisterAllocation_Exs(void* ptr, size_t size, const char* file, int line, const char* type = "unknown");
    void UnregisterAllocation_Exs(void* ptr);
    void UpdatePeak_Exs();
};

template<typename T, typename... Args>
T* MemoryManager_Exs::New_Exs(Args&&... args) {
    void* memory = Allocate_Exs(sizeof(T), __FILE__, __LINE__);
    if (!memory) return nullptr;
    
    try {
        T* obj = new(memory) T(std::forward<Args>(args)...);
        RegisterAllocation_Exs(memory, sizeof(T), __FILE__, __LINE__, typeid(T).name());
        return obj;
    } catch (...) {
        Deallocate_Exs(memory);
        throw;
    }
}

template<typename T>
void MemoryManager_Exs::Delete_Exs(T* ptr) {
    if (!ptr) return;
    
    ptr->~T();
    Deallocate_Exs(ptr);
}

} // namespace Common
} // namespace Internal
} // namespace CyberGuardian

#endif
