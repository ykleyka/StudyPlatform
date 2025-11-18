#ifndef TEXTQUESTION_H
#define TEXTQUESTION_H

#include "question.h"

class TextQuestion : public Question {
private:
    QString correctAnswer;

public:
    TextQuestion(const QString& text, int pts, const QString& correctAns)
        : Question(text, pts), correctAnswer(correctAns) {
        questionType = "text";
    }

    bool checkAnswer(const QString& answer) const override {
        return answer.trimmed().compare(correctAnswer.trimmed(), Qt::CaseInsensitive) == 0;
    }

    QStringList getCorrectAnswers() const override {
        return {correctAnswer};
    }

    QString getDisplayType() const override { return "Текстовый ответ"; }
    QString getCorrectAnswer() const { return correctAnswer; }
};

#endif // TEXTQUESTION_H
