#pragma once
#ifndef CYBERGUARDIAN_LAUNCHER_PRIVATETYPES_H
#define CYBERGUARDIAN_LAUNCHER_PRIVATETYPES_H

#include <string>
#include <vector>
#include <chrono>

namespace CyberGuardian {
namespace Internal {
namespace Launcher {

enum class GameState_Exs {
    NOT_INSTALLED_Exs,
    INSTALLING_Exs,
    UPDATING_Exs,
    READY_Exs,
    RUNNING_Exs,
    ERROR_Exs
};

enum class InstallationPhase_Exs {
    INITIALIZING_Exs,
    EXTRACTING_Exs,
    VERIFYING_Exs,
    CREATING_SHORTCUTS_Exs,
    REGISTERING_Exs,
    COMPLETE_Exs
};

enum class UpdatePriority_Exs {
    OPTIONAL_Exs,
    RECOMMENDED_Exs,
    REQUIRED_Exs,
    CRITICAL_Exs
};

enum class SettingType_Exs {
    INTEGER_Exs,
    FLOAT_Exs,
    BOOLEAN_Exs,
    STRING_Exs
};

struct LaunchParams_Exs {
    std::string profileId_Exs;
    std::string resolution_Exs;
    bool fullscreen_Exs;
    std::vector<std::string> launchArguments_Exs;
};

struct GameFile_Exs {
    std::string fileName_Exs;
    std::string filePath_Exs;
    size_t fileSize_Exs;
    std::string checksum_Exs;
};

struct PatchNote_Exs {
    std::string version_Exs;
    std::vector<std::string> changes_Exs;
    std::chrono::system_clock::time_point date_Exs;
};

struct UpdateFile_Exs {
    std::string fileName_Exs;
    std::string downloadUrl_Exs;
    size_t fileSize_Exs;
    std::string targetPath_Exs;
};

struct Achievement_Exs {
    std::string achievementId_Exs;
    std::string achievementName_Exs;
    std::string description_Exs;
    std::chrono::system_clock::time_point unlockedDate_Exs;
};

struct PlayerStats_Exs {
    int totalPlayTime_Exs;
    int missionsCompleted_Exs;
    int enemiesDefeated_Exs;
    int itemsCollected_Exs;
    float highestScore_Exs;
};

} // namespace Launcher
} // namespace Internal
} // namespace CyberGuardian

#endif
