#include "camera.h"

Camera::Camera(real fovy, real aspect, Transform const& transform) noexcept 
    : m_transform(transform)
{
    m_projection = glm::perspective(fovy, aspect, static_cast<real>(0.1), static_cast<real>(100.0));
    m_view_proj = m_projection * m_transform.get_matrix();
}

void Camera::set_rotation(vec3 const& rot) noexcept {
    m_transform.set_rotation(rot);
    m_view_proj = m_projection * m_transform.get_matrix();
}

void Camera::set_position(vec3 const& pos) noexcept {
    m_transform.set_position(pos);
    m_view_proj = m_projection * m_transform.get_matrix();
}