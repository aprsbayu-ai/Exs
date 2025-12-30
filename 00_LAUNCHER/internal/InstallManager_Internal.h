#pragma once
#ifndef CYBERGUARDIAN_INSTALLMANAGER_INTERNAL_H
#define CYBERGUARDIAN_INSTALLMANAGER_INTERNAL_H

#include <string>
#include <vector>
#include <filesystem>
#include "Launcher_PrivateTypes.h"

namespace CyberGuardian {
namespace Internal {
namespace Launcher {

class InstallManager_Exs {
private:
    struct InstallationProgress_Exs {
        float overallProgress_Exs;
        float currentFileProgress_Exs;
        std::string currentFileName_Exs;
        InstallationPhase_Exs currentPhase_Exs;
        std::vector<std::string> installedFiles_Exs;
    };
    
    InstallationProgress_Exs progress_Exs;
    std::string targetDirectory_Exs;
    
public:
    InstallManager_Exs();
    
    bool BeginInstallation_Exs(const std::string& installPath);
    bool InstallGameFiles_Exs(const std::vector<GameFile_Exs>& files);
    bool VerifyInstallation_Exs();
    bool CreateShortcuts_Exs();
    bool RegisterGame_Exs();
    
    InstallationProgress_Exs GetProgress_Exs() const;
    bool IsInstallationComplete_Exs() const;
    
private:
    bool ExtractArchive_Exs(const std::string& archivePath);
    bool CreateDirectoryStructure_Exs();
    bool WriteRegistryEntries_Exs();
    bool ValidateFileChecksums_Exs();
};

} // namespace Launcher
} // namespace Internal
} // namespace CyberGuardian

#endif
