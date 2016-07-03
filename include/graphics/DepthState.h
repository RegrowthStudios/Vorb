//
// DepthState.h
// Vorb Engine
//
// Created by Cristian Zaloj on 22 Jan 2015
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*!
 * \file DepthState.h
 * 
 * \brief Changes GPU depth functionality.
 */

#pragma once

#ifndef Vorb_DepthState_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_DepthState_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "GLEnums.h"

namespace vorb {
	namespace graphics {
		// Encapsulates Z-Buffer Operations
		class DepthState {
		public:
		    DepthState(bool read, DepthFunction depthFunction, bool write);
		    DepthState(bool read, GLenum depthFunction, bool write) :
		        DepthState(read, static_cast<DepthFunction>(depthFunction), write) {}
		
		    // Apply State In The Rendering Pipeline
		    void set() const;
		
		    // Z-Buffer Access
		    bool shouldRead;
		    bool shouldWrite;
		
		    // Comparison Against Destination Depth For Pixel Write/Discard
		    DepthFunction depthFunc;
		
		    // Always Draw Without Touching Depth
		    static const DepthState NONE;
		    // Only Draw When Depth Is Less Than Z-Buffer Without Modifying The Z-Buffer
		    static const DepthState READ;
		    // Always Draw And Overwrite The Z-Buffer With New Depth
		    static const DepthState WRITE;
		    // Only Draw When Depth Is Less Than Z-Buffer And Overwrite The Z-Buffer With New Depth
		    static const DepthState FULL;
		};
	}
}
namespace vg = vorb::graphics;

#endif // !Vorb_DepthState_h__
