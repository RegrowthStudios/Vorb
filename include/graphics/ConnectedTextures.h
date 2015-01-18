///
/// ConnectedTextures.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 24 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Utilities for connected textures
///

#pragma once

#ifndef ConnectedTextures_h__
#define ConnectedTextures_h__

namespace vorb {
    namespace core {
		namespace graphics {
            /// Helper functions for connected textures
            class ConnectedTextureHelper {
            public:
                /// Given a set of fully surrounding neighbors, find the corresponding connected texture index
                /// @param neighbors: 8 bits designating which sides/corners have neighbors
                /// @return Texture offset in range [0,46]
                static const size_t& getOffsetFull(const size_t& neighbors);
                /// Given a set of neighbors, find the corresponding connected texture index
                /// @param neighbors: 5 bits designating which sides/corners have neighbors
                /// @return Texture offset in range [0,8]
                static const size_t& getOffsetSmall(const size_t& neighbors);

                /// Initializes offsets array
                static void init();
            private:
                static size_t offsetsFull[256]; ///< Array of offsets indexed by neighbors (full connectivity)
                static size_t offsetsSmall[32]; ///< Array of offsets indexed by neighbors (partial connectivity)
            };
		}
    }
}
namespace vg = vorb::core::graphics;

#endif // ConnectedTextures_h__