// stage5/platform_info/platform_info.h
//
// Common interface — the same header regardless of platform.
// Each platform gets its own .cc implementation file.

#pragma once
#include <string>

namespace platform {

// Returns a human-readable description of the current OS.
std::string os_name();

// Returns a human-readable description of the CPU architecture.
std::string cpu_name();

// Returns compiler name + version string.
std::string compiler();

}  // namespace platform
