//
//  Vorb.h
//  Vorb Engine
//
//  Created by Ben Arnold on 2/10/2014
//  Copyright 2014 Regrowth Studios
//  All Rights Reserved
//
//  Summary:
//  This is the main header for the vorb engine. It
//  contains the namespace declarations.

#pragma once

#ifndef Vorb_h__
#define Vorb_h__
#include "stdafx.h"
#include "VorbPreDecl.inl"

namespace vorb {
    /// Defines various subsystems that Vorb initializes before using
    enum class InitParam {
        NONE = 0x00,
        GRAPHICS = 0x01,
        IO = 0x02,
        SOUND = 0x04,
        NET = 0x08,
        ALL = GRAPHICS | SOUND | IO | NET
    };
    ENUM_CLASS_OPS_INL(vorb::InitParam, ui64)

    /// Initialize Vorb subsystems so that features from them may be used
    /// @param p: Request of subsystems that should be initialized
    /// @return Subsystems that successfully initialized
    InitParam init(const InitParam& p);
    /// Dispose Vorb subsystems
    /// @param p: Request of subsystems that should be destroyed
    /// @return Subsystems that were successfully disposed
    InitParam dispose(const InitParam& p);
}

#endif // Vorb_h__