#include "renderer.h"
#include <cassert>
#include <algorithm>

Renderer::Renderer(RenderConfig config) noexcept :
	m_config(config), m_vao(0) {}

Renderer::~Renderer() noexcept {
    if(valid()) {
        assert(m_vao);
        for (GLuint const handle : m_buffer_handles) assert(handle);

        glDeleteBuffers(get_bufhandle_size(), get_bufhandles());
        glDeleteVertexArrays(1, &m_vao);
    }
}

auto Renderer::exec(Cmd const& cmd) noexcept -> Result<void> {
    (void)cmd;
    return std::string { "Not implemented" };
}

auto Renderer::open_scene(Scene scene) noexcept -> Result<void> {
    GLenum err;
    auto roll_back = [this]() {
        if (m_vao) glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(get_bufhandle_size(), get_bufhandles());
    };

#define CHECK_GL() do {\
	err = glGetError();\
	if(err != GL_NO_ERROR) {\
		roll_back();\
		return GLErrorResult<void>(err);\
	} } while(false)

    if(!valid()) {
        // set up buffers
        glGenVertexArrays(1, &m_vao);

        m_buffer_handles.resize(2);
        glGenBuffers(2, m_buffer_handles.data());

        auto const res = m_res.init(m_config.width, m_config.height);
        if(!res.valid()) {
            return res.error();
        }
        CHECK_GL();
    }

    // fill buffers with data
    glBindVertexArray(m_vao);
    {
        glBindBuffer(GL_ARRAY_BUFFER, get_vbo());
        {
            // gather all vertices from all objects in the scene
            std::vector<Vertex> all_vertices;
            for(auto&& obj : scene.objects()) {
                all_vertices.insert(all_vertices.end(), obj.vertices().begin(), obj.vertices().end());
            }

            glBufferData(
                GL_ARRAY_BUFFER, 
                all_vertices.size() * sizeof(Vertex),
                all_vertices.data(),
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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, get_ebo());
        {
            std::vector<Triangle> all_triangles;
            for (size_t i = 0, offset = 0; i < scene.objects().size(); ++i) {
                auto&& obj = scene.objects()[i];
                std::vector<Triangle> triangles(obj.triangles().size());
                std::transform(obj.triangles().begin(), obj.triangles().end(), triangles.begin(), 
                    [offset](Triangle const& in)->Triangle {
	                    Triangle out = in;
	                    for (auto&& vi : out.vert_indices) {
	                        vi += static_cast<unsigned>(offset);
	                    }
	                    return out;
					}
                );

                all_triangles.insert(all_triangles.end(), triangles.begin(), triangles.end());
                offset += obj.vertices().size();
            }

            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                all_triangles.size() * sizeof(Triangle),
                all_triangles.data(),
                GL_STATIC_DRAW
            );
            CHECK_GL();
        }
    }
    glBindVertexArray(0);
    CHECK_GL();

    m_scene = std::move(scene);


    // Set a few settings/modes in OpenGL rendering
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, get_ebo());
    m_res.prepare();
    CHECK_GL();

    return Result<void>::ok();

#undef CHECK_GL
}

auto Renderer::render() noexcept -> Result<RenderResult const&> {
    if(!valid()) {
        return std::string { "render() called on invalid renderer" };
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (size_t i = 0, ebo_offset = 0; i < m_scene.objects().size(); ++i) {
        auto&& obj = m_scene.objects()[i];

        auto res = obj.begin_draw();
        if (!res.valid()) {
            return res.error();
        }
        {
            glDrawElements(GL_TRIANGLES,
                static_cast<GLsizei>(obj.triangles().size() * 3),
                GL_UNSIGNED_INT,
                reinterpret_cast<void*>(ebo_offset * sizeof(Triangle))
            );
            ebo_offset += obj.triangles().size();
        }
        res = obj.end_draw();
        if (!res.valid()) {
            return res.error();
        }
    }

    // check for errors
    auto const err = glGetError();
    if (err != GL_NO_ERROR) {
        return GLErrorResult<RenderResult const&>(err);
    }

    return m_res;
}