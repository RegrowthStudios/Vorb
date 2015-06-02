//
// IResource.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file IResource.h
 * @brief 
 */

#pragma once

#ifndef Vorb_IResource_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IResource_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace graphics {
        class IContext;

        /* @brief
         */
        class IResource {
            friend class IContext;
        public:
            virtual ~IResource() {
                // Empty
            }

            virtual size_t getMemoryUsed() = 0;
        protected:
            IResource(IContext* owner) : 
                m_owner(owner) {
                // Empty
            }

            IContext* m_owner; ///< The context that created this resource
        };

        /* @brief
         */
        class IBuffer : public IResource {
        public:

        protected:
            IBuffer(IContext* owner);
        };

        /* @brief
         */
        class ITexture : public IResource {
        public:

        protected:
            ITexture(IContext* owner);
        };

        /* @brief
         */
        class IShader : public IResource {
        public:

        protected:
            IShader(IContext* owner);
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_IResource_h__
