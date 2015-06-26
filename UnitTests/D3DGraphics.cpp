#include "stdafx.h"
#include "macros.h"

#undef UNIT_TEST_BATCH
#define UNIT_TEST_BATCH Vorb_Graphics_D3D_

#include <include/Vorb.h>
#include <include/graphics/IAdapter.h>
#include <include/graphics/IContext.h>
#include <include/graphics/IDevice.h>

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
RWTexture2D<float4> DataOut : register(u0);

[numthreads(16, 8, 1)]
void main(uint3 input : SV_DispatchThreadID) {
    uint index = input.y * 1024 + input.x;
    DataOut[input.xy] = float4(float(input.x) / 1023.0, float(input.y) / 1023.0, float(index) / (1024.0 * 1024.0 - 1), 1.0);
}
)";

TEST(CreateContext) {
    vorb::init(vorb::InitParam::GRAPHICS);

    vg::IAdapter* adapter = vg::getD3DAdapter();
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

    vg::IAdapter* adapter = vg::getD3DAdapter();
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

    vg::IPixelShader* pixelShader = ctx->createPixelShader(shaderCode);
    testAssert(__FILE__, __LINE__, pixelShader != nullptr, "Shader creation failed");

    vorb::dispose(vorb::InitParam::GRAPHICS);
    return true;
}

TEST(CreateComputeShader) {
    vorb::init(vorb::InitParam::GRAPHICS);

    vg::IAdapter* adapter = vg::getD3DAdapter();
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

    vg::IComputeShader* computeShader = ctx->createComputeShader(shaderCode);
    testAssert(__FILE__, __LINE__, computeShader != nullptr, "Shader creation failed");

    vorb::dispose(vorb::InitParam::GRAPHICS);
    return true;
}

TEST(ComputeOutput) {
    vorb::init(vorb::InitParam::GRAPHICS);

    vg::IAdapter* adapter = vg::getD3DAdapter();
    vg::IContext* ctx = nullptr;
    vg::IDevice* defaultDevice = nullptr;
    ctx = adapter->createContext(&defaultDevice);

    vg::ShaderCompilerInfo info {};
    info.version.major = 5;
    info.version.minor = 0;
    vg::ShaderBytecode byteCode = ctx->compileShaderSource(srcCompute, sizeof(srcCompute), vg::ShaderType::COMPUTE_SHADER, info);
    vg::IShaderCode* shaderCode = ctx->loadCompiledShader(byteCode);
    vg::IComputeShader* computeShader = ctx->createComputeShader(shaderCode);

    defaultDevice->computeUse(computeShader);
    defaultDevice->dispatchThreads(1024, 1024, 1);

    vorb::dispose(vorb::InitParam::GRAPHICS);
    return true;
}
