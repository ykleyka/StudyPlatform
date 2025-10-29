#include "testparser.h"

TestParser::TestParser() {
    resetStats();
}

void TestParser::resetStats() {
    lastError.clear();
    totalLinesProcessed = 0;
    successfullyParsed = 0;
    currentFileName.clear();
}

Test* TestParser::parseTestFromFile(const QString& filename, const QString& teacherLogin) {
    resetStats();
    currentFileName = filename;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setError("Не удалось открыть файл");
        return nullptr;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    // Читаем первую строку с метаданными теста
    if (in.atEnd()) {
        setError("Файл пуст");
        return nullptr;
    }

    QString headerLine = in.readLine().trimmed();
    totalLinesProcessed++;

    QStringList headerParts = headerLine.split(';');
    if (headerParts.size() < 3) {
        setError("Неверный формат заголовка теста. Ожидается: название;предмет;логин_преподавателя", 1);
        return nullptr;
    }

    QString title = headerParts[0].trimmed();
    QString subject = headerParts[1].trimmed();

    if (title.isEmpty() || subject.isEmpty()) {
        setError("Название теста и предмет не могут быть пустыми", 1);
        return nullptr;
    }

    Test* test = new Test(-1, title, subject, teacherLogin);

    // Читаем вопросы
    int lineNumber = 1;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        lineNumber++;
        totalLinesProcessed++;

        if (line.isEmpty()) continue;

        Question* question = parseQuestionLine(line, lineNumber);
        if (question) {
            test->addQuestion(question);
            successfullyParsed++;
            qDebug() << "Успешно распарсен вопрос:" << question->getQuestionText();
        }
    }

    file.close();

    if (successfullyParsed == 0) {
        setError("В файле не найдено ни одного валидного вопроса");
        delete test;
        return nullptr;
    }

    qDebug() << "Парсинг завершен. Успешно:" << successfullyParsed
             << "из" << totalLinesProcessed << "строк";

    return test;
}

Question* TestParser::parseQuestionLine(const QString& line, int lineNumber) {
    QStringList parts = line.split(';');

    if (parts.size() < 5) {
        setError("Недостаточно данных для вопроса. Ожидается минимум 5 полей", lineNumber);
        return nullptr;
    }

    QString type = parts[0].trimmed().toLower();
    QString text = parts[1].trimmed();

    if (text.isEmpty()) {
        setError("Текст вопроса не может быть пустым", lineNumber);
        return nullptr;
    }

    bool pointsOk;
    int points = parts[2].toInt(&pointsOk);

    if (!pointsOk || points <= 0) {
        setError("Некорректное количество баллов. Должно быть положительным числом", lineNumber);
        return nullptr;
    }

    if (type == "single") {
        return parseSingleChoice(parts, lineNumber);
    } else if (type == "multiple") {
        return parseMultipleChoice(parts, lineNumber);
    } else if (type == "text") {
        return parseTextQuestion(parts, lineNumber);
    } else {
        setError("Неизвестный тип вопроса: " + type + " (ожидается: single, multiple, text)", lineNumber);
        return nullptr;
    }
}

Question* TestParser::parseSingleChoice(const QStringList& parts, int lineNumber) {
    // Формат: single;текст;баллы;кол-во вариантов;правильный индекс;вар1;вар2;...
    if (parts.size() < 6) {
        setError("Недостаточно данных для вопроса с одиночным выбором", lineNumber);
        return nullptr;
    }

    QString text = parts[1];
    int points = parts[2].toInt();

    bool optionsCountOk, correctOk;
    int optionsCount = parts[3].toInt(&optionsCountOk);
    int correctIndex = parts[4].toInt(&correctOk);

    if (!optionsCountOk || !correctOk) {
        setError("Некорректные числовые значения для вопроса с одиночным выбором", lineNumber);
        return nullptr;
    }

    if (parts.size() - 5 != optionsCount) {
        setError(QString("Несоответствие количества вариантов. Ожидается: %1, получено: %2")
                     .arg(optionsCount).arg(parts.size() - 5), lineNumber);
        return nullptr;
    }

    if (correctIndex < 0 || correctIndex >= optionsCount) {
        setError(QString("Индекс правильного ответа вне диапазона. Допустимо: 0-%1, получено: %2")
                     .arg(optionsCount - 1).arg(correctIndex), lineNumber);
        return nullptr;
    }

    QStringList options;
    for (int i = 5; i < parts.size(); ++i) {
        QString option = parts[i].trimmed();
        if (option.isEmpty()) {
            setError("Вариант ответа не может быть пустым", lineNumber);
            return nullptr;
        }
        options.append(option);
    }

    return new SingleChoiceQuestion(text, points, options, correctIndex);
}

Question* TestParser::parseMultipleChoice(const QStringList& parts, int lineNumber) {
    // Формат: multiple;текст;баллы;кол-во вариантов;правильные_индексы;вар1;вар2;...
    if (parts.size() < 6) {
        setError("Недостаточно данных для вопроса с множественным выбором", lineNumber);
        return nullptr;
    }

    QString text = parts[1];
    int points = parts[2].toInt();

    bool optionsCountOk;
    int optionsCount = parts[3].toInt(&optionsCountOk);

    if (!optionsCountOk || optionsCount <= 0) {
        setError("Некорректное количество вариантов ответа", lineNumber);
        return nullptr;
    }

    if (parts.size() - 5 != optionsCount) {
        setError(QString("Несоответствие количества вариантов. Ожидается: %1, получено: %2")
                     .arg(optionsCount).arg(parts.size() - 5), lineNumber);
        return nullptr;
    }

    // Парсим правильные индексы (формат: "0,2,3")
    QStringList correctIndicesStr = parts[4].split(',', Qt::SkipEmptyParts);
    QList<int> correctIndices;

    for (const QString& idxStr : correctIndicesStr) {
        bool ok;
        int idx = idxStr.trimmed().toInt(&ok);
        if (!ok || idx < 0 || idx >= optionsCount) {
            setError(QString("Некорректный индекс правильного ответа: %1. Допустимо: 0-%2")
                         .arg(idxStr).arg(optionsCount - 1), lineNumber);
            return nullptr;
        }
        correctIndices.append(idx);
    }

    if (correctIndices.isEmpty()) {
        setError("Не указаны правильные ответы", lineNumber);
        return nullptr;
    }

    QStringList options;
    for (int i = 5; i < parts.size(); ++i) {
        QString option = parts[i].trimmed();
        if (option.isEmpty()) {
            setError("Вариант ответа не может быть пустым", lineNumber);
            return nullptr;
        }
        options.append(option);
    }

    return new MultipleChoiceQuestion(text, points, options, correctIndices);
}

Question* TestParser::parseTextQuestion(const QStringList& parts, int lineNumber) {
    // Формат: text;текст;баллы;1;правильный_ответ
    if (parts.size() < 5) {
        setError("Недостаточно данных для текстового вопроса", lineNumber);
        return nullptr;
    }

    QString text = parts[1];
    int points = parts[2].toInt();
    QString correctAnswer = parts[4].trimmed();

    if (correctAnswer.isEmpty()) {
        setError("Правильный ответ не может быть пустым", lineNumber);
        return nullptr;
    }

    return new TextQuestion(text, points, correctAnswer);
}

void TestParser::setError(const QString& error, int lineNumber) {
    if (lineNumber > 0) {
        lastError = QString("Строка %1: %2").arg(lineNumber).arg(error);
    } else {
        lastError = error;
    }
    qWarning() << "TestParser Error:" << lastError;
}
