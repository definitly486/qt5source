#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_selectFile_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_passwd_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    void decryptFile(const QString &inputFile, const QString &outputFile, const QString &password);
    void showMessage(const QString &title, const QString &message);
    void showWarning(const QString &message);
};

#endif // MAINWINDOW_H
