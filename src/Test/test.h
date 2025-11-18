#ifndef TEST_H
#define TEST_H

#include <QList>
#include <QString>
#include "testinfo.h"
#include "../Question/question.h"
#include "../Question/singlechoicequestion.h"
#include "../Question/multiplechoicequestion.h"
#include "../Question/textquestion.h"

class Test {
private:
    int testId;
    QString title;
    QString subject;
    QString teacherLogin;
    QList<Question*> questions;

public:
    Test(int id, const QString& title, const QString& subject, const QString& teacher)
        : testId(id), title(title), subject(subject), teacherLogin(teacher) {}
    Test(TestInfo* info);
    ~Test() {
        for (Question* q : questions) delete q;
    }

    void addQuestion(Question* question) { questions.append(question); }

    int evaluateTest(const QList<QString>& studentAnswers) const {
        if (studentAnswers.size() != questions.size()) return -1;

        int totalScore = 0;
        for (int i = 0; i < questions.size(); ++i) {
            if (questions[i]->checkAnswer(studentAnswers[i])) {
                totalScore += questions[i]->getPoints();
            }
        }
        return totalScore;
    }

    int getId() const { return testId; }
    void setId(int id) { testId = id; }
    QString getTitle() const { return title; }
    QString getSubject() const { return subject; }
    QString getTeacherLogin() const { return teacherLogin; }
    void setTeacherLogin(const QString& login) { teacherLogin = login; }
    const QList<Question*>& getQuestions() const { return questions; }
    int getQuestionCount() const { return questions.size(); }
    int getMaxScore() const {
        int max = 0;
        for (Question* q : questions) {
            max += q->getPoints();
        }
        return max;
    }
};

#endif // TEST_H
