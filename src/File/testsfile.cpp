#include "testsfile.h"
#include "../Test/testparser.h"

TestsFile::TestsFile(const QString& path, const QString& testsDir) : File(path), testsPath(testsDir) {}

bool TestsFile::saveTest(Test* test) {
    int questionCount = test->getQuestionCount();
    int maxScore = test->getMaxScore();

    return saveTestRecord(
        test->getId(),
        test->getTitle(),
        test->getSubject(),
        test->getTeacherLogin(),
        questionCount,
        maxScore
        );
}

bool TestsFile::saveTestRecord(int testId, const QString& title, const QString& subject,
                               const QString& teacherLogin, int questionCount, int maxScore) {
    QFile file(filePath);
    QTextStream out;

    if (!openAppend(file, out)) {
        return false;
    }

    // Сохраняем 6 полей вместо 5
    out << testId << ";" << title << ";" << subject << ";"
        << teacherLogin << ";" << questionCount << ";" << maxScore << "\n";

    file.close();
    return true;
}

QList<TestInfo*> TestsFile::loadAllTestInfos() {
    QList<TestInfo*> testInfos;
    QFile file(filePath);
    QTextStream in;

    if (!openRead(file, in)) {
        return testInfos;
    }

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        TestInfo* testInfo = parseTestInfoLine(line);
        if (testInfo) {
            testInfos.append(testInfo);
        }
    }

    file.close();
    return testInfos;
}

TestInfo* TestsFile::parseTestInfoLine(const QString& line) {
    QStringList parts = line.split(';');

    if (parts.size() < 5) {
        return nullptr;
    }

    bool ok;
    int testId = parts[0].toInt(&ok);
    if (!ok) return nullptr;

    QString title = parts[1];
    QString subject = parts[2];
    QString teacherLogin = parts[3];

    int questionCount = parts[4].toInt();
    int maxScore = parts[5].toInt();

    return new TestInfo(testId, title, subject, teacherLogin, questionCount, maxScore);
}

TestInfo* TestsFile::loadTestInfo(int testId) {
    QList<TestInfo*> allInfos = loadAllTestInfos();

    for (TestInfo* info : allInfos) {
        if (info->getId() == testId) {
            return info;
        }
        delete info;
    }

    return nullptr;
}

Test* TestsFile::loadTest(int testId) {
    QString testFilename = testsPath + "/test_" + QString::number(testId) + ".txt";
    TestInfo* info = loadTestInfo(testId);

    Test* fullTest = new Test(info);
    TestParser parser;
    parser.parseTestQuestions(testFilename, fullTest);

    delete info;
    return fullTest;
}

bool TestsFile::removeTest(int testId) {
    QList<TestInfo*> allInfos = loadAllTestInfos();
    QFile file(filePath);
    QTextStream out;

    if (!openWrite(file, out)) {
        for (TestInfo* info : allInfos) delete info;
        return false;
    }

    bool found = false;
    for (TestInfo* info : allInfos) {
        if (info->getId() != testId) {
            // Сохраняем тест обратно
            out << info->getId() << ";"
                << info->getTitle() << ";"
                << info->getSubject() << ";"
                << info->getTeacherLogin() << ";"
                << info->getQuestionCount() << ";"
                << info->getMaxScore() << "\n";
        } else {
            found = true;
        }
        delete info;
    }

    file.close();
    return found;
}

int TestsFile::getNextId() {
    QList<TestInfo*> infos = loadAllTestInfos();
    int maxId = 0;

    for (TestInfo* info : infos) {
        if (info->getId() > maxId) {
            maxId = info->getId();
        }
        delete info;
    }

    return maxId + 1;
}
