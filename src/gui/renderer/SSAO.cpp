//
// Created by tomas on 05.04.22.
//

#include <renderer/SSAO.h>
#include <functional>
#include <StaticFunctions.h>

#include <random>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders_gui/Shaders.h"

namespace MapGenerator{
    SSAO::SSAO(const std::shared_ptr<ge::gl::Context>& gl, int width, int height ,const std::function<void()>& drawFn) {
        this->gl = gl;
        this->width = width;
        this->height = height;
        this->drawFn = drawFn;
        this->initSSAO();
        this->initBlur();
    }

    void SSAO::initSSAO(){
        std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
        std::default_random_engine generator;
        for (unsigned int i = 0; i < 64; ++i) {
            glm::vec3 sample(
                    randomFloats(generator) * 2.0 - 1.0,
                    randomFloats(generator)  * 2.0 - 1.0,
                    randomFloats(generator)

            );
            sample = glm::normalize(sample);
            sample *= randomFloats(generator);
            float scale = (float) i / 64.0;
            scale = lerp(0.1f, 1.0f, scale * scale); // scale samples s.t. they're more aligned to center of kernel
            sample *= scale;
            ssaoKernel.push_back(sample);
        }
        //Prepare the noise and its texture
        std::vector<glm::vec3> ssaoNoise;
        for (unsigned int i = 0; i < 16; i++) {
            glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
            ssaoNoise.push_back(noise);
        }
        this->noiseTexture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RGBA16F, 0, 4, 4);
        this->noiseTexture->setData2D(ssaoNoise.data(), GL_RGB, GL_FLOAT, 0, GL_TEXTURE_2D, 0, 0, 4, 4);
        this->noiseTexture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        this->noiseTexture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        this->noiseTexture->texParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
        this->noiseTexture->texParameteri(GL_TEXTURE_WRAP_T, GL_REPEAT);

        this->ssaoFBO = std::make_shared<ge::gl::Framebuffer>();
        this->ssaoFBO->bind();
        this->ssaoColorTexture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RED, 0, width, height);
        this->ssaoColorTexture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        this->ssaoColorTexture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->ssaoColorTexture->getId(), 0);

        this->ssaoFS = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, GUIShaders::getSSAOFS());
        this->ssaoVS = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, GUIShaders::getSSAOVS());
        this->ssaoProgram = std::make_shared<ge::gl::Program>(ssaoVS, ssaoFS);
        //rebind default frame buffer to avoid errors
        gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    void SSAO::initBlur(){
        this->blurFBO = std::make_shared<ge::gl::Framebuffer>();
        gl->glBindFramebuffer(GL_FRAMEBUFFER, this->blurFBO->getId());
        this->blurColorTexture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RED, 0, width, height);
        this->blurColorTexture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        this->blurColorTexture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->blurColorTexture->getId(), 0);

        this->blurFS = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER, GUIShaders::getSSAOBlurFS());
        this->blurVS = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER, GUIShaders::getSSAOBlurVS());
        this->blurProgram = std::make_shared<ge::gl::Program>(blurVS, blurFS);
    }

    void SSAO::render(const std::shared_ptr<ge::gl::Texture> &depthTexture, const std::shared_ptr<ge::gl::Texture> &normalTexture) {
        ssaoFBO->bind(GL_FRAMEBUFFER);
        gl->glClear(GL_COLOR_BUFFER_BIT);
        ssaoProgram->use();
        //ssaoProgram->use();
        depthTexture->bind(0);
        normalTexture->bind(1);
        noiseTexture->bind(2);
        auto projection = glm::perspective(glm::radians(60.0f), (float) width / (float) height, 0.005f, 100.0f);
        if (ssaoProgram->getUniformLocation("projection") != -1)
            ssaoProgram->setMatrix4fv("projection", glm::value_ptr(projection));
        if (ssaoProgram->getUniformLocation("noiseScale") != -1)
            ssaoProgram->set2f("noiseScale", width / 4.0, height / 4.0);
        if (ssaoProgram->getUniformLocation("samples") != -1)
            ssaoProgram->set3fv("samples", glm::value_ptr(ssaoKernel[0]), 64);

        drawFn();

        gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void SSAO::renderBlur() {
        gl->glBindFramebuffer(GL_FRAMEBUFFER, this->blurFBO->getId());
        gl->glClear(GL_COLOR_BUFFER_BIT);
        blurProgram->use();
        this->ssaoColorTexture->bind(0);
        drawFn();
        gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void SSAO::resize(){
        //Scale the textures to current width and height
        this->ssaoFBO->bind(GL_FRAMEBUFFER);
        this->ssaoColorTexture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RED, 0, width, height);
        this->ssaoColorTexture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        this->ssaoColorTexture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->ssaoColorTexture->getId(), 0);

        this->blurFBO->bind(GL_FRAMEBUFFER);
        this->blurColorTexture = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D, GL_RED, 0, width, height);
        this->blurColorTexture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        this->blurColorTexture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        gl->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->blurColorTexture->getId(), 0);

        gl->glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    void SSAO::setDimensions(int width, int height) {
        this->width = width;
        this->height = height;
        this->resize();
    }

    void SSAO::bindWithoutBlur(int unit) {
        ssaoColorTexture->bind(unit);
    }
    void SSAO::bind(int unit){
        blurColorTexture->bind(unit);
    }


}


