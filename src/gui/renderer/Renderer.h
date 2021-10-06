//
// Created by tomas on 06.10.21.
//

#pragma once


#include <QExposeEvent>
#include <geGL/OpenGLContext.h>
#include <QWidget>
#include <QSurfaceFormat>
#include <QWindow>
#include <QOpenGLContext>

class Renderer : public QWindow {
Q_OBJECT
public:
    explicit Renderer(QWindow *parent = nullptr);

    virtual void render();
    virtual void initialize();

public slots:

    void renderNow();

protected:
    bool event(QEvent *event) override;
    void exposeEvent(QExposeEvent *event);

    bool initialized;

    QOpenGLContext *context;

    std::shared_ptr<ge::gl::Context> gl;
    std::shared_ptr<ge::gl::Buffer> buffer;
    std::shared_ptr<ge::gl::Buffer> indicesBuffer;
    std::shared_ptr<ge::gl::Program> shaderProgram;
    std::shared_ptr<ge::gl::VertexArray> vao;

    QSurfaceFormat surfaceFormat;

};



