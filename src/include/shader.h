#pragma once

#include <GL/glew.h>
#include <string>
#include <string_view>

#include "result.h"

enum class ShaderType {
    Vertex,
    Fragment
};

struct Shader {
    friend struct ShaderProgram;
    Shader(ShaderType type) noexcept;
    ~Shader() noexcept;

    auto from_file(std::string_view file) noexcept -> Result<void>;
    auto from_src(std::string_view src) noexcept -> Result<void>;
    auto valid() const noexcept -> bool { return m_handle != 0; }
    void use() const noexcept;

private:
    Shader() noexcept : m_handle(0) { }
    GLenum m_type;
    GLuint m_handle;
};

struct ShaderProgram;
struct ShaderProgramUseScope {
    ShaderProgramUseScope(ShaderProgram const& program) noexcept;
    ~ShaderProgramUseScope() noexcept;
private:
    ShaderProgram const& m_program;
};

struct ShaderProgram {
    ShaderProgram() noexcept : m_handle(0) { }
    ~ShaderProgram() noexcept;

    auto from_shaders(Shader vs, Shader ps) -> Result<void>;
    auto valid() const noexcept -> bool { return m_handle != 0; }
    void use() const noexcept;
    void unuse() const noexcept;
    ShaderProgramUseScope use_scope() const noexcept { return ShaderProgramUseScope(*this); }
private:
    GLuint m_handle;
    Shader m_vs;
    Shader m_ps;
};

inline ShaderProgramUseScope::ShaderProgramUseScope(ShaderProgram const& program) noexcept : m_program(program) {
    m_program.use();
}
inline ShaderProgramUseScope::~ShaderProgramUseScope() noexcept {
    m_program.unuse();
}