//
// SoundResource.h
// Vorb Engine
//
// Created by Cristian Zaloj on 3 Jan 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file SoundResource.h
 * 
 * \brief A wrapper for sound data container.
 */

#pragma once

#ifndef Vorb_SoundResource_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_SoundResource_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "../IDGenerator.h"
#include "ISoundImpl.h"

namespace vorb {
    namespace sound {
        typedef ui32 ResourceID; ///< ID type for a resource
        typedef vcore::IDGenerator<ui32> ResourceIDGenerator; ///< ID generator for sound resources

        /// Sound data used for instancing
        class Resource {
            friend class Engine;
        public:
            /// \return True if this resource has no sound data
            bool isNull() const {
                return m_id == ID_GENERATOR_NULL_ID;
            }
        private:
            ResourceID m_id = ID_GENERATOR_NULL_ID; ///< Unique ID
            impl::ResourceData* m_data = nullptr; ///< Implementation-defined resource data
        };
    }
}
namespace vsound = vorb::sound;

#endif // !Vorb_SoundResource_h__
