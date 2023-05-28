#include "cloudsvisualiser.h"

cloudsVisualiser::cloudsVisualiser(QWidget *parent)
{
    dist = 5;
    angleX = M_PI / 3;
    angleY = M_PI / 4;

    zeroX = 0;
    zeroY = 0;
    zeroZ = 0;

    isLeft = false;
    isRight = false;
}

void cloudsVisualiser::updateCamPos()
{
    if(angleX > M_PI) angleX -= 2 * M_PI;
    else if(angleX < -M_PI) angleX += 2 * M_PI;
    if(angleY > M_PI) angleY -= 2 * M_PI;
    else if(angleY < -M_PI) angleY += 2 * M_PI;

    posX = dist * sin(angleX) * cos(angleY) + zeroX;
    posY = dist * sin(angleX) * sin(angleY) + zeroY;
    posZ = dist * cos(angleX) + zeroZ;

    dirX = -posX + zeroX;
    dirY = -posY + zeroY;
    dirZ = -posZ + zeroZ;
}

void cloudsVisualiser::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}

void cloudsVisualiser::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(85.0f, (GLfloat)w/(GLfloat)h, 0.01f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
}

void paintCoordSys(float zX, float zY, float zZ)
{
    glLineWidth(4);


    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(zX, zY, zZ);
    glVertex3f(zX + 1, zY, zZ);
    glEnd();


    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(zX, zY, zZ);
    glVertex3f(zX, zY + 1, zZ);
    glEnd();


    glBegin(GL_LINE_STRIP);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(zX, zY, zZ);
    glVertex3f(zX, zY, zZ + 1);
    glEnd();
}

void paintFloorCells(float leftSide, float downSide)
{
    for(int x = leftSide; x <= leftSide + 100; x++){
        for(int y = downSide; y <= downSide + 100; y++){
            glBegin(GL_LINE_LOOP);
            glColor3f(0.5f, 0.5f, 0.5f);
            glVertex3f(x, y, 0.0f);
            glVertex3f(x + 1.0f, y, 0.0f);
            glVertex3f(x + 1.0f, y + 1.0f, 0.0f);
            glVertex3f(x, y + 1.0f, 0.0f);
            glEnd();
        }
    }
}

void cloudsVisualiser::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glLoadIdentity();

    cloudsVisualiser::updateCamPos();

    gluLookAt(posX, posY, posZ, posX + dirX, posY + dirY, posZ + dirZ, 0, 0, 1);

    paintCoordSys(zeroX, zeroY, zeroZ);
    paintFloorCells(-50 + int(zeroX), -50 + int(zeroY));
}


void cloudsVisualiser::mousePressEvent(QMouseEvent *event)
{
    mPrevPos = event->pos(); //change mouse previous position
    if(event->button() == Qt::LeftButton) isLeft = true;
    if(event->button() == Qt::RightButton) isRight = true;
    update();
}

void cloudsVisualiser::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) isLeft = false;
    if(event->button() == Qt::RightButton) isRight = false;
    update();
}

void cloudsVisualiser::mouseMoveEvent(QMouseEvent *event)
{
    if(isLeft){
        angleY -= (event->pos().x() - mPrevPos.x()) * MOUSE_SENS;
        angleX -= (event->pos().y() - mPrevPos.y()) * MOUSE_SENS;
    }
    if(isRight){
        float leftToRight = -(event->pos().x() - mPrevPos.x()) * MOUSE_SENS * dist;
        float upToDown = -(event->pos().y() - mPrevPos.y()) * MOUSE_SENS * dist;

        zeroX += upToDown * cos(angleY) + leftToRight * sin(-angleY);
        zeroY += upToDown * sin(angleY) + leftToRight * cos(-angleY);
    }
    mPrevPos = event->pos();

    update();
}


void cloudsVisualiser::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W) zeroZ += kSens; //up
    if(event->key() == Qt::Key_S) zeroZ -= kSens; //down
    kSens += 0.05; //increase move speed
    update();
}

void cloudsVisualiser::keyReleaseEvent(QKeyEvent *event)
{
    kSens = 0.05;
}

void cloudsVisualiser::wheelEvent(QWheelEvent *event)
{
    float numSteps = event->angleDelta().y(); //how many steps of moving
    dist -= numSteps * 0.005; //move relative zero position

    update();
}
