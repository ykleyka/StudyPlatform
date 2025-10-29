#ifndef TESTPARSER_H
#define TESTPARSER_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "test.h"
#include "../Question/singlechoicequestion.h"
#include "../Question/multiplechoicequestion.h"
#include "../Question/textquestion.h"

class TestParser {
private:
    QString lastError;
    int totalLinesProcessed;
    int successfullyParsed;
    QString currentFileName;

public:
    TestParser();

    // Основной метод парсинга
    Test* parseTestFromFile(const QString& filename, const QString& teacherLogin);

    // Методы для получения информации о процессе
    QString getLastError() const { return lastError; }
    int getTotalLinesProcessed() const { return totalLinesProcessed; }
    int getSuccessfullyParsed() const { return successfullyParsed; }
    double getSuccessRate() const {
        return totalLinesProcessed > 0 ? (successfullyParsed * 100.0 / totalLinesProcessed) : 0;
    }
    void resetStats();

private:
    void setError(const QString& error, int lineNumber = -1);
    Question* parseQuestionLine(const QString& line, int lineNumber);
    Question* parseSingleChoice(const QStringList& parts, int lineNumber);
    Question* parseMultipleChoice(const QStringList& parts, int lineNumber);
    Question* parseTextQuestion(const QStringList& parts, int lineNumber);
};

#endif // TESTPARSER_H
