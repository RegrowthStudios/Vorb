struct VSInput {
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
};
struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.position = input.position;
    output.uv = input.uv;
    return output;
}