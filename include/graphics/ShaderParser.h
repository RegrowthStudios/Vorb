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

namespace vorb {
    namespace graphics {

        class ShaderParser {
        public:
            static bool parseShader(const nString& inputCode, OUT nString& resultCode);
        };
    }
}

#endif // ShaderParser_h__