//
// Vorb.h
// Vorb Engine
//
// Created by Cristian Zaloj on 16 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file Vorb.h
 * @brief Vorb module initialization and disposal functions are declared here.
 */

#pragma once

#ifndef Vorb_Vorb_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_Vorb_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    /** @brief Defines various subsystems that Vorb must initialize before usage
     * 
     * With the exception of NONE and ALL, each value is a single unique bit
     * that acts as a flag. Thus combinations such as (GRAPHICS | IO) can be
     * created and used.
     */
    enum class InitParam {
        NONE = 0x00, ///< No submodules selected
        GRAPHICS = 0x01, ///< The graphics submodule
        IO = 0x02, ///< The IO submodule
        SOUND = 0x04, ///< The sound submodule
        NET = 0x08, ///< The network submodule
        ALL = GRAPHICS | SOUND | IO | NET ///< All submodules are selected
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

#endif // !Vorb_Vorb_h__

/** \example "Vorb Module Initialization"
*
* When using Vorb, you must initialize and dispose the submodules that you wish to use
* at the top level. You can specify any combinations as well as initialize and dispose at
* any time.
* \include VorbInit.cpp
*/
