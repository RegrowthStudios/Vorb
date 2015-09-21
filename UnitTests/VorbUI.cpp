#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_UI_

#include <d3dcompiler.h>
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
#include <include/ui/OSWindow.h>


struct Vertex {
    f32v3 position;
    color4 color;
};

class TestScreen : public vui::IGameScreen {
public:
    virtual i32 getNextScreen() const {
        return SCREEN_INDEX_NO_SCREEN;
    }
    virtual i32 getPreviousScreen() const {
        return SCREEN_INDEX_NO_SCREEN;
    }
    virtual void build() {
        auto& dispatcher = m_game->getWindow().getDispatcher();
        pool.addAutoHook(dispatcher.key.onFocusGained, [] (Sender) {
            puts("Key focus gained");
        });
        pool.addAutoHook(dispatcher.key.onFocusLost, [] (Sender) {
            puts("Key focus lost");
        });
        pool.addAutoHook(dispatcher.key.onKeyDown, [] (Sender, const vui::KeyEvent& e) {
            printf("Key: %d-%d was pressed\n", e.keyCode, e.scanCode);
        });
        pool.addAutoHook(dispatcher.key.onKeyUp, [] (Sender, const vui::KeyEvent& e) {
            printf("Key: %d-%d was released\n", e.keyCode, e.scanCode);
        });
        pool.addAutoHook(dispatcher.key.onText, [] (Sender, const vui::TextEvent& e) {
            char c[2] = { e.text[0], 0 };
            printf("Text: %s\n", c);
        });
        pool.addAutoHook(dispatcher.mouse.onButtonDown, [] (Sender, const vui::MouseButtonEvent& e) {
            printf("Mouse: %d was pressed at (%d,%d)\n", e.button, e.x, e.y);
        });
        pool.addAutoHook(dispatcher.mouse.onButtonUp, [] (Sender, const vui::MouseButtonEvent& e) {
            printf("Mouse: %d was released at (%d,%d)\n", e.button, e.x, e.y);
        });
        pool.addAutoHook(dispatcher.mouse.onFocusGained, [] (Sender, const vui::MouseEvent& e) {
            printf("Mouse: gained at (%d,%d)\n", e.x, e.y);
        });
        pool.addAutoHook(dispatcher.mouse.onFocusLost, [] (Sender, const vui::MouseEvent& e) {
            printf("Mouse: lost at (%d,%d)\n", e.x, e.y);
        });
        pool.addAutoHook(dispatcher.mouse.onMotion, [] (Sender, const vui::MouseMotionEvent& e) {
            printf("Mouse: motion (%d,%d) at (%d,%d)\n", e.dx, e.dy, e.x, e.y);
        });
        pool.addAutoHook(dispatcher.mouse.onWheel, [] (Sender, const vui::MouseWheelEvent& e) {
            printf("Mouse: scroll (%d,%d) at (%d,%d)\n", e.dx, e.dy, e.x, e.y);
        });
        pool.addAutoHook(vui::InputDispatcher::onDragDrop, [] (Sender, const vui::DragDropEvent& e) {
            printf("File: %s\n", e.path);
        });
        pool.addAutoHook(dispatcher.window.onResize, [] (Sender, const vui::WindowResizeEvent& e) {
            printf("Resize: (%d,%d)\n", e.w, e.h);
        });
    }
    virtual void destroy(const vui::GameTime& gameTime) {
        pool.dispose();
    }
    virtual void onEntry(const vui::GameTime& gameTime) {
    }
    virtual void onExit(const vui::GameTime& gameTime) {
    }
    virtual void update(const vui::GameTime& gameTime) {
    }
    virtual void draw(const vui::GameTime& gameTime) {
    }

    AutoDelegatePool pool;
};

class WidgetTestScreen : public vui::IGameScreen {
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
        //font.init("Data/chintzy.ttf", 32);
        //form.init("main", this, f32v4(0.0f, 0.0f, (f32)m_viewportDims.x, (f32)m_viewportDims.y));

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
    virtual void onExit(const vui::GameTime& gameTime) {
        //form.dispose();
       // font.dispose();
    }
    virtual void update(const vui::GameTime& gameTime) {
      //  form.update();
    }
    virtual void draw(const vui::GameTime& gameTime) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //form.draw();
    }

    ui32v2 m_viewportDims = ui32v2(800, 600);
    vui::FormScriptEnvironment env;
    //vui::Form form;
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
    { VGTestApp(new TestScreen).run(); }
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
    auto dev = (ID3D11Device*)window.getContext();
    ID3D11DeviceContext* context;
    dev->GetImmediateContext(&context);

    ID3D11Buffer* tri_buffer = nullptr;
    void* pData;
    Vertex aTriangle[3] = {
        { f32v3(-1.0f, -1.0f, 0.0f), color4(0x00, 0xff, 0x00, 0xff) },
        { f32v3(0.0f, 1.0f, 0.0f), color4(0x00, 0x00, 0xff, 0xff) },
        { f32v3(1.0f, 0.0f, 0.0f), color4(0xff, 0x00, 0x00, 0xff) }
    };

    // Fill in a buffer description.
    D3D11_BUFFER_DESC bufferDesc;
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(Vertex) * 3;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    // Fill in the subresource data.
    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = aTriangle;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;
    dev->CreateBuffer(&bufferDesc, &InitData, &tri_buffer);

    ID3D11VertexShader* fxVertex;
    ID3D11InputLayout* vLayout;
    {
        char src[] = R"(
struct VSInput {
    float4 pos : POSITION0;
    float4 color : COLOR0;
};
struct VSOutput {
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
};

VSOutput main(VSInput input) {
    VSOutput res;
    res.pos = input.pos;
    res.color = input.color;
    return res;
}
)";
        ID3DBlob* bytecode;
        printf("Size of source: %d\n", sizeof(src));
        auto hr = D3DCompile(src, sizeof(src), nullptr, nullptr, nullptr, "main", "vs_4_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &bytecode, nullptr);
        hr = dev->CreateVertexShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), nullptr, &fxVertex);

        D3D11_INPUT_ELEMENT_DESC vDesc[2] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(Vertex, position), D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, offsetof(Vertex, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };
        hr = dev->CreateInputLayout(vDesc, 2, bytecode->GetBufferPointer(), bytecode->GetBufferSize(), &vLayout);
        bytecode->Release();

    }
    ID3D11PixelShader* fxPixel;
    {
        char src[] = R"(
struct PSInput {
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

float4 main(PSInput input) : SV_TARGET {
    return input.color;
}
)";
        ID3DBlob* bytecode;
        printf("Size of source: %d\n", sizeof(src));
        auto hr = D3DCompile(src, sizeof(src), nullptr, nullptr, nullptr, "main", "ps_4_0", D3DCOMPILE_OPTIMIZATION_LEVEL3, 0, &bytecode, nullptr);
        hr = dev->CreatePixelShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), nullptr, &fxPixel);
        bytecode->Release();
    }

#endif

    vorb::PreciseTimer timer;
    timer.start();
    while (!window.shouldQuit()) {
        // Do something here
#if defined(VORB_IMPL_GRAPHICS_OPENGL)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#elif defined(VORB_IMPL_GRAPHICS_D3D)
        f32 clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
        context->ClearRenderTargetView(window.getMainRenderTargetView(), clearColor);
 
        ui32 vertSize = sizeof(Vertex);
        ui32 vertOffset = 0;
        context->IASetVertexBuffers(0, 1, &tri_buffer, &vertSize, &vertOffset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context->IASetInputLayout(vLayout);
        context->VSSetShader(fxVertex, nullptr, 0);
        context->PSSetShader(fxPixel, nullptr, 0);
        context->Draw(3, 0);
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

TEST(OSWindow) {
    vorb::init(vorb::InitParam::ALL);

    {
        vui::OSWindow window {};
        vui::OSWindowSettings settings {};
        settings.width = 800;
        settings.height = 600;
        settings.isBorderless = false;
        settings.isFullscreen = false;
        settings.isResizable = false;
        vui::OSWindow::create(window, settings);

        for (size_t i = 0; i < 20; i++) {

            Sleep(100);
        }
    }

    vorb::dispose(vorb::InitParam::ALL);
    return true;
}
