#pragma once
#include <GL/glew.h>
#include <vector>
#include "result.h"
#include "types.h"

struct RenderResult {
    friend struct Renderer;
	RenderResult() = default;
    ~RenderResult();
	RenderResult(RenderResult&) = delete;
    RenderResult(RenderResult&&) = delete;

    [[nodiscard]] auto init(unsigned width, unsigned height) noexcept -> Result<void>;
    [[nodiscard]] auto get_pixels() const noexcept -> std::vector<byte> const& {
        return m_pixels;
    }
    [[nodiscard]] void save(std::string_view file_path) const noexcept;
private:
    void prepare() const noexcept;
    void update() noexcept;

    GLuint m_fbo;
    GLuint m_tex;
    GLuint m_depth_rbo;
    
    std::vector<byte> m_pixels;
    GLsizei m_width, m_height, m_linear_sz;
};