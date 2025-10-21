#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QList>
#include "User/nameduser.h"
#include "User/student.h"
#include "User/teacher.h"

class FileManager {
private:
    QString dirPath = "C:/Users/Administrator/Documents/untitled/data";
    QString usersFile = dirPath + "/users.txt";
    QString testsFile = dirPath + "/tests.txt";
    QString resultsFile = dirPath + "/results.txt";

public:
    bool initDataDirectory();
    bool createDefaultFiles();

    // Работа с пользователями
    bool saveUser(NamedUser* user);
    NamedUser* loadUser(const QString& login);
    bool checkUser(const User* in);
    bool userExists(const QString& login);
    QList<NamedUser*> loadAllUsers();
/*
    // Работа с тестами
    bool saveTest(const Test& test);
    QList<Test> loadAllTests();
    QList<Test> loadTestsBySubject(const QString& subject);
    QList<Test> loadTestsByTeacher(const QString& teacherLogin);
*/
    // Работа с результатами
    bool saveTestResult(const QString& studentLogin, int testId, double score);
    double calculateStudentAverage(const QString& studentLogin);
    QList<QPair<int, double>> getStudentResults(const QString& studentLogin);
};
#endif // FILEMANAGER_H
