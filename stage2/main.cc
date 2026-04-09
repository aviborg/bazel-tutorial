// stage2/main.cc
//
// Bazel include paths are workspace-rooted, like:
//   #include "stage2/math/math_utils.h"
// NOT relative like:
//   #include "../math/math_utils.h"   <- won't work in Bazel
//
// This matches CMake's convention when you use:
//   include_directories(${CMAKE_SOURCE_DIR})

#include <iostream>
#include "stage2/math/math_utils.h"

int main() {
    std::cout << "3 + 4 = "      << math::add(3, 4)       << "\n";
    std::cout << "3 * 4 = "      << math::multiply(3, 4)  << "\n";
    std::cout << "2 ^ 10 = "     << math::power(2, 10)    << "\n";
    return 0;
}
