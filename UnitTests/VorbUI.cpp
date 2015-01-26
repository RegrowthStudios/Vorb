#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_UI_

#include <include/Vorb.h>
#include <include/ui/MainGame.h>
#include <include/ui/ScreenList.h>
#include <include/ui/IGameScreen.h>
#include <include/ui/InputDispatcher.h>

class TestScreen : public vui::IGameScreen {
public:
    virtual i32 getNextScreen() const {
        return SCREEN_INDEX_NO_SCREEN;
    }
    virtual i32 getPreviousScreen() const {
        return SCREEN_INDEX_NO_SCREEN;
    }
    virtual void build() {
    }
    virtual void destroy(const vui::GameTime& gameTime) {
    }
    virtual void onEntry(const vui::GameTime& gameTime) {
    }
    virtual void onExit(const vui::GameTime& gameTime) {
    }
    virtual void update(const vui::GameTime& gameTime) {
    }
    virtual void draw(const vui::GameTime& gameTime) {
    }
};
class App : public vui::MainGame {
public:
    App(i32 index) :
    m_index(index) {
        // Empty
    }

    virtual void onInit() {
        // Empty
    }
    virtual void addScreens() {
        _screenList->addScreen(new TestScreen);
        _screenList->setScreen(m_index);
    }
    virtual void onExit() {
        // Empty
    }
private:
    i32 m_index;
};

TEST(MainGame) {
    vorb::init(vorb::InitParam::ALL);
    { App(0).run(); }
    vorb::dispose(vorb::InitParam::ALL);
    return true;
}

TEST(InputFuncs) {
    vorb::init(vorb::InitParam::ALL);

    AutoDelegatePool pool;
    pool.addAutoHook(&vui::InputDispatcher::key.onFocusGained, [] (Sender) {
        puts("Key focus gained");
    });
    pool.addAutoHook(&vui::InputDispatcher::key.onFocusLost, [] (Sender) {
        puts("Key focus lost");
    });
    pool.addAutoHook(&vui::InputDispatcher::key.onKeyDown, [] (Sender, const vui::KeyEvent& e) {
        printf("Key: %d-%d was pressed\n", e.keyCode, e.scanCode);
    });
    pool.addAutoHook(&vui::InputDispatcher::key.onKeyUp, [] (Sender, const vui::KeyEvent& e) {
        printf("Key: %d-%d was released\n", e.keyCode, e.scanCode);
    });
    pool.addAutoHook(&vui::InputDispatcher::key.onText, [] (Sender, const vui::TextEvent& e) {
        char c[2] = { e.text[0], 0 };
        printf("Text: %s\n", c);
    });
    pool.addAutoHook(&vui::InputDispatcher::mouse.onButtonDown, [] (Sender, const vui::MouseButtonEvent& e) {
        printf("Mouse: %d was pressed at (%d,%d)\n", e.button, e.x, e.y);
    });
    pool.addAutoHook(&vui::InputDispatcher::mouse.onButtonUp, [] (Sender, const vui::MouseButtonEvent& e) {
        printf("Mouse: %d was released at (%d,%d)\n", e.button, e.x, e.y);
    });
    pool.addAutoHook(&vui::InputDispatcher::mouse.onFocusGained, [] (Sender, const vui::MouseEvent& e) {
        printf("Mouse: gained at (%d,%d)\n", e.x, e.y);
    });
    pool.addAutoHook(&vui::InputDispatcher::mouse.onFocusLost, [] (Sender, const vui::MouseEvent& e) {
        printf("Mouse: lost at (%d,%d)\n", e.x, e.y);
    });
    pool.addAutoHook(&vui::InputDispatcher::mouse.onMotion, [] (Sender, const vui::MouseMotionEvent& e) {
        printf("Mouse: motion (%d,%d) at (%d,%d)\n", e.dx, e.dy, e.x, e.y);
    });
    pool.addAutoHook(&vui::InputDispatcher::mouse.onWheel, [] (Sender, const vui::MouseWheelEvent& e) {
        printf("Mouse: scroll (%d,%d) at (%d,%d)\n", e.dx, e.dy, e.x, e.y);
    });
    pool.addAutoHook(&vui::InputDispatcher::window.onFile, [] (Sender, const vui::WindowFileEvent& e) {
        printf("File: %s\n", e.file);
    });
    pool.addAutoHook(&vui::InputDispatcher::window.onResize, [] (Sender, const vui::WindowResizeEvent& e) {
        printf("Resize: (%d,%d)\n", e.w, e.h);
    });


    { App(0).run(); }

    pool.dispose();

    vorb::dispose(vorb::InitParam::ALL);
    return true;
}
