//
// Created by tomas on 02.10.21.
//

#include "Renderer.h"

#include <iostream>
#include <QOpenGLContext>
#include <shaders/Shaders.h>

Renderer::Renderer(Camera *camera, QWidget *parent) : QOpenGLWidget(parent) {

}
void Renderer::tempSetup(){ //Testing setup
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
    };
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &VertexSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &FragmentSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Renderer::initializeGL() {
    std::cout << "Init" << std::endl;
    initializeOpenGLFunctions();
    glClearColor(1.0,1.0,1.0,1.0);

    tempSetup();

    QOpenGLWidget::initializeGL();
}

void Renderer::paintGL() {
    glClearColor(0.0,0.0,0.0,0.0);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    std::cout << "Paint" << std::endl;
    QOpenGLWidget::paintGL();
}

void Renderer::resizeGL(int w, int h) {
    std::cout << "Resize" << std::endl;
    QOpenGLWidget::resizeGL(w, h);
}


