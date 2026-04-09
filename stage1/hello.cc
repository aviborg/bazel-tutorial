// stage1/hello.cc — the simplest possible Bazel C++ target.
//
// BUILD COMMAND:
//   bazel build //stage1:hello
//
// RUN COMMAND:
//   bazel run //stage1:hello
//
// The compiled binary lands at:
//   bazel-bin/stage1/hello
// — never in the source tree, unlike a plain 'g++ -o hello hello.cc'.

#include <iostream>

int main() {
    std::cout << "Hello from Bazel!\n";
    return 0;
}
