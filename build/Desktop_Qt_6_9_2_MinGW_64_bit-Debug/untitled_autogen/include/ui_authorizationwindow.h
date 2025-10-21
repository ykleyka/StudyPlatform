/********************************************************************************
** Form generated from reading UI file 'authorizationwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUTHORIZATIONWINDOW_H
#define UI_AUTHORIZATIONWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AuthorizationWindow
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QLabel *loginText;
    QLineEdit *lineEdit;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_4;
    QLineEdit *lineEdit_3;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *verticalLayout_5;
    QPushButton *nextButton;
    QPushButton *regButton;

    void setupUi(QWidget *AuthorizationWindow)
    {
        if (AuthorizationWindow->objectName().isEmpty())
            AuthorizationWindow->setObjectName("AuthorizationWindow");
        AuthorizationWindow->resize(400, 550);
        QFont font;
        font.setFamilies({QString::fromUtf8("Lora")});
        font.setPointSize(16);
        AuthorizationWindow->setFont(font);
        verticalLayoutWidget = new QWidget(AuthorizationWindow);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 0, 401, 71));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName("label");
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Lora")});
        font1.setPointSize(20);
        label->setFont(font1);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(label);

        verticalLayoutWidget_2 = new QWidget(AuthorizationWindow);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(20, 150, 361, 85));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(verticalLayoutWidget_2->sizePolicy().hasHeightForWidth());
        verticalLayoutWidget_2->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        loginText = new QLabel(verticalLayoutWidget_2);
        loginText->setObjectName("loginText");
        sizePolicy.setHeightForWidth(loginText->sizePolicy().hasHeightForWidth());
        loginText->setSizePolicy(sizePolicy);

        verticalLayout_2->addWidget(loginText);

        lineEdit = new QLineEdit(verticalLayoutWidget_2);
        lineEdit->setObjectName("lineEdit");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy1);
        lineEdit->setMaxLength(20);
        lineEdit->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(lineEdit);

        verticalLayoutWidget_3 = new QWidget(AuthorizationWindow);
        verticalLayoutWidget_3->setObjectName("verticalLayoutWidget_3");
        verticalLayoutWidget_3->setGeometry(QRect(20, 260, 361, 85));
        sizePolicy.setHeightForWidth(verticalLayoutWidget_3->sizePolicy().hasHeightForWidth());
        verticalLayoutWidget_3->setSizePolicy(sizePolicy);
        verticalLayout_3 = new QVBoxLayout(verticalLayoutWidget_3);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_4 = new QLabel(verticalLayoutWidget_3);
        label_4->setObjectName("label_4");
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        verticalLayout_3->addWidget(label_4);

        lineEdit_3 = new QLineEdit(verticalLayoutWidget_3);
        lineEdit_3->setObjectName("lineEdit_3");
        sizePolicy1.setHeightForWidth(lineEdit_3->sizePolicy().hasHeightForWidth());
        lineEdit_3->setSizePolicy(sizePolicy1);
        lineEdit_3->setMaxLength(100);
        lineEdit_3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_3->addWidget(lineEdit_3);

        verticalLayoutWidget_4 = new QWidget(AuthorizationWindow);
        verticalLayoutWidget_4->setObjectName("verticalLayoutWidget_4");
        verticalLayoutWidget_4->setGeometry(QRect(0, 440, 401, 111));
        verticalLayout_5 = new QVBoxLayout(verticalLayoutWidget_4);
        verticalLayout_5->setObjectName("verticalLayout_5");
        verticalLayout_5->setSizeConstraint(QLayout::SizeConstraint::SetMaximumSize);
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        nextButton = new QPushButton(verticalLayoutWidget_4);
        nextButton->setObjectName("nextButton");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(nextButton->sizePolicy().hasHeightForWidth());
        nextButton->setSizePolicy(sizePolicy2);

        verticalLayout_5->addWidget(nextButton);

        regButton = new QPushButton(verticalLayoutWidget_4);
        regButton->setObjectName("regButton");
        sizePolicy2.setHeightForWidth(regButton->sizePolicy().hasHeightForWidth());
        regButton->setSizePolicy(sizePolicy2);

        verticalLayout_5->addWidget(regButton);


        retranslateUi(AuthorizationWindow);

        QMetaObject::connectSlotsByName(AuthorizationWindow);
    } // setupUi

    void retranslateUi(QWidget *AuthorizationWindow)
    {
        AuthorizationWindow->setWindowTitle(QCoreApplication::translate("AuthorizationWindow", "Form", nullptr));
        label->setText(QCoreApplication::translate("AuthorizationWindow", "\320\220\320\222\320\242\320\236\320\240\320\230\320\227\320\220\320\246\320\230\320\257", nullptr));
        loginText->setText(QCoreApplication::translate("AuthorizationWindow", "\320\233\320\276\320\263\320\270\320\275", nullptr));
        label_4->setText(QCoreApplication::translate("AuthorizationWindow", "\320\237\320\260\321\200\320\276\320\273\321\214", nullptr));
        nextButton->setText(QCoreApplication::translate("AuthorizationWindow", "\320\237\321\200\320\276\320\264\320\276\320\273\320\266\320\270\321\202\321\214", nullptr));
        regButton->setText(QCoreApplication::translate("AuthorizationWindow", "\320\227\320\260\321\200\320\265\320\263\320\270\321\201\321\202\321\200\320\270\321\200\320\276\320\262\320\260\321\202\321\214\321\201\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AuthorizationWindow: public Ui_AuthorizationWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUTHORIZATIONWINDOW_H
