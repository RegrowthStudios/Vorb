///
/// decorators.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 29 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Useful function decorators
///

#pragma once

#ifndef decorators_h__
#define decorators_h__

#define OUT /* This pointer/reference will be modified as an extra return value */
#define OPT /* This pointer is optional (may be null) */

#define CALLEE_DELETE /* Function called will delete this resource */
#define CALLER_DELETE /* Function caller must manage this resource */

#define VORB_INTERNAL /* Internal function not meant for an end-user */

#define UNIT_SPACE(SPACE_TYPE) /* The value is found in SPACE_TYPE space */

#define VORB_EXPORT extern "C" __declspec(dllexport);
#define VORB_IMPORT extern "C" __declspec(dllimport);

#endif // decorators_h__
