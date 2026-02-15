#version 430 core 

in vec2 UVs;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main() {    
    FragColor = texture(texture_diffuse1, UVs);
    // FragColor = vec4(fract(UVs), 0.0, 1.0);
}