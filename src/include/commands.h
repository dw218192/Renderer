#pragma once
#include <variant>
#include "ext.h"

struct Cmd_CameraRot {
    vec3 angles_deg;
};
struct Cmd_CameraMove {
    vec3 delta;
};
struct Cmd_CameraZoom {
    real delta;
};

using Cmd = std::variant<
    Cmd_CameraRot,
    Cmd_CameraMove,
    Cmd_CameraZoom
>;