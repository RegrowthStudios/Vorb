///
///  Vorb.h
///  Vorb Engine
///
///  Created by Ben Arnold on 2/10/2014
///  Copyright 2014 Regrowth Studios
///  All Rights Reserved
///
///  Summary:
///  This is the main header for the vorb engine. It
///  contains the namespace declarations.

#pragma once

#ifndef Vorb_h__
#define Vorb_h__

namespace vorb {
    /** @brief Defines various subsystems that Vorb must initialize before usage
     * 
     * With the exception of NONE and ALL, each value is a single unique bit
     * that acts as a flag. Thus combinations such as (GRAPHICS | IO) can be
     * created and used.
     */
    enum class InitParam {
        NONE = 0x00,
        GRAPHICS = 0x01,
        IO = 0x02,
        SOUND = 0x04,
        NET = 0x08,
        ALL = GRAPHICS | SOUND | IO | NET
    };
    ENUM_CLASS_OPS_INL(vorb::InitParam, ui64)

    /*! @brief Initialize a set of Vorb subsystems
     * 
     * If a subsystem is already initialized, then this has no effect on that subsystem
     * and it returns as a success.
     * 
     * @param p: Request of subsystems that should be initialized
     * @return Subsystems that successfully initialized
     */
    InitParam init(const InitParam& p);
    /*! @brief Dispose a set of Vorb subsystems
     * 
     * If a subsystem is not initialized, then this has no effect on that subsystem
     * and it returns as a success.
     * 
     * @param p: Request of subsystems that should be destroyed
     * @return Subsystems that were successfully disposed
     */
    InitParam dispose(const InitParam& p);
}

#endif // Vorb_h__