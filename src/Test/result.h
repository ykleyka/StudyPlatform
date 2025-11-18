#ifndef RESULT_H
#define RESULT_H

#include <QString>
#include <QDateTime>

class Result {
private:
    QString studentLogin;
    int testId;
    double score;
    QDateTime timestamp;

public:
    Result(const QString& login, int testId, double score, const QDateTime& time = QDateTime::currentDateTime());

    // Геттеры
    QString getStudentLogin() const { return studentLogin; }
    int getTestId() const { return testId; }
    double getScore() const { return score; }
    QDateTime getTimestamp() const { return timestamp; }
    QString getTimestampString() const { return timestamp.toString("yyyy-MM-dd hh:mm:ss"); }

    // Для сохранения в файл
    QString toFileString() const;

    // Статический метод для создания из строки файла
    static Result fromFileString(const QString& fileString);
};

#endif // RESULT_H
