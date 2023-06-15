#pragma once
#include "result.h"
#include "ext.h"
#include "types.h"
#include "commands.h"
#include "renderResult.h"
#include "scene.h"
#include <vector>
#include <string>
#include <string_view>
#include <array>

struct Renderer {
    Renderer(Scene scene);
    ~Renderer();
    void exec(Cmd const& cmd);
    auto render() -> Result<RenderResult>;

private:
    Scene m_scene;
};