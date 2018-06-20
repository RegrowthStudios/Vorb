//
// TextureRecycler.hpp
// Vorb Engine
//
// Created by Benjamin Arnold on 24 Dec 2014
// Copyright 2014 Regrowth Studios
// All Rights Reserved
//

/*! \file TextureRecycler.hpp
 * @brief TextureRecycler will recycle a specific type and size of texture.
 *
 * It is useful for terrain rendering for instance, when all textures are the same format and type.
 */

#pragma once

#ifndef Vorb_TextureRecycler_hpp__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_TextureRecycler_hpp__
//! @endcond

#ifndef VORB_USING_PCH
#include <set>
#include <vector>

#include "Vorb/types.h"
#endif // !VORB_USING_PCH

#include "graphics/gtypes.h"
#include "graphics/GpuMemory.h"

namespace vorb {
    namespace graphics {

        class TextureRecycler {
        public:
            /// Constructor
            /// @param width: Width of the textures
            /// @param height: Height of the textures
            /// @param samplingParameters: Sampling parameters for the textures
            /// @param mipmapLevels: Number of mipmap levels for the textures
            /// @param internalFormat: Internal pixel data format
            /// @param maxSize: Maximum number of textures to cache
            TextureRecycler(ui32 width,
                            ui32 height,
                            SamplerState* samplingParameters,
                            i32 mipmapLevels,
                            vg::TextureInternalFormat internalFormat,
                            ui32 maxSize = UINT_MAX) :
                        m_width(width),
                        m_height(height),
                        m_samplingParameters(samplingParameters),
                        m_mipmapLevels(mipmapLevels),
                        m_internalFormat(internalFormat),
                        m_maxSize(maxSize) {
                // Empty
            }
            ~TextureRecycler() {
                freeAll();
            }

            /// Get the number of cached textures
            size_t getNumTextures() const { return m_textures.size(); }

            /// Sets the max number of textures to keep in memory
            void setMaxSize(ui32 maxSize) { m_maxSize = maxSize; }

            /// Produces a texture, either a new one or a recycled one
            /// @return textureID.
            VGTexture produce(const ui8* pixels = nullptr,
                                vg::TextureFormat format = vg::TextureFormat::RGBA) {
                VGTexture rv;
                if (m_recycled.size()) {
                    rv = m_textures.back();
                    m_textures.pop_back();
                    m_recycled.erase(rv);
                } else {
                    rv = vg::GpuMemory::uploadTexture(pixels, m_width, m_height,
                                                      TexturePixelType::UNSIGNED_BYTE,
                                                      TextureTarget::TEXTURE_2D,
                                                      m_samplingParameters,
                                                      m_internalFormat,
                                                      format,
                                                      m_mipmapLevels);
                }
                return rv;
            }

            /// Will recycle the texture and delete it if there is no more
            /// room.
            void recycle(VGTexture t) {
                // Don't recycle twice.
                if (m_recycled.find(t) != m_recycled.end()) return;

                if (m_textures.size() < m_maxSize) {
                    m_recycled.insert(t);
                    m_textures.push_back(t);
                } else {
                    vg::GpuMemory::freeTexture(t);
                }
            }

            /// Frees all textures
            void freeAll() {
                for (auto& t : m_textures) {
                    vg::GpuMemory::freeTexture(t);
                }

                std::set<VGTexture>().swap(m_recycled);
                std::vector <VGTexture>().swap(m_textures);
            }
        private:
            ui32 m_width; ///< Width of the textures
            ui32 m_height; ///< Height of the texture
            SamplerState* m_samplingParameters; ///< Texture sampling parameters of the texture
            i32 m_mipmapLevels; ///< Texture mipmap levels

            std::set<VGTexture> m_recycled; ///< To check if a texture is already recycled

            std::vector<VGTexture> m_textures; ///< Cached textures

            vg::TextureInternalFormat m_internalFormat; ///< Internal pixel format for textures

            ui32 m_maxSize; ///< Maximum number of textures to hold
        };
    }
}
namespace vg = vorb::graphics;

#endif // !Vorb_TextureRecycler_hpp__
