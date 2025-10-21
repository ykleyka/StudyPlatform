#include "registrationwindow.h"
#include "ui_registrationwindow.h"
#include <QRegularExpression>

RegistrationWindow::RegistrationWindow(FileManager* fm, QWidget *parent)
    : fm(fm), QDialog(parent)
    , ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);
    ui->subjLine->hide();
    ui->subjText->hide();
    ui->passLine->setEchoMode(QLineEdit::Password);
    ui->passConfirmLine->setEchoMode(QLineEdit::Password);
}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}

bool RegistrationWindow::checkName(const QString& name)
{
    // Убираем лишние пробелы в начале и конце
    QString trimmedName = name.trimmed();

    if (trimmedName.isEmpty()) {
        ui->mistake->setText("Поле имени пустое");
        return false;
    }

    // Разделяем на части, убирая множественные пробелы
    QStringList parts = trimmedName.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

    if (parts.size() != 2) {
        ui->mistake->setText("Поле имени должно иметь формат \"Имя Фамилия\"");
        return false;
    }

    QString firstName = parts[0];
    QString lastName = parts[1];

    // Регулярное выражение для русских букв, включая Ё
    QRegularExpression cyrillicRegex("^[А-ЯЁ][а-яё]*$");
    if (cyrillicRegex.match(firstName).hasMatch() &&
        cyrillicRegex.match(lastName).hasMatch()) {
        return true;
    } else {
        ui->mistake->setText("Поле имени должно содержать\nтолько русские буквы");
        return false;
    }
}

void RegistrationWindow::on_backButton_clicked()
{
    this->close();
    emit regResult(false);
}


void RegistrationWindow::on_teacherButton_clicked()
{
    ui->subjLine->show();
    ui->subjText->show();
}


void RegistrationWindow::on_studentButton_clicked()
{
    ui->subjLine->hide();
    ui->subjText->hide();
}

void RegistrationWindow::on_regButton_clicked()
{
    if (ui->loginLine->text().isEmpty()) {
        ui->mistake->setText("Поле логина пустое");
        return;
    }
    if (fm->userExists(ui->loginLine->text())) {
        ui->mistake->setText("Аккаунт уже существует");
        return;
    }
    if (ui->passLine->text().isEmpty()) {
        ui->mistake->setText("Поле пароля пустое");
        return;
    }
    if (ui->passLine->text() != ui->passConfirmLine->text()) {
        ui->mistake->setText("Пароли не совпадают");
        return;
    }
    if (!checkName(ui->nameLine->text())) {
        return;
    }
    if (ui->studentButton->isChecked()){
        Student user(ui->loginLine->text(), ui->passLine->text(), ui->nameLine->text());
        fm->saveUser(&user);
    }
    else if (ui->teacherButton->isChecked() && !ui->subjLine->text().isEmpty()) {
        Teacher user(ui->loginLine->text(), ui->passLine->text(), ui->nameLine->text(), ui->subjLine->text());
        fm->saveUser(&user);
    } else {
        ui->mistake->setText("Поле предмета пустое");
        return;
    }
    this->close();
    emit regResult(true);
}

