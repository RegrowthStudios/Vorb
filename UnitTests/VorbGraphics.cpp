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
        m_hooks.addAutoHook(&vui::InputDispatcher::window.onFile, [&] (Sender, const vui::WindowFileEvent& e) {
            auto bmp = vg::ImageIO().load(e.file, vg::ImageIOFormat::RGBA_UI8);
            this->m_bmp = bmp;
        });

        m_sb.init();
        m_tex = {};
        m_tex.width = m_tex.height = 1;
        m_bmp = {};

        glGenTextures(1, &m_tex.id);
        glBindTexture(GL_TEXTURE_2D, m_tex.id);
        ui32 pix = 0xffffffffu;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pix);
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_tex.width, m_tex.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_bmp.data);
            glBindTexture(GL_TEXTURE_2D, 0);
            vg::ImageIO::free(m_bmp);
            m_bmp = {};
        }
    }
    virtual void draw(const vui::GameTime& gameTime) {
        m_sb.begin();
        m_sb.draw(m_tex.id, f32v2(0, 0), f32v2(400, 400), color::White);
        m_sb.end(vg::SpriteSortMode::NONE);
        m_sb.renderBatch(f32v2(800, 600));
    }
private:
    AutoDelegatePool m_hooks;
    vg::BitmapResource m_bmp;
    vg::Texture m_tex;
    vg::SpriteBatch m_sb;
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
