#include "Vorb/stdafx.h"
#include "Vorb/ui/KeyboardEventDispatcher.h"

vui::KeyboardEventDispatcher::KeyboardEventDispatcher() {
    for (auto& key : m_presses) std::atomic_init(&key, 0);
}

i32 vui::KeyboardEventDispatcher::getNumPresses(VirtualKey k) const {
    return std::atomic_load(&m_presses[k]);
}
bool vui::KeyboardEventDispatcher::hasFocus() const {
    return std::atomic_load(&m_focus) != 0;
}

void vui::KeyboardEventDispatcher::addPress(VirtualKey k) {

}
void vui::KeyboardEventDispatcher::release(VirtualKey k) {
    std::atomic_store(&m_presses[k], 0);
}
