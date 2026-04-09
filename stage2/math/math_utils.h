// stage2/math/math_utils.h
//
// In Bazel, headers are declared explicitly in 'hdrs'.
// Bazel uses a hermetic include sandbox — if you don't list a header
// in 'hdrs' (or 'srcs' for private headers), dependents can't see it.
// This is stricter than CMake's target_include_directories() but catches
// forgotten dependencies at build time rather than on another machine.

#pragma once

namespace math {

int add(int a, int b);
int multiply(int a, int b);
double power(double base, int exp);

}  // namespace math
