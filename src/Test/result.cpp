#include "result.h"

Result::Result(const QString& login, int testId, double score, const QDateTime& time)
    : studentLogin(login), testId(testId), score(score), timestamp(time) {}

QString Result::toFileString() const {
    return studentLogin + ";" +
           QString::number(testId) + ";" +
           QString::number(score, 'f', 2) + ";" +
           getTimestampString();
}

Result Result::fromFileString(const QString& fileString) {
    QStringList parts = fileString.split(';');
    if (parts.size() >= 4) {
        QString login = parts[0];
        int testId = parts[1].toInt();
        double score = parts[2].toDouble();
        QDateTime time = QDateTime::fromString(parts[3], "yyyy-MM-dd hh:mm:ss");
        return Result(login, testId, score, time);
    }
    return Result("", -1, 0.0);
}
