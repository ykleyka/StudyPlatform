#ifndef CHOICEQUESTION_H
#define CHOICEQUESTION_H

#include "question.h"

class ChoiceQuestion : public Question {
protected:
    QStringList options;

public:
    ChoiceQuestion(const QString& text, int pts, const QStringList& opts)
        : Question(text, pts), options(opts) {
        questionType = "choice";
    }

    QStringList getOptions() const { return options; }
    virtual QString getDisplayType() const override { return "Выбор"; }
};

#endif // CHOICEQUESTION_H
