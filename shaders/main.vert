#version 460 core

layout (location = 0) in vec2 pos;

layout (location = 0) out vec2 fragPos;


void main() {
    gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);

    fragPos = pos.xy;
}
