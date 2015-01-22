namespace vorb {
    namespace core {
        namespace graphics {
            namespace impl {
                const cString SPRITEBATCH_VS_SRC = R"(
uniform mat4 World;
uniform mat4 VP;

in vec4 vPosition;
in vec2 vUV;
in vec4 vUVRect;
in vec4 vTint;

out vec2 fUV;
flat out vec4 fUVRect;
out vec4 fTint;

void main() {
    fTint = vTint;
    fUV = vUV;
    fUVRect = vUVRect;
    vec4 worldPos = World * vPosition;
    gl_Position = VP * worldPos;
}
)";
                const cString SPRITEBATCH_FS_SRC = R"(
uniform sampler2D SBTex;

in vec2 fUV;
flat in vec4 fUVRect;
in vec4 fTint;

out vec4 fColor;

void main() {
    fColor = texture(SBTex, fract(fUV.xy) * fUVRect.zw + fUVRect.xy) * fTint;
}
)";
            }
        }
    }
}

