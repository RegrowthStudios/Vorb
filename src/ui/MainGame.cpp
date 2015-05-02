#include "stdafx.h"
#include "ui/MainGame.h"

#include <thread>

#if defined(VORB_IMPL_UI_SDL)
#if defined(OS_WINDOWS)
#include <SDL/SDL.h>
#else
#include <SDL2/SDL.h>
#endif
#define MS_TIME (SDL_GetTicks())
#elif defined(VORB_IMPL_UI_GLFW)
#include <GLFW/glfw3.h>
#define MS_TIME ((ui32)(glfwGetTime() * 1000.0))
#elif defined(VORB_IMPL_UI_SFML)
#include <SFML/System/Clock.hpp>
// TODO: This is pretty effing hacky...
static ui32 getCurrentTime() {
    static sf::Clock clock;
    static ui32 lastMS = 0;

    lastMS = (ui32)clock.getElapsedTime().asMilliseconds();
    return lastMS;
}
#define MS_TIME getCurrentTime()
#endif

#include "../ImplGraphicsH.inl"

#include "graphics/GLStates.h"
#include "ui/IGameScreen.h"
#include "ui/InputDispatcher.h"
#include "graphics/GraphicsDevice.h"
#include "ui/ScreenList.h"
#include "utils.h"
#include "Timing.h"
#include "InputDispatcherEventCatcher.h"

vui::MainGame::MainGame() :
    m_screenList(this) {
    // Empty
}
vui::MainGame::~MainGame() {
    // Empty
}

bool vui::MainGame::init() {
    m_lastTime = {};
    m_curTime = {};

    // This Is Vital
    if (!initSystems()) return false;
    m_window.setTitle(nullptr);

    // Initialize Logic And Screens
    onInit();
    addScreens();

    // Try To Get A Screen
    m_screen = m_screenList.getCurrent();
    if (m_screen) {
        // Run The First Game Screen
        m_screen->setRunning();
        m_screen->onEntry(m_lastTime);
    }

    // Set last known time
    m_lastMS = MS_TIME;
    return true;
}
bool vui::MainGame::initSystems() {
    // Create The Window
    if (!m_window.init()) return false;

#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    // TODO: Replace With BlendState
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set A Default OpenGL State
    vg::DepthState::FULL.set();
    vg::RasterizerState::CULL_CLOCKWISE.set();
#elif defined(VORB_IMPL_GRAPHICS_D3D)

#endif

    return true;
}
void vui::MainGame::exitGame() {
    if (m_screen) {
        m_screen->onExit(m_lastTime);
        m_screen = nullptr;
    }
    m_screenList.destroy(m_lastTime);
    onExit();
    m_window.dispose();
    m_isRunning = false;
}

bool vui::MainGame::shouldTerminate() const {
    return !m_isRunning || m_window.shouldQuit() || m_screen == nullptr;
}
bool vui::MainGame::checkScreenChange() {
    // If no screen, then the frame should not do anything
    if (!m_screen) return true;

    switch (m_screen->getState()) {
    case ScreenState::CHANGE_NEXT:
        m_screen->onExit(m_curTime);
        m_screen = m_screenList.moveNext();
        if (m_screen != nullptr) {
            m_screen->setRunning();
            m_screen->onEntry(m_curTime);
        }
        return true;
    case ScreenState::CHANGE_PREVIOUS:
        m_screen->onExit(m_curTime);
        m_screen = m_screenList.movePrevious();
        if (m_screen != nullptr) {
            m_screen->setRunning();
            m_screen->onEntry(m_curTime);
        }
        return true;
    case ScreenState::EXIT_APPLICATION:
        m_isRunning = false;
        return true;
    default:
        // No change occurs otherwise
        return false;
    }
}

void vui::MainGame::run() {
#if defined(VORB_IMPL_UI_SDL)
    // Initialize everything except SDL audio and SDL haptic feedback.
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK);

    // Make sure we are using hardware acceleration
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
#elif defined(VORB_IMPL_GRAPHICS_D3D)
    // Empty
#endif

#elif defined(VORB_IMPL_UI_GLFW)
    glfwInit();
#elif defined(VORB_IMPL_UI_SFML)
    // Nothing to do...
#endif

    // For counting the fps
    FpsCounter fpsCounter;

    // Game Loop
    if (init()) {
        m_isRunning = true;
        while (!shouldTerminate()) {
            // Start the FPS counter
            fpsCounter.beginFrame();

            // Refresh time information for this frame
            refreshElapsedTime();

            // Scree logic
            if (!checkScreenChange()) {
                // Update
                onUpdateFrame();
                if (!checkScreenChange()) {
                    // Render
                    onRenderFrame();
                }
            }

            // Swap buffers and synchronize time-step and window input
            ui32 curMS = MS_TIME;
            m_window.sync(curMS - m_lastMS);

            // Get the FPS
            m_fps = fpsCounter.endFrame();
        }

        // Exit application logic
        exitGame();
    }


#if defined(VORB_IMPL_UI_SDL)
    SDL_Quit();
#elif defined(VORB_IMPL_UI_GLFW)
    glfwTerminate();
#elif defined(VORB_IMPL_UI_SFML)
    // Don't have to do anything
#endif
}

void vui::MainGame::refreshElapsedTime() {
    ui32 ct = MS_TIME;
    f64 et = (ct - m_lastMS) / 1000.0;
    m_lastMS = ct;

    m_lastTime = m_curTime;
    m_curTime.elapsed = et;
    m_curTime.total += et;
}
void vui::MainGame::onUpdateFrame() {
    // Perform the screen's update logic
    m_screen->update(m_curTime);
}
void vui::MainGame::onRenderFrame() {
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    // TODO: Investigate Removing This
    glViewport(0, 0, m_window.getWidth(), m_window.getHeight());
#elif defined(VORB_IMPL_GRAPHICS_D3D)
    {
#if defined(VORB_DX_9)
        D3DVIEWPORT9 vp;
        vp.X = 0;
        vp.Y = 0;
        vp.Width = m_window.getWidth();
        vp.Height = m_window.getHeight();
        vp.MinZ = 0.0f;
        vp.MaxZ = 1.0f;
        VG_DX_DEVICE(m_window.getContext())->SetViewport(&vp);
#endif
    }
    VG_DX_DEVICE(m_window.getContext())->BeginScene();
#endif

    // Draw the screen
    m_screen->draw(m_curTime);

#if defined(VORB_IMPL_GRAPHICS_D3D)
    VG_DX_DEVICE(m_window.getContext())->EndScene();
#endif
}

