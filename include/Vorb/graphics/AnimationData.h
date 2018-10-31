//
// AnimationData.h
// Vorb Engine
//
// Created by Cristian Zaloj on 3 Mar 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file AnimationData.h
 * @brief 
 */

#pragma once

#ifndef Vorb_AnimationData_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_AnimationData_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "../types.h"
#endif // !VORB_USING_PCH

#include "../io/Keg.h"

namespace vorb {
    namespace graphics {
        struct BoneTransform {
        public:
            f32q rotation;
            f32v3 translation;
        };
        KEG_TYPE_DECL(VG_BoneTransform);

        struct Keyframe {
        public:
            i32 frame;
            BoneTransform transform;
        };
        KEG_TYPE_DECL(VG_KeyFrame);

        struct BoneInformation {
        public:
            nString name;
            nString parent;

            BoneTransform rest;
            Array<Keyframe> keyframes;
        };
        KEG_TYPE_DECL(VG_BoneInformation);

        struct Bone {
        public:
            ui32 index;
            nString name;
            Bone* parent;
            Bone** children;
            size_t numChildren;

            BoneTransform rest;
            Keyframe* keyframes;
            size_t numFrames;

        };
        struct Skeleton {
        public:
            Bone* bones;
            size_t numBones;
            
            Keyframe* frames;
            size_t numFrames;

            Bone** childrenArray;
            size_t numChildrenReferences;
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_AnimationData_h__
