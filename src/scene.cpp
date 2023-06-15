#include "scene.h"
#include "ext.h"

#include <iostream>

auto Scene::from_obj(std::string_view filename) noexcept -> Result<void> {
    m_vertices.clear();
    m_normals.clear();
    m_uvs.clear();
    m_triangles.clear();

    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = "./";
    
    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(filename.data(), config)) {
        if (!reader.Error().empty()) {
            return Result<void>{ reader.Error() };
        } else {
            return Result<void>{ "Unknown error" };
        }
        return false;
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader:\n" << reader.Warning();
    }

    auto const& attrib = reader.GetAttrib();
    for (size_t i = 2; i < attrib.vertices.size(); i += 3) {
        m_vertices.emplace_back(
            attrib.vertices[i - 2],
            attrib.vertices[i - 1],
            attrib.vertices[i]
        );
    }

    // TODO: normals and textures
    for (auto const& s : reader.GetShapes()) {
        auto const& indices = s.mesh.indices;
        for (size_t i = 0; i < s.mesh.material_ids.size(); ++i) {
            Triangle t;
            for(size_t j = 0; j < 3; ++j) {
                t.verts[j] = indices[3 * i + j].vertex_index;
                t.norms[j] = indices[3 * i + j].normal_index;
                t.uvs[j] = indices[3 * i + j].texcoord_index;
            }
        }
    }
    return Result<void>::ok();
}