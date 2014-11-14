#ifndef W_CAPTURE_H
#define W_CAPTURE_H

#include <QDialog>
#include <QFileDialog>
#include <QtCore>
#include <Core_Fuctions/openfoam.h>

namespace Ui {
    class W_Capture;
}

class W_Capture : public QDialog
{
    Q_OBJECT

public:
    explicit W_Capture(QWidget *parent = 0);
    QString GetFormat();
    QString GetFilePath();
    ~W_Capture();
    QString saveFileName;

private:
    Ui::W_Capture *ui;
    QDateTime *GetvalueCurrentTime;

private slots:
    void on_btn_Browse_clicked();
    void on_rbn_SaveToThis_clicked(bool checked);
    void on_rbn_SaveToOther_clicked(bool checked);
    void on_txt_namePicture_editingFinished();
};

#endif // W_CAPTURE_H
