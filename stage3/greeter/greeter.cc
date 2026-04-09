// stage3/greeter/greeter.cc
//
// This library depends on //stage3/strings:strings.
// Bazel enforces that dependency graph: if you use a symbol from
// another target you MUST list it in 'deps', otherwise the build
// fails with a "missing dependency" error — great for catching
// accidental transitive deps that CMake often silently allows.

#include "stage3/greeter/greeter.h"
#include "stage3/strings/strings.h"

namespace greeter {

Greeter::Greeter(std::vector<std::string> names) : names_(std::move(names)) {}

std::string Greeter::greet() const {
    return "Hello, " + strings::join(names_, ", ") + "!";
}

std::string Greeter::shout() const {
    return strings::to_upper(greet());
}

}  // namespace greeter
