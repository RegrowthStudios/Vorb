///
/// RTSwapChain.h
/// Vorb Engine
///
/// Created by Cristian Zaloj on 6 Nov 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// A series of FBOs that are swapped
///

#pragma once

#ifndef RTSwapChain_h__
#define RTSwapChain_h__

#include "GLRenderTarget.h"

namespace vorb {
    namespace graphics {
        /// A list of render targets
        template<int N>
        class RTSwapChain {
        public:
            /// Initialize all of the FBOs on the chain
            /// @param w: Width in pixels of each FBO
            /// @param h: Height in pixels of each FBO
            /// @param format: Internal pixel format for the color textures
            void init(ui32 w, ui32 h, TextureInternalFormat format = TextureInternalFormat::RGBA8) {
                for (i32 i = 0; i < N; i++) {
                    _fbos[i].setSize(w, h);
                    _fbos[i].init(format, 0);
                }
            }
            /// Dispose all of the FBOs on the chain
            void dispose() {
                for (i32 i = 0; i < N; i++) {
                    _fbos[i].dispose();
                }
            }

            /// Reset the swapchain with a fully rendered FBO as an input
            /// @param textureUnit: Texture unit placement for input FBO [0, MaxTextureUnits)
            /// @param rt: Rendered input FBO
            /// @param isMSAA: True if the input FBO has MSAA turned on
            /// @param shouldClear: True if the current target should have its color cleared
            void reset(ui32 textureUnit, VGFramebuffer fb, VGTexture rt, bool isMSAA, bool shouldClear = true) {
                _current = 0;
                if (isMSAA) {
                    ui32 w = getCurrent().getWidth();
                    ui32 h = getCurrent().getHeight();
                    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getCurrent().getID());
                    glBindFramebuffer(GL_READ_FRAMEBUFFER, fb);
                    glDrawBuffer(GL_COLOR_ATTACHMENT0);
                    glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);

                    swap();
                    rt = getPrevious().getTextureID();
                }

                getCurrent().use();
                if (shouldClear) glClear(GL_COLOR_BUFFER_BIT);
                glActiveTexture(GL_TEXTURE0 + textureUnit);
                glBindTexture(GL_TEXTURE_2D, rt);
                m_previousTexture = rt;
            }
            /// Move pointer to the next FBO on the chain
            void swap() {
                _current++;
                _current %= N;
                m_previousTexture = getPrevious().getTextureID();
            }
            /// Setup FBO IO via a designated texture unit
            /// @param textureUnit: Texture unit placement for previous FBO [0, MaxTextureUnits)
            /// @param shouldClear: True if the current FBO should have its color cleared
            void use(ui32 textureUnit, bool shouldClear = true) {
                getCurrent().use();
                if (shouldClear) glClear(GL_COLOR_BUFFER_BIT);

                bindPreviousTexture(textureUnit);
            }
            /// Unuse SwapChain and bind default framebuffer
            /// @param w: Width for setting viewport
            /// @param h: Height for setting viewport
            const void unuse(ui32 w, ui32 h) {
                getCurrent().unuse(w, h);
            }
            /*! @brief Binds the texture from the previous swapchain stage
             * 
             * @param textureUnit: Texture unit to bind the texture to 
             */
            void bindPreviousTexture(ui32 textureUnit) {
                glActiveTexture(GL_TEXTURE0 + textureUnit);
                glBindTexture(GL_TEXTURE_2D, m_previousTexture);
            }
            /// @return The current FBO (output)
            const GLRenderTarget& getCurrent() {
                return _fbos[_current];
            }
            /// @return The previous FBO (input)
            const GLRenderTarget& getPrevious() {
                return _fbos[(_current + N - 1) % N];
            }
        private:
            GLRenderTarget _fbos[N]; ///< A ring buffer of FBOs
            VGTexture m_previousTexture = 0;
            i32 _current = 0; ///< Pointer to the current output
        };
    }
}
namespace vg = vorb::graphics;

#endif // RTSwapChain_h__