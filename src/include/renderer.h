#pragma once
#include "result.h"
#include "ext.h"
#include "types.h"
#include "commands.h"
#include "renderResult.h"
#include "scene.h"
#include "shader.h"

#include <vector>
#include <string>
#include <string_view>
#include <array>

struct RenderConfig {
    unsigned width, height;
};

struct Renderer {
	explicit Renderer(RenderConfig config) noexcept;
    ~Renderer() noexcept;

    [[nodiscard]] auto open_scene(Scene scene) noexcept -> Result<void>;

    void set_shader(ShaderProgram&& shader) = delete;
    void set_shader(ShaderProgram const& shader) noexcept;

    [[nodiscard]] auto exec(Cmd const& cmd) noexcept -> Result<void>;
    [[nodiscard]] auto render() noexcept -> Result<RenderResult const&>;
    [[nodiscard]] auto valid() const noexcept -> bool { return m_vao != 0; }
private:
    RenderConfig m_config;
    RenderResult m_res;
    ShaderProgram const* m_shader;
    Scene m_scene;

    // TODO: may abstract these into a class later
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ebo;
};