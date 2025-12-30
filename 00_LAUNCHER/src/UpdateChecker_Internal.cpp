#include "internal/UpdateChecker_Internal.h"
#include "../../SHARED_INTERNAL/Common_Internal/FileSystem_Internal.h"
#include "../../SHARED_INTERNAL/Common_Internal/Logging_Internal.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>

namespace CyberGuardian {
namespace Internal {
namespace Launcher {

UpdateChecker_Exs::UpdateChecker_Exs() {
    currentVersion_Exs = "1.0.0";
    updateServerUrl_Exs = "https://updates.cyberguardian.game/api/v1";
}

bool UpdateChecker_Exs::CheckForUpdates_Exs() {
    Logger_Exs* logger = Logger_Exs::GetInstance_Exs();
    logger->LogInfo_Exs("Checking for updates...");
    
    if (!FetchUpdateManifest_Exs()) {
        logger->LogError_Exs("Failed to fetch update manifest");
        return false;
    }
    
    if (CompareVersions_Exs(currentVersion_Exs, updateInfo_Exs.latestVersion_Exs)) {
        logger->LogInfo_Exs("Update available: " + updateInfo_Exs.latestVersion_Exs);
        return true;
    }
    
    logger->LogInfo_Exs("No updates available");
    return false;
}

bool UpdateChecker_Exs::DownloadUpdate_Exs() {
    Logger_Exs* logger = Logger_Exs::GetInstance_Exs();
    logger->LogInfo_Exs("Downloading update...");
    
    for (const auto& file : updateInfo_Exs.updateFiles_Exs) {
        logger->LogInfo_Exs("Downloading: " + file.fileName_Exs);
        
        if (!DownloadUpdateFile_Exs(file)) {
            logger->LogError_Exs("Failed to download: " + file.fileName_Exs);
            return false;
        }
    }
    
    logger->LogInfo_Exs("Download complete");
    return true;
}

bool UpdateChecker_Exs::ApplyUpdate_Exs() {
    Logger_Exs* logger = Logger_Exs::GetInstance_Exs();
    logger->LogInfo_Exs("Applying update...");
    
    for (const auto& file : updateInfo_Exs.updateFiles_Exs) {
        logger->LogInfo_Exs("Applying: " + file.fileName_Exs);
        
        if (!ApplyPatch_Exs(file.targetPath_Exs)) {
            logger->LogError_Exs("Failed to apply: " + file.fileName_Exs);
            return false;
        }
    }
    
    logger->LogInfo_Exs("Update applied successfully");
    return true;
}

bool UpdateChecker_Exs::VerifyUpdate_Exs() {
    Logger_Exs* logger = Logger_Exs::GetInstance_Exs();
    logger->LogInfo_Exs("Verifying update...");
    
    FileSystem_Exs* fs = FileSystem_Exs::GetInstance_Exs();
    
    for (const auto& file : updateInfo_Exs.updateFiles_Exs) {
        if (!fs->FileExists_Exs(file.targetPath_Exs)) {
            logger->LogError_Exs("Updated file not found: " + file.targetPath_Exs);
            return false;
        }
        
        // Verify file size
        size_t fileSize = fs->GetFileSize_Exs(file.targetPath_Exs);
        if (fileSize != file.fileSize_Exs) {
            logger->LogError_Exs("File size mismatch for: " + file.fileName_Exs);
            return false;
        }
    }
    
    logger->LogInfo_Exs("Update verification successful");
    return true;
}

UpdateInfo_Exs UpdateChecker_Exs::GetUpdateInfo_Exs() const {
    return updateInfo_Exs;
}

bool UpdateChecker_Exs::IsUpdateAvailable_Exs() const {
    if (updateInfo_Exs.latestVersion_Exs.empty()) {
        return false;
    }
    
    return CompareVersions_Exs(currentVersion_Exs, updateInfo_Exs.latestVersion_Exs);
}

bool UpdateChecker_Exs::IsUpdateRequired_Exs() const {
    return updateInfo_Exs.priority_Exs == UpdatePriority_Exs::REQUIRED_Exs ||
           updateInfo_Exs.priority_Exs == UpdatePriority_Exs::CRITICAL_Exs;
}

bool UpdateChecker_Exs::FetchUpdateManifest_Exs() {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return false;
    }
    
    std::string url = updateServerUrl_Exs + "/check-update?version=" + currentVersion_Exs;
    std::string response;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback_Exs);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        return false;
    }
    
    try {
        nlohmann::json json = nlohmann::json::parse(response);
        
        updateInfo_Exs.latestVersion_Exs = json["latest_version"];
        updateInfo_Exs.totalSize_Exs = json["total_size"];
        
        std::string priorityStr = json["priority"];
        if (priorityStr == "optional") updateInfo_Exs.priority_Exs = UpdatePriority_Exs::OPTIONAL_Exs;
        else if (priorityStr == "recommended") updateInfo_Exs.priority_Exs = UpdatePriority_Exs::RECOMMENDED_Exs;
        else if (priorityStr == "required") updateInfo_Exs.priority_Exs = UpdatePriority_Exs::REQUIRED_Exs;
        else updateInfo_Exs.priority_Exs = UpdatePriority_Exs::CRITICAL_Exs;
        
        // Parse patch notes
        for (const auto& note : json["patch_notes"]) {
            PatchNote_Exs patchNote;
            patchNote.version_Exs = note["version"];
            patchNote.changes_Exs = note["changes"].get<std::vector<std::string>>();
            updateInfo_Exs.patchNotes_Exs.push_back(patchNote);
        }
        
        // Parse update files
        for (const auto& file : json["files"]) {
            UpdateFile_Exs updateFile;
            updateFile.fileName_Exs = file["name"];
            updateFile.downloadUrl_Exs = file["url"];
            updateFile.fileSize_Exs = file["size"];
            updateFile.targetPath_Exs = file["target"];
            updateInfo_Exs.updateFiles_Exs.push_back(updateFile);
        }
        
    } catch (const std::exception& e) {
        return false;
    }
    
    return true;
}

bool UpdateChecker_Exs::CompareVersions_Exs(const std::string& v1, const std::string& v2) {
    // Simple version comparison
    // Real implementation would parse version numbers properly
    return v1 != v2;
}

bool UpdateChecker_Exs::DownloadUpdateFile_Exs(const UpdateFile_Exs& file) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return false;
    }
    
    FILE* fp = fopen(file.targetPath_Exs.c_str(), "wb");
    if (!fp) {
        curl_easy_cleanup(curl);
        return false;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, file.downloadUrl_Exs.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    
    CURLcode res = curl_easy_perform(curl);
    
    fclose(fp);
    curl_easy_cleanup(curl);
    
    return res == CURLE_OK;
}

bool UpdateChecker_Exs::ApplyPatch_Exs(const std::string& patchPath) {
    FileSystem_Exs* fs = FileSystem_Exs::GetInstance_Exs();
    
    // Check if file exists
    if (!fs->FileExists_Exs(patchPath)) {
        return false;
    }
    
    // Apply patch logic
    // This would involve patching existing files or replacing them
    
    return true;
}

size_t UpdateChecker_Exs::WriteCallback_Exs(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

} // namespace Launcher
} // namespace Internal
} // namespace CyberGuardian
