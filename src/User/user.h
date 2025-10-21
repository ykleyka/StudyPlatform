#ifndef USER_H
#define USER_H
#include <QString>
#include <QList>

class User
{
protected:
    QString name;
    QString login;
    QString password;
public:
    User(const QString& login, const QString& password) :
         login(login), password(password) {}
    virtual ~User() = default;
    virtual QString getRole() const {return NULL;};
    bool checkPassword(const QString& inPassword) const;
    QString getLogin() const { return login; }
    QString getPassword() const { return password; }

};

#endif // USER_H
