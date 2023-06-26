#pragma once
#include "types.h"
#include "ext.h"
#include "transform.h"

struct Camera {
    Camera(real fovy, real aspect, Transform const& transform) noexcept;
    
    [[nodiscard]] auto get_view_proj() const noexcept -> mat4 const& { return m_view_proj; }
    [[nodiscard]] auto get_transform() const noexcept -> Transform const& { return m_transform; }
    [[nodiscard]] auto get_projection() const noexcept -> mat4 const& { return m_projection; }
    void set_rotation(vec3 const& rot) noexcept; 
    void set_position(vec3 const& pos) noexcept;
private:
    Transform m_transform;
    mat4 m_projection;
    mat4 m_view_proj;
};