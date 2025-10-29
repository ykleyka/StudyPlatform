#include "multiplechoicequestion.h"

bool MultipleChoiceQuestion::checkAnswer(const QString& answer) const {
    QStringList selectedIndices = answer.split(',', Qt::SkipEmptyParts);
    if (selectedIndices.size() != correctIndices.size()) return false;

    QList<int> selected;
    for (const QString& idxStr : selectedIndices) {
        bool ok;
        int idx = idxStr.toInt(&ok);
        if (!ok) return false;
        selected.append(idx);
    }

    std::sort(selected.begin(), selected.end());
    QList<int> sortedCorrect = correctIndices;
    std::sort(sortedCorrect.begin(), sortedCorrect.end());

    return selected == sortedCorrect;
}

QStringList MultipleChoiceQuestion::getCorrectAnswers() const {
    QStringList correct;
    for (int idx : correctIndices) {
        correct.append(options[idx]);
    }
    return correct;
}
