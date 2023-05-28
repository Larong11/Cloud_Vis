#ifndef CLOUDSVISUALISER_H
#define CLOUDSVISUALISER_H

#include <QtOpenGL>
#include <OpenGL/glu.h>
#include <glm.h>
#include <iostream>
#include <QOpenGLWidget>
#include <pointcloud.h>

template <typename T>
struct insertedClouds
{
    std::vector<std::pair<pointCloud<T>, bool>> clouds;
    std::vector<QString> names;
};

struct pointCloudData
{
    GLfloat *vertexData;
    GLubyte *colorData;
    unsigned int size;
    pointCloudData(){size = 0;}
};

class cloudsVisualiser : public QOpenGLWidget
{
    Q_OBJECT
public:
    cloudsVisualiser(QWidget *parent = 0);

//    void addClouds(insertedClouds<point> *cloudsX, insertedClouds<pointI> *cloudsI, insertedClouds<pointRGB> *cloudsR);

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

    insertedClouds<point> *cloudsXYZ;
    insertedClouds<pointI> *cloudsXYZI;
    insertedClouds<pointRGB> *cloudsXYZRGB;

    std::vector<pointCloudData> dataXYZ, dataXYZI, dataXYZRGB;
};

#endif // CLOUDSVISUALISER_H
