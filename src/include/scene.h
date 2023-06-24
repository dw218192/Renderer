#pragma once
#include "ext.h"
#include "result.h"

#include <string_view>
#include <vector>
#include <array>

struct Triangle {
    std::array<int, 3> vert_indices;
};

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
};

struct Scene {
    [[nodiscard]] auto from_obj(std::string_view filename) noexcept -> Result<void>;
    [[nodiscard]] auto vertices() const noexcept -> std::vector<Vertex> const& { return m_vertices; }
    [[nodiscard]] auto triangles() const noexcept -> std::vector<Triangle> const& { return m_triangles; }

// for test only
    static auto make_triangle_scene() noexcept -> Scene;

private:
    std::vector<Vertex> m_vertices;
    std::vector<Triangle> m_triangles;
};