#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(real fovy, real aspect, Transform const& transform) noexcept 
    : m_transform(transform)
{
    m_projection = glm::perspective(glm::radians(fovy), aspect, static_cast<real>(0.1), static_cast<real>(100.0));
    m_view_proj = m_projection * m_transform.get_matrix();
}

void Camera::set_rotation(TransformSpace space, vec3 const& rot) noexcept {
    m_transform.set_rotation(space, rot);
    m_view_proj = m_projection * m_transform.get_matrix();
}

void Camera::set_position(TransformSpace space, vec3 const& pos) noexcept {
    m_transform.set_position(space, pos);
    m_view_proj = m_projection * m_transform.get_matrix();
}