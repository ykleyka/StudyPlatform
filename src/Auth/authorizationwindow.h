#ifndef AUTHORIZATIONWINDOW_H
#define AUTHORIZATIONWINDOW_H
#include "../User/nameduser.h"
#include "../filemanager.h"
#include <QWidget>

namespace Ui {
class AuthorizationWindow;
}

class AuthorizationWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AuthorizationWindow(FileManager* fm, QWidget *parent = nullptr);
    ~AuthorizationWindow();

private slots:
    void on_nextButton_clicked();
    void on_regButton_clicked();

private:
    Ui::AuthorizationWindow *ui;
    FileManager* fm;

signals:
    void loginSuccess(NamedUser* user);
};

#endif // AUTHORIZATIONWINDOW_H
