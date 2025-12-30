#include "internal/ProfileSelector_Internal.h"
#include "../../SHARED_INTERNAL/Common_Internal/FileSystem_Internal.h"
#include "../../SHARED_INTERNAL/Common_Internal/Logging_Internal.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace CyberGuardian {
namespace Internal {
namespace Launcher {

ProfileSelector_Exs::ProfileSelector_Exs() {
    profilesDirectory_Exs = "Profiles";
    selectedProfileId_Exs = "";
}

bool ProfileSelector_Exs::LoadProfiles_Exs() {
    Logger_Exs* logger = Logger_Exs::GetInstance_Exs();
    FileSystem_Exs* fs = FileSystem_Exs::GetInstance_Exs();
    
    logger->LogInfo_Exs("Loading profiles from: " + profilesDirectory_Exs);
    
    if (!fs->DirectoryExists_Exs(profilesDirectory_Exs)) {
        logger->LogWarning_Exs("Profiles directory not found, creating...");
        if (!fs->CreateDirectory_Exs(profilesDirectory_Exs)) {
            logger->LogError_Exs("Failed to create profiles directory");
            return false;
        }
    }
    
    auto files = fs->ListFiles_Exs(profilesDirectory_Exs, "*.profile");
    
    for (const auto& file : files) {
        std::string filePath = profilesDirectory_Exs + "/" + file;
        if (!LoadProfileFromFile_Exs(filePath)) {
            logger->LogWarning_Exs("Failed to load profile: " + file);
        }
    }
    
    logger->LogInfo_Exs("Loaded " + std::to_string(availableProfiles_Exs.size()) + " profiles");
    return true;
}

bool ProfileSelector_Exs::CreateProfile_Exs(const std::string& name, const std::string& avatar) {
    Logger_Exs* logger = Logger_Exs::GetInstance_Exs();
    
    // Generate unique profile ID
    std::string profileId = "profile_" + std::to_string(std::time(nullptr));
    
    UserProfile_Exs newProfile;
    newProfile.profileId_Exs = profileId;
    newProfile.displayName_Exs = name;
    newProfile.avatarPath_Exs = avatar;
    newProfile.lastPlayed_Exs = std::chrono::system_clock::now();
    
    if (!ValidateProfileData_Exs(newProfile)) {
        logger->LogError_Exs("Invalid profile data");
        return false;
    }
    
    if (!CreateProfileDirectory_Exs(profileId)) {
        logger->LogError_Exs("Failed to create profile directory");
        return false;
    }
    
    if (!SaveProfileToFile_Exs(newProfile)) {
        logger->LogError_Exs("Failed to save profile");
        return false;
    }
    
    availableProfiles_Exs.push_back(newProfile);
    logger->LogInfo_Exs("Created new profile: " + name);
    
    return true;
}

bool ProfileSelector_Exs::DeleteProfile_Exs(const std::string& profileId) {
    Logger_Exs* logger = Logger_Exs::GetInstance_Exs();
    FileSystem_Exs* fs = FileSystem_Exs::GetInstance_Exs();
    
    auto it = std::remove_if(availableProfiles_Exs.begin(), availableProfiles_Exs.end(),
        [&profileId](const UserProfile_Exs& profile) {
            return profile.profileId_Exs == profileId;
        });
    
    if (it != availableProfiles_Exs.end()) {
        availableProfiles_Exs.erase(it, availableProfiles_Exs.end());
        
        std::string profileFile = profilesDirectory_Exs + "/" + profileId + ".profile";
        if (fs->FileExists_Exs(profileFile)) {
            if (!fs->DeleteFile_Exs(profileFile)) {
                logger->LogWarning_Exs("Failed to delete profile file: " + profileFile);
            }
        }
        
        std::string profileDir = profilesDirectory_Exs + "/" + profileId;
        if (fs->DirectoryExists_Exs(profileDir)) {
            if (!fs->DeleteDirectory_Exs(profileDir)) {
                logger->LogWarning_Exs("Failed to delete profile directory: " + profileDir);
            }
        }
        
        logger->LogInfo_Exs("Deleted profile: " + profileId);
        return true;
    }
    
    logger->LogWarning_Exs("Profile not found: " + profileId);
    return false;
}

bool ProfileSelector_Exs::SelectProfile_Exs(const std::string& profileId) {
    for (const auto& profile : availableProfiles_Exs) {
        if (profile.profileId_Exs == profileId) {
            selectedProfileId_Exs = profileId;
            Logger_Exs::GetInstance_Exs()->LogInfo_Exs("Selected profile: " + profile.displayName_Exs);
            return true;
        }
    }
    
    Logger_Exs::GetInstance_Exs()->LogError_Exs("Profile not found: " + profileId);
    return false;
}

bool ProfileSelector_Exs::SaveProfile_Exs(const UserProfile_Exs& profile) {
    if (!ValidateProfileData_Exs(profile)) {
        return false;
    }
    
    // Update existing profile or add new one
    for (auto& existingProfile : availableProfiles_Exs) {
        if (existingProfile.profileId_Exs == profile.profileId_Exs) {
            existingProfile = profile;
            existingProfile.lastPlayed_Exs = std::chrono::system_clock::now();
            return SaveProfileToFile_Exs(existingProfile);
        }
    }
    
    // Profile doesn't exist, add it
    availableProfiles_Exs.push_back(profile);
    return SaveProfileToFile_Exs(profile);
}

std::vector<UserProfile_Exs> ProfileSelector_Exs::GetProfiles_Exs() const {
    return availableProfiles_Exs;
}

UserProfile_Exs ProfileSelector_Exs::GetSelectedProfile_Exs() const {
    for (const auto& profile : availableProfiles_Exs) {
        if (profile.profileId_Exs == selectedProfileId_Exs) {
            return profile;
        }
    }
    
    // Return empty profile if none selected
    return UserProfile_Exs();
}

bool ProfileSelector_Exs::LoadProfileFromFile_Exs(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    
    try {
        nlohmann::json json;
        file >> json;
        
        UserProfile_Exs profile;
        profile.profileId_Exs = json["profileId"];
        profile.displayName_Exs = json["displayName"];
        profile.avatarPath_Exs = json["avatarPath"];
        
        // Parse achievements
        for (const auto& achievement : json["achievements"]) {
            Achievement_Exs ach;
            ach.achievementId_Exs = achievement["id"];
            ach.achievementName_Exs = achievement["name"];
            ach.description_Exs = achievement["description"];
            profile.achievements_Exs.push_back(ach);
        }
        
        // Parse stats
        auto stats = json["stats"];
        profile.stats_Exs.totalPlayTime_Exs = stats["totalPlayTime"];
        profile.stats_Exs.missionsCompleted_Exs = stats["missionsCompleted"];
        profile.stats_Exs.enemiesDefeated_Exs = stats["enemiesDefeated"];
        profile.stats_Exs.itemsCollected_Exs = stats["itemsCollected"];
        profile.stats_Exs.highestScore_Exs = stats["highestScore"];
        
        availableProfiles_Exs.push_back(profile);
        return true;
        
    } catch (const std::exception& e) {
        return false;
    }
}

bool ProfileSelector_Exs::SaveProfileToFile_Exs(const UserProfile_Exs& profile) {
    std::string filePath = profilesDirectory_Exs + "/" + profile.profileId_Exs + ".profile";
    
    nlohmann::json json;
    json["profileId"] = profile.profileId_Exs;
    json["displayName"] = profile.displayName_Exs;
    json["avatarPath"] = profile.avatarPath_Exs;
    
    // Save achievements
    nlohmann::json achievementsJson = nlohmann::json::array();
    for (const auto& achievement : profile.achievements_Exs) {
        nlohmann::json achJson;
        achJson["id"] = achievement.achievementId_Exs;
        achJson["name"] = achievement.achievementName_Exs;
        achJson["description"] = achievement.description_Exs;
        achievementsJson.push_back(achJson);
    }
    json["achievements"] = achievementsJson;
    
    // Save stats
    nlohmann::json statsJson;
    statsJson["totalPlayTime"] = profile.stats_Exs.totalPlayTime_Exs;
    statsJson["missionsCompleted"] = profile.stats_Exs.missionsCompleted_Exs;
    statsJson["enemiesDefeated"] = profile.stats_Exs.enemiesDefeated_Exs;
    statsJson["itemsCollected"] = profile.stats_Exs.itemsCollected_Exs;
    statsJson["highestScore"] = profile.stats_Exs.highestScore_Exs;
    json["stats"] = statsJson;
    
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    
    file << json.dump(4);
    return true;
}

bool ProfileSelector_Exs::ValidateProfileData_Exs(const UserProfile_Exs& profile) {
    if (profile.profileId_Exs.empty()) {
        return false;
    }
    
    if (profile.displayName_Exs.empty()) {
        return false;
    }
    
    // Check for duplicate profile IDs
    for (const auto& existingProfile : availableProfiles_Exs) {
        if (existingProfile.profileId_Exs == profile.profileId_Exs) {
            return false;
        }
    }
    
    return true;
}

bool ProfileSelector_Exs::CreateProfileDirectory_Exs(const std::string& profileId) {
    FileSystem_Exs* fs = FileSystem_Exs::GetInstance_Exs();
    
    std::string profileDir = profilesDirectory_Exs + "/" + profileId;
    
    if (!fs->CreateDirectory_Exs(profileDir)) {
        return false;
    }
    
    // Create subdirectories
    std::vector<std::string> subdirs = {
        profileDir + "/Saves",
        profileDir + "/Screenshots",
        profileDir + "/Config"
    };
    
    for (const auto& subdir : subdirs) {
        if (!fs->CreateDirectory_Exs(subdir)) {
            return false;
        }
    }
    
    return true;
}

} // namespace Launcher
} // namespace Internal
} // namespace CyberGuardian
