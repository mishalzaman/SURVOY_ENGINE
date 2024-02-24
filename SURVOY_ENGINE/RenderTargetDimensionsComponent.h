#pragma once

namespace ECS {
    struct RenderTargetDimensionsComponent {
        float ScreenWidth, ScreenHeight;

        // You can still have constructors in a struct.
        RenderTargetDimensionsComponent(float screenWidth, float screenHeight) : ScreenWidth(screenWidth), ScreenHeight(screenHeight) {}
        // Add other constructors or methods as needed.
    };
}
#pragma once
