#pragma once
#include "ext.h"
#include "result.h"

#include <string_view>
#include <vector>
#include <array>

struct Triangle {
    std::array<int, 3> verts;
    std::array<int, 3> norms;
    std::array<int, 3> uvs;
};

struct Scene {
    Scene() {}
    ~Scene() {}
    auto from_obj(std::string_view filename) noexcept -> Result<void>;
    auto vertices() const noexcept -> std::vector<vec3> const& { return m_vertices; }
    auto normals() const noexcept -> std::vector<vec3> const& { return m_normals; }
    auto uvs() const noexcept -> std::vector<vec2> const& { return m_uvs; }
    auto triangles() const noexcept -> std::vector<Triangle> const& { return m_triangles; }

private:
    std::vector<vec3> m_vertices;
    std::vector<vec3> m_normals;
    std::vector<vec2> m_uvs;
    std::vector<Triangle> m_triangles;
    
};