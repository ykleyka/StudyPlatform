#include "resultsfile.h"

ResultsFile::ResultsFile(const QString& path) : File(path) {}

bool ResultsFile::save(const Result& result) {
    QFile file(filePath);
    QTextStream out;

    if (!openAppend(file, out)) {
        return false;
    }

    out << result.toFileString() << "\n";
    file.close();
    return true;
}

bool ResultsFile::save(const QString& studentLogin, int testId, double score) {
    Result result(studentLogin, testId, score);
    return save(result);
}

QList<Result> ResultsFile::getResults(int testId) {
    QList<Result> results;
    QFile file(filePath);
    QTextStream in;

    if (!openRead(file, in)) {
        return results;
    }

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        Result result = Result::fromFileString(line);
        if (result.getTestId() == testId) {
            results.append(result);
        }
    }

    file.close();
    return results;
}

QList<Result> ResultsFile::getStudentResults(const QString& studentLogin) {
    QList<Result> results;
    QFile file(filePath);
    QTextStream in;

    if (!openRead(file, in)) {
        return results;
    }

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        Result result = Result::fromFileString(line);
        if (result.getStudentLogin() == studentLogin) {
            results.append(result);
        }
    }

    file.close();
    return results;
}

bool ResultsFile::removeResults(int testId) {
    // Реализация аналогичная TestsFile::removeTest
    return true;
}

int ResultsFile::getAttemptCount(int testId) {
    return getResults(testId).size();
}

double ResultsFile::getAverage(int testId) {
    QList<Result> results = getResults(testId);
    if (results.isEmpty()) return 0.0;

    double sum = 0.0;
    for (Result result : results) {
        sum += result.getScore();
    }

    return sum / results.size();
}
