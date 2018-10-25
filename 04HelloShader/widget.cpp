#include "widget.h"
#include <iostream>
#include <loadShader.h>
#include <QDir>
#include <QApplication>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent), m_greenValue(1.0), m_target(0)
{
    this->setFixedSize(800, 600);
    std::cout<<"-----------------MENU------------------------------"<<std::endl;
    std::cout<<"1. 按ESC退出"<<std::endl;
    std::cout<<"2. 按鼠标左键改变颜色强度"<<std::endl;
    std::cout<<"3. 按鼠标右键改变三角形颜色"<<std::endl;
    std::cout<<"---------------------------------------------------"<<std::endl;

    QDir::setCurrent(QApplication::applicationDirPath());
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
        std::cout << "failed to initialize GLEW" << std::endl;
        return;
    }

    /* 顶点数据 */
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };

    float vertices2[] = {
        // 位置              // 颜色
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };

    /* 三角形数据 */
    glGenVertexArrays(oNum, m_VAOs);
    glGenBuffers(oNum, m_VBOs);

    /* 绿色三角形 */
    glBindVertexArray(m_VAOs[oGreenTriangle]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[oGreenTriangle]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(vPosition);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* 彩色三角形 */
    glBindVertexArray(m_VAOs[oColorTriangle]);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[oColorTriangle]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(vColor);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /* 着色器 */
    ShaderInfo shaders1[] = {
        {GL_VERTEX_SHADER, "../shader/shader1.vert"},
        {GL_FRAGMENT_SHADER, "../shader/shader1.frag"},
        {GL_NONE, NULL}
    };
    ShaderInfo shaders2[] = {
        {GL_VERTEX_SHADER, "../shader/shader2.vert"},
        {GL_FRAGMENT_SHADER, "../shader/shader2.frag"},
        {GL_NONE, NULL}
    };

    m_program[oGreenTriangle] = loadShaders(shaders1);
    m_program[oColorTriangle] = loadShaders(shaders2);

    glUseProgram(m_program[oGreenTriangle]);
    glUniform4f(glGetUniformLocation(m_program[oGreenTriangle], "uColor"),
            0.0f, m_greenValue, 0.0f, 1.0f);
    glUseProgram(0);
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    /* 绘制三角形 */
    if(m_target == oGreenTriangle)
    {
        glUseProgram(m_program[oGreenTriangle]);
        glBindVertexArray(m_VAOs[oGreenTriangle]);
    }
    else
    {
        glUseProgram(m_program[oColorTriangle]);
        glBindVertexArray(m_VAOs[oColorTriangle]);
    }
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton){
        m_greenValue -= 0.1f;
        if(m_greenValue < 0.0f){
            m_greenValue = 1.0f;
        }
        glUseProgram(m_program[oGreenTriangle]);
        glUniform4f(glGetUniformLocation(m_program[oGreenTriangle], "uColor"),
                0.0f, m_greenValue, 0.0f, 1.0f);
        glUseProgram(0);
    }else if(e->buttons() == Qt::RightButton){
        if(m_target == oGreenTriangle)
        {
            m_target = oColorTriangle;
        }
        else
        {
            m_target = oGreenTriangle;
        }
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
