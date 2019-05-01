#include "src/static.hpp"
#include "src/dynamic_linkage.hpp"

#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <dlfcn.h>

namespace console {

    std::string _ERROR = "\033[31;1m";
    std::string _DEFAULT = "\033[0m";

    int report(std::string const &message, int err = 0) {
        std::cerr << _ERROR << message;
        if (err != 0) {
            std::cerr << std::strerror(errno);
        }
        std::cerr << std::endl << _DEFAULT;
        return 0;
    }

}

int main() {

    std::cout << "Please input a positive integer number: ";
    unsigned int value;
    std::cin >> value;

    print_ascii_number(value);
    print_inversion(value);

    std::string library_name = "lib/dynamic_load.so";
    void *lib = dlopen(library_name.c_str(), RTLD_LAZY);

    if (lib == nullptr) {
        return console::report("Can not link with external library:", errno);
    }
    auto print_random_name = (void(*)()) dlsym(lib, "print_random_name");
    if (print_random_name == nullptr) {
        dlclose(lib);
        return console::report("Can not load function from linked library:", errno);
    }

    (*print_random_name)();
    dlclose(lib);

}
