//
// ResourceDescriptions.h
// Vorb Engine
//
// Created by Cristian Zaloj on 2 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file ResourceDescriptions.h
 * @brief 
 */

#pragma once

#ifndef Vorb_ResourceDescriptions_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ResourceDescriptions_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "GLEnums.h"

namespace vorb {
    namespace graphics {
        struct BufferDescription {
        public:
            vg::BufferTarget type;
            vg::BufferUsageHint usage;
            size_t size;
        };

        struct CBufferDescription;
        CBufferDescription* compile(const BufferDescription& desc);
        void free(CBufferDescription* desc);
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_ResourceDescriptions_h__
