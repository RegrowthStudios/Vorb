#include "Vorb/stdafx.h"
#include "Vorb/ui/widgets/yaml/ComboBox.h"

#include "Vorb/graphics/FontCache.h"
#include "Vorb/graphics/TextureCache.h"
#include "Vorb/ui/widgets/ComboBox.h"
#include "Vorb/ui/widgets/yaml/Parser.h"
#include "Vorb/ui/widgets/yaml/Widget.h"

bool vui::parseComboBoxEntry(keg::ReadContext& context, vui::ComboBox* comboBox, const nString& name, keg::Node value, Delegate<vui::IWidget*, const nString&, keg::Node>* widgetParser, vg::FontCache* fontCache, vg::TextureCache* textureCache) {
    if (name == "font") {
        FontDescriptor fontDescriptor;
        if (!parseFont(*value, fontDescriptor)) return false;

        vg::SpriteFont* font = fontCache->getFont(fontDescriptor.filepath, fontDescriptor.size, fontDescriptor.startingCharacter, fontDescriptor.endingCharacter);

        comboBox->setFont(font);
    } else if (name == "main_button_font") {
        FontDescriptor fontDescriptor;
        if (!parseFont(*value, fontDescriptor)) return false;

        vg::SpriteFont* font = fontCache->getFont(fontDescriptor.filepath, fontDescriptor.size, fontDescriptor.startingCharacter, fontDescriptor.endingCharacter);

        comboBox->setMainButtonFont(font);
    } else if (name == "drop_button_font") {
        FontDescriptor fontDescriptor;
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["font"]) {
            if (!parseFont({ value->data["font"] }, fontDescriptor)) return false;

            vg::SpriteFont* font = fontCache->getFont(fontDescriptor.filepath, fontDescriptor.size, fontDescriptor.startingCharacter, fontDescriptor.endingCharacter);

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonFont(font, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonFont(font, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            if (!parseFont(*value, fontDescriptor)) return false;

            vg::SpriteFont* font = fontCache->getFont(fontDescriptor.filepath, fontDescriptor.size, fontDescriptor.startingCharacter, fontDescriptor.endingCharacter);

            comboBox->setDropButtonFont(font);
        }
    }
    if (name == "hover_font") {
        FontDescriptor fontDescriptor;
        if (!parseFont(*value, fontDescriptor)) return false;

        vg::SpriteFont* font = fontCache->getFont(fontDescriptor.filepath, fontDescriptor.size, fontDescriptor.startingCharacter, fontDescriptor.endingCharacter);

        comboBox->setHoverFont(font);
    } else if (name == "main_button_hover_font") {
        FontDescriptor fontDescriptor;
        if (!parseFont(*value, fontDescriptor)) return false;

        vg::SpriteFont* font = fontCache->getFont(fontDescriptor.filepath, fontDescriptor.size, fontDescriptor.startingCharacter, fontDescriptor.endingCharacter);

        comboBox->setMainButtonHoverFont(font);
    } else if (name == "drop_button_hover_font") {
        FontDescriptor fontDescriptor;
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["font"]) {
            if (!parseFont({ value->data["font"] }, fontDescriptor)) return false;

            vg::SpriteFont* font = fontCache->getFont(fontDescriptor.filepath, fontDescriptor.size, fontDescriptor.startingCharacter, fontDescriptor.endingCharacter);

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonHoverFont(font, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonHoverFont(font, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            if (!parseFont(*value, fontDescriptor)) return false;

            vg::SpriteFont* font = fontCache->getFont(fontDescriptor.filepath, fontDescriptor.size, fontDescriptor.startingCharacter, fontDescriptor.endingCharacter);

            comboBox->setDropButtonHoverFont(font);
        }
    } else if (name == "main_button_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        comboBox->setMainButtonTexture(tex.id);
    } else if (name == "drop_box_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        comboBox->setDropBoxTexture(tex.id);
    } else if (name == "drop_button_texture") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["tex"]) {
            nString texturePath;
            if (!parseValue({ value->data["tex"] }, texturePath)) return false;

            vg::Texture tex = textureCache->addTexture(texturePath);

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonTexture(tex.id, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonTexture(tex.id, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            nString texturePath;
            if (!parseValue(*value, texturePath)) return false;

            vg::Texture tex = textureCache->addTexture(texturePath);

            comboBox->setDropButtonTexture(tex.id);
        }

        return true;
    } else if (name == "main_button_hover_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        comboBox->setMainButtonHoverTexture(tex.id);
    } else if (name == "drop_box_hover_texture") {
        nString texturePath;
        if (!parseValue(*value, texturePath)) return false;

        vg::Texture tex = textureCache->addTexture(texturePath);

        comboBox->setDropBoxHoverTexture(tex.id);
    } else if (name == "drop_button_hover_texture") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["tex"]) {
            nString texturePath;
            if (!parseValue({ value->data["tex"] }, texturePath)) return false;

            vg::Texture tex = textureCache->addTexture(texturePath);

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonHoverTexture(tex.id, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonHoverTexture(tex.id, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            nString texturePath;
            if (!parseValue(*value, texturePath)) return false;

            vg::Texture tex = textureCache->addTexture(texturePath);

            comboBox->setDropButtonHoverTexture(tex.id);
        }

        return true;
    } else if (name == "bg_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        comboBox->setBackColor(color);
    } else if (name == "main_button_bg_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        comboBox->setMainButtonBackColor(color);
    } else if (name == "drop_box_bg_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        comboBox->setDropBoxBackColor(color);
    } else if (name == "drop_button_bg_color") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["color"]) {
            color4 color;
            if (!parseColor({ value->data["color"] }, color)) return false;

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonBackColor(color, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonBackColor(color, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            color4 color;
            if (!parseColor(*value, color)) return false;

            comboBox->setDropButtonBackColor(color);
        }

        return true;
    } else if (name == "bg_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        comboBox->setBackHoverColor(color);
    } else if (name == "main_button_bg_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        comboBox->setMainButtonBackHoverColor(color);
    } else if (name == "drop_box_bg_hover_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        comboBox->setDropBoxBackHoverColor(color);
    } else if (name == "drop_button_bg_hover_color") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["color"]) {
            color4 color;
            if (!parseColor({ value->data["color"] }, color)) return false;

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonBackHoverColor(color, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonBackHoverColor(color, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            color4 color;
            if (!parseColor(*value, color)) return false;

            comboBox->setDropButtonBackHoverColor(color);
        }

        return true;
    } else if (name == "text_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        comboBox->setTextColor(color);
    } else if (name == "main_button_text_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        comboBox->setMainButtonTextColor(color);
    } else if (name == "drop_button_text_color") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["color"]) {
            color4 color;
            if (!parseColor({ value->data["color"] }, color)) return false;

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonTextColor(color, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonTextColor(color, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            color4 color;
            if (!parseColor(*value, color)) return false;

            comboBox->setDropButtonTextColor(color);
        }

        return true;
    } else if (name == "hover_text_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        comboBox->setHoverTextColor(color);
    } else if (name == "main_button_hover_text_color") {
        color4 color;
        if (!parseColor(*value, color)) return false;

        comboBox->setMainButtonHoverTextColor(color);
    } else if (name == "drop_button_hover_text_color") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["color"]) {
            color4 color;
            if (!parseColor({ value->data["color"] }, color)) return false;

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonHoverTextColor(color, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonHoverTextColor(color, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            color4 color;
            if (!parseColor(*value, color)) return false;

            comboBox->setDropButtonHoverTextColor(color);
        }

        return true;
    } else if (name == "text_scale") {
        f32v2 scale;
        if (!parseVec2(*value, scale)) return false;

        comboBox->setTextScale(scale);
    } else if (name == "main_button_text_scale") {
        f32v2 scale;
        if (!parseVec2(*value, scale)) return false;

        comboBox->setMainButtonTextScale(scale);
    } else if (name == "drop_button_text_scale") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["scale"]) {
            f32v2 scale;
            if (!parseVec2({ value->data["scale"] }, scale)) return false;

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonTextScale(scale, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonTextScale(scale, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            f32v2 scale;
            if (!parseVec2(*value, scale)) return false;

            comboBox->setDropButtonTextScale(scale);
        }

        return true;
    } else if (name == "text_hover_scale") {
        f32v2 scale;
        if (!parseVec2(*value, scale)) return false;

        comboBox->setHoverTextScale(scale);
    } else if (name == "main_button_text_hover_scale") {
        f32v2 scale;
        if (!parseVec2(*value, scale)) return false;

        comboBox->setMainButtonHoverTextScale(scale);
    } else if (name == "drop_button_text_hover_scale") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["scale"]) {
            f32v2 scale;
            if (!parseVec2({ value->data["scale"] }, scale)) return false;

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonHoverTextScale(scale, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonHoverTextScale(scale, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            f32v2 scale;
            if (!parseVec2(*value, scale)) return false;

            comboBox->setDropButtonHoverTextScale(scale);
        }

        return true;
    } else if (name == "text_align") {
        vg::TextAlign align;
        if (!parseTextAlign(*value, align)) return false;

        comboBox->setTextAlign(align);
    } else if (name == "main_button_text_align") {
        vg::TextAlign align;
        if (!parseTextAlign(*value, align)) return false;

        comboBox->setMainButtonTextAlign(align);
    } else if (name == "drop_button_text_align") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["text_align"]) {
            vg::TextAlign textAlign;
            if (!parseTextAlign({ value->data["text_align"] }, textAlign)) return false;

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonTextAlign(textAlign, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonTextAlign(textAlign, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            vg::TextAlign textAlign;
            if (!parseTextAlign(*value, textAlign)) return false;

            comboBox->setDropButtonTextAlign(textAlign);
        }

        return true;
    } else if (name == "hover_text_align") {
        vg::TextAlign align;
        if (!parseTextAlign(*value, align)) return false;

        comboBox->setHoverTextAlign(align);
    } else if (name == "main_button_hover_text_align") {
        vg::TextAlign align;
        if (!parseTextAlign(*value, align)) return false;

        comboBox->setMainButtonHoverTextAlign(align);
    } else if (name == "drop_button_hover_text_align") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["text_align"]) {
            vg::TextAlign textAlign;
            if (!parseTextAlign({ value->data["text_align"] }, textAlign)) return false;

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonHoverTextAlign(textAlign, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonHoverTextAlign(textAlign, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            vg::TextAlign textAlign;
            if (!parseTextAlign(*value, textAlign)) return false;

            comboBox->setDropButtonHoverTextAlign(textAlign);
        }

        return true;
    } else if (name == "main_button_text") {
        nString text;
        if (!parseValue(*value, text)) return false;

        comboBox->setMainButtonText(text);
    } else if (name == "drop_button_text") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["text"]) {
            nString text;
            if (!parseValue({ value->data["text"] }, text)) return false;

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonText(text, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonText(text, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            nString text;
            if (!parseValue(*value, text)) return false;

            comboBox->setDropButtonText(text);
        }

        return true;
    } else if (name == "main_button_hover_text") {
        nString text;
        if (!parseValue(*value, text)) return false;

        comboBox->setMainButtonHoverText(text);
    } else if (name == "drop_button_hover_text") {
        if (keg::getType(value) == keg::NodeType::MAP
                && value->data.size() == 2
                    && value->data["text_align"]) {
            nString text;
            if (!parseValue({ value->data["text_align"] }, text)) return false;

            if (value->data["item"]) {
                nString item;
                if (!parseValue({ value->data["item"] }, item)) return false;

                comboBox->setDropButtonHoverText(text, item);
            } else if (value->data["index"]) {
                size_t index;
                if (!parseValue({ value->data["index"] }, index)) return false;

                comboBox->setDropButtonHoverText(text, index);
            } else if (value->data["comp"]) {
                // TODO(Matthew): Implement.
                return false;
            } else {
                return false;
            }
        } else {
            nString text;
            if (!parseValue(*value, text)) return false;

            comboBox->setDropButtonHoverText(text);
        }

        return true;
    } else if (name == "max_drop_height") {
        f32 maxDropHeight;
        if (!parseValue(*value, maxDropHeight)) return false;

        comboBox->setMaxDropHeight(maxDropHeight);
    } else {
        return vui::parseWidgetEntry(context, comboBox, name, value, widgetParser);
    }

    return true;
}
