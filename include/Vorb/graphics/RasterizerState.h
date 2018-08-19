//
// RasterizerState.h
// Vorb Engine
//
// Created by Cristian Zaloj on 22 Jan 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file RasterizerState.h
 * @brief Changes GPU polygon drawing and fill ops.
 */

#pragma once

#ifndef Vorb_RasterizerState_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_RasterizerState_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

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

#endif // !Vorb_RasterizerState_h__
