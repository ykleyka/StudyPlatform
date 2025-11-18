#include "testwindow.h"
#include "ui_testwindow.h"
#include <QMessageBox>
#include <QRadioButton>
#include <QCheckBox>

TestWindow::TestWindow(FileManager* fm, Student* student, int testId, QWidget *parent)
    : QDialog(parent), ui(new Ui::TestWindow), fm(fm), student(student),
    currentQuestionIndex(0), singleChoiceGroup(new QButtonGroup(this)),
    multipleChoiceGroup(new QButtonGroup(this))
{
    ui->setupUi(this);

    // Настраиваем группы кнопок
    singleChoiceGroup->setExclusive(true);
    multipleChoiceGroup->setExclusive(false);

    // Загружаем тест
    test = fm->loadFullTest(testId);
    if (!test) {
        QMessageBox::critical(this, "Ошибка", "Не удалось загрузить тест");
        this->close();
        return;
    }

    // Инициализируем массив ответов
    studentAnswers.fill("", test->getQuestionCount());

    // Настраиваем интерфейс
    ui->testTitleLabel->setText(test->getTitle());
    showQuestion(0);
}

TestWindow::~TestWindow()
{
    delete test;
    delete ui;
}

void TestWindow::showQuestion(int index)
{
    if (index < 0 || index >= test->getQuestionCount()) {
        return;
    }

    currentQuestionIndex = index;

    Question* question = test->getQuestions()[index];
    ui->questionTextLabel->setText(question->getQuestionText());

    // Обновляем прогресс
    ui->progressLabel->setText(
        QString("Вопрос %1 из %2\n%3 баллов").arg(index + 1).arg(test->getQuestionCount()).arg(question->getPoints())
    );

    // Очищаем предыдущие варианты ответов
    clearAnswerWidgets();
    ui->textAnswerField->clear();

    // Настраиваем интерфейс в зависимости от типа вопроса
    if (SingleChoiceQuestion* scq = dynamic_cast<SingleChoiceQuestion*>(question)) {
        setupSingleChoiceQuestion(scq);
        ui->answerStackWidget->setCurrentIndex(0);
    } else if (MultipleChoiceQuestion* mcq = dynamic_cast<MultipleChoiceQuestion*>(question)) {
        setupMultipleChoiceQuestion(mcq);
        ui->answerStackWidget->setCurrentIndex(1);
    } else if (TextQuestion* tq = dynamic_cast<TextQuestion*>(question)) {
        setupTextQuestion(tq);
        ui->answerStackWidget->setCurrentIndex(2);
    }

    // Загружаем сохраненный ответ (если есть)
    loadCurrentAnswer();

    // Обновляем кнопки навигации
    updateNavigationButtons();
}

void TestWindow::clearAnswerWidgets()
{
    // Очищаем single choice
    QLayoutItem* item;
    while ((item = ui->singleChoiceLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Очищаем multiple choice
    while ((item = ui->multipleChoiceLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Очищаем группы кнопок
    singleChoiceGroup->setExclusive(false); // Временно снимаем exclusive для очистки
    QList<QAbstractButton*> singleButtons = singleChoiceGroup->buttons();
    for (QAbstractButton* button : singleButtons) {
        singleChoiceGroup->removeButton(button);
    }
    singleChoiceGroup->setExclusive(true);

    QList<QAbstractButton*> multipleButtons = multipleChoiceGroup->buttons();
    for (QAbstractButton* button : multipleButtons) {
        multipleChoiceGroup->removeButton(button);
    }
}

void TestWindow::setupSingleChoiceQuestion(SingleChoiceQuestion* question)
{
    const QStringList& options = question->getOptions();
    qDebug() << "correct:" << question->getCorrectAnswers();
    for (int i = 0; i < options.size(); ++i) {
        QRadioButton* radioButton = new QRadioButton(options[i]);
        radioButton->setStyleSheet("font-size: 11pt; padding: 5px;");
        ui->singleChoiceLayout->addWidget(radioButton);
        singleChoiceGroup->addButton(radioButton, i);
    }
}

void TestWindow::setupMultipleChoiceQuestion(MultipleChoiceQuestion* question)
{
    const QStringList& options = question->getOptions();
    qDebug() << "correct:" << question->getCorrectAnswers();
    for (int i = 0; i < options.size(); ++i) {
        QCheckBox* checkBox = new QCheckBox(options[i]);
        checkBox->setStyleSheet("font-size: 11pt; padding: 5px;");
        ui->multipleChoiceLayout->addWidget(checkBox);
        multipleChoiceGroup->addButton(checkBox, i);
    }
}

void TestWindow::setupTextQuestion(TextQuestion* question)
{
    qDebug() << "correct:" << question->getCorrectAnswers();
    Q_UNUSED(question);
}

void TestWindow::saveCurrentAnswer()
{
    if (currentQuestionIndex < 0 || currentQuestionIndex >= studentAnswers.size()) {
        return;
    }

    Question* question = test->getQuestions()[currentQuestionIndex];
    QString answer;

    if (SingleChoiceQuestion* scq = dynamic_cast<SingleChoiceQuestion*>(question)) {
        // Получаем выбранную кнопку через группу
        QAbstractButton* selectedButton = singleChoiceGroup->checkedButton();
        if (selectedButton) {
            answer = QString::number(singleChoiceGroup->id(selectedButton));
        }
    } else if (MultipleChoiceQuestion* mcq = dynamic_cast<MultipleChoiceQuestion*>(question)) {
        // Собираем выбранные чекбоксы через группу
        QStringList selectedIndices;
        QList<QAbstractButton*> buttons = multipleChoiceGroup->buttons();
        for (QAbstractButton* button : buttons) {
            if (button->isChecked()) {
                selectedIndices.append(QString::number(multipleChoiceGroup->id(button)));
            }
        }
        answer = selectedIndices.join(",");
    } else if (TextQuestion* tq = dynamic_cast<TextQuestion*>(question)) {
        answer = ui->textAnswerField->text().trimmed();
    }

    studentAnswers[currentQuestionIndex] = answer;
}

void TestWindow::loadCurrentAnswer()
{
    if (currentQuestionIndex < 0 || currentQuestionIndex >= studentAnswers.size()) {
        return;
    }

    QString answer = studentAnswers[currentQuestionIndex];
    if (answer.isEmpty()) {
        return;
    }

    Question* question = test->getQuestions()[currentQuestionIndex];

    if (SingleChoiceQuestion* scq = dynamic_cast<SingleChoiceQuestion*>(question)) {
        bool ok;
        int index = answer.toInt(&ok);
        if (ok) {
            QAbstractButton* button = singleChoiceGroup->button(index);
            if (button) {
                button->setChecked(true);
            }
        }
    } else if (MultipleChoiceQuestion* mcq = dynamic_cast<MultipleChoiceQuestion*>(question)) {
        QStringList indices = answer.split(',', Qt::SkipEmptyParts);
        for (const QString& indexStr : indices) {
            bool ok;
            int index = indexStr.toInt(&ok);
            if (ok) {
                QAbstractButton* button = multipleChoiceGroup->button(index);
                if (button) {
                    button->setChecked(true);
                }
            }
        }
    } else if (TextQuestion* tq = dynamic_cast<TextQuestion*>(question)) {
        ui->textAnswerField->setText(answer);
    }
}

void TestWindow::updateNavigationButtons()
{
    // Кнопка "Назад"
    ui->prevButton->setEnabled(currentQuestionIndex > 0);

    // Кнопки "Далее" и "Завершить"
    bool isLastQuestion = (currentQuestionIndex == test->getQuestionCount() - 1);
    ui->nextButton->setVisible(!isLastQuestion);
    ui->submitButton->setVisible(isLastQuestion);
}

void TestWindow::on_prevButton_clicked()
{
    saveCurrentAnswer();
    showQuestion(currentQuestionIndex - 1);
}

void TestWindow::on_nextButton_clicked()
{
    saveCurrentAnswer();
    showQuestion(currentQuestionIndex + 1);
}

void TestWindow::on_submitButton_clicked()
{
    saveCurrentAnswer();

    // Подтверждение завершения теста
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Завершение теста",
        "Вы уверены, что хотите завершить тест?\n\nПосле завершения изменить ответы будет невозможно.",
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        calculateAndSaveResult();
    }
}

void TestWindow::calculateAndSaveResult()
{
    // Вычисляем результат
    int score = test->evaluateTest(studentAnswers);
    int maxScore = test->getMaxScore();

    // Сохраняем результат
    if (fm->saveTestResult(student->getLogin(), test->getId(), score)) {
        // Показываем результат
        double percentage = maxScore > 0 ? (score * 100.0) / maxScore : 0;
        QString message = QString(
                              "Тест завершен!\n\n"
                              "Ваш результат: %1 из %2 баллов\n"
                              "Процент выполнения: %3%"
                              ).arg(score).arg(maxScore).arg(percentage, 0, 'f', 1);

        QMessageBox::information(this, "Результат теста", message);
        this->close();
        emit testEnd();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить результат теста");
    }
}
