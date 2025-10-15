#include "mainwindow.h"
#include "ui_guiopenssl.h" // предполагается, что ваш UI файл называется mainwindow.ui
#include <QFileDialog>
#include <QProcess>
#include <QInputDialog>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Вспомогательная функция для отображения сообщений
void MainWindow::showMessage(const QString &title, const QString &message) {
    QMessageBox::information(this, title, message);
}

// Вспомогательная функция для отображения предупреждений
void MainWindow::showWarning(const QString &message) {
    QMessageBox::warning(this, "Ошибка", message);
}

void MainWindow::on_pushButton_selectFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(
        this,
        "Выберите файл",
        "",
        "Все файлы (*.*)"
        );

    if (!filename.isEmpty()) {
        ui->labelFilePath->setText(filename);
    } else {
        ui->labelFilePath->setText("Файл не выбран");
    }
}


void MainWindow::decryptFile(const QString &inputFile, const QString &outputFile, const QString &password)
{
    QString program = "openssl";


 if (!QFile::exists(inputFile)) {
        showWarning("Входной файл не найден: " + inputFile);
        return;
    }

    QStringList arguments;
    arguments << "enc"
              << "-aes-256-cbc"
              << "-pbkdf2"
              << "-iter" << "100000"
              << "-e" //
              << "-in" << inputFile
              << "-out" << outputFile
              << "-pass" << QString("pass:%1").arg(password); // Передача пароля

    QProcess process;
    process.start(program, arguments);

    if (!process.waitForStarted()) {
        showWarning("Не удалось запустить openssl.");
        return;
    }

    if (!process.waitForFinished()) {
        showWarning("Процесс завершился с ошибкой.");
        return;
    }

    QByteArray stdoutOutput = process.readAllStandardOutput();
    QByteArray stderrOutput = process.readAllStandardError();

    // Можно вывести в консоль для отладки, или убрать
    qDebug() << "Stdout:" << stdoutOutput;
    qDebug() << "Stderr:" << stderrOutput;

    if (process.exitStatus() == QProcess::NormalExit && process.exitCode() == 0) {
        showMessage("Успех", "Файл успешно расшифрован.");
    } else {
        showWarning("Ошибка при расшифровке файла.");
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString inputFile = ui->labelFilePath->text();
    if (inputFile.isEmpty() || inputFile == "Файл не выбран") {
        showWarning("Пожалуйста, выберите файл для расшифровки.");
        return;
    }

    // Получение пароля из lineEdit_passwd
    QString password = ui->lineEdit_passwd->text();
    if (password.isEmpty()) {
        showWarning("Пароль не введен.");
        return;
    }

    QString outputFile = QFileDialog::getSaveFileName(this, "Выберите место сохранения файла", "", "Все файлы (*.*)");
    if (outputFile.isEmpty()) {
        showWarning("Место сохранения не выбрано.");
        return;
    }

    decryptFile(inputFile, outputFile, password);
}

void MainWindow::on_lineEdit_passwd_textChanged(const QString &arg1)
{
    // Можно оставить пустым или использовать для динамической проверки
}