#ifndef SINGLECHOICEQUESTION_H
#define SINGLECHOICEQUESTION_H

#include "choicequestion.h"

class SingleChoiceQuestion : public ChoiceQuestion {
private:
    int correctIndex;

public:
    SingleChoiceQuestion(const QString& text, int pts, const QStringList& opts, int correctIdx)
        : ChoiceQuestion(text, pts, opts), correctIndex(correctIdx) {}

    bool checkAnswer(const QString& answer) const override {
        bool ok;
        int selectedIndex = answer.toInt(&ok);
        return ok && selectedIndex == correctIndex;
    }

    QStringList getCorrectAnswers() const override {
        return {options[correctIndex]};
    }

    QString getDisplayType() const override { return "Одиночный выбор"; }
    int getCorrectIndex() const { return correctIndex; }
};

#endif // SINGLECHOICEQUESTION_H
