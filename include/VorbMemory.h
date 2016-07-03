//
// VorbMemory.h
// Vorb Engine
//
// Created by Benjamin Arnold on 20 Aug 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file VorbMemory.h
 * 
* \brief Defines memory utilities such as aligned allocation.
*/

#pragma once

#ifndef Vorb_VorbMemory_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_VorbMemory_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

#include <stdlib.h>
#include <memory>

namespace vorb {
    namespace core {
        /*! \brief Mallocs aligned memory.
         * 
         * \param size: Size in bytes of the returned block.
         * \param alignment: Alignment in bytes of the returned block.
         * \return pointer to new memory block.
         */
        void* aligned_malloc(size_t size, size_t alignment) {
            void* p1; // original block
            void** p2; // aligned block
            int offset = alignment - 1 + sizeof(void*);
            if ((p1 = (void*)malloc(size + offset)) == NULL) return NULL;
            p2 = (void**)(((size_t)(p1)+offset) & ~(alignment - 1));
            p2[-1] = p1;
            return p2;
        }
        /*! \brief Frees memory allocated with aligned_malloc.
         * 
         * \param p: The pointer to free.
         */
        void aligned_free(void *p) {
            free(((void**)p)[-1]);
        }
    }
}
namespace vcore = vorb::core;

#endif // !Vorb_VorbMemory_h__
