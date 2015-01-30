#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Graphics_

#include <include/Vorb.h>
#include <include/graphics/ImageIO.h>
#include <include/graphics/SpriteBatch.h>
#include <include/graphics/Texture.h>
#include <include/ui/InputDispatcher.h>
#include <include/ui/MainGame.h>
#include <include/ui/IGameScreen.h>
#include <include/graphics/GLStates.h>
#include <include/colors.h>
#include <include/ui/ScreenList.h>

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

class VGTestApp : public vui::MainGame {
public:
    virtual void onInit() {
    }
    virtual void addScreens() {
        _screenList->addScreen(new ImageViewer);
        _screenList->setScreen(0);
    }
    virtual void onExit() {
    }
};


TEST(ImageViewer) {
    vorb::init(vorb::InitParam::ALL);
    { VGTestApp().run(); }
    vorb::dispose(vorb::InitParam::ALL);
    return true;
}
