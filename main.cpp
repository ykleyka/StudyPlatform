#include "src/Auth/authorizationwindow.h"
#include "src/Lesson/lessonwindow.h"
#include "src/Teacher/teacherwindow.h"
#include "src/User/nameduser.h"
#include "src/filemanager.h"

#include <QApplication>
#include <QMessageBox>
#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication qtApp(argc, argv);

    FileManager* fm = new FileManager();
    if (!fm) {
        QMessageBox::critical(NULL, "Ошибка инициализации",
                              "Не удалось инициализировать систему данных.\n"
                              "Проверьте права доступа к файлам.");
        return -1;
    }
    // Создаем и показываем окно авторизации
    AuthorizationWindow auth(fm);
    auth.show();

    // Подключаем сигнал успешной авторизации
    QObject::connect(&auth, &AuthorizationWindow::loginSuccess,
                     [&](NamedUser* user) {
        qDebug() << "Auth role:" << user->getRole();
        if (user->getRole() == "student") {
            Student* student = static_cast<Student*>(user);
            LessonWindow* lessonWindow = new LessonWindow(fm, student);
            lessonWindow->show();
        } else if (user->getRole() == "teacher") {
            Teacher* teacher = static_cast<Teacher*>(user);
            TeacherWindow* dashboard = new TeacherWindow(fm, teacher);
            dashboard->show();
        }
    });
    return qtApp.exec();
}
