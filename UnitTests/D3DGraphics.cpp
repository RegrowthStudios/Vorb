#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Graphics_D3D_

#include <include/Vorb.h>
#include <include/graphics/IAdapter.h>
#include <include/graphics/IContext.h>
#include <include/graphics/IDevice.h>
#include <include/ui/OSWindow.h>
#include <include/io/IOManager.h>
#include <include/graphics/ImageIO.h>

const char srcPixel[] = R"(
struct PSOut {
    float4 color : SV_TARGET;
};

PSOut main() {
    PSOut output;
    output.color = float4(1, 0, 0, 1);
    return output;
}
)";
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



TEST(CreateContext) {
    vorb::init(vorb::InitParam::GRAPHICS);

    vg::IAdapter* adapter = vg::getAdapter(vg::API::DIRECT_3D, 11, 0);
    testAssert(__FILE__, __LINE__, adapter != nullptr, "Null Adapter");

    vg::IContext* context = nullptr;
    vg::IDevice* defaultDevice = nullptr;
    context = adapter->createContext(&defaultDevice);
    testAssert(__FILE__, __LINE__, context != nullptr, "Null Context");
    testAssert(__FILE__, __LINE__, defaultDevice != nullptr, "Null Default Device");

    vorb::dispose(vorb::InitParam::GRAPHICS);
    return true;
}

TEST(CreateShader) {
    vorb::init(vorb::InitParam::GRAPHICS);

    vg::IAdapter* adapter = vg::getAdapter(vg::API::DIRECT_3D, 11, 0);
    vg::IContext* ctx = nullptr;
    vg::IDevice* defaultDevice = nullptr;
    ctx = adapter->createContext(&defaultDevice);

    vg::ShaderCompilerInfo info {};
    info.version.major = 4;
    info.version.minor = 0;
    vg::ShaderBytecode byteCode = ctx->compileShaderSource(srcPixel, sizeof(srcPixel), vg::ShaderType::FRAGMENT_SHADER, info);
    testAssert(__FILE__, __LINE__, byteCode.code != nullptr, "Shader compilation failed");

    vg::IShaderCode* shaderCode = ctx->loadCompiledShader(byteCode);
    testAssert(__FILE__, __LINE__, shaderCode != nullptr, "Shader code load failed");
    byteCode.free();

    vg::IPixelShader* pixelShader = ctx->createPixelShader(shaderCode);
    testAssert(__FILE__, __LINE__, pixelShader != nullptr, "Shader creation failed");
    shaderCode->dispose();

    pixelShader->dispose();

    vorb::dispose(vorb::InitParam::GRAPHICS);
    return true;
}

TEST(CreateComputeShader) {
    vorb::init(vorb::InitParam::GRAPHICS);

    vg::IAdapter* adapter = vg::getAdapter(vg::API::DIRECT_3D, 11, 0);
    vg::IContext* ctx = nullptr;
    vg::IDevice* defaultDevice = nullptr;
    ctx = adapter->createContext(&defaultDevice);

    vg::ShaderCompilerInfo info {};
    info.version.major = 5;
    info.version.minor = 0;
    vg::ShaderBytecode byteCode = ctx->compileShaderSource(srcCompute, sizeof(srcCompute), vg::ShaderType::COMPUTE_SHADER, info);
    testAssert(__FILE__, __LINE__, byteCode.code != nullptr, "Shader compilation failed");

    vg::IShaderCode* shaderCode = ctx->loadCompiledShader(byteCode);
    testAssert(__FILE__, __LINE__, shaderCode != nullptr, "Shader code load failed");
    byteCode.free();

    vg::IComputeShader* computeShader = ctx->createComputeShader(shaderCode);
    testAssert(__FILE__, __LINE__, computeShader != nullptr, "Shader creation failed");
    shaderCode->dispose();

    computeShader->dispose();

    vorb::dispose(vorb::InitParam::GRAPHICS);
    return true;
}

TEST(CreateTexture) {
    vorb::init(vorb::InitParam::GRAPHICS);

    vg::IAdapter* adapter = vg::getAdapter(vg::API::DIRECT_3D, 11, 0);
    vg::IContext* ctx = nullptr;
    vg::IDevice* defaultDevice = nullptr;
    ctx = adapter->createContext(&defaultDevice);

    // Create the rendering output
    vg::Texture2DDescription textureDesc {};
    textureDesc.width = 1024;
    textureDesc.height = 1024;
    vg::ITexture2D* texture = ctx->create(textureDesc);
    testAssert(__FILE__, __LINE__, texture != nullptr, "Texture creation failed");

    vg::IRenderTarget* rtCompute = defaultDevice->create(texture);
    testAssert(__FILE__, __LINE__, rtCompute != nullptr, "Render target creation failed");

    vorb::dispose(vorb::InitParam::GRAPHICS);
    return true;
}

TEST(ComputeOutput) {
    vorb::init(vorb::InitParam::GRAPHICS);

    vg::IAdapter* adapter = vg::getAdapter(vg::API::DIRECT_3D, 11, 0);
    vg::IContext* ctx = nullptr;
    vg::IDevice* defaultDevice = nullptr;
    ctx = adapter->createContext(&defaultDevice);

    // Create the compute shader
    vg::ShaderCompilerInfo info {};
    info.version.major = 5;
    info.version.minor = 0;
    vg::ShaderBytecode byteCode = ctx->compileShaderSource(srcCompute, sizeof(srcCompute), vg::ShaderType::COMPUTE_SHADER, info);
    vg::IShaderCode* shaderCode = ctx->loadCompiledShader(byteCode);
    byteCode.free();
    vg::IComputeShader* computeShader = ctx->createComputeShader(shaderCode);
    shaderCode->dispose();

    // Create the rendering output
    vg::Texture2DDescription textureDesc {};
    textureDesc.width = 1024;
    textureDesc.height = 1024;
    vg::ITexture2D* texture = ctx->create(textureDesc);
    vg::IComputeResourceView* rtCompute = ctx->makeComputeView(texture);

    // Render to output
    defaultDevice->computeUse(computeShader);
    defaultDevice->dispatchThreads(1024, 1024, 1);

    // Destroy all resources
    rtCompute->dispose();
    texture->dispose();
    computeShader->dispose();

    vorb::dispose(vorb::InitParam::GRAPHICS);
    return true;
}

TEST(DrawTriangle) {
    vorb::init(vorb::InitParam::GRAPHICS);

    vg::IAdapter* adapter = vg::getAdapter(vg::API::DIRECT_3D, 11, 0);
    vg::IContext* ctx = nullptr;
    vg::IDevice* defaultDevice = nullptr;
    ctx = adapter->createContext(&defaultDevice);

    vg::IBuffer* vertData = nullptr;
    vg::IBufferView* vertView = nullptr;
    vg::IVertexDeclaration* decl = nullptr;
    vg::IVertexShader* vertexShader = nullptr;
    vg::IPixelShader* pixelShader = nullptr;

    { // Create the vertex shader
        vg::ShaderCompilerInfo info {};
        info.version.major = 3;
        info.version.minor = 0;
        vg::ShaderBytecode byteCode = ctx->compileShaderSource(srcCompute, sizeof(srcCompute), vg::ShaderType::VERTEX_SHADER, info);
        vg::IShaderCode* shaderCode = ctx->loadCompiledShader(byteCode);
        byteCode.free();
        vertexShader = ctx->createVertexShader(shaderCode);
        shaderCode->dispose();
    }
    { // Create the pixel shader
        vg::ShaderCompilerInfo info {};
        info.version.major = 3;
        info.version.minor = 0;
        vg::ShaderBytecode byteCode = ctx->compileShaderSource(srcCompute, sizeof(srcCompute), vg::ShaderType::FRAGMENT_SHADER, info);
        vg::IShaderCode* shaderCode = ctx->loadCompiledShader(byteCode);
        byteCode.free();
        pixelShader = ctx->createPixelShader(shaderCode);
        shaderCode->dispose();
    }

    // Render to output
    defaultDevice->setTopology(vg::PrimitiveType::TRIANGLES);
    defaultDevice->vertexUse(vertexShader);
    defaultDevice->pixelUse(pixelShader);

    // Destroy all resources
    vertexShader->dispose();

    vorb::dispose(vorb::InitParam::GRAPHICS);
    return true;
}

struct VertexPosUV {
public:
    f32v3 pos;
    f32v2 uv;
};

vg::IShaderCode* compileShader(vg::IContext* ctx, const vpath& file, vg::ShaderType type) {
    // Read code
    vio::IOManager iom;
    const cString src = iom.readFileToString(file);
    size_t srcLength = strlen(src);

    // Create compiler info
    vg::ShaderCompilerInfo info {};
    info.version.major = 4;
    info.version.minor = 0;

    // Compile source
    vg::ShaderBytecode byteCode = ctx->compileShaderSource(src, srcLength, type, info);
    delete[] src;

    // Load to custom type
    vg::IShaderCode* shaderCode = ctx->loadCompiledShader(byteCode);
    byteCode.free();

    return shaderCode;
}

TEST(DrawImage) {
    vorb::init(vorb::InitParam::GRAPHICS | vorb::InitParam::IO);

    vui::OSWindow window = {};
    vui::OSWindowSettings ws = {};
    ws.width = 640;
    ws.height = 640;
    vui::OSWindow::create(window, ws);

    // Create context and device
    vg::IAdapter* adapter = vg::getAdapter(vg::API::DIRECT_3D, 11, 0);
    vg::IContext* ctx = nullptr;
    vg::IDevice* defaultDevice = nullptr;
    ctx = adapter->createContext(&defaultDevice);
    adapter->attachToWindow(ctx, window.getWindowHandle());

    vg::IBuffer* vertData = nullptr;
    vg::IVertexDeclaration* decl = nullptr;
    vg::ITexture2D* texture = nullptr;
    vg::IShaderCode* code = nullptr;

    // Load vertex shader and description
    vg::IVertexShader* vertexShader =  nullptr;
    {
        vg::VertexElementDescription desc[] = {
            { 0, vg::Semantic::POSITION, 0, offsetof(VertexPosUV, pos), sizeof(f32v3), vg::MemoryFormat::R32G32B32_FLOAT },
            { 0, vg::Semantic::TEXCOORD, 0, offsetof(VertexPosUV, uv), sizeof(f32v2), vg::MemoryFormat::R32G32_FLOAT }
        };

        code = compileShader(ctx, "data/shaders/d3d/VSImage.hlsl", vg::ShaderType::VERTEX_SHADER);
        vertexShader = ctx->createVertexShader(code);
        decl = ctx->create(desc, code, 2);
        code->dispose();
    }

    // Load pixel shader
    code = compileShader(ctx, "data/shaders/d3d/PSImage.hlsl", vg::ShaderType::FRAGMENT_SHADER);
    vg::IPixelShader* pixelShader = ctx->createPixelShader(code);
    code->dispose();

    { // Create texture
        vg::Texture2DDescription desc {};
        desc.format = vg::MemoryFormat::R8G8B8A8_UNORM_SRGB;
        desc.atlasPages = 0;
        vg::ImageIO iio;
        vg::ScopedBitmapResource bmp = iio.load("data/BigImage.png", vg::ImageIOFormat::RGBA_UI8, false);
        vg::InitalResourceData data {};
        data.data = bmp.data;
        desc.width = bmp.width;
        desc.height = bmp.height;
        data.stride.texture2DRow = bmp.width * sizeof(color4);
        texture = ctx->create(desc, &data);
    }

    { // Create triangle
        vg::BufferDescription desc {};
        desc.format = vg::MemoryFormat::R8G8B8A8_UNORM_SRGB;
        VertexPosUV verts[3] = {
            { f32v3(1, -1, 0), f32v2(1, 0) },
            { f32v3(-1, -1, 0), f32v2(0, 0) },
            { f32v3(-1, 1, 0), f32v2(0, 1) }
        };
        desc.type = vg::BufferTarget::ARRAY_BUFFER;
        desc.usage = vg::BufferUsageHint::DYNAMIC_DRAW;
        desc.size = sizeof(verts);
        desc.structSize = 5 * sizeof(f32);
        vg::InitalResourceData data {};
        data.data = verts;
        vertData = ctx->create(desc, &data);
    }

    vg::ISamplerState* ss;
    {
        vg::SamplerStateDescription desc = {};
        desc.addressMode.u = vg::TextureAddressMode::BORDER;
        desc.addressMode.v = vg::TextureAddressMode::BORDER;
        desc.addressMode.w = vg::TextureAddressMode::BORDER;
        desc.magnificationFilter = vg::TextureFilterMode::POINT;
        desc.minificationFilter = vg::TextureFilterMode::POINT;
        ss = defaultDevice->create(desc);
    }

    // Create views
    vg::ITexture2DView* textureView = ctx->makeView(texture);

    // Setup shaders
    defaultDevice->vertexUse(vertexShader);
    defaultDevice->pixelUse(pixelShader);
    defaultDevice->pixelUse(0, 1, (vg::IResourceView**)&textureView);
    defaultDevice->pixelUse(0, 1, &ss);

    { // Set vertex buffer
        ui32 off = 0;
        ui32 stride = sizeof(VertexPosUV);
        vg::BufferBindings bb = { &vertData, 0, 1, &off, &stride };
        defaultDevice->setVertexBuffers(bb);
    }

    // Use correct topology and vertex layout
    defaultDevice->use(decl);
    defaultDevice->setTopology(vg::PrimitiveType::TRIANGLES);

    vg::Viewport vp {};
    vp.min = f32v2(0.0f);
    vp.size = f32v2(640.0f, 480.0f);
    vp.depth.min = 0.0f;
    vp.depth.max = 1.0f;
    defaultDevice->setViewports(1, &vp);

    // Draw a triangle
    puts("Begin");
    bool running = true;
    window.getDispatcher().init();
    window.getDispatcher().onQuit.addFunctor([&] (Sender) {
        running = false;
    });
    while (running) {
        for (size_t i = 0; i < 25; i++) {
            defaultDevice->draw(3, 0);
        }
        puts("Present");
        window.update();
        ctx->present();
    }

    // Destroy all resources
    vertexShader->dispose();
    pixelShader->dispose();
    textureView->dispose();
    texture->dispose();
    decl->dispose();
    vertData->dispose();

    // Destroy graphics API
    defaultDevice->dispose();
    ctx->dispose();

    vorb::dispose(vorb::InitParam::GRAPHICS | vorb::InitParam::IO);
    return true;
}
