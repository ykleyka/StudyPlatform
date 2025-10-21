#ifndef NAMEDUSER_H
#define NAMEDUSER_H

#include <QString>
#include "user.h"

class NamedUser: public User
{
protected:
    QString name;
public:
    NamedUser(const QString& login, const QString& password, const QString& name) :
        User(login, password), name(name) {}
    virtual QString toFileString() const;
};

#endif // NAMEDUSER_H
