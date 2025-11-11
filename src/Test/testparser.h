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
    int successfullyParsed;

public:
    TestParser();

    Test* parseTestFromFile(const QString& filename, const QString& teacherLogin);

    QString getLastError() const { return lastError; }
    int getSuccessfullyParsed() const { return successfullyParsed; }

private:
    void setError(const QString& error);
    Question* parseQuestionLine(const QString& line);
    Question* parseSingleChoice(const QStringList& parts, const QString& text, int points);
    Question* parseMultipleChoice(const QStringList& parts, const QString& text, int points);
    Question* parseTextQuestion(const QStringList& parts, const QString& text, int points);
};

#endif // TESTPARSER_H
