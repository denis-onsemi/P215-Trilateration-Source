/********************************************************************************
** Form generated from reading UI file 'uartuartcontroller.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UARTUARTCONTROLLER_H
#define UI_UARTUARTCONTROLLER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>
#include <customcontrols_checkbox.h>
#include "customcontrols.h"
#include "customcontrols_combobox.h"
#include "customcontrols_lineedit.h"

QT_BEGIN_NAMESPACE

class Ui_UartUartController
{
public:
    QGroupBox *groupBox;
    QLineEdit *lineEditWriteDataUART1;
    QLineEdit *lineEditReadDataUART1;
    QLabel *label_2;
    CustomControls::PushButton *pushButton_9;
    QLabel *label_3;
    QLineEdit *lineEditReadDataCountUART1;
    QLabel *label_10;
    QFrame *line;
    QLabel *label_22;
    QLabel *label_23;
    CustomControls::PushButton *pushButton_16;
    QFrame *line_5;
    QLabel *label_5;
    QLabel *label_6;
    CustomControls::PushButton *pushButton_10;
    QLineEdit *lineEditReadDataCountUART2;
    QLabel *label_20;
    QLabel *label_21;
    QLabel *label_13;
    QLabel *label_4;
    QLabel *label_8;
    QFrame *line_2;
    QLineEdit *lineEditWriteDataUART2;
    QLineEdit *lineEditReadDataUART2;
    QFrame *line_6;
    CustomControls::PushButton *pushButton_18;
    QLabel *label_15;
    QLabel *label_24;
    QFrame *line_7;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *label_7;
    QLineEdit *lineEditReadDataCountBRIDGE;
    QLineEdit *lineEditWriteDataBRIDGE;
    QLineEdit *lineEditReadDataBRIDGE;
    CustomControls::PushButton *pushButton_17;
    QGroupBox *groupBox_2;
    CustomControls::PushButton *pushButton_2;
    CustomControls::PushButton *pushButton;
    CustomControls::PushButton *pushButton_3;
    CustomControls::PushButton *pushButton_4;
    CustomControls::CheckBox *checkBox_22;
    CustomControls::CheckBox *checkBox_19;
    QFrame *line_4;
    QFrame *line_8;
    QGroupBox *groupBox_4;
    CustomControls::PushButton *pushButton_5;
    QFrame *line_10;
    QLabel *label_11;
    CustomControls::LineEdit *lineEdit_7;
    CustomControls::ComboBox *comboBox_4;
    CustomControls::PushButton *pushButton_7;
    CustomControls::PushButton *pushButton_6;
    QFrame *line_11;
    CustomControls::LineEdit *lineEdit_8;
    QLabel *label_12;
    QLabel *label_14;
    CustomControls::LineEdit *lineEdit_9;
    CustomControls::PushButton *pushButton_8;
    CustomControls::PushButton *pushButton_11;
    CustomControls::ComboBox *comboBox_5;
    CustomControls::ComboBox *comboBox_6;
    QGroupBox *groupBox_3;
    QLineEdit *lineEditPDCMdataCRC;
    QLineEdit *lineEditPDCMslot;
    QLabel *label_28;
    QLabel *label_29;
    CustomControls::PushButton *pushButton_19;
    QLineEdit *lineEditPDCMcrccorrect;
    QLineEdit *lineEditPDCMcrcres;
    QLabel *label_30;
    QLabel *label_31;

    void setupUi(QWidget *UartUartController)
    {
        if (UartUartController->objectName().isEmpty())
            UartUartController->setObjectName(QString::fromUtf8("UartUartController"));
        UartUartController->resize(946, 913);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(UartUartController->sizePolicy().hasHeightForWidth());
        UartUartController->setSizePolicy(sizePolicy);
        UartUartController->setStyleSheet(QString::fromUtf8("QTabWidget{\n"
"	background-color: rgb(255, 255, 255);\n"
"}\n"
"\n"
"QGroupBox::title {    \n"
"	background-color: rgb(233, 125, 46);\n"
"    border:2px solid rgb(233, 125, 46);\n"
"    border-radius:5px;\n"
"    color:#ffffff;\n"
"}\n"
"\n"
"QGroupBox{\n"
"	background-color: rgb(240, 240, 240);\n"
"    border: 2px solid rgb(233, 125, 46);\n"
"	border-radius:10px;\n"
"    font-weight: bold;\n"
"}\n"
"\n"
"QLineEdit:read-only{\n"
"	background-color: rgb(240, 240, 240);\n"
"}\n"
"\n"
"QCheckBox::indicator{\n"
"	border: 1px solid #5A5A5A;\n"
"	border-radius: 3px;\n"
"	background-color: rgb(255, 255, 255);\n"
"}\n"
"QCheckBox::indicator::checked{\n"
"	background-color: rgb(100, 100, 100);\n"
"}"));
        groupBox = new QGroupBox(UartUartController);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setEnabled(true);
        groupBox->setGeometry(QRect(20, 370, 901, 341));
        lineEditWriteDataUART1 = new QLineEdit(groupBox);
        lineEditWriteDataUART1->setObjectName(QString::fromUtf8("lineEditWriteDataUART1"));
        lineEditWriteDataUART1->setGeometry(QRect(120, 40, 311, 20));
        lineEditWriteDataUART1->setMinimumSize(QSize(150, 0));
        lineEditReadDataUART1 = new QLineEdit(groupBox);
        lineEditReadDataUART1->setObjectName(QString::fromUtf8("lineEditReadDataUART1"));
        lineEditReadDataUART1->setGeometry(QRect(470, 40, 361, 20));
        lineEditReadDataUART1->setMinimumSize(QSize(150, 0));
        lineEditReadDataUART1->setReadOnly(true);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(610, 10, 81, 31));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);
        pushButton_9 = new CustomControls::PushButton(groupBox);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(380, 70, 141, 23));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(840, 40, 41, 21));
        lineEditReadDataCountUART1 = new QLineEdit(groupBox);
        lineEditReadDataCountUART1->setObjectName(QString::fromUtf8("lineEditReadDataCountUART1"));
        lineEditReadDataCountUART1->setGeometry(QRect(120, 70, 50, 20));
        lineEditReadDataCountUART1->setMinimumSize(QSize(50, 0));
        lineEditReadDataCountUART1->setReadOnly(false);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(180, 70, 41, 21));
        line = new QFrame(groupBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(440, 40, 21, 20));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(10, 40, 101, 21));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_23 = new QLabel(groupBox);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(10, 70, 101, 21));
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton_16 = new CustomControls::PushButton(groupBox);
        pushButton_16->setObjectName(QString::fromUtf8("pushButton_16"));
        pushButton_16->setGeometry(QRect(250, 70, 111, 23));
        line_5 = new QFrame(groupBox);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        line_5->setGeometry(QRect(10, 100, 881, 21));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(370, 0, 161, 41));
        QFont font1;
        font1.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        font1.setPointSize(8);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        label_5->setFont(font1);
        label_5->setStyleSheet(QString::fromUtf8(""));
        label_5->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(370, 110, 161, 41));
        label_6->setFont(font1);
        label_6->setStyleSheet(QString::fromUtf8(""));
        label_6->setAlignment(Qt::AlignCenter);
        pushButton_10 = new CustomControls::PushButton(groupBox);
        pushButton_10->setObjectName(QString::fromUtf8("pushButton_10"));
        pushButton_10->setGeometry(QRect(380, 180, 141, 23));
        lineEditReadDataCountUART2 = new QLineEdit(groupBox);
        lineEditReadDataCountUART2->setObjectName(QString::fromUtf8("lineEditReadDataCountUART2"));
        lineEditReadDataCountUART2->setGeometry(QRect(120, 180, 50, 20));
        lineEditReadDataCountUART2->setMinimumSize(QSize(50, 0));
        lineEditReadDataCountUART2->setReadOnly(false);
        label_20 = new QLabel(groupBox);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(10, 150, 101, 21));
        label_20->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_21 = new QLabel(groupBox);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(10, 180, 101, 21));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_13 = new QLabel(groupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(180, 180, 41, 21));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(560, 120, 181, 31));
        label_4->setFont(font);
        label_4->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(840, 150, 41, 21));
        line_2 = new QFrame(groupBox);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(440, 150, 21, 21));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        lineEditWriteDataUART2 = new QLineEdit(groupBox);
        lineEditWriteDataUART2->setObjectName(QString::fromUtf8("lineEditWriteDataUART2"));
        lineEditWriteDataUART2->setGeometry(QRect(120, 150, 311, 20));
        lineEditWriteDataUART2->setMinimumSize(QSize(150, 0));
        lineEditReadDataUART2 = new QLineEdit(groupBox);
        lineEditReadDataUART2->setObjectName(QString::fromUtf8("lineEditReadDataUART2"));
        lineEditReadDataUART2->setGeometry(QRect(470, 150, 361, 20));
        lineEditReadDataUART2->setMinimumSize(QSize(150, 0));
        lineEditReadDataUART2->setReadOnly(true);
        line_6 = new QFrame(groupBox);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setGeometry(QRect(10, 210, 881, 21));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);
        pushButton_18 = new CustomControls::PushButton(groupBox);
        pushButton_18->setObjectName(QString::fromUtf8("pushButton_18"));
        pushButton_18->setGeometry(QRect(380, 290, 141, 23));
        label_15 = new QLabel(groupBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(620, 230, 61, 31));
        label_15->setFont(font);
        label_15->setAlignment(Qt::AlignCenter);
        label_24 = new QLabel(groupBox);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(10, 290, 101, 21));
        label_24->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        line_7 = new QFrame(groupBox);
        line_7->setObjectName(QString::fromUtf8("line_7"));
        line_7->setGeometry(QRect(440, 260, 21, 21));
        line_7->setFrameShape(QFrame::VLine);
        line_7->setFrameShadow(QFrame::Sunken);
        label_25 = new QLabel(groupBox);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(10, 260, 101, 21));
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_26 = new QLabel(groupBox);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(840, 260, 41, 21));
        label_27 = new QLabel(groupBox);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(180, 290, 41, 21));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(300, 220, 301, 41));
        label_7->setFont(font1);
        label_7->setStyleSheet(QString::fromUtf8(""));
        label_7->setAlignment(Qt::AlignCenter);
        lineEditReadDataCountBRIDGE = new QLineEdit(groupBox);
        lineEditReadDataCountBRIDGE->setObjectName(QString::fromUtf8("lineEditReadDataCountBRIDGE"));
        lineEditReadDataCountBRIDGE->setGeometry(QRect(120, 290, 50, 20));
        lineEditReadDataCountBRIDGE->setMinimumSize(QSize(50, 0));
        lineEditReadDataCountBRIDGE->setReadOnly(false);
        lineEditWriteDataBRIDGE = new QLineEdit(groupBox);
        lineEditWriteDataBRIDGE->setObjectName(QString::fromUtf8("lineEditWriteDataBRIDGE"));
        lineEditWriteDataBRIDGE->setGeometry(QRect(120, 260, 311, 20));
        lineEditWriteDataBRIDGE->setMinimumSize(QSize(150, 0));
        lineEditReadDataBRIDGE = new QLineEdit(groupBox);
        lineEditReadDataBRIDGE->setObjectName(QString::fromUtf8("lineEditReadDataBRIDGE"));
        lineEditReadDataBRIDGE->setGeometry(QRect(470, 260, 361, 20));
        lineEditReadDataBRIDGE->setMinimumSize(QSize(150, 0));
        lineEditReadDataBRIDGE->setReadOnly(true);
        pushButton_17 = new CustomControls::PushButton(groupBox);
        pushButton_17->setObjectName(QString::fromUtf8("pushButton_17"));
        pushButton_17->setGeometry(QRect(250, 180, 111, 23));
        groupBox_2 = new QGroupBox(UartUartController);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setEnabled(true);
        groupBox_2->setGeometry(QRect(20, 20, 901, 111));
        pushButton_2 = new CustomControls::PushButton(groupBox_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(20, 60, 121, 23));
        pushButton = new CustomControls::PushButton(groupBox_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setEnabled(true);
        pushButton->setGeometry(QRect(20, 30, 121, 23));
        pushButton_3 = new CustomControls::PushButton(groupBox_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(160, 40, 181, 31));
        pushButton_4 = new CustomControls::PushButton(groupBox_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(460, 40, 91, 31));
        checkBox_22 = new CustomControls::CheckBox(groupBox_2);
        checkBox_22->setObjectName(QString::fromUtf8("checkBox_22"));
        checkBox_22->setGeometry(QRect(380, 30, 71, 21));
        checkBox_19 = new CustomControls::CheckBox(groupBox_2);
        checkBox_19->setObjectName(QString::fromUtf8("checkBox_19"));
        checkBox_19->setGeometry(QRect(380, 60, 71, 21));
        line_4 = new QFrame(groupBox_2);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(350, 10, 21, 91));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);
        line_8 = new QFrame(groupBox_2);
        line_8->setObjectName(QString::fromUtf8("line_8"));
        line_8->setGeometry(QRect(570, 10, 21, 91));
        line_8->setFrameShape(QFrame::VLine);
        line_8->setFrameShadow(QFrame::Sunken);
        groupBox_4 = new QGroupBox(UartUartController);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setEnabled(true);
        groupBox_4->setGeometry(QRect(20, 150, 901, 201));
        pushButton_5 = new CustomControls::PushButton(groupBox_4);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(780, 30, 101, 21));
        line_10 = new QFrame(groupBox_4);
        line_10->setObjectName(QString::fromUtf8("line_10"));
        line_10->setGeometry(QRect(10, 50, 881, 31));
        line_10->setFrameShape(QFrame::HLine);
        line_10->setFrameShadow(QFrame::Sunken);
        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(30, 30, 151, 21));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_7 = new CustomControls::LineEdit(groupBox_4);
        lineEdit_7->setObjectName(QString::fromUtf8("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(190, 30, 41, 20));
        comboBox_4 = new CustomControls::ComboBox(groupBox_4);
        comboBox_4->setObjectName(QString::fromUtf8("comboBox_4"));
        comboBox_4->setGeometry(QRect(40, 80, 301, 20));
        QFont font2;
        font2.setFamily(QString::fromUtf8("MS Shell Dlg 2"));
        comboBox_4->setFont(font2);
        pushButton_7 = new CustomControls::PushButton(groupBox_4);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(780, 80, 101, 21));
        pushButton_6 = new CustomControls::PushButton(groupBox_4);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(670, 80, 101, 21));
        line_11 = new QFrame(groupBox_4);
        line_11->setObjectName(QString::fromUtf8("line_11"));
        line_11->setGeometry(QRect(10, 130, 881, 31));
        line_11->setFrameShape(QFrame::HLine);
        line_11->setFrameShadow(QFrame::Sunken);
        lineEdit_8 = new CustomControls::LineEdit(groupBox_4);
        lineEdit_8->setObjectName(QString::fromUtf8("lineEdit_8"));
        lineEdit_8->setGeometry(QRect(190, 160, 41, 20));
        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(30, 160, 151, 21));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(300, 160, 151, 21));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_9 = new CustomControls::LineEdit(groupBox_4);
        lineEdit_9->setObjectName(QString::fromUtf8("lineEdit_9"));
        lineEdit_9->setGeometry(QRect(460, 160, 41, 20));
        pushButton_8 = new CustomControls::PushButton(groupBox_4);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(780, 160, 101, 21));
        pushButton_11 = new CustomControls::PushButton(groupBox_4);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));
        pushButton_11->setGeometry(QRect(670, 160, 101, 21));
        comboBox_5 = new CustomControls::ComboBox(groupBox_4);
        comboBox_5->setObjectName(QString::fromUtf8("comboBox_5"));
        comboBox_5->setGeometry(QRect(420, 80, 181, 20));
        comboBox_5->setFont(font2);
        comboBox_6 = new CustomControls::ComboBox(groupBox_4);
        comboBox_6->setObjectName(QString::fromUtf8("comboBox_6"));
        comboBox_6->setGeometry(QRect(40, 110, 301, 20));
        comboBox_6->setFont(font2);
        groupBox_3 = new QGroupBox(UartUartController);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setEnabled(true);
        groupBox_3->setGeometry(QRect(20, 730, 901, 111));
        lineEditPDCMdataCRC = new QLineEdit(groupBox_3);
        lineEditPDCMdataCRC->setObjectName(QString::fromUtf8("lineEditPDCMdataCRC"));
        lineEditPDCMdataCRC->setGeometry(QRect(180, 30, 401, 20));
        lineEditPDCMdataCRC->setMinimumSize(QSize(150, 0));
        lineEditPDCMslot = new QLineEdit(groupBox_3);
        lineEditPDCMslot->setObjectName(QString::fromUtf8("lineEditPDCMslot"));
        lineEditPDCMslot->setGeometry(QRect(180, 60, 50, 20));
        lineEditPDCMslot->setMinimumSize(QSize(50, 0));
        lineEditPDCMslot->setReadOnly(false);
        label_28 = new QLabel(groupBox_3);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(10, 30, 161, 21));
        label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_29 = new QLabel(groupBox_3);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(70, 60, 101, 21));
        label_29->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton_19 = new CustomControls::PushButton(groupBox_3);
        pushButton_19->setObjectName(QString::fromUtf8("pushButton_19"));
        pushButton_19->setGeometry(QRect(440, 60, 141, 23));
        lineEditPDCMcrccorrect = new QLineEdit(groupBox_3);
        lineEditPDCMcrccorrect->setObjectName(QString::fromUtf8("lineEditPDCMcrccorrect"));
        lineEditPDCMcrccorrect->setGeometry(QRect(790, 50, 50, 20));
        lineEditPDCMcrccorrect->setMinimumSize(QSize(50, 0));
        lineEditPDCMcrccorrect->setReadOnly(false);
        lineEditPDCMcrcres = new QLineEdit(groupBox_3);
        lineEditPDCMcrcres->setObjectName(QString::fromUtf8("lineEditPDCMcrcres"));
        lineEditPDCMcrcres->setGeometry(QRect(790, 20, 50, 20));
        lineEditPDCMcrcres->setMinimumSize(QSize(50, 0));
        lineEditPDCMcrcres->setReadOnly(false);
        label_30 = new QLabel(groupBox_3);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(680, 20, 101, 21));
        label_30->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_31 = new QLabel(groupBox_3);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(680, 50, 101, 21));
        label_31->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        retranslateUi(UartUartController);
    } // setupUi

    void retranslateUi(QWidget *UartUartController)
    {
        UartUartController->setWindowTitle(QApplication::translate("UartUartController", "UartController2", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("UartUartController", "Master commands", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditWriteDataUART1->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditWriteDataUART1->setInputMask(QString());
        lineEditWriteDataUART1->setText(QApplication::translate("UartUartController", "00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditReadDataUART1->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditReadDataUART1->setInputMask(QString());
        lineEditReadDataUART1->setText(QString());
        label_2->setText(QApplication::translate("UartUartController", "READ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_9->setAccessibleName(QApplication::translate("UartUartController", "WriteReadUART1()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_9->setText(QApplication::translate("UartUartController", "Write and Read", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("UartUartController", "hex", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditReadDataCountUART1->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditReadDataCountUART1->setInputMask(QString());
        lineEditReadDataCountUART1->setText(QApplication::translate("UartUartController", "00", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("UartUartController", "dec", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("UartUartController", "WRITE DATA", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("UartUartController", "BYTES to READ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_16->setAccessibleName(QApplication::translate("UartUartController", "AddUART1CRC()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_16->setText(QApplication::translate("UartUartController", "Add UART CRC", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("UartUartController", "UART1  COMMAND", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("UartUartController", "UART2  COMMAND", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_10->setAccessibleName(QApplication::translate("UartUartController", "WriteReadUART2()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_10->setText(QApplication::translate("UartUartController", "Write and Read", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditReadDataCountUART2->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditReadDataCountUART2->setInputMask(QString());
        lineEditReadDataCountUART2->setText(QApplication::translate("UartUartController", "00", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("UartUartController", "WRITE DATA", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("UartUartController", "BYTES to READ", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("UartUartController", "dec", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("UartUartController", "READ", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("UartUartController", "hex", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditWriteDataUART2->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditWriteDataUART2->setInputMask(QString());
        lineEditWriteDataUART2->setText(QApplication::translate("UartUartController", "00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditReadDataUART2->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditReadDataUART2->setInputMask(QString());
        lineEditReadDataUART2->setText(QString());
#ifndef QT_NO_ACCESSIBILITY
        pushButton_18->setAccessibleName(QApplication::translate("UartUartController", "WriteReadBRIDGE()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_18->setText(QApplication::translate("UartUartController", "Write and Read", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("UartUartController", "READ", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("UartUartController", "BYTES to READ", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("UartUartController", "WRITE DATA", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("UartUartController", "hex", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("UartUartController", "dec", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("UartUartController", "RAW  UART-UART bridge COMMAND", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditReadDataCountBRIDGE->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditReadDataCountBRIDGE->setInputMask(QString());
        lineEditReadDataCountBRIDGE->setText(QApplication::translate("UartUartController", "00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditWriteDataBRIDGE->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditWriteDataBRIDGE->setInputMask(QString());
        lineEditWriteDataBRIDGE->setText(QApplication::translate("UartUartController", "00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditReadDataBRIDGE->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditReadDataBRIDGE->setInputMask(QString());
        lineEditReadDataBRIDGE->setText(QString());
#ifndef QT_NO_ACCESSIBILITY
        pushButton_17->setAccessibleName(QApplication::translate("UartUartController", "AddUART2CRC()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_17->setText(QApplication::translate("UartUartController", "Add UART CRC", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("UartUartController", "MDSEL and NINTx signals control", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_2->setAccessibleName(QApplication::translate("UartUartController", "SetMDSELlow()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_2->setText(QApplication::translate("UartUartController", "Set MDSEL low", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton->setAccessibleName(QApplication::translate("UartUartController", "SetMDSELhigh()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton->setText(QApplication::translate("UartUartController", "Set MDSEL high", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_3->setAccessibleName(QApplication::translate("UartUartController", "SetMDSELpulse()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_3->setText(QApplication::translate("UartUartController", "RESET (negative MDSEL pulse)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_4->setAccessibleName(QApplication::translate("UartUartController", "GetNINTx()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_4->setText(QApplication::translate("UartUartController", "Get NINTx", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_22->setAccessibleName(QApplication::translate("UartUartController", "MReadNINT[NINT1]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_22->setText(QApplication::translate("UartUartController", "NINT1", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_19->setAccessibleName(QApplication::translate("UartUartController", "MReadNINT[NINT2]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_19->setText(QApplication::translate("UartUartController", "NINT2", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("UartUartController", "UART-UART bridge - Config memory", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_5->setAccessibleName(QApplication::translate("UartUartController", "ReadReg00()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_5->setText(QApplication::translate("UartUartController", "Read reg 0", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("UartUartController", "FPGA version", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_7->setAccessibleName(QApplication::translate("UartUartController", "UREG00[FPGAversion]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        comboBox_4->clear();
        comboBox_4->insertItems(0, QStringList()
         << QApplication::translate("UartUartController", "J_UART <-> DSI3 master", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("UartUartController", "UART2UART bridge <-> DSI3 master", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_ACCESSIBILITY
        comboBox_4->setAccessibleName(QApplication::translate("UartUartController", "UREG01[UART_INTERNAL]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        pushButton_7->setAccessibleName(QApplication::translate("UartUartController", "ReadReg01()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_7->setText(QApplication::translate("UartUartController", "Read reg 1", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_6->setAccessibleName(QApplication::translate("UartUartController", "WriteReg01()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_6->setText(QApplication::translate("UartUartController", "Write reg 1", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_8->setAccessibleName(QApplication::translate("UartUartController", "UREGANY[DATA]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_12->setText(QApplication::translate("UartUartController", "ANY reg DATA (8b)", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("UartUartController", "ANY reg ADDR (8b)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_9->setAccessibleName(QApplication::translate("UartUartController", "UREGANY[ADDR]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        pushButton_8->setAccessibleName(QApplication::translate("UartUartController", "ReadANYReg()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_8->setText(QApplication::translate("UartUartController", "Read ANY reg", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_11->setAccessibleName(QApplication::translate("UartUartController", "WriteANYReg()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_11->setText(QApplication::translate("UartUartController", "Write ANY reg", 0, QApplication::UnicodeUTF8));
        comboBox_5->clear();
        comboBox_5->insertItems(0, QStringList()
         << QApplication::translate("UartUartController", "UART1 on PDM7/8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("UartUartController", "UART2 on PDM7/8", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_ACCESSIBILITY
        comboBox_5->setAccessibleName(QApplication::translate("UartUartController", "UREG01[PDM_SEL]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        comboBox_6->clear();
        comboBox_6->insertItems(0, QStringList()
         << QApplication::translate("UartUartController", "UART2UART bridge <-> J_UART  disabled", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("UartUartController", "UART2UART bridge <-> J_UART  enabled", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_ACCESSIBILITY
        comboBox_6->setAccessibleName(QApplication::translate("UartUartController", "UREG01[UART_BRIDGE]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        groupBox_3->setTitle(QApplication::translate("UartUartController", "PDCM CRC test 8b", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditPDCMdataCRC->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditPDCMdataCRC->setInputMask(QString());
        lineEditPDCMdataCRC->setText(QApplication::translate("UartUartController", "00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditPDCMslot->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditPDCMslot->setInputMask(QString());
        lineEditPDCMslot->setText(QApplication::translate("UartUartController", "1", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("UartUartController", "PDCM data, incl CRC", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("UartUartController", "PDCM slot", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_19->setAccessibleName(QApplication::translate("UartUartController", "CheckCRC()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_19->setText(QApplication::translate("UartUartController", "Check", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditPDCMcrccorrect->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditPDCMcrccorrect->setInputMask(QString());
        lineEditPDCMcrccorrect->setText(QApplication::translate("UartUartController", "1", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditPDCMcrcres->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        lineEditPDCMcrcres->setInputMask(QString());
        lineEditPDCMcrcres->setText(QApplication::translate("UartUartController", "1", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("UartUartController", "res", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("UartUartController", "Correct CRC", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UartUartController: public Ui_UartUartController {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UARTUARTCONTROLLER_H
