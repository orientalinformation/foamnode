#ifndef W_MESHBOUNDREFINEGENERAL_H
#define W_MESHBOUNDREFINEGENERAL_H

#include <QDialog>
#include "Core_Fuctions/dmesh.h"
#include <QString>

namespace Ui {
    class W_MeshBoundRefineGeneral;
}

class W_MeshBoundRefineGeneral : public QDialog
{
    Q_OBJECT

public:
    explicit W_MeshBoundRefineGeneral(DMesh *dmesh, QWidget *parent = 0);
    ~W_MeshBoundRefineGeneral();

private:
    Ui::W_MeshBoundRefineGeneral *ui;

    DMesh *mesh;
private slots:
    void on_buttonBox_accepted();
};

#endif // W_MeshBoundRefineGeneral_H
