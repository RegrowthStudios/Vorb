//
// decorators.h
// Vorb Engine
//
// Created by Cristian Zaloj on 16 Feb 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file decorators.h
 * 
 * \brief Function decorators for clarity.
 */

#pragma once

#ifndef Vorb_decorators_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_decorators_h__
//! @endcond

/*!
 * \def OUT
 * 
 * \brief This pointer/reference will be modified as an extra return value.
 */
#define OUT
/*!
 * \def OPT
 *
 * \brief This pointer is optional (may be null).
 */
#define OPT

/*!
 * \def CALLEE_DELETE
 *
 * \brief Function called will delete this resource.
 */
#define CALLEE_DELETE
/*!
 * \def CALLER_DELETE
 *
 * \brief Function caller must manage this resource.
 */
#define CALLER_DELETE

/*!
 * \def VORB_INTERNAL
 *
 * \brief Internal function not meant for an end-user.
 */
#define VORB_INTERNAL

/*!
 * \def UNIT_SPACE(SPACE_TYPE)
 *
 * \brief A macro that defines unit space for a piece of data.
 *
 * \param SPACE_TYPE The decorated value is found in <SPACE_TYPE> space.
 */
#define UNIT_SPACE(SPACE_TYPE)
/*!
 * \def UNIONIZE(M) struct
 *
 * \brief Encloses M in an unnamed struct so it can be used inside a union.
 *  
 *  Multiple members can be added and separated by commas. Also, access modifiers can be placed
 *  in, but it is not recommended.
 *
 * \param M The member(s) to enclose.
 */
#define UNIONIZE(M) struct { M; }

/*!
 * \def VORB_NON_COPYABLE(CLASS)
 *
 * \brief This class may not be copied, so all copy and assignment constructors are hidden.
 *
 * \param CLASS The class.
 */
#define VORB_NON_COPYABLE(CLASS) \
    CLASS(const CLASS& o) = delete; \
    CLASS& operator=(const CLASS& o) = delete

/*!
 * \def VORB_COPYABLE_DECL(CLASS)
 *
 * \brief Declaration for a copy constructor and operator overload. It may be used as an inline
 *  definition too.
 *
 * \param CLASS The class.
 */
#define VORB_COPYABLE_DECL(CLASS) \
    CLASS(const CLASS& o) { *this = o; } \
    CLASS& operator=(const CLASS& o)

/*!
 * \def VORB_COPYABLE_DEF(CLASS, VAR_NAME)
 *
 * \brief Definition for the copy operator overload, which is used in the copy constructor as well.
 *
 * \param CLASS The class.
 * \param VAR_NAME Name of the variable to be copied listed as a parameter in the function.
 */
#define VORB_COPYABLE_DEF(CLASS, VAR_NAME) \
    CLASS& CLASS::operator=(const CLASS& VAR_NAME)

/*!
 * \def VORB_NON_MOVABLE(CLASS)
 *
 * \brief This class may not be moved, so all move constructors and operators are hidden.
 *
 * \param CLASS The class.
 */
#define VORB_NON_MOVABLE(CLASS) \
    CLASS(CLASS&& o) = delete; \
    CLASS& operator=(CLASS&& o) = delete

/*!
 * \def VORB_MOVABLE_DECL(CLASS)
 *
 * \brief Declaration for a move constructor and operator overload. It may be used as an inline
 *  definition too.
 *
 * \param CLASS The class.
 */
#define VORB_MOVABLE_DECL(CLASS) \
    CLASS(CLASS&& o) { *this = std::move(o); } \
    CLASS& operator=(CLASS&& o)

/*!
 * \def VORB_MOVABLE_DEF(CLASS, VAR_NAME)
 *
 * \brief Definition for the move operator overload, which is used in the move constructor as well.
 *
 * \param CLASS The class.
 * \param VAR_NAME Name of the variable to be moved listed as a parameter in the function.
 */
#define VORB_MOVABLE_DEF(CLASS, VAR_NAME) \
    CLASS& CLASS::operator=(CLASS&& VAR_NAME)

#endif // !Vorb_decorators_h__
