#include "widget.h"
#include <iostream>
#include <loadShader.h>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    this->setFixedSize(800, 600);
}

Widget::~Widget()
{
    glDeleteVertexArrays(oNum, m_VAOs);
    glDeleteBuffers(oNum, m_VBOs);
}

void Widget::initializeGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "initialize GLEW : NO" << std::endl;
        return;
    }else{
        std::cout << "initialize GLEW : OK" << std::endl;
    }

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };

    /* 三角形数据 */
    glGenVertexArrays(oNum, m_VAOs);
    glGenBuffers(oNum, m_VBOs);

    glBindVertexArray(m_VAOs[oTriangle]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[oTriangle]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* 着色器 */
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "Resource/shader/simple.vert"},
        {GL_FRAGMENT_SHADER, "Resource/shader/simple.frag"},
        {GL_NONE, NULL}
    };

    m_program[oTriangle] = loadShaders(shaders);
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    glUseProgram(m_program[oTriangle]);
    glBindVertexArray(m_VAOs[oTriangle]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
