#include "statswindow.h"
#include "ui_statswindow.h"
#include "../Test/result.h"
#include <QList>

StatsWindow::StatsWindow(FileManager* fm, int testId, TestInfo* testInfo, QWidget *parent)
    : QDialog(parent), ui(new Ui::StatsWindow), fm(fm), testId(testId), testInfo(testInfo)
{
    ui->setupUi(this);
    ui->titleLabel->setText("Статистика теста: " + testInfo->getTitle());

    loadTestResults();
}

StatsWindow::~StatsWindow()
{
    delete testInfo;
    delete ui;
}

void StatsWindow::loadTestResults()
{
    QList<Result> results = fm->getTestResults(testId);
    ui->attemptsLabel->setText("Всего попыток: " + QString::number(results.size()));

    double averageScore = fm->getTestAverageScore(testId);
    ui->averageLabel->setText("Средний балл: " + QString::number(averageScore, 'f', 1) + "/" + QString::number(testInfo->getMaxScore(), 'f', 1));

    ui->resultsTable->setRowCount(results.size());
    for (int i = 0; i < results.size(); ++i) {
        const Result& result = results[i];
        NamedUser* s = fm->loadUser(result.getStudentLogin());
        QString studentItem = result.getStudentLogin() + ":" + s->getName();
        // Ученик
        ui->resultsTable->setItem(i, 0, new QTableWidgetItem(studentItem));

        // Баллы
        ui->resultsTable->setItem(i, 1, new QTableWidgetItem(QString::number(result.getScore(), 'f', 1)));

        // Результат в процентах
        double percentage = (result.getScore() / testInfo->getMaxScore()) * 100;
        ui->resultsTable->setItem(i, 2, new QTableWidgetItem(QString::number(percentage, 'f', 1) + "%"));

        // Дата и время
        ui->resultsTable->setItem(i, 3, new QTableWidgetItem(result.getTimestampString()));
    }

    // Настраиваем ширину колонок
    ui->resultsTable->resizeColumnsToContents();
}
