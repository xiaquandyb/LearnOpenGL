#include "widget.h"
#include <iostream>

Widget::Widget(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

Widget::~Widget()
{

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
}


void Widget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}


void Widget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
}
