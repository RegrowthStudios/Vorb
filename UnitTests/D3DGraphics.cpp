#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Graphics_D3D_

#include <include/Vorb.h>
#include <include/graphics/IAdapter.h>
#include <include/graphics/IContext.h>
#include <include/graphics/IDevice.h>
#include <include/ui/OSWindow.h>

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
