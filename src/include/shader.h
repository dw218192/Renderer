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
    explicit Shader(ShaderType type) noexcept;
    ~Shader() noexcept;

    // shouldn't be copied because we have handles to GL resources
    Shader(Shader&) = delete;

    [[nodiscard]] auto from_file(std::string_view file) noexcept -> Result<void>;
    [[nodiscard]] auto from_src(std::string_view src) noexcept -> Result<void>;
    [[nodiscard]] auto valid() const noexcept -> bool { return m_handle != 0; }

private:
    Shader() noexcept : m_type{0}, m_handle(0) { }
    GLenum m_type;
    GLuint m_handle;
};

struct ShaderProgram;
struct ShaderProgramUseScope {
    explicit ShaderProgramUseScope(ShaderProgram const& program) noexcept;
    ~ShaderProgramUseScope() noexcept;
private:
    ShaderProgram const& m_program;
};

struct ShaderProgram {
    ShaderProgram() noexcept : m_handle(0) { }
    ~ShaderProgram() noexcept;

    // shouldn't be copied because we have handles to GL resources
    ShaderProgram(ShaderProgram&) = delete;

    // disallow rvalues to be passed as it would cause dangling reference
	auto from_shaders(Shader&& vs, Shader&& ps)->Result<void> = delete;
    auto from_shaders(Shader const& vs, Shader&& ps)->Result<void> = delete;
    auto from_shaders(Shader&& vs, Shader const& ps)->Result<void> = delete;

    [[nodiscard]] auto from_shaders(Shader const& vs, Shader const& ps) noexcept -> Result<void>;
    [[nodiscard]] auto valid() const noexcept -> bool { return m_handle != 0; }
    void use() const noexcept;
    void unuse() const noexcept;
    [[nodiscard]] ShaderProgramUseScope use_scope() const noexcept { return ShaderProgramUseScope(*this); }
private:
    GLuint m_handle;
    Shader const* m_vs;
    Shader const* m_ps;
};

inline ShaderProgramUseScope::ShaderProgramUseScope(ShaderProgram const& program) noexcept : m_program(program) {
    m_program.use();
}
inline ShaderProgramUseScope::~ShaderProgramUseScope() noexcept {
    m_program.unuse();
}