// stage3/greeter/greeter.h
#pragma once
#include <string>
#include <vector>

namespace greeter {

// Holds names and produces greeting messages.
class Greeter {
public:
    explicit Greeter(std::vector<std::string> names);
    std::string greet() const;
    std::string shout() const;  // upper-case version

private:
    std::vector<std::string> names_;
};

}  // namespace greeter
