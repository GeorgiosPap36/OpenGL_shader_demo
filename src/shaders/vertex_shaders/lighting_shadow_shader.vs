#version 430 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 UV;

out vec3 pos;
out vec3 normal;
out vec2 uv;
out vec3 fragPos;

out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	fragPos = vec3(model * vec4(Pos, 1.0));
	normal = mat3(transpose(inverse(model))) * Normal;
	uv = UV;
	pos = Pos;

	fragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);

	gl_Position = projection * view * model * vec4(Pos, 1.0);
}