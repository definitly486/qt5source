#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QDebug>
#include <QProcess>
#include <QtWidgets>

class EncryptWidget : public QWidget {
public:
    EncryptWidget(QWidget *parent = nullptr);
private slots:
    void selectFile();
    void encryptFile();
private:
    QLineEdit* filePathEdit;
    QLineEdit* passwordEdit;
};

EncryptWidget::EncryptWidget(QWidget *parent)
    : QWidget(parent)
{
    // Виджет выбора файла
    QLabel* labelFilePath = new QLabel("Выберите файл:", this);
    filePathEdit = new QLineEdit(this);
    QPushButton* btnSelectFile = new QPushButton("Обзор", this);
    connect(btnSelectFile, &QPushButton::clicked, this, &EncryptWidget::selectFile);

    // Поле ввода пароля
    QLabel* labelPassword = new QLabel("Пароль для шифрования:", this);
    passwordEdit = new QLineEdit(this);
    passwordEdit->setEchoMode(QLineEdit::Password); // Скрываем вводимый пароль

    // Кнопка запуска процесса шифрования
    QPushButton* btnEncrypt = new QPushButton("Зашифровать", this);
    connect(btnEncrypt, &QPushButton::clicked, this, &EncryptWidget::encryptFile);

    // Расположение элементов интерфейса
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(labelFilePath);
    layout->addWidget(filePathEdit);
    layout->addWidget(btnSelectFile);
    layout->addWidget(labelPassword);
    layout->addWidget(passwordEdit);
    layout->addWidget(btnEncrypt);
}

void EncryptWidget::selectFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   tr("Открыть файл"),
                                                   "",
                                                   tr("Все файлы (*)"));
    if (!fileName.isEmpty())
        filePathEdit->setText(fileName);
}

void EncryptWidget::encryptFile()
{
    QString filePath = filePathEdit->text().trimmed(); // Проверяем введенный путь к файлу
    QString password = passwordEdit->text().trimmed(); // Пароль для шифрования

    if (filePath.isEmpty() || password.isEmpty()) {
        qWarning() << "Необходимо выбрать файл и ввести пароль!";
        return;
    }

    QProcess process;
    QStringList arguments;
    arguments   << "-c"  	
		<< "--batch"                // Без интерактивного режима
		<< "--passphrase=" + password  // Прямая передача пароля через аргумент
                << filePath;                     // Файл для шифрования

    process.start("gpg", arguments);           // Запускаем команду gpg

    if (!process.waitForFinished(-1)) {        // Ждём завершения команды
        qWarning() << "Ошибка выполнения команды:" << process.errorString();
        return;
    }

    qInfo() << "Файл успешно зашифрован.";
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    EncryptWidget widget;
    widget.resize(400, 200);
    int screenWidth = QApplication::desktop()->screenGeometry().width();
    int screenHeight = QApplication::desktop()->screenGeometry().height();
    int windowWidth = widget.width();
    int windowHeight = widget.height();
    int x = (screenWidth - windowWidth) / 2;
    int y = (screenHeight - windowHeight) / 2;
    widget.move(x, y);
    widget.show();
    widget.setWindowIcon(QIcon("gpg.png")); 
    return app.exec();
}