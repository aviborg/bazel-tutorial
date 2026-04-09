// stage5/main.cc
#include <iostream>
#include "stage5/platform_info/platform_info.h"

int main() {
    std::cout << "OS:       " << platform::os_name()  << "\n";
    std::cout << "CPU:      " << platform::cpu_name() << "\n";
    std::cout << "Compiler: " << platform::compiler() << "\n";
    return 0;
}
