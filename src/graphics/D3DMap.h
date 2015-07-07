//
// D3DMap.h
// Vorb Engine
//
// Created by Cristian Zaloj on 26 Jun 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file D3DMap.h
 * @brief 
 *
 *
 */

#pragma once

#ifndef Vorb_D3DMap_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_D3DMap_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include <d3d11.h>

#include "graphics/VGEnums.h"
#include "graphics/GLEnums.h"

namespace vorb {
    namespace graphics {
        extern DXGI_FORMAT formatMapDXGI[(size_t)MemoryFormat::MAX_VALUE];
        extern D3D11_PRIMITIVE_TOPOLOGY topologyMapD3D[(size_t)PrimitiveType::MAX_VALUE];

        void fillMaps();
        void fillDXGIMAP(DXGI_FORMAT* f);
        void fillTopologyMap(D3D11_PRIMITIVE_TOPOLOGY* f);
    }
}

#endif // !Vorb_D3DMap_h__
