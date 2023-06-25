#include "object.h"

#include <iostream>

Transform::Transform(vec3 pos, vec3 rot, vec3 scale) noexcept {
    mat4 const t = glm::translate(mat4(1.0), pos);
    mat4 r = glm::rotate(mat4(1.0), rot.x, vec3(1, 0, 0));
    r = glm::rotate(r, rot.y, vec3(0, 1, 0));
    r = glm::rotate(r, rot.z, vec3(0, 0, 1));
    mat4 const s = glm::scale(mat4(1.0), scale);
    m_trans = t * r * s;
}

Object::Object(ShaderProgram const& shader, Transform transform) 
    : m_program{&shader}, m_transform{std::move(transform)} 
{ }

auto Object::from_obj(std::string_view filename) noexcept -> Result<void> {
    m_vertices.clear();
    m_triangles.clear();

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
    for (size_t i = 2; i < attrib.vertices.size(); i += 3) {
        Vertex v;
        v.position = vec3{ attrib.vertices[i - 2], attrib.vertices[i - 1], attrib.vertices[i] };

        // these two will be overwritten later (if they exist)
        v.normal = vec3{ -1, -1, -1 };
        v.uv = vec2{ -1, -1 };

        m_vertices.emplace_back(v);
    }

    // TODO: normals and textures
    for (auto const& s : reader.GetShapes()) {
        auto const& indices = s.mesh.indices;
        for (size_t i = 0; i < s.mesh.material_ids.size(); ++i) {

            Triangle t;
            for (size_t j = 0; j < 3; ++j) {
                t.vert_indices[j] = indices[3 * i + j].vertex_index;
                int ni = indices[3 * i + j].normal_index;
                int uvi = indices[3 * i + j].texcoord_index;

                // fill back in the normals and textures if they exist
                if (ni != -1) {
                    m_vertices[t.vert_indices[j]].normal = vec3{
                        attrib.normals[3 * ni + 0],
                        attrib.normals[3 * ni + 1],
                        attrib.normals[3 * ni + 2]
                    };
                }
                if (uvi != -1) {
                    m_vertices[t.vert_indices[j]].uv = vec2{
                        attrib.texcoords[2 * uvi + 0],
                        attrib.texcoords[2 * uvi + 1]
                    };
                }
            }

        }
    }
    return Result<void>::ok();
}

void Object::set_shader(ShaderProgram const& shader) noexcept {
    m_program = &shader;
}

auto Object::begin_draw() const noexcept -> Result<void> {
    m_program->use();
    auto const res = m_program->set_uniform(k_uniform_model, m_transform.get_matrix());
    if (!res.valid()) {
        return res;
    }
    return Result<void>::ok();
}

auto Object::end_draw() const noexcept -> Result<void> {
    m_program->unuse();
    return Result<void>::ok();
}

auto Object::make_triangle_obj(ShaderProgram const& shader, Transform const& trans) noexcept -> Object {
    Object obj{ shader, trans };
	obj.m_vertices = {
	    Vertex{ vec3{ -0.5, -0.5, 0 }, vec3{ 0, 0, 1 }, vec2{ 0, 0 } },
	    Vertex{ vec3{ 0.5, -0.5, 0 }, vec3{ 0, 0, 1 }, vec2{ 1, 0 } },
	    Vertex{ vec3{ 0, 0.5, 0 }, vec3{ 0, 0, 1 }, vec2{ 0.5, 1 } }
    };
    obj.m_triangles = {
        Triangle{ { 0, 1, 2 } }
    };
    
    return obj;
}
