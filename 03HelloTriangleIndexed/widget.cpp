#include "widget.h"
#include <iostream>
#include <loadShader.h>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent), m_polygonMode(false)
{
    this->setFixedSize(800, 600);
    std::cout<<"-----------------MENU------------------------------"<<std::endl;
    std::cout<<"1. 按ESC退出"<<std::endl;
    std::cout<<"2. 按鼠标左/右键改变线框模式"<<std::endl;
    std::cout<<"---------------------------------------------------"<<std::endl;
}

Widget::~Widget()
{
    glDeleteVertexArrays(oNum, m_VAOs);
    glDeleteBuffers(oNum, m_VBOs);
    glDeleteBuffers(oNum, m_EBOs);
}

void Widget::initializeGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "failed to initialize GLEW" << std::endl;
        return;
    }

    /* 顶点数据 */
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // 右上
         0.5f, -0.5f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f,  // 左下
        -0.5f,  0.5f, 0.0f   // 左上
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    glGenVertexArrays(oNum, m_VAOs);
    glGenBuffers(oNum, m_VBOs);
    glGenBuffers(oNum, m_EBOs);

    glBindVertexArray(m_VAOs[oRectangle]);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[oRectangle]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOs[oRectangle]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* 着色器 */
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "../shader/simple.vert"},
        {GL_FRAGMENT_SHADER, "../shader/simple.frag"},
        {GL_NONE, NULL}
    };

    m_program[oRectangle] = loadShaders(shaders);
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if(m_polygonMode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    /* 绘制矩形 */
    glUseProgram(m_program[oRectangle]);
    glBindVertexArray(m_VAOs[oRectangle]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton){
        m_polygonMode = true;
    }else if(e->buttons() == Qt::RightButton){
        m_polygonMode = false;
    }
    this->update();
}

void Widget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()){
        case Qt::Key_Escape: this->close(); break;
        default: break;
    }
}
