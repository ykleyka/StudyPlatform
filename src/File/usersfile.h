#ifndef USERSFILE_H
#define USERSFILE_H

#include "file.h"
#include "../User/nameduser.h"
#include "../User/student.h"
#include "../User/teacher.h"

class UsersFile : public File {
public:
    UsersFile(const QString& path);

    bool saveUser(NamedUser* user);
    NamedUser* loadUser(const QString& login);
    bool userExists(const QString& login);
    QList<NamedUser*> loadAllUsers();
};

#endif // USERSFILE_H
