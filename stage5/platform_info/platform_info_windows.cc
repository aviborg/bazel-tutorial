// stage5/platform_info/platform_info_windows.cc
// Selected by Bazel when the target platform is Windows.
#include "stage5/platform_info/platform_info.h"

namespace platform {

std::string os_name() { return "Windows"; }

std::string cpu_name() {
#if defined(_M_ARM64) || defined(__aarch64__)
    return "arm64";
#else
    return "x86_64";
#endif
}

std::string compiler() {
#if defined(_MSC_VER)
    return "MSVC " + std::to_string(_MSC_VER);
#elif defined(__clang__)
    return "clang-cl " __clang_version__;
#else
    return "unknown compiler";
#endif
}

}  // namespace platform
