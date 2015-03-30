///
/// ShaderParser.h
/// Vorb
///
/// Created by Benjamin Arnold on 29 Mar 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Implements parser for shader code
///

#pragma once

#ifndef ShaderParser_h__
#define ShaderParser_h__

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
            static void parseVertexShader(const cString inputCode, OUT nString& resultCode,
                                          OUT std::vector<nString>& attributeNames, 
                                          OUT std::vector<VGSemantic>& semantics,
                                          vio::IOManager* iom = nullptr);
            static void parseFragmentShader(const cString inputCode, OUT nString& resultCode,
                                            vio::IOManager* iom = nullptr);

            static Event<nString> onParseError;
        private:
            static void initSemantics();
            static nString tryParseInclude(const cString s, size_t& i);
            static nString tryParseAttribute(const cString s, size_t i, VGSemantic& semantic);

            static std::map<nString, Semantic> m_semantics;
        };
    }
}

#endif // ShaderParser_h__