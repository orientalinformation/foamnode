#ifndef DMESH_H
#define DMESH_H

#include <QGLWidget>
#include <QtGui>
#include <QFile>

#include <vector>
#include "math.h"
#include "time.h"
#include "GL/glu.h"
#include "Core_Fuctions/snappy_dmesh.h"
#include "Core_Fuctions/block_dmesh.h"
#include "Core_Fuctions/patchdict.h"

class DMesh : public QGLWidget
{
    Q_OBJECT
public:
    explicit DMesh(QGLWidget *parent = 0);
    Block_Dmesh *blockd;
    Snappy_Dmesh * snappyd;    
    PatchDict *patchDict;
    void SetHomeView();
    void SetRotate(float x, float y, float z);
    void SetDrawCoordinate(bool note,bool center);
    void SetViewList(QStringList views);
    QStringList GetViewList();


private:
    QVector<float> radiusCaculationMax;
    QVector<float> radiusCaculationMin;
    int screenWidth,screenHeight;
    int radian_Rotate;

    int zoomScale;
    int f; //var for adjust zoomscale
    int zOut;
    int zIn;
    bool flagFirstStart;

    QPoint lastPos;

    QStringList viewList;
    int lastPosZ;

    float vMax;  //var of OpenGL to pust the object fix to screen.

    float xRot;
    float yRot;
    float zRot;

    float xMove;
    float yMove;
    float zMove;


    float xCenter;
    float yCenter;
    float zCenter;

    float scalePoint;
    float xTranslate;
    float yTranslate;
    float zTranslate;

    int viewType;
    bool is_DrawCenterCoordinate;
    bool is_DrawNoteCoordinate;

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void FindVMax();
    void DrawCenterCoordinate();
    void DrawNoteCoordinate();
    void DrawLocationPoint();
    void ShowHeighWidthGreen();

    void DrawEdges();
    void DrawFaces();
    void DrawPoint();
    void DrawCells();

    void DrawSTL();
    void DrawGeometry();
    void DrawBounding();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);

    float minValue(QVector<float> values);
    float maxValue(QVector<float> values);
signals:

public slots:

};

#endif // DMESH_H
