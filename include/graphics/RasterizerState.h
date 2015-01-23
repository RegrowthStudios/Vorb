///
/// RasterizerState.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 22 Jan 2015
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Changes GPU polygon drawing and fill ops
///

#pragma once

#ifndef RasterizerState_h__
#define RasterizerState_h__

#include "GLEnums.h"

namespace vorb {
	namespace graphics {
		
		// Specify How Triangles Are Drawn (Specifically Which Are Culled)
		class RasterizerState {
		public:
		    RasterizerState(bool use, CullFaceMode cullFaceMode, FrontFaceDirection frontFaceDirection);
		
		    // Apply State In The Rendering Pipeline
		    void set() const;
		
		    bool useCulling;
		    CullFaceMode cullMode;
		    FrontFaceDirection faceOrientation;
		
		    static const RasterizerState CULL_NONE;
		    static const RasterizerState CULL_CLOCKWISE;
		    static const RasterizerState CULL_COUNTER_CLOCKWISE;
		};
	}
}
namespace vg = vorb::graphics;

#endif // RasterizerState_h__