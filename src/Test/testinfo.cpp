#include "testinfo.h"

TestInfo::TestInfo(int id, const QString& title, const QString& subject,
                   const QString& teacherLogin, int questionCount, int maxScore)
    : testId(id), title(title), subject(subject), teacherLogin(teacherLogin),
    questionCount(questionCount), maxScore(maxScore) {}
