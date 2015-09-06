#pragma once

#ifndef Vorb_UIStyle_h__
//! @cond DOXY_SHOW_HEADER_GUARDS
#define Vorb_UIStyle_h__
//! @endcond

#ifndef VORB_USING_PCH
#include "types.h"
#endif // !VORB_USING_PCH

namespace vorb {
    namespace ui {
        class UIStyle {
        public:
            UIStyle();
            ~UIStyle();

            /************************************************************************/
            /* Getters                                                              */
            /************************************************************************/
            int getFlags();

            /************************************************************************/
            /* Setters                                                              */
            /************************************************************************/
            void setFlags(int);

            /************************************************************************/
            /* Members                                                              */
            /************************************************************************/
            color4 color;
            int flags = 0;
        };

        UIStyle operator+(const UIStyle& p, const UIStyle& q);
    }
}

namespace vui = vorb::ui;

#endif // !Vorb_UIStyle_h__