//
// VorbAssert.hpp
// Vorb Engine
//
// Created by Benjamin Arnold on 27 Aug 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file VorbAssert.hpp
* @brief Defines custom vorb assertion.
*/

#pragma once

#ifndef Vorb_VorbAssert_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_VorbAssert_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include <string>
#include <iostream>
#include <sstream>

#include <exception>

// https://www.softwariness.com/articles/assertions-in-cpp/
class AssertionFailureException : public std::exception {
public:
    AssertionFailureException(const char* expression, const char* file, int line, const std::string& message) :
        expression(expression), file(file), line(line), message(message) {

        std::ostringstream outputStream;

        if (!message.empty()) {
            outputStream << message << ": ";
        }

        if ((strcmp(expression, "false") == 0) || (strcmp(expression, "0") == 0)) {
            outputStream << "Unreachable code assertion";
        } else {
            outputStream << "Assertion '" << expression << "'";
        }

        outputStream << " failed in file '" << file << "' line " << line;
        report = outputStream.str();

        logError();
    }
    ~AssertionFailureException() throw() {}

    /// Helper class for formatting assertion message
    class StreamFormatter {  
    public:
        operator const std::string() const {
            return m_stream.str();
        }

        template<typename T>
        StreamFormatter& operator << (const T& value) {
            m_stream << value;
            return *this;
        }
    private:
        std::ostringstream m_stream;
    };

    /// Log error before throwing
    // TODO(Ben): Handle redirecting and dumping to files.
    void logError() {
        std::cerr << report << std::endl;
    }


    /// The assertion message
    virtual const char* what() const throw() override {
        return report.c_str();
    }

    const char* expression;
    const char* file;
    int line;
    const std::string message;
    std::string report;
};

#ifdef NDEBUG
#define vorb_assert(EXPRESSION, MESSAGE) ((void)0)
#else
/// Assert that EXPRESSION evaluates to true, otherwise raise AssertionFailureException with associated MESSAGE (which may use C++ stream-style message formatting)
#define vorb_assert(EXPRESSION, MESSAGE) if(!(EXPRESSION)) { throw AssertionFailureException(#EXPRESSION, __FILE__, __LINE__, (AssertionFailureException::StreamFormatter() << MESSAGE)); }
#endif

#endif // !Vorb_VorbAssert_hpp__
