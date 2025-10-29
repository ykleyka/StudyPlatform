#ifndef MULTIPLECHOICEQUESTION_H
#define MULTIPLECHOICEQUESTION_H

#include "choicequestion.h"
#include <QList>

class MultipleChoiceQuestion : public ChoiceQuestion {
private:
    QList<int> correctIndices;

public:
    MultipleChoiceQuestion(const QString& text, int pts, const QStringList& opts, const QList<int>& correctIndices)
        : ChoiceQuestion(text, pts, opts), correctIndices(correctIndices) {}

    bool checkAnswer(const QString& answer) const override;
    QStringList getCorrectAnswers() const override;
    QString getDisplayType() const override { return "Множественный выбор"; }
};
#endif // MULTIPLECHOICEQUESTION_H
