//
// MathScriptInterface.h
// Vorb Engine
//
// Created by Benjamin Arnold on 20 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file MathScriptInterface.h
* @brief Scripting interface for common math functions and types.
*/

#pragma once

#ifndef Vorb_MathScriptInterface_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_MathScriptInterface_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include "Environment.h"

namespace vorb {
    namespace script {
        class MathScriptInterface {
            friend class Environment;
        protected:
            template<typename T>
            glm::tvec2<T>* packV2(T x, T y) {
                return new glm::tvec2<T>(x, y);
            }
            template<typename T>
            glm::tvec3<T>* packV3(T x, T y, T z) {
                return new glm::tvec3<T>(x, y, z);
            }
            template<typename T>
            glm::tvec4<T>* packV4(T x, T y, T z, T w) {
                return new glm::tvec4<T>(x, y, z, w);
            }
            template<typename T>
            std::tuple<T, T> unpackV2(glm::tvec3<T>& v) {

            }

            // TODO(Ben): Move to cpp
            static void* packF32(lua_State* L) {
                // Get arguments
           /*     int args = lua_gettop(L);
                switch (args) {
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        break;
                    default:
                        return nullptr;
                }*/
            }

        };
    }
}
namespace vscript = vorb::script;

#endif // !Vorb_MathScriptInterface_h__

