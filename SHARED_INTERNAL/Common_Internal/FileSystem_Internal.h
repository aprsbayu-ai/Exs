#pragma once
#ifndef CYBERGUARDIAN_FILESYSTEM_INTERNAL_H
#define CYBERGUARDIAN_FILESYSTEM_INTERNAL_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <mutex>

namespace CyberGuardian {
namespace Internal {
namespace Common {

class FileSystem_Exs {
private:
    static FileSystem_Exs* instance_Exs;
    std::string gameRoot_Exs;
    std::mutex fileMutex_Exs;
    
    FileSystem_Exs();
    ~FileSystem_Exs();
    
public:
    static FileSystem_Exs* GetInstance_Exs();
    static void DestroyInstance_Exs();
    
    bool Initialize_Exs(const std::string& rootPath);
    
    bool FileExists_Exs(const std::string& path) const;
    bool DirectoryExists_Exs(const std::string& path) const;
    bool CreateDirectory_Exs(const std::string& path);
    bool DeleteFile_Exs(const std::string& path);
    bool DeleteDirectory_Exs(const std::string& path);
    
    std::vector<std::string> ListFiles_Exs(const std::string& path, const std::string& extension = "") const;
    std::vector<std::string> ListDirectories_Exs(const std::string& path) const;
    
    size_t GetFileSize_Exs(const std::string& path) const;
    std::string GetFileExtension_Exs(const std::string& path) const;
    std::string GetFileName_Exs(const std::string& path) const;
    std::string GetDirectory_Exs(const std::string& path) const;
    
    bool ReadFile_Exs(const std::string& path, std::vector<uint8_t>& buffer);
    bool ReadTextFile_Exs(const std::string& path, std::string& content);
    bool WriteFile_Exs(const std::string& path, const std::vector<uint8_t>& buffer);
    bool WriteTextFile_Exs(const std::string& path, const std::string& content);
    bool AppendToFile_Exs(const std::string& path, const std::string& content);
    
    bool CopyFile_Exs(const std::string& source, const std::string& destination);
    bool MoveFile_Exs(const std::string& source, const std::string& destination);
    bool RenameFile_Exs(const std::string& oldPath, const std::string& newPath);
    
    std::string GetFullPath_Exs(const std::string& relativePath) const;
    std::string GetRelativePath_Exs(const std::string& absolutePath) const;
    
    std::string GetGameRoot_Exs() const;
    std::string GetSaveDirectory_Exs() const;
    std::string GetConfigDirectory_Exs() const;
    std::string GetLogDirectory_Exs() const;
    std::string GetAssetDirectory_Exs() const;
    
    bool IsAbsolutePath_Exs(const std::string& path) const;
    bool IsRelativePath_Exs(const std::string& path) const;
    
private:
    std::string NormalizePath_Exs(const std::string& path) const;
    bool EnsureDirectoryExists_Exs(const std::string& path);
};

} // namespace Common
} // namespace Internal
} // namespace CyberGuardian

#endif
