#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H
#include "../filemanager.h"
#include <QDialog>

namespace Ui {
class RegistrationWindow;
}

class RegistrationWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegistrationWindow(FileManager* fm, QWidget *parent = NULL);
    ~RegistrationWindow();
    bool checkName(const QString& name);

private slots:
    void on_backButton_clicked();

    void on_teacherButton_clicked();

    void on_studentButton_clicked();

    void on_regButton_clicked();

signals:
    void regResult(bool res);

private:
    Ui::RegistrationWindow *ui;
    FileManager* fm;
};

#endif // REGISTRATIONWINDOW_H
