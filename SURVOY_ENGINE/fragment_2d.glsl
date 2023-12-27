#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D text;

void main() {    
    vec4 sampled = texture(text, TexCoords);
    FragColor = sampled; // Use the original color directly
}