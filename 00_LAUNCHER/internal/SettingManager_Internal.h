#pragma once
#ifndef CYBERGUARDIAN_SETTINGSMANAGER_INTERNAL_H
#define CYBERGUARDIAN_SETTINGSMANAGER_INTERNAL_H

#include <string>
#include <unordered_map>
#include <variant>
#include "Launcher_PrivateTypes.h"

namespace CyberGuardian {
namespace Internal {
namespace Launcher {

class SettingsManager_Exs {
private:
    struct SettingValue_Exs {
        std::variant<int, float, bool, std::string> value_Exs;
        SettingType_Exs type_Exs;
        std::string description_Exs;
        std::string category_Exs;
    };
    
    std::unordered_map<std::string, SettingValue_Exs> settings_Exs;
    std::string settingsFilePath_Exs;
    std::string defaultSettingsPath_Exs;
    
public:
    SettingsManager_Exs();
    
    bool LoadSettings_Exs();
    bool SaveSettings_Exs();
    bool ResetToDefaults_Exs();
    
    template<typename T>
    T GetSetting_Exs(const std::string& key) const;
    
    template<typename T>
    bool SetSetting_Exs(const std::string& key, const T& value);
    
    std::unordered_map<std::string, SettingValue_Exs> GetAllSettings_Exs() const;
    
private:
    bool ParseSettingsFile_Exs(const std::string& filePath);
    bool WriteSettingsFile_Exs(const std::string& filePath);
    bool ValidateSettingValue_Exs(const std::string& key, const SettingValue_Exs& value);
    bool ApplySettingsChanges_Exs();
};

} // namespace Launcher
} // namespace Internal
} // namespace CyberGuardian

#endif
