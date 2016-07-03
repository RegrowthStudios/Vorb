//
// ShaderParser.h
// Vorb Engine
//
// Created by Benjamin Arnold on 29 Mar 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file ShaderParser.h
 * 
 * \brief Handles parsing of semantics and includes for shaders.
 */

#pragma once

#ifndef Vorb_ShaderParser_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ShaderParser_h__
//! @endcond

#ifndef VORB_USING_PCH
#include <map>
#include <set>

#include "../types.h"
#endif // !VORB_USING_PCH

#include "../Events.hpp"
#include "../Vorb.h"
#include "VorbPreDecl.inl"
#include "graphics/GLEnums.h"
#include "gtypes.h"
#include <vector>

DECL_VIO(class IOManager);

namespace vorb {
    namespace graphics {

        class ShaderParser {
        public:
            /// Parses includes and semantics for a vertex shader
            /// \param inputCode: The input code to use for parsing
            /// \param resultCode: The stored resulting code after parse
            /// \param attributeNames: The stored attribute names
            /// \param semantics: The stored semantics, 1 to 1 with attributeNames
            /// \param iom: Optional iomanager to use for include lookups
            static void parseVertexShader(const cString inputCode, OUT nString& resultCode,
                                          OUT std::vector<nString>& attributeNames, 
                                          OUT std::vector<VGSemantic>& semantics,
                                          vio::IOManager* iom = nullptr);
            // Parses includes for a fragment shader
            /// \param inputCode: The input code to use for parsing
            /// \param resultCode: The stored resulting code after parse
            /// \param iom: Optional iomanager to use for include lookups
            static void parseFragmentShader(const cString inputCode, OUT nString& resultCode,
                                            vio::IOManager* iom = nullptr);
            
            static Event<nString> onParseError; ///< Event that fires on a parsing error
        private:
            /// Initializes the semantic map. Call once.
            static void initSemantics();
            /// Attempts to parse an #include line
            /// \param s: The code to parse
            /// \param i: Position in the s string
            /// \return true if parse was successful
            /// @post s may be resized with new included code
            static bool tryParseInclude(nString& s, size_t i);
            /// Attempts to parse an attribute
            /// \param s: The code to parse
            /// \param i: Position in the s string
            /// \param semantic: Stores the semantic type
            /// \return the attribute name if successful or the empty string
            static nString tryParseAttribute(const cString s, size_t i, OUT VGSemantic& semantic);
            /// Checks if there is a comment at a point in the string,
            /// and sets the internal comment bools as well.
            /// \param s: The code to check
            /// \param i: Position in the code
            /// \return true if comment was found
            static bool checkForComment(const cString s, size_t i);
           
            static bool isComment() { return isBlockComment || isNormalComment; }

            static std::map<nString, Semantic> m_semantics; ///< List of all possible semantics
            static std::set<nString> m_parsedIncludes; ///< Cache of already parsed includes to check for circular includes
            static bool isNormalComment; ///< True when in a standard C++ style comment
            static bool isBlockComment; ///< True when in a block comment
            static vio::IOManager* ioManager; ///< IOManager handle to avoid passing
        };
    }
}

#endif // !Vorb_ShaderParser_h__
