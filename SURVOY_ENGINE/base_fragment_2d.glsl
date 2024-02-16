    #version 440 core
    out vec4 FragColor;

    in vec2 TexCoord;

    uniform sampler2D text;

    void main() {
        vec4 sampled = texture(text, TexCoord);
        FragColor = sampled; // Use the original color directly
    }