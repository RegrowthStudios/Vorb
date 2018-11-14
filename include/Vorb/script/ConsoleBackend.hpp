//
// ConsoleBackend.h
// Vorb Engine
//
// Created by Matthew Marshall on 14 Nov 2018
// Based on Ben Arnold's LuaDevConsole implementation.
// Copyright 2018 Regrowth Studios
// MIT License
//

/*! \file ConsoleBackend.h
 * \brief Provides a generic backend for consoles that handles script commands.
 */

#pragma once

#ifndef Vorb_ConsoleBackend_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ConsoleBackend_h__
//! @endcond

#ifdef VORB_USING_SCRIPT

#include "Vorb/types.h"
#include "Vorb/script/IEnvironment.hpp"

namespace vorb {
    namespace script {
        /// Stores MemFiles for output and error streams - we use these to catch script output.
        struct ConsoleOutStreams {
            FILE* out;
            FILE* err;
        };

        /// Stores Events for output from scripts.
        struct ConsoleOutEvents {
            Event<const cString> out;
            Event<const cString> err;
        };

        template <typename EnvironmentImpl>
        class ConsoleBackend {
            using Environment = IEnvironment<EnvironmentImpl>;
        public:
            /*!
             * \brief Default Constructor.
             */
            ConsoleBackend();
            ~ConsoleBackend();

            /*!
             * \brief Initialises the console backend instance using the specified (or otherwise default)
             * command history size.
             *
             * \param env A pointer to the script environment to invoke commands through.
             * \param commandHistorySize The number of commands to store in the command history.
             */
            init(Environment* env, size_t commandHistorySize = DEFAULT_COMMAND_HISTORY_SIZE);

            /*!
             * \brief Disposes the console backend instance.
             */
            dispose();

            /*!
             * \brief Invokes the provided command within the console.
             */
            void invokeCommand(const nString& command);

            /*!
             * \brief Gets the latest command in the command history.
             */
            nString getLatestCommand();

            /*!
             * \brief Gets the index-th command in the command history.
             *
             * \param index The index of the command to retrieve.
             */
            nString getCommand(size_t index);

            /// The default size of the command history.
            const static size_t DEFAULT_COMMAND_HISTORY_SIZE = 16;

            /// The events that will be triggered on any script output.
            ConsoleOutEvents onConsoleOutput;
        protected:
            nString*                       m_commandHistory; ///< The history of commands invoked in this console backend instance.
            ui16                           m_size;           ///< The maximum number of commands that may be stored in the command history.
            ui16                           m_count;          ///< The number of commands so far stored in the command history.
            ui16                           m_head;           ///< The position in the vector of the latest command.
            IEnvironment<EnvironmentImpl>* m_env;            ///< The script environment in which to invoke provided commands.
            ConsoleOutStreams              m_streams;        ///< Temporary streams to capture output of the scripts

            inline void checkOut(MemFile* stream, i64 initialPos, Event<const cString>* event);
        };
    }
}
namespace vscript = vorb::script;

template <typename EnvironmentImpl>
vscript::ConsoleBackend<EnvironmentImpl>::ConsoleBackend() :
    m_commandHistory(nullptr),
    m_size(0), m_count(0), m_head(0),
    m_env(nullptr),
    m_streams({ nullptr, nullptr })
{ /* Empty */ }

template <typename EnvironmentImpl>
~vscript::ConsoleBackend<EnvironmentImpl>::ConsoleBackend() {
    dispose();
}

template <typename EnvironmentImpl>
vscript::ConsoleBackend<EnvironmentImpl>::init(Environment* env, size_t commandHistorySize /*= DEFAULT_COMMAND_HISTORY_SIZE*/) {
    assert(commandHistorySize > 0);

    m_commandHistory = new nString(commandHistorySize);
    m_size = commandHistorySize;

    m_env = env;

    m_streams.out = tmpfile();
    m_streams.err = tmpfile();
}

template <typename EnvironmentImpl>
vscript::ConsoleBackend<EnvironmentImpl>::dispose() {
    if (m_commandHistory) delete m_commandHistory;
    m_commandHistory = nullptr;
    m_size  = 0;
    m_count = 0;
    m_head  = 0;
    m_env   = nullptr;
    if (m_streams.out) fclose(m_streams.out);
    if (m_streams.err) fclose(m_streams.err);
    m_streams.out = nullptr;
    m_streams.err = nullptr;
}

template <typename EnvironmentImpl>
vscript::ConsoleBackend<EnvironmentImpl>::invokeCommand(const nString& command) {
    // Record the initial cursor values of the two capturing streams.
    i64 initialPosOut = ftell(m_streams.out);
    i64 initialPosErr = ftell(m_streams.in);

    // TODO(Matthew): This feels super dirty for multi-threaded...
    // Store temporaries of the current stdout, stderr objects.
    FILE oldOut = *stdout;
    FILE oldErr = *stderr;

    // Set stdout & stderr to be our MemFile instances.
    *stdout = m_streams.out;
    *stderr = m_streams.err;

    // Run the command in our script environment.
    m_env->run(command);

    // Store the ran command in our history.
    m_commandHistory[m_head++] = command;
    if (m_head == m_size) m_head = 0;
    if (m_count < m_size) ++m_count;

    // Reset stdout & stderr.
    *stdout = oldOut;
    *stderr = oldErr;

    // Check our MemFile instances for any output from the script that ran.
    checkOut(m_streams.out, initialPosOut, &onConsoleOutput.out);
    checkOut(m_streams.err, initialPosErr, &onConsoleOutput.err);
}

template <typename EnvironmentImpl>
nString vscript::ConsoleBackend<EnvironmentImpl>::getLatestCommand() {
    if (m_count == 0) return "";

    return m_commandHistory[(m_size + m_head - 1) % m_size];
}

template <typename EnvironmentImpl>
nString vscript::ConsoleBackend<EnvironmentImpl>::getCommand(size_t index) {
    if (index >= m_count) return "";

    return m_commandHistory[(m_size + m_head - index - 1) % m_size];
}

template <typename EnvironmentImpl>
void vscript::ConsoleBackend<EnvironmentImpl>::checkOut(FILE* stream, i64 initialPos, Event<const cString>* event) {
    // Get the length of the output in this stream.
    size_t length = (size_t) (ftell(stream) - initialPos);

    // Leave if we have nothing to show.
    if (length == 0) return;

    // Create a character buffer to store the output in.
    char* buffer = new char[length + 1];

    // Move cursor to initial position, and read contents of the determined length.
    fseek(stream, initialPos, SEEK_SET);
    fread(buffer, 1, length, stream);
    buffer[length] = '\0';

    // Trigger our event.
    event->call(buffer);

    // Clean up.
    delete[] buffer;
    rewind(stream);
}

#endif // VORB_USING_SCRIPT

#endif // !Vorb_ConsoleBackend_h__
