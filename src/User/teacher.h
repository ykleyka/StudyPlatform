#ifndef TEACHER_H
#define TEACHER_H
#include "nameduser.h"

class Teacher : public NamedUser
{
    QString name;
    QString subject;
public:
    Teacher(const QString& login, const QString& password, const QString& name, const QString& subject) :
        NamedUser(login, password, name), subject(subject) {};
    QString getRole() const override { return "teacher"; }
    QString toFileString() const override;
};

#endif // TEACHER_H
