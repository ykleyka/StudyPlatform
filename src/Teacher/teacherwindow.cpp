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
    QList<Test*> teacherTests = fm->loadTestsByTeacher(user->getLogin());

    for (Test* test : teacherTests) {
        int row = ui->testsTable->rowCount();
        ui->testsTable->insertRow(row);

        ui->testsTable->setItem(row, 0, new QTableWidgetItem(QString::number(test->getId())));
        ui->testsTable->setItem(row, 1, new QTableWidgetItem(test->getTitle()));
        ui->testsTable->setItem(row, 2, new QTableWidgetItem(test->getSubject()));
        ui->testsTable->setItem(row, 3, new QTableWidgetItem(QString::number(test->getQuestionCount())));
        /* Колонки с результатами тестов
        int attempts = fm->getTestAttemptCount(test->getId());
        double avgScore = fm->getTestAverageScore(test->getId());

        ui->testsTable->setItem(row, 4, new QTableWidgetItem(QString::number(attempts)));
        ui->testsTable->setItem(row, 5, new QTableWidgetItem(QString::number(avgScore, 'f', 1)));
        */
    }
}

void TeacherWindow::on_importButton_clicked() {
    TestParser parser;
    QFileDialog file;
    QString fileName = file.getOpenFileName(this, "Открыть файл", "", "Файлы (*.txt)");
    if (!fileName.isEmpty()) {
        Test* test = parser.parseTestFromFile(fileName, user->getLogin());
        fm->saveTest(test);
    }
    refreshTable();
}

int TeacherWindow::getSelectedTestId() const {
    QList<QTableWidgetItem*> selectedItems = ui->testsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        return -1;
    }

    // Берем первую выбранную строку
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

void TeacherWindow::on_statsButton_clicked() {

}

void TeacherWindow::on_previewButton_clicked() {

}

void TeacherWindow::on_refreshButton_clicked() {
    refreshTable();
}

void TeacherWindow::on_closeButton_clicked() {

}
