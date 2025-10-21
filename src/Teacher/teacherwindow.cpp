#include "teacherwindow.h"
#include "ui_teacherwindow.h"

TeacherWindow::TeacherWindow(FileManager* fm, Teacher* user, QWidget *parent)
    : fm(fm), QDialog(parent), ui(new Ui::TeacherWindow), user(user)
{
    ui->setupUi(this);
}

TeacherWindow::~TeacherWindow()
{
    delete ui;
}
