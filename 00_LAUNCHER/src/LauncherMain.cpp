#include "../include/GameLauncher.h"
#include "../include/InstallManager.h"
#include "../include/UpdateChecker.h"
#include "../include/ProfileSelector.h"
#include "../include/SettingsManager.h"
#include "../../SHARED_INTERNAL/Common_Internal/Logging_Internal.h"

int main(int argc, char* argv[]) {
    // Initialize logging
    CyberGuardian::Internal::Logger_Exs* logger = CyberGuardian::Internal::Logger_Exs::GetInstance_Exs();
    logger->Initialize_Exs("CyberGuardianLauncher.log");
    
    logger->LogInfo_Exs("CyberGuardian Launcher Starting...");
    logger->LogInfo_Exs("Version: 1.0.0");
    logger->LogInfo_Exs("Build Date: " __DATE__ " " __TIME__);
    
    // Initialize systems
    CyberGuardian::Launcher::SettingsManager settingsManager;
    if (!settingsManager.LoadSettings()) {
        logger->LogError_Exs("Failed to load settings");
        return 1;
    }
    
    CyberGuardian::Launcher::ProfileSelector profileSelector;
    if (!profileSelector.LoadProfiles()) {
        logger->LogWarning_Exs("No profiles found or failed to load");
    }
    
    CyberGuardian::Launcher::UpdateChecker updateChecker;
    if (updateChecker.CheckForUpdates()) {
        logger->LogInfo_Exs("Update available");
        if (updateChecker.IsUpdateRequired()) {
            logger->LogInfo_Exs("Required update found, downloading...");
            if (!updateChecker.DownloadUpdate()) {
                logger->LogError_Exs("Failed to download update");
                return 1;
            }
            if (!updateChecker.ApplyUpdate()) {
                logger->LogError_Exs("Failed to apply update");
                return 1;
            }
            if (!updateChecker.VerifyUpdate()) {
                logger->LogError_Exs("Update verification failed");
                return 1;
            }
            logger->LogInfo_Exs("Update applied successfully");
        }
    }
    
    CyberGuardian::Launcher::GameLauncher gameLauncher;
    if (!gameLauncher.Initialize("config/launcher_config.json")) {
        logger->LogError_Exs("Failed to initialize game launcher");
        return 1;
    }
    
    if (!gameLauncher.ValidateInstallation()) {
        logger->LogError_Exs("Game installation validation failed");
        
        // Offer to reinstall
        CyberGuardian::Launcher::InstallManager installManager;
        logger->LogInfo_Exs("Starting repair installation...");
        
        if (!installManager.BeginInstall("C:/Program Files/CyberGuardian")) {
            logger->LogError_Exs("Failed to begin repair installation");
            return 1;
        }
        
        // In real implementation, would have list of game files
        std::vector<CyberGuardian::Launcher::GameFile> gameFiles;
        // Populate gameFiles...
        
        if (!installManager.InstallFiles(gameFiles)) {
            logger->LogError_Exs("Failed to install game files");
            return 1;
        }
        
        if (!installManager.VerifyInstall()) {
            logger->LogError_Exs("Installation verification failed");
            return 1;
        }
        
        if (!installManager.CreateShortcuts()) {
            logger->LogWarning_Exs("Failed to create shortcuts");
        }
        
        if (!installManager.RegisterWithSystem()) {
            logger->LogWarning_Exs("Failed to register with system");
        }
        
        logger->LogInfo_Exs("Repair installation completed");
    }
    
    // Launch game with selected profile
    CyberGuardian::Launcher::LaunchParameters params;
    params.profileId = profileSelector.GetSelectedProfile().profileId;
    params.resolution = settingsManager.GetString("resolution");
    params.fullscreen = settingsManager.GetBool("fullscreen");
    
    if (!gameLauncher.LaunchGame(params)) {
        logger->LogError_Exs("Failed to launch game");
        return 1;
    }
    
    // Wait for game to close
    gameLauncher.Shutdown();
    
    // Save settings
    if (!settingsManager.SaveSettings()) {
        logger->LogWarning_Exs("Failed to save settings");
    }
    
    logger->LogInfo_Exs("CyberGuardian Launcher Shutting Down");
    logger->Shutdown_Exs();
    
    return 0;
}
