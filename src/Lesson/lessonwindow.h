#ifndef LESSONWINDOW_H
#define LESSONWINDOW_H

#include "../User/student.h"
#include "../filemanager.h"
#include "testwindow.h"
#include <QDialog>

namespace Ui {
class LessonWindow;
}

class LessonWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LessonWindow(FileManager* fm, Student* user, QWidget *parent = NULL);
    ~LessonWindow();

private slots:
    void on_startTestButton_clicked();
    void on_refreshButton_clicked();
    void on_searchButton_clicked();
    void on_closeButton_clicked();
    void on_testsList_itemSelectionChanged();

private:
    Ui::LessonWindow *ui;
    Student* user;
    FileManager* fm;

    void loadAvailableTests();
    void updateStudentInfo();
    void updateInfoLabel();
    double getStudentBestScore(int testId);
    QString formatTestItemText(TestInfo* test, double bestScore, bool isCompleted);
};

#endif // LESSONWINDOW_H
