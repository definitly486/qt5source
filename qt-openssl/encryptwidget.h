#ifndef ENCRYPTWIDGET_H
#define ENCRYPTWIDGET_H

#include <QMainWindow>
#include "ui_encryptwidget.h"

namespace Ui {
class EncryptWidget;
}

class EncryptWidget : public QMainWindow
{
    Q_OBJECT

public:
    EncryptWidget(QWidget *parent = nullptr);
    ~EncryptWidget();

private slots:
    void selectFile();  // Декларируем метод
    void encryptFile();
    void decryptFile();

private:
    Ui::EncryptWidget *ui;
};

#endif // ENCRYPTWIDGET_H



