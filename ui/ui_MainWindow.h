/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *acLoadConfig;
    QAction *acAutoU3D;
    QWidget *centralwidget;
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QLabel *label_10;
    QVBoxLayout *verticalLayout_4;
    QLineEdit *leTemplate;
    QLineEdit *leUserInput;
    QLineEdit *leDefaultInput;
    QLineEdit *leInputFlags;
    QLineEdit *leEnabledPages;
    QLineEdit *leEnabledModels;
    QLineEdit *leEnabledViews;
    QLineEdit *leMeshLabServer;
    QGroupBox *groupBox_4;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_3;
    QLabel *label_4;
    QVBoxLayout *verticalLayout_3;
    QLineEdit *leOutputDir;
    QLineEdit *leReportName;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_12;
    QPushButton *pbShowModels;
    QPushButton *pbGenerateViews;
    QHBoxLayout *horizontalLayout;
    QPushButton *pbGenerateData;
    QPushButton *pbGeneratePDF;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout;
    QTextBrowser *tbLog;
    QProgressBar *progressBar;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(478, 707);
        acLoadConfig = new QAction(MainWindow);
        acLoadConfig->setObjectName(QStringLiteral("acLoadConfig"));
        acAutoU3D = new QAction(MainWindow);
        acAutoU3D->setObjectName(QStringLiteral("acAutoU3D"));
        acAutoU3D->setCheckable(true);
        acAutoU3D->setChecked(true);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        gridLayout_2 = new QGridLayout(centralwidget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout->addWidget(label_5);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout->addWidget(label_6);

        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        verticalLayout->addWidget(label_7);

        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QStringLiteral("label_8"));

        verticalLayout->addWidget(label_8);

        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QStringLiteral("label_9"));

        verticalLayout->addWidget(label_9);

        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QStringLiteral("label_10"));

        verticalLayout->addWidget(label_10);


        horizontalLayout_2->addLayout(verticalLayout);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        leTemplate = new QLineEdit(groupBox_2);
        leTemplate->setObjectName(QStringLiteral("leTemplate"));
        leTemplate->setReadOnly(true);

        verticalLayout_4->addWidget(leTemplate);

        leUserInput = new QLineEdit(groupBox_2);
        leUserInput->setObjectName(QStringLiteral("leUserInput"));
        leUserInput->setReadOnly(true);

        verticalLayout_4->addWidget(leUserInput);

        leDefaultInput = new QLineEdit(groupBox_2);
        leDefaultInput->setObjectName(QStringLiteral("leDefaultInput"));
        leDefaultInput->setReadOnly(true);

        verticalLayout_4->addWidget(leDefaultInput);

        leInputFlags = new QLineEdit(groupBox_2);
        leInputFlags->setObjectName(QStringLiteral("leInputFlags"));
        leInputFlags->setReadOnly(true);

        verticalLayout_4->addWidget(leInputFlags);

        leEnabledPages = new QLineEdit(groupBox_2);
        leEnabledPages->setObjectName(QStringLiteral("leEnabledPages"));
        leEnabledPages->setReadOnly(true);

        verticalLayout_4->addWidget(leEnabledPages);

        leEnabledModels = new QLineEdit(groupBox_2);
        leEnabledModels->setObjectName(QStringLiteral("leEnabledModels"));
        leEnabledModels->setReadOnly(true);

        verticalLayout_4->addWidget(leEnabledModels);

        leEnabledViews = new QLineEdit(groupBox_2);
        leEnabledViews->setObjectName(QStringLiteral("leEnabledViews"));
        leEnabledViews->setReadOnly(true);

        verticalLayout_4->addWidget(leEnabledViews);

        leMeshLabServer = new QLineEdit(groupBox_2);
        leMeshLabServer->setObjectName(QStringLiteral("leMeshLabServer"));
        leMeshLabServer->setReadOnly(true);

        verticalLayout_4->addWidget(leMeshLabServer);


        horizontalLayout_2->addLayout(verticalLayout_4);


        gridLayout_2->addWidget(groupBox_2, 0, 0, 1, 1);

        groupBox_4 = new QGroupBox(centralwidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        horizontalLayout_4 = new QHBoxLayout(groupBox_4);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_2->addWidget(label_3);

        label_4 = new QLabel(groupBox_4);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_2->addWidget(label_4);


        horizontalLayout_4->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        leOutputDir = new QLineEdit(groupBox_4);
        leOutputDir->setObjectName(QStringLiteral("leOutputDir"));
        leOutputDir->setReadOnly(true);

        verticalLayout_3->addWidget(leOutputDir);

        leReportName = new QLineEdit(groupBox_4);
        leReportName->setObjectName(QStringLiteral("leReportName"));

        verticalLayout_3->addWidget(leReportName);


        horizontalLayout_4->addLayout(verticalLayout_3);


        gridLayout_2->addWidget(groupBox_4, 1, 0, 1, 1);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_12 = new QHBoxLayout(groupBox);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        pbShowModels = new QPushButton(groupBox);
        pbShowModels->setObjectName(QStringLiteral("pbShowModels"));

        horizontalLayout_12->addWidget(pbShowModels);

        pbGenerateViews = new QPushButton(groupBox);
        pbGenerateViews->setObjectName(QStringLiteral("pbGenerateViews"));

        horizontalLayout_12->addWidget(pbGenerateViews);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pbGenerateData = new QPushButton(groupBox);
        pbGenerateData->setObjectName(QStringLiteral("pbGenerateData"));

        horizontalLayout->addWidget(pbGenerateData);


        horizontalLayout_12->addLayout(horizontalLayout);

        pbGeneratePDF = new QPushButton(groupBox);
        pbGeneratePDF->setObjectName(QStringLiteral("pbGeneratePDF"));

        horizontalLayout_12->addWidget(pbGeneratePDF);


        gridLayout_2->addWidget(groupBox, 2, 0, 1, 1);

        groupBox_3 = new QGroupBox(centralwidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        gridLayout = new QGridLayout(groupBox_3);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        tbLog = new QTextBrowser(groupBox_3);
        tbLog->setObjectName(QStringLiteral("tbLog"));

        gridLayout->addWidget(tbLog, 0, 0, 1, 1);


        gridLayout_2->addWidget(groupBox_3, 3, 0, 1, 1);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        gridLayout_2->addWidget(progressBar, 4, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 478, 26));
        menu = new QMenu(menubar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menu->addAction(acLoadConfig);
        menu_2->addAction(acAutoU3D);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        acLoadConfig->setText(QApplication::translate("MainWindow", "\350\275\275\345\205\245\351\205\215\347\275\256\346\226\207\344\273\266", Q_NULLPTR));
        acAutoU3D->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\350\275\254\346\215\242PLY\345\210\260U3D", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\350\276\223\345\205\245\344\277\241\346\201\257", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "\346\250\241\346\235\277\346\226\207\344\273\266\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "\347\224\250\346\210\267\350\276\223\345\205\245\357\274\232", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "\351\273\230\350\256\244\350\276\223\345\205\245\357\274\232", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "\345\257\271\345\272\224\350\247\204\345\210\231\357\274\232", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "\345\220\257\347\224\250\347\232\204\351\241\265\347\240\201\357\274\232", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "\345\220\257\347\224\250\347\232\204\346\250\241\345\236\213\357\274\232", Q_NULLPTR));
        label_9->setText(QApplication::translate("MainWindow", "\345\220\257\347\224\250\347\232\204\350\247\206\345\233\276\357\274\232", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "MeshLabServer\357\274\232", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "\350\276\223\345\207\272\344\277\241\346\201\257", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "\345\267\245\344\275\234\347\233\256\345\275\225\357\274\232", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "\346\212\245\345\221\212\345\220\215\347\247\260\357\274\232", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("MainWindow", "\345\212\237\350\203\275\345\214\272", Q_NULLPTR));
        pbShowModels->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\2723D\346\250\241\345\236\213", Q_NULLPTR));
        pbGenerateViews->setText(QApplication::translate("MainWindow", "\344\273\205\347\224\237\346\210\220\350\247\206\345\233\276", Q_NULLPTR));
        pbGenerateData->setText(QApplication::translate("MainWindow", "\344\273\205\347\224\237\346\210\2203D\346\225\260\346\215\256", Q_NULLPTR));
        pbGeneratePDF->setText(QApplication::translate("MainWindow", "\344\270\200\351\224\256\347\224\237\346\210\220PDF\346\212\245\345\221\212", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\346\227\245\345\277\227\346\240\217", Q_NULLPTR));
        menu->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("MainWindow", "\350\256\276\347\275\256", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
