#pragma once
#include <variant>
#include <glm/glm.hpp>

struct Cmd_CameraYaw {
    float angle_deg;
};
struct Cmd_CameraPitch {
    float angle_deg;
};
struct Cmd_CameraRoll {
    float angle_deg;
};
struct Cmd_CameraMove {
    glm::vec3 delta;
};
struct Cmd_CameraZoom {
    float delta;
};

using Cmd = std::variant<
    Cmd_CameraYaw,
    Cmd_CameraPitch,
    Cmd_CameraRoll,
    Cmd_CameraMove,
    Cmd_CameraZoom
>;