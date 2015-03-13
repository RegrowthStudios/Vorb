#include "stdafx.h"
#include "ui/GameWindow.h"

#if defined(VORB_IMPL_UI_SDL)
#if defined(OS_WINDOWS)
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#define VUI_WINDOW_HANDLE(WINDOW_VAR) ( (SDL_Window*) WINDOW_VAR )
#elif defined(VORB_IMPL_UI_GLFW)
#include <GLFW/glfw3.h>
#define VUI_WINDOW_HANDLE(WINDOW_VAR) ( (GLFWwindow*) WINDOW_VAR )
#elif defined(VORB_IMPL_UI_SFML)
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include "InputDispatcherEventCatcher.h"
#define VUI_WINDOW_HANDLE(WINDOW_VAR) ( (sf::RenderWindow*) WINDOW_VAR )
#endif

#include "../ImplGraphicsH.inl"
#include "io/IOManager.h"
#include "ui/InputDispatcher.h"

#if defined(VORB_IMPL_GRAPHICS_OPENGL)
#define DEFAULT_WINDOW_FLAGS (SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)
#elif defined(VORB_IMPL_GRAPHICS_D3D)
#define DEFAULT_WINDOW_FLAGS (SDL_WINDOW_SHOWN)
#endif

KEG_ENUM_DEF(GameSwapInterval, vui::GameSwapInterval, ke) {
    using namespace keg;
    ke.addValue("Unlimited", vui::GameSwapInterval::UNLIMITED_FPS);
    ke.addValue("VSync", vui::GameSwapInterval::V_SYNC);
    ke.addValue("LowSync", vui::GameSwapInterval::LOW_SYNC);
    ke.addValue("PowerSaver", vui::GameSwapInterval::POWER_SAVER);
    ke.addValue("ValueCap", vui::GameSwapInterval::USE_VALUE_CAP);
}

KEG_TYPE_DEF(GameDisplayMode, vui::GameDisplayMode, kt) {
    using namespace keg;
    kt.addValue("ScreenWidth", Value::basic(offsetof(vui::GameDisplayMode, screenWidth), BasicType::I32));
    kt.addValue("ScreenHeight", Value::basic(offsetof(vui::GameDisplayMode, screenHeight), BasicType::I32));
    kt.addValue("IsFullscreen", Value::basic(offsetof(vui::GameDisplayMode, isFullscreen), BasicType::BOOL));
    kt.addValue("IsBorderless", Value::basic(offsetof(vui::GameDisplayMode, isBorderless), BasicType::BOOL));
    kt.addValue("SwapInterval", Value::custom(offsetof(vui::GameDisplayMode, swapInterval), "GameSwapInterval", true));
    kt.addValue("MaxFPS", Value::basic(offsetof(vui::GameDisplayMode, maxFPS), BasicType::F32));
    kt.addValue("GraphicsMajor", Value::basic(offsetof(vui::GameDisplayMode, major), BasicType::UI32));
    kt.addValue("GraphicsMinor", Value::basic(offsetof(vui::GameDisplayMode, minor), BasicType::UI32));
    kt.addValue("GraphicsCore", Value::basic(offsetof(vui::GameDisplayMode, core), BasicType::BOOL));
}

// For Comparing Display Modes When Saving Data
static bool operator==(const vui::GameDisplayMode& m1, const vui::GameDisplayMode& m2) {
    return
        m1.screenWidth == m2.screenWidth &&
        m1.screenHeight == m2.screenHeight &&
        m1.isFullscreen == m2.isFullscreen &&
        m1.isBorderless == m2.isBorderless &&
        m1.swapInterval == m2.swapInterval &&
        m1.maxFPS == m2.maxFPS
        ;
}
static bool operator!=(const vui::GameDisplayMode& m1, const vui::GameDisplayMode& m2) {
    return !(m1 == m2);
}

vui::GameWindow::GameWindow() {
    setDefaultSettings(&m_displayMode);
}

bool vui::GameWindow::init() {
    // Attempt To Read Custom Settings
    readSettings();

#if defined(VORB_IMPL_UI_SDL)
    SDL_WindowFlags flags = (SDL_WindowFlags)DEFAULT_WINDOW_FLAGS;
    if (m_displayMode.isBorderless) flags = (SDL_WindowFlags)(flags | SDL_WINDOW_BORDERLESS);
    if (m_displayMode.isFullscreen) flags = (SDL_WindowFlags)(flags | SDL_WINDOW_FULLSCREEN);
    m_window = SDL_CreateWindow(DEFAULT_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_displayMode.screenWidth, m_displayMode.screenHeight, flags);
#elif defined(VORB_IMPL_UI_GLFW)
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, 0);
    if (m_displayMode.isBorderless) glfwWindowHint(GLFW_DECORATED, 0);

    GLFWmonitor* monitor = m_displayMode.isFullscreen ? glfwGetPrimaryMonitor() : nullptr;
    m_window = glfwCreateWindow(m_displayMode.screenWidth, m_displayMode.screenHeight, DEFAULT_TITLE, monitor, nullptr);
#elif defined(VORB_IMPL_UI_SFML)
    sf::ContextSettings context;
    context.depthBits = 24;
    context.stencilBits = 8;
    context.majorVersion = 3;
    context.minorVersion = 2;
    context.antialiasingLevel = 0;

    ui32 flags = sf::Style::None;
    if (!m_displayMode.isBorderless) flags |= sf::Style::Close;
    if (m_displayMode.isFullscreen) flags |= sf::Style::Fullscreen;
    m_window = new sf::RenderWindow(sf::VideoMode(m_displayMode.screenWidth, m_displayMode.screenHeight), DEFAULT_TITLE, flags, context);
    VUI_WINDOW_HANDLE(m_window)->setFramerateLimit(0);
#endif

    // Create The Window
    if (m_window == nullptr) {
        printf("Window Creation Failed\r\n");
        return false;
    }

#if defined(VORB_IMPL_UI_SDL)
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, (int)m_displayMode.major);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, (int)m_displayMode.minor);
    if (m_displayMode.core) {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    } else {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    }
    m_glc = SDL_GL_CreateContext(VUI_WINDOW_HANDLE(m_window));
    SDL_GL_MakeCurrent(VUI_WINDOW_HANDLE(m_window), (SDL_GLContext)m_glc);
#elif defined(VORB_IMPL_GRAPHICS_D3D)
    m_glc = new D3DContext;
    VUI_COM(m_glc) = Direct3DCreate9(D3D_SDK_VERSION);

    {
        D3DPRESENT_PARAMETERS pp = {};
        pp.hDeviceWindow = GetActiveWindow();
        pp.Windowed = TRUE;
        pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        pp.EnableAutoDepthStencil = true;
        pp.AutoDepthStencilFormat = D3DFMT_D16;
        pp.BackBufferWidth = 800;
        pp.BackBufferHeight = 600;
        pp.BackBufferFormat = D3DFMT_R5G6B5;
        pp.MultiSampleType = D3DMULTISAMPLE_NONE;

        VUI_COM(m_glc)->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            pp.hDeviceWindow,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &pp,
            &(VUI_CONTEXT(m_glc))
            );
    }
#endif

#elif defined(VORB_IMPL_UI_GLFW)
    // Initialize OpenGL
    glfwMakeContextCurrent(VUI_WINDOW_HANDLE(m_window));
    m_glc = m_window;
#elif defined(VORB_IMPL_UI_SFML)
    VUI_WINDOW_HANDLE(m_window)->setActive(true);
    m_glc = m_window;
#endif

    if (m_glc == nullptr) {
        printf("Could Not Create OpenGL Context");
        return false;
    }

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        printf("Glew failed to initialize. Your graphics card is probably WAY too old. Or you forgot to extract the .zip. It might be time for an upgrade :)");
        return false;
    }

    // Set More Display Settings
    setSwapInterval(m_displayMode.swapInterval, true);

    // Make sure default clear depth is 1.0f
    glClearDepth(1.0f);

    // Push input from this window
    vui::InputDispatcher::init(this);

    return true;
}
void vui::GameWindow::dispose() {
    vui::InputDispatcher::dispose();
    saveSettings();

#if defined(VORB_IMPL_UI_SDL)
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    if (m_glc) {
        SDL_GL_DeleteContext((SDL_GLContext)m_glc);
        m_glc = nullptr;
    }
#elif defined(VORB_IMPL_GRAPHICS_D3D)
    if (m_glc) {
        VUI_CONTEXT(m_glc)->Release();
        VUI_COM(m_glc)->Release();
    }
#endif
    if (m_window) {
        SDL_DestroyWindow(VUI_WINDOW_HANDLE(m_window));
        m_window = nullptr;
    }
#elif defined(VORB_IMPL_UI_GLFW)
    if (m_window) {
        glfwDestroyWindow(VUI_WINDOW_HANDLE(m_window));
        m_window = nullptr;
    }
#elif defined(VORB_IMPL_UI_SFML)
    if (m_window) {
        VUI_WINDOW_HANDLE(m_window)->close();
        delete VUI_WINDOW_HANDLE(m_window);
        m_window = nullptr;
    }
#endif
}

void vui::GameWindow::setDefaultSettings(GameDisplayMode* mode) {
    mode->screenWidth = DEFAULT_WINDOW_WIDTH;
    mode->screenHeight = DEFAULT_WINDOW_HEIGHT;
    mode->isBorderless = false;
    mode->isFullscreen = false;
    mode->maxFPS = DEFAULT_MAX_FPS;
    mode->swapInterval = DEFAULT_SWAP_INTERVAL;
    mode->major = 3;
    mode->minor = 2;
    mode->core = false;
}

void vui::GameWindow::readSettings() {
    vio::IOManager iom;
    nString data;
    iom.readFileToString(DEFAULT_APP_CONFIG_FILE, data);
    if (data.length()) {
        keg::parse(&m_displayMode, data.c_str(), "GameDisplayMode");
    } else {
        // If there is no app.config, save a default one.
        saveSettings();
    }
}

void vui::GameWindow::saveSettings() const {
    GameDisplayMode modeBasis = {};
    setDefaultSettings(&modeBasis);

    if (m_displayMode != modeBasis) {
        nString data = keg::write(&m_displayMode, "GameDisplayMode", nullptr);
        std::ofstream file(DEFAULT_APP_CONFIG_FILE);
        file << data << std::endl;
        file.flush();
        file.close();
    }
}

void vui::GameWindow::setScreenSize(const i32& w, const i32& h, const bool& overrideCheck /*= false*/) {
    // Apply A Minimal State Change
    if (overrideCheck || m_displayMode.screenWidth != w || m_displayMode.screenHeight != h) {
        m_displayMode.screenWidth = w;
        m_displayMode.screenHeight = h;
#if defined(VORB_IMPL_UI_SDL)
        SDL_SetWindowSize(VUI_WINDOW_HANDLE(m_window), m_displayMode.screenWidth, m_displayMode.screenHeight);
#elif defined(VORB_IMPL_UI_SDL)
        glfwSetWindowSize(VUI_WINDOW_HANDLE(m_window), m_displayMode.screenWidth, m_displayMode.screenHeight);
#elif defined(VORB_IMPL_UI_SFML)
        VUI_WINDOW_HANDLE(m_window)->setSize(sf::Vector2u(m_displayMode.screenWidth, m_displayMode.screenHeight));
#endif
    }
}
void vui::GameWindow::setFullscreen(const bool& useFullscreen, const bool& overrideCheck /*= false*/) {
    if (overrideCheck || m_displayMode.isFullscreen != useFullscreen) {
        m_displayMode.isFullscreen = useFullscreen;
#if defined(VORB_IMPL_UI_SDL)
        SDL_SetWindowFullscreen(VUI_WINDOW_HANDLE(m_window), m_displayMode.isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
#elif defined(VORB_IMPL_UI_GLFW)
        // TODO: GLFW Impl
#elif defined(VORB_IMPL_UI_SFML)
        ui32 flags = sf::Style::None;
        if (!m_displayMode.isBorderless) flags |= sf::Style::Close;
        if (m_displayMode.isFullscreen) flags |= sf::Style::Fullscreen;
        VUI_WINDOW_HANDLE(m_window)->create(sf::VideoMode(m_displayMode.screenWidth, m_displayMode.screenHeight), DEFAULT_TITLE, flags);
#endif
    }
}
void vui::GameWindow::setBorderless(const bool& useBorderless, const bool& overrideCheck /*= false*/) {
    if (overrideCheck || m_displayMode.isBorderless != useBorderless) {
        m_displayMode.isBorderless = useBorderless;
#if defined(VORB_IMPL_UI_SDL)
        SDL_SetWindowBordered((SDL_Window*)m_window, m_displayMode.isBorderless ? SDL_FALSE : SDL_TRUE);
#elif defined(VORB_IMPL_UI_GLFW)
        // TODO: GLFW Impl
#elif defined(VORB_IMPL_UI_SFML)
        ui32 flags = sf::Style::None;
        if (!m_displayMode.isBorderless) flags |= sf::Style::Close;
        if (m_displayMode.isFullscreen) flags |= sf::Style::Fullscreen;
        VUI_WINDOW_HANDLE(m_window)->create(sf::VideoMode(m_displayMode.screenWidth, m_displayMode.screenHeight), DEFAULT_TITLE, flags);
#endif
    }
}
void vui::GameWindow::setSwapInterval(const GameSwapInterval& mode, const bool& overrideCheck /*= false*/) {
    if (overrideCheck || m_displayMode.swapInterval != mode) {
        m_displayMode.swapInterval = mode;
#if defined(VORB_IMPL_UI_SDL)
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
        switch (m_displayMode.swapInterval) {
        case GameSwapInterval::UNLIMITED_FPS:
        case GameSwapInterval::USE_VALUE_CAP:
            SDL_GL_SetSwapInterval(0);
            break;
        default:
            SDL_GL_SetSwapInterval(static_cast<i32>(DEFAULT_SWAP_INTERVAL));
            break;
        }
#elif defined(VORB_IMPL_GRAPHICS_D3D)
        // TODO(Cristian): Swap interval
#endif
#elif defined(VORB_IMPL_UI_GLFW)
        // TODO: GLFW Impl
#elif defined(VORB_IMPL_UI_SFML)
        switch (m_displayMode.swapInterval) {
        case GameSwapInterval::UNLIMITED_FPS:
        case GameSwapInterval::USE_VALUE_CAP:
            VUI_WINDOW_HANDLE(m_window)->setVerticalSyncEnabled(false);
            break;
        default:
            // This is the best we can get with SFML
            VUI_WINDOW_HANDLE(m_window)->setVerticalSyncEnabled(true);
            break;
        }
#endif

    }
}
void vui::GameWindow::setMaxFPS(const f32& fpsLimit) {
    m_displayMode.maxFPS = fpsLimit;
}
void vui::GameWindow::setTitle(const cString title) const {
    if (!title) title = DEFAULT_TITLE;
#if defined(VORB_IMPL_UI_SDL)
    SDL_SetWindowTitle((SDL_Window*)m_window, title);
#elif defined(VORB_IMPL_UI_GLFW)
    glfwSetWindowTitle((GLFWwindow*)m_window, title);
#elif defined(VORB_IMPL_UI_SFML)
    VUI_WINDOW_HANDLE(m_window)->setTitle(title);
#endif
}

void vui::GameWindow::sync(ui32 frameTime) {
    pollInput();

#if defined(VORB_IMPL_UI_SDL)
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    SDL_GL_SwapWindow(VUI_WINDOW_HANDLE(m_window));
#elif defined(VORB_IMPL_GRAPHICS_D3D)
    VUI_CONTEXT(m_glc)->Present(nullptr, nullptr, nullptr, nullptr);
#endif
#elif defined(VORB_IMPL_UI_GLFW)
    glfwSwapBuffers(VUI_WINDOW_HANDLE(m_window));
    glfwPollEvents();
#elif defined(VORB_IMPL_UI_SFML)
    VUI_WINDOW_HANDLE(m_window)->display();
#endif

    // Limit FPS
    if (m_displayMode.swapInterval == GameSwapInterval::USE_VALUE_CAP) {
        f32 desiredFPS = 1000.0f / (f32)m_displayMode.maxFPS;
        ui32 sleepTime = (ui32)(desiredFPS - frameTime);
        if (desiredFPS > frameTime && sleepTime > 0) Sleep(sleepTime);
    }
}

vui::GraphicsContext vui::GameWindow::getContext() const {
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    return m_glc;
#elif defined(VORB_IMPL_GRAPHICS_D3D)
    return VUI_CONTEXT(m_glc);
#endif
}
i32 vui::GameWindow::getX() const {
    i32 v;
#if defined(VORB_IMPL_UI_SDL)
    SDL_GetWindowPosition(VUI_WINDOW_HANDLE(m_window), &v, nullptr);
#elif defined(VORB_IMPL_UI_GLFW)
    glfwGetWindowPos(VUI_WINDOW_HANDLE(m_window), &v, nullptr);
#elif defined(VORB_IMPL_UI_SFML)
    v = VUI_WINDOW_HANDLE(m_window)->getPosition().x;
#endif
    return v;
}
i32 vui::GameWindow::getY() const {
    i32 v;
#if defined(VORB_IMPL_UI_SDL)
    SDL_GetWindowPosition(VUI_WINDOW_HANDLE(m_window), nullptr, &v);
#elif defined(VORB_IMPL_UI_GLFW)
    glfwGetWindowPos(VUI_WINDOW_HANDLE(m_window), nullptr, &v);
#elif defined(VORB_IMPL_UI_SFML)
    v = VUI_WINDOW_HANDLE(m_window)->getPosition().y;
#endif
    return v;
}
i32v2 vui::GameWindow::getPosition() const {
    i32v2 v;
#if defined(VORB_IMPL_UI_SDL)
    SDL_GetWindowPosition(VUI_WINDOW_HANDLE(m_window), &v.x, &v.y);
#elif defined(VORB_IMPL_UI_GLFW)
    glfwGetWindowPos(VUI_WINDOW_HANDLE(m_window), &v.x, &v.y);
#elif defined(VORB_IMPL_UI_SFML)
   auto sfv = VUI_WINDOW_HANDLE(m_window)->getPosition();
   v.x = sfv.x;
   v.y = sfv.y;
#endif
   return v;
}

void vui::GameWindow::pollInput() {
#if defined(VORB_IMPL_UI_SDL)
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) continue;
#elif defined(VORB_IMPL_UI_SFML)
    sf::Event e;
    while (VUI_WINDOW_HANDLE(m_window)->pollEvent(e)) {
        vorb::ui::impl::InputDispatcherEventCatcher::onSFMLEvent(VUI_WINDOW_HANDLE(m_window), e);
    }
#endif
}

