#include "alsaout/alsaout.hpp"

#include <iostream>
#include <string>

int main() {
    std::string message = "Hello, World!";
    std::cout << message << std::endl;
    alsaoutrun();
    return 0;
}
