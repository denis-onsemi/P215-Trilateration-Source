/********************************************************************************
** Form generated from reading UI file 'graph.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRAPH_H
#define UI_GRAPH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_Graph
{
public:
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_5;
    QLabel *labelName;
    QSpacerItem *verticalSpacer_4;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QLabel *label_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QCustomPlot *ptGraph;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QWidget *Graph)
    {
        if (Graph->objectName().isEmpty())
            Graph->setObjectName(QString::fromUtf8("Graph"));
        Graph->resize(1574, 1120);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Graph->sizePolicy().hasHeightForWidth());
        Graph->setSizePolicy(sizePolicy);
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        palette.setBrush(QPalette::Active, QPalette::Window, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
        Graph->setPalette(palette);
        Graph->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        verticalLayout_2 = new QVBoxLayout(Graph);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalSpacer_5 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_2->addItem(verticalSpacer_5);

        labelName = new QLabel(Graph);
        labelName->setObjectName(QString::fromUtf8("labelName"));
        labelName->setMaximumSize(QSize(16777215, 200));
        QFont font;
        font.setFamily(QString::fromUtf8("Inter"));
        font.setPointSize(36);
        font.setBold(true);
        font.setWeight(75);
        labelName->setFont(font);
        labelName->setStyleSheet(QString::fromUtf8("color: rgb(233, 125, 46);"));
        labelName->setTextFormat(Qt::AutoText);
        labelName->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(labelName);

        verticalSpacer_4 = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_2->addItem(verticalSpacer_4);

        label_3 = new QLabel(Graph);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMaximumSize(QSize(16777215, 50));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Inter"));
        font1.setPointSize(16);
        font1.setBold(true);
        font1.setWeight(75);
        label_3->setFont(font1);
        label_3->setStyleSheet(QString::fromUtf8(""));
        label_3->setTextFormat(Qt::AutoText);
        label_3->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        verticalLayout_2->addWidget(label_3);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        pushButton = new QPushButton(Graph);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(400, 120));
        pushButton->setMaximumSize(QSize(400, 120));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Inter"));
        font2.setPointSize(48);
        font2.setBold(true);
        font2.setWeight(75);
        pushButton->setFont(font2);
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton->setFlat(false);

        horizontalLayout->addWidget(pushButton);

        horizontalSpacer_2 = new QSpacerItem(50, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 100, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout_2->addItem(verticalSpacer);

        label_2 = new QLabel(Graph);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMaximumSize(QSize(16777215, 50));
        label_2->setFont(font1);
        label_2->setStyleSheet(QString::fromUtf8(""));
        label_2->setTextFormat(Qt::AutoText);
        label_2->setAlignment(Qt::AlignBottom|Qt::AlignHCenter);

        verticalLayout_2->addWidget(label_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(50, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);

        ptGraph = new QCustomPlot(Graph);
        ptGraph->setObjectName(QString::fromUtf8("ptGraph"));
        ptGraph->setMinimumSize(QSize(450, 350));
        ptGraph->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_2->addWidget(ptGraph);

        horizontalSpacer_4 = new QSpacerItem(50, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_4);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_3 = new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout->addItem(verticalSpacer_3);


        verticalLayout_2->addLayout(verticalLayout);

        labelName->raise();
        label_2->raise();
        label_3->raise();

        retranslateUi(Graph);
    } // setupUi

    void retranslateUi(QWidget *Graph)
    {
        Graph->setWindowTitle(QApplication::translate("Graph", "Graph", 0, QApplication::UnicodeUTF8));
        labelName->setText(QApplication::translate("Graph", "ULTRASONIC VIRTUAL BUTTON", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("Graph", "Status:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("Graph", "OFF", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Graph", "Phase measurement:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Graph: public Ui_Graph {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRAPH_H
