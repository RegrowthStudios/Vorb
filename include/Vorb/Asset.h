//
// Asset.h
// Vorb Engine
//
// Created by Benjamin Arnold on 4 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Asset.h
 * @brief Generic asset class.
 *
 * Custom assets should derive from this class.
 */

#pragma once

#ifndef Vorb_Asset_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Asset_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace core {
        class Asset {
        public:
            nString name;
            volatile bool isLoaded;
            bool shouldFree;
        };
    }
}
namespace vcore = vorb::core;

#endif // !Vorb_Asset_h__
