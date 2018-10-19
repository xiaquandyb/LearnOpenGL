#ifndef WIDGET_H
#define WIDGET_H

#include <GL/glew.h>
#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Object_IDs{oTriangle, oRectangle, oNum};
enum Texture_IDs{tContainer, tAwesomeface, tNum};
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
    GLuint m_texs[tNum];
    GLuint m_program[oNum];
    glm::mat4 m_transform;
    int m_angle;
};

#endif // WIDGET_H
