#include "testparser.h"

TestParser::TestParser() {
    lastError.clear();
    successfullyParsed = 0;
}

Test* TestParser::parseTestFromFile(const QString& filename, const QString& teacherLogin) {
    lastError.clear();
    successfullyParsed = 0;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        setError("Не удалось открыть файл: " + filename);
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
    QStringList headerParts = headerLine.split(';');

    if (headerParts.size() < 3) {
        setError("Неверный формат заголовка теста");
        return nullptr;
    }

    QString title = headerParts[0].trimmed();
    QString subject = headerParts[1].trimmed();

    // Создаем тест
    Test* test = new Test(-1, title, subject, teacherLogin);

    // Читаем вопросы
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        Question* question = parseQuestionLine(line);
        if (question) {
            test->addQuestion(question);
            successfullyParsed++;
        }
    }

    file.close();

    if (!successfullyParsed) {
        setError("В файле не найдено вопросов");
        delete test;
        return NULL;
    }

    qDebug() << "Успешно распарсено вопросов:" << successfullyParsed;
    return test;
}

Question* TestParser::parseQuestionLine(const QString& line) {
    QStringList parts = line.split(';');

    if (parts.size() < 5) {
        qWarning() << "Недостаточно данных для вопроса:" << line;
        return nullptr;
    }

    QString type = parts[0].trimmed().toLower();
    QString text = parts[1].trimmed();

    // Парсим баллы (просто пытаемся преобразовать, без проверок)
    int points = parts[2].toInt();
    if (points <= 0) points = 1; // Минимум 1 балл

    if (type == "single") {
        return parseSingleChoice(parts, text, points);
    } else if (type == "multiple") {
        return parseMultipleChoice(parts, text, points);
    } else if (type == "text") {
        return parseTextQuestion(parts, text, points);
    } else {
        qWarning() << "Неизвестный тип вопроса:" << type;
        return nullptr;
    }
}

Question* TestParser::parseSingleChoice(const QStringList& parts, const QString& text, int points) {
    // Формат: single;текст;баллы;кол-во вариантов;правильный индекс;вар1;вар2;...
    if (parts.size() < 6) {
        qWarning() << "Недостаточно данных для single choice:" << parts.join(";");
        return nullptr;
    }

    int optionsCount = parts[3].toInt();
    int correctIndex = parts[4].toInt();

    // Проверяем границы
    if (correctIndex < 0) correctIndex = 0;
    if (correctIndex >= optionsCount) correctIndex = optionsCount - 1;

    QStringList options;
    for (int i = 5; i < parts.size() && i < 5 + optionsCount; ++i) {
        options.append(parts[i].trimmed());
    }

    // Если вариантов меньше чем указано, добавляем пустые
    while (options.size() < optionsCount) {
        options.append("Вариант " + QString::number(options.size() + 1));
    }

    return new SingleChoiceQuestion(text, points, options, correctIndex);
}

Question* TestParser::parseMultipleChoice(const QStringList& parts, const QString& text, int points) {
    // Формат: multiple;текст;баллы;кол-во вариантов;правильные_индексы;вар1;вар2;...
    if (parts.size() < 6) {
        qWarning() << "Недостаточно данных для multiple choice:" << parts.join(";");
        return nullptr;
    }

    int optionsCount = parts[3].toInt();

    // Парсим правильные индексы
    QStringList correctIndicesStr = parts[4].split(',', Qt::SkipEmptyParts);
    QList<int> correctIndices;

    for (const QString& idxStr : correctIndicesStr) {
        int idx = idxStr.trimmed().toInt();
        if (idx >= 0 && idx < optionsCount) {
            correctIndices.append(idx);
        }
    }

    // Если нет правильных ответов, берем первый
    if (correctIndices.isEmpty() && optionsCount > 0) {
        correctIndices.append(0);
    }

    QStringList options;
    for (int i = 5; i < parts.size() && i < 5 + optionsCount; ++i) {
        options.append(parts[i].trimmed());
    }

    // Если вариантов меньше чем указано, добавляем пустые
    while (options.size() < optionsCount) {
        options.append("Вариант " + QString::number(options.size() + 1));
    }

    return new MultipleChoiceQuestion(text, points, options, correctIndices);
}

Question* TestParser::parseTextQuestion(const QStringList& parts, const QString& text, int points) {
    // Формат: text;текст;баллы;1;правильный_ответ
    if (parts.size() < 5) {
        qWarning() << "Недостаточно данных для text question:" << parts.join(";");
        return nullptr;
    }

    QString correctAnswer = parts[4].trimmed();
    if (correctAnswer.isEmpty()) {
        correctAnswer = "Ответ";
    }

    return new TextQuestion(text, points, correctAnswer);
}

void TestParser::setError(const QString& error) {
    lastError = error;
    qWarning() << "TestParser Error:" << error;
}
