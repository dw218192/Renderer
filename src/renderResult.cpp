#include "renderResult.h"
#include "ext.h"

auto RenderResult::init(unsigned width, unsigned height) noexcept -> Result<void> {
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
    // TODO: don't hard code channel
    m_linear_sz = m_width * m_height * 3;
    m_pixels.resize(m_linear_sz);
    m_fbo = fbo;
    m_depth_rbo = rbo;
    m_tex = tex;
    return Result<void>::ok();
}

void RenderResult::save(std::string_view file_path) const noexcept {
    const_cast<RenderResult*>(this)->update();

    stbi_flip_vertically_on_write(1);
    stbi_write_png(file_path.data(), m_width, m_height, 3, m_pixels.data(), m_width * 3);
}

void RenderResult::prepare() const noexcept {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_width, m_height);
}

void RenderResult::update() noexcept {
    glReadPixels(0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, m_pixels.data());
}

RenderResult::~RenderResult() {
    glDeleteTextures(1, &m_tex);
}
