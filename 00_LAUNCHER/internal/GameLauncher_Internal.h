#pragma once
#ifndef CYBERGUARDIAN_GAMELAUNCHER_INTERNAL_H
#define CYBERGUARDIAN_GAMELAUNCHER_INTERNAL_H

#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <mutex>
#include <functional>
#include "Launcher_PrivateTypes.h"
#include "../../SHARED_INTERNAL/Common_Internal/Logging_Internal.h"

namespace CyberGuardian {
namespace Internal {
namespace Launcher {

class GameLauncher_Exs {
private:
    struct GameLauncherData_Exs {
        std::string installationPath_Exs;
        std::string versionString_Exs;
        std::vector<std::string> installedModules_Exs;
        GameState_Exs currentState_Exs;
        std::atomic<bool> isRunning_Exs;
        std::mutex launcherMutex_Exs;
    };
    
    std::unique_ptr<GameLauncherData_Exs> data_Exs;
    Logger_Exs* logger_Exs;
    
public:
    GameLauncher_Exs();
    ~GameLauncher_Exs();
    
    bool InitializeLauncher_Exs(const std::string& configPath);
    bool LaunchGame_Exs(const LaunchParams_Exs& params);
    bool ValidateInstallation_Exs();
    void ShutdownLauncher_Exs();
    
private:
    bool LoadConfiguration_Exs(const std::string& configPath);
    bool CheckSystemRequirements_Exs();
    bool InitializeGameWindow_Exs();
    bool LoadGameAssets_Exs();
    void CleanupResources_Exs();
};

} // namespace Launcher
} // namespace Internal
} // namespace CyberGuardian

#endif
