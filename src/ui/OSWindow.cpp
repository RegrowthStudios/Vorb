#include "stdafx.h"
#include "ui/OSWindow.h"

#if defined(VORB_IMPL_UI_SDL)
#include "ImplUISDL.h"
#endif

#if defined(VORB_IMPL_UI_SDL)
bool vui::OSWindow::create(OSWindow& window, OSWindowSettings& settings) {
    SDL_WindowFlags flags = SDL_WINDOW_SHOWN;
    if (settings.isResizable) flags = (SDL_WindowFlags)(flags | SDL_WINDOW_RESIZABLE);
    if (settings.isBorderless) flags = (SDL_WindowFlags)(flags | SDL_WINDOW_BORDERLESS);
    if (settings.isFullscreen) flags = (SDL_WindowFlags)(flags | SDL_WINDOW_FULLSCREEN);
    
    // Create a window
    SDL_Window* w = SDL_CreateWindow(DEFAULT_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)settings.width, (int)settings.height, flags);
    if (!w) return nullptr;

    window.m_apiHandle = w;
    
    // Obtain the native OS handle to the window
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(w, &wmInfo);
    window.m_hWnd = wmInfo.info.win.window;

    // Create this window's input dispatcher
    ui32 id = SDL_GetWindowID(w);
    window.m_dispatcher = new InputDispatcher {};
    window.m_dispatcher->m_window = &window;
    dispatchers[id] = window.m_dispatcher;

    return true;
}

void vui::OSWindow::moveValues(OSWindow& dest, OSWindow&& src) {
    // Move and invalidate
    dest.m_apiHandle = src.m_apiHandle;
    src.m_apiHandle = nullptr;
    dest.m_hWnd = src.m_hWnd;
    src.m_hWnd = nullptr;
    dest.m_dispatcher = src.m_dispatcher;
    src.m_dispatcher = nullptr;

    // Update the dispatcher
    dest.m_dispatcher->m_window = &dest;
}

vui::OSWindow::~OSWindow() {
    if (m_apiHandle) {
        // Get window ID
        SDL_Window* w = (SDL_Window*)m_apiHandle;
        ui32 id = SDL_GetWindowID(w);

        // Destroy the InputDispatcher
        delete m_dispatcher;
        dispatchers.erase(dispatchers.find(id));

        // Destroy the window
        SDL_DestroyWindow(w);
        m_apiHandle = nullptr;
        m_hWnd = nullptr;
    }
}

void vui::OSWindow::update() {
    SDL_PumpEvents();
}

i64v2 vui::OSWindow::getPosition() const {
    int x, y;
    SDL_GetWindowPosition((SDL_Window*)m_apiHandle, &x, &y);
    return i64v2(x, y);
}

bool vui::OSWindow::setFullscreen(bool value) {
    return SDL_SetWindowFullscreen((SDL_Window*)m_apiHandle, value ? SDL_WINDOW_FULLSCREEN : 0) == 0;
}
bool vui::OSWindow::setBorderless(bool value) {
    static const LONG styleCheck = (WS_CAPTION | WS_THICKFRAME | WS_SYSMENU);

    // Get and check current style
    LONG style = GetWindowLongPtr(m_hWnd, GWL_STYLE);
    bool isBordered = (style & styleCheck) != 0;
    if (value != isBordered) return true;

    // Update style value
    if (value) {
        style &= ~styleCheck;
    } else {
        style |= styleCheck;
    }
    SetWindowLongPtr(m_hWnd, GWL_STYLE, style);

    // Update window
    SetWindowPos(m_hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
    UpdateWindow(m_hWnd);

    return true;
}
bool vui::OSWindow::setResizable(bool value) {
    // TODO(Cristian): This is still incorrect... the fault of a lazy programmer from Microsoft
    LONG style = GetWindowLongPtr(m_hWnd, GWL_STYLE);

    // Update style value
    if (value) {
        style |= WS_THICKFRAME;
    } else {
        style &= ~WS_THICKFRAME;
    }
    SetWindowLongPtr(m_hWnd, GWL_STYLE, style);

    // Retrieve window rectangle
    int rect[4];
    SDL_GetWindowPosition((SDL_Window*)m_apiHandle, &rect[0], &rect[1]);
    SDL_GetWindowSize((SDL_Window*)m_apiHandle, &rect[2], &rect[3]);

    // Update window
    SetWindowPos(m_hWnd, NULL, rect[0], rect[1], rect[2], rect[3], SWP_FRAMECHANGED);
    return true;
}
bool vui::OSWindow::setPosition(UNIT_SPACE(PIXEL) i64 x, UNIT_SPACE(PIXEL) i64 y) {
    SDL_SetWindowPosition((SDL_Window*)m_apiHandle, (int)x, (int)y);
    return true;
}
bool vui::OSWindow::setSize(UNIT_SPACE(PIXEL) i64 w, UNIT_SPACE(PIXEL) i64 h) {
    SDL_SetWindowSize((SDL_Window*)m_apiHandle, (int)w, (int)h);
    return true;
}
bool vui::OSWindow::setTitle(const cString value) {
    SDL_SetWindowTitle((SDL_Window*)m_apiHandle, value);
    return true;
}
#elif defined(VORB_IMPL_UI_GLFW)
#error TODO(Cristian): Implement
#elif defined(VORB_IMPL_UI_SFML)
#error Fuck you... really... grab your right hand and do it.
#endif
