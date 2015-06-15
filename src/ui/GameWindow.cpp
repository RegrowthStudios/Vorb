#include "stdafx.h"
#include "ui/GameWindow.h"

#if defined(VORB_IMPL_UI_SDL)
#if defined(OS_WINDOWS)
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
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
KEG_TYPE_DEF(GraphicsContextProperties, vui::GraphicsContextProperties, kt) {
    using namespace keg;
    kt.addValue("Major", Value::value(&vui::GraphicsContextProperties::major));
    kt.addValue("Minor", Value::value(&vui::GraphicsContextProperties::minor));
    kt.addValue("Core", Value::value(&vui::GraphicsContextProperties::core));
    kt.addValue("Debugging", Value::value(&vui::GraphicsContextProperties::debugging));
    kt.addValue("SwapChainSize", Value::value(&vui::GraphicsContextProperties::swapChainSize));
}
KEG_TYPE_DEF(GameDisplayMode, vui::GameDisplayMode, kt) {
    using namespace keg;
    kt.addValue("ScreenWidth", Value::value(&vui::GameDisplayMode::screenWidth));
    kt.addValue("ScreenHeight", Value::value(&vui::GameDisplayMode::screenHeight));
    kt.addValue("IsFullscreen", Value::value(&vui::GameDisplayMode::isFullscreen));
    kt.addValue("IsBorderless", Value::value(&vui::GameDisplayMode::isBorderless));
    kt.addValue("SwapInterval", Value::custom(offsetof(vui::GameDisplayMode, swapInterval), "GameSwapInterval", true));
    kt.addValue("MaxFPS", Value::value(&vui::GameDisplayMode::maxFPS));
    kt.addValue("GraphicsContext", Value::custom(offsetof(vui::GameDisplayMode, context), "GraphicsContextProperties", false));
}

vui::GameWindow::GameWindow() :
    onQuit(this) {
    setDefaultSettings(&m_displayMode);
}
VORB_MOVABLE_DEF(vui::GameWindow, o) {
    std::swap(m_glc, o.m_glc);
    std::swap(m_window, o.m_window);
    std::swap(m_displayMode, o.m_displayMode);
    std::swap(m_quitSignal, o.m_quitSignal);

    // Swap events, but keep correct senders
    std::swap(onQuit, o.onQuit);
    this->onQuit.setSender(this);
    o.onQuit.setSender(&o);
    return *this;
}

bool vui::GameWindow::init(bool isResizable /*= true*/) {
    if (isInitialized()) return false;
    m_displayMode.isResizable = isResizable;

    // Attempt to read custom settings
    readSettings();
    // TODO(Ben): Needs comments or functions
#if defined(VORB_IMPL_UI_SDL)
    SDL_WindowFlags flags = (SDL_WindowFlags)DEFAULT_WINDOW_FLAGS;
    if (m_displayMode.isResizable) flags = (SDL_WindowFlags)(flags | SDL_WINDOW_RESIZABLE);
    if (m_displayMode.isBorderless) flags = (SDL_WindowFlags)(flags | SDL_WINDOW_BORDERLESS);
    if (m_displayMode.isFullscreen) flags = (SDL_WindowFlags)(flags | SDL_WINDOW_FULLSCREEN);
    m_window = SDL_CreateWindow(DEFAULT_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_displayMode.screenWidth, m_displayMode.screenHeight, flags);
    
    // Obtain the native OS handle to the window
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(VUI_WINDOW_HANDLE(m_window), &wmInfo);
#if defined(OS_WINDOWS)
    HWND hWnd = wmInfo.info.win.window;
#else
#error Plug'n'play your OS here
#endif

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
    D3DContext* d3dContext = new D3DContext;
    m_glc = d3dContext;
#if defined(VORB_DX_9)
    // Make COM object for D3D initialization
    VUI_COM(m_glc) = Direct3DCreate9(D3D_SDK_VERSION);

    { // Create D3D device context
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
#else
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 1;
    sd.BufferDesc.Width = 800;
    sd.BufferDesc.Height = 600;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = GetActiveWindow();
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;

    const D3D_FEATURE_LEVEL lvl[7] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2, 
        D3D_FEATURE_LEVEL_9_1 
    };

    UINT createDeviceFlags = 0;
#ifdef DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
        
    D3D_FEATURE_LEVEL featureLevelsSupported;
    ID3D11Device* device = nullptr;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, lvl, 7, D3D11_SDK_VERSION, &sd, &d3dContext->dxgi, &d3dContext->device, &featureLevelsSupported, &d3dContext->immediateContext);
    if (hr != S_OK) {
        hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, &lvl[1], 6, D3D11_SDK_VERSION, &sd, &d3dContext->dxgi, &d3dContext->device, &featureLevelsSupported, &d3dContext->immediateContext);
    }
    
    // Get a pointer to the back buffer and set it
    d3dContext->dxgi->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&d3dContext->backBufferTexture);
    d3dContext->device->CreateRenderTargetView(d3dContext->backBufferTexture, NULL, &d3dContext->backBufferRenderTargetView);
    d3dContext->immediateContext->OMSetRenderTargets(1, &d3dContext->backBufferRenderTargetView, NULL);
    
    // Setup the viewport
    D3D11_VIEWPORT vp;
    vp.Width = 640;
    vp.Height = 480;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    d3dContext->immediateContext->RSSetViewports(1, &vp);
#endif
#endif

#elif defined(VORB_IMPL_UI_GLFW)
    // Initialize OpenGL
    glfwMakeContextCurrent(VUI_WINDOW_HANDLE(m_window));
    m_glc = m_window;
#elif defined(VORB_IMPL_UI_SFML)
    VUI_WINDOW_HANDLE(m_window)->setActive(true);
    m_glc = m_window;
#endif

    // Check for a valid context
    if (m_glc == nullptr) {
        printf("Could Not Create OpenGL Context");
        return false;
    }

#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        printf("Glew failed to initialize. Your graphics card is probably WAY too old. Or you forgot to extract the .zip. It might be time for an upgrade :)");
        return false;
    }

    // Create default clear values
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);

    // Initialize Frame Buffer
    glViewport(0, 0, getWidth(), getHeight());
#elif defined(VORB_IMPL_GRAPHICS_D3D)
    // TODO(Cristian): Place some defaults?
#endif

    // TODO(Ben): Handle for other IMPLs.
#if defined(VORB_IMPL_UI_SDL)
    { // Get supported window resolutions
            SDL_DisplayMode mode;
        // TODO(Ben): Handle other displays indices.
        int displayIndex = 0;
        int numDisplayModes = SDL_GetNumDisplayModes(displayIndex);
        for (int i = 0; i < numDisplayModes; i++) {
            SDL_GetDisplayMode(displayIndex, i, &mode);
            ui32v2 res(mode.w, mode.h);
            if (i == 0 || m_supportedResolutions.back() != res) {
                m_supportedResolutions.push_back(res);
            }
        }
    }
#else
    throw 84;
#endif

    // Set More Display Settings
    setSwapInterval(m_displayMode.swapInterval, true);

    // Push input from this window and receive quit signals
    m_dispatcher->window.onClose += makeDelegate(*this, &GameWindow::onQuitSignal);
    vui::InputDispatcher::onQuit += makeDelegate(*this, &GameWindow::onQuitSignal);
    m_dispatcher->window.onResize += makeDelegate(*this, &GameWindow::onResize);
    m_quitSignal = false;

    return true;
}
void vui::GameWindow::dispose() {
    if (!isInitialized()) return;

    vui::InputDispatcher::onQuit -= makeDelegate(*this, &GameWindow::onQuitSignal);
    m_dispatcher->window.onClose -= makeDelegate(*this, &GameWindow::onQuitSignal);
    m_dispatcher->window.onResize -= makeDelegate(*this, &GameWindow::onResize);
    saveSettings();

#if defined(VORB_IMPL_UI_SDL)
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    if (m_glc) {
        SDL_GL_DeleteContext((SDL_GLContext)m_glc);
    }
#elif defined(VORB_IMPL_GRAPHICS_D3D)
    if (m_glc) {
        D3DContext* d3dContext = ((D3DContext*)m_glc);
        d3dContext->device->Release();
#if defined(VORB_DX_9)
        VUI_COM(m_glc)->Release();
#else
        d3dContext->backBufferRenderTargetView->Release();
        d3dContext->backBufferTexture->Release();
        d3dContext->immediateContext->Release();
        d3dContext->dxgi->Release();
#endif
        delete m_glc;
    }
#endif
    if (m_window) {
        SDL_DestroyWindow(VUI_WINDOW_HANDLE(m_window));
    }
#elif defined(VORB_IMPL_UI_GLFW)
    if (m_window) {
        glfwDestroyWindow(VUI_WINDOW_HANDLE(m_window));
    }
#elif defined(VORB_IMPL_UI_SFML)
    if (m_window) {
        VUI_WINDOW_HANDLE(m_window)->close();
        delete VUI_WINDOW_HANDLE(m_window);
    }
#endif

    // Get rid of dangling pointers
    m_window = nullptr;
    m_glc = nullptr;
}

void vui::GameWindow::setDefaultSettings(GameDisplayMode* mode) {
    mode->screenWidth = DEFAULT_WINDOW_WIDTH;
    mode->screenHeight = DEFAULT_WINDOW_HEIGHT;
    mode->isBorderless = false;
    mode->isFullscreen = false;
    mode->isResizable = true;
    mode->maxFPS = DEFAULT_MAX_FPS;
    mode->swapInterval = DEFAULT_SWAP_INTERVAL;

    // Set up default graphics context properties
#if defined(VORB_IMPL_GRAPHICS_D3D)
    mode->context.major = 9;
    mode->context.minor = 0;
#elif defined(VORB_IMPL_GRAPHICS_OPENGL)
    mode->context.major = 3;
    mode->context.minor = 2;
#endif
    mode->context.core = false;
    mode->context.debugging = true;
    mode->context.swapChainSize = 2;
}

void vui::GameWindow::readSettings() {
    vio::IOManager iom;
    cString data = iom.readFileToString(DEFAULT_APP_CONFIG_FILE);
    if (data) {
        keg::parse(&m_displayMode, data, "GameDisplayMode");
        delete[] data;
    } else {
        // If there is no app.config, save a default one.
        saveSettings();
    }
}
void vui::GameWindow::saveSettings() const {
    nString data = keg::write(&m_displayMode, "GameDisplayMode", nullptr);
    std::ofstream file(DEFAULT_APP_CONFIG_FILE);
    file << data << std::endl;
    file.flush();
    file.close();
}

void vui::GameWindow::setScreenSize(i32 w, i32 h, bool overrideCheck /*= false*/) {
    // Apply A Minimal State Change
    if ((overrideCheck || m_displayMode.screenWidth != w || m_displayMode.screenHeight != h) && !m_displayMode.isFullscreen){
        m_displayMode.screenWidth = w;
        m_displayMode.screenHeight = h;
#if defined(VORB_IMPL_UI_SDL)
        SDL_SetWindowSize(VUI_WINDOW_HANDLE(m_window), m_displayMode.screenWidth, m_displayMode.screenHeight);
        m_dispatcher->window.onResize({ w, h }); // TODO(Ben): This feels so dirty, but is necessary for LUA UI
#elif defined(VORB_IMPL_UI_SDL)
        glfwSetWindowSize(VUI_WINDOW_HANDLE(m_window), m_displayMode.screenWidth, m_displayMode.screenHeight);
#elif defined(VORB_IMPL_UI_SFML)
        VUI_WINDOW_HANDLE(m_window)->setSize(sf::Vector2u(m_displayMode.screenWidth, m_displayMode.screenHeight));
#endif
    }
}
void vui::GameWindow::setFullscreen(bool useFullscreen, bool overrideCheck /*= false*/) {
    if (overrideCheck || m_displayMode.isFullscreen != useFullscreen) {
        m_displayMode.isFullscreen = useFullscreen;
#if defined(VORB_IMPL_UI_SDL)
        SDL_SetWindowFullscreen(VUI_WINDOW_HANDLE(m_window), m_displayMode.isFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
#elif defined(VORB_IMPL_UI_GLFW)
        throw 84; // TODO: GLFW Impl
#elif defined(VORB_IMPL_UI_SFML)
        ui32 flags = sf::Style::None;
        if (!m_displayMode.isBorderless) flags |= sf::Style::Close;
        if (m_displayMode.isFullscreen) flags |= sf::Style::Fullscreen;
        VUI_WINDOW_HANDLE(m_window)->create(sf::VideoMode(m_displayMode.screenWidth, m_displayMode.screenHeight), DEFAULT_TITLE, flags);
#endif
    }
}
void vui::GameWindow::setBorderless(bool useBorderless, bool overrideCheck /*= false*/) {
    if ((overrideCheck || m_displayMode.isBorderless != useBorderless) && !m_displayMode.isFullscreen) {
        m_displayMode.isBorderless = useBorderless;
#if defined(VORB_IMPL_UI_SDL)
        SDL_SetWindowBordered((SDL_Window*)m_window, m_displayMode.isBorderless ? SDL_FALSE : SDL_TRUE);
#elif defined(VORB_IMPL_UI_GLFW)
        throw 84; // TODO: GLFW Impl
#elif defined(VORB_IMPL_UI_SFML)
        ui32 flags = sf::Style::None;
        if (!m_displayMode.isBorderless) flags |= sf::Style::Close;
        if (m_displayMode.isFullscreen) flags |= sf::Style::Fullscreen;
        VUI_WINDOW_HANDLE(m_window)->create(sf::VideoMode(m_displayMode.screenWidth, m_displayMode.screenHeight), DEFAULT_TITLE, flags);
#endif
    }
}

void vui::GameWindow::setSwapInterval(GameSwapInterval mode, bool overrideCheck /*= false*/) {
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
        throw 84; // TODO(Cristian): Swap interval
#endif
#elif defined(VORB_IMPL_UI_GLFW)
        throw 84; // TODO: GLFW Impl
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
void vui::GameWindow::setMaxFPS(f32 fpsLimit) {
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

void vui::GameWindow::setPosition(int x, int y) {
#if defined(VORB_IMPL_UI_SDL)
    SDL_SetWindowPosition((SDL_Window*)m_window, x, y);
#elif defined(VORB_IMPL_UI_GLFW)
    throw 84; // TODO(Ben): Implement
#elif defined(VORB_IMPL_UI_SFML)
    throw 84; // TODO(Ben): Implement
#endif
}

void vui::GameWindow::sync(ui32 frameTime) {
    pollInput();

#if defined(VORB_IMPL_UI_SDL)
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    SDL_GL_SwapWindow(VUI_WINDOW_HANDLE(m_window));
#elif defined(VORB_IMPL_GRAPHICS_D3D)
    // TODO(Cristian): All-around improvements to D3D commands everywhere
#if defined(VORB_DX_9)
    VUI_CONTEXT(m_glc)->Present(nullptr, nullptr, nullptr, nullptr);
#else
    ((D3DContext*)m_glc)->dxgi->Present(1, 0);
#endif
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
#if defined(VORB_DX_11)
ID3D11RenderTargetView* vui::GameWindow::getMainRenderTargetView() const {
    return ((D3DContext*)m_glc)->backBufferRenderTargetView;
}
#endif
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

void vorb::ui::GameWindow::onResize(Sender s, const WindowResizeEvent& e) {
    m_displayMode.screenWidth = e.w;
    m_displayMode.screenHeight = e.h;
}

void vorb::ui::GameWindow::onQuitSignal(Sender) {
    m_quitSignal = true;
}
