// filemanager.cpp
#include "filemanager.h"
#include <QDebug>
#include <QDir>

FileManager::FileManager() {
    qDebug() << "Initializing FileManager...";
    qDebug() << "Data directory:" << dataDir;

    // 1. Создаем директорию data если её нет
    QDir dir;
    if (!dir.exists(dataDir)) {
        qDebug() << "Creating data directory...";
        if (!dir.mkpath(dataDir)) {
            qCritical() << "Failed to create data directory:" << dataDir;
        }
    }

    // 2. Создаем папку tests если её нет
    if (!dir.exists(testsDataDir)) {
        qDebug() << "Creating tests directory...";
        if (!dir.mkpath(testsDataDir)) {
            qCritical() << "Failed to create tests directory:" << testsDataDir;
        }
    }

    usersFile = new UsersFile(dataDir + "/users.txt");
    testsFile = new TestsFile(dataDir + "/tests.txt", testsDataDir);
    resultsFile = new ResultsFile(dataDir + "/results.txt");

    if (!createDefaultFiles()) {
        qCritical() << "Failed to create default files";
    }

    qDebug() << "FileManager initialized successfully";
}

FileManager::~FileManager() {
    delete usersFile;
    delete testsFile;
    delete resultsFile;
}

bool FileManager::createDefaultFiles()
{
    if (!usersFile->exists() && !usersFile->create()) return false;
    if (!testsFile->exists() && !testsFile->create()) return false;
    if (!resultsFile->exists() && !resultsFile->create()) return false;
    return true;
}

bool FileManager::saveUser(NamedUser* user) {
    return usersFile->saveUser(user);
}

NamedUser* FileManager::loadUser(const QString& login) {
    return usersFile->loadUser(login);
}

bool FileManager::checkUser(const User* in) {
    NamedUser* user = usersFile->loadUser(in->getLogin());
    if (user != NULL) {
        bool passwordMatch = (user->getPassword() == in->getPassword());
        delete user;
        return passwordMatch;
    }
    return false;
}

bool FileManager::userExists(const QString& login) {
    return usersFile->userExists(login);
}

QList<NamedUser*> FileManager::loadAllUsers() {
    return usersFile->loadAllUsers();
}

// ==================== РАБОТА С ТЕСТАМИ ====================

bool FileManager::saveTest(Test* test) {
    test->setId(getNextTestId());

    int realQuestionCount = test->getQuestionCount();
    int realMaxScore = test->getMaxScore();

    qDebug() << "Сохранение теста:" << test->getTitle()
             << "Вопросов:" << realQuestionCount
             << "Макс. балл:" << realMaxScore;

    bool success = testsFile->saveTestRecord(
        test->getId(),
        test->getTitle(),
        test->getSubject(),
        test->getTeacherLogin(),
        realQuestionCount,
        realMaxScore
        );

    if (success) {
        QString testFilename = testsDataDir + "/test_" + QString::number(test->getId()) + ".txt";
        QFile testFile(testFilename);
        if (testFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&testFile);

            out << test->getTitle() << ";"
                << test->getSubject() << ";"
                << test->getTeacherLogin() << "\n";

            const QList<Question*>& questions = test->getQuestions();
            for (Question* question : questions) {
                if (SingleChoiceQuestion* scq = dynamic_cast<SingleChoiceQuestion*>(question)) {
                    out << "single;" << scq->getQuestionText() << ";"
                        << scq->getPoints() << ";" << scq->getOptions().size() << ";"
                        << scq->getCorrectIndex() << ";";
                    for (const QString& option : scq->getOptions()) {
                        out << option << ";";
                    }
                    out << "\n";
                } else if (MultipleChoiceQuestion* mcq = dynamic_cast<MultipleChoiceQuestion*>(question)) {
                    out << "multiple;" << mcq->getQuestionText() << ";"
                        << mcq->getPoints() << ";" << mcq->getOptions().size() << ";"
                        << mcq->getCorrectIndices() << ";";
                    for (const QString& option : mcq->getOptions()) {
                        out << option << ";";
                    }
                    out << "\n";
                } else if (TextQuestion* tq = dynamic_cast<TextQuestion*>(question)) {
                    out << "text;" << tq->getQuestionText() << ";"
                        << tq->getPoints() << ";1;"
                        << tq->getCorrectAnswer() << "\n";
                }
            }
            testFile.close();
            qDebug() << "Тест сохранен в файл:" << testFilename;
        }
    }

    return success;
}

TestInfo* FileManager::loadTestInfo(int testId) {
    QList<TestInfo*> infos = testsFile->loadAllTestInfos();
    TestInfo* reqInfo = NULL;
    for (TestInfo* info : infos) {
        if (info->getId() == testId) {
            reqInfo = info;
        } else delete info;
    }
    return reqInfo;
};

QList<TestInfo*> FileManager::loadAllTestInfos() {
    return testsFile->loadAllTestInfos();
}

QList<TestInfo*> FileManager::loadTestInfosByTeacher(const QString& teacherLogin) {
    QList<TestInfo*> allInfos = loadAllTestInfos();
    QList<TestInfo*> teacherInfos;

    for (TestInfo* info : allInfos) {
        if (info->getTeacherLogin() == teacherLogin) {
            teacherInfos.append(info);
        } else {
            delete info;
        }
    }

    return teacherInfos;
}

QList<TestInfo*> FileManager::loadTestInfosBySubject(const QString& subject) {
    QList<TestInfo*> allInfos = loadAllTestInfos();
    QList<TestInfo*> subjectInfos;

    for (TestInfo* info : allInfos) {
        if (info->getSubject() == subject) {
            subjectInfos.append(info);
        } else {
            delete info;
        }
    }

    return subjectInfos;
}

bool FileManager::isUniqueTestTitle(const QString& title) {
    QList<TestInfo*> allInfos = loadAllTestInfos();
    bool res = true;
    for (TestInfo* info : allInfos) {
        if (title == info->getTitle()) res = false;
        delete info;
    }
    return res;
}

Test* FileManager::loadFullTest(int testId) {
    return testsFile->loadTest(testId);
}

int FileManager::getNextTestId() {
    return testsFile->getNextId();
}

bool FileManager::deleteTest(int testId) {
    if (!testsFile->removeTest(testId)) {
        qWarning() << "Failed to remove test record. ID:" << testId;
        return false;
    }

    QString testFilename = testsDataDir + "/test_" + QString::number(testId) + ".txt";
    if (QFile::exists(testFilename)) {
        if (!QFile::remove(testFilename)) {
            qWarning() << "Failed to remove test data file:" << testFilename;
        } else {
            qDebug() << "Test data file removed:" << testFilename;
        }
    }

    if (!resultsFile->removeResults(testId)) {
        qWarning() << "Failed to remove test results. ID:" << testId;
    }

    qDebug() << "Test deleted successfully. ID:" << testId;
    return true;
}

// ==================== РАБОТА С РЕЗУЛЬТАТАМИ ====================

bool FileManager::saveTestResult(const QString& studentLogin, int testId, double score) {
    return resultsFile->save(studentLogin, testId, score);
}

bool FileManager::saveTestResult(const Result& result) {
    return resultsFile->save(result);
}

double FileManager::calculateTestAverage(int testId) {
    return resultsFile->getAverage(testId);
}

QList<Result> FileManager::getTestResults(int testId) {
    return resultsFile->getResults(testId);
}

QList<Result> FileManager::getStudentResults(const QString& studentLogin) {
    return resultsFile->getStudentResults(studentLogin);
}

int FileManager::getTestAttemptCount(int testId) {
    return resultsFile->getAttemptCount(testId);
}

double FileManager::getTestAverageScore(int testId) {
    return calculateTestAverage(testId);
}

double FileManager::calculateStudentAverage(const QString& studentLogin) {
    QList<Result> results = getStudentResults(studentLogin);

    if (results.isEmpty()) {
        return 0.0;
    }

    double totalScore = 0.0;
    int testCount = 0;
    QSet<int> processedTests;

    for (const Result& result : results) {
        if (!processedTests.contains(result.getTestId())) {
            double bestScore = getStudentBestScoreForTest(studentLogin, result.getTestId());
            totalScore += bestScore;
            testCount++;
            processedTests.insert(result.getTestId());
        }
    }

    return testCount > 0 ? totalScore / testCount : 0.0;
}

QMap<int, double> FileManager::getStudentCompletedTests(const QString& studentLogin) {
    QMap<int, double> completedTests;
    QList<Result> results = getStudentResults(studentLogin);

    for (const Result& result : results) {
        int testId = result.getTestId();
        double score = result.getScore();

        if (!completedTests.contains(testId) || score > completedTests[testId]) {
            completedTests[testId] = score;
        }
    }

    return completedTests;
}

bool FileManager::hasStudentCompletedTest(const QString& studentLogin, int testId) {
    return getStudentBestScoreForTest(studentLogin, testId) >= 0;
}

double FileManager::getStudentBestScoreForTest(const QString& studentLogin, int testId) {
    QList<Result> results = getStudentResults(studentLogin);
    double bestScore = -1; // -1 если тест не пройден

    for (const Result& result : results) {
        if (result.getTestId() == testId && result.getScore() > bestScore) {
            bestScore = result.getScore();
        }
    }

    return bestScore;
}

// ==================== ИМПОРТ ТЕСТА ====================

bool FileManager::importTestFromFile(const QString& filename, const QString& teacherLogin, QString& errorMessage) {
    QString title, subject;
    if (!extractTestMetadata(filename, title, subject, errorMessage)) {
        return false;
    }

    // Парсим тест из файла
    Test* test = new Test(-1, title, subject, teacherLogin);
    TestParser parser;
    parser.parseTestQuestions(filename, test);

    bool success = saveTest(test);

    if (success) {
        qDebug() << "Test imported successfully. ID:" << test->getId();
    } else {
        errorMessage = "Не удалось сохранить тест в систему";
    }

    delete test;
    return success;
}

bool FileManager::extractTestMetadata(const QString& filename, QString& title, QString& subject, QString& errorMessage) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorMessage = "Не удалось открыть файл теста: " + filename;
        return false;
    }

    QTextStream in(&file);

    if (in.atEnd()) {
        errorMessage = "Файл теста пуст";
        file.close();
        return false;
    }

    QString headerLine = in.readLine().trimmed();
    file.close();

    QStringList parts = headerLine.split(';');
    if (parts.size() < 3) {
        errorMessage = "Неверный формат заголовка теста. Ожидается: название;предмет;логин";
        return false;
    }

    title = parts[0].trimmed();
    subject = parts[1].trimmed();

    return true;
}
