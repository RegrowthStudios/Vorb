//
// decorators.h
// Vorb Engine
//
// Created by Cristian Zaloj on 16 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file decorators.h
 * @brief Function decorators for clarity.
 */

#pragma once

#ifndef Vorb_decorators_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_decorators_h__
//! @endcond

#define OUT /*!< This pointer/reference will be modified as an extra return value */
#define OPT /*!< This pointer is optional (may be null) */

#define CALLEE_DELETE /*!< Function called will delete this resource */
#define CALLER_DELETE /*!< Function caller must manage this resource */

#define VORB_INTERNAL /*!< Internal function not meant for an end-user */

#define UNIT_SPACE(SPACE_TYPE) /*!< The decorated value is found in <SPACE_TYPE> space */

/*! @brief This class may not be copied, so all copy and assignment constructors are hidden
 */
#define VORB_NON_COPYABLE(CLASS) \
    CLASS(const CLASS& o) = delete; \
    CLASS& operator=(const CLASS& o) = delete

#endif // !Vorb_decorators_h__
