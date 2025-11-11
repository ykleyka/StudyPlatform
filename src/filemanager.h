#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QList>
#include "User/nameduser.h"
#include "User/student.h"
#include "User/teacher.h"
#include "Test/test.h"
#include "Test/testparser.h"

class FileManager {
private:
    QString dataDir = "C:/Users/Administrator/Documents/untitled/data";
    QString usersFile = dataDir + "/users.txt";
    QString testsFile = dataDir + "/tests.txt";
    QString resultsFile = dataDir + "/results.txt";
    QString testsDataDir = dataDir + "/tests";

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
    void createQuestionFromData(const QMap<QString, QString>& data, Test* test);
    QList<Test*> loadAllTests();
    QList<Test*> loadTestsByTeacher(const QString& teacherLogin);
    int getNextTestId();
    bool deleteTest(int testId);

    // Работа с результатами
    bool saveTestResult(const QString& studentLogin, int testId, double score);
    double calculateTestAverage(int testId);
    QList<QPair<QString, double>> getTestResults(int testId);
    int getTestAttemptCount(int testId);
    double getTestAverageScore(int testId);

    // Импорт теста
    Test* importTestFromFile(const QString& filename, const QString& teacherLogin, QString& errorMessage);
};
#endif // FILEMANAGER_H
