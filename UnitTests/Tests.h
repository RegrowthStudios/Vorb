#pragma once

#ifndef Tests_h__
#define Tests_h__

#include "../include/os.h"

typedef std::map<nString, void*> TestLibrary; ///< Container type for tests
__declspec(dllexport) TestLibrary& getLibrary(); ///< Member variable that stores tests

#define assert(...) ut_assert(__VA_ARGS__, __FILE__, __LINE__)
inline void ut_assert(bool v, const cString file, int line) {
    if (!v) {
        char s[1024];
        sprintf(s, "Assertion failed at %s : %d\n", file, line);
        throw std::exception(s);
    }
}

namespace UnitTests {
    /// Implementation hiding
    namespace Adder {
        class TestsAdder {
        public:
            static bool addTest(const nString& name, void* f);
        };
    }

    /// Unit test manager
    class __declspec(dllexport) Tests {
    public:

        /// Runs a unit test
        /// @param name: Name of test to run
        /// @return Success of the test
        static bool runTest(const nString& name);

        /// @return Iteration start of tests
        static TestLibrary::iterator begin();
        /// @return Iteration end of tests
        static TestLibrary::iterator end();

#ifdef OLD_CLR
        /// @return List of all unit test names
        static TestLibrary::KeyCollection^ getTests();

        /// Convert a unit test name into a friendly representation
        /// @return Array of batch strings (l[l.Length - 1] = friendly name of test)
        static array<String^>^ getBatches(String^ name);
#endif // OLD_CLR

    };
}

#endif // Tests_h__
