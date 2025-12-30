#pragma once
#ifndef CYBERGUARDIAN_UTILITYFUNCTIONS_INTERNAL_H
#define CYBERGUARDIAN_UTILITYFUNCTIONS_INTERNAL_H

#include <string>
#include <vector>
#include <random>
#include <functional>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

namespace CyberGuardian {
namespace Internal {
namespace Common {

class Utilities_Exs {
public:
    static std::string GenerateUUID_Exs();
    static std::string GenerateRandomString_Exs(size_t length, const std::string& charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");
    
    static std::string ToLower_Exs(const std::string& str);
    static std::string ToUpper_Exs(const std::string& str);
    static std::string Trim_Exs(const std::string& str);
    static std::string TrimLeft_Exs(const std::string& str);
    static std::string TrimRight_Exs(const std::string& str);
    
    static bool StartsWith_Exs(const std::string& str, const std::string& prefix);
    static bool EndsWith_Exs(const std::string& str, const std::string& suffix);
    static bool Contains_Exs(const std::string& str, const std::string& substring);
    
    static std::vector<std::string> Split_Exs(const std::string& str, char delimiter);
    static std::vector<std::string> Split_Exs(const std::string& str, const std::string& delimiter);
    static std::string Join_Exs(const std::vector<std::string>& parts, const std::string& delimiter);
    
    static std::string Replace_Exs(const std::string& str, const std::string& from, const std::string& to);
    static std::string ReplaceAll_Exs(const std::string& str, const std::string& from, const std::string& to);
    
    static std::string Format_Exs(const std::string& format, ...);
    static std::string FormatTime_Exs(const std::chrono::system_clock::time_point& time, const std::string& format = "%Y-%m-%d %H:%M:%S");
    static std::string FormatDuration_Exs(uint64_t milliseconds);
    
    static std::string BytesToString_Exs(size_t bytes);
    static size_t StringToBytes_Exs(const std::string& str);
    
    static std::string HexEncode_Exs(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> HexDecode_Exs(const std::string& hex);
    
    static std::string Base64Encode_Exs(const std::vector<uint8_t>& data);
    static std::vector<uint8_t> Base64Decode_Exs(const std::string& base64);
    
    static std::string CalculateMD5_Exs(const std::string& data);
    static std::string CalculateSHA1_Exs(const std::string& data);
    static std::string CalculateSHA256_Exs(const std::string& data);
    
    static bool IsValidEmail_Exs(const std::string& email);
    static bool IsValidURL_Exs(const std::string& url);
    static bool IsValidIPAddress_Exs(const std::string& ip);
    
    static std::string EscapeString_Exs(const std::string& str);
    static std::string UnescapeString_Exs(const std::string& str);
    
    static std::string GetCurrentDateTimeString_Exs();
    static std::string GetCurrentTimeString_Exs();
    static std::string GetCurrentDateString_Exs();
    
    static uint64_t GetCurrentTimestamp_Exs();
    static uint64_t GetCurrentMilliseconds_Exs();
    static uint64_t GetCurrentMicroseconds_Exs();
    
    static double Lerp_Exs(double a, double b, double t);
    static float Lerp_Exs(float a, float b, float t);
    
    static double Clamp_Exs(double value, double min, double max);
    static float Clamp_Exs(float value, float min, float max);
    static int Clamp_Exs(int value, int min, int max);
    
    static double SmoothStep_Exs(double edge0, double edge1, double x);
    static float SmoothStep_Exs(float edge0, float edge1, float x);
    
    static double MapRange_Exs(double value, double fromMin, double fromMax, double toMin, double toMax);
    static float MapRange_Exs(float value, float fromMin, float fromMax, float toMin, float toMax);
    
    static bool IsPowerOfTwo_Exs(uint32_t x);
    static bool IsPowerOfTwo_Exs(uint64_t x);
    
    static uint32_t NextPowerOfTwo_Exs(uint32_t x);
    static uint64_t NextPowerOfTwo_Exs(uint64_t x);
    
    static uint32_t CountBits_Exs(uint32_t x);
    static uint64_t CountBits_Exs(uint64_t x);
    
    static std::string GetFileExtension_Exs(const std::string& filename);
    static std::string GetFileNameWithoutExtension_Exs(const std::string& filename);
    static std::string GetDirectoryPath_Exs(const std::string& path);
    
    static bool CreateDirectories_Exs(const std::string& path);
    static bool DeleteDirectoryRecursive_Exs(const std::string& path);
    
    template<typename T>
    static T Min_Exs(T a, T b) {
        return a < b ? a : b;
    }
    
    template<typename T>
    static T Max_Exs(T a, T b) {
        return a > b ? a : b;
    }
    
    template<typename T>
    static T Abs_Exs(T value) {
        return value < 0 ? -value : value;
    }
    
    template<typename T>
    static T Sign_Exs(T value) {
        if (value > 0) return 1;
        if (value < 0) return -1;
        return 0;
    }
};

class Random_Exs {
private:
    static std::mt19937_64 generator_Exs;
    static bool initialized_Exs;
    
public:
    static void Initialize_Exs(uint64_t seed = std::random_device{}());
    
    static int RandomInt_Exs(int min, int max);
    static float RandomFloat_Exs(float min, float max);
    static double RandomDouble_Exs(double min, double max);
    
    static bool RandomBool_Exs();
    static bool RandomChance_Exs(float probability);
    
    template<typename T>
    static T RandomElement_Exs(const std::vector<T>& container) {
        if (container.empty()) return T();
        return container[RandomInt_Exs(0, static_cast<int>(container.size() - 1))];
    }
    
    template<typename T>
    static void Shuffle_Exs(std::vector<T>& container) {
        std::shuffle(container.begin(), container.end(), generator_Exs);
    }
};

} // namespace Common
} // namespace Internal
} // namespace CyberGuardian

#endif
