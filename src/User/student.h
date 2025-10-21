#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QMap>
#include "nameduser.h"

class Student : public NamedUser
{
    QMap<int, int> completed_tests;
public:
    Student(const QString& login, const QString& password, const QString& name) :
        NamedUser(login, password, name) {};
    QString getRole() const override { return "student"; }
    void addTestResult(int testId, double score);
    double getAverageScore() const;
};

#endif // STUDENT_H
