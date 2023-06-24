#include "renderer.h"
#include <cassert>

Renderer::~Renderer() noexcept {
    if(valid()) {
        assert(m_vao && m_vbo && m_ebo);

        GLuint const handles[] = { m_vbo, m_ebo };
        glDeleteBuffers(2, handles);
        glDeleteVertexArrays(1, &m_vao);
    }
}

auto Renderer::exec(Cmd const& cmd) noexcept -> Result<void> {
    (void)cmd;
    return std::string { "Not implemented" };
}

auto Renderer::open_scene(Scene scene) noexcept -> Result<void> {
    GLenum err;
    GLuint vao = m_vao, vbo = m_vbo, ebo = m_ebo;

    auto roll_back = [vao, vbo, ebo]() {
        if (vao) glDeleteVertexArrays(1, &vao);
        if (vbo) glDeleteBuffers(1, &vbo);
        if (ebo) glDeleteBuffers(1, &ebo);
    };
#define CHECK_GL() do { err = glGetError(); if(err != GL_NO_ERROR) {\
	roll_back(); return std::string { "OpenGL error code: " } + std::to_string(err); } } while(false)

    if(!valid()) {
        // set up buffers
        glGenVertexArrays(1, &vao);

        GLuint handles[2];
        glGenBuffers(2, handles);

        vbo = handles[0];
        ebo = handles[1];

        // Set a few settings/modes in OpenGL rendering
        glEnable(GL_POLYGON_SMOOTH);
        glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

        CHECK_GL();
    }

    // fill buffers with data
    glBindVertexArray(vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        {
            glBufferData(
                GL_ARRAY_BUFFER, 
                scene.vertices().size() * sizeof(Vertex), 
                scene.vertices().data(),
                GL_STATIC_DRAW
            );
            CHECK_GL();
            
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            glVertexAttribPointer(0, 3, GLConstants::GL_FLOAT_TYPE, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<void*>(offsetof(Vertex, position)));
            glVertexAttribPointer(1, 3, GLConstants::GL_FLOAT_TYPE, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<void*>(offsetof(Vertex, normal)));
            glVertexAttribPointer(2, 2, GLConstants::GL_FLOAT_TYPE, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<void*>(offsetof(Vertex, uv)));
            CHECK_GL();
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        {
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER, 
                scene.triangles().size() * sizeof(Triangle),
                scene.triangles().data(),
                GL_STATIC_DRAW
            );
            CHECK_GL();
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);
    CHECK_GL();
    m_scene = std::move(scene);
    m_vao = vao;
    m_vbo = vbo;
    m_ebo = ebo;
    return Result<void>::ok();

#undef CHECK_GL
}

void Renderer::set_shader(ShaderProgram const& shader) noexcept {
    m_shader = &shader;
}

auto Renderer::render() noexcept -> Result<RenderResult> {
    if(!valid()) {
        return std::string { "render() called on invalid renderer" };
    }
    if(!m_shader->valid()) {
        return std::string { "render() called on renderer with invalid shader" };
    }
    
    glBindVertexArray(m_vao);
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        {
            // check for errors
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                return std::string{ "OpenGL error code: " } + std::to_string(err);
            }

            auto scope = m_shader->use_scope();
            glDrawElements(GL_TRIANGLES,
                static_cast<GLsizei>(m_scene.triangles().size() * 3),
                GL_UNSIGNED_INT,
                0);

            // check for errors
            err = glGetError();
            if (err != GL_NO_ERROR) {
                return std::string{ "OpenGL error code: " } + std::to_string(err);
            }
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);
    return RenderResult{};
}