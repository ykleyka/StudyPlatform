#include "student.h"

void Student::addTestResult(int testId, double score) {
    completed_tests.insert(testId, score);
};

double Student::getAverageScore() const {
    if (!completed_tests.isEmpty())
    {
        QMap<int, int>::const_iterator i;
        int j = 0, score = 0;
        for (i = completed_tests.constBegin(); i != completed_tests.constEnd(); ++i, ++j) {
            score += i.value();
        }
        return score / j;
    }
    return 0;
};
