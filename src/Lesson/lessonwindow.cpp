#include "lessonwindow.h"
#include "ui_lessonwindow.h"

LessonWindow::LessonWindow(FileManager* fm, Student* user, QWidget *parent)
    : fm(fm), QDialog(parent), ui(new Ui::LessonWindow), user(user)
{
    ui->setupUi(this);
}

LessonWindow::~LessonWindow()
{
    delete ui;
}
