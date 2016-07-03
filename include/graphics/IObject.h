//
// IObject.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file IObject.h
 * 
 * \brief 
 */

#pragma once

#ifndef Vorb_IObject_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_IObject_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace graphics {
        class IDevice;

        /*! \brief
         */
        class IObject {
            friend class IDevice;
        public:
            virtual ~IObject() {
                // Empty
            }

            void dispose();
        protected:
            IObject(IDevice* owner) :
                m_owner(owner) {
                // Empty
            }
            
            virtual void disposeInternal() = 0;

            IDevice* m_owner; ///< The state view that owns this object.
        };

        /*! \brief
         */
        class IVertexStateBind : public IObject {
        public:

        protected:
            IVertexStateBind(IDevice* owner) : IObject(owner) {
                // Empty
            }
        };

        /*! \brief
         */
        class IEffect : public IObject {
        public:

        protected:
            IEffect(IDevice* owner) : IObject(owner) {
                // Empty
            }
        };

        /*! \brief
         */
        class IRenderTarget : public IObject {
        public:

        protected:
            IRenderTarget(IDevice* owner) : IObject(owner) {
                // Empty
            }
        };

        /*! \brief
         */
        class IBlendState : public IObject {
        public:

        protected:
            IBlendState(IDevice* owner) : IObject(owner) {
                // Empty
            }
        };

        /*! \brief
         */
        class IDepthStencilState : public IObject {
        public:

        protected:
            IDepthStencilState(IDevice* owner) : IObject(owner) {
                // Empty
            }
        };

        /*! \brief
         */
        class IRasterizerState : public IObject {
        public:

        protected:
            IRasterizerState(IDevice* owner) : IObject(owner) {
                // Empty
            }
        };

        /*! \brief
         */
        class ISamplerState : public IObject {
        public:

        protected:
            ISamplerState(IDevice* owner) : IObject(owner) {
                // Empty
            }
        };


        /*! \brief
          */
        class IQuery : public IObject {
        public:

        protected:
            IQuery(IDevice* owner) : IObject(owner) {
                // Empty
            }
        };

        /*! \brief
         */
        class IPredicate : public IQuery {
        public:

        protected:
            IPredicate(IDevice* owner) : IQuery(owner) {
                // Empty
            }
        };

        /*! \brief
         */
        class ISyncFence : public IObject {
        public:

        protected:
            ISyncFence(IDevice* owner) : IObject(owner) {
                // Empty
            }
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_IObject_h__
