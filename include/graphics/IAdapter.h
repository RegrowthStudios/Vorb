//
// IAdapter.h
// Vorb Engine
//
// Created by Cristian Zaloj on 3 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IAdapter.h
 * @brief 
 */

#pragma once

#ifndef Vorb_IAdapter_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IAdapter_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

// https://developer.nvidia.com/sites/default/files/akamai/gamedev/files/gdc12/Efficient_Buffer_Management_McDonald.pdf

namespace vorb {
    namespace graphics {
        class IContext;
        class IDevice;

        /*! @brief An enumeration of the supported 3D APIs.
         */
        enum class API {
            DIRECT_3D, ///< Microsoft's 3D standard.
            OPENGL ///< Khronos' 3D standard.
        };
        ENUM_CLASS_OPS_INL(API, ui32);

        /*! @brief A specific version of an API denoted by versioning numbers.
         */
        struct APIVersion {
        public:
            API api; ///< The targeted API.
            struct {
                /*! @brief The major version of this API.
                 * 
                 * For D3D, this must be at least 9.
                 */
                ui32 major;
                ui32 minor; ///< The minor version number.
            } version;

            bool operator < (const APIVersion rhs) const {
                if (api < rhs.api) return true;
                if (api == rhs.api) {
                    if (version.major < rhs.version.major) return true;
                    if (version.major == rhs.version.major) {
                        return version.minor < rhs.version.minor;
                    }
                }
                return false;
            }
            bool operator > (const APIVersion rhs) const {
                if (api > rhs.api) return true;
                if (api == rhs.api) {
                    if (version.major > rhs.version.major) return true;
                    if (version.major == rhs.version.major) {
                        return version.minor > rhs.version.minor;
                    }
                }
                return false;
            }
            bool operator == (const APIVersion rhs) const {
                return api == rhs.api &&
                    version.major == rhs.version.major &&
                    version.minor == rhs.version.minor;
            }
            bool operator != (const APIVersion rhs) const {
                return api != rhs.api ||
                    version.major != rhs.version.major ||
                    version.minor != rhs.version.minor;
            }
        };

        class IAdapter {
        public:
            typedef void* WindowHandle;

            IAdapter(APIVersion version);

            static void listAdapterTypes(std::vector<APIVersion>& apis);

            virtual IContext* createContext(OUT OPT IDevice** defaultDevice) = 0;
            virtual IDevice* createDevice(IContext* context) = 0;

            virtual void attachToWindow(IContext* context, void* hWnd) = 0;

            const APIVersion& getRequestedVersion() const {
                return m_requestedVersion;
            }
        protected:
            APIVersion m_requestedVersion; ///< This the API version that the adapter will use for creating contexts
        };

        IAdapter* getAdapter(API api, ui32 majorVersion, ui32 minorVersion);
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_IAdapter_h__
