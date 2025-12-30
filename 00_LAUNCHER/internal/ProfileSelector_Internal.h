#pragma once
#ifndef CYBERGUARDIAN_PROFILESELECTOR_INTERNAL_H
#define CYBERGUARDIAN_PROFILESELECTOR_INTERNAL_H

#include <string>
#include <vector>
#include <unordered_map>
#include "Launcher_PrivateTypes.h"

namespace CyberGuardian {
namespace Internal {
namespace Launcher {

class ProfileSelector_Exs {
private:
    struct UserProfile_Exs {
        std::string profileId_Exs;
        std::string displayName_Exs;
        std::string avatarPath_Exs;
        std::vector<Achievement_Exs> achievements_Exs;
        PlayerStats_Exs stats_Exs;
        std::chrono::system_clock::time_point lastPlayed_Exs;
    };
    
    std::vector<UserProfile_Exs> availableProfiles_Exs;
    std::string selectedProfileId_Exs;
    std::string profilesDirectory_Exs;
    
public:
    ProfileSelector_Exs();
    
    bool LoadProfiles_Exs();
    bool CreateProfile_Exs(const std::string& name, const std::string& avatar);
    bool DeleteProfile_Exs(const std::string& profileId);
    bool SelectProfile_Exs(const std::string& profileId);
    bool SaveProfile_Exs(const UserProfile_Exs& profile);
    
    std::vector<UserProfile_Exs> GetProfiles_Exs() const;
    UserProfile_Exs GetSelectedProfile_Exs() const;
    
private:
    bool LoadProfileFromFile_Exs(const std::string& filePath);
    bool SaveProfileToFile_Exs(const UserProfile_Exs& profile);
    bool ValidateProfileData_Exs(const UserProfile_Exs& profile);
    bool CreateProfileDirectory_Exs(const std::string& profileId);
};

} // namespace Launcher
} // namespace Internal
} // namespace CyberGuardian

#endif
