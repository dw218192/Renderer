#pragma once
#include <GL/glew.h>
#include <vector>
#include "result.h"
#include "types.h"

struct RenderResult {
    friend struct Renderer;
    RenderResult() noexcept;
    ~RenderResult() noexcept;
	RenderResult(RenderResult&) = delete;
    RenderResult(RenderResult&&) = delete;
    RenderResult& operator=(RenderResult&) = delete;
    RenderResult& operator=(RenderResult&&) = delete;

    [[nodiscard]] auto init(unsigned width, unsigned height) noexcept -> Result<void>;
    [[nodiscard]] auto get_pixels() const noexcept -> std::vector<byte> const& {
        return m_pixels;
    }
    [[nodiscard]] auto get_width() const noexcept -> uint { return m_width; }
    [[nodiscard]] auto get_height() const noexcept -> uint { return m_height; }

    void save(std::string_view file_path) const noexcept;
    auto upload_to_frame_buffer() const noexcept -> Result<void>;
private:
    void prepare() const noexcept;
    void update() noexcept;

    GLuint m_fbo;
    GLuint m_tex;
    GLuint m_depth_rbo;
    
    std::vector<byte> m_pixels;
    GLsizei m_width, m_height, m_linear_sz;
};