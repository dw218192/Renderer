#include "transform.h"
#include <glm/gtc/matrix_access.hpp>

Transform::Transform() noexcept 
    : m_pos(0,0,0), m_rot(0,0,0), m_scale(1,1,1), m_trans(1.0)
{ }

Transform::Transform(mat4 const& mat) noexcept 
    : m_trans{mat} 
{
    m_pos = glm::column(mat, 3);
    vec3 r1 = glm::row(mat, 0), 
         r2 = glm::row(mat, 1), 
         r3 = glm::row(mat, 2);
    m_scale = vec3 {
        glm::length(r1),
        glm::length(r2),
        glm::length(r3)
    };
    assert(glm::length(r1) > 0.0 && glm::length(r2) > 0.0 && glm::length(r3) > 0.0);
    
    r1 /= m_scale.x;
    r2 /= m_scale.y;
    r3 /= m_scale.z;
    m_rot = vec3 {
        glm::degrees(atan2(r3.y, r3.z)),
        glm::degrees(atan2(-r3.x, sqrt(r3.y * r3.y + r3.z * r3.z))),
        glm::degrees(atan2(r2.x, r1.x))
    };

    assert(m_trans == 
        glm::translate(mat4(1.0), m_pos) * 
        glm::rotate(mat4(1.0), glm::radians(m_rot.x), vec3(1, 0, 0)) * 
        glm::rotate(mat4(1.0), glm::radians(m_rot.y), vec3(0, 1, 0)) * 
        glm::rotate(mat4(1.0), glm::radians(m_rot.z), vec3(0, 0, 1)) * glm::scale(mat4(1.0), m_scale)
    );
}

Transform::Transform(vec3 const& pos, vec3 const& rot, vec3 const& scale) noexcept
    : m_pos(pos), m_rot(rot), m_scale(scale)
{
    mat4 const t = glm::translate(mat4(1.0), pos);
    mat4 r = glm::rotate(mat4(1.0), rot.x, vec3(1, 0, 0));
    r = glm::rotate(r, rot.y, vec3(0, 1, 0));
    r = glm::rotate(r, rot.z, vec3(0, 0, 1));
    mat4 const s = glm::scale(mat4(1.0), scale);
    m_trans = t * r * s;
}

auto Transform::look_at(vec3 const& pos, vec3 const& target, vec3 const& up) noexcept -> Transform {
    mat4 trans = glm::lookAt(pos, target, up);
    return Transform{ trans };
}

void Transform::set_rotation(vec3 const& rot) noexcept {
    *this = Transform(m_pos, rot, m_scale);
}

void Transform::set_position(vec3 const& pos) noexcept {
    *this = Transform(pos, m_rot, m_scale);
}

void Transform::set_scale(vec3 const& scale) noexcept {
    *this = Transform(m_pos, m_rot, scale);
}