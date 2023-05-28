#ifndef CLOUDSVISUALISER_H
#define CLOUDSVISUALISER_H

#include <QtOpenGL>
#include <OpenGL/glu.h>
#include <glm.h>
#include <iostream>
#include <QOpenGLWidget>

class cloudsVisualiser : public QOpenGLWidget
{
    Q_OBJECT
public:
    cloudsVisualiser(QWidget *parent = 0);
    virtual ~cloudsVisualiser() = default;

public slots:
    void initializeGL(); //init
    void paintGL(); //paint
    void resizeGL(int w, int h); //resize

    void mousePressEvent(QMouseEvent *event); //mouse press
    void mouseMoveEvent(QMouseEvent *event); //mouse move
    void mouseReleaseEvent(QMouseEvent *event); //mouse release
    void keyPressEvent(QKeyEvent *event); //keynoard press
    void keyReleaseEvent(QKeyEvent *event); //keyboard release
    void wheelEvent(QWheelEvent *event); //mouse wheel event
private:

    void updateCamPos();

    float posX, posY, posZ;
    float dirX, dirY, dirZ;
    double angleX, angleY;
    float zeroX, zeroY, zeroZ;
    float dist;

    QPoint mPrevPos;
    bool isLeft, isRight;
    const double MOUSE_SENS = 0.005;

    float kSens = 0.5;
};

#endif // CLOUDSVISUALISER_H
