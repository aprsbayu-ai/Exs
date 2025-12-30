#pragma once
#ifndef CYBERGUARDIAN_PROFILING_INTERNAL_H
#define CYBERGUARDIAN_PROFILING_INTERNAL_H

#include <string>
#include <chrono>
#include <unordered_map>
#include <mutex>
#include <vector>
#include <memory>

namespace CyberGuardian {
namespace Internal {
namespace Common {

struct ProfileSample_Exs {
    std::string name_Exs;
    std::chrono::high_resolution_clock::time_point startTime_Exs;
    std::chrono::high_resolution_clock::time_point endTime_Exs;
    uint64_t duration_Exs;
    std::thread::id threadId_Exs;
    int depth_Exs;
};

class Profiler_Exs {
private:
    static Profiler_Exs* instance_Exs;
    
    struct ThreadData_Exs {
        std::vector<ProfileSample_Exs> samples_Exs;
        std::vector<int> sampleStack_Exs;
        std::chrono::high_resolution_clock::time_point frameStart_Exs;
    };
    
    std::unordered_map<std::thread::id, ThreadData_Exs> threadData_Exs;
    std::mutex profilerMutex_Exs;
    bool enabled_Exs;
    uint64_t frameCount_Exs;
    uint64_t maxSamplesPerFrame_Exs;
    
    Profiler_Exs();
    ~Profiler_Exs();
    
public:
    static Profiler_Exs* GetInstance_Exs();
    static void DestroyInstance_Exs();
    
    void BeginFrame_Exs();
    void EndFrame_Exs();
    
    void BeginSample_Exs(const std::string& name);
    void EndSample_Exs();
    
    void Enable_Exs(bool enable);
    bool IsEnabled_Exs() const;
    
    void SetMaxSamplesPerFrame_Exs(uint64_t maxSamples);
    
    std::vector<ProfileSample_Exs> GetSamples_Exs() const;
    void ClearSamples_Exs();
    
    double GetAverageFrameTime_Exs() const;
    double GetMinFrameTime_Exs() const;
    double GetMaxFrameTime_Exs() const;
    
    void DumpProfileData_Exs(const std::string& filename = "profile_data.json");
    
private:
    ThreadData_Exs& GetThreadData_Exs();
};

class ScopedProfile_Exs {
private:
    std::string name_Exs;
    
public:
    ScopedProfile_Exs(const std::string& name);
    ~ScopedProfile_Exs();
};

#define PROFILE_SCOPE_Exs(name) \
    CyberGuardian::Internal::Common::ScopedProfile_Exs profileScope_##__LINE__(name)

#define PROFILE_FUNCTION_Exs() \
    PROFILE_SCOPE_Exs(__FUNCTION__)

} // namespace Common
} // namespace Internal
} // namespace CyberGuardian

#endif
