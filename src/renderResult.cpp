#include "renderResult.h"
#include "ext.h"
#include "shader.h"

auto RenderResult::init(unsigned width, unsigned height) noexcept -> Result<void> {
    if(m_fbo) {
        return std::string{ "reinitialization is not supported"};
    }

    GLuint fbo, rbo, tex;
    GLenum err;
    GLsizei const w = width;
    GLsizei const h = height;

    // create frame buffer
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    {
        // we need depth testing
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        {
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);

        err = glGetError();
        if (err != GL_NO_ERROR) {
            return GLErrorResult<void>(err);
        }

        // create tex to render to
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        {
            glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGB, // RGB color format
                w, h,
                0, GL_RGB, GL_UNSIGNED_BYTE, 0
            );

            err = glGetError();
            if (err != GL_NO_ERROR) {
                return GLErrorResult<void>(err);
            }

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex, 0);
            GLenum const draw_buffer = GL_COLOR_ATTACHMENT0;
            glDrawBuffers(1, &draw_buffer);

            err = glGetError();
            if (err != GL_NO_ERROR) {
                return GLErrorResult<void>(err);
            }

            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                return std::string{ "frame buffer is not valid" };
            }
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_width = w;
    m_height = h;
    // TODO: don't hardcode num of channels
    m_linear_sz = m_width * m_height * 3;
    m_pixels.resize(m_linear_sz);
    m_fbo = fbo;
    m_depth_rbo = rbo;
    m_tex = tex;
    return Result<void>::ok();
}

void RenderResult::save(std::string_view file_path) const noexcept {
    const_cast<RenderResult*>(this)->update();

    // stbi_flip_vertically_on_write(1);
    stbi_write_png(file_path.data(), m_width, m_height, 3, m_pixels.data(), m_width * 3);
}

auto RenderResult::upload_to_frame_buffer() const noexcept -> Result<void> {
    //static Shader pass_through_vs{ ShaderType::Vertex };
    //static Shader pass_through_ps{ ShaderType::Fragment };
    //static ShaderProgram program;
    //if(!pass_through_vs.valid()) {
	   // // first time init
    //    auto const res = pass_through_vs.from_src("\
	   //     #version 330 core\n\
	   //     layout (location = 0) in vec3 aPos;\n\
	   //     void main() {\n\
	   //         gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
	   //     }\n\
	   // ");
    //}

    return std::string{ "not implemented" };
}

void RenderResult::prepare() const noexcept {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
}

void RenderResult::update() noexcept {
    glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, m_pixels.data());
}

RenderResult::RenderResult() noexcept
	: m_fbo{0}, m_tex{0}, m_depth_rbo{0}, m_width{0}, m_height{0}, m_linear_sz{0} {}

RenderResult::~RenderResult() noexcept {
    if (m_fbo) {
        assert(m_fbo && m_depth_rbo && m_tex);
        glDeleteFramebuffers(1, &m_fbo);
        glDeleteRenderbuffers(1, &m_depth_rbo);
        glDeleteTextures(1, &m_tex);
    }
}
