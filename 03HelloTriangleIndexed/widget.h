﻿#ifndef WIDGET_H
#define WIDGET_H

#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>

enum Object_IDs{oTriangle, oRectangle, oNum};
enum Attrib_IDs{vPosition, vColor, vTexture};

class Widget : public QOpenGLWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);

private:
    GLuint m_VAOs[oNum];
    GLuint m_VBOs[oNum];
    GLuint m_EBOs[oNum];
    GLuint m_program[oNum];

    bool m_polygonMode;
};

#endif // WIDGET_H
