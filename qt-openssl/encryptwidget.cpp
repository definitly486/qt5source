#include "encryptwidget.h"
#include "ui_encryptwidget.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QProcess>

EncryptWidget::EncryptWidget(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::EncryptWidget)
{
    ui->setupUi(this);  // Подготавливаем UI-элементы

    // Подключаем сигналы кнопок
    connect(ui->btnSelectFile, &QPushButton::clicked, this, &EncryptWidget::selectFile);
    connect(ui->pushButton_enc, &QPushButton::clicked, this, &EncryptWidget::encryptFile);
    connect(ui->pushButton_dec_2, &QPushButton::clicked, this, &EncryptWidget::decryptFile);
}

EncryptWidget::~EncryptWidget()
{
    delete ui;
}

// Выбор файла
void EncryptWidget::selectFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   tr("Открыть файл"),
                                                   "",
                                                   tr("Все файлы (*.*)"));
    if (!fileName.isEmpty())
        ui->lineEdit->setText(fileName);  // Обращаемся через ui
}

// Шифрование файла
void EncryptWidget::encryptFile()
{
    QString filePath = ui->lineEdit->text().trimmed();  // Получаем путь к файлу через ui
    QString password = ui->passwordEdit->text().trimmed();  // Получаем пароль через ui

    if (filePath.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите файл и введите пароль!");
        return;
    }

    // Временный файл для хранения пароля
    QTemporaryFile tempPassFile;
    if (!tempPassFile.open()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать временный файл для пароля.");
        return;
    }

    tempPassFile.write(password.toUtf8());
    tempPassFile.flush();

    // Настраиваем выходной файл
    QString outputFile = filePath + ".enc";

    // Параметры для openssl
    QStringList arguments;
    arguments << "enc"
              << "-aes-256-cbc"                 // Алгоритм шифрования
              << "-pbkdf2"                       // Использование соли
              << "-iter"                         // Количество итераций хэширования
              << "100000"                        // Значение количества итераций
              << "-e"                            // Шифрование
              << "-in" << filePath               // Входной файл
              << "-out" << outputFile          // Выходной файл
              << "-pass"   
              << "pass:" + password;  // Прямая передача пароля через аргумент

    QProcess process;
    process.setStandardInputFile(tempPassFile.fileName()); // Передаем файл с паролем
    process.start("openssl", arguments);                   // Запускаем процесс

    if (!process.waitForFinished(-1)) {
        QMessageBox::critical(this, "Ошибка", "Ошибка шифрования файла:\n" + process.readAllStandardError());
        return;
    }

    tempPassFile.remove();                                // Удаляем временный файл с паролем

    QMessageBox::information(this, "Готово", "Файл успешно зашифрован.\nЗашифрованный файл сохранён как " + outputFile);
}

// Расшифровка файла
void EncryptWidget::decryptFile()
{
    QString filePath = ui->lineEdit->text().trimmed();  // Получаем путь к файлу через ui
    QString password = ui->passwordEdit->text().trimmed();  // Получаем пароль через ui

    if (filePath.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите файл и введите пароль!");
        return;
    }

    // Временный файл для хранения пароля
    QTemporaryFile tempPassFile;
    if (!tempPassFile.open()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось создать временный файл для пароля.");
        return;
    }

    tempPassFile.write(password.toUtf8());
    tempPassFile.flush();

    // Определим имя выходного файла (убираем расширение .enc)
    QString decryptedFile = filePath.left(filePath.length() - 4);

    // Параметры для openssl
    QStringList arguments;
    arguments << "enc"                           // Операция расшифровки
              << "-aes-256-cbc"                 // Алгоритм шифрования
              << "-pbkdf2"                       // Использование соли
              << "-iter"                         // Количество итераций хэширования
              << "100000"                        // Значение количества итераций
              << "-e"                            // Шифрование                       // Значение количества итераций
              << "-d"                            // Расшифровка
              << "-in" << filePath               // Входной файл
              << "-out" << decryptedFile        // Выходной файл
           << "-pass"   
              << "pass:" + password;  // Прямая передача пароля через аргумент

    QProcess process;
    process.setStandardInputFile(tempPassFile.fileName()); // Передаем файл с паролем
    process.start("openssl", arguments);                   // Запускаем процесс

    if (!process.waitForFinished(-1)) {
        QMessageBox::critical(this, "Ошибка", "Ошибка расшифровки файла:\n" + process.readAllStandardError());
        return;
    }

    tempPassFile.remove();                                // Удаляем временный файл с паролем

    QMessageBox::information(this, "Готово", "Файл успешно расшифрован.\nРасшифрованный файл сохранён как " + decryptedFile);
}

