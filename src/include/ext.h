#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifdef DOUBLE_PRECISION
#define TINYOBJLOADER_USE_DOUBLE
enum GLConstants {
    GL_FLOAT_TYPE = GL_DOUBLE
};
using gltype = GLdouble;
using vec3 = glm::dvec3;
using vec2 = glm::dvec2;
using mat3 = glm::dmat3;
using mat4 = glm::dmat4;
#elif SINGLE_PRECISION
enum GLConstants {
    GL_FLOAT_TYPE = GL_FLOAT
};
using gltype = GLfloat;
using vec3 = glm::vec3;
using vec2 = glm::vec2;
using mat3 = glm::mat3;
using mat4 = glm::mat4;
#else
#error "Must define either SINGLE_PRECISION or DOUBLE_PRECISION"
#endif

#include <tiny_obj_loader.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>