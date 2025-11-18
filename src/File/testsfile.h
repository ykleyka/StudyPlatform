#ifndef TESTSFILE_H
#define TESTSFILE_H

#include "file.h"
#include "../Test/testinfo.h"
#include "../Test/test.h"
#include <QList>

class TestsFile : public File {
public:
    TestsFile(const QString& path, const QString& testsDir);

    bool saveTest(Test* test);
    bool saveTestRecord(int testId, const QString& title, const QString& subject,
                        const QString& teacherLogin, int questionCount, int maxScore);
    QList<TestInfo*> loadAllTestInfos();
    TestInfo* loadTestInfo(int testId);
    Test* loadTest(int testId);
    bool removeTest(int testId);
    int getNextId();

private:
    QString testsPath;
    TestInfo* parseTestInfoLine(const QString& line);
};

#endif // TESTSFILE_H
