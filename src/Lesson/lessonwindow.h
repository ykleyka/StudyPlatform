#ifndef LESSONWINDOW_H
#define LESSONWINDOW_H
#include "../User/student.h"
#include "../filemanager.h"
#include <QDialog>

namespace Ui {
class LessonWindow;
}

class LessonWindow : public QDialog
{
    Q_OBJECT
public:
    explicit LessonWindow(FileManager* fm, Student* user, QWidget *parent = nullptr);
    ~LessonWindow();

private:
    Ui::LessonWindow *ui;
    Student* user;
    FileManager* fm;
};

#endif // LESSONWINDOW_H
