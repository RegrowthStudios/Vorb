//
// ComboBoxScriptFuncs.h
// Vorb Engine
//
// Created by Benjamin Arnold on 11 May 2015
// Copyright 2014 Regrowth Studios
// MIT License
//

/*! \file ComboBoxScriptFuncs.h
* \brief Registers functions and consts for buttons to a script environment.
*/

#pragma once

#ifndef Vorb_ComboBoxScriptFuncs_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_ComboBoxScriptFuncs_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "Vorb/types.h"
#endif // !VORB_USING_PCH


#include "Vorb/VorbPreDecl.inl"
#include "Vorb/graphics/gtypes.h"

DECL_VG(class SpriteFont; enum class TextAlign)
DECL_VSCRIPT(template <typename EnvironmentImpl> class IEnvironment)

namespace vorb {
    namespace ui {
        // Forward Declarations
        class ComboBox;

        namespace ComboBoxScriptFuncs {
            template <typename ScriptEnvironmentImpl>
            void registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            template <typename ScriptEnvironmentImpl>
            void registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>* env);

            namespace impl {
                void addItem(ComboBox* comboBox, nString item);
                bool addItemAtIndex(ComboBox* comboBox, size_t index, nString item);
                bool removeItem(ComboBox* comboBox, nString item);
                bool removeItemAtIndex(ComboBox* comboBox, size_t index);
                bool selectItem(ComboBox* comboBox, nString item);
                bool selectItemAtIndex(ComboBox* comboBox, size_t index);

                bool isInDropBounds(ComboBox* comboBox, f32 x, f32 y);

                /************************************************************************/
                /* Getters                                                              */
                /************************************************************************/
                VGTexture getTexture(ComboBox* comboBox);
                const vorb::graphics::SpriteFont* getFont(ComboBox* comboBox);
                color4 getBackColor(ComboBox* comboBox);
                color4 getBackHoverColor(ComboBox* comboBox);
                color4 getTextColor(ComboBox* comboBox);
                color4 getTextHoverColor(ComboBox* comboBox);
                f32v2 getTextScale(ComboBox* comboBox);
                nString getItem(ComboBox* comboBox, size_t index);
                size_t getItemIndex(ComboBox* comboBox, nString item);
                size_t getItemCount(ComboBox* comboBox);
                vg::TextAlign getTextAlign(ComboBox* comboBox);
                nString getText(ComboBox* comboBox);
                f32 getMaxDropHeight(ComboBox* comboBox);

                /************************************************************************/
                /* Setters                                                              */
                /************************************************************************/
                void setFont(ComboBox* comboBox, vg::SpriteFont* font);
                void setMainButtonTexture(ComboBox* comboBox, VGTexture texture);
                void setDropBoxTexture(ComboBox* comboBox, VGTexture texture);
                void setDropButtonTexture(ComboBox* comboBox, VGTexture texture);
                void setDropButtonTextureOfItem(ComboBox* comboBox, VGTexture texture, nString item);
                void setDropButtonTextureAtIndex(ComboBox* comboBox, VGTexture texture, size_t index);
                void setBackColor(ComboBox* comboBox, color4 color);
                void setMainButtonBackColor(ComboBox* comboBox, color4 color);
                void setDropButtonBackColor(ComboBox* comboBox, color4 color);
                void setDropButtonBackColorOfItem(ComboBox* comboBox, color4 color, nString item);
                void setDropButtonBackColorAtIndex(ComboBox* comboBox, color4 color, size_t index);
                void setBackHoverColor(ComboBox* comboBox, color4 color);
                void setMainButtonBackHoverColor(ComboBox* comboBox, color4 color);
                void setDropButtonBackHoverColor(ComboBox* comboBox, color4 color);
                void setDropButtonBackHoverColorOfItem(ComboBox* comboBox, color4 color, nString item);
                void setDropButtonBackHoverColorAtIndex(ComboBox* comboBox, color4 color, size_t index);
                void setTextColor(ComboBox* comboBox, color4 color);
                void setMainButtonTextColor(ComboBox* comboBox, color4 color);
                void setDropButtonTextColor(ComboBox* comboBox, color4 color);
                void setDropButtonTextColorOfItem(ComboBox* comboBox, color4 color, nString item);
                void setDropButtonTextColorAtIndex(ComboBox* comboBox, color4 color, size_t index);
                void setTextHoverColor(ComboBox* comboBox, color4 color);
                void setMainButtonTextHoverColor(ComboBox* comboBox, color4 color);
                void setDropButtonTextHoverColor(ComboBox* comboBox, color4 color);
                void setDropButtonTextHoverColorOfItem(ComboBox* comboBox, color4 color, nString item);
                void setDropButtonTextHoverColorAtIndex(ComboBox* comboBox, color4 color, size_t index);
                void setTextScale(ComboBox* comboBox, f32v2 textScale);
                void setMainButtonTextScale(ComboBox* comboBox, f32v2 textScale);
                void setDropButtonTextScale(ComboBox* comboBox, f32v2 textScale);
                void setDropButtonTextScaleOfItem(ComboBox* comboBox, f32v2 textScale, nString item);
                void setDropButtonTextScaleAtIndex(ComboBox* comboBox, f32v2 textScale, size_t index);
                void setTextAlign(ComboBox* comboBox, vg::TextAlign align);
                void setMainButtonTextAlign(ComboBox* comboBox, vg::TextAlign align);
                void setDropButtonTextAlign(ComboBox* comboBox, vg::TextAlign align);
                void setDropButtonTextAlignOfItem(ComboBox* comboBox, vg::TextAlign align, nString item);
                void setDropButtonTextAlignAtIndex(ComboBox* comboBox, vg::TextAlign align, size_t index);
                void setText(ComboBox* comboBox, nString text);
                void setMaxDropHeight(ComboBox* comboBox, f32 maxDropHeight);

                // void onValueChange(Sender s, nString& v);
            }
        }
    }
}
namespace vui = vorb::ui;

#endif // !Vorb_ComboBoxScriptFuncs_h__
