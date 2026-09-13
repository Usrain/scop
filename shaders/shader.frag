#version 450

layout(location = 0) in vec2 fragUV;
layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants {
    vec3 materialColor;
} pc;

void main() {
    outColor = vec4(pc.materialColor, 1.0);
}