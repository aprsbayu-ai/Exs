#pragma once
#ifndef CYBERGUARDIAN_PLATFORMABSTRACTION_INTERNAL_H
#define CYBERGUARDIAN_PLATFORMABSTRACTION_INTERNAL_H

#include <string>
#include <vector>

namespace CyberGuardian {
namespace Internal {
namespace Common {

enum class PlatformType_Exs {
    WINDOWS_Exs,
    LINUX_Exs,
    MACOS_Exs,
    ANDROID_Exs,
    IOS_Exs,
    UNKNOWN_Exs
};

enum class CPUArchitecture_Exs {
    X86_Exs,
    X64_Exs,
    ARM_Exs,
    ARM64_Exs,
    UNKNOWN_Exs
};

struct SystemInfo_Exs {
    PlatformType_Exs platform_Exs;
    CPUArchitecture_Exs architecture_Exs;
    std::string platformName_Exs;
    std::string platformVersion_Exs;
    std::string cpuName_Exs;
    int cpuCoreCount_Exs;
    size_t totalMemory_Exs;
    size_t availableMemory_Exs;
    std::string gpuName_Exs;
    std::string gpuVendor_Exs;
    size_t gpuMemory_Exs;
    std::string displayName_Exs;
    int displayWidth_Exs;
    int displayHeight_Exs;
    int displayRefreshRate_Exs;
};

class Platform_Exs {
private:
    static Platform_Exs* instance_Exs;
    SystemInfo_Exs systemInfo_Exs;
    
    Platform_Exs();
    ~Platform_Exs();
    
public:
    static Platform_Exs* GetInstance_Exs();
    static void DestroyInstance_Exs();
    
    bool Initialize_Exs();
    void Shutdown_Exs();
    
    SystemInfo_Exs GetSystemInfo_Exs() const;
    PlatformType_Exs GetPlatformType_Exs() const;
    CPUArchitecture_Exs GetArchitecture_Exs() const;
    
    std::string GetExecutablePath_Exs() const;
    std::string GetCurrentDirectory_Exs() const;
    bool SetCurrentDirectory_Exs(const std::string& path);
    
    std::string GetEnvironmentVariable_Exs(const std::string& name) const;
    bool SetEnvironmentVariable_Exs(const std::string& name, const std::string& value);
    
    size_t GetPageSize_Exs() const;
    size_t GetAllocationGranularity_Exs() const;
    
    void* AllocateMemory_Exs(size_t size, size_t alignment = 0);
    void FreeMemory_Exs(void* ptr);
    bool ProtectMemory_Exs(void* ptr, size_t size, bool read, bool write, bool execute);
    
    uint64_t GetPerformanceCounter_Exs() const;
    uint64_t GetPerformanceFrequency_Exs() const;
    uint64_t GetSystemTime_Exs() const;
    
    void Sleep_Exs(uint64_t milliseconds);
    void Yield_Exs();
    
    std::string GetClipboardText_Exs() const;
    bool SetClipboardText_Exs(const std::string& text);
    
    bool OpenURL_Exs(const std::string& url);
    bool ExecuteProcess_Exs(const std::string& command, const std::vector<std::string>& args);
    
    std::string GetUniqueDeviceId_Exs() const;
    std::string GetSystemLanguage_Exs() const;
    std::string GetSystemRegion_Exs() const;
    
    bool IsDebuggerPresent_Exs() const;
    void DebugBreak_Exs();
    
private:
    void DetectPlatform_Exs();
    void DetectCPU_Exs();
    void DetectGPU_Exs();
    void DetectDisplay_Exs();
    void DetectMemory_Exs();
};

} // namespace Common
} // namespace Internal
} // namespace CyberGuardian

#endif
