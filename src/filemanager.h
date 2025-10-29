#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QList>
#include "User/nameduser.h"
#include "User/student.h"
#include "User/teacher.h"
#include "Test/test.h"

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

    // Работа с тестами
    bool saveTest(Test* test);
    Test* loadTest(int testId);
    QList<Test*> loadAllTests();
    QList<Test*> loadTestsByTeacher(const QString& teacherLogin);
    QList<Test*> loadTestsBySubject(const QString& subject);
    int getNextTestId();

    // Работа с результатами
    bool saveTestResult(const QString& studentLogin, int testId, double score);
    double calculateTestAverage(int testId);
    QList<QPair<QString, double>> getTestResults(int testId);
};
#endif // FILEMANAGER_H
