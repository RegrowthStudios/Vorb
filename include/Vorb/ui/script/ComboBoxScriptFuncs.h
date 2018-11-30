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
#include "Vorb/script/IEnvironment.hpp"
#include "Vorb/ui/script/WidgetScriptFuncs.h"
#include "Vorb/ui/widgets/ComboBox.h"

DECL_VG(class SpriteFont; enum class TextAlign)

namespace vorb {
    namespace ui {
        // Forward Declarations
        class ComboBox;
        class Button;

        // TODO(Matthew): "OfItem" -> "ByName"?
        namespace ComboBoxScriptFuncs {
            using DropButtonComparator = Delegate<bool, size_t, Button*>;

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
                VGTexture getMainButtonTexture(ComboBox* comboBox);
                VGTexture getDropBoxTexture(ComboBox* comboBox);
                VGTexture getDropButtonTextureOfItem(ComboBox* comboBox, nString item);
                VGTexture getDropButtonTextureAtIndex(ComboBox* comboBox, size_t index);
                VGTexture getMainButtonHoverTexture(ComboBox* comboBox);
                VGTexture getDropBoxHoverTexture(ComboBox* comboBox);
                VGTexture getDropButtonHoverTextureOfItem(ComboBox* comboBox, nString item);
                VGTexture getDropButtonHoverTextureAtIndex(ComboBox* comboBox, size_t index);
                const vorb::graphics::SpriteFont* getMainButtonFont(ComboBox* comboBox);
                const vorb::graphics::SpriteFont* getDropButtonFontOfItem(ComboBox* comboBox, nString item);
                const vorb::graphics::SpriteFont* getDropButtonFontAtIndex(ComboBox* comboBox, size_t index);
                color4 getMainButtonBackColor(ComboBox* comboBox);
                color4 getDropBoxBackColor(ComboBox* comboBox);
                color4 getDropButtonBackColorOfItem(ComboBox* comboBox, nString item);
                color4 getDropButtonBackColorAtIndex(ComboBox* comboBox, size_t index);
                color4 getMainButtonBackHoverColor(ComboBox* comboBox);
                color4 getDropBoxBackHoverColor(ComboBox* comboBox);
                color4 getDropButtonBackHoverColorOfItem(ComboBox* comboBox, nString item);
                color4 getDropButtonBackHoverColorAtIndex(ComboBox* comboBox, size_t index);
                color4 getMainButtonTextColor(ComboBox* comboBox);
                color4 getDropButtonTextColorOfItem(ComboBox* comboBox, nString item);
                color4 getDropButtonTextColorAtIndex(ComboBox* comboBox, size_t index);
                color4 getMainButtonTextHoverColor(ComboBox* comboBox);
                color4 getDropButtonTextHoverColorOfItem(ComboBox* comboBox, nString item);
                color4 getDropButtonTextHoverColorAtIndex(ComboBox* comboBox, size_t index);
                f32v2 getMainButtonTextScale(ComboBox* comboBox);
                f32v2 getDropButtonTextScaleOfItem(ComboBox* comboBox, nString item);
                f32v2 getDropButtonTextScaleAtIndex(ComboBox* comboBox, size_t index);
                vg::TextAlign getMainButtonTextAlign(ComboBox* comboBox);
                vg::TextAlign getDropButtonTextAlignOfItem(ComboBox* comboBox, nString item);
                vg::TextAlign getDropButtonTextAlignAtIndex(ComboBox* comboBox, size_t index);
                nString getItem(ComboBox* comboBox, size_t index);
                size_t getItemIndex(ComboBox* comboBox, nString item);
                size_t getItemCount(ComboBox* comboBox);
                nString getText(ComboBox* comboBox);
                f32 getMaxDropHeight(ComboBox* comboBox);

                /************************************************************************/
                /* Setters                                                              */
                /************************************************************************/
                void setFont(ComboBox* comboBox, vg::SpriteFont* font);
                void setMainButtonFont(ComboBox* comboBox, vg::SpriteFont* font);
                void setDropButtonFont(ComboBox* comboBox, vg::SpriteFont* font);
                void setDropButtonFontOfItem(ComboBox* comboBox, vg::SpriteFont* font, nString item);
                void setDropButtonFontAtIndex(ComboBox* comboBox, vg::SpriteFont* font, size_t index);
                void setDropButtonFontByCompare(ComboBox* comboBox, vg::SpriteFont* font, DropButtonComparator comparator);
                void setMainButtonTexture(ComboBox* comboBox, VGTexture texture);
                void setDropBoxTexture(ComboBox* comboBox, VGTexture texture);
                void setDropButtonTexture(ComboBox* comboBox, VGTexture texture);
                void setDropButtonTextureOfItem(ComboBox* comboBox, VGTexture texture, nString item);
                void setDropButtonTextureAtIndex(ComboBox* comboBox, VGTexture texture, size_t index);
                void setDropButtonTextureByCompare(ComboBox* comboBox, VGTexture texture, DropButtonComparator comparator);
                void setMainButtonHoverTexture(ComboBox* comboBox, VGTexture texture);
                void setDropBoxHoverTexture(ComboBox* comboBox, VGTexture texture);
                void setDropButtonHoverTexture(ComboBox* comboBox, VGTexture texture);
                void setDropButtonHoverTextureOfItem(ComboBox* comboBox, VGTexture texture, nString item);
                void setDropButtonHoverTextureAtIndex(ComboBox* comboBox, VGTexture texture, size_t index);
                void setDropButtonHoverTextureByCompare(ComboBox* comboBox, VGTexture texture, DropButtonComparator comparator);
                void setBackColor(ComboBox* comboBox, color4 color);
                void setMainButtonBackColor(ComboBox* comboBox, color4 color);
                void setDropBoxBackColor(ComboBox* comboBox, color4 color);
                void setDropButtonBackColor(ComboBox* comboBox, color4 color);
                void setDropButtonBackColorOfItem(ComboBox* comboBox, color4 color, nString item);
                void setDropButtonBackColorAtIndex(ComboBox* comboBox, color4 color, size_t index);
                void setDropButtonBackColorByCompare(ComboBox* comboBox, color4 color, DropButtonComparator comparator);
                void setBackHoverColor(ComboBox* comboBox, color4 color);
                void setMainButtonBackHoverColor(ComboBox* comboBox, color4 color);
                void setDropBoxBackHoverColor(ComboBox* comboBox, color4 color);
                void setDropButtonBackHoverColor(ComboBox* comboBox, color4 color);
                void setDropButtonBackHoverColorOfItem(ComboBox* comboBox, color4 color, nString item);
                void setDropButtonBackHoverColorAtIndex(ComboBox* comboBox, color4 color, size_t index);
                void setDropButtonBackHoverColorByCompare(ComboBox* comboBox, color4 color, DropButtonComparator comparator);
                void setTextColor(ComboBox* comboBox, color4 color);
                void setMainButtonTextColor(ComboBox* comboBox, color4 color);
                void setDropButtonTextColor(ComboBox* comboBox, color4 color);
                void setDropButtonTextColorOfItem(ComboBox* comboBox, color4 color, nString item);
                void setDropButtonTextColorAtIndex(ComboBox* comboBox, color4 color, size_t index);
                void setDropButtonTextColorByCompare(ComboBox* comboBox, color4 color, DropButtonComparator comparator);
                void setTextHoverColor(ComboBox* comboBox, color4 color);
                void setMainButtonTextHoverColor(ComboBox* comboBox, color4 color);
                void setDropButtonTextHoverColor(ComboBox* comboBox, color4 color);
                void setDropButtonTextHoverColorOfItem(ComboBox* comboBox, color4 color, nString item);
                void setDropButtonTextHoverColorAtIndex(ComboBox* comboBox, color4 color, size_t index);
                void setDropButtonTextHoverColorByCompare(ComboBox* comboBox, color4 color, DropButtonComparator comparator);
                void setTextScale(ComboBox* comboBox, f32v2 textScale);
                void setMainButtonTextScale(ComboBox* comboBox, f32v2 textScale);
                void setDropButtonTextScale(ComboBox* comboBox, f32v2 textScale);
                void setDropButtonTextScaleOfItem(ComboBox* comboBox, f32v2 textScale, nString item);
                void setDropButtonTextScaleAtIndex(ComboBox* comboBox, f32v2 textScale, size_t index);
                void setDropButtonTextScaleByCompare(ComboBox* comboBox, f32v2 textScale, DropButtonComparator comparator);
                void setTextAlign(ComboBox* comboBox, vg::TextAlign align);
                void setMainButtonTextAlign(ComboBox* comboBox, vg::TextAlign align);
                void setDropButtonTextAlign(ComboBox* comboBox, vg::TextAlign align);
                void setDropButtonTextAlignOfItem(ComboBox* comboBox, vg::TextAlign align, nString item);
                void setDropButtonTextAlignAtIndex(ComboBox* comboBox, vg::TextAlign align, size_t index);
                void setDropButtonTextAlignByCompare(ComboBox* comboBox, vg::TextAlign align, DropButtonComparator comparator);
                void setText(ComboBox* comboBox, nString text);
                void setMaxDropHeight(ComboBox* comboBox, f32 maxDropHeight);

                // void onValueChange(Sender s, nString& v);
            }
        }
    }
}
namespace vui = vorb::ui;

template <typename ScriptEnvironmentImpl>
void vui::ComboBoxScriptFuncs::registerFuncs(const nString& namespace_, vscript::IEnvironment<ScriptEnvironmentImpl>* env) {
    env->setNamespaces("UI", namespace_);
    env->addCDelegate("addItem",           makeDelegate(&impl::addItem));
    env->addCDelegate("addItemAtIndex",    makeDelegate(&impl::addItemAtIndex));
    env->addCDelegate("removeItem",        makeDelegate(&impl::removeItem));
    env->addCDelegate("removeItemAtIndex", makeDelegate(&impl::removeItemAtIndex));
    env->addCDelegate("selectItem",        makeDelegate(&impl::selectItem));
    env->addCDelegate("selectItemAtIndex", makeDelegate(&impl::selectItemAtIndex));

    env->addCDelegate("isInDropBounds", makeDelegate(&impl::isInDropBounds));

    env->addCDelegate("getMainButtonTexture",               makeDelegate(&impl::getMainButtonTexture));
    env->addCDelegate("getDropBoxTexture",                  makeDelegate(&impl::getDropBoxTexture));
    env->addCDelegate("getDropButtonTextureOfItem",         makeDelegate(&impl::getDropButtonTextureOfItem));
    env->addCDelegate("getDropButtonTextureAtIndex",        makeDelegate(&impl::getDropButtonTextureAtIndex));
    env->addCDelegate("getDropBoxHoverTexture",             makeDelegate(&impl::getDropBoxHoverTexture));
    env->addCDelegate("getDropButtonHoverTextureOfItem",    makeDelegate(&impl::getDropButtonHoverTextureOfItem));
    env->addCDelegate("getDropButtonHoverTextureAtIndex",   makeDelegate(&impl::getDropButtonHoverTextureAtIndex));
    env->addCDelegate("getMainButtonFont",                  makeDelegate(&impl::getMainButtonFont));
    env->addCDelegate("getDropButtonFontOfItem",            makeDelegate(&impl::getDropButtonFontOfItem));
    env->addCDelegate("getDropButtonFontAtIndex",           makeDelegate(&impl::getDropButtonFontAtIndex));
    env->addCDelegate("getMainButtonBackColor",             makeDelegate(&impl::getMainButtonBackColor));
    env->addCDelegate("getDropBoxBackColor",                makeDelegate(&impl::getDropBoxBackColor));
    env->addCDelegate("getDropButtonBackColorOfItem",       makeDelegate(&impl::getDropButtonBackColorOfItem));
    env->addCDelegate("getDropButtonBackColorAtIndex",      makeDelegate(&impl::getDropButtonBackColorAtIndex));
    env->addCDelegate("getMainButtonBackHoverColor",        makeDelegate(&impl::getMainButtonBackHoverColor));
    env->addCDelegate("getDropBoxBackHoverColor",           makeDelegate(&impl::getDropBoxBackHoverColor));
    env->addCDelegate("getDropButtonBackHoverColorOfItem",  makeDelegate(&impl::getDropButtonBackHoverColorOfItem));
    env->addCDelegate("getDropButtonBackHoverColorAtIndex", makeDelegate(&impl::getDropButtonBackHoverColorAtIndex));
    env->addCDelegate("getMainButtonTextColor",             makeDelegate(&impl::getMainButtonTextColor));
    env->addCDelegate("getDropButtonTextColorOfItem",       makeDelegate(&impl::getDropButtonTextColorOfItem));
    env->addCDelegate("getDropButtonTextColorAtIndex",      makeDelegate(&impl::getDropButtonTextColorAtIndex));
    env->addCDelegate("getMainButtonTextHoverColor",        makeDelegate(&impl::getMainButtonTextHoverColor));
    env->addCDelegate("getDropButtonTextHoverColorOfItem",  makeDelegate(&impl::getDropButtonTextHoverColorOfItem));
    env->addCDelegate("getDropButtonTextHoverColorAtIndex", makeDelegate(&impl::getDropButtonTextHoverColorAtIndex));
    env->addCDelegate("getMainButtonTextScale",             makeDelegate(&impl::getMainButtonTextScale));
    env->addCDelegate("getDropButtonTextScaleOfItem",       makeDelegate(&impl::getDropButtonTextScaleOfItem));
    env->addCDelegate("getDropButtonTextScaleAtIndex",      makeDelegate(&impl::getDropButtonTextScaleAtIndex));
    env->addCDelegate("getMainButtonTextAlign",             makeDelegate(&impl::getMainButtonTextAlign));
    env->addCDelegate("getDropButtonTextAlignOfItem",       makeDelegate(&impl::getDropButtonTextAlignOfItem));
    env->addCDelegate("getDropButtonTextAlignAtIndex",      makeDelegate(&impl::getDropButtonTextAlignAtIndex));
    env->addCDelegate("getItem",                            makeDelegate(&impl::getItem));
    env->addCDelegate("getItemIndex",                       makeDelegate(&impl::getItemIndex));
    env->addCDelegate("getItemCount",                       makeDelegate(&impl::getItemCount));
    env->addCDelegate("getText",                            makeDelegate(&impl::getText));
    env->addCDelegate("getMaxDropHeight",                   makeDelegate(&impl::getMaxDropHeight));

    env->addCDelegate("setFont",                             makeDelegate(&impl::setFont));
    env->addCDelegate("setMainButtonFont",                   makeDelegate(&impl::setMainButtonFont));
    env->addCDelegate("setDropButtonFont",                   makeDelegate(&impl::setDropButtonFont));
    env->addCDelegate("setDropButtonFontOfItem",             makeDelegate(&impl::setDropButtonFontOfItem));
    env->addCDelegate("setDropButtonFontAtIndex",            makeDelegate(&impl::setDropButtonFontAtIndex));
    env->addCDelegate("setDropButtonFontByCompare",          makeFunctor([env] (ComboBox* comboBox, vg::SpriteFont* font, nString compareName) {
        DropButtonComparator* compare = env->template getScriptDelegate<bool, size_t, Button*>(compareName);

        if (!compare) return;

        impl::setDropButtonFontByCompare(comboBox, font, *compare);
    }));
    env->addCDelegate("setMainButtonTexture",                 makeDelegate(&impl::setMainButtonTexture));
    env->addCDelegate("setDropBoxTexture",                    makeDelegate(&impl::setDropBoxTexture));
    env->addCDelegate("setDropButtonTexture",                 makeDelegate(&impl::setDropButtonTexture));
    env->addCDelegate("setDropButtonTextureOfItem",           makeDelegate(&impl::setDropButtonTextureOfItem));
    env->addCDelegate("setDropButtonTextureAtIndex",          makeDelegate(&impl::setDropButtonTextureAtIndex));
    env->addCDelegate("setDropButtonTextureByCompare",        makeFunctor([env] (ComboBox* comboBox, VGTexture texture, nString compareName) {
        DropButtonComparator* compare = env->template getScriptDelegate<bool, size_t, Button*>(compareName);

        if (!compare) return;

        impl::setDropButtonTextureByCompare(comboBox, texture, *compare);
    }));
    env->addCDelegate("setMainButtonHoverTexture",            makeDelegate(&impl::setMainButtonHoverTexture));
    env->addCDelegate("setDropBoxHoverTexture",               makeDelegate(&impl::setDropBoxHoverTexture));
    env->addCDelegate("setDropButtonHoverTexture",            makeDelegate(&impl::setDropButtonHoverTexture));
    env->addCDelegate("setDropButtonHoverTextureOfItem",      makeDelegate(&impl::setDropButtonHoverTextureOfItem));
    env->addCDelegate("setDropButtonHoverTextureAtIndex",     makeDelegate(&impl::setDropButtonHoverTextureAtIndex));
    env->addCDelegate("setDropButtonHoverTextureByCompare",   makeFunctor([env] (ComboBox* comboBox, VGTexture texture, nString compareName) {
        DropButtonComparator* compare = env->template getScriptDelegate<bool, size_t, Button*>(compareName);

        if (!compare) return;

        impl::setDropButtonHoverTextureByCompare(comboBox, texture, *compare);
    }));
    env->addCDelegate("setBackColor",                         makeDelegate(&impl::setBackColor));
    env->addCDelegate("setMainButtonBackColor",               makeDelegate(&impl::setMainButtonBackColor));
    env->addCDelegate("setDropBoxBackColor",                  makeDelegate(&impl::setDropBoxBackColor));
    env->addCDelegate("setDropButtonBackColor",               makeDelegate(&impl::setDropButtonBackColor));
    env->addCDelegate("setDropButtonBackColorOfItem",         makeDelegate(&impl::setDropButtonBackColorOfItem));
    env->addCDelegate("setDropButtonBackColorAtIndex",        makeDelegate(&impl::setDropButtonBackColorAtIndex));
    env->addCDelegate("setDropButtonBackColorByCompare",      makeFunctor([env] (ComboBox* comboBox, color4 color, nString compareName) {
        DropButtonComparator* compare = env->template getScriptDelegate<bool, size_t, Button*>(compareName);

        if (!compare) return;

        impl::setDropButtonBackColorByCompare(comboBox, color, *compare);
    }));
    env->addCDelegate("setBackHoverColor",                    makeDelegate(&impl::setBackHoverColor));
    env->addCDelegate("setMainButtonBackHoverColor",          makeDelegate(&impl::setMainButtonBackHoverColor));
    env->addCDelegate("setDropBoxBackHoverColor",             makeDelegate(&impl::setDropBoxBackHoverColor));
    env->addCDelegate("setDropButtonBackHoverColor",          makeDelegate(&impl::setDropButtonBackHoverColor));
    env->addCDelegate("setDropButtonBackHoverColorOfItem",    makeDelegate(&impl::setDropButtonBackHoverColorOfItem));
    env->addCDelegate("setDropButtonBackHoverColorAtIndex",   makeDelegate(&impl::setDropButtonBackHoverColorAtIndex));
    env->addCDelegate("setDropButtonBackHoverColorByCompare", makeFunctor([env] (ComboBox* comboBox, color4 color, nString compareName) {
        DropButtonComparator* compare = env->template getScriptDelegate<bool, size_t, Button*>(compareName);

        if (!compare) return;

        impl::setDropButtonBackHoverColorByCompare(comboBox, color, *compare);
    }));
    env->addCDelegate("setTextColor",                         makeDelegate(&impl::setTextColor));
    env->addCDelegate("setMainButtonTextColor",               makeDelegate(&impl::setMainButtonTextColor));
    env->addCDelegate("setDropButtonTextColor",               makeDelegate(&impl::setDropButtonTextColor));
    env->addCDelegate("setDropButtonTextColorOfItem",         makeDelegate(&impl::setDropButtonTextColorOfItem));
    env->addCDelegate("setDropButtonTextColorAtIndex",        makeDelegate(&impl::setDropButtonTextColorAtIndex));
    env->addCDelegate("setDropButtonTextColorByCompare",      makeFunctor([env] (ComboBox* comboBox, color4 color, nString compareName) {
        DropButtonComparator* compare = env->template getScriptDelegate<bool, size_t, Button*>(compareName);

        if (!compare) return;

        impl::setDropButtonTextColorByCompare(comboBox, color, *compare);
    }));
    env->addCDelegate("setTextHoverColor",                    makeDelegate(&impl::setTextHoverColor));
    env->addCDelegate("setMainButtonTextHoverColor",          makeDelegate(&impl::setMainButtonTextHoverColor));
    env->addCDelegate("setDropButtonTextHoverColor",          makeDelegate(&impl::setDropButtonTextHoverColor));
    env->addCDelegate("setDropButtonTextHoverColorOfItem",    makeDelegate(&impl::setDropButtonTextHoverColorOfItem));
    env->addCDelegate("setDropButtonTextHoverColorAtIndex",   makeDelegate(&impl::setDropButtonTextHoverColorAtIndex));
    env->addCDelegate("setDropButtonTextHoverColorByCompare", makeFunctor([env] (ComboBox* comboBox, color4 color, nString compareName) {
        DropButtonComparator* compare = env->template getScriptDelegate<bool, size_t, Button*>(compareName);

        if (!compare) return;

        impl::setDropButtonTextHoverColorByCompare(comboBox, color, *compare);
    }));
    env->addCDelegate("setTextScale",                         makeDelegate(&impl::setTextScale));
    env->addCDelegate("setMainButtonTextScale",               makeDelegate(&impl::setMainButtonTextScale));
    env->addCDelegate("setDropButtonTextScale",               makeDelegate(&impl::setDropButtonTextScale));
    env->addCDelegate("setDropButtonTextScaleOfItem",         makeDelegate(&impl::setDropButtonTextScaleOfItem));
    env->addCDelegate("setDropButtonTextScaleAtIndex",        makeDelegate(&impl::setDropButtonTextScaleAtIndex));
    env->addCDelegate("setDropButtonTextScaleByCompare",      makeFunctor([env] (ComboBox* comboBox, f32v2 scale, nString compareName) {
        DropButtonComparator* compare = env->template getScriptDelegate<bool, size_t, Button*>(compareName);

        if (!compare) return;

        impl::setDropButtonTextScaleByCompare(comboBox, scale, *compare);
    }));
    env->addCDelegate("setTextAlign",                         makeDelegate(&impl::setTextAlign));
    env->addCDelegate("setMainButtonTextAlign",               makeDelegate(&impl::setMainButtonTextAlign));
    env->addCDelegate("setDropButtonTextAlign",               makeDelegate(&impl::setDropButtonTextAlign));
    env->addCDelegate("setDropButtonTextAlignOfItem",         makeDelegate(&impl::setDropButtonTextAlignOfItem));
    env->addCDelegate("setDropButtonTextAlignAtIndex",        makeDelegate(&impl::setDropButtonTextAlignAtIndex));
    env->addCDelegate("setDropButtonTextAlignByCompare",      makeFunctor([env] (ComboBox* comboBox, vg::TextAlign align, nString compareName) {
        DropButtonComparator* compare = env->template getScriptDelegate<bool, size_t, Button*>(compareName);

        if (!compare) return;

        impl::setDropButtonTextAlignByCompare(comboBox, align, *compare);
    }));
    env->addCDelegate("setText",                              makeDelegate(&impl::setText));
    env->addCDelegate("setMaxDropHeight",                     makeDelegate(&impl::setMaxDropHeight));

    // TODO(Matthew): Need to give ownership of these delegates to someone in order to not end up leaking them.
    env->addCDelegate("onValueChange", makeFunctor([=](ComboBox* comboBox, nString name) {
        auto del = env->template getScriptDelegate<void, Sender, const nString&>(name);

        comboBox->ValueChange.add(*del, true);

        del->neuter();
        delete del;
    }));
    env->setNamespaces();

    WidgetScriptFuncs::registerFuncs(namespace_, env);
}

template <typename ScriptEnvironmentImpl>
void vui::ComboBoxScriptFuncs::registerConsts(vscript::IEnvironment<ScriptEnvironmentImpl>*) {
    // Empty
}

#endif // !Vorb_ComboBoxScriptFuncs_h__
