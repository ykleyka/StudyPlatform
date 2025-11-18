#include "usersfile.h"

UsersFile::UsersFile(const QString& path) : File(path) {}

bool UsersFile::saveUser(NamedUser* user) {
    QFile file(filePath);
    QTextStream out;

    if (!openAppend(file, out)) {
        return false;
    }

    out << user->toFileString() << "\n";
    file.close();
    return true;
}

NamedUser* UsersFile::loadUser(const QString& login) {
    QFile file(filePath);
    QTextStream in;

    if (!openRead(file, in)) {
        return NULL;
    }

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(";");
        if (parts.size() < 3) continue;

        QString userLogin = parts[1];
        if (userLogin == login) {
            if (parts[0] == "student" && parts.size() >= 4) {
                return new Student(parts[1], parts[2], parts[3]);
            } else if (parts[0] == "teacher" && parts.size() >= 5) {
                return new Teacher(parts[1], parts[2], parts[3], parts[4]);
            }
        }
    }

    file.close();
    return NULL;
}

bool UsersFile::userExists(const QString& login) {
    NamedUser* user = loadUser(login);
    bool exists = (user != NULL);
    delete user;
    return exists;
}

QList<NamedUser*> UsersFile::loadAllUsers() {
    QList<NamedUser*> users;
    QFile file(filePath);
    QTextStream in;

    if (!openRead(file, in)) {
        return users;
    }

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(";");
        if (parts.size() < 3) continue;

        NamedUser* user = NULL;
        if (parts[0] == "student" && parts.size() >= 4) {
            user = new Student(parts[1], parts[2], parts[3]);
        } else if (parts[0] == "teacher" && parts.size() >= 5) {
            user = new Teacher(parts[1], parts[2], parts[3], parts[4]);
        }

        if (user) {
            users.append(user);
        }
    }

    file.close();
    return users;
}
