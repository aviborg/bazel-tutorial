// stage3/main.cc
#include <iostream>
#include "stage3/greeter/greeter.h"

int main() {
    greeter::Greeter g({"Alice", "Bob", "Charlie"});
    std::cout << g.greet()  << "\n";
    std::cout << g.shout()  << "\n";
    return 0;
}
