#pragma once

#include <GL/glew.h>

namespace ECS {
    struct BuffersComponent {
        unsigned int VAO, VBO, EBO, DepthFBO;

        // You can still have constructors in a struct.
        BuffersComponent() : VAO(0), VBO(0), EBO(0), DepthFBO(0) {}
        // Add other constructors or methods as needed.
    };
}
