struct PSOut {
    float4 color : SV_TARGET;
};

PSOut main() {
    PSOut output;
    output.color = float4(1, 0, 0, 1);
    return output;
}