#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

layout(binding = 0) uniform UBO {
    mat4 mvp;
} ubo;

layout(location = 0) out vec2 fragUV;

void main() {
    gl_Position = ubo.mvp * vec4(inPosition, 1.0);
    fragUV = inUV;
}