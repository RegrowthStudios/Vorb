#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Graphics_

#include <random>

#include <glm/gtx/transform.hpp>
#include <SDL/SDL_events.h> // TODO(Cristian): remove
#include <include/MeshGenerators.h>
#include <include/Timing.h>
#include <include/Vorb.h>
#include <include/colors.h>
#include <include/graphics/GLProgram.h>
#include <include/graphics/GLStates.h>
#include <include/graphics/ImageIO.h>
#include <include/graphics/ModelIO.h>
#include <include/graphics/ShaderManager.h>
#include <include/graphics/SpriteBatch.h>
#include <include/graphics/SpriteFont.h>
#include <include/graphics/Texture.h>
#include <include/io/IOManager.h>
#include <include/ui/IGameScreen.h>
#include <include/ui/InputDispatcher.h>
#include <include/ui/MainGame.h>
#include <include/ui/ScreenList.h>
#include <include/graphics/IAdapter.h>
#include <include/graphics/IContext.h>
#include <include/graphics/IDevice.h>
#include <include/ui/OSWindow.h>

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
        auto& dispatcher = m_game->getWindow().getDispatcher();
        m_hooks.addAutoHook(dispatcher.onDragDrop, [&] (Sender, const vui::DragDropEvent& e) {
            auto bmp = vg::ImageIO().load(e.path, m_imageFormat.format);
            this->m_bmp = bmp;
        });
        m_hooks.addAutoHook(dispatcher.key.onKeyDown, [&] (Sender, const vui::KeyEvent& e) {
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
        m_sb.render(f32v2(800, 600));
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
        batch.render(f32v2(800, 600));
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
        auto& dispatcher = m_game->getWindow().getDispatcher();
        pool.addAutoHook(dispatcher.key.onKeyDown, [&] (Sender, const vui::KeyEvent& e) {
            if (e.repeatCount > 0) return;
            switch (e.keyCode) {
            case VKEY_W: pitchInput += 1; break;
            case VKEY_S: pitchInput -= 1; break;
            case VKEY_A: yawInput -= 1; break;
            case VKEY_D: yawInput += 1; break;
            }
        });
        pool.addAutoHook(dispatcher.key.onKeyUp, [&] (Sender, const vui::KeyEvent& e) {
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
    pColor = vec4(texture(unTexture, coords).rgb, 1.0);
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
        vg::ScopedBitmapResource bmp = vg::ImageIO().load("data/TW.jpg", vg::ImageIOFormat::RGBA_UI8);
        if (bmp.data == nullptr) {
            std::cerr << "Error: Failed to load data/TW.jpg\n";
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bmp.width, bmp.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp.data);
        vg::SamplerState::POINT_WRAP.set(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        spriteBatch.init();
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
        pitch = fmod(pitch + 6.28f, 6.28f);
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

        // Render the minimap
        spriteBatch.begin();
        // Draw minimap
        spriteBatch.draw(texture, f32v2(700.0f, 0.0f), f32v2(100.0f), color::White);
        // Draw location
        f32v2 pointSize(8.0f);
        spriteBatch.draw(texture, f32v2(700.0f + ((yaw / PI_2) * 100.0f),
            (100.0f - (pitch / PI_2) * 100.0f)) - pointSize.y / 2.0f, pointSize, color::Red);
        spriteBatch.end();
        spriteBatch.render(f32v2(800.0f, 600.0f));
        vg::SamplerState::POINT_WRAP.set(GL_TEXTURE_2D);
        vg::DepthState::FULL.set();
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
    vg::SpriteBatch spriteBatch;

    f32 yaw = 0;
    f32 pitch = 0;
};

class SpriteBatchTester : public vui::IGameScreen {
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

        // Init sprites
        std::mt19937 rgen(0);
        std::uniform_real_distribution<f32>x(0.0f, 800.0f);
        std::uniform_real_distribution<f32>y(0.0f, 600.0f);
        spritePositions.resize(NUM_SPRITES);
        for (auto& p : spritePositions) {
            p.x = x(rgen);
            p.y = y(rgen);
        }

        // Load texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        vg::ScopedBitmapResource bmp = vg::ImageIO().load("data/BigImage.png", vg::ImageIOFormat::RGBA_UI8);
        if (bmp.data == nullptr) {
            std::cerr << "Error: Failed to load data/BigImage.png\n";
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bmp.width, bmp.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp.data);
        vg::SamplerState::POINT_WRAP.set(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    virtual void onExit(const vui::GameTime& gameTime) {
        batch.dispose();
    }

    virtual void update(const vui::GameTime& gameTime) {
        angle += 0.1f;
    }
    virtual void draw(const vui::GameTime& gameTime) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        timer.start();
        batch.begin();
        for (auto& p : spritePositions) {
            batch.draw(texture, nullptr, nullptr, p, f32v2(0.5f), size, angle, color::White);
        }
        f64 drawTime = timer.stop();
        timer.start();
        batch.end();
        f64 endTime = timer.stop();
        timer.start();
        batch.render(f32v2(800, 600));
        glFinish();
        f64 renderTime = timer.stop();
        f64 totalTime = drawTime + endTime + renderTime;
        printf("Draw: %5.2lf End: %5.2lf Render %5.2lf Total %5.2lf\n", drawTime, endTime, renderTime, totalTime);
    }
    vorb::PreciseTimer timer;
    f32 angle = 0.0f;
    f32v2 size = f32v2(30.0f);
    const int NUM_SPRITES = 100000;
    std::vector<f32v2> spritePositions;
    vg::SpriteBatch batch;
    VGTexture texture;
};

class AnimViewer : public vui::IGameScreen {
public:
    static f32m4 fromQuat(const f32q& q) {
        return glm::mat4_cast(q);
    }

    static f32q slerp(const f32q& q1, const f32q& q2, f32 t) { // TODO: I didn't write this
        f32q q3;
        f32 dot = glm::dot(q1, q2);
        if (dot < 0) {
            dot = -dot;
            q3 = -q2;
        } else q3 = q2;

        if (dot < 0.95f) {
            float angle = acosf(dot);
            return (q1*sinf(angle*(1 - t)) + q3*sinf(angle*t)) / sinf(angle);
        } else {
            f32v4* v1 = (f32v4*)&q1;
            f32v4* v2 = (f32v4*)&q3;
            f32v4 nv = lerp(*v1, *v2, t);
            *v2 = nv;
            return q3;
        }
    }
    void rest(vg::Bone& bone, const f32m4& parent) {
        printf("Rest Bone %d\n", bone.index);
        
        // Add inverse of rest pose
        f32m4 mRest = glm::translate(bone.rest.translation) * fromQuat(bone.rest.rotation);
        mRest = parent * mRest;

        // Loop children
        for (size_t i = 0; i < bone.numChildren; i++) {
            rest(*bone.children[i], parent);
        }

        mRestInv[bone.index] = glm::inverse(mRest);
    }
    void walk(vg::Bone& bone, const f32m4& parent, i32 frame) {
        i32 pfi = 0, nfi = 0;
        while (nfi < (i32)bone.numFrames && bone.keyframes[nfi].frame <= frame) nfi++;
        pfi = nfi - 1;
        if (pfi < 0) pfi = 0;
        if (nfi >= (i32)bone.numFrames) nfi = bone.numFrames - 1;

        if (nfi == pfi) {
            // Compute world transform
            f32m4 local = glm::translate(bone.keyframes[pfi].transform.translation) * fromQuat(bone.keyframes[pfi].transform.rotation);
            mWorld[bone.index] = parent * local;
        } else {
            // Lerp
            i32 fl = bone.keyframes[nfi].frame - bone.keyframes[pfi].frame;
            f32 r = (f32)(frame - bone.keyframes[pfi].frame) / (f32)fl;
            f32q rotation = slerp(bone.keyframes[pfi].transform.rotation, bone.keyframes[nfi].transform.rotation, r);
            f32v3 translation = lerp(bone.keyframes[pfi].transform.translation, bone.keyframes[nfi].transform.translation, r);

            // Compute world transforms
            f32m4 local = glm::translate(translation) * fromQuat(rotation);
            mWorld[bone.index] = parent * local;
        }


        // Loop children
        for (size_t i = 0; i < bone.numChildren; i++) {
            walk(*bone.children[i], parent, frame);
        }

        // Add inverse of rest pose
        mWorld[bone.index] = mWorld[bone.index] * mRestInv[bone.index];
    }

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
        std::unordered_map<ui8, VGAttribute> attrmap;
        vg::VertexAttributeIndexed vai;
        vai.type = vg::VertexAttributeUsage::Position;
        vai.index = 0;
        attrmap[vai.value] = 0;
        vai.type = vg::VertexAttributeUsage::Normal;
        vai.index = 0;
        attrmap[vai.value] = 1;
        vai.type = vg::VertexAttributeUsage::TextureCoordinate;
        vai.index = 0;
        attrmap[vai.value] = 2;
        vai.type = vg::VertexAttributeUsage::TextureCoordinate;
        vai.index = 1;
        attrmap[vai.value] = 3;
        vai.type = vg::VertexAttributeUsage::BoneWeights;
        vai.index = 0;
        attrmap[vai.value] = 4;
        vai.type = vg::VertexAttributeUsage::BoneIndices;
        vai.index = 0;
        attrmap[vai.value] = 5;

        program.init();
        program.addShader(vg::ShaderType::VERTEX_SHADER, R"(
uniform mat4 unWorld[32];
uniform mat4 unVP;

in vec4 vPosition;
in vec3 vNormal;
in vec2 vUV;
in vec2 vUVLight;
in vec4 vBoneWeights;
in uvec4 vBoneIndices;

out vec4 fUV;

void main() {
    vec4 worldPos = (unWorld[vBoneIndices.x] * vPosition) * vBoneWeights.x;
    worldPos += (unWorld[vBoneIndices.y] * vPosition) * vBoneWeights.y;
    worldPos += (unWorld[vBoneIndices.z] * vPosition) * vBoneWeights.z;
    worldPos += (unWorld[vBoneIndices.w] * vPosition) * vBoneWeights.w;
    gl_Position = unVP * worldPos;

    fUV = vec4(abs(vPosition.x), abs(vPosition.z), (vBoneIndices.x / 2.0), 1);
}
)"
            );
        program.addShader(vg::ShaderType::FRAGMENT_SHADER, R"(
in vec4 fUV;

out vec4 pColor;

void main() {
    pColor = fUV;
}
)"
            );
        program.setAttribute("vPosition", 0);
        program.setAttribute("vNormal", 1);
        program.setAttribute("vUV", 2);
        program.setAttribute("vUVLight", 3);
        program.setAttribute("vBoneWeights", 4);
        program.setAttribute("vBoneIndices", 5);
        program.link();
        program.initAttributes();
        program.initUniforms();

        vg::VertexDeclaration decl;
        ui32 indSize;
        vio::IOManager iom;
        std::vector<ui8> data;
        iom.readFileToData("data/models/VRAW/Heavy.vraw", data);
        vg::MeshDataRaw mesh = vg::ModelIO::loadRAW(data.data(), decl, indSize);
        data.clear();
        
        glGenBuffers(1, &inds);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inds);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * mesh.indexCount, mesh.indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        indexCount = mesh.indexCount;
        delete[] mesh.indices;

        vertexSize = 0;
        for (size_t vi = 0; vi < decl.size(); vi++) {
            vertexSize += decl[vi].componentCount * decl[vi].componentSize;
        }
        glGenBuffers(1, &verts);
        glBindBuffer(GL_ARRAY_BUFFER, verts);
        glBufferData(GL_ARRAY_BUFFER, vertexSize * mesh.vertexCount, mesh.vertices, GL_STATIC_DRAW);
        delete[] mesh.vertices;

        glGenVertexArrays(1, &vdecl);
        glBindVertexArray(vdecl);
        for (size_t vi = 0; vi < decl.size(); vi++) {
            vg::VertexElement& ve = decl[vi];
            VGAttribute attr = attrmap[ve.usage.value];
            bool norm = (ve.flags & vg::VertexElementFlags::NORMALIZED) == vg::VertexElementFlags::NORMALIZED;
            vg::VertexAttribPointerType vt = vg::getComponentType(ve.flags, ve.componentSize);

            glEnableVertexAttribArray(attr);
            if ((ve.flags & vg::VertexElementFlags::FLOAT) != vg::VertexElementFlags::FLOAT) {
                glVertexAttribIPointer(attr, ve.componentCount, (GLenum)vt, vertexSize, (void*)ve.offset);
            } else {
                glVertexAttribPointer(attr, ve.componentCount, (GLenum)vt, norm, vertexSize, (void*)ve.offset);
            }
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        iom.readFileToData("data/animation/Heavy.anim", data);
        skeleton = vg::ModelIO::loadAnim(data.data());

        mWorld = new f32m4[skeleton.numBones];
        mRestInv = new f32m4[skeleton.numBones];
        for (size_t i = 0; i < skeleton.numBones; i++) {
            mWorld[i] = f32m4(1.0f);
            mRestInv[i] = f32m4(1.0f);
        }
        for (size_t i = 0; i < skeleton.numBones; i++) {
            if (!skeleton.bones[i].parent) {
                rest(skeleton.bones[i], f32m4(1.0f));
                walk(skeleton.bones[i], f32m4(1.0f), 102);
            }
        }

        mVP = glm::perspectiveFov(50.0f, 800.0f, 600.0f, 0.01f, 1000.0f) *
            glm::lookAt(f32v3(-1, 5, 1), f32v3(0, 0, 0.5f), f32v3(0, 0, 1));
        frame = 0;

        glClearColor(1, 1, 1, 1);
        glClearDepth(1.0);
    }
    virtual void onExit(const vui::GameTime& gameTime) {
        delete[] skeleton.bones;
        delete[] skeleton.frames;
        delete[] skeleton.childrenArray;
        delete[] mWorld;
        delete[] mRestInv;
    }

    virtual void update(const vui::GameTime& gameTime) {
    }
    virtual void draw(const vui::GameTime& gameTime) {
        vg::DepthState::FULL.set();
        vg::RasterizerState::CULL_NONE.set();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        frame++;
        frame = (frame - 1) % 260;
        frame++;
        for (size_t i = 0; i < skeleton.numBones; i++) {
            if (!skeleton.bones[i].parent) {
                walk(skeleton.bones[i], f32m4(1.0f), frame);
            }
        }

        program.use();
        glUniformMatrix4fv(program.getUniform("unVP"), 1, false, &mVP[0][0]);
        glUniformMatrix4fv(program.getUniform("unWorld"), skeleton.numBones, false, &mWorld[0][0][0]);

        glBindVertexArray(vdecl);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, inds);
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }



    VGVertexArray vdecl;
    VGVertexBuffer verts;
    VGIndexBuffer inds;
    i32 indexCount;
    ui32 vertexSize;
    vg::GLProgram program;
    vg::Skeleton skeleton;

    i32 frame;
    f32m4 mVP;
    f32m4* mWorld;
    f32m4* mRestInv;
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
        m_screenList.addScreen(screen);
        m_screenList.setScreen(0);
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

TEST(AnimViewer) {
    vorb::init(vorb::InitParam::ALL);
    { VGTestApp(new AnimViewer).run(); }
    vorb::dispose(vorb::InitParam::ALL);
    return true;
}

TEST(SpriteBatch) {
    vorb::init(vorb::InitParam::ALL);
    { VGTestApp(new SpriteBatchTester).run(); }
    vorb::dispose(vorb::InitParam::ALL);
    return true;
}

TEST(D3DContext) {
    const char srcCompute[] = R"(
RWTexture2D<float> DataIn : register(u0);
RWTexture2D<float4> DataOut : register(u2);

[numthreads(16, 8, 1)]
void main(uint3 input : SV_DispatchThreadID) {
    uint index = input.y * 1024 + input.x;
    float4 tint;
    tint.r = DataIn[input.xy];
    tint.g = DataIn[input.xy + uint2(1, 0)];
    tint.b = DataIn[input.xy + uint2(0, 1)];
    tint.a = DataIn[input.xy + uint2(1, 1)];
    DataOut[input.xy] = float4(float(input.x) / 1023.0, float(input.y) / 1023.0, float(index) / (1024.0 * 1024.0 - 1), 1.0) * tint;
}
)";

    vorb::init(vorb::InitParam::ALL);

    // Create a graphics context
    vg::IAdapter* adapter = vg::getD3DAdapter();
    vg::IDevice* device = nullptr;
    vg::IContext* context = adapter->createContext(&device);

    // Create a window
    vui::OSWindow window {};
    vui::OSWindowSettings settings {};
    settings.width = 800;
    settings.height = 600;
    settings.isBorderless = false;
    settings.isFullscreen = false;
    settings.isResizable = false;
    vui::OSWindow::create(window, settings);

    // Bind the graphics context to the window
    adapter->attachToWindow(context, window.getWindowHandle());

    vg::ShaderCompilerInfo info {};
    info.version.major = 5;
    info.version.minor = 0;
    vg::ShaderBytecode byteCode = context->compileShaderSource(srcCompute, sizeof(srcCompute), vg::ShaderType::COMPUTE_SHADER, info);
    vg::IShaderCode* shaderCode = context->loadCompiledShader(byteCode);
    byteCode.free();
    vg::IComputeShader* computeShader = context->createComputeShader(shaderCode);
    shaderCode->dispose();
    computeShader->dispose();

    // Graphics thread
    bool running = true;
    std::thread tRender([&] () {
        f64 v = 0;
        LARGE_INTEGER freq, value, lastValue;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&lastValue);
        while (running) {
            v = glm::fract(v + 0.01);
            device->setClearColor(f64v4(v, 0.0, 1 - v, 1.0));
            device->clear(vg::ClearBits::COLOR | vg::ClearBits::DEPTH);

            context->present();

            QueryPerformanceCounter(&value);
            // printf("Present Time: %f\n", (f32)(value.QuadPart - lastValue.QuadPart) / freq.QuadPart);
            lastValue = value;
        }
    });

    vui::InputDispatcher::init();
    window.getDispatcher().key.onKeyDown.addFunctor([&] (Sender, const vui::KeyEvent& e) {
        switch (e.keyCode) {
        case VKEY_Q:
            window.setBorderless(true);
            break;
        case VKEY_A:
            window.setBorderless(false);
            break;
        case VKEY_W:
            window.setFullscreen(true);
            break;
        case VKEY_S:
            window.setFullscreen(false);
            break;
        case VKEY_E:
            window.setResizable(true);
            break;
        case VKEY_D:
            window.setResizable(false);
            break;
        default:
            break;
        }
    });
    vui::InputDispatcher::onQuit.addFunctor([&] (Sender) {
        running = false;
    });

    // TODO(Cristian): Event loop
    while (running) {
        window.update();

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            default:
                break;
            }
            if (e.type == SDL_QUIT) {
                running = false;
                break;
            }
        }
    }

    // Stop the application
    tRender.join();
    vui::InputDispatcher::dispose();
    device->dispose();
    context->dispose();

    vorb::dispose(vorb::InitParam::ALL);
    return true;
}

//DEVMODE newSettings {};
//// now fill the DEVMODE with standard settings, mainly monitor frequency
//EnumDisplaySettings(NULL, 0, &newSettings);
//// set desired screen size and resolution	
//newSettings.dmPelsWidth = 800;
//newSettings.dmPelsHeight = 600;
//newSettings.dmBitsPerPel = 16;
//// set those flags to let the next function know what we want to change
//newSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
//// and apply the new settings
//if (ChangeDisplaySettings(&newSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) return false;
