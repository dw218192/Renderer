#pragma once
#include "ext.h"
#include "result.h"
#include "shader.h"

#include <array>
#include <string_view>

// TODO: move these to a separate file
struct Triangle {
    std::array<unsigned, 3> vert_indices;
};

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
};

struct Transform {
	explicit Transform(mat4 const& trans = mat4(1.0)) noexcept : m_trans(trans) {}

    // create a transform from a position, rotation, and scale
    // rotation is in degrees, and is applied in the order XYZ
    Transform(vec3 pos, vec3 rot, vec3 scale = vec3(1,1,1)) noexcept;
    [[nodiscard]] auto get_matrix() const noexcept -> mat4 const& { return m_trans; }
private:
    mat4 m_trans;
};

struct Object {
    Object(ShaderProgram const& shader, Transform transform);
    [[nodiscard]] auto from_obj(std::string_view filename) noexcept -> Result<void>;

    void set_shader(ShaderProgram&&) = delete;
	void set_shader(ShaderProgram const& shader) noexcept;

    [[nodiscard]] auto begin_draw() const noexcept -> Result<void>;
    [[nodiscard]] auto end_draw() const noexcept -> Result<void>;
    [[nodiscard]] auto vertices() const noexcept -> std::vector<Vertex> const& { return m_vertices; }
    [[nodiscard]] auto triangles() const noexcept -> std::vector<Triangle> const& { return m_triangles; }
    [[nodiscard]] static auto make_triangle_obj(ShaderProgram const& shader, Transform const& trans) noexcept -> Object;

private:
    Transform m_transform;
    ShaderProgram const* m_program;
    std::vector<Vertex> m_vertices;
    std::vector<Triangle> m_triangles;
};
