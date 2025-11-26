#include "lessonwindow.h"
#include "ui_lessonwindow.h"
#include <QListWidgetItem>
#include <QMessageBox>

LessonWindow::LessonWindow(FileManager* fm, Student* user, QWidget *parent)
    : fm(fm), QDialog(parent), ui(new Ui::LessonWindow), user(user)
{
    ui->setupUi(this);

    // Устанавливаем приветствие
    ui->welcomeLabel->setText("Добро пожаловать, " + user->getName());

    // Загружаем тесты
    loadAvailableTests();
    updateStudentInfo();

    setWindowTitle("Выбор теста - " + user->getName());
}

LessonWindow::~LessonWindow()
{
    delete ui;
}

double LessonWindow::getStudentBestScore(int testId) {
    QList<Result> results = fm->getStudentResults(user->getLogin());

    double bestScore = -1; // -1 если тест еще не пройден

    for (Result& result : results) {
        if (result.getTestId() == testId) {
            if (result.getScore() > bestScore) {
                bestScore = result.getScore();
            }
        }
    }

    return bestScore;
}

void LessonWindow::loadAvailableTests() {
    ui->testsList->clear();

    QList<TestInfo*> allTests = fm->loadAllTestInfos();

    for (TestInfo* test : allTests) {
        int testId = test->getId();
        double bestScore = fm->getStudentBestScoreForTest(user->getLogin(), testId);
        bool isCompleted = (bestScore >= 0);

        QString itemText = QString("[ID:%1] %2")
                               .arg(testId)
                               .arg(formatTestItemText(test, bestScore, isCompleted));

        QListWidgetItem* item = new QListWidgetItem(itemText);

        if (isCompleted) {
            item->setBackground(QColor(230, 245, 230));
        }

        ui->testsList->addItem(item);
        delete test;
    }

    updateInfoLabel();
}


void LessonWindow::updateStudentInfo() {
    // Используем FileManager для расчета среднего балла
    double averageScore = fm->calculateStudentAverage(user->getLogin());
    ui->averageScoreLabel->setText(
        QString("Средний балл: %1").arg(averageScore, 0, 'f', 1)
        );
}

void LessonWindow::updateInfoLabel() {
    int totalTests = ui->testsList->count();
    ui->infoLabel->setText(QString("Доступно тестов: %1").arg(totalTests));
}

void LessonWindow::on_startTestButton_clicked() {
    QListWidgetItem* selectedItem = ui->testsList->currentItem();
    if (!selectedItem) {
        QMessageBox::information(this, "Информация", "Выберите тест для начала");
        return;
    }

    QString itemText = selectedItem->text();

    QRegularExpression idPattern("\\[ID:(\\d+)\\]");
    QRegularExpressionMatch match = idPattern.match(itemText);

    if (match.hasMatch()) {
        TestWindow* testWindow = new TestWindow(fm, user, match.captured(1).toInt());
        testWindow->show();
        this->hide();
        QObject::connect(testWindow, &TestWindow::testEnd,
                         [&]() { this->show(); });
    }
    loadAvailableTests();
}

QString LessonWindow::formatTestItemText(TestInfo* test, double bestScore, bool isCompleted) {
    QString baseText;

    if (isCompleted) {
        double percentage = test->getMaxScore() > 0 ? (bestScore * 100.0) / test->getMaxScore() : 0;
        baseText = QString("%1\nПредмет: %2 | Вопросов: %3\nРезультат: %4/%5 (%6%)")
                       .arg(test->getTitle())
                       .arg(test->getSubject())
                       .arg(test->getQuestionCount())
                       .arg(bestScore, 0, 'f', 1)
                       .arg(test->getMaxScore())
                       .arg(percentage, 0, 'f', 1);
    } else {
        baseText = QString("%1\nПредмет: %2 | Вопросов: %3 | Макс. баллов: %4")
                       .arg(test->getTitle())
                       .arg(test->getSubject())
                       .arg(test->getQuestionCount())
                       .arg(test->getMaxScore());
    }

    return baseText;
}

void LessonWindow::on_refreshButton_clicked() {
    loadAvailableTests();
    updateStudentInfo();
}

void LessonWindow::on_searchButton_clicked() {
    QString searchText = ui->searchField->text().trimmed().toLower();

    if (searchText.isEmpty()) {
        loadAvailableTests();
        return;
    }

    for (int i = 0; i < ui->testsList->count(); ++i) {
        QListWidgetItem* item = ui->testsList->item(i);
        bool match = item->text().toLower().contains(searchText);
        item->setHidden(!match);
    }

    updateInfoLabel();
}

void LessonWindow::on_closeButton_clicked() {
    this->close();
}

void LessonWindow::on_testsList_itemSelectionChanged() {
    bool hasSelection = !ui->testsList->selectedItems().isEmpty();
    ui->startTestButton->setEnabled(hasSelection);
}
