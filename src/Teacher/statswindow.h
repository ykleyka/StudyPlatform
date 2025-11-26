#ifndef STATSWINDOW_H
#define STATSWINDOW_H

#include "../filemanager.h"
#include "../Test/testinfo.h"
#include <QDialog>

namespace Ui {
class StatsWindow;
}

class StatsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit StatsWindow(FileManager* fm, int testId, TestInfo* testInfo, QWidget *parent = nullptr);
    ~StatsWindow();

private slots:
    void loadTestResults();

private:
    Ui::StatsWindow *ui;
    FileManager* fm;
    int testId;
    TestInfo* testInfo;
};

#endif // STATSWINDOW_H
