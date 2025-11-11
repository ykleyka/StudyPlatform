// filemanager.cpp
#include "filemanager.h"
#include <QDebug>
#include <QDir>

bool FileManager::initDataDirectory() {
    qDebug() << "Initializing FileManager...";
    qDebug() << "Data directory:" << dataDir;

    // 1. Создаем директорию data если её нет
    QDir dir;
    if (!dir.exists(dataDir)) {
        qDebug() << "Creating data directory...";
        if (!dir.mkpath(dataDir)) {
            qCritical() << "Failed to create data directory:" << dataDir;
            return false;
        }
    }

    // 2. Создаем папку tests если её нет
    if (!dir.exists(testsDataDir)) {
        qDebug() << "Creating tests directory...";
        if (!dir.mkpath(testsDataDir)) {
            qCritical() << "Failed to create tests directory:" << testsDataDir;
            return false;
        }
    }

    // 3. Создаем необходимые файлы
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
    return user != NULL;
}

bool FileManager::checkUser(const User* in)
{
    NamedUser* user = loadUser(in->getLogin());
    if (user != nullptr) {
        return user->getPassword() == in->getPassword();
    }
    return false;
}


// ==================== РАБОТА С ТЕСТАМИ ====================

bool FileManager::saveTest(Test* test) {
    // Сохраняем основную информацию о тесте в tests.txt
    QFile file(testsFile);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Cannot open tests file for writing:" << testsFile;
        return false;
    }

    int id = getNextTestId();
    test->setId(id);

    QTextStream out(&file);
    out << test->getId() << ";"
        << test->getTitle() << ";"
        << test->getSubject() << ";"
        << test->getTeacherLogin() << ";"
        << "test_" << test->getId() << ".txt"
        << "\n";
    file.close();

    // Сохраняем сам тест в CSV формате в папке tests/
    QString testFilename = testsDataDir + "/test_" + QString::number(test->getId()) + ".txt";
    QFile testFile(testFilename);
    if (!testFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open test data file:" << testFilename;
        return false;
    }

    QTextStream testOut(&testFile);

    // Заголовок теста (первая строка)
    testOut << test->getTitle() << ";"
            << test->getSubject() << ";"
            << test->getTeacherLogin() << "\n";

    // Вопросы (последующие строки)
    const QList<Question*>& questions = test->getQuestions();
    for (Question* question : questions) {
        if (SingleChoiceQuestion* scq = dynamic_cast<SingleChoiceQuestion*>(question)) {
            testOut << "single;"
                    << scq->getQuestionText() << ";"
                    << scq->getPoints() << ";"
                    << scq->getOptions().size() << ";"
                    << scq->getCorrectIndex() << ";";

            // Варианты ответов
            for (const QString& option : scq->getOptions()) {
                testOut << option << ";";
            }
            testOut << "\n";

        } else if (MultipleChoiceQuestion* mcq = dynamic_cast<MultipleChoiceQuestion*>(question)) {
            testOut << "multiple;"
                    << mcq->getQuestionText() << ";"
                    << mcq->getPoints() << ";"
                    << mcq->getOptions().size() << ";"
                    << mcq->getCorrectIndices();

            // Варианты ответов
            for (const QString& option : mcq->getOptions()) {
                testOut << option << ";";
            }
            testOut << "\n";

        } else if (TextQuestion* tq = dynamic_cast<TextQuestion*>(question)) {
            testOut << "text;"
                    << tq->getQuestionText() << ";"
                    << tq->getPoints() << ";"
                    << "1;"
                    << tq->getCorrectAnswers().first() << "\n";
        }
    }

    testFile.close();

    qDebug() << "Test saved in CSV format. ID:" << test->getId() << "File:" << testFilename;
    return true;
}

Test* FileManager::loadTest(int testId) {
    // Загружаем основную информацию о тесте
    QFile file(testsFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open tests file for reading:" << testsFile;
        return nullptr;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(';');
        if (parts.size() < 5) continue;

        bool ok;
        int currentId = parts[0].toInt(&ok);
        if (!ok || currentId != testId) continue;

        QString title = parts[1];
        QString subject = parts[2];
        QString teacherLogin = parts[3];
        QString testDataFile = parts[4];

        // Загружаем тест через TestParser
        QString testFilename = testsDataDir + "/" + testDataFile;
        TestParser parser;
        Test* test = parser.parseTestFromFile(testFilename, teacherLogin);

        if (test) {
            // Устанавливаем правильный ID
            test->setId(testId);
        }

        file.close();
        return test;
    }

    file.close();
    return NULL;
}

QList<Test*> FileManager::loadAllTests() {
    QList<Test*> tests;

    if (!QFile::exists(testsFile)) {
        qWarning() << "Tests file doesn't exist:" << testsFile;
        return tests;
    }

    QFile file(testsFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open tests file for reading:" << testsFile;
        return tests;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(';');
        if (parts.size() < 5) continue;

        bool ok;
        int testId = parts[0].toInt(&ok);
        if (!ok) continue;

        QString title = parts[1];
        QString subject = parts[2];
        QString teacherLogin = parts[3];
        QString testDataFile = parts[4];

        QString testFilename = testsDataDir + "/" + testDataFile;
        if (QFile::exists(testFilename)) {
            TestParser parser;
            Test* test = parser.parseTestFromFile(testFilename, teacherLogin);
            if (test) {
                test->setId(testId);
                tests.append(test);
            }
        }
    }

    file.close();
    return tests;
}

Test* FileManager::importTestFromFile(const QString& filename, const QString& teacherLogin, QString& errorMessage) {
    TestParser parser;
    Test* test = parser.parseTestFromFile(filename, teacherLogin);

    if (!test) {
        errorMessage = parser.getLastError();
        return nullptr;
    }

    // Устанавливаем ID
    int testId = getNextTestId();
    test->setId(testId);

    // Копируем файл теста в папку data/tests/
    QString destFilename = testsDataDir + "/test_" + QString::number(testId) + ".txt";
    if (QFile::copy(filename, destFilename)) {
        qDebug() << "Test file copied to:" << destFilename;

        // Сохраняем запись о тесте
        QFile file(testsFile);
        if (!file.open(QIODevice::Append | QIODevice::Text)) {
            errorMessage = "Не удалось открыть файл тестов для записи";
            delete test;
            return nullptr;
        }

        QTextStream out(&file);
        out << testId << ";"
            << test->getTitle() << ";"
            << test->getSubject() << ";"
            << teacherLogin << ";"
            << "test_" << testId << ".txt"
            << "\n";
        file.close();

        qDebug() << "Test imported successfully. ID:" << testId << "Questions:" << parser.getSuccessfullyParsed();
        return test;
    } else {
        errorMessage = "Не удалось скопировать файл теста в data/tests/";
        delete test;
        return nullptr;
    }
}

QList<Test*> FileManager::loadTestsByTeacher(const QString& teacherLogin) {
    QList<Test*> tests = loadAllTests();

    for (int i = 0; i < tests.size(); i++) {
        if (tests[i]->getTeacherLogin() != teacherLogin) {
            tests.remove(i);
        }
    }
    return tests;
}

int FileManager::getNextTestId() {
    if (!QFile::exists(testsFile)) {
        return 1;
    }

    QFile file(testsFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 1;
    }

    int maxId = 0;
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(';');
        if (parts.size() < 1) continue;

        bool ok;
        int currentId = parts[0].toInt(&ok);

        if (ok && currentId > maxId) {
            maxId = currentId;
        }
    }
    file.close();

    return maxId + 1;
}

bool FileManager::deleteTest(int testId) {
    if (!QFile::exists(testsFile)) {
        qWarning() << "Tests file doesn't exist";
        return false;
    }

    // Читаем все тесты, исключая удаляемый
    QList<QString> remainingTests;
    bool testFound = false;

    QFile file(testsFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open tests file for reading:" << testsFile;
        return false;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(';');
        if (parts.size() < 5) continue;

        bool ok;
        int currentId = parts[0].toInt(&ok);

        if (ok && currentId == testId) {
            testFound = true;
            // Пропускаем удаляемый тест
            continue;
        }

        remainingTests.append(line);
    }
    file.close();

    if (!testFound) {
        qWarning() << "Test not found for deletion. ID:" << testId;
        return false;
    }

    // Перезаписываем файл без удаленного теста
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open tests file for writing:" << testsFile;
        return false;
    }

    QTextStream out(&file);
    for (const QString& testLine : remainingTests) {
        out << testLine << "\n";
    }
    file.close();

    // Удаляем файл теста
    QString testFilename = testsDataDir + "/test_" + QString::number(testId) + ".txt";
    if (QFile::exists(testFilename)) {
        if (!QFile::remove(testFilename)) {
            qWarning() << "Failed to remove test data file:" << testFilename;
            // Не считаем это критической ошибкой - основная запись уже удалена
        } else {
            qDebug() << "Test data file removed:" << testFilename;
        }
    }

    qDebug() << "Test deleted successfully. ID:" << testId;
    return true;
}

