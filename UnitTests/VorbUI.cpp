#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_UI_

#include <include/Graphics.h>
#include <include/Timing.h>
#include <include/Vorb.h>
#include <include/graphics/ImageIO.h>
#include <include/graphics/SamplerState.h>
#include <include/graphics/SpriteBatch.h>
#include <include/graphics/SpriteFont.h>
#include <include/ui/CheckBox.h>
#include <include/ui/ComboBox.h>
#include <include/ui/Button.h>
#include <include/ui/Form.h>
#include <include/ui/FormScriptEnvironment.h>
#include <include/ui/IGameScreen.h>
#include <include/ui/InputDispatcher.h>
#include <include/ui/MainGame.h>
#include <include/ui/ScreenList.h>
#include <include/ui/Slider.h>
#include <include/ui/UIRenderer.h>

struct Vertex {
    f32v3 position;
    color4 color;
};

class TestScreen : public vui::IGameScreen {
public:
    virtual i32 getNextScreen() const override {
        return SCREEN_INDEX_NO_SCREEN;
    }
    virtual i32 getPreviousScreen() const override {
        return SCREEN_INDEX_NO_SCREEN;
    }
    virtual void build() override {
    }
    virtual void destroy(const vui::GameTime& gameTime) override {
    }
    virtual void onEntry(const vui::GameTime& gameTime) override {
    }
    virtual void onExit(const vui::GameTime& gameTime) override {
    }
    virtual void registerRendering(vg::Renderer& renderer) override {
    }
    virtual void onRenderFrame(const vui::GameTime& gameTime) override {
    }
    virtual void update(const vui::GameTime& gameTime) override {
    }
};

class WidgetTestScreen : public vui::IGameScreen {
public:
    virtual i32 getNextScreen() const override {
        return SCREEN_INDEX_NO_SCREEN;
    }
    virtual i32 getPreviousScreen() const override {
        return SCREEN_INDEX_NO_SCREEN;
    }
    virtual void build() override {
    }
    virtual void destroy(const vui::GameTime& gameTime) override {
    }
    virtual void onEntry(const vui::GameTime& gameTime) override {
        font.init("Data/chintzy.ttf", 32);
        form.init("main", this, f32v4(0.0f, 0.0f, (f32)m_viewportDims.x, (f32)m_viewportDims.y));

        // Load textures
        /* glGenTextures(1, &texture);
         glBindTexture(GL_TEXTURE_2D, texture);
         vg::ScopedBitmapResource bmp = vg::ImageIO().load("data/button_test.jpg", vg::ImageIOFormat::RGBA_UI8);
         if (bmp.data == nullptr) {
         std::cerr << "Error: Failed to load data/button_test.jpg\n";
         }
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bmp.width, bmp.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp.data);
         vg::SamplerState::POINT_WRAP.set(GL_TEXTURE_2D);

         glGenTextures(1, &checkedTexture);
         glBindTexture(GL_TEXTURE_2D, checkedTexture);
         vg::ScopedBitmapResource bmp2 = vg::ImageIO().load("data/checked_test.jpg", vg::ImageIOFormat::RGBA_UI8);
         if (bmp.data == nullptr) {
         std::cerr << "Error: Failed to load data/checked_test.jpg\n";
         }
         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bmp.width, bmp.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp2.data);
         vg::SamplerState::POINT_WRAP.set(GL_TEXTURE_2D);
         glBindTexture(GL_TEXTURE_2D, 0);*/

        // Load script file and init
     //   env.init(&form, &m_game->getWindow());
     //   env.loadForm("data/scripts/Form1.lua");
    }
    virtual void onExit(const vui::GameTime& gameTime) override {
        form.dispose();
       // font.dispose();
    }
    virtual void registerRendering(vg::Renderer& renderer) override {
        // Empty
    }
    virtual void update(const vui::GameTime& gameTime) override {
      //  form.update();
    }
    virtual void onRenderFrame(const vui::GameTime& gameTime) override {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        form.draw();
    }

    ui32v2 m_viewportDims = ui32v2(800, 600);
    vui::FormScriptEnvironment env;
    vui::Form form;
    vg::SpriteFont font;
    VGTexture texture, checkedTexture;
};
namespace {
    class VGTestApp : public vui::MainGame {
    public:
        VGTestApp(vui::IGameScreen* s) :
            screen(s) {
            // Empty
        }

        virtual void onInit() {
        }
        virtual void addScreens() {
            m_screenList.addScreen(screen);
            m_screenList.setScreen(0);
        }
        virtual void onExit() {
            delete screen;
            screen = nullptr;
        }

        vui::IGameScreen* screen = nullptr;
    };
}

TEST(MainGame) {
    vorb::init(vorb::InitParam::ALL);
    { VGTestApp(new TestScreen).run(); }
    vorb::dispose(vorb::InitParam::ALL);
    return true;
}

TEST(Widgets) {
    vorb::init(vorb::InitParam::ALL);
    { VGTestApp(new WidgetTestScreen).run(); }
    vorb::dispose(vorb::InitParam::ALL);
    return true;
}

TEST(InputFuncs) {
    vorb::init(vorb::InitParam::ALL);

    AutoDelegatePool pool;
    pool.addAutoHook(vui::InputDispatcher::key.onFocusGained, [] (Sender) {
        puts("Key focus gained");
    });
    pool.addAutoHook(vui::InputDispatcher::key.onFocusLost, [] (Sender) {
        puts("Key focus lost");
    });
    pool.addAutoHook(vui::InputDispatcher::key.onKeyDown, [] (Sender, const vui::KeyEvent& e) {
        printf("Key: %d-%d was pressed\n", e.keyCode, e.scanCode);
    });
    pool.addAutoHook(vui::InputDispatcher::key.onKeyUp, [] (Sender, const vui::KeyEvent& e) {
        printf("Key: %d-%d was released\n", e.keyCode, e.scanCode);
    });
    pool.addAutoHook(vui::InputDispatcher::key.onText, [] (Sender, const vui::TextEvent& e) {
        char c[2] = { e.text[0], 0 };
        printf("Text: %s\n", c);
    });
    pool.addAutoHook(vui::InputDispatcher::mouse.onButtonDown, [] (Sender, const vui::MouseButtonEvent& e) {
        printf("Mouse: %d was pressed at (%d,%d)\n", e.button, e.x, e.y);
    });
    pool.addAutoHook(vui::InputDispatcher::mouse.onButtonUp, [] (Sender, const vui::MouseButtonEvent& e) {
        printf("Mouse: %d was released at (%d,%d)\n", e.button, e.x, e.y);
    });
    pool.addAutoHook(vui::InputDispatcher::mouse.onFocusGained, [] (Sender, const vui::MouseEvent& e) {
        printf("Mouse: gained at (%d,%d)\n", e.x, e.y);
    });
    pool.addAutoHook(vui::InputDispatcher::mouse.onFocusLost, [] (Sender, const vui::MouseEvent& e) {
        printf("Mouse: lost at (%d,%d)\n", e.x, e.y);
    });
    pool.addAutoHook(vui::InputDispatcher::mouse.onMotion, [] (Sender, const vui::MouseMotionEvent& e) {
        printf("Mouse: motion (%d,%d) at (%d,%d)\n", e.dx, e.dy, e.x, e.y);
    });
    pool.addAutoHook(vui::InputDispatcher::mouse.onWheel, [] (Sender, const vui::MouseWheelEvent& e) {
        printf("Mouse: scroll (%d,%d) at (%d,%d)\n", e.dx, e.dy, e.x, e.y);
    });
    pool.addAutoHook(vui::InputDispatcher::window.onFile, [] (Sender, const vui::WindowFileEvent& e) {
        printf("File: %s\n", e.file);
    });
    pool.addAutoHook(vui::InputDispatcher::window.onResize, [] (Sender, const vui::WindowResizeEvent& e) {
        printf("Resize: (%d,%d)\n", e.w, e.h);
    });


    { VGTestApp(new TestScreen).run(); }

    pool.dispose();

    vorb::dispose(vorb::InitParam::ALL);
    return true;
}

TEST(SoloWindow) {
    vorb::init(vorb::InitParam::GRAPHICS);

    vui::GameWindow window;
    window.init();

#if defined(VORB_IMPL_GRAPHICS_OPENGL)
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
#elif defined(VORB_IMPL_GRAPHICS_D3D)
    auto dev = (IDirect3DDevice9*)window.getContext();
    f32m4 mat = vmath::perspectiveFov(90.0f, 800.0f, 600.0f, 0.01f, 100.0f);
    dev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&mat[0][0]);
    dev->SetRenderState(D3DRS_AMBIENT, RGB(255, 255, 255));
    dev->SetRenderState(D3DRS_LIGHTING, false);
    dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    dev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
    dev->SetFVF((D3DFVF_XYZ | D3DFVF_DIFFUSE));
    IDirect3DVertexBuffer9* tri_buffer = nullptr;
    void* pData;
    Vertex aTriangle[3] = {
        { f32v3(-1.0f, -1.0f, -2.0f), color4(0x00, 0xff, 0x00, 0xff) },
        { f32v3(0.0f, 1.0f, -3.0f), color4(0x00, 0x00, 0xff, 0xff) },
        { f32v3(1.0f, 0.0f, -3.0f), color4(0xff, 0x00, 0x00, 0xff) }
    };

    dev->CreateVertexBuffer(sizeof(aTriangle), D3DUSAGE_WRITEONLY,
        (D3DFVF_XYZ | D3DFVF_DIFFUSE),
        D3DPOOL_MANAGED, &tri_buffer, NULL);
    tri_buffer->Lock(0, sizeof(pData), (void**)&pData, 0);
    memcpy(pData, aTriangle, sizeof(aTriangle));
    tri_buffer->Unlock();
#endif

    PreciseTimer timer;
    timer.start();
    while (!window.shouldQuit()) {
        // Do something here
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#elif defined(VORB_IMPL_GRAPHICS_D3D)
        dev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

        dev->BeginScene();
        dev->SetStreamSource(0, tri_buffer, 0, sizeof(Vertex));
        dev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
        dev->EndScene();
#endif

        // Synchronize frame-step
        ui32 ms = (ui32)timer.stop();
        timer.start();

        // Update the window
        window.sync(ms);
    }
    window.dispose();

    vorb::dispose(vorb::InitParam::GRAPHICS);
    return true;
}
