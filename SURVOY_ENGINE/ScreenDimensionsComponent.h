#pragma once

namespace ECS {
    struct ScreenDimensionsComponent {
        float ScreenWidth, ScreenHeight;

        // You can still have constructors in a struct.
        ScreenDimensionsComponent(float screenWidth, float screenHeight) : ScreenWidth(screenWidth), ScreenHeight(screenHeight) {}
        // Add other constructors or methods as needed.
    };
}
#pragma once
