/*
-----------------------------------------------------------------------------
This source file is part of the SendMail program

Copyright (c) 2007 Didier Collard <didiercollard@gmail.com>

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation would be
    appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice may not be removed or altered from any source
    distribution.
-----------------------------------------------------------------------------
*/

#ifndef PREVIEWDIALOG_H
#define PREVIEWDIALOG_H

#include <QtGui/QDialog>
#include <QString>
#include <QUrl>
#include "ui_previewdialog.h"

class previewDialog : public QDialog
{
    Q_OBJECT

public:
    previewDialog(QString message, QWidget *parent = 0);
    ~previewDialog();   
private:
    Ui::previewDialogClass ui;
};

#endif // PREVIEWDIALOG_H
