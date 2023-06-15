#include <GL/gl.h>
#include <string>
#include <string_view>

struct Shader {
    Shader(std::string_view vs_file, std::string_view ps_file);
    Shader(std::string_view vs_src, std::string_view ps_src);
    ~Shader();
    void use();

private:
    GLuint m_program;
    GLuint m_vertex_shader;
    GLuint m_pixel_shader;
};