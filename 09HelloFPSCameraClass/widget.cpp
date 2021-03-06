﻿#include "widget.h"
#include <iostream>
#include <loadShader.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <QDir>
#include <QApplication>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent),
      m_angle(0),
      m_lastPos(400.0f, 300.0f),
      m_glCamera_FPS(glm::vec3(0.0f, 0.0f, 3.0f))
{
    this->setFixedSize(800, 600);

    std::cout<<"-----------------MENU------------------------------"<<std::endl;
    std::cout<<"1. 按ESC退出"<<std::endl;
    std::cout<<"2. 按鼠标左键旋转盒子"<<std::endl;
    std::cout<<"3. 按鼠标右键移动鼠标改变方向"<<std::endl;
    std::cout<<"4. 按WSAD移动摄像机"<<std::endl;
    std::cout<<"5. 鼠标滚轮进行缩放"<<std::endl;
    std::cout<<"---------------------------------------------------"<<std::endl;

    QDir::setCurrent(QApplication::applicationDirPath());

    this->setMouseTracking(true);
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
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    /* 生成各对象 */
    glGenVertexArrays(oNum, m_VAOs);
    glGenBuffers(oNum, m_VBOs);
    glGenTextures(tNum, m_texs);

    /* ----------顶点属性----------begin */
    glBindVertexArray(m_VAOs[oRectangle]);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[oRectangle]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vTexture, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(vTexture);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    /* ----------顶点属性----------end */

    /* 着色器 */
    ShaderInfo shaders[] = {
        {GL_VERTEX_SHADER, "../shader/shader5.vert"},
        {GL_FRAGMENT_SHADER, "../shader/shader5.frag"},
        {GL_NONE, NULL}
    };
    m_program[oRectangle] = loadShaders(shaders);

    /* ----------纹理----------begin */
    glBindTexture(GL_TEXTURE_2D, m_texs[tContainer]);

    /* 为当前绑定的纹理对象设置环绕、过滤方式 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* 加载并生成纹理 */
    int texW, texH, texChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("../Resources/textures/container.jpg", &texW, &texH, &texChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texW, texH, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    glBindTexture(GL_TEXTURE_2D, m_texs[tAwesomeface]);

    /* 为当前绑定的纹理对象设置环绕、过滤方式 */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* 加载并生成纹理 */
    data = stbi_load("../Resources/textures/awesomeface.png", &texW, &texH, &texChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glUseProgram(m_program[oRectangle]);
    glUniform1i(glGetUniformLocation(m_program[oRectangle], "uTexture1"), 0);
    glUniform1i(glGetUniformLocation(m_program[oRectangle], "uTexture2"), 1);
    /* ----------纹理----------end */
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    /* 绘制 */
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texs[tContainer]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texs[tAwesomeface]);
    glUseProgram(m_program[oRectangle]);
    glBindVertexArray(m_VAOs[oRectangle]);

    glm::mat4 projection = glm::perspective(glm::radians(m_glCamera_FPS.Zoom), 800.0f/600.0f, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(m_program[oRectangle], "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glm::mat4 view = m_glCamera_FPS.GetViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(m_program[oRectangle], "view"), 1, GL_FALSE, glm::value_ptr(view));

    for(int i = 0; i < 10; ++i)
    {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      model = glm::rotate(model, glm::radians((i+m_angle)*30.0f), glm::vec3(1.0f, 0.3f, 0.5f));
      glUniformMatrix4fv(glGetUniformLocation(m_program[oRectangle], "model"), 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
    glUseProgram(0);
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        ++m_angle;
        m_angle= m_angle%12;
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

