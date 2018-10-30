#include "widget.h"
#include <iostream>
#include <loadShader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <QDir>
#include <QApplication>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_lastPos(400.0f, 300.0f),
      m_glCamera_FPS(glm::vec3(0.0f, 0.0f, 3.0f))
{
    this->setFixedSize(800, 600);

    std::cout<<"-----------------MENU------------------------------"<<std::endl;
    std::cout<<"1. 按ESC退出"<<std::endl;
    std::cout<<"2. 按鼠标左键改变光源颜色"<<std::endl;
    std::cout<<"3. 按鼠标右键移动鼠标改变方向"<<std::endl;
    std::cout<<"4. 按WSAD移动摄像机"<<std::endl;
    std::cout<<"5. 按鼠标滚轮进行缩放"<<std::endl;
    std::cout<<"---------------------------------------------------"<<std::endl;

    QDir::setCurrent(QApplication::applicationDirPath());

    this->setMouseTracking(true);

    m_glCamera_FPS.MovementSpeed = 0.1f;
}

Widget::~Widget()
{
    glDeleteVertexArrays(oNum, m_VAOs);
    glDeleteBuffers(oNum, m_VBOs);
}

void Widget::initializeGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "failed to initialize GLEW" << std::endl;
        return;
    }

    /* 顶点数据 */
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    /* 生成各对象 */
    glGenVertexArrays(oNum, m_VAOs);
    glGenBuffers(oNum, m_VBOs);

    /* ----------顶点属性----------begin */
    glBindVertexArray(m_VAOs[oLight]);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[oLight]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(vPosition);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(m_VAOs[oCube]);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[oCube]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(vNormal);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    /* ----------顶点属性----------end */

    /* 着色器 */
    ShaderInfo shaders1[] = {
        {GL_VERTEX_SHADER, "../shader/shader7.vert"},
        {GL_FRAGMENT_SHADER, "../shader/shader7.frag"},
        {GL_NONE, NULL}
    };
    m_program[pObj1] = loadShaders(shaders1);
    ShaderInfo shaders2[] = {
        {GL_VERTEX_SHADER, "../shader/light.vert"},
        {GL_FRAGMENT_SHADER, "../shader/light.frag"},
        {GL_NONE, NULL}
    };
    m_program[pLight] = loadShaders(shaders2);

    glUseProgram(m_program[pObj1]);
    glUniform3f(glGetUniformLocation(m_program[pObj1], "objectColor"), 1.0f, 0.5f, 0.31f);
    glUniform3f(glGetUniformLocation(m_program[pObj1], "lightColor"), 1.0f, 1.0f, 1.0f);
//    glUniform3f(glGetUniformLocation(m_program[pObj1], "lightColor"), 0.0f, 1.0f, 0.0f);
//    glUniform3f(glGetUniformLocation(m_program[pObj1], "lightColor"), 0.33f, 0.42f, 0.18f);

    glUseProgram(m_program[pLight]);
    glUniform3f(glGetUniformLocation(m_program[pLight], "lightColor"), 1.0f, 1.0f, 1.0f);
//    glUniform3f(glGetUniformLocation(m_program[pLight], "lightColor"), 0.0f, 1.0f, 0.0f);
//    glUniform3f(glGetUniformLocation(m_program[pLight], "lightColor"), 0.33f, 0.42f, 0.18f);
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.2f, 1.0f, 2.0f)
    };

    glm::mat4 projection = glm::perspective(glm::radians(m_glCamera_FPS.Zoom), 800.0f/600.0f, 0.1f, 100.0f);
    glm::mat4 view = m_glCamera_FPS.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    /* 绘制光源 */
    glUseProgram(m_program[pLight]);
    glBindVertexArray(m_VAOs[oLight]);
    glUniformMatrix4fv(glGetUniformLocation(m_program[pLight], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(m_program[pLight], "view"), 1, GL_FALSE, glm::value_ptr(view));

    model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[1]);
    model = glm::scale(model, glm::vec3(0.2f));
    glUniformMatrix4fv(glGetUniformLocation(m_program[pLight], "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glUseProgram(0);

    /* 绘制物体 */
    glUseProgram(m_program[pObj1]);
    glBindVertexArray(m_VAOs[oCube]);
    glUniformMatrix4fv(glGetUniformLocation(m_program[pObj1], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(m_program[pObj1], "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(glGetUniformLocation(m_program[pObj1], "lightPos"), 1, glm::value_ptr(cubePositions[1]));
    glUniform3fv(glGetUniformLocation(m_program[pObj1], "viewPos"), 1, glm::value_ptr(m_glCamera_FPS.Position));

    model = glm::mat4(1.0f);
    model = glm::translate(model, cubePositions[0]);
    glUniformMatrix4fv(glGetUniformLocation(m_program[pObj1], "model"), 1, GL_FALSE, glm::value_ptr(model));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glUseProgram(0);
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        static int index = 0;
        index = ++index%3;
        switch(index)
        {
        case 0:
            glUseProgram(m_program[pObj1]);
            glUniform3f(glGetUniformLocation(m_program[pObj1], "lightColor"), 1.0f, 1.0f, 1.0f);
            glUseProgram(m_program[pLight]);
            glUniform3f(glGetUniformLocation(m_program[pLight], "lightColor"), 1.0f, 1.0f, 1.0f);
            break;
        case 1:
            glUseProgram(m_program[pObj1]);
            glUniform3f(glGetUniformLocation(m_program[pObj1], "lightColor"), 0.0f, 1.0f, 0.0f);
            glUseProgram(m_program[pLight]);
            glUniform3f(glGetUniformLocation(m_program[pLight], "lightColor"), 0.0f, 1.0f, 0.0f);
            break;
        case 2:
            glUseProgram(m_program[pObj1]);
            glUniform3f(glGetUniformLocation(m_program[pObj1], "lightColor"), 0.33f, 0.42f, 0.18f);
            glUseProgram(m_program[pLight]);
            glUniform3f(glGetUniformLocation(m_program[pLight], "lightColor"), 0.33f, 0.42f, 0.18f);
            break;
        default:
            break;
        }
    }else if(e->button() == Qt::RightButton){
        m_lastPos = e->pos();
    }
    this->update();
}

void Widget::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()){
        case Qt::Key_Escape:
            this->close();
            break;
        case Qt::Key_W:
            m_glCamera_FPS.ProcessKeyboard(FORWARD, 1);
            break;
        case Qt::Key_S:
            m_glCamera_FPS.ProcessKeyboard(BACKWARD, 1);
            break;
        case Qt::Key_A:
            m_glCamera_FPS.ProcessKeyboard(LEFT, 1);
            break;
        case Qt::Key_D:
            m_glCamera_FPS.ProcessKeyboard(RIGHT, 1);
            break;
        default: break;
    }
    this->update();
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::RightButton)
    {
        QPointF moveLen;
        moveLen = e->pos() - m_lastPos;
        if(!(moveLen.x() >= 5.0f ||
             moveLen.x() <= -5.0f ||
             moveLen.y() >= 5.0f ||
             moveLen.y() <= -5.0f))
        {
            return;
        }
        m_lastPos = e->pos();

        m_glCamera_FPS.ProcessMouseMovement(-moveLen.x(), moveLen.y());
        this->update();
    }
}

void Widget::wheelEvent(QWheelEvent *e)
{
    m_glCamera_FPS.ProcessMouseScroll(e->delta()/120.0f);
    this->update();
}

