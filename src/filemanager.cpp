// filemanager.cpp
#include "filemanager.h"
#include <QDebug>
#include <QDir>

bool FileManager::initDataDirectory()
{
    qDebug() << "Initializing FileManager...";

    // 1. Создаем директорию data если её нет
    QDir dir;
    if (!dir.exists(dirPath)) {
        qDebug() << "Creating data directory...";
        if (!dir.mkpath(dirPath)) {
            qCritical() << "Failed to create data directory:" << dirPath;
            return false;
        }
    }

    // 2. Создаем необходимые файлы
    if (!createDefaultFiles()) {
        qCritical() << "Failed to create default files";
        return false;
    }

    qDebug() << "FileManager initialized successfully";
    return true;
}

bool FileManager::createDefaultFiles()
{
    QFile uFile(usersFile);
    QFile tFile(testsFile);
    QFile rFile(resultsFile);

    // Создаем файлы если они не существуют
    bool success = true;

    if (!uFile.exists()) {
        if (!uFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical() << "Cannot create users file:" << usersFile;
            success = false;
        } else {
            uFile.close();
        }
    }

    if (!tFile.exists()) {
        if (!tFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical() << "Cannot create tests file:" << testsFile;
            success = false;
        } else {
            tFile.close();
        }
    }

    if (!rFile.exists()) {
        if (!rFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qCritical() << "Cannot create results file:" << resultsFile;
            success = false;
        } else {
            rFile.close();
        }
    }

    return success;
}

bool FileManager::saveUser(NamedUser* user)
{
    QFile file(usersFile);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Cannot open users file for writing:" << usersFile;
        return false;
    }

    QTextStream out(&file);
    out << user->toFileString() << "\n";
    file.close();

    return true;
}

NamedUser* FileManager::loadUser(const QString& login)
{
    QFile file(usersFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open users file for reading:" << usersFile;
        return nullptr;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(";");
        if (parts.size() < 3) continue;

        QString role = parts[0];
        QString userLogin = parts[1];

        if (userLogin == login) {
            if (role == "student" && parts.size() >= 4) {
                return new Student(parts[1], parts[2], parts[3]);
            } else if (role == "teacher" && parts.size() >= 5) {
                return new Teacher(parts[1], parts[2], parts[3], parts[4]);
            }
        }
    }

    file.close();
    return nullptr;
}

bool FileManager::userExists(const QString& login) {
    NamedUser* user = loadUser(login);
    return user != nullptr;
}

bool FileManager::checkUser(const User* in)
{
    NamedUser* user = loadUser(in->getLogin());
    if (user != nullptr) {
        return user->getPassword() == in->getPassword();
    }
    return false;
}

/*
// Остальные методы реализуются аналогично...

int FileManager::getNextTestId()
{
    QList<Test> tests = loadAllTests();
    int maxId = 0;
    for (const Test& test : tests) {
        if (test.getId() > maxId) {
            maxId = test.getId();
        }
    }
    return maxId + 1;
}

void FileManager::clearAllData()
{
    QFile::remove(usersFile);
    QFile::remove(testsFile);
    QFile::remove(resultsFile);
    init(); // Пересоздаем пустые файлы
}
*/
