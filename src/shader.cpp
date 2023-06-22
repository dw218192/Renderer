#include "shader.h"
#include <fstream>

Shader::Shader(ShaderType type) noexcept : m_handle(0) {
    switch (type) {
    case ShaderType::Vertex:
        m_type = GL_VERTEX_SHADER;
        break;
    case ShaderType::Fragment:
        m_type = GL_FRAGMENT_SHADER;
        break;
    }
}
Shader::~Shader() noexcept {
    if (m_handle) {
        glDeleteShader(m_handle);
    }
}

auto Shader::from_file(std::string_view file) noexcept -> Result<void> {
    std::ifstream stream(file.data());
    if (!stream.is_open()) {
        return std::string("Failed to open vertex shader file");
    }

    std::string src{std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()};    
    return from_src(src);
}

auto Shader::from_src(std::string_view src) noexcept -> Result<void> {
    GLuint handle = glCreateShader(m_type);
    if (!handle) {
        if (m_type == GL_VERTEX_SHADER) {
            return std::string("Failed to create vertex shader");
        } else {
            return std::string("Failed to create pixel shader");
        }
    }

    auto src_ptr = src.data();
    glShaderSource(handle, 1, static_cast<GLchar const**>(&src_ptr), nullptr);
    glCompileShader(handle);

    GLint status;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &maxLength);

        std::string infoLog(maxLength, '\0');
        glGetShaderInfoLog(handle, maxLength, &maxLength, static_cast<GLchar*>(infoLog.data()));
        
        return infoLog;
    }

    m_handle = handle;
    return Result<void>::ok();
}


ShaderProgram::~ShaderProgram() {
    if (m_handle) {
        glDeleteProgram(m_handle);
    }
}
void ShaderProgram::use() const noexcept {
    glUseProgram(m_handle);
}
void ShaderProgram::unuse() const noexcept {
    glUseProgram(0);
}


auto ShaderProgram::from_shaders(Shader vs, Shader ps) -> Result<void> {
    if(!vs.valid()) {
        return std::string("Invalid vertex shader");
    }
    if(!ps.valid()) {
        return std::string("Invalid pixel shader");
    }

    GLuint program = glCreateProgram();
    if (!program) {
        return std::string("Failed to create shader program");
    }
    glAttachShader(program, vs.m_handle);
    glAttachShader(program, ps.m_handle);
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        std::string infoLog(maxLength, '\0');
        glGetProgramInfoLog(program, maxLength, &maxLength, static_cast<GLchar*>(infoLog.data()));
        
        return infoLog;
    }

    m_handle = program;
    m_vs = std::move(vs);
    m_ps = std::move(ps);
    return Result<void>::ok();
}