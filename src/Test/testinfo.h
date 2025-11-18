#ifndef TESTINFO_H
#define TESTINFO_H

#include <QString>

class TestInfo {
private:
    int testId;
    QString title;
    QString subject;
    QString teacherLogin;
    int questionCount;
    int maxScore;

public:
    TestInfo(int id, const QString& title, const QString& subject,
             const QString& teacherLogin, int questionCount, int maxScore);

    int getId() const { return testId; }
    QString getTitle() const { return title; }
    QString getSubject() const { return subject; }
    QString getTeacherLogin() const { return teacherLogin; }
    int getQuestionCount() const { return questionCount; }
    int getMaxScore() const { return maxScore; }
};

#endif // TESTINFO_H
