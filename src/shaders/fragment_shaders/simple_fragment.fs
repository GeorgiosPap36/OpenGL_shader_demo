#version 430 core 

in vec2 uv;
in vec3 normal;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

void main() {    
    // FragColor = texture(texture_diffuse1, uv);
    // FragColor = vec4(fract(uv), 0.0, 1.0);
    // FragColor = vec4(step(0.5, uv.x), step(0.5, uv.y), 0.0, 1.0);
    // FragColor = vec4(fract(uv.x * 50.0), fract(uv.y * 50.0), 0.0, 1.0);
    // FragColor = vec4(normal, 1.0);
    FragColor = texture(texture_diffuse1, vec2(uv.x , 1 - uv.y));
}