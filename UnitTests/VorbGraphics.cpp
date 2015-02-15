#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Graphics_

#include <glm/gtx/transform.hpp>
#include <include/Vorb.h>
#include <include/graphics/ImageIO.h>
#include <include/graphics/SpriteBatch.h>
#include <include/graphics/SpriteFont.h>
#include <include/graphics/Texture.h>
#include <include/graphics/GLProgram.h>
#include <include/ui/InputDispatcher.h>
#include <include/ui/MainGame.h>
#include <include/ui/IGameScreen.h>
#include <include/graphics/GLStates.h>
#include <include/colors.h>
#include <include/ui/ScreenList.h>
#include <include/MeshGenerators.h>

struct ImageTestFormats {
public:
    vg::ImageIOFormat format;
    vg::TextureInternalFormat texFormatInternal;
    vg::TextureFormat texFormat;
    vg::TexturePixelType pixelType;
};

class ImageViewer : public vui::IGameScreen {
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
        m_imageFormat = m_testFormats[0];
        m_hooks.addAutoHook(&vui::InputDispatcher::window.onFile, [&] (Sender, const vui::WindowFileEvent& e) {
            auto bmp = vg::ImageIO().load(e.file, m_imageFormat.format);
            this->m_bmp = bmp;
        });
        m_hooks.addAutoHook(&vui::InputDispatcher::key.onKeyDown, [&] (Sender, const vui::KeyEvent& e) {
            switch (e.keyCode) {
            case VKEY_1:
            case VKEY_2:
            case VKEY_3:
            case VKEY_4:
            case VKEY_5:
            case VKEY_6:
                m_imageFormat = m_testFormats[e.keyCode - VKEY_1];
                break;
            case VKEY_S:
            {
                           ui8 pix[16] = {
                               0, 0, 0, 255u,
                               255, 0, 0, 255u,
                               0, 255, 0, 255u,
                               0, 0, 255, 255u
                           };
                           vg::ImageIO().save("TestImageIO.png", pix, 2, 2, vg::ImageIOFormat::RGBA_UI8);
            }
                break;
            default:
                break;
            }
        });

        m_sb.init();
        m_tex = {};
        m_tex.width = m_tex.height = 1;
        m_bmp = {};

        glGenTextures(1, &m_tex.id);
        glBindTexture(GL_TEXTURE_2D, m_tex.id);
        ui32 pix = 0xffffffffu;
        glTexImage2D(GL_TEXTURE_2D, 0, 
            (VGEnum)m_imageFormat.texFormatInternal,
            1, 1, 0, 
            (VGEnum)m_imageFormat.texFormat,
            (VGEnum)m_imageFormat.pixelType,
            &pix);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        vg::SamplerState::LINEAR_WRAP.set(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    virtual void onExit(const vui::GameTime& gameTime) {
        m_sb.dispose();
        if (m_bmp.data) vg::ImageIO::free(m_bmp);
        if (m_tex.id != 0) glDeleteTextures(1, &m_tex.id);
    }

    virtual void update(const vui::GameTime& gameTime) {
        if (m_bmp.data) {
            glBindTexture(GL_TEXTURE_2D, m_tex.id);
            m_tex.width = m_bmp.width;
            m_tex.height = m_bmp.height;
            glTexImage2D(GL_TEXTURE_2D, 0,
                (VGEnum)m_imageFormat.texFormatInternal,
                m_tex.width, m_tex.height, 0,
                (VGEnum)m_imageFormat.texFormat,
                (VGEnum)m_imageFormat.pixelType,
                m_bmp.data);
            glBindTexture(GL_TEXTURE_2D, 0);
            vg::ImageIO::free(m_bmp);
            m_bmp = {};
        }
    }
    virtual void draw(const vui::GameTime& gameTime) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        m_sb.begin();
        m_sb.draw(m_tex.id, f32v2(0, 0), f32v2(400, 400), color::White);
        m_sb.end(vg::SpriteSortMode::NONE);
        m_sb.renderBatch(f32v2(800, 600));
    }
private:
    static ImageTestFormats m_testFormats[6];

    AutoDelegatePool m_hooks;
    vg::BitmapResource m_bmp;
    vg::Texture m_tex;
    vg::SpriteBatch m_sb;
    ImageTestFormats m_imageFormat;
};
ImageTestFormats ImageViewer::m_testFormats[6] = {
    {
        vg::ImageIOFormat::RGB_UI8,
        vg::TextureInternalFormat::RGB8,
        vg::TextureFormat::RGB,
        vg::TexturePixelType::UNSIGNED_BYTE
    },
    {
        vg::ImageIOFormat::RGBA_UI8,
        vg::TextureInternalFormat::RGBA8,
        vg::TextureFormat::RGBA,
        vg::TexturePixelType::UNSIGNED_BYTE
    },
    {
        vg::ImageIOFormat::RGB_UI16,
        vg::TextureInternalFormat::RGB16,
        vg::TextureFormat::RGB,
        vg::TexturePixelType::UNSIGNED_SHORT
    },
    {
        vg::ImageIOFormat::RGBA_UI16,
        vg::TextureInternalFormat::RGBA16,
        vg::TextureFormat::RGBA,
        vg::TexturePixelType::UNSIGNED_SHORT
    },
    {
        vg::ImageIOFormat::RGB_F32,
        vg::TextureInternalFormat::RGB32F,
        vg::TextureFormat::RGB,
        vg::TexturePixelType::FLOAT
    },
    {
        vg::ImageIOFormat::RGBA_F32,
        vg::TextureInternalFormat::RGBA32F,
        vg::TextureFormat::RGBA,
        vg::TexturePixelType::FLOAT
    }
};

class FontViewer : public vui::IGameScreen {
public:
    virtual i32 getNextScreen() const {
        return SCREEN_INDEX_NO_SCREEN;
    }
    virtual i32 getPreviousScreen() const {
        return SCREEN_INDEX_NO_SCREEN;
    }

    virtual void build() {
        // Empty
    }
    virtual void destroy(const vui::GameTime& gameTime) {
        // Empty
    }

    virtual void onEntry(const vui::GameTime& gameTime) {
        batch.init();
        font.init("Data/chintzy.ttf", 32);
    }
    virtual void onExit(const vui::GameTime& gameTime) {
        batch.dispose();
        font.dispose();
    }

    virtual void update(const vui::GameTime& gameTime) {
        // Empty
    }
    virtual void draw(const vui::GameTime& gameTime) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        batch.begin();
        batch.drawString(&font, "Hello World", f32v2(10, 10), 80.0f, 1.0f, color4(1.0f, 1.0f, 1.0f));
        batch.end();
        batch.renderBatch(f32v2(800, 600));
    }

    vg::SpriteFont font;
    vg::SpriteBatch batch;
};

class TorusViewer : public vui::IGameScreen {
public:
    virtual i32 getNextScreen() const {
        return SCREEN_INDEX_NO_SCREEN;
    }
    virtual i32 getPreviousScreen() const {
        return SCREEN_INDEX_NO_SCREEN;
    }

    virtual void build() {
        pitchInput = 0;
        yawInput = 0;
        pool.addAutoHook(&vui::InputDispatcher::key.onKeyDown, [&] (Sender, const vui::KeyEvent& e) {
            if (e.repeatCount > 0) return;
            switch (e.keyCode) {
            case VKEY_W: pitchInput += 1; break;
            case VKEY_S: pitchInput -= 1; break;
            case VKEY_A: yawInput -= 1; break;
            case VKEY_D: yawInput += 1; break;
            }
        });
        pool.addAutoHook(&vui::InputDispatcher::key.onKeyUp, [&] (Sender, const vui::KeyEvent& e) {
            switch (e.keyCode) {
            case VKEY_W: pitchInput -= 1; break;
            case VKEY_S: pitchInput += 1; break;
            case VKEY_A: yawInput += 1; break;
            case VKEY_D: yawInput -= 1; break;
            }
        });
    }
    virtual void destroy(const vui::GameTime& gameTime) {
        pool.dispose();
    }

    virtual void onEntry(const vui::GameTime& gameTime) {
        glGenBuffers(1, &verts);
        glGenBuffers(1, &inds);
        glGenVertexArrays(1, &vdecl);
        program.init();
        program.addShader(vg::ShaderType::VERTEX_SHADER, R"(
uniform mat4 unWVP;
in vec4 vPosition;
out vec3 fPosition;
void main() {
    fPosition = vPosition.xyz;
    gl_Position = unWVP * vPosition;
}
)");
        program.addShader(vg::ShaderType::FRAGMENT_SHADER, R"(
uniform vec2 unShift;
uniform sampler2D unTexture;
in vec3 fPosition;
out vec4 pColor;
void main() {
    vec3 n = normalize(fPosition);
    float u = atan(n.z, n.x) / 6.28;
    float v = (asin(n.y) / 1.57) * 0.25 + 0.5;
    vec2 coords = vec2(u, v) + unShift;
    coords = mod(coords, 1.0);
    pColor = texture(unTexture, coords);
}
)");
        program.link();
        program.initAttributes();
        program.initUniforms();

        std::vector<ui32> iData;
        std::vector<f32v3> vData;
        vcore::mesh::generateIcosphereMesh(4, iData, vData);
        glBindBuffer(GL_ARRAY_BUFFER, verts);
        glBufferData(GL_ARRAY_BUFFER, vData.size() * sizeof(f32v3), vData.data(), GL_STATIC_DRAW);
        glBindVertexArray(vdecl);
        glEnableVertexAttribArray(program.getAttribute("vPosition"));
        glVertexAttribPointer(program.getAttribute("vPosition"), 3, GL_FLOAT, false, sizeof(f32v3), nullptr);
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inds);
        indsCount = iData.size();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, iData.size() * sizeof(ui32), iData.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        auto bmp = vg::ImageIO().load("data/TW.jpg", vg::ImageIOFormat::RGBA_UI8);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bmp.width, bmp.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp.data);
        vg::ImageIO::free(bmp);
        vg::SamplerState::POINT_WRAP.set(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    virtual void onExit(const vui::GameTime& gameTime) {
        glDeleteBuffers(1, &verts);
        glDeleteBuffers(1, &inds);
        glDeleteVertexArrays(1, &vdecl);
        glDeleteTextures(1, &texture);
        program.dispose();
    }

    virtual void update(const vui::GameTime& gameTime) {
        yaw += (f32)(gameTime.elapsed * yawInput);
        pitch += (f32)(gameTime.elapsed * pitchInput);
        yaw = fmod(yaw + 6.28f, 6.28f);
        if (pitch > 1.5f) pitch = 1.5f;
        else if (pitch < -1.5f) pitch = -1.5f;
    }
    virtual void draw(const vui::GameTime& gameTime) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        f32v3 eye;
        eye.x = (f32)(cos(yaw) * cos(pitch));
        eye.z = (f32)(sin(yaw) * cos(pitch));
        eye.y = (f32)(sin(pitch));
        eye *= 3.0f;
        f32m4 wvp = glm::perspectiveFov(90.0f, 800.0f, 600.0f, 0.01f, 100.0f) * glm::lookAt(f32v3(0.0f, 0.0f, 2.1f), f32v3(0.0f), f32v3(0.0f, 1.0f, 0.0f));

        program.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(program.getUniform("unTexture"), 0);
        glUniformMatrix4fv(program.getUniform("unWVP"), 1, false, &wvp[0][0]);
#define PI_2 (3.14159f * 2)
        glUniform2f(program.getUniform("unShift"), (yaw / PI_2), (pitch / PI_2));
        glBindVertexArray(vdecl);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inds);
        glDrawElements(GL_TRIANGLES, indsCount, GL_UNSIGNED_INT, nullptr);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    i32 yawInput;
    i32 pitchInput;
    AutoDelegatePool pool;

    vg::GLProgram program;
    VGVertexBuffer verts;
    VGVertexArray vdecl;
    VGIndexBuffer inds;
    VGTexture texture;
    ui32 indsCount;

    f32 yaw = 0;
    f32 pitch = 0;
};

class VGTestApp : public vui::MainGame {
public:
    VGTestApp(vui::IGameScreen* s) :
        screen(s) {
        // Empty
    }

    virtual void onInit() {
    }
    virtual void addScreens() {
        _screenList->addScreen(screen);
        _screenList->setScreen(0);
    }
    virtual void onExit() {
        delete screen;
        screen = nullptr;
    }

    vui::IGameScreen* screen = nullptr;
};


TEST(FontViewer) {
    vorb::init(vorb::InitParam::ALL);
    { VGTestApp(new FontViewer).run(); }
    vorb::dispose(vorb::InitParam::ALL);
    return true;
}

TEST(ImageViewer) {
    vorb::init(vorb::InitParam::ALL);
    { VGTestApp(new ImageViewer).run(); }
    vorb::dispose(vorb::InitParam::ALL);
    return true;
}

TEST(TorusWorld) {
    vorb::init(vorb::InitParam::ALL);
    { VGTestApp(new TorusViewer).run(); }
    vorb::dispose(vorb::InitParam::ALL);
    return true;
}

