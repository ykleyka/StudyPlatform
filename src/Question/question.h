#ifndef QUESTION_H
#define QUESTION_H

#include <QString>
#include <QStringList>

class Question {
protected:
    QString questionText;
    int points;
    QString questionType;

public:
    Question(const QString& text, int pts) : questionText(text), points(pts) {}
    virtual ~Question() = default;

    virtual QString getQuestionText() const { return questionText; }
    virtual int getPoints() const { return points; }
    virtual QString getType() const { return questionType; }

    // Абстрактные методы для проверки ответов
    virtual bool checkAnswer(const QString& answer) const = 0;
    virtual QStringList getCorrectAnswers() const = 0;
    virtual QString getDisplayType() const = 0;
};

#endif // QUESTION_H
