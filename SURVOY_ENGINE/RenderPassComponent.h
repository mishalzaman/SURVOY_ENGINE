#pragma once

namespace ECS {
    struct RenderPassComponent {
        enum Pipeline {
            SHADOW_MAP,
            COLOUR_MAP,
            FINAL
        };

        Pipeline State;

        RenderPassComponent(Pipeline pipeline) : State(pipeline) {}
    };
}
