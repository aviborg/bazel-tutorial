// stage2/math/math_utils.cc
#include "stage2/math/math_utils.h"

namespace math {

int add(int a, int b) { return a + b; }

int multiply(int a, int b) { return a * b; }

double power(double base, int exp) {
    double result = 1.0;
    for (int i = 0; i < exp; ++i) result *= base;
    return result;
}

}  // namespace math
