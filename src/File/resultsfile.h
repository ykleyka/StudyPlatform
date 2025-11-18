#ifndef RESULTSFILE_H
#define RESULTSFILE_H

#include "file.h"
#include "../Test/result.h"
#include <QList>

class ResultsFile : public File {
public:
    ResultsFile(const QString& path);

    bool save(const Result& result);
    bool save(const QString& studentLogin, int testId, double score);
    QList<Result> getResults(int testId);
    QList<Result> getStudentResults(const QString& studentLogin);
    bool removeResults(int testId);
    int getAttemptCount(int testId);
    double getAverage(int testId);
};

#endif // RESULTSFILE_H
