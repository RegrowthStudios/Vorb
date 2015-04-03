///
/// ShaderManager.h
/// Vorb
///
/// Created by Benjamin Arnold on 29 Mar 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Implements ShaderManager, which handles caching, creation, and destruction of GLPrograms
///

#pragma once

#ifndef ShaderManager_h__
#define ShaderManager_h__

#include <map>
#include "../Events.hpp"
#include "../VorbPreDecl.inl"
#include "../io/Path.h"

DECL_VIO(class IOManager)

namespace vorb {
    namespace graphics {

        class GLProgram;
        typedef std::map<nString, GLProgram*> GLProgramMap;

        /// Static class that handles caching, creation, and destruction of GLPrograms
        class ShaderManager {
        public:
            /// TODO(Ben): Add overloads for tesselation, geometry, ect. shaders
            /// Creates a program from source code
            /// Does not register
            static CALLER_DELETE GLProgram* createProgram(const cString vertSrc, const cString fragSrc,
                                                          vio::IOManager* vertIOM = nullptr,
                                                          vio::IOManager* fragIOM = nullptr,
                                                          cString defines = nullptr);
            /// Creates a program using code loaded from files
            /// Does not register
            static CALLER_DELETE GLProgram* createProgramFromFile(const vio::Path& vertPath, const vio::Path& fragPath,
                                                                  vio::IOManager* iom = nullptr, cString defines = nullptr);

            /// Disposes and deallocates a program and sets handle to nullptr
            static void destroyProgram(CALLEE_DELETE GLProgram** program);

            /// Disposes and deallocates all cached programs and clears the cache
            static void destroyAllPrograms();

            /// Adds a program to the global cache
            /// returns false if a program is already cached on that name
            static bool registerProgram(const nString& name, GLProgram* program);

            /// Removes a program from the global cache and returns it
            static CALLER_DELETE GLProgram* unregisterProgram(const nString& name);
            /// Removes a program from the global cache and returns it
            /// WARNING: Is slower than the nString version - O(n) lookup
            /// instead of O(log(n))
            static bool unregisterProgram(const GLProgram* program);

            /// Gets size of program cache
            static GLProgramMap::size_type getNumCachedPrograms() { return m_programMap.size(); };

            /// Gets the program map of cached programs
            static const GLProgramMap& getProgramCache() { return m_programMap; }

            /// Event that triggers when IO fails for reading the input files
            static Event<const nString&> onFileIOFailure;
            static Event<const nString&> onShaderCompilationError; ///< Event signaled during addShader when an error occurs
            static Event<const nString&> onProgramLinkError; ///< Event signaled during link when an error occurs
        private:
            static void triggerShaderCompilationError(Sender s, const nString& n);
            static void triggerProgramLinkError(Sender s, const nString& n);
            static GLProgramMap m_programMap; ///< For globally caching programs
        };
    }
}
namespace vg = vorb::graphics;

#endif // ShaderManager_h__