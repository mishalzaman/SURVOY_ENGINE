#pragma once

namespace ECS {
    struct CameraFOVComponent {
        float FOV;

        CameraFOVComponent(float fov) : FOV(fov) {}
    };
}
