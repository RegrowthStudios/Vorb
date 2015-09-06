#pragma once

#include <include/utils.h>
#include <include/types.h>

#ifndef Vorb_UIUtils_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_UIUtils_h__
//! @endcond

namespace vorb {
    namespace ui {
        enum WidgetFlags {
            ENABLED          = 0x01, // Widget Enabled
            CLIPPING_ENABLED = 0x02, // Widget clipping Enabled
            HOVER            = 0x04, // Mouse within the clip rect
            CLICK            = 0x08,  // Mouse clicked
            DRAWABLE_RELOAD  = 0x10 // Needs drawable reload
        };

        enum StyleFlags {
            COLOR = 0x01,
            TEXTURE = 0x02,
            R = 0x20,
            G = 0x40,
            B = 0x80,
            A = 0x100,
            RGB = 0x200,
            RGBA = 0x400,
        };

        enum MergeFlags {
            BLEND = 0x01,
            HIGH = 0x02,
            LOW = 0x04,
        };

        class UIUtils {
        public:
            /*! @brief Checks if a point is inside the container
            *
            * @param point: The point to check
            * @return true if point is in the bounds of the container
            */
            static bool isInBounds(f32v2 mousePos, f32v2 position, f32v2 dimensions) {
                return (mousePos.x >= position.x && mousePos.x < position.x + dimensions.x &&
                    mousePos.y >= position.y && mousePos.y < position.y + dimensions.y);
            }

            static void addFlag(int* current, int flag) {
                if (!checkFlag(current, flag)) {
                    *current |= flag;
                }
            }

            static void removeFlag(int* current, int flag) {
                if (checkFlag(current, flag)) {
                    *current &= ~flag;
                }
            }

            static bool checkFlag(int* current, int flag) {
                return (*current & flag);
            }
        };
    }
}

namespace vui = vorb::ui;

#endif // !Vorb_Container_h__