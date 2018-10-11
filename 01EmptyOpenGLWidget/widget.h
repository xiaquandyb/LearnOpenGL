#ifndef WIDGET_H
#define WIDGET_H

#include <GL/glew.h>
#include <QOpenGLWidget>

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
};

#endif // WIDGET_H
