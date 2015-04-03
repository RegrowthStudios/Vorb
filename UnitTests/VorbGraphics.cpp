#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Graphics_

#include <glm/gtx/transform.hpp>
#include <include/MeshGenerators.h>
#include <include/Vorb.h>
#include <include/colors.h>
#include <include/graphics/GLProgram.h>
#include <include/graphics/GLStates.h>
#include <include/graphics/IRenderStage.h>
#include <include/graphics/ImageIO.h>
#include <include/graphics/ModelIO.h>
#include <include/graphics/RenderPipeline.h>
#include <include/graphics/SpriteBatch.h>
#include <include/graphics/SpriteFont.h>
#include <include/graphics/Texture.h>
#include <include/io/IOManager.h>
#include <include/ui/IGameScreen.h>
#include <include/ui/InputDispatcher.h>
#include <include/ui/MainGame.h>
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
        m_hooks.addAutoHook(vui::InputDispatcher::window.onFile, [&] (Sender, const vui::WindowFileEvent& e) {
            auto bmp = vg::ImageIO().load(e.file, m_imageFormat.format);
            this->m_bmp = bmp;
        });
        m_hooks.addAutoHook(vui::InputDispatcher::key.onKeyDown, [&] (Sender, const vui::KeyEvent& e) {
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
        pool.addAutoHook(vui::InputDispatcher::key.onKeyDown, [&] (Sender, const vui::KeyEvent& e) {
            if (e.repeatCount > 0) return;
            switch (e.keyCode) {
            case VKEY_W: pitchInput += 1; break;
            case VKEY_S: pitchInput -= 1; break;
            case VKEY_A: yawInput -= 1; break;
            case VKEY_D: yawInput += 1; break;
            }
        });
        pool.addAutoHook(vui::InputDispatcher::key.onKeyUp, [&] (Sender, const vui::KeyEvent& e) {
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
        auto bmp = vg::ImageIO().load("data/TW.jpg", vg::ImageIOFormat::RGBA_UI8);
        if (bmp.data == nullptr) {
            std::cerr << "Error: Failed to load data/TW.jpg\n";
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bmp.width, bmp.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bmp.data);
        vg::ImageIO::free(bmp);
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
        spriteBatch.renderBatch(f32v2(800.0f, 600.0f));
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
        while (nfi < bone.numFrames && bone.keyframes[nfi].frame <= frame) nfi++;
        pfi = nfi - 1;
        if (pfi < 0) pfi = 0;
        if (nfi >= bone.numFrames) nfi = bone.numFrames - 1;

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

class PipelineViewer : public vui::IGameScreen {
public:

    class QuadRenderStage : public vg::IRenderStage {
    public:
        void render() override {
            if (!m_program) {
                printf("Building shader\n");
                m_program = new vg::GLProgram(true);
                m_program->addShader(vg::ShaderType::VERTEX_SHADER, R"(
uniform vec2 unOffset;
in vec4 vPosition;
void main() {
    gl_Position = vPosition + vec4(unOffset, 0.0, 0.0);
}
)");
                m_program->addShader(vg::ShaderType::FRAGMENT_SHADER, R"(
uniform vec4 unColor;
out vec4 pColor;
void main() {
    pColor = unColor;
}
)");
                m_program->link();
                m_program->initAttributes();
                m_program->initUniforms();
                if (!m_program->getIsLinked()) throw 123;
            }
            if (!m_verts) {
                printf("Building vbo\n");
                glGenBuffers(1, &m_verts);
                float verts[12] = { -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
                    1.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f };
                glBindBuffer(GL_ARRAY_BUFFER, m_verts);
                glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
            }
            m_program->use();
            m_program->enableVertexAttribArrays();
            glUniform4fv(m_program->getUniform("unColor"), 1, &m_color[0]);
            glUniform2fv(m_program->getUniform("unOffset"), 1, &m_offset[0]);
            glBindBuffer(GL_ARRAY_BUFFER, m_verts);
            glVertexAttribPointer(m_program->getAttribute("vPosition"), 2, GL_FLOAT, GL_FALSE, 0, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            m_program->disableVertexAttribArrays();
            m_program->unuse();
        }
        void dispose() override {
            IRenderStage::dispose();
            if (m_verts) {
                glDeleteBuffers(1, &m_verts);
                m_verts = 0;
            }
        }

        void setColor(f32v4 colr) { m_color = colr; }
        void setOffset(f32v2 offset) { m_offset = offset; }
    private:
        VGVertexBuffer m_verts = 0;
        f32v4 m_color = f32v4(1.0f, 1.0f, 1.0f, 1.0f);
        f32v2 m_offset = f32v2(0.0f, 0.0f);
    };

    virtual i32 getNextScreen() const {
        return SCREEN_INDEX_NO_SCREEN;
    }
    virtual i32 getPreviousScreen() const {
        return SCREEN_INDEX_NO_SCREEN;
    }

    virtual void build() {
    }
    virtual void destroy(const vui::GameTime& gameTime) {
        m_pipeline.destroy(true);
    }

    virtual void onEntry(const vui::GameTime& gameTime) {
        printf(" *** Press 1 to reload shaders. ***\n");
        m_hooks.addAutoHook(vui::InputDispatcher::key.onKeyUp, [&](Sender, const vui::KeyEvent& e) {
            switch (e.keyCode) {
                case VKEY_1: m_reloadShaders = true; break;
            }
        });
        std::shared_ptr<QuadRenderStage> s1 = std::make_shared<QuadRenderStage>();
        std::shared_ptr<QuadRenderStage> s2 = std::make_shared<QuadRenderStage>();
        s2->setColor(f32v4(1.0f, 0.0f, 0.0f, 1.0f));
        s2->setOffset(f32v2(1.0f, 0.0f));
        std::shared_ptr<QuadRenderStage> s3 = std::make_shared<QuadRenderStage>();
        s3->setColor(f32v4(0.0f, 1.0f, 0.0f, 1.0f));
        s3->setOffset(f32v2(0.0f, 1.0f));
        std::shared_ptr<QuadRenderStage> s4 = std::make_shared<QuadRenderStage>();
        s4->setColor(f32v4(0.0f, 0.0f, 1.0f, 1.0f));
        s4->setOffset(f32v2(1.0f, 1.0f));
        m_pipeline.addStage(s1);
        m_pipeline.addStage(s2);
        m_pipeline.addStage(s3);
        m_pipeline.addStage(s4);
    }
    virtual void onExit(const vui::GameTime& gameTime) {
        m_pipeline.destroy(true);
    }

    virtual void update(const vui::GameTime& gameTime) {
        if (m_reloadShaders) {
            printf("Reloading shaders\n");
            m_pipeline.reloadShaders();
            m_reloadShaders = false;
        }
    }
    virtual void draw(const vui::GameTime& gameTime) {
        vg::DepthState::NONE.set();
        glDisable(GL_CULL_FACE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_pipeline.render();
        vg::DepthState::FULL.set();
    }
private:
    vg::RenderPipeline m_pipeline;
    AutoDelegatePool m_hooks;
    bool m_reloadShaders = false;
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

TEST(AnimViewer) {
    vorb::init(vorb::InitParam::ALL);
    { VGTestApp(new AnimViewer).run(); }
    vorb::dispose(vorb::InitParam::ALL);
    return true;
}

TEST(PipelineViewer) {
    vorb::init(vorb::InitParam::ALL);
    { VGTestApp(new PipelineViewer).run(); }
    vorb::dispose(vorb::InitParam::ALL);
    return true;
}
