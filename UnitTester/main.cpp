#include <include/stdafx.h>
#include <UnitTests/Tests.h>

namespace UnitTests {
    class __declspec(dllimport) Tests;
}

int main(int argc, char** argv) {
    using namespace UnitTests;
    nString test;
    while (true) {
        // Print all tests
        std::cout << "*** Tests ***\n";
        for (auto& it = Tests::begin(); it != Tests::end(); it++) {
            std::cout << it->first << std::endl;
        }
        std::cout << "_____________\n\n";
        // Get test input
        std::cout << "Enter Test: ";
        std::cin >> test;
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