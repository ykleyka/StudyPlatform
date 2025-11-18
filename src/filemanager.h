#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QList>
#include "User/nameduser.h"
#include "Test/test.h"
#include "File/usersfile.h"
#include "File/testsfile.h"
#include "File/resultsfile.h"
#include "Test/testparser.h"

class FileManager {
private:
    QString dataDir = "C:/Users/Administrator/Documents/untitled/data";
    QString testsDataDir = dataDir + "/tests";

    UsersFile* usersFile;
    TestsFile* testsFile;
    ResultsFile* resultsFile;

    bool createDefaultFiles();
    Test* loadFullTest(Test* basicTest);

public:
    FileManager();
    ~FileManager();

    // Работа с пользователями
    bool saveUser(NamedUser* user);
    NamedUser* loadUser(const QString& login);
    bool checkUser(const User* in);
    bool userExists(const QString& login);
    QList<NamedUser*> loadAllUsers();

    // Работа с тестами
    bool saveTest(Test* test);
    Test* loadFullTest(int testId);
    QList<TestInfo*> loadAllTestInfos();
    QList<TestInfo*> loadTestInfosByTeacher(const QString& teacherLogin);
    QList<TestInfo*> loadTestInfosBySubject(const QString& subject);
    int getNextTestId();
    bool deleteTest(int testId);

    // Работа с результатами
    bool saveTestResult(const QString& studentLogin, int testId, double score);
    bool saveTestResult(const Result& result);
    double calculateTestAverage(int testId);
    QList<Result> getTestResults(int testId);
    QList<Result> getStudentResults(const QString& studentLogin);
    int getTestAttemptCount(int testId);
    double getTestAverageScore(int testId);

    double calculateStudentAverage(const QString& studentLogin);
    QMap<int, double> getStudentCompletedTests(const QString& studentLogin);
    bool hasStudentCompletedTest(const QString& studentLogin, int testId);
    double getStudentBestScoreForTest(const QString& studentLogin, int testId);

    // Импорт теста
    bool importTestFromFile(const QString& filename, const QString& teacherLogin, QString& errorMessage);

private:
    bool extractTestMetadata(const QString& filename, QString& title, QString& subject, QString& errorMessage);

};
#endif // FILEMANAGER_H
