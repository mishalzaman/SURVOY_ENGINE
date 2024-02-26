#version 440 core

in vec3 vPos;

uniform vec3 colour;
uniform vec3 viewPos;

out vec4 fragColor; // Define an output variable for the fragment shader

float alpha = 1.0f;

// using 20 units of space
float v = 1.0f;

void main()
{
    // Colours:
    // X - RED
    // Y - GREEN
    // Z - BLUE

    // X
    if (vPos.y == 0 && vPos.z == 0) {
        float c = 1.0f; 
        if (vPos.x < 0) {
            float a = abs(vPos.x);
            c = abs((a-20)/20);
        }

        fragColor = vec4(vec3(c,0,0), 1.0f); // Use the user-defined output variable
    }
    // Y
    else if (vPos.x == 0 && vPos.z == 0) {
        float c = 1.0f; 
        if (vPos.y < 0) {
            float a = abs(vPos.y);
            c = abs((a-20)/20);
        }

        fragColor = vec4(vec3(0,c,0), 1.0f); // Use the user-defined output variable
    }
    // Z
    else if (vPos.x == 0 && vPos.y == 0) {
        float c = 1.0f; 
        if (vPos.z < 0) {
            float a = abs(vPos.z);
            c = abs((a-20)/20);
        }

        fragColor = vec4(vec3(0,0,c), 1.0f); // Use the user-defined output variable
    }
    else
    {
        fragColor = vec4(colour, 1.0f); // Use the user-defined output variable
    }
}
