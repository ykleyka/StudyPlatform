#include "authorizationwindow.h"
#include "ui_authorizationwindow.h"
#include "../Register/registrationwindow.h"
#include "../User/user.h"


AuthorizationWindow::AuthorizationWindow(FileManager* fm, QWidget *parent)
    : fm(fm), QWidget(parent)
    , ui(new Ui::AuthorizationWindow)
{
    ui->setupUi(this);
    ui->passLine->setEchoMode(QLineEdit::Password);
    ui->passLine->setText(" ");
    ui->passLine->setText("");
}

AuthorizationWindow::~AuthorizationWindow()
{
    delete ui;
}

void AuthorizationWindow::on_nextButton_clicked()
{
    if (!ui->loginLine->text().isEmpty() && !ui->passLine->text().isEmpty()) {
        User in(ui->loginLine->text(), ui->passLine->text());
        if (fm->checkUser(&in)) {
            this->close();
            emit loginSuccess(fm->loadUser(in.getLogin()));
        } else {
            ui->mistake->setText("Неверный логин или пароль");
        }
    }
}


void AuthorizationWindow::on_regButton_clicked()
{
    RegistrationWindow* reg = new RegistrationWindow(fm);
    reg->show();
    this->hide();
    QObject::connect(reg, &RegistrationWindow::regResult,
                     [&](bool res) {
                         this->show();
                         if (res) ui->mistake->setText("Регистрация прошла успешно!");
    });
}



