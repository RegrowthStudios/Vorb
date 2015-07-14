SamplerState Sampler;
Texture2D<float4> Texture;

struct PSInput {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};
struct PSOutput {
    float4 color : SV_TARGET;
};

PSOutput main(PSInput input) {
    PSOutput output;
    output.color = Texture.Sample(Sampler, input.uv);
    return output;
}
