#include "object.h"
#include "camera.h"
#include <iostream>

Object::Object(ShaderProgram const& shader, Transform transform) 
    : m_program{&shader}, m_transform{std::move(transform)} 
{ }

auto Object::from_obj(std::string_view filename) noexcept -> Result<void> {
    m_vertices.clear();

    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = "./";

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filename.data(), config)) {
        if (!reader.Error().empty()) {
            return "Tiny Obj Error:\n" + reader.Error();
        }
        else {
            return std::string{ "Unknown Tiny Obj Error" };
        }
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader:\n" << reader.Warning();
    }

    auto const& attrib = reader.GetAttrib();
    // TODO: triangles with the same vertices may not have the same normal or uv
    for (auto const& s : reader.GetShapes()) {
        auto const& indices = s.mesh.indices;
        for (size_t i = 0; i < s.mesh.material_ids.size(); ++i) {
            for (size_t j = 0; j < 3; ++j) {
                int vi = indices[3 * i + j].vertex_index;
                int ni = indices[3 * i + j].normal_index;
                int uvi = indices[3 * i + j].texcoord_index;

                // fill back in the normals and textures if they exist
                Vertex vertex;
                if (ni != -1) {
                    vertex.normal = vec3{ 
                        attrib.normals[3 * ni + 0],
                        attrib.normals[3 * ni + 1],
                        attrib.normals[3 * ni + 2]
                    };
                }
                if (uvi != -1) {
                    vertex.uv = vec2{
                        attrib.texcoords[2 * uvi + 0],
                        attrib.texcoords[2 * uvi + 1]
                    };
                }
                vertex.position = vec3{ 
                    attrib.vertices[3 * vi + 0], 
                    attrib.vertices[3 * vi + 1],
                    attrib.vertices[3 * vi + 2]
                };

                m_vertices.emplace_back(vertex);
            }
        }
    }
    return Result<void>::ok();
}

void Object::set_shader(ShaderProgram const& shader) noexcept {
    m_program = &shader;
}

auto Object::begin_draw(Camera const& cam) const noexcept -> Result<void> {
    m_program->use();
    auto res = m_program->set_uniform(k_uniform_model, m_transform.get_matrix());
    if (!res.valid()) {
        return res;
    }
    res = m_program->set_uniform(k_uniform_view, cam.get_transform().get_matrix());
    if (!res.valid()) {
        return res;
    }
    res = m_program->set_uniform(k_uniform_projection, cam.get_projection());
    if (!res.valid()) {
        return res;
    }
    return Result<void>::ok();
}

void Object::end_draw() const noexcept {
    m_program->unuse();
}

auto Object::make_triangle_obj(ShaderProgram const& shader, Transform const& trans) noexcept -> Object {
    Object obj{ shader, trans };
	obj.m_vertices = {
	    Vertex{ vec3{ -0.5, -0.5, 0 }, vec3{ 0, 0, 1 }, vec2{ 0, 0 } },
	    Vertex{ vec3{ 0.5, -0.5, 0 }, vec3{ 0, 0, 1 }, vec2{ 1, 0 } },
	    Vertex{ vec3{ 0, 0.5, 0 }, vec3{ 0, 0, 1 }, vec2{ 0.5, 1 } }
    };

    return obj;
}
