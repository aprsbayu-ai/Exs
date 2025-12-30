#include "internal/ProfileSelector_Internal.h"
#include "../include/ProfileSelector.h"

namespace CyberGuardian {
namespace Launcher {

ProfileSelector::ProfileSelector() : impl_Exs(std::make_unique<Internal::Launcher::ProfileSelector_Exs>()) {}

ProfileSelector::~ProfileSelector() = default;

bool ProfileSelector::LoadProfiles() {
    return impl_Exs->LoadProfiles_Exs();
}

bool ProfileSelector::CreateProfile(const std::string& name, const std::string& avatar) {
    return impl_Exs->CreateProfile_Exs(name, avatar);
}

bool ProfileSelector::DeleteProfile(const std::string& profileId) {
    return impl_Exs->DeleteProfile_Exs(profileId);
}

bool ProfileSelector::SelectProfile(const std::string& profileId) {
    return impl_Exs->SelectProfile_Exs(profileId);
}

bool ProfileSelector::SaveProfile(const UserProfile& profile) {
    Internal::Launcher::UserProfile_Exs internalProfile;
    internalProfile.profileId_Exs = profile.profileId;
    internalProfile.displayName_Exs = profile.displayName;
    internalProfile.avatarPath_Exs = profile.avatarPath;
    
    return impl_Exs->SaveProfile_Exs(internalProfile);
}

std::vector<UserProfile> ProfileSelector::GetProfiles() const {
    auto internalProfiles = impl_Exs->GetProfiles_Exs();
    std::vector<UserProfile> profiles;
    
    for (const auto& internalProfile : internalProfiles) {
        UserProfile profile;
        profile.profileId = internalProfile.profileId_Exs;
        profile.displayName = internalProfile.displayName_Exs;
        profile.avatarPath = internalProfile.avatarPath_Exs;
        profiles.push_back(profile);
    }
    
    return profiles;
}

UserProfile ProfileSelector::GetSelectedProfile() const {
    auto internalProfile = impl_Exs->GetSelectedProfile_Exs();
    
    UserProfile profile;
    profile.profileId = internalProfile.profileId_Exs;
    profile.displayName = internalProfile.displayName_Exs;
    profile.avatarPath = internalProfile.avatarPath_Exs;
    
    return profile;
}

} // namespace Launcher
} // namespace CyberGuardian
