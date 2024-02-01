#pragma once

namespace ECS {
    struct CameraMouseComponent {
        float MouseRelX, MouseRelY;

        CameraMouseComponent() : MouseRelX(0), MouseRelY(0) {}
    };
}
#pragma once
