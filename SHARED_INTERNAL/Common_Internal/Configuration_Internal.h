#pragma once
#ifndef CYBERGUARDIAN_CONFIGURATION_INTERNAL_H
#define CYBERGUARDIAN_CONFIGURATION_INTERNAL_H

#include <string>
#include <unordered_map>
#include <variant>
#include <memory>
#include <mutex>
#include "FileSystem_Internal.h"

namespace CyberGuardian {
namespace Internal {
namespace Common {

class Configuration_Exs {
private:
    static Configuration_Exs* instance_Exs;
    
    struct ConfigValue_Exs {
        std::variant<int, float, bool, std::string> value_Exs;
        std::string description_Exs;
        bool isDefault_Exs;
    };
    
    std::unordered_map<std::string, ConfigValue_Exs> configMap_Exs;
    std::string configFilePath_Exs;
    mutable std::mutex configMutex_Exs;
    bool isDirty_Exs;
    
    Configuration_Exs();
    ~Configuration_Exs();
    
public:
    static Configuration_Exs* GetInstance_Exs();
    static void DestroyInstance_Exs();
    
    bool Initialize_Exs(const std::string& configFile = "config/game_config.json");
    bool Load_Exs();
    bool Save_Exs();
    bool Reload_Exs();
    
    template<typename T>
    T Get_Exs(const std::string& key, const T& defaultValue = T()) const;
    
    template<typename T>
    bool Set_Exs(const std::string& key, const T& value);
    
    bool HasKey_Exs(const std::string& key) const;
    bool RemoveKey_Exs(const std::string& key);
    void Clear_Exs();
    
    std::vector<std::string> GetAllKeys_Exs() const;
    std::string GetDescription_Exs(const std::string& key) const;
    
    void SetDefault_Exs(const std::string& key, const std::string& description = "");
    
private:
    bool ParseConfigFile_Exs();
    bool WriteConfigFile_Exs();
    bool ValidateKey_Exs(const std::string& key) const;
};

template<typename T>
T Configuration_Exs::Get_Exs(const std::string& key, const T& defaultValue) const {
    std::lock_guard<std::mutex> lock(configMutex_Exs);
    
    auto it = configMap_Exs.find(key);
    if (it == configMap_Exs.end()) {
        return defaultValue;
    }
    
    try {
        return std::get<T>(it->second.value_Exs);
    } catch (const std::bad_variant_access&) {
        return defaultValue;
    }
}

template<typename T>
bool Configuration_Exs::Set_Exs(const std::string& key, const T& value) {
    std::lock_guard<std::mutex> lock(configMutex_Exs);
    
    if (!ValidateKey_Exs(key)) {
        return false;
    }
    
    ConfigValue_Exs configValue;
    configValue.value_Exs = value;
    configValue.isDefault_Exs = false;
    
    configMap_Exs[key] = configValue;
    isDirty_Exs = true;
    
    return true;
}

} // namespace Common
} // namespace Internal
} // namespace CyberGuardian

#endif
