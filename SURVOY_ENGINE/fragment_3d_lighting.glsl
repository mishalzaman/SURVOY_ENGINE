#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, TexCoord);
    // Set alpha to a constant value, e.g., 0.5
    texColor.a = 1.0;
    FragColor = texColor;
}
