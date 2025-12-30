#include "internal/InstallManager_Internal.h"
#include "../include/InstallManager.h"

namespace CyberGuardian {
namespace Launcher {

InstallManager::InstallManager() : impl_Exs(std::make_unique<Internal::Launcher::InstallManager_Exs>()) {}

InstallManager::~InstallManager() = default;

bool InstallManager::BeginInstall(const std::string& installPath) {
    return impl_Exs->BeginInstallation_Exs(installPath);
}

bool InstallManager::InstallFiles(const std::vector<GameFile>& files) {
    std::vector<Internal::Launcher::GameFile_Exs> internalFiles;
    
    for (const auto& file : files) {
        Internal::Launcher::GameFile_Exs internalFile;
        internalFile.fileName_Exs = file.fileName;
        internalFile.filePath_Exs = file.filePath;
        internalFile.fileSize_Exs = file.fileSize;
        internalFile.checksum_Exs = file.checksum;
        internalFiles.push_back(internalFile);
    }
    
    return impl_Exs->InstallGameFiles_Exs(internalFiles);
}

bool InstallManager::VerifyInstall() {
    return impl_Exs->VerifyInstallation_Exs();
}

bool InstallManager::CreateShortcuts() {
    return impl_Exs->CreateShortcuts_Exs();
}

bool InstallManager::RegisterWithSystem() {
    return impl_Exs->RegisterGame_Exs();
}

InstallProgress InstallManager::GetProgress() const {
    auto internalProgress = impl_Exs->GetProgress_Exs();
    
    InstallProgress progress;
    progress.overallProgress = internalProgress.overallProgress_Exs;
    progress.currentFileProgress = internalProgress.currentFileProgress_Exs;
    progress.currentFileName = internalProgress.currentFileName_Exs;
    
    return progress;
}

bool InstallManager::IsComplete() const {
    return impl_Exs->IsInstallationComplete_Exs();
}

} // namespace Launcher
} // namespace CyberGuardian
