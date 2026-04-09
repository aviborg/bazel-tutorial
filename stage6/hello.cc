// stage6/hello.cc — a trivially simple binary so we can focus on
// the toolchain, not the application code.
// After a successful cross-compile, 'file bazel-bin/stage6/hello'
// will report: ELF 64-bit LSB executable, ARM aarch64

#include <iostream>

int main() {
    std::cout << "Hello from a cross-compiled ARM64 binary!\n";
    return 0;
}
