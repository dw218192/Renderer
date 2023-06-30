#pragma once
#include "result.h"
#include "types.h"
#include "commands.h"
#include "renderResult.h"
#include "scene.h"
#include "camera.h"
#include <vector>

struct RenderConfig {
	RenderConfig(unsigned width, unsigned height, real fovy, real max_fps)
		: width{width}, height{height}, fovy{fovy}, max_fps{max_fps}, min_frame_time{REAL_LITERAL(1.0) / max_fps}
	{ }

	unsigned width, height;
    real fovy;
    real max_fps;
    real min_frame_time;
};

struct Renderer {
	explicit Renderer(RenderConfig config) noexcept;
    ~Renderer() noexcept;

	// don't copy because we have handles to GL resources
	Renderer(Renderer&) = delete;
    Renderer& operator=(Renderer&) = delete;

    // may implement move, but delete them for now
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;


    [[nodiscard]] auto open_scene(Scene scene) noexcept -> Result<void>;
    [[nodiscard]] auto exec(Cmd const& cmd) noexcept -> Result<void>;
    [[nodiscard]] auto render() noexcept -> Result<RenderResult const&>;
    [[nodiscard]] auto valid() const noexcept -> bool { return m_vao != 0; }
    [[nodiscard]] auto get_config() const noexcept -> RenderConfig const& { return m_config; }
private:
    RenderConfig m_config;
    Camera m_cam;
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