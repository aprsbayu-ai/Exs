#include "GameLauncher_Internal.h"
#include "../include/GameLauncher.h"
#include "../../SHARED_INTERNAL/Common_Internal/MemoryManager_Internal.h"

namespace CyberGuardian {
namespace Launcher {

GameLauncher::GameLauncher() : impl_Exs(std::make_unique<Internal::Launcher::GameLauncher_Exs>()) {}

GameLauncher::~GameLauncher() = default;

bool GameLauncher::Initialize(const std::string& configPath) {
    return impl_Exs->InitializeLauncher_Exs(configPath);
}

bool GameLauncher::LaunchGame(const LaunchParameters& params) {
    Internal::Launcher::LaunchParams_Exs internalParams;
    internalParams.profileId_Exs = params.profileId;
    internalParams.resolution_Exs = params.resolution;
    internalParams.fullscreen_Exs = params.fullscreen;
    internalParams.launchArguments_Exs = params.launchArguments;
    
    return impl_Exs->LaunchGame_Exs(internalParams);
}

bool GameLauncher::ValidateInstallation() {
    return impl_Exs->ValidateInstallation_Exs();
}

void GameLauncher::Shutdown() {
    impl_Exs->ShutdownLauncher_Exs();
}

} // namespace Launcher
} // namespace CyberGuardian
