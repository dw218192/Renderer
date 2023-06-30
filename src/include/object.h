#pragma once
#include "ext.h"
#include "types.h"
#include "result.h"
#include "shader.h"
#include "transform.h"

#include <array>
#include <string_view>

struct AABB {
    vec3 min_pos, max_pos;

    [[nodiscard]] vec3 get_center() const noexcept {
        return (min_pos + max_pos) * REAL_LITERAL(0.5);
    }
    [[nodiscard]] vec3 get_extent() const noexcept {
        return (max_pos - min_pos) * REAL_LITERAL(0.5);
    }
};

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
};

struct Object {
    Object(ShaderProgram const& shader, Transform transform);
    [[nodiscard]] auto from_obj(std::string_view filename) noexcept -> Result<void>;

    void set_shader(ShaderProgram&&) = delete;
	void set_shader(ShaderProgram const& shader) noexcept;

    [[nodiscard]] auto begin_draw(struct Camera const& cam) const noexcept -> Result<void>;
    void end_draw() const noexcept;
    [[nodiscard]] auto vertices() const noexcept -> std::vector<Vertex> const& { return m_vertices; }
    [[nodiscard]] static auto make_triangle_obj(ShaderProgram const& shader, Transform const& trans) noexcept -> Object;
    [[nodiscard]] auto bound() const noexcept -> AABB const& { return m_bound; }
private:
    AABB m_bound;
    Transform m_transform;
    ShaderProgram const* m_program;
    std::vector<Vertex> m_vertices;
};
