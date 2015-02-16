#include <include/stdafx.h>
#include <UnitTests/Tests.h>
#include <sstream>

namespace UnitTests {
    class __declspec(dllimport) Tests;
}

int main(int argc, char** argv) {
    using namespace UnitTests;
    nString test;
    int testi;
    while (true) {
        // Print all tests
        std::cout << "*** Tests ***\n";
        int i = 0;
        for (auto& it = Tests::begin(); it != Tests::end(); it++) {
            std::cout << i++ << ": " << it->first << std::endl;
        }
        std::cout << "_____________\n\n";
        // Get test input
        std::cout << "Enter Test: ";
        std::cin >> test;
        // Check for integer input
        std::istringstream buffer(test);
        if (buffer >> testi) {
            if (testi < 0 || testi >= i) {
                std::cerr << "Invalid input. Enter any key to quit...";
                std::cin >> i;
                return 1;
            }
            int j = 0;
            for (auto& it = Tests::begin(); it != Tests::end(); it++) {
                if (j == testi) {
                    test = it->first;
                    break;
                }
                j++;
            }
        }
        if (test != "q") {
            try {
                puts("===============================");
                printf("Running %s\n", test.c_str());
                bool res = Tests::runTest(test);
                puts("=  =  =  =  =  =  =  =  =  =  =");
                puts(res ? "Result: PASS" : "Result: FAIL");
                puts("===============================");
            } catch (...) {
                puts("Exception(al) Failure");
            }
        } else {
            break;
        }
    }
    return 0;
}