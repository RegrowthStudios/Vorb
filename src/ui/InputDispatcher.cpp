#include "stdafx.h"
#include "ui/InputDispatcher.h"

#if defined(WIN32) || defined(WIN64)
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "ui/GameWindow.h"

namespace vorb {
    namespace ui {
        namespace impl {
            class InputDispatcherEventCatcher {
            public:
                static i32 onSDLEvent(void* userData, SDL_Event* e);
            };
        }
    }
}

vui::MouseEventDispatcher vui::InputDispatcher::mouse;
vui::KeyboardEventDispatcher vui::InputDispatcher::key;
vui::WindowEventDispatcher vui::InputDispatcher::window;
volatile bool vui::InputDispatcher::m_isInit = false;
GameWindow* vui::InputDispatcher::m_window = nullptr;
Event<> vui::InputDispatcher::onQuit(nullptr);


void vui::InputDispatcher::init(GameWindow* w) {
    if (m_isInit) throw std::runtime_error("Input dispatcher is already initialized");
    m_isInit = true;
    m_window = w;
    SDL_SetEventFilter(impl::InputDispatcherEventCatcher::onSDLEvent, nullptr);
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    SDL_StartTextInput();

    // Clear values
    memset(key.m_state, 0, sizeof(key.m_state));
    mouse.m_fullScroll = i32v2(0, 0);
    mouse.m_lastPos = i32v2(0, 0);
}
void vui::InputDispatcher::dispose() {
    if (!m_isInit) return;
    m_isInit = false;
    SDL_StopTextInput();
    SDL_SetEventFilter(nullptr, nullptr);
}

/// Memory-efficient way to split through multiple event types
typedef union {
    vui::MouseEvent mouse;
    vui::MouseButtonEvent mouseButton;
    vui::MouseMotionEvent mouseMotion;
    vui::MouseWheelEvent mouseWheel;
    vui::KeyEvent key;
    vui::TextEvent text;
    vui::WindowResizeEvent windowResize;
    vui::WindowFileEvent windowFile;
} InputEvent;

void convert(vui::KeyModifiers& km, const ui16& sm) {
#define MASK_BOOL(F) ((F) == 0) ? false : true;
    km.lAlt = MASK_BOOL(sm & KMOD_LALT);
    km.rAlt = MASK_BOOL(sm & KMOD_RALT);
    km.lCtrl = MASK_BOOL(sm & KMOD_LCTRL);
    km.rCtrl = MASK_BOOL(sm & KMOD_RCTRL);
    km.lGUI = MASK_BOOL(sm & KMOD_LGUI);
    km.rGUI = MASK_BOOL(sm & KMOD_RGUI);
    km.lShift = MASK_BOOL(sm & KMOD_LSHIFT);
    km.rShift = MASK_BOOL(sm & KMOD_RSHIFT);
    km.caps = MASK_BOOL(sm & KMOD_CAPS);
    km.num = MASK_BOOL(sm & KMOD_NUM);
#undef MASK_BOOL
}
void convert(vui::MouseButton& mb, const ui8& sb) {
    switch (sb) {
    case SDL_BUTTON_LEFT:
        mb = vui::MouseButton::LEFT;
        break;
    case SDL_BUTTON_MIDDLE:
        mb = vui::MouseButton::MIDDLE;
        break;
    case SDL_BUTTON_RIGHT:
        mb = vui::MouseButton::RIGHT;
        break;
    case SDL_BUTTON_X1:
        mb = vui::MouseButton::X1;
        break;
    case SDL_BUTTON_X2:
        mb = vui::MouseButton::X2;
        break;
    default:
        mb = vui::MouseButton::UNKNOWN;
        break;
    }
}

i32 vui::impl::InputDispatcherEventCatcher::onSDLEvent(void*, SDL_Event* e) {
    InputEvent ie;

    switch (e->type) {
    case SDL_KEYDOWN:
        convert(ie.key.mod, e->key.keysym.mod);
        ie.key.keyCode = SDL_GetScancodeFromKey(e->key.keysym.sym);
        ie.key.scanCode = e->key.keysym.scancode;
        ie.key.repeatCount = e->key.repeat;
        vui::InputDispatcher::key.m_state[ie.key.keyCode] = true;
        vui::InputDispatcher::key.onKeyDown(ie.key);
        vui::InputDispatcher::key.onEvent();
        break;
    case SDL_KEYUP:
        convert(ie.key.mod, e->key.keysym.mod);
        ie.key.keyCode = SDL_GetScancodeFromKey(e->key.keysym.sym);
        ie.key.scanCode = e->key.keysym.scancode;
        ie.key.repeatCount = e->key.repeat;
        vui::InputDispatcher::key.m_state[ie.key.keyCode] = false;
        vui::InputDispatcher::key.onKeyUp(ie.key);
        vui::InputDispatcher::key.onEvent();
        break;
    case SDL_MOUSEMOTION:
        ie.mouseMotion.x = e->motion.x;
        ie.mouseMotion.y = e->motion.y;
        ie.mouseMotion.dx = e->motion.xrel;
        ie.mouseMotion.dy = e->motion.yrel;
        vui::InputDispatcher::mouse.m_lastPos.x = ie.mouseMotion.x;
        vui::InputDispatcher::mouse.m_lastPos.y = ie.mouseMotion.y;
        vui::InputDispatcher::mouse.onMotion(ie.mouseMotion);
        vui::InputDispatcher::mouse.onEvent(ie.mouseMotion);
        break;
    case SDL_MOUSEBUTTONDOWN:
        convert(ie.mouseButton.button, e->button.button);
        ie.mouseButton.x = e->button.x;
        ie.mouseButton.y = e->button.y;
        ie.mouseButton.clicks = e->button.clicks;
        vui::InputDispatcher::mouse.onButtonDown(ie.mouseButton);
        vui::InputDispatcher::mouse.onEvent(ie.mouseMotion);
        break;
    case SDL_MOUSEBUTTONUP:
        convert(ie.mouseButton.button, e->button.button);
        ie.mouseButton.x = e->button.x;
        ie.mouseButton.y = e->button.y;
        ie.mouseButton.clicks = e->button.clicks;
        vui::InputDispatcher::mouse.onButtonUp(ie.mouseButton);
        vui::InputDispatcher::mouse.onEvent(ie.mouseMotion);
        break;
    case SDL_MOUSEWHEEL:
        ie.mouseWheel.x = vui::InputDispatcher::mouse.m_lastPos.x;
        ie.mouseWheel.y = vui::InputDispatcher::mouse.m_lastPos.y;
        ie.mouseWheel.dx = e->wheel.x;
        ie.mouseWheel.dy = e->wheel.y;
        vui::InputDispatcher::mouse.m_fullScroll.x += ie.mouseWheel.dx;
        vui::InputDispatcher::mouse.m_fullScroll.y += ie.mouseWheel.dy;
        ie.mouseWheel.sx = vui::InputDispatcher::mouse.m_fullScroll.x;
        ie.mouseWheel.sy = vui::InputDispatcher::mouse.m_fullScroll.y;
        vui::InputDispatcher::mouse.onWheel(ie.mouseWheel);
        vui::InputDispatcher::mouse.onEvent(ie.mouseWheel);
        break;
    case SDL_QUIT:
        InputDispatcher::onQuit();
        break;
    case SDL_WINDOWEVENT:
        switch (e->window.event) {
        case SDL_WINDOWEVENT_CLOSE:
            vui::InputDispatcher::window.onClose();
            vui::InputDispatcher::window.onEvent();
        case SDL_WINDOWEVENT_RESIZED:
            ie.windowResize.w = e->window.data1;
            ie.windowResize.h = e->window.data2;
            vui::InputDispatcher::window.onResize(ie.windowResize);
            vui::InputDispatcher::window.onEvent();
            break;
        case SDL_WINDOWEVENT_ENTER:
            // We must poll this one instance
#ifdef OS_WINDOWS
            {
                POINT mp;
                GetCursorPos(&mp);
                i32v2 wp = vui::InputDispatcher::m_window->getPosition();
                vui::InputDispatcher::mouse.m_lastPos.x = mp.x - wp.x;
                vui::InputDispatcher::mouse.m_lastPos.y = mp.y - wp.y;
            }
#else
            // TODO: This is currently not working
            SDL_GetMouseState(&vui::InputDispatcher::mouse.m_lastPos.x, &vui::InputDispatcher::mouse.m_lastPos.y);
#endif
            ie.mouse.x = vui::InputDispatcher::mouse.m_lastPos.x;
            ie.mouse.y = vui::InputDispatcher::mouse.m_lastPos.y;
            vui::InputDispatcher::mouse.onFocusGained(ie.mouse);
            vui::InputDispatcher::mouse.onEvent(ie.mouse);
            break;
        case SDL_WINDOWEVENT_LEAVE:
            ie.mouse.x = vui::InputDispatcher::mouse.m_lastPos.x;
            ie.mouse.y = vui::InputDispatcher::mouse.m_lastPos.y;
            vui::InputDispatcher::mouse.onFocusLost(ie.mouse);
            vui::InputDispatcher::mouse.onEvent(ie.mouse);
            break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            vui::InputDispatcher::key.onFocusGained();
            vui::InputDispatcher::key.onEvent();
            break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
            vui::InputDispatcher::key.onFocusLost();
            vui::InputDispatcher::key.onEvent();
            break;
        default:
            // Unrecognized window event
            return 1;
        }
        break;
    case SDL_TEXTINPUT:
        memcpy(ie.text.text, e->text.text, 32);
        mbstowcs(ie.text.wtext, ie.text.text, 16);
        vui::InputDispatcher::key.onText(ie.text);
        vui::InputDispatcher::key.onEvent();
        break;
    case SDL_DROPFILE:
        ie.windowFile.file = e->drop.file;
        vui::InputDispatcher::window.onFile(ie.windowFile);
        vui::InputDispatcher::window.onEvent();
        SDL_free(e->drop.file);
    default:
        // Unrecognized event
        return 1;
    }
    return 0;
}
