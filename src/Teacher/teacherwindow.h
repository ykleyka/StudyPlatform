#ifndef TEACHERWINDOW_H
#define TEACHERWINDOW_H
#include "../User/teacher.h"
#include "../filemanager.h"
#include <QDialog>

namespace Ui {
class TeacherWindow;
}

class TeacherWindow : public QDialog
{
    Q_OBJECT
public:
    explicit TeacherWindow(FileManager* fm, Teacher* user, QWidget *parent = nullptr);
    ~TeacherWindow();

private:
    Ui::TeacherWindow *ui;
    Teacher* user;
    FileManager* fm;
};

#endif // TEACHERWINDOW_H
