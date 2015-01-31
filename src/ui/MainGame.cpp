#include "stdafx.h"
#include "ui/MainGame.h"

#include <thread>

#if defined(VORB_IMPL_UI_SDL)
#if defined(OS_WINDOWS)
#include <SDL/SDL.h>
#include <TTF/SDL_ttf.h>
#else
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#endif
#define MS_TIME (SDL_GetTicks())
#else
#include <GLFW/glfw3.h>
#define MS_TIME ((ui32)(glfwGetTime() * 1000.0))
#endif

#include "graphics/GLStates.h"
#include "ui/IGameScreen.h"
#include "ui/InputDispatcher.h"
#include "graphics/GraphicsDevice.h"
#include "ui/ScreenList.h"
#include "utils.h"
#include "Timing.h"

vui::MainGame::MainGame() : _fps(0) {
    m_fOnQuit = createDelegate<>([=] (Sender) {
        m_signalQuit = true;
    });
}
vui::MainGame::~MainGame() {
    // Empty
}

bool vui::MainGame::initSystems() {
    // Create The Window
    if (!_window.init()) return false;

    // Initialize input
    vui::InputDispatcher::init(&_window);
    vui::InputDispatcher::onQuit += m_fOnQuit;

    // Get The Machine's Graphics Capabilities
    _gDevice = new vg::GraphicsDevice(_window.getHandle());
    _gDevice->refreshInformation();

    // Set A Default OpenGL State
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    // TODO: Replace With BlendState
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    vg::DepthState::FULL.set();
    vg::RasterizerState::CULL_CLOCKWISE.set();
    vg::SamplerState::initPredefined();

    // Initialize Frame Buffer
    glViewport(0, 0, _window.getWidth(), _window.getHeight());

    // Initialize Fonts Library
#if defined(VORB_IMPL_UI_SDL) && defined(VORB_IMPL_FONT_SDL)
    if (TTF_Init() == -1) {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return false;
    }
#else
    // TODO: FreeType library
#endif
    return true;
}

void vui::MainGame::run() {
#if defined(VORB_IMPL_UI_SDL)
    // Initialize everything except SDL audio and SDL haptic feedback.
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK);

    // Make sure we are using hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
#else
    glfwInit();
#endif

    // For counting the fps
    FpsCounter fpsCounter;

    // Game Loop
    if (init()) {
        _isRunning = true;
        while (_isRunning) {
            // Start the FPS counter
            fpsCounter.beginFrame();
            // Refresh Time Information
            refreshElapsedTime();

            // Main Game Logic
            checkInput();
            if (!_isRunning) break;
            onUpdateFrame();
            onRenderFrame();

            // Swap buffers
            ui32 curMS = MS_TIME;
            _window.sync(curMS - _lastMS);

            // Get the FPS
            _fps = fpsCounter.endFrame();
        }
    }

#if defined(VORB_IMPL_UI_SDL)
    SDL_Quit();
#else
    glfwTerminate();
#endif
}
void vui::MainGame::exitGame() {
    if (_screen) {
        _screen->onExit(_lastTime);
    }
    if (_screenList) {
        _screenList->destroy(_lastTime);
    }
    vui::InputDispatcher::onQuit -= m_fOnQuit;
    delete m_fOnQuit;
    vui::InputDispatcher::dispose();
    _window.dispose();
    _isRunning = false;
}

bool vui::MainGame::init() {
    // This Is Vital
    if (!initSystems()) return false;
    _window.setTitle(nullptr);

    // Initialize Logic And Screens
    _screenList = new ScreenList(this);
    onInit();
    addScreens();

    // Try To Get A Screen
    _screen = _screenList->getCurrent();
    if (_screen == nullptr) {
        exitGame();
        return false;
    }

    // Run The First Game Screen
    _screen->setRunning();
    _lastTime = {};
    _curTime = {};
    _screen->onEntry(_lastTime);
    _lastMS = MS_TIME;

    return true;
}
void vui::MainGame::refreshElapsedTime() {
    ui32 ct = MS_TIME;
    f64 et = (ct - _lastMS) / 1000.0;
    _lastMS = ct;

    _lastTime = _curTime;
    _curTime.elapsed = et;
    _curTime.total += et;
}
void vui::MainGame::checkInput() {
#if defined(VORB_IMPL_UI_SDL)
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) continue;
#endif

    if (m_signalQuit) {
        m_signalQuit = false;
        exitGame();
    }
}

void vui::MainGame::onUpdateFrame() {
    if (_screen != nullptr) {
        switch (_screen->getState()) {
        case ScreenState::RUNNING:
            _screen->update(_curTime);
            break;
        case ScreenState::CHANGE_NEXT:
            _screen->onExit(_curTime);
            _screen = _screenList->moveNext();
            if (_screen != nullptr) {
                _screen->setRunning();
                _screen->onEntry(_curTime);
            }
            break;
        case ScreenState::CHANGE_PREVIOUS:
            _screen->onExit(_curTime);
            _screen = _screenList->movePrevious();
            if (_screen != nullptr) {
                _screen->setRunning();
                _screen->onEntry(_curTime);
            }
            break;
        case ScreenState::EXIT_APPLICATION:
            exitGame();
            return;
        default:
            return;
        }
    } else {
        exitGame();
        return;
    }
}

void vui::MainGame::onRenderFrame() {
    // TODO: Investigate Removing This
    glViewport(0, 0, _window.getWidth(), _window.getHeight());
    if (_screen != nullptr && _screen->getState() == ScreenState::RUNNING) {
        _screen->draw(_curTime);
    }
}
