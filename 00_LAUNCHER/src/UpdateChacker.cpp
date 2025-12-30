#include "internal/UpdateChecker_Internal.h"
#include "../include/UpdateChecker.h"

namespace CyberGuardian {
namespace Launcher {

UpdateChecker::UpdateChecker() : impl_Exs(std::make_unique<Internal::Launcher::UpdateChecker_Exs>()) {}

UpdateChecker::~UpdateChecker() = default;

bool UpdateChecker::CheckForUpdates() {
    return impl_Exs->CheckForUpdates_Exs();
}

bool UpdateChecker::DownloadUpdate() {
    return impl_Exs->DownloadUpdate_Exs();
}

bool UpdateChecker::ApplyUpdate() {
    return impl_Exs->ApplyUpdate_Exs();
}

bool UpdateChecker::VerifyUpdate() {
    return impl_Exs->VerifyUpdate_Exs();
}

UpdateInfo UpdateChecker::GetUpdateInfo() const {
    auto internalInfo = impl_Exs->GetUpdateInfo_Exs();
    
    UpdateInfo info;
    info.latestVersion = internalInfo.latestVersion_Exs;
    info.totalSize = internalInfo.totalSize_Exs;
    
    return info;
}

bool UpdateChecker::IsUpdateAvailable() const {
    return impl_Exs->IsUpdateAvailable_Exs();
}

bool UpdateChecker::IsUpdateRequired() const {
    return impl_Exs->IsUpdateRequired_Exs();
}

} // namespace Launcher
} // namespace CyberGuardian
