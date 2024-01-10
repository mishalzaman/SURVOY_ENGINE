#version 330 core

in vec3 vPos;

uniform vec3 colour;
uniform vec3 viewPos;

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

        gl_FragColor = vec4(vec3(c,0,0), 1.0f);
    }
    // Y
    else if (vPos.x == 0 && vPos.z == 0) {
        float c = 1.0f; 
        if (vPos.y < 0) {
            float a = abs(vPos.y);
            c = abs((a-20)/20);
        }

        gl_FragColor = vec4(vec3(0,c,0), 1.0f);
    }
    // Z
    else if (vPos.x == 0 && vPos.y == 0) {
        float c = 1.0f; 
        if (vPos.z < 0) {
            float a = abs(vPos.z);
            c = abs((a-20)/20);
        }

        gl_FragColor = vec4(vec3(0,0,c), 1.0f);
    }
    else
    {
        gl_FragColor = vec4(colour, 1.0f);
    }
}