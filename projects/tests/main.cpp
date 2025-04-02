#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    int result = Catch::Session().run(argc, argv);

    std::cout << "Press enter to exit...";
    std::cin.get();

    return result;
}