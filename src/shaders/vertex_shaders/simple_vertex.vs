#version 330 core

layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;

void main() {

	uv = vec2(UV.x, 1.0 - UV.y);

	gl_Position = projection * view * model * vec4(Pos, 1.0);
}