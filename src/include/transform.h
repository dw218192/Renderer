#pragma once

#include "ext.h"

struct Transform {
    Transform() noexcept;
    Transform(mat4 const& mat) noexcept;
    // create a transform from a position, rotation, and scale
    // rotation is in degrees, and is applied in the order XYZ
    Transform(vec3 const& pos, vec3 const& rot, vec3 const& scale = vec3(1,1,1)) noexcept;
    static auto look_at(vec3 const& pos, vec3 const& target, vec3 const& up) noexcept -> Transform;

    [[nodiscard]] auto get_position() const noexcept -> vec3 const& { return m_pos; }
    [[nodiscard]] auto get_rotation() const noexcept -> vec3 const& { return m_rot; }
    [[nodiscard]] auto get_scale() const noexcept -> vec3 const& { return m_scale; }
    [[nodiscard]] auto get_matrix() const noexcept -> mat4 const& { return m_trans; }

    void set_rotation(vec3 const& rot) noexcept;
    void set_position(vec3 const& pos) noexcept;
    void set_scale(vec3 const& scale) noexcept;

private:
    vec3 m_pos;
    vec3 m_rot;
    vec3 m_scale;
    mat4 m_trans;
};