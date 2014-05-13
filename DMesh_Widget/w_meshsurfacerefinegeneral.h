#ifndef W_MESHSURFACEREFINEGENERAL_H
#define W_MESHSURFACEREFINEGENERAL_H

#include <QDialog>
#include "Core_Fuctions/dmesh.h"
#include <QString>

namespace Ui {
    class W_MeshSurfaceRefineGeneral;
}

class W_MeshSurfaceRefineGeneral : public QDialog
{
    Q_OBJECT

public:
    explicit W_MeshSurfaceRefineGeneral(DMesh *dmesh, QWidget *parent = 0);
    ~W_MeshSurfaceRefineGeneral();
private:
    Ui::W_MeshSurfaceRefineGeneral *ui;
    DMesh *mesh;

private slots:
    void on_buttonBox_accepted();
};

#endif // W_MESHSURFACEREFINEGENERAL_H
