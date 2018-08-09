#include "Vorb/stdafx.h"
#include "Vorb/ui/InputDispatcher.h"

#include "Vorb/os.h"

#include "Vorb/ui/InputDispatcherEventCatcher.h"
#include "Vorb/ui/GameWindow.h"
#include "Vorb/ui/KeyMappings.inl"

#if defined(VORB_IMPL_UI_GLFW) || defined(VORB_IMPL_UI_SFML)
vui::KeyModifiers vui::impl::InputDispatcherEventCatcher::mods = {};
#endif

vui::MouseEventDispatcher vui::InputDispatcher::mouse;
vui::KeyboardEventDispatcher vui::InputDispatcher::key;
vui::WindowEventDispatcher vui::InputDispatcher::window;
volatile bool vui::InputDispatcher::m_isInit = false;
vui::GameWindow* vui::InputDispatcher::m_window = nullptr;
Event<> vui::InputDispatcher::onQuit(nullptr);

void vui::InputDispatcher::init(GameWindow* w) {
    if (m_isInit) throw std::runtime_error("Input dispatcher is already initialized");
    m_isInit = true;
    m_window = w;

#if defined(VORB_IMPL_UI_SDL)
    SDL_SetEventFilter(impl::InputDispatcherEventCatcher::onSDLEvent, nullptr);
    SDL_EventState(SDL_DROPFILE, SDL_ENABLE);
    SDL_StartTextInput();
#elif defined(VORB_IMPL_UI_GLFW)
    GLFWwindow* window = (GLFWwindow*)m_window->getHandle();
    glfwSetCharCallback(window, impl::InputDispatcherEventCatcher::onCharEvent);
    glfwSetCursorEnterCallback(window, impl::InputDispatcherEventCatcher::onMouseFocusEvent);
    glfwSetCursorPosCallback(window, impl::InputDispatcherEventCatcher::onMousePosEvent);
    glfwSetDropCallback(window, impl::InputDispatcherEventCatcher::onFileDropEvent);
    glfwSetFramebufferSizeCallback(window, impl::InputDispatcherEventCatcher::onFramebufferSizeEvent);
    glfwSetKeyCallback(window, impl::InputDispatcherEventCatcher::onKeyEvent);
    glfwSetMonitorCallback(impl::InputDispatcherEventCatcher::onMonitorEvent);
    glfwSetMouseButtonCallback(window, impl::InputDispatcherEventCatcher::onMouseButtonEvent);
    glfwSetScrollCallback(window, impl::InputDispatcherEventCatcher::onMouseScrollEvent);
    glfwSetWindowCloseCallback(window, impl::InputDispatcherEventCatcher::onWindowCloseEvent);
    glfwSetWindowFocusCallback(window, impl::InputDispatcherEventCatcher::onKeyFocusEvent);
    glfwSetWindowIconifyCallback(window, impl::InputDispatcherEventCatcher::onIconifyEvent);
    glfwSetWindowPosCallback(window, impl::InputDispatcherEventCatcher::onWindowPosEvent);
    glfwSetWindowSizeCallback(window, impl::InputDispatcherEventCatcher::onWindowSizeEvent);
#endif

    // Clear values
    memset(key.m_state, 0, sizeof(key.m_state));
    mouse.m_fullScroll = i32v2(0, 0);
    mouse.m_lastPos = i32v2(0, 0);
}
void vui::InputDispatcher::dispose() {
    if (!m_isInit) return;
    m_isInit = false;
#if defined(VORB_IMPL_UI_SDL)
    SDL_StopTextInput();
    SDL_SetEventFilter(nullptr, nullptr);
#elif defined(VORB_IMPL_UI_GLFW)
    GLFWwindow* window = (GLFWwindow*)m_window->getHandle();
    glfwSetCharCallback(window, nullptr);
    glfwSetCursorEnterCallback(window, nullptr);
    glfwSetCursorPosCallback(window, nullptr);
    glfwSetDropCallback(window, nullptr);
    glfwSetFramebufferSizeCallback(window, nullptr);
    glfwSetKeyCallback(window, nullptr);
    glfwSetMonitorCallback(nullptr);
    glfwSetMouseButtonCallback(window, nullptr);
    glfwSetScrollCallback(window, nullptr);
    glfwSetWindowCloseCallback(window, nullptr);
    glfwSetWindowFocusCallback(window, nullptr);
    glfwSetWindowIconifyCallback(window, nullptr);
    glfwSetWindowPosCallback(window, nullptr);
    glfwSetWindowSizeCallback(window, nullptr);
#endif
    m_window = nullptr;
}

#if defined(VORB_IMPL_UI_SDL) || defined(VORB_IMPL_UI_SFML)
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
#endif

#if defined(VORB_IMPL_UI_SDL)
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
        ie.key.keyCode = vui::impl::mapping[SDL_GetScancodeFromKey(e->key.keysym.sym) + 1];
        ie.key.scanCode = e->key.keysym.scancode;
        ie.key.repeatCount = e->key.repeat;
        vui::InputDispatcher::key.m_state[ie.key.keyCode] = true;
        vui::InputDispatcher::key.onKeyDown(ie.key);
        vui::InputDispatcher::key.onEvent();
        break;
    case SDL_KEYUP:
        convert(ie.key.mod, e->key.keysym.mod);
        ie.key.keyCode = vui::impl::mapping[SDL_GetScancodeFromKey(e->key.keysym.sym) + 1];
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
            break;
        case SDL_WINDOWEVENT_RESIZED:
            ie.windowResize.w = e->window.data1;
            ie.windowResize.h = e->window.data2;
            vui::InputDispatcher::window.onResize(ie.windowResize);
            vui::InputDispatcher::window.onEvent();
            break;
        case SDL_WINDOWEVENT_ENTER:
            // We must poll this one instance
#ifdef VORB_OS_WINDOWS
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
        break;
    default:
        // Unrecognized event
        return 1;
    }
    return 0;
}
#elif defined(VORB_IMPL_UI_GLFW)
void vui::impl::InputDispatcherEventCatcher::onMonitorEvent(GLFWmonitor* monitor, int) {
    // Empty
    // TODO: Add support?
}
void vui::impl::InputDispatcherEventCatcher::onFileDropEvent(GLFWwindow*, int count, const cString* files) {
    vui::WindowFileEvent e;
    for (i32 i = 0; i < count; i++) {
        e.file = files[i];
        vui::InputDispatcher::window.onFile(e);
        vui::InputDispatcher::window.onEvent();
    }
}
void vui::impl::InputDispatcherEventCatcher::onKeyFocusEvent(GLFWwindow*, int value) {
    if (value != 0) {
        vui::InputDispatcher::key.onFocusGained();
    } else {
        vui::InputDispatcher::key.onFocusLost();
    }
    vui::InputDispatcher::key.onEvent();
}
void vui::impl::InputDispatcherEventCatcher::onKeyEvent(GLFWwindow*, int key, int scan, int action, int mod) {
    // We don't want repeat events here
    // TODO: Actually, we do... we can correctly calculate counts, duh
    if (action == GLFW_REPEAT) return;

    KeyEvent e;
    e.keyCode = vui::impl::mapping[key + 1];
    e.scanCode = scan;
    e.repeatCount = 0;
    switch (key) {
    case GLFW_KEY_LEFT_ALT:
        impl::InputDispatcherEventCatcher::mods.lAlt = action == GLFW_PRESS;
        break;
    case GLFW_KEY_RIGHT_ALT:
        impl::InputDispatcherEventCatcher::mods.rAlt = action == GLFW_PRESS;
        break;
    case GLFW_KEY_LEFT_CONTROL:
        impl::InputDispatcherEventCatcher::mods.lCtrl = action == GLFW_PRESS;
        break;
    case GLFW_KEY_RIGHT_CONTROL:
        impl::InputDispatcherEventCatcher::mods.rCtrl = action == GLFW_PRESS;
        break;
    case GLFW_KEY_LEFT_SUPER:
        impl::InputDispatcherEventCatcher::mods.lGUI = action == GLFW_PRESS;
        break;
    case GLFW_KEY_RIGHT_SUPER:
        impl::InputDispatcherEventCatcher::mods.rGUI = action == GLFW_PRESS;
        break;
    case GLFW_KEY_LEFT_SHIFT:
        impl::InputDispatcherEventCatcher::mods.lShift = action == GLFW_PRESS;
        break;
    case GLFW_KEY_RIGHT_SHIFT:
        impl::InputDispatcherEventCatcher::mods.rShift = action == GLFW_PRESS;
        break;
    default:
        break;
    }
    e.mod = mods;
    if (action == GLFW_PRESS) {
        vui::InputDispatcher::key.onKeyDown(e);
    } else {
        vui::InputDispatcher::key.onKeyUp(e);
    }
    vui::InputDispatcher::key.onEvent();
}
void vui::impl::InputDispatcherEventCatcher::onCharEvent(GLFWwindow*, unsigned int unicode) {
    TextEvent e;
    memcpy(e.text, &unicode, sizeof(unsigned int));
    e.text[sizeof(unsigned int)] = 0;
    vui::InputDispatcher::key.onText(e);
    vui::InputDispatcher::key.onEvent();
}
void vui::impl::InputDispatcherEventCatcher::onMouseFocusEvent(GLFWwindow* window, int value) {
#ifdef VORB_OS_WINDOWS
    {
        POINT mp;
        GetCursorPos(&mp);
        int wx, wy;
        glfwGetWindowPos(window, &wx, &wy);
        vui::InputDispatcher::mouse.m_lastPos.x = mp.x - wx;
        vui::InputDispatcher::mouse.m_lastPos.y = mp.y - wy;
    }
#else
    // TODO: Does this work?
    {
        f64v2 mp;
        glfwGetCursorPos(window, &mp.x, &mp.y);
        vui::InputDispatcher::mouse.m_lastPos.x = (i32)mp.x;
        vui::InputDispatcher::mouse.m_lastPos.y = (i32)mp.y;
    }
#endif
    MouseEvent e;
    e.x = vui::InputDispatcher::mouse.m_lastPos.x;
    e.y = vui::InputDispatcher::mouse.m_lastPos.y;
    if (value != 0) {
        vui::InputDispatcher::mouse.onFocusGained(e);
    } else {
        vui::InputDispatcher::mouse.onFocusLost(e);
    }
    vui::InputDispatcher::mouse.onEvent(e);
}
void vui::impl::InputDispatcherEventCatcher::onMousePosEvent(GLFWwindow*, f64 x, f64 y) {
    MouseMotionEvent e;
    e.x = (i32)x;
    e.y = (i32)y;
    e.dx = e.x - vui::InputDispatcher::mouse.m_lastPos.x;
    e.dy = e.y - vui::InputDispatcher::mouse.m_lastPos.y;
    if (e.dx == 0 && e.dy == 0) return;
    vui::InputDispatcher::mouse.m_lastPos.x = e.x;
    vui::InputDispatcher::mouse.m_lastPos.y = e.y;
    vui::InputDispatcher::mouse.onMotion(e);
    vui::InputDispatcher::mouse.onEvent(e);
}
void vui::impl::InputDispatcherEventCatcher::onMouseButtonEvent(GLFWwindow*, int button, int action, int) {
    MouseButtonEvent e;
    e.x = vui::InputDispatcher::mouse.m_lastPos.x;
    e.y = vui::InputDispatcher::mouse.m_lastPos.y;
    switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        e.button = MouseButton::LEFT;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        e.button = MouseButton::RIGHT;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        e.button = MouseButton::MIDDLE;
        break;
    case GLFW_MOUSE_BUTTON_4:
        e.button = MouseButton::X1;
        break;
    case GLFW_MOUSE_BUTTON_5:
        e.button = MouseButton::X2;
        break;
    default:
        e.button = MouseButton::UNKNOWN;
        break;
    }
    e.clicks = 1;

    if (action == GLFW_PRESS) {
        vui::InputDispatcher::mouse.onButtonDown(e);
    } else {
        vui::InputDispatcher::mouse.onButtonUp(e);
    }
    vui::InputDispatcher::mouse.onEvent(e);
}
void vui::impl::InputDispatcherEventCatcher::onMouseScrollEvent(GLFWwindow*, f64 x, f64 y) {
    MouseWheelEvent e;
    e.dx = (i32)x;
    e.dy = (i32)y;
    if (e.dx == 0 && e.dy == 0) return;
    vui::InputDispatcher::mouse.m_fullScroll.x += e.dx;
    vui::InputDispatcher::mouse.m_fullScroll.y += e.dy;

    e.x = vui::InputDispatcher::mouse.m_lastPos.x;
    e.y = vui::InputDispatcher::mouse.m_lastPos.y;
    e.sx = vui::InputDispatcher::mouse.m_fullScroll.x;
    e.sy = vui::InputDispatcher::mouse.m_fullScroll.y;

    vui::InputDispatcher::mouse.onWheel(e);
    vui::InputDispatcher::mouse.onEvent(e);
}
void vui::impl::InputDispatcherEventCatcher::onWindowCloseEvent(GLFWwindow*) {
    vui::InputDispatcher::window.onClose();
    vui::InputDispatcher::window.onEvent();
    vui::InputDispatcher::onQuit();
}
void vui::impl::InputDispatcherEventCatcher::onWindowPosEvent(GLFWwindow*, int x, int y) {
    // Empty
    // TODO: Add support?
}
void vui::impl::InputDispatcherEventCatcher::onWindowSizeEvent(GLFWwindow*, int w, int h) {
    WindowResizeEvent e;
    e.w = (ui32)w;
    e.h = (ui32)h;
    vui::InputDispatcher::window.onResize(e);
    vui::InputDispatcher::window.onEvent();
}
void vui::impl::InputDispatcherEventCatcher::onFramebufferSizeEvent(GLFWwindow*, int w, int h) {
    // TODO: Check difference with above
    WindowResizeEvent e;
    e.w = (ui32)w;
    e.h = (ui32)h;
    vui::InputDispatcher::window.onResize(e);
    vui::InputDispatcher::window.onEvent();
}
void vui::impl::InputDispatcherEventCatcher::onIconifyEvent(GLFWwindow*, int value) {
    // Empty
    // TODO: Add support?
}
#elif defined(VORB_IMPL_UI_SFML)
static void checkMods(sf::Keyboard::Key code, bool action) {
    switch (code) {
    case sf::Keyboard::Key::LAlt:
        vui::impl::InputDispatcherEventCatcher::mods.lAlt = action;
        break;
    case sf::Keyboard::Key::RAlt:
        vui::impl::InputDispatcherEventCatcher::mods.rAlt = action;
        break;
    case sf::Keyboard::Key::LControl:
        vui::impl::InputDispatcherEventCatcher::mods.lCtrl = action;
        break;
    case sf::Keyboard::Key::RControl:
        vui::impl::InputDispatcherEventCatcher::mods.rCtrl = action;
        break;
    case sf::Keyboard::Key::LSystem:
        vui::impl::InputDispatcherEventCatcher::mods.lGUI = action;
        break;
    case sf::Keyboard::Key::RSystem:
        vui::impl::InputDispatcherEventCatcher::mods.rGUI = action;
        break;
    case sf::Keyboard::Key::LShift:
        vui::impl::InputDispatcherEventCatcher::mods.lShift = action;
        break;
    case sf::Keyboard::Key::RShift:
        vui::impl::InputDispatcherEventCatcher::mods.rShift = action;
        break;
    default:
        break;
    }
}
static vui::MouseButton convert(sf::Mouse::Button b) {
    switch (b) {
    case sf::Mouse::Left:
        return vui::MouseButton::LEFT;
    case sf::Mouse::Right:
        return vui::MouseButton::RIGHT;
    case sf::Mouse::Middle:
        return vui::MouseButton::MIDDLE;
    case sf::Mouse::XButton1:
        return vui::MouseButton::X1;
    case sf::Mouse::XButton2:
        return vui::MouseButton::X2;
    default:
        return vui::MouseButton::UNKNOWN;
    }
}
void vui::impl::InputDispatcherEventCatcher::onSFMLEvent(sf::RenderWindow* userData, sf::Event& e) {
    InputEvent ie = {};
    sf::Vector2i sfv;
    switch (e.type) {
    case sf::Event::Closed:
        vui::InputDispatcher::window.onClose();
        vui::InputDispatcher::window.onEvent();
        vui::InputDispatcher::onQuit();
        break;
    case sf::Event::GainedFocus:
        vui::InputDispatcher::key.onFocusGained();
        vui::InputDispatcher::key.onEvent();
        break;
    case sf::Event::LostFocus:
        vui::InputDispatcher::key.onFocusLost();
        vui::InputDispatcher::key.onEvent();
        break;
    case sf::Event::KeyPressed:
        ie.key.keyCode = vui::impl::mapping[e.key.code + 1];
        if (vui::InputDispatcher::key.m_state[ie.key.keyCode]) break;

        checkMods(e.key.code, true);
        ie.key.mod = mods;
        ie.key.scanCode = ie.key.keyCode; // TODO: Fuck...
        ie.key.repeatCount = 0; // TODO: Fuck me with a pickle
        vui::InputDispatcher::key.m_state[ie.key.keyCode] = true;
        vui::InputDispatcher::key.onKeyDown(ie.key);
        vui::InputDispatcher::key.onEvent();
        break;
    case sf::Event::KeyReleased:
        checkMods(e.key.code, false);
        ie.key.mod = mods;
        ie.key.keyCode = vui::impl::mapping[e.key.code + 1];
        ie.key.scanCode = ie.key.keyCode;
        ie.key.repeatCount = 0;
        vui::InputDispatcher::key.m_state[ie.key.keyCode] = false;
        vui::InputDispatcher::key.onKeyUp(ie.key);
        vui::InputDispatcher::key.onEvent();
        break;
    case sf::Event::MouseButtonPressed:
        ie.mouseButton.x = e.mouseButton.x;
        ie.mouseButton.y = e.mouseButton.y;
        ie.mouseButton.button = convert(e.mouseButton.button);
        ie.mouseButton.clicks = 1; // Ugh....
        vui::InputDispatcher::mouse.onButtonDown(ie.mouseButton);
        vui::InputDispatcher::mouse.onEvent(ie.mouseButton);
        break;
    case sf::Event::MouseButtonReleased:
        ie.mouseButton.x = e.mouseButton.x;
        ie.mouseButton.y = e.mouseButton.y;
        ie.mouseButton.button = convert(e.mouseButton.button);
        ie.mouseButton.clicks = 1;
        vui::InputDispatcher::mouse.onButtonUp(ie.mouseButton);
        vui::InputDispatcher::mouse.onEvent(ie.mouseButton);
        break;
    case sf::Event::MouseEntered:
        sfv = sf::Mouse::getPosition(*userData);
        ie.mouse.x = sfv.x;
        ie.mouse.y = sfv.y;
        vui::InputDispatcher::mouse.setPos(ie.mouse.x, ie.mouse.y);
        vui::InputDispatcher::mouse.setFocus(true);
        vui::InputDispatcher::mouse.onFocusGained(ie.mouse);
        vui::InputDispatcher::mouse.onEvent(ie.mouse);
        break;
    case sf::Event::MouseLeft:
        sfv = sf::Mouse::getPosition(*userData);
        ie.mouse.x = sfv.x;
        ie.mouse.y = sfv.y;
        vui::InputDispatcher::mouse.setPos(ie.mouse.x, ie.mouse.y);
        vui::InputDispatcher::mouse.setFocus(false);
        vui::InputDispatcher::mouse.onFocusLost(ie.mouse);
        vui::InputDispatcher::mouse.onEvent(ie.mouse);
        break;
    case sf::Event::MouseMoved:
        ie.mouseMotion.x = e.mouseMove.x;
        ie.mouseMotion.y = e.mouseMove.y;
        ie.mouseMotion.dx = ie.mouseMotion.x - vui::InputDispatcher::mouse.m_lastPos.x;
        ie.mouseMotion.dy = ie.mouseMotion.y - vui::InputDispatcher::mouse.m_lastPos.y;
        if (ie.mouseMotion.dx == 0 && ie.mouseMotion.dy == 0) return;
        vui::InputDispatcher::mouse.m_lastPos.x = ie.mouseMotion.x;
        vui::InputDispatcher::mouse.m_lastPos.y = ie.mouseMotion.y;
        vui::InputDispatcher::mouse.setPos(ie.mouseMotion.x, ie.mouseMotion.y);
        vui::InputDispatcher::mouse.onMotion(ie.mouseMotion);
        vui::InputDispatcher::mouse.onEvent(ie.mouseMotion);
        break;
    case sf::Event::MouseWheelMoved:
        ie.mouseWheel.dx = 0;
        ie.mouseWheel.dy = e.mouseWheel.delta;
        if (ie.mouseWheel.dx == 0 && ie.mouseWheel.dy == 0) return;
        vui::InputDispatcher::mouse.m_fullScroll.x += ie.mouseWheel.dx;
        vui::InputDispatcher::mouse.m_fullScroll.y += ie.mouseWheel.dy;
        ie.mouseWheel.x = e.mouseWheel.x;
        ie.mouseWheel.y = e.mouseWheel.y;
        ie.mouseWheel.sx = vui::InputDispatcher::mouse.m_fullScroll.x;
        ie.mouseWheel.sy = vui::InputDispatcher::mouse.m_fullScroll.y;
        vui::InputDispatcher::mouse.onWheel(ie.mouseWheel);
        vui::InputDispatcher::mouse.onEvent(ie.mouseWheel);
        break;
    case sf::Event::Resized:
        ie.windowResize.w = e.size.width;
        ie.windowResize.h = e.size.height;
        vui::InputDispatcher::window.onResize(ie.windowResize);
        vui::InputDispatcher::window.onEvent();
        break;
    case sf::Event::TextEntered:
        if (e.text.unicode < 32 || (e.text.unicode > 126 && e.text.unicode < 160)) break;

        memcpy(ie.text.text, &e.text.unicode, sizeof(sf::Uint32));
        ie.text.text[sizeof(sf::Uint32)] = 0;
        vui::InputDispatcher::key.onText(ie.text);
        vui::InputDispatcher::key.onEvent();
        break;
    default:
        break;
    }
}
#endif
