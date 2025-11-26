#include "teacherwindow.h"
#include "ui_teacherwindow.h"

TeacherWindow::TeacherWindow(FileManager* fm, Teacher* user, QWidget *parent)
    : fm(fm), QDialog(parent), ui(new Ui::TeacherWindow), user(user)
{
    ui->setupUi(this);

    ui->welcomeLabel->setText("Добро пожаловать, " + user->getName());
    ui->subjectLabel->setText("Предмет " + user->getSubject());
    refreshTable();
}

TeacherWindow::~TeacherWindow()
{
    delete ui;
}

void TeacherWindow::refreshTable() {
    ui->testsTable->setRowCount(0);
    QList<TestInfo*> teacherTests = fm->loadTestInfosByTeacher(user->getLogin());

    for (TestInfo* test : teacherTests) {
        int row = ui->testsTable->rowCount();
        ui->testsTable->insertRow(row);

        ui->testsTable->setItem(row, 0, new QTableWidgetItem(QString::number(test->getId())));
        ui->testsTable->setItem(row, 1, new QTableWidgetItem(test->getTitle()));
        ui->testsTable->setItem(row, 2, new QTableWidgetItem(test->getSubject()));
        ui->testsTable->setItem(row, 3, new QTableWidgetItem(QString::number(test->getQuestionCount())));
        ui->testsTable->setItem(row, 4, new QTableWidgetItem(QString::number(fm->getTestAttemptCount(test->getId()))));
        ui->testsTable->setItem(row, 5, new QTableWidgetItem(QString::number(fm->getTestAverageScore(test->getId()), 'f', 1)));
    }
}

void TeacherWindow::on_importButton_clicked() {
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Выберите файл теста", QDir::homePath(), "Text files (*.txt)");

    if (filename.isEmpty()) return;
    QString title, fileSubject, errorMessage;
    fm->extractTestMetadata(filename, title, fileSubject, errorMessage);

    if (fileSubject.isEmpty() || title.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось определить предмет теста или его название\n" + errorMessage);
        return;
    }

    if (fileSubject != user->getSubject()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Предмет не совпадает",
            QString("Этот тест по предмету \"%1\", а вы преподаете \"%2\".\n\n"
                    "Рекомендуется добавлять тесты только по вашему предмету.\n"
                    "Все равно добавить этот тест?")
                .arg(fileSubject).arg(user->getSubject()),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
            );

        if (reply != QMessageBox::Yes) {
            return;
        }
    }

    if (!fm->isUniqueTestTitle(title)) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Тест с таким названием уже существует",
            QString("Тест с названием %1 уже существует в базе.\n"
                    "Рекомендуется добавлять тесты с уникальными именами.\n"
                    "Все равно добавить этот тест?")
                .arg(fileSubject).arg(user->getSubject()),
            QMessageBox::Yes | QMessageBox::No,
            QMessageBox::No
            );

        if (reply != QMessageBox::Yes) {
            return;
        }
    }

    if (fm->importTestFromFile(filename, user->getLogin(), errorMessage)) {
        QString message = QString("Тест успешно добавлен!\nПредмет: %1").arg(fileSubject);
        if (fileSubject != user->getSubject()) {
            message += "\n\n️ Обратите внимание: предмет теста отличается от вашего основного";
        }
        QMessageBox::information(this, "Успех", message);
        refreshTable();
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить тест:\n" + errorMessage);
    }
}

int TeacherWindow::getSelectedTestId() const {
    QList<QTableWidgetItem*> selectedItems = ui->testsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        return -1;
    }

    int row = selectedItems.first()->row();
    QTableWidgetItem* idItem = ui->testsTable->item(row, 0);

    if (!idItem) {
        return -1;
    }

    return idItem->text().toInt();
}

void TeacherWindow::on_deleteButton_clicked() {
    int testId = getSelectedTestId();
    if (testId == -1) {
        QMessageBox::information(this, "Информация", "Выберите тест для удаления");
        return;
    }

    // Получаем информацию о тесте для подтверждения
    int row = ui->testsTable->currentRow();
    QString testName = ui->testsTable->item(row, 1)->text();
    QString testSubject = ui->testsTable->item(row, 2)->text();

    // Диалог подтверждения
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Подтверждение удаления",
        QString("Вы действительно хотите удалить тест?\n\n"
                "Название: %1\n"
                "Предмет: %2\n"
                "ID: %3\n\n"
                "Это действие нельзя отменить!")
            .arg(testName)
            .arg(testSubject)
            .arg(testId),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        if (fm->deleteTest(testId)) {
            QMessageBox::information(this, "Успех", "Тест успешно удален");

            refreshTable();

            qDebug() << "Test deleted successfully. ID:" << testId;
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось удалить тест");
            qWarning() << "Failed to delete test. ID:" << testId;
        }
    }
}

void TeacherWindow::on_statsButton_clicked()
{
    int testId = getSelectedTestId();
    if (testId == -1) {
        QMessageBox::information(this, "Информация", "Выберите тест для просмотра статистики");
        return;
    }

    // Получаем информацию о тесте
    TestInfo* testInfo = fm->loadTestInfo(testId);
    if (!testInfo) {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить информацию о тесте");
        return;
    }

    StatsWindow* statsWindow = new StatsWindow(fm, testId, testInfo, this);
    statsWindow->exec();
}

void TeacherWindow::on_previewButton_clicked() {
    int testId = getSelectedTestId();
    if (testId == -1) {
        QMessageBox::information(this, "Информация", "Выберите тест для просмотра статистики");
        return;
    }

    Student* student = new Student("", "", "");
    TestWindow* testWindow = new TestWindow(fm, student, testId);
    testWindow->show();
    this->hide();
    QObject::connect(testWindow, &TestWindow::testEnd,
                         [&]() { this->show(); });
}

void TeacherWindow::on_refreshButton_clicked() {
    refreshTable();
}

void TeacherWindow::on_closeButton_clicked() {

}
