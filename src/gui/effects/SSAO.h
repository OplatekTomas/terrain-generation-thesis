//
// Created by tomas on 05.04.22.
//


#pragma once

#include "geGL/OpenGLContext.h"
#include <functional>
#include <glm/glm.hpp>
#include <memory>
#include "geGL/Texture.h"
#include "geGL/Program.h"
#include "geGL/Buffer.h"
#include "geGL/VertexArray.h"
#include "geGL/Framebuffer.h"

namespace MapGenerator {
    class SSAO {
    public:
        SSAO(const std::shared_ptr<ge::gl::Context> &gl, int width, int height, int fbo,
             const std::function<void()> &drawFn);

        void clear(int fbo);

        void setDimensions(int width, int height, int fbo = 0);

        void render(const std::shared_ptr<ge::gl::Texture> &depthTexture,
                    const std::shared_ptr<ge::gl::Texture> &normalTexture, int fbo);

        void bind(int unit);

        void bindWithoutBlur(int unit);

        void renderBlur(int fbo);

    private:


        void initBlur();

        void initSSAO(int fbo);

        void resize(int fbo);

        std::function<void()> drawFn;
        std::shared_ptr<ge::gl::Context> gl;
        std::shared_ptr<ge::gl::Texture> noiseTexture;
        std::shared_ptr<ge::gl::Texture> ssaoColorTexture;
        std::shared_ptr<ge::gl::Texture> blurColorTexture;
        std::shared_ptr<ge::gl::Framebuffer> ssaoFBO;

        std::vector<glm::vec3> ssaoKernel;
        std::shared_ptr<ge::gl::Shader> ssaoFS;
        std::shared_ptr<ge::gl::Shader> ssaoVS;
        std::shared_ptr<ge::gl::Program> ssaoProgram;

        std::shared_ptr<ge::gl::Framebuffer> blurFBO;
        std::shared_ptr<ge::gl::Shader> blurFS;
        std::shared_ptr<ge::gl::Shader> blurVS;
        std::shared_ptr<ge::gl::Program> blurProgram;


        int height;
        int width;

    };
}




