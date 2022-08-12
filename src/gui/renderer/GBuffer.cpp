//
// Created by tomas on 2.8.22.
//

#include <Logger.h>
#include <geGL/geGL.h>
#include <renderer/GBuffer.h>

namespace MapGenerator::Renderer {

    GBuffer::GBuffer(const ge::gl::Context& gl) { this->gl = gl; }

    void GBuffer::init(int width, int height, int defaultFbo) {
        // Prepare quad VAO
        this->quadBuffer = std::make_shared<ge::gl::Buffer>(12 * sizeof(float), vertices.data());
        this->quadVAO = std::make_shared<ge::gl::VertexArray>();
        this->quadVAO->bind();
        quadVAO->addAttrib(quadBuffer, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);
        // Prepare gBuffer
        this->gBuffer = std::make_shared<ge::gl::Framebuffer>();
        // This will prepare all textures and attachments
        resize(width, height, defaultFbo);
    }

    void GBuffer::resize(int width, int height, int defaultFbo) {
        this->gBuffer->bind(GL_FRAMEBUFFER);

        // Prepare position texture
        auto test = ge::gl::Texture(GL_TEXTURE_2D, GL_RGBA16F, 0, width, height);
        this->gPosition = std::make_shared<class ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA16F, 0, width, height);
        gl.glBindTexture(GL_TEXTURE_2D, this->gPosition->getId());
        this->gPosition->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        this->gPosition->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->gPosition->getId(), 0);

        // Prepare normal texture
        this->gNormal = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA16F, 0, width, height);
        gl.glBindTexture(GL_TEXTURE_2D, this->gNormal->getId());
        this->gNormal->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        this->gNormal->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, this->gNormal->getId(), 0);

        // Prepare the specular texture
        this->gSpecular = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA, 0, width, height);
        gl.glBindTexture(GL_TEXTURE_2D, gSpecular->getId());
        gSpecular->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        gSpecular->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gSpecular->getId(), 0);

        auto err = gl.glGetError();
        if (err != GL_NO_ERROR) {
            std::cout << "GBuffer error: " << err << std::endl;
        }
        unsigned int att[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
        gl.glDrawBuffers(4, att);
        // prep the buffers
        this->rboDepth = std::make_shared<ge::gl::Renderbuffer>();
        this->rboDepth->bind();
        gl.glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        gl.glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth->getId());

        // finally check if framebuffer is complete
        if (gl.glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            Logger::log("Framebuffer not complete. You should not see this message!!");
        }
        gl.glBindFramebuffer(GL_FRAMEBUFFER, defaultFbo);
    }

    std::shared_ptr<ge::gl::Texture> GBuffer::positionTexture() const {
        return this->gPosition;
    }
    std::shared_ptr<ge::gl::Texture> GBuffer::normalTexture() const {
        return this->gNormal;
    }

    std::shared_ptr<ge::gl::Texture> GBuffer::colorTexture() const {
        return this->gSpecular;
    }

    void GBuffer::bindBuffer() {
        gl.glBindFramebuffer(GL_FRAMEBUFFER, this->gBuffer->getId());
    }

} // namespace MapGenerator::Renderer


