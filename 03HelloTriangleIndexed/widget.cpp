#include "widget.h"
#include <iostream>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    this->setFixedSize(800, 600);
    std::cout<<"-----------------MENU------------------------------"<<std::endl;
    std::cout<<"1. 按ESC退出"<<std::endl;
    std::cout<<"2. 按鼠标左键..."<<std::endl;
    std::cout<<"3. 按鼠标右键..."<<std::endl;
    std::cout<<"---------------------------------------------------"<<std::endl;
}

Widget::~Widget()
{
}

void Widget::initializeGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK){
        std::cout << "failed to initialize GLEW" << std::endl;
        return;
    }
}

void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() == Qt::LeftButton){

    }else if(e->buttons() == Qt::RightButton){

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
