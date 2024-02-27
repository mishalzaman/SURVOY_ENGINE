#pragma once

#include <string>

namespace ECS {
    struct ActiveCameraComponent {
        std::string CameraTag;

        // You can still have constructors in a struct.
        ActiveCameraComponent(std::string cameraTag): CameraTag(cameraTag) {}
        // Add other constructors or methods as needed.
    };
}
