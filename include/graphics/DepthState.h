///
/// DepthState.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 22 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Changes GPU depth functionality
///

#pragma once

#ifndef DepthState_h__
#define DepthState_h__

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

#endif // DepthState_h__