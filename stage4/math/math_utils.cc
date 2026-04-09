// stage4/math/math_utils.cc
#include "stage4/math/math_utils.h"

namespace math {

int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }

bool is_prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

}  // namespace math
