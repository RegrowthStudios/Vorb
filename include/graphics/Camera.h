//
// Camera.h
// Vorb Engine
//
// Created by Benjamin Arnold on 20 Jun 2016
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Camera.h
* @brief 
* Basic 3D camera implementation.
*
*/

#pragma once

#ifndef Vorb_Camera_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Camera_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH
namespace vorb     {
namespace graphics {

    class Camera {
    public:

    private:
        f64v3 m_focalPoint = f64v3(0.0);
        f64v3 m_position   = f64v3(0.0);
    };

}
}
namespace vg = vorb::graphics;

#endif // !Vorb_Camera_h__

