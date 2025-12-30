#include "internal/SettingsManager_Internal.h"
#include "../../SHARED_INTERNAL/Common_Internal/FileSystem_Internal.h"
#include "../../SHARED_INTERNAL/Common_Internal/Logging_Internal.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace CyberGuardian {
namespace Internal {
namespace Launcher {

SettingsManager_Exs::SettingsManager_Exs() {
    settingsFilePath_Exs = "Settings/launcher_settings.json";
    defaultSettingsPath_Exs = "Settings/default_settings.json";
}

bool SettingsManager_Exs::LoadSettings_Exs() {
    Logger_Exs* logger = Logger_Exs::GetInstance_Exs();
    FileSystem_Exs* fs = FileSystem_Exs::GetInstance_Exs();
    
    logger->LogInfo_Exs("Loading settings...");
    
    // First try to load user settings
    if (fs->FileExists_Exs(settingsFilePath_Exs)) {
        if (!ParseSettingsFile_Exs(settingsFilePath_Exs)) {
            logger->LogWarning_Exs("Failed to parse user settings, loading defaults");
            if (!ParseSettingsFile_Exs(defaultSettingsPath_Exs)) {
                logger->LogError_Exs("Failed to load default settings");
                return false;
            }
        }
    } else {
        // Load default settings
        logger->LogInfo_Exs("User settings not found, loading defaults");
        if (!ParseSettingsFile_Exs(defaultSettingsPath_Exs)) {
            logger->LogError_Exs("Failed to load default settings");
            return false;
        }
        
        // Save default settings as user settings
        if (!WriteSettingsFile_Exs(settingsFilePath_Exs)) {
            logger->LogWarning_Exs("Failed to save default settings as user settings");
        }
    }
    
    logger->LogInfo_Exs("Settings loaded successfully");
    return true;
}

bool SettingsManager_Exs::SaveSettings_Exs() {
    Logger_Exs* logger = Logger_Exs::GetInstance_Exs();
    
    logger->LogInfo_Exs("Saving settings...");
    
    if (!WriteSettingsFile_Exs(settingsFilePath_Exs)) {
        logger->LogError_Exs("Failed to save settings");
        return false;
    }
    
    if (!ApplySettingsChanges_Exs()) {
        logger->LogWarning_Exs("Failed to apply some setting changes");
    }
    
    logger->LogInfo_Exs("Settings saved successfully");
    return true;
}

bool SettingsManager_Exs::ResetToDefaults_Exs() {
    Logger_Exs* logger = Logger_Exs::GetInstance_Exs();
    
    logger->LogInfo_Exs("Resetting settings to defaults...");
    
    // Clear current settings
    settings_Exs.clear();
    
    // Load default settings
    if (!ParseSettingsFile_Exs(defaultSettingsPath_Exs)) {
        logger->LogError_Exs("Failed to load default settings");
        return false;
    }
    
    // Save as user settings
    if (!WriteSettingsFile_Exs(settingsFilePath_Exs)) {
        logger->LogError_Exs("Failed to save default settings");
        return false;
    }
    
    logger->LogInfo_Exs("Settings reset to defaults");
    return true;
}

template<typename T>
T SettingsManager_Exs::GetSetting_Exs(const std::string& key) const {
    auto it = settings_Exs.find(key);
    if (it == settings_Exs.end()) {
        throw std::runtime_error("Setting not found: " + key);
    }
    
    try {
        return std::get<T>(it->second.value_Exs);
    } catch (const std::bad_variant_access&) {
        throw std::runtime_error("Setting type mismatch for: " + key);
    }
}

template<typename T>
bool SettingsManager_Exs::SetSetting_Exs(const std::string& key, const T& value) {
    auto it = settings_Exs.find(key);
    if (it == settings_Exs.end()) {
        return false;
    }
    
    SettingValue_Exs newValue = it->second;
    newValue.value_Exs = value;
    
    if (!ValidateSettingValue_Exs(key, newValue)) {
        return false;
    }
    
    it->second = newValue;
    return true;
}

std::unordered_map<std::string, SettingValue_Exs> SettingsManager_Exs::GetAllSettings_Exs() const {
    return settings_Exs;
}

bool SettingsManager_Exs::ParseSettingsFile_Exs(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    
    try {
        nlohmann::json json;
        file >> json;
        
        for (const auto& [key, value] : json.items()) {
            SettingValue_Exs settingValue;
            
            if (value.is_number_integer()) {
                settingValue.type_Exs = SettingType_Exs::INTEGER_Exs;
                settingValue.value_Exs = value.get<int>();
            } else if (value.is_number_float()) {
                settingValue.type_Exs = SettingType_Exs::FLOAT_Exs;
                settingValue.value_Exs = value.get<float>();
            } else if (value.is_boolean()) {
                settingValue.type_Exs = SettingType_Exs::BOOLEAN_Exs;
                settingValue.value_Exs = value.get<bool>();
            } else if (value.is_string()) {
                settingValue.type_Exs = SettingType_Exs::STRING_Exs;
                settingValue.value_Exs = value.get<std::string>();
            }
            
            // Parse category and description if available
            if (json.contains(key + "_category")) {
                settingValue.category_Exs = json[key + "_category"];
            }
            
            if (json.contains(key + "_description")) {
                settingValue.description_Exs = json[key + "_description"];
            }
            
            settings_Exs[key] = settingValue;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        return false;
    }
}

bool SettingsManager_Exs::WriteSettingsFile_Exs(const std::string& filePath) {
    nlohmann::json json;
    
    for (const auto& [key, value] : settings_Exs) {
        switch (value.type_Exs) {
            case SettingType_Exs::INTEGER_Exs:
                json[key] = std::get<int>(value.value_Exs);
                break;
            case SettingType_Exs::FLOAT_Exs:
                json[key] = std::get<float>(value.value_Exs);
                break;
            case SettingType_Exs::BOOLEAN_Exs:
                json[key] = std::get<bool>(value.value_Exs);
                break;
            case SettingType_Exs::STRING_Exs:
                json[key] = std::get<std::string>(value.value_Exs);
                break;
        }
        
        // Save category and description
        if (!value.category_Exs.empty()) {
            json[key + "_category"] = value.category_Exs;
        }
        
        if (!value.description_Exs.empty()) {
            json[key + "_description"] = value.description_Exs;
        }
    }
    
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    
    file << json.dump(4);
    return true;
}

bool SettingsManager_Exs::ValidateSettingValue_Exs(const std::string& key, const SettingValue_Exs& value) {
    // Validate based on setting type
    switch (value.type_Exs) {
        case SettingType_Exs::INTEGER_Exs: {
            int intValue = std::get<int>(value.value_Exs);
            
            // Validate resolution settings
            if (key.find("resolution") != std::string::npos) {
                return intValue >= 480 && intValue <= 4320;
            }
            
            // Validate volume settings
            if (key.find("volume") != std::string::npos) {
                return intValue >= 0 && intValue <= 100;
            }
            
            break;
        }
        
        case SettingType_Exs::FLOAT_Exs: {
            float floatValue = std::get<float>(value.value_Exs);
            
            // Validate gamma settings
            if (key == "gamma") {
                return floatValue >= 0.5f && floatValue <= 3.0f;
            }
            
            break;
        }
        
        case SettingType_Exs::STRING_Exs: {
            std::string stringValue = std::get<std::string>(value.value_Exs);
            
            // Validate language settings
            if (key == "language") {
                std::vector<std::string> validLanguages = {
                    "en", "es", "fr", "de", "ja", "ko", "zh"
                };
                return std::find(validLanguages.begin(), validLanguages.end(), stringValue) != validLanguages.end();
            }
            
            break;
        }
        
        default:
            break;
    }
    
    return true;
}

bool SettingsManager_Exs::ApplySettingsChanges_Exs() {
    bool success = true;
    
    // Apply graphics settings
    if (settings_Exs.find("resolution_width") != settings_Exs.end() &&
        settings_Exs.find("resolution_height") != settings_Exs.end()) {
        int width = std::get<int>(settings_Exs["resolution_width"].value_Exs);
        int height = std::get<int>(settings_Exs["resolution_height"].value_Exs);
        // Apply resolution change
    }
    
    // Apply audio settings
    if (settings_Exs.find("master_volume") != settings_Exs.end()) {
        int volume = std::get<int>(settings_Exs["master_volume"].value_Exs);
        // Apply volume change
    }
    
    return success;
}

// Explicit template instantiations
template int SettingsManager_Exs::GetSetting_Exs<int>(const std::string& key) const;
template float SettingsManager_Exs::GetSetting_Exs<float>(const std::string& key) const;
template bool SettingsManager_Exs::GetSetting_Exs<bool>(const std::string& key) const;
template std::string SettingsManager_Exs::GetSetting_Exs<std::string>(const std::string& key) const;

template bool SettingsManager_Exs::SetSetting_Exs<int>(const std::string& key, const int& value);
template bool SettingsManager_Exs::SetSetting_Exs<float>(const std::string& key, const float& value);
template bool SettingsManager_Exs::SetSetting_Exs<bool>(const std::string& key, const bool& value);
template bool SettingsManager_Exs::SetSetting_Exs<std::string>(const std::string& key, const std::string& value);

} // namespace Launcher
} // namespace Internal
} // namespace CyberGuardian
