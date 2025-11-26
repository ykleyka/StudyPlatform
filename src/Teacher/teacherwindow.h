#ifndef TEACHERWINDOW_H
#define TEACHERWINDOW_H

#include "../User/teacher.h"
#include "statswindow.h"
#include "../Lesson/testwindow.h"
#include "../filemanager.h"
#include <QFileDialog>
#include <QDialog>
#include <QMessageBox>

namespace Ui {
class TeacherWindow;
}

class TeacherWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TeacherWindow(FileManager* fm, Teacher* user, QWidget *parent = NULL);
    ~TeacherWindow();

private slots:

    void on_importButton_clicked();
    void on_deleteButton_clicked();
    void on_statsButton_clicked();
    void on_previewButton_clicked();
    void on_refreshButton_clicked();
    void on_closeButton_clicked();

private:
    Ui::TeacherWindow *ui;
    Teacher* user;
    FileManager* fm;

    void refreshTable();
    void populateTestsTable();
    int getSelectedTestId() const;
    void updateInfoLabel();
    void applySearchFilter(const QString& searchText);
};

#endif // TEACHERWINDOW_H
