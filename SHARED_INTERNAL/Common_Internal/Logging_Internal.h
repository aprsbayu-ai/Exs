#pragma once
#ifndef CYBERGUARDIAN_LOGGING_INTERNAL_H
#define CYBERGUARDIAN_LOGGING_INTERNAL_H

#include <string>
#include <fstream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <sstream>
#include <chrono>

namespace CyberGuardian {
namespace Internal {
namespace Common {

enum class LogLevel_Exs {
    TRACE_Exs,
    DEBUG_Exs,
    INFO_Exs,
    WARNING_Exs,
    ERROR_Exs,
    CRITICAL_Exs
};

struct LogMessage_Exs {
    LogLevel_Exs level_Exs;
    std::string message_Exs;
    std::string source_Exs;
    std::chrono::system_clock::time_point timestamp_Exs;
    int threadId_Exs;
};

class Logger_Exs {
private:
    static Logger_Exs* instance_Exs;
    
    std::ofstream logFile_Exs;
    std::mutex logMutex_Exs;
    std::queue<LogMessage_Exs> logQueue_Exs;
    std::thread logThread_Exs;
    std::condition_variable logCondition_Exs;
    std::atomic<bool> running_Exs;
    LogLevel_Exs minLevel_Exs;
    bool echoToConsole_Exs;
    
    Logger_Exs();
    ~Logger_Exs();
    
    void ProcessLogs_Exs();
    void WriteLog_Exs(const LogMessage_Exs& message);
    std::string LevelToString_Exs(LogLevel_Exs level) const;
    std::string FormatTimestamp_Exs(const std::chrono::system_clock::time_point& timestamp) const;
    
public:
    static Logger_Exs* GetInstance_Exs();
    static void DestroyInstance_Exs();
    
    bool Initialize_Exs(const std::string& logFile = "logs/game.log");
    void Shutdown_Exs();
    
    void SetMinLevel_Exs(LogLevel_Exs level);
    void SetEchoToConsole_Exs(bool echo);
    
    void Log_Exs(LogLevel_Exs level, const std::string& message, const std::string& source = "");
    
    void LogTrace_Exs(const std::string& message, const std::string& source = "");
    void LogDebug_Exs(const std::string& message, const std::string& source = "");
    void LogInfo_Exs(const std::string& message, const std::string& source = "");
    void LogWarning_Exs(const std::string& message, const std::string& source = "");
    void LogError_Exs(const std::string& message, const std::string& source = "");
    void LogCritical_Exs(const std::string& message, const std::string& source = "");
    
    void Flush_Exs();
    
private:
    class LogStream_Exs {
    private:
        LogLevel_Exs level_Exs;
        std::string source_Exs;
        std::ostringstream stream_Exs;
        
    public:
        LogStream_Exs(LogLevel_Exs level, const std::string& source);
        ~LogStream_Exs();
        
        template<typename T>
        LogStream_Exs& operator<<(const T& value) {
            stream_Exs << value;
            return *this;
        }
    };
};

#define LOG_TRACE_Exs(message) \
    CyberGuardian::Internal::Common::Logger_Exs::GetInstance_Exs()->LogTrace_Exs(message, __FILE__)

#define LOG_DEBUG_Exs(message) \
    CyberGuardian::Internal::Common::Logger_Exs::GetInstance_Exs()->LogDebug_Exs(message, __FILE__)

#define LOG_INFO_Exs(message) \
    CyberGuardian::Internal::Common::Logger_Exs::GetInstance_Exs()->LogInfo_Exs(message, __FILE__)

#define LOG_WARNING_Exs(message) \
    CyberGuardian::Internal::Common::Logger_Exs::GetInstance_Exs()->LogWarning_Exs(message, __FILE__)

#define LOG_ERROR_Exs(message) \
    CyberGuardian::Internal::Common::Logger_Exs::GetInstance_Exs()->LogError_Exs(message, __FILE__)

#define LOG_CRITICAL_Exs(message) \
    CyberGuardian::Internal::Common::Logger_Exs::GetInstance_Exs()->LogCritical_Exs(message, __FILE__)

} // namespace Common
} // namespace Internal
} // namespace CyberGuardian

#endif
