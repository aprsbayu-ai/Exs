#pragma once
#ifndef CYBERGUARDIAN_UPDATECHECKER_INTERNAL_H
#define CYBERGUARDIAN_UPDATECHECKER_INTERNAL_H

#include <string>
#include <vector>
#include <chrono>
#include "Launcher_PrivateTypes.h"

namespace CyberGuardian {
namespace Internal {
namespace Launcher {

class UpdateChecker_Exs {
private:
    struct UpdateInfo_Exs {
        std::string latestVersion_Exs;
        std::vector<PatchNote_Exs> patchNotes_Exs;
        std::vector<UpdateFile_Exs> updateFiles_Exs;
        std::chrono::system_clock::time_point releaseDate_Exs;
        UpdatePriority_Exs priority_Exs;
        size_t totalSize_Exs;
    };
    
    UpdateInfo_Exs updateInfo_Exs;
    std::string currentVersion_Exs;
    std::string updateServerUrl_Exs;
    
public:
    UpdateChecker_Exs();
    
    bool CheckForUpdates_Exs();
    bool DownloadUpdate_Exs();
    bool ApplyUpdate_Exs();
    bool VerifyUpdate_Exs();
    
    UpdateInfo_Exs GetUpdateInfo_Exs() const;
    bool IsUpdateAvailable_Exs() const;
    bool IsUpdateRequired_Exs() const;
    
private:
    bool FetchUpdateManifest_Exs();
    bool CompareVersions_Exs(const std::string& v1, const std::string& v2);
    bool DownloadUpdateFile_Exs(const UpdateFile_Exs& file);
    bool ApplyPatch_Exs(const std::string& patchPath);
};

} // namespace Launcher
} // namespace Internal
} // namespace CyberGuardian

#endif
