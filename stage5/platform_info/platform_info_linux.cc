// stage5/platform_info/platform_info_linux.cc
// Selected by Bazel when the target platform is Linux.
#include "stage5/platform_info/platform_info.h"

namespace platform {

std::string os_name() { return "Linux"; }

std::string cpu_name() {
#if defined(__aarch64__)
    return "arm64";
#elif defined(__x86_64__)
    return "x86_64";
#elif defined(__arm__)
    return "arm32";
#elif defined(__riscv)
    return "riscv";
#else
    return "unknown";
#endif
}

std::string compiler() {
#if defined(__clang__)
    return "clang " __clang_version__;
#elif defined(__GNUC__)
    return "GCC " __VERSION__;
#else
    return "unknown compiler";
#endif
}

}  // namespace platform
