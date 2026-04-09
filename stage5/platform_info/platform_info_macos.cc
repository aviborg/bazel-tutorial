// stage5/platform_info/platform_info_macos.cc
// Selected by Bazel when the target platform is macOS.
#include "stage5/platform_info/platform_info.h"

namespace platform {

std::string os_name() { return "macOS"; }

std::string cpu_name() {
#if defined(__aarch64__)
    return "Apple Silicon (arm64)";
#else
    return "Intel (x86_64)";
#endif
}

std::string compiler() {
#if defined(__clang__)
    return "clang " __clang_version__;
#else
    return "unknown compiler";
#endif
}

}  // namespace platform
