#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include "../Test/test.h"
#include "../User/student.h"
#include "../filemanager.h"
#include <QDialog>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCheckBox>

namespace Ui {
class TestWindow;
}

class TestWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TestWindow(FileManager* fm, Student* student, int testId, QWidget *parent = nullptr);
    ~TestWindow();

private slots:
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_submitButton_clicked();

private:
    Ui::TestWindow *ui;
    FileManager* fm;
    Student* student;
    Test* test;
    int currentQuestionIndex;
    QList<QString> studentAnswers;

    QButtonGroup* singleChoiceGroup;
    QButtonGroup* multipleChoiceGroup;

    void showQuestion(int index);
    void setupSingleChoiceQuestion(SingleChoiceQuestion* question);
    void setupMultipleChoiceQuestion(MultipleChoiceQuestion* question);
    void setupTextQuestion(TextQuestion* question);
    void saveCurrentAnswer();
    void loadCurrentAnswer();
    void updateNavigationButtons();
    void calculateAndSaveResult();
    void clearAnswerWidgets();

signals:
    void testEnd();
};

#endif // TESTWINDOW_H
