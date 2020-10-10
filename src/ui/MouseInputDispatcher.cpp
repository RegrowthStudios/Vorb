#include "Vorb/stdafx.h"
#include "Vorb/ui/MouseInputDispatcher.h"

void vorb::ui::MouseEventDispatcher::getPosition(i32* x, i32* y) const {
    *x = std::atomic_load(&m_x);
    *y = std::atomic_load(&m_y);
}

bool vorb::ui::MouseEventDispatcher::hasFocus() const {
    return std::atomic_load(&m_focus) != 0;
}

bool vorb::ui::MouseEventDispatcher::isRelative() const {
    return std::atomic_load(&m_relative) != 0;
}

bool vorb::ui::MouseEventDispatcher::isHidden() const {
    return std::atomic_load(&m_hidden) != 0;
}

i32v2 vorb::ui::MouseEventDispatcher::getScroll() const {
    return std::atomic_load(&m_fullScroll);
}

void vui::MouseEventDispatcher::setPosition(i32 x, i32 y) {
    std::atomic_store(&m_x, x);
    std::atomic_store(&m_y, y);
}

void vui::MouseEventDispatcher::setFocus(bool v) {
    std::atomic_store(&m_focus, v ? 1 : 0);
}

void vui::MouseEventDispatcher::setRelative(bool v) {
    std::atomic_store(&m_relative, v ? 1 : 0);
}

void vui::MouseEventDispatcher::setHidden(bool v) {
    std::atomic_store(&m_hidden, v ? 1 : 0);
}

void vui::MouseEventDispatcher::setScroll(i32v2 scroll) {
    std::atomic_store(&m_fullScroll, scroll);
}
