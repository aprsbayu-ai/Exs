#include "internal/SettingsManager_Internal.h"
#include "../include/SettingsManager.h"

namespace CyberGuardian {
namespace Launcher {

SettingsManager::SettingsManager() : impl_Exs(std::make_unique<Internal::Launcher::SettingsManager_Exs>()) {}

SettingsManager::~SettingsManager() = default;

bool SettingsManager::LoadSettings() {
    return impl_Exs->LoadSettings_Exs();
}

bool SettingsManager::SaveSettings() {
    return impl_Exs->SaveSettings_Exs();
}

bool SettingsManager::ResetToDefaults() {
    return impl_Exs->ResetToDefaults_Exs();
}

int SettingsManager::GetInt(const std::string& key) const {
    return impl_Exs->GetSetting_Exs<int>(key);
}

float SettingsManager::GetFloat(const std::string& key) const {
    return impl_Exs->GetSetting_Exs<float>(key);
}

bool SettingsManager::GetBool(const std::string& key) const {
    return impl_Exs->GetSetting_Exs<bool>(key);
}

std::string SettingsManager::GetString(const std::string& key) const {
    return impl_Exs->GetSetting_Exs<std::string>(key);
}

bool SettingsManager::SetInt(const std::string& key, int value) {
    return impl_Exs->SetSetting_Exs<int>(key, value);
}

bool SettingsManager::SetFloat(const std::string& key, float value) {
    return impl_Exs->SetSetting_Exs<float>(key, value);
}

bool SettingsManager::SetBool(const std::string& key, bool value) {
    return impl_Exs->SetSetting_Exs<bool>(key, value);
}

bool SettingsManager::SetString(const std::string& key, const std::string& value) {
    return impl_Exs->SetSetting_Exs<std::string>(key, value);
}

SettingsMap SettingsManager::GetAllSettings() const {
    auto internalSettings = impl_Exs->GetAllSettings_Exs();
    SettingsMap settings;
    
    for (const auto& [key, internalValue] : internalSettings) {
        SettingValue value;
        
        switch (internalValue.type_Exs) {
            case Internal::Launcher::SettingType_Exs::INTEGER_Exs:
                value.type = SettingType::INTEGER;
                value.intValue = std::get<int>(internalValue.value_Exs);
                break;
            case Internal::Launcher::SettingType_Exs::FLOAT_Exs:
                value.type = SettingType::FLOAT;
                value.floatValue = std::get<float>(internalValue.value_Exs);
                break;
            case Internal::Launcher::SettingType_Exs::BOOLEAN_Exs:
                value.type = SettingType::BOOLEAN;
                value.boolValue = std::get<bool>(internalValue.value_Exs);
                break;
            case Internal::Launcher::SettingType_Exs::STRING_Exs:
                value.type = SettingType::STRING;
                value.stringValue = std::get<std::string>(internalValue.value_Exs);
                break;
        }
        
        settings[key] = value;
    }
    
    return settings;
}

} // namespace Launcher
} // namespace CyberGuardian
