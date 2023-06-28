#pragma once
#include "types.h"
#include "ext.h"
#include "transform.h"

struct Camera {
    Camera(real fovy, real aspect, Transform const& transform) noexcept;
    
    [[nodiscard]] auto get_view_proj() const noexcept -> mat4 const& { return m_view_proj; }
    [[nodiscard]] auto get_transform() const noexcept -> Transform const& { return m_transform; }
    [[nodiscard]] auto get_projection() const noexcept -> mat4 const& { return m_projection; }
    [[nodiscard]] auto get_fov() const noexcept { return m_fov; }

    void set_rotation(TransformSpace space, vec3 const& rot) noexcept;
    void set_position(TransformSpace space, vec3 const& pos) noexcept;
    void set_fov(real fov) noexcept;
private:
    static constexpr real k_near = REAL_LITERAL(0.1), k_far = REAL_LITERAL(100.0);
    Transform m_transform;
    real m_fov, m_aspect;
    mat4 m_projection;
    mat4 m_view_proj;
}; 