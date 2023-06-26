#pragma once
#include "result.h"
#include "types.h"
#include "commands.h"
#include "renderResult.h"
#include "scene.h"
#include "camera.h"
#include <vector>

struct RenderConfig {
	RenderConfig(unsigned width, unsigned height, real fovy, Transform const& cam_trans)
		: width{width}, height{height}, cam{fovy, static_cast<real>(width) / static_cast<real>(height), cam_trans  }
	{ }

	unsigned width, height;
    Camera cam;
};

struct Renderer {
	explicit Renderer(RenderConfig config) noexcept;
    ~Renderer() noexcept;

    [[nodiscard]] auto open_scene(Scene scene) noexcept -> Result<void>;
    [[nodiscard]] auto exec(Cmd const& cmd) noexcept -> Result<void>;
    [[nodiscard]] auto render() noexcept -> Result<RenderResult const&>;
    [[nodiscard]] auto valid() const noexcept -> bool { return m_vao != 0; }
private:
    RenderConfig m_config;
    RenderResult m_res;
    Scene m_scene;

    // TODO: may abstract these into a class later
    // use 1 vao, 1 vbo for all meshes (objects)
	GLuint m_vao;

    [[nodiscard]] auto get_vbo() const noexcept { return m_buffer_handles[0]; }
    [[nodiscard]] auto get_bufhandle_size() const noexcept { return static_cast<GLsizei>(m_buffer_handles.size()); }
    [[nodiscard]] auto get_bufhandles() const noexcept { return m_buffer_handles.data(); }
    std::vector<GLuint> m_buffer_handles;
};