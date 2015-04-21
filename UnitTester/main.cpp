#include <cstdio>
#include <iostream>
#include <map>

#include <include\decorators.h>
#include <include\types.h>

#include <UnitTests/Tests.h>
#include <sstream>

namespace UnitTests {
    class __declspec(dllimport) Tests;
}

int printAllTests() {
    using namespace UnitTests;

    // Print all tests
    std::cout << "*** Tests ***\n";
    int i = 0;
    for (auto& it = Tests::begin(); it != Tests::end(); it++) {
        std::cout << i++ << ": " << it->first << std::endl;
    }
    std::cout << "_____________\n\n";
    return i;
}

int main(int argc, char** argv) {
    using namespace UnitTests;
    nString test;
    int testi;
    int i = printAllTests();

    while (true) {
        // Get test input
        std::cout << "Enter Test: ";
        std::cin >> test;
        // Check for integer input
        std::istringstream buffer(test);
        if (buffer >> testi) {
            if (testi < 0 || testi >= i) {
                i = printAllTests();
                std::cerr << "Invalid input." << std::endl;
                continue;
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