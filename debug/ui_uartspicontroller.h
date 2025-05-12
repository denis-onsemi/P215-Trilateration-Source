/********************************************************************************
** Form generated from reading UI file 'uartspicontroller.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UARTSPICONTROLLER_H
#define UI_UARTSPICONTROLLER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include <customcontrols_checkbox.h>
#include "customcontrols.h"
#include "customcontrols_combobox.h"
#include "customcontrols_lineedit.h"

QT_BEGIN_NAMESPACE

class Ui_UartSpiController
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox_4;
    CustomControls::PushButton *pushButtonWriteRead_2;
    CustomControls::CheckBox *checkBox_28;
    CustomControls::CheckBox *checkBox_29;
    CustomControls::CheckBox *checkBox_30;
    CustomControls::CheckBox *checkBox_31;
    CustomControls::CheckBox *checkBox_32;
    CustomControls::CheckBox *checkBox_33;
    CustomControls::CheckBox *checkBox_34;
    CustomControls::CheckBox *checkBox_35;
    CustomControls::CheckBox *checkBox_36;
    CustomControls::CheckBox *checkBox_37;
    CustomControls::CheckBox *checkBox_38;
    CustomControls::CheckBox *checkBox_39;
    QGroupBox *groupBox_3;
    QLineEdit *lineEditBRCReadData;
    CustomControls::PushButton *pushButtonSPIWriteRead_2;
    QLabel *label_23;
    QLabel *label_24;
    CustomControls::LineEdit *lineEditBRC;
    QLabel *label_25;
    QLabel *label_26;
    QLabel *label_27;
    QLabel *label_28;
    QLineEdit *lineEditBRCReadDataCount;
    QLabel *label_29;
    QLabel *label_30;
    CustomControls::LineEdit *lineEditBRC_2;
    CustomControls::LineEdit *lineEditBRC_3;
    CustomControls::LineEdit *lineEditBRC_4;
    QLabel *label_31;
    QLabel *label_33;
    QLabel *label_35;
    QLabel *label_34;
    QLabel *label_36;
    CustomControls::LineEdit *lineEditBRC_6;
    QLabel *label_37;
    QLabel *label_38;
    QLabel *label_59;
    CustomControls::LineEdit *lineEditBRC_7;
    QLabel *label_60;
    CustomControls::ComboBox *comboBoxS1R1_3;
    QGroupBox *groupBox_2;
    QLineEdit *lineEditSPIWriteData;
    QLineEdit *lineEditSPIReadData;
    QLabel *label_11;
    CustomControls::PushButton *pushButtonSPIWriteRead;
    QLabel *label_12;
    QLabel *label_13;
    QLineEdit *lineEditSPIWriteDataCount;
    QLabel *label_15;
    QLabel *label_16;
    QLineEdit *lineEditSPIReadDataCount;
    QLabel *label_20;
    CustomControls::PushButton *pushButtonSPICRC;
    CustomControls::LineEdit *lineEditStatus;
    CustomControls::LineEdit *lineEditStatus_2;
    QLabel *label_39;
    QLabel *label_40;
    QLabel *label_41;
    CustomControls::LineEdit *lineEditStatus_7;
    QLabel *label_42;
    CustomControls::LineEdit *lineEditStatus_8;
    QLabel *label_43;
    QLabel *label_44;
    CustomControls::LineEdit *lineEditStatus_9;
    CustomControls::LineEdit *lineEditStatus_10;
    QLabel *label_21;
    QLabel *label_22;
    QLabel *label_45;
    QLabel *label_46;
    QLabel *label_47;
    QLabel *label_48;
    QLabel *label_49;
    CustomControls::LineEdit *lineEditStatus_11;
    CustomControls::LineEdit *lineEditStatus_12;
    QLabel *label_52;
    CustomControls::CheckBox *checkBox_22;
    CustomControls::CheckBox *checkBox_23;
    CustomControls::CheckBox *checkBox_24;
    CustomControls::CheckBox *checkBox_25;
    CustomControls::CheckBox *checkBox_26;
    CustomControls::CheckBox *checkBox_27;
    QFrame *line_2;
    QFrame *line_3;
    QLabel *label_14;
    QLabel *label_51;
    QLabel *label_53;
    QLabel *label_50;
    QLabel *label_54;
    QLabel *label_55;
    QLabel *label_56;
    QLabel *label_57;
    QLabel *label_58;
    QGroupBox *groupBox;
    QLineEdit *lineEditWriteData;
    QLineEdit *lineEditReadData;
    QLabel *label_2;
    CustomControls::PushButton *pushButtonWriteRead;
    QLabel *label;
    CustomControls::PushButton *pushButtonWriteOnly;
    QLabel *label_3;
    QLineEdit *lineEditWriteDataCount;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *lineEditReadDataCount;
    QLabel *label_10;
    CustomControls::PushButton *pushButtonWriteOnly_2;
    QFrame *line;
    QWidget *tab_2;
    QGroupBox *groupBox_5;
    CustomControls::LineEdit *lineEdit_21;
    QLabel *label_73;
    QLabel *label_74;
    CustomControls::PushButton *pushButton_22;
    CustomControls::PushButton *pushButton_23;
    CustomControls::LineEdit *lineEdit_27;
    QLabel *label_77;
    QLabel *label_78;
    QFrame *line_4;
    QFrame *line_9;
    QFrame *line_10;
    CustomControls::ComboBox *comboBoxS1R1_4;
    CustomControls::PushButton *pushButton_24;
    CustomControls::PushButton *pushButton_25;
    QLabel *label_75;
    QLabel *label_76;
    QLabel *label_79;
    CustomControls::ComboBox *comboBoxS1R1_5;
    CustomControls::ComboBox *comboBoxS1R1_6;

    void setupUi(QWidget *UartSpiController)
    {
        if (UartSpiController->objectName().isEmpty())
            UartSpiController->setObjectName(QString::fromUtf8("UartSpiController"));
        UartSpiController->resize(810, 1117);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(UartSpiController->sizePolicy().hasHeightForWidth());
        UartSpiController->setSizePolicy(sizePolicy);
        UartSpiController->setStyleSheet(QString::fromUtf8("QTabWidget{\n"
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
        tabWidget = new QTabWidget(UartSpiController);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 10, 791, 1091));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        groupBox_4 = new QGroupBox(tab);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 410, 761, 111));
        pushButtonWriteRead_2 = new CustomControls::PushButton(groupBox_4);
        pushButtonWriteRead_2->setObjectName(QString::fromUtf8("pushButtonWriteRead_2"));
        pushButtonWriteRead_2->setGeometry(QRect(310, 70, 141, 23));
        checkBox_28 = new CustomControls::CheckBox(groupBox_4);
        checkBox_28->setObjectName(QString::fromUtf8("checkBox_28"));
        checkBox_28->setGeometry(QRect(20, 20, 121, 20));
        checkBox_29 = new CustomControls::CheckBox(groupBox_4);
        checkBox_29->setObjectName(QString::fromUtf8("checkBox_29"));
        checkBox_29->setGeometry(QRect(20, 40, 121, 20));
        checkBox_30 = new CustomControls::CheckBox(groupBox_4);
        checkBox_30->setObjectName(QString::fromUtf8("checkBox_30"));
        checkBox_30->setGeometry(QRect(260, 20, 121, 20));
        checkBox_31 = new CustomControls::CheckBox(groupBox_4);
        checkBox_31->setObjectName(QString::fromUtf8("checkBox_31"));
        checkBox_31->setGeometry(QRect(140, 20, 121, 20));
        checkBox_32 = new CustomControls::CheckBox(groupBox_4);
        checkBox_32->setObjectName(QString::fromUtf8("checkBox_32"));
        checkBox_32->setGeometry(QRect(140, 40, 121, 20));
        checkBox_33 = new CustomControls::CheckBox(groupBox_4);
        checkBox_33->setObjectName(QString::fromUtf8("checkBox_33"));
        checkBox_33->setGeometry(QRect(260, 40, 121, 20));
        checkBox_34 = new CustomControls::CheckBox(groupBox_4);
        checkBox_34->setObjectName(QString::fromUtf8("checkBox_34"));
        checkBox_34->setGeometry(QRect(530, 20, 121, 20));
        checkBox_35 = new CustomControls::CheckBox(groupBox_4);
        checkBox_35->setObjectName(QString::fromUtf8("checkBox_35"));
        checkBox_35->setGeometry(QRect(410, 40, 121, 20));
        checkBox_36 = new CustomControls::CheckBox(groupBox_4);
        checkBox_36->setObjectName(QString::fromUtf8("checkBox_36"));
        checkBox_36->setGeometry(QRect(530, 40, 121, 20));
        checkBox_37 = new CustomControls::CheckBox(groupBox_4);
        checkBox_37->setObjectName(QString::fromUtf8("checkBox_37"));
        checkBox_37->setGeometry(QRect(650, 20, 111, 20));
        checkBox_38 = new CustomControls::CheckBox(groupBox_4);
        checkBox_38->setObjectName(QString::fromUtf8("checkBox_38"));
        checkBox_38->setGeometry(QRect(410, 20, 121, 20));
        checkBox_39 = new CustomControls::CheckBox(groupBox_4);
        checkBox_39->setObjectName(QString::fromUtf8("checkBox_39"));
        checkBox_39->setGeometry(QRect(650, 40, 111, 20));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 530, 761, 171));
        lineEditBRCReadData = new QLineEdit(groupBox_3);
        lineEditBRCReadData->setObjectName(QString::fromUtf8("lineEditBRCReadData"));
        lineEditBRCReadData->setGeometry(QRect(110, 110, 481, 20));
        lineEditBRCReadData->setMinimumSize(QSize(150, 0));
        lineEditBRCReadData->setReadOnly(true);
        pushButtonSPIWriteRead_2 = new CustomControls::PushButton(groupBox_3);
        pushButtonSPIWriteRead_2->setObjectName(QString::fromUtf8("pushButtonSPIWriteRead_2"));
        pushButtonSPIWriteRead_2->setGeometry(QRect(600, 50, 141, 31));
        label_23 = new QLabel(groupBox_3);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setGeometry(QRect(500, 50, 91, 21));
        label_24 = new QLabel(groupBox_3);
        label_24->setObjectName(QString::fromUtf8("label_24"));
        label_24->setGeometry(QRect(600, 110, 41, 21));
        lineEditBRC = new CustomControls::LineEdit(groupBox_3);
        lineEditBRC->setObjectName(QString::fromUtf8("lineEditBRC"));
        lineEditBRC->setGeometry(QRect(440, 50, 50, 20));
        lineEditBRC->setMinimumSize(QSize(50, 0));
        label_25 = new QLabel(groupBox_3);
        label_25->setObjectName(QString::fromUtf8("label_25"));
        label_25->setGeometry(QRect(30, 20, 81, 21));
        label_25->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_26 = new QLabel(groupBox_3);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(220, 50, 211, 21));
        label_26->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_27 = new QLabel(groupBox_3);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        label_27->setGeometry(QRect(0, 110, 101, 21));
        label_27->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_28 = new QLabel(groupBox_3);
        label_28->setObjectName(QString::fromUtf8("label_28"));
        label_28->setGeometry(QRect(0, 130, 101, 21));
        label_28->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditBRCReadDataCount = new QLineEdit(groupBox_3);
        lineEditBRCReadDataCount->setObjectName(QString::fromUtf8("lineEditBRCReadDataCount"));
        lineEditBRCReadDataCount->setGeometry(QRect(110, 130, 50, 20));
        lineEditBRCReadDataCount->setMinimumSize(QSize(50, 0));
        lineEditBRCReadDataCount->setReadOnly(true);
        label_29 = new QLabel(groupBox_3);
        label_29->setObjectName(QString::fromUtf8("label_29"));
        label_29->setGeometry(QRect(500, 20, 61, 21));
        label_30 = new QLabel(groupBox_3);
        label_30->setObjectName(QString::fromUtf8("label_30"));
        label_30->setGeometry(QRect(170, 130, 41, 21));
        lineEditBRC_2 = new CustomControls::LineEdit(groupBox_3);
        lineEditBRC_2->setObjectName(QString::fromUtf8("lineEditBRC_2"));
        lineEditBRC_2->setGeometry(QRect(120, 80, 50, 20));
        lineEditBRC_2->setMinimumSize(QSize(50, 0));
        lineEditBRC_3 = new CustomControls::LineEdit(groupBox_3);
        lineEditBRC_3->setObjectName(QString::fromUtf8("lineEditBRC_3"));
        lineEditBRC_3->setGeometry(QRect(440, 20, 50, 20));
        lineEditBRC_3->setMinimumSize(QSize(50, 0));
        lineEditBRC_4 = new CustomControls::LineEdit(groupBox_3);
        lineEditBRC_4->setObjectName(QString::fromUtf8("lineEditBRC_4"));
        lineEditBRC_4->setGeometry(QRect(120, 50, 50, 20));
        lineEditBRC_4->setMinimumSize(QSize(50, 0));
        label_31 = new QLabel(groupBox_3);
        label_31->setObjectName(QString::fromUtf8("label_31"));
        label_31->setGeometry(QRect(0, 50, 111, 21));
        label_31->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_33 = new QLabel(groupBox_3);
        label_33->setObjectName(QString::fromUtf8("label_33"));
        label_33->setGeometry(QRect(0, 80, 111, 21));
        label_33->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_35 = new QLabel(groupBox_3);
        label_35->setObjectName(QString::fromUtf8("label_35"));
        label_35->setGeometry(QRect(280, 20, 151, 21));
        label_35->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_34 = new QLabel(groupBox_3);
        label_34->setObjectName(QString::fromUtf8("label_34"));
        label_34->setGeometry(QRect(180, 50, 41, 21));
        label_36 = new QLabel(groupBox_3);
        label_36->setObjectName(QString::fromUtf8("label_36"));
        label_36->setGeometry(QRect(180, 80, 41, 21));
        lineEditBRC_6 = new CustomControls::LineEdit(groupBox_3);
        lineEditBRC_6->setObjectName(QString::fromUtf8("lineEditBRC_6"));
        lineEditBRC_6->setGeometry(QRect(650, 140, 50, 20));
        lineEditBRC_6->setMinimumSize(QSize(50, 0));
        label_37 = new QLabel(groupBox_3);
        label_37->setObjectName(QString::fromUtf8("label_37"));
        label_37->setGeometry(QRect(440, 140, 201, 21));
        label_37->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_38 = new QLabel(groupBox_3);
        label_38->setObjectName(QString::fromUtf8("label_38"));
        label_38->setGeometry(QRect(710, 140, 41, 21));
        label_59 = new QLabel(groupBox_3);
        label_59->setObjectName(QString::fromUtf8("label_59"));
        label_59->setGeometry(QRect(500, 80, 111, 21));
        lineEditBRC_7 = new CustomControls::LineEdit(groupBox_3);
        lineEditBRC_7->setObjectName(QString::fromUtf8("lineEditBRC_7"));
        lineEditBRC_7->setGeometry(QRect(440, 80, 50, 20));
        lineEditBRC_7->setMinimumSize(QSize(50, 0));
        label_60 = new QLabel(groupBox_3);
        label_60->setObjectName(QString::fromUtf8("label_60"));
        label_60->setGeometry(QRect(220, 80, 211, 21));
        label_60->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        comboBoxS1R1_3 = new CustomControls::ComboBox(groupBox_3);
        comboBoxS1R1_3->setObjectName(QString::fromUtf8("comboBoxS1R1_3"));
        comboBoxS1R1_3->setGeometry(QRect(120, 20, 51, 22));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 130, 761, 270));
        groupBox_2->setMinimumSize(QSize(0, 270));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        groupBox_2->setFont(font);
        lineEditSPIWriteData = new QLineEdit(groupBox_2);
        lineEditSPIWriteData->setObjectName(QString::fromUtf8("lineEditSPIWriteData"));
        lineEditSPIWriteData->setGeometry(QRect(70, 50, 291, 20));
        lineEditSPIWriteData->setMinimumSize(QSize(150, 0));
        lineEditSPIWriteData->setMaxLength(32767);
        lineEditSPIReadData = new QLineEdit(groupBox_2);
        lineEditSPIReadData->setObjectName(QString::fromUtf8("lineEditSPIReadData"));
        lineEditSPIReadData->setGeometry(QRect(400, 50, 291, 20));
        lineEditSPIReadData->setMinimumSize(QSize(150, 0));
        lineEditSPIReadData->setReadOnly(true);
        label_11 = new QLabel(groupBox_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(400, 20, 101, 31));
        label_11->setFont(font);
        label_11->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        pushButtonSPIWriteRead = new CustomControls::PushButton(groupBox_2);
        pushButtonSPIWriteRead->setObjectName(QString::fromUtf8("pushButtonSPIWriteRead"));
        pushButtonSPIWriteRead->setGeometry(QRect(310, 80, 141, 23));
        pushButtonSPIWriteRead->setFont(font);
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(70, 20, 131, 31));
        label_12->setFont(font);
        label_12->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_13 = new QLabel(groupBox_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(700, 50, 41, 21));
        lineEditSPIWriteDataCount = new QLineEdit(groupBox_2);
        lineEditSPIWriteDataCount->setObjectName(QString::fromUtf8("lineEditSPIWriteDataCount"));
        lineEditSPIWriteDataCount->setGeometry(QRect(71, 70, 50, 20));
        lineEditSPIWriteDataCount->setMinimumSize(QSize(50, 0));
        lineEditSPIWriteDataCount->setReadOnly(true);
        label_15 = new QLabel(groupBox_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 50, 51, 21));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_16 = new QLabel(groupBox_2);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(0, 70, 61, 21));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditSPIReadDataCount = new QLineEdit(groupBox_2);
        lineEditSPIReadDataCount->setObjectName(QString::fromUtf8("lineEditSPIReadDataCount"));
        lineEditSPIReadDataCount->setGeometry(QRect(640, 70, 50, 20));
        lineEditSPIReadDataCount->setMinimumSize(QSize(50, 0));
        lineEditSPIReadDataCount->setReadOnly(true);
        label_20 = new QLabel(groupBox_2);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(700, 70, 41, 21));
        pushButtonSPICRC = new CustomControls::PushButton(groupBox_2);
        pushButtonSPICRC->setObjectName(QString::fromUtf8("pushButtonSPICRC"));
        pushButtonSPICRC->setGeometry(QRect(140, 80, 151, 23));
        lineEditStatus = new CustomControls::LineEdit(groupBox_2);
        lineEditStatus->setObjectName(QString::fromUtf8("lineEditStatus"));
        lineEditStatus->setGeometry(QRect(130, 190, 71, 20));
        lineEditStatus->setMinimumSize(QSize(50, 0));
        lineEditStatus_2 = new CustomControls::LineEdit(groupBox_2);
        lineEditStatus_2->setObjectName(QString::fromUtf8("lineEditStatus_2"));
        lineEditStatus_2->setGeometry(QRect(130, 210, 71, 20));
        lineEditStatus_2->setMinimumSize(QSize(50, 0));
        label_39 = new QLabel(groupBox_2);
        label_39->setObjectName(QString::fromUtf8("label_39"));
        label_39->setGeometry(QRect(0, 190, 121, 21));
        label_39->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_40 = new QLabel(groupBox_2);
        label_40->setObjectName(QString::fromUtf8("label_40"));
        label_40->setGeometry(QRect(0, 210, 121, 21));
        label_40->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_41 = new QLabel(groupBox_2);
        label_41->setObjectName(QString::fromUtf8("label_41"));
        label_41->setGeometry(QRect(530, 150, 81, 21));
        label_41->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditStatus_7 = new CustomControls::LineEdit(groupBox_2);
        lineEditStatus_7->setObjectName(QString::fromUtf8("lineEditStatus_7"));
        lineEditStatus_7->setGeometry(QRect(620, 150, 71, 20));
        lineEditStatus_7->setMinimumSize(QSize(50, 0));
        label_42 = new QLabel(groupBox_2);
        label_42->setObjectName(QString::fromUtf8("label_42"));
        label_42->setGeometry(QRect(30, 230, 91, 21));
        label_42->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditStatus_8 = new CustomControls::LineEdit(groupBox_2);
        lineEditStatus_8->setObjectName(QString::fromUtf8("lineEditStatus_8"));
        lineEditStatus_8->setGeometry(QRect(130, 230, 71, 20));
        lineEditStatus_8->setMinimumSize(QSize(50, 0));
        label_43 = new QLabel(groupBox_2);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setGeometry(QRect(500, 190, 111, 21));
        label_43->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_44 = new QLabel(groupBox_2);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(510, 170, 101, 21));
        label_44->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditStatus_9 = new CustomControls::LineEdit(groupBox_2);
        lineEditStatus_9->setObjectName(QString::fromUtf8("lineEditStatus_9"));
        lineEditStatus_9->setGeometry(QRect(620, 170, 71, 20));
        lineEditStatus_9->setMinimumSize(QSize(50, 0));
        lineEditStatus_10 = new CustomControls::LineEdit(groupBox_2);
        lineEditStatus_10->setObjectName(QString::fromUtf8("lineEditStatus_10"));
        lineEditStatus_10->setGeometry(QRect(620, 190, 71, 20));
        lineEditStatus_10->setMinimumSize(QSize(50, 0));
        label_21 = new QLabel(groupBox_2);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(210, 190, 41, 21));
        label_22 = new QLabel(groupBox_2);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(210, 210, 41, 21));
        label_45 = new QLabel(groupBox_2);
        label_45->setObjectName(QString::fromUtf8("label_45"));
        label_45->setGeometry(QRect(700, 170, 41, 21));
        label_46 = new QLabel(groupBox_2);
        label_46->setObjectName(QString::fromUtf8("label_46"));
        label_46->setGeometry(QRect(700, 190, 41, 21));
        label_47 = new QLabel(groupBox_2);
        label_47->setObjectName(QString::fromUtf8("label_47"));
        label_47->setGeometry(QRect(210, 230, 41, 21));
        label_48 = new QLabel(groupBox_2);
        label_48->setObjectName(QString::fromUtf8("label_48"));
        label_48->setGeometry(QRect(700, 150, 41, 21));
        label_49 = new QLabel(groupBox_2);
        label_49->setObjectName(QString::fromUtf8("label_49"));
        label_49->setGeometry(QRect(500, 210, 111, 21));
        label_49->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditStatus_11 = new CustomControls::LineEdit(groupBox_2);
        lineEditStatus_11->setObjectName(QString::fromUtf8("lineEditStatus_11"));
        lineEditStatus_11->setGeometry(QRect(620, 210, 71, 20));
        lineEditStatus_11->setMinimumSize(QSize(20, 0));
        lineEditStatus_12 = new CustomControls::LineEdit(groupBox_2);
        lineEditStatus_12->setObjectName(QString::fromUtf8("lineEditStatus_12"));
        lineEditStatus_12->setGeometry(QRect(620, 230, 71, 20));
        lineEditStatus_12->setMinimumSize(QSize(20, 0));
        label_52 = new QLabel(groupBox_2);
        label_52->setObjectName(QString::fromUtf8("label_52"));
        label_52->setGeometry(QRect(520, 230, 91, 21));
        label_52->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        checkBox_22 = new CustomControls::CheckBox(groupBox_2);
        checkBox_22->setObjectName(QString::fromUtf8("checkBox_22"));
        checkBox_22->setGeometry(QRect(160, 150, 131, 20));
        checkBox_23 = new CustomControls::CheckBox(groupBox_2);
        checkBox_23->setObjectName(QString::fromUtf8("checkBox_23"));
        checkBox_23->setGeometry(QRect(160, 170, 131, 20));
        checkBox_24 = new CustomControls::CheckBox(groupBox_2);
        checkBox_24->setObjectName(QString::fromUtf8("checkBox_24"));
        checkBox_24->setGeometry(QRect(420, 150, 31, 20));
        checkBox_25 = new CustomControls::CheckBox(groupBox_2);
        checkBox_25->setObjectName(QString::fromUtf8("checkBox_25"));
        checkBox_25->setGeometry(QRect(420, 170, 31, 20));
        checkBox_26 = new CustomControls::CheckBox(groupBox_2);
        checkBox_26->setObjectName(QString::fromUtf8("checkBox_26"));
        checkBox_26->setGeometry(QRect(420, 190, 41, 20));
        checkBox_27 = new CustomControls::CheckBox(groupBox_2);
        checkBox_27->setObjectName(QString::fromUtf8("checkBox_27"));
        checkBox_27->setGeometry(QRect(420, 210, 41, 20));
        line_2 = new QFrame(groupBox_2);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setGeometry(QRect(370, 20, 21, 51));
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);
        line_3 = new QFrame(groupBox_2);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(20, 110, 721, 21));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        label_14 = new QLabel(groupBox_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(240, 120, 281, 31));
        label_14->setFont(font);
        label_14->setAlignment(Qt::AlignCenter);
        label_51 = new QLabel(groupBox_2);
        label_51->setObjectName(QString::fromUtf8("label_51"));
        label_51->setGeometry(QRect(10, 170, 111, 21));
        label_51->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_53 = new QLabel(groupBox_2);
        label_53->setObjectName(QString::fromUtf8("label_53"));
        label_53->setGeometry(QRect(10, 150, 111, 21));
        label_53->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_50 = new QLabel(groupBox_2);
        label_50->setObjectName(QString::fromUtf8("label_50"));
        label_50->setGeometry(QRect(700, 210, 41, 21));
        label_54 = new QLabel(groupBox_2);
        label_54->setObjectName(QString::fromUtf8("label_54"));
        label_54->setGeometry(QRect(700, 230, 41, 21));
        label_55 = new QLabel(groupBox_2);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        label_55->setGeometry(QRect(290, 210, 121, 21));
        label_55->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_56 = new QLabel(groupBox_2);
        label_56->setObjectName(QString::fromUtf8("label_56"));
        label_56->setGeometry(QRect(290, 190, 121, 21));
        label_56->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_57 = new QLabel(groupBox_2);
        label_57->setObjectName(QString::fromUtf8("label_57"));
        label_57->setGeometry(QRect(250, 170, 161, 21));
        label_57->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_58 = new QLabel(groupBox_2);
        label_58->setObjectName(QString::fromUtf8("label_58"));
        label_58->setGeometry(QRect(250, 150, 161, 21));
        label_58->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 761, 111));
        lineEditWriteData = new QLineEdit(groupBox);
        lineEditWriteData->setObjectName(QString::fromUtf8("lineEditWriteData"));
        lineEditWriteData->setGeometry(QRect(70, 40, 291, 20));
        lineEditWriteData->setMinimumSize(QSize(150, 0));
        lineEditReadData = new QLineEdit(groupBox);
        lineEditReadData->setObjectName(QString::fromUtf8("lineEditReadData"));
        lineEditReadData->setGeometry(QRect(400, 40, 291, 20));
        lineEditReadData->setMinimumSize(QSize(150, 0));
        lineEditReadData->setReadOnly(true);
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(400, 10, 291, 31));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        label_2->setFont(font1);
        label_2->setAlignment(Qt::AlignCenter);
        pushButtonWriteRead = new CustomControls::PushButton(groupBox);
        pushButtonWriteRead->setObjectName(QString::fromUtf8("pushButtonWriteRead"));
        pushButtonWriteRead->setGeometry(QRect(310, 70, 141, 23));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 10, 291, 31));
        label->setFont(font1);
        label->setAlignment(Qt::AlignCenter);
        pushButtonWriteOnly = new CustomControls::PushButton(groupBox);
        pushButtonWriteOnly->setObjectName(QString::fromUtf8("pushButtonWriteOnly"));
        pushButtonWriteOnly->setGeometry(QRect(200, 70, 91, 23));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(700, 40, 41, 21));
        lineEditWriteDataCount = new QLineEdit(groupBox);
        lineEditWriteDataCount->setObjectName(QString::fromUtf8("lineEditWriteDataCount"));
        lineEditWriteDataCount->setGeometry(QRect(70, 60, 50, 20));
        lineEditWriteDataCount->setMinimumSize(QSize(50, 0));
        lineEditWriteDataCount->setReadOnly(true);
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 40, 51, 21));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(0, 60, 61, 21));
        label_6->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditReadDataCount = new QLineEdit(groupBox);
        lineEditReadDataCount->setObjectName(QString::fromUtf8("lineEditReadDataCount"));
        lineEditReadDataCount->setGeometry(QRect(640, 60, 50, 20));
        lineEditReadDataCount->setMinimumSize(QSize(50, 0));
        lineEditReadDataCount->setReadOnly(true);
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(700, 60, 41, 21));
        pushButtonWriteOnly_2 = new CustomControls::PushButton(groupBox);
        pushButtonWriteOnly_2->setObjectName(QString::fromUtf8("pushButtonWriteOnly_2"));
        pushButtonWriteOnly_2->setGeometry(QRect(470, 70, 91, 23));
        line = new QFrame(groupBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(370, 20, 21, 41));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        groupBox_5 = new QGroupBox(tab_2);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 10, 761, 1041));
        groupBox_5->setCheckable(false);
        lineEdit_21 = new CustomControls::LineEdit(groupBox_5);
        lineEdit_21->setObjectName(QString::fromUtf8("lineEdit_21"));
        lineEdit_21->setGeometry(QRect(110, 30, 51, 22));
        label_73 = new QLabel(groupBox_5);
        label_73->setObjectName(QString::fromUtf8("label_73"));
        label_73->setGeometry(QRect(10, 30, 91, 21));
        label_73->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_74 = new QLabel(groupBox_5);
        label_74->setObjectName(QString::fromUtf8("label_74"));
        label_74->setGeometry(QRect(170, 30, 31, 21));
        pushButton_22 = new CustomControls::PushButton(groupBox_5);
        pushButton_22->setObjectName(QString::fromUtf8("pushButton_22"));
        pushButton_22->setGeometry(QRect(510, 30, 111, 21));
        pushButton_23 = new CustomControls::PushButton(groupBox_5);
        pushButton_23->setObjectName(QString::fromUtf8("pushButton_23"));
        pushButton_23->setGeometry(QRect(630, 30, 111, 21));
        lineEdit_27 = new CustomControls::LineEdit(groupBox_5);
        lineEdit_27->setObjectName(QString::fromUtf8("lineEdit_27"));
        lineEdit_27->setGeometry(QRect(370, 30, 51, 22));
        label_77 = new QLabel(groupBox_5);
        label_77->setObjectName(QString::fromUtf8("label_77"));
        label_77->setGeometry(QRect(270, 30, 91, 21));
        label_77->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_78 = new QLabel(groupBox_5);
        label_78->setObjectName(QString::fromUtf8("label_78"));
        label_78->setGeometry(QRect(430, 30, 31, 21));
        line_4 = new QFrame(groupBox_5);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setGeometry(QRect(10, 190, 741, 20));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);
        line_9 = new QFrame(groupBox_5);
        line_9->setObjectName(QString::fromUtf8("line_9"));
        line_9->setGeometry(QRect(10, 60, 741, 21));
        line_9->setFrameShape(QFrame::HLine);
        line_9->setFrameShadow(QFrame::Sunken);
        line_10 = new QFrame(groupBox_5);
        line_10->setObjectName(QString::fromUtf8("line_10"));
        line_10->setGeometry(QRect(10, 70, 741, 21));
        line_10->setFrameShape(QFrame::HLine);
        line_10->setFrameShadow(QFrame::Sunken);
        comboBoxS1R1_4 = new CustomControls::ComboBox(groupBox_5);
        comboBoxS1R1_4->setObjectName(QString::fromUtf8("comboBoxS1R1_4"));
        comboBoxS1R1_4->setGeometry(QRect(170, 100, 311, 22));
        pushButton_24 = new CustomControls::PushButton(groupBox_5);
        pushButton_24->setObjectName(QString::fromUtf8("pushButton_24"));
        pushButton_24->setGeometry(QRect(510, 130, 111, 21));
        pushButton_25 = new CustomControls::PushButton(groupBox_5);
        pushButton_25->setObjectName(QString::fromUtf8("pushButton_25"));
        pushButton_25->setGeometry(QRect(630, 130, 111, 21));
        label_75 = new QLabel(groupBox_5);
        label_75->setObjectName(QString::fromUtf8("label_75"));
        label_75->setGeometry(QRect(50, 100, 111, 21));
        label_75->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_76 = new QLabel(groupBox_5);
        label_76->setObjectName(QString::fromUtf8("label_76"));
        label_76->setGeometry(QRect(50, 130, 111, 21));
        label_76->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_79 = new QLabel(groupBox_5);
        label_79->setObjectName(QString::fromUtf8("label_79"));
        label_79->setGeometry(QRect(50, 160, 111, 21));
        label_79->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        comboBoxS1R1_5 = new CustomControls::ComboBox(groupBox_5);
        comboBoxS1R1_5->setObjectName(QString::fromUtf8("comboBoxS1R1_5"));
        comboBoxS1R1_5->setGeometry(QRect(170, 160, 311, 22));
        comboBoxS1R1_6 = new CustomControls::ComboBox(groupBox_5);
        comboBoxS1R1_6->setObjectName(QString::fromUtf8("comboBoxS1R1_6"));
        comboBoxS1R1_6->setGeometry(QRect(170, 130, 311, 22));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(UartSpiController);

        tabWidget->setCurrentIndex(0);

    } // setupUi

    void retranslateUi(QWidget *UartSpiController)
    {
        UartSpiController->setWindowTitle(QApplication::translate("UartSpiController", "UartController2", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("UartSpiController", "Demultiplexers", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButtonWriteRead_2->setAccessibleName(QApplication::translate("UartSpiController", "SetDemux()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButtonWriteRead_2->setText(QApplication::translate("UartSpiController", "Set", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_28->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M1DEMUX1]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_28->setText(QApplication::translate("UartSpiController", "M1DEMUX1", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_29->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M1DEMUX2]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_29->setText(QApplication::translate("UartSpiController", "M1DEMUX2", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_30->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M1DEMUX5]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_30->setText(QApplication::translate("UartSpiController", "M1DEMUX5", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_31->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M1DEMUX3]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_31->setText(QApplication::translate("UartSpiController", "M1DEMUX3", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_32->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M1DEMUX4]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_32->setText(QApplication::translate("UartSpiController", "M1DEMUX4", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_33->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M1DEMUX6]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_33->setText(QApplication::translate("UartSpiController", "M1DEMUX6", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_34->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M2DEMUX3]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_34->setText(QApplication::translate("UartSpiController", "M2DEMUX3", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_35->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M2DEMUX2]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_35->setText(QApplication::translate("UartSpiController", "M2DEMUX2", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_36->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M2DEMUX4]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_36->setText(QApplication::translate("UartSpiController", "M2DEMUX4", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_37->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M2DEMUX5]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_37->setText(QApplication::translate("UartSpiController", "M2DEMUX5", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_38->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M2DEMUX1]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_38->setText(QApplication::translate("UartSpiController", "M2DEMUX1", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_39->setAccessibleName(QApplication::translate("UartSpiController", "DEMUX[M2DEMUX6]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_39->setText(QApplication::translate("UartSpiController", "M2DEMUX6", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("UartSpiController", "BRC command", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditBRCReadData->setAccessibleName(QApplication::translate("UartSpiController", "ReadData", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditBRCReadData->setInputMask(QString());
        lineEditBRCReadData->setText(QString());
#ifndef QT_NO_ACCESSIBILITY
        pushButtonSPIWriteRead_2->setAccessibleName(QApplication::translate("UartSpiController", "PerformBRC()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButtonSPIWriteRead_2->setText(QApplication::translate("UartSpiController", "Perform", 0, QApplication::UnicodeUTF8));
        label_23->setText(QApplication::translate("UartSpiController", "0 - 8191 us", 0, QApplication::UnicodeUTF8));
        label_24->setText(QApplication::translate("UartSpiController", "hex", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditBRC->setAccessibleName(QApplication::translate("UartSpiController", "BRC_PERIOD", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditBRC->setInputMask(QString());
        lineEditBRC->setText(QApplication::translate("UartSpiController", "6", 0, QApplication::UnicodeUTF8));
        label_25->setText(QApplication::translate("UartSpiController", "Channel", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("UartSpiController", "Min time between BRC transfers", 0, QApplication::UnicodeUTF8));
        label_27->setText(QApplication::translate("UartSpiController", "READ DATA", 0, QApplication::UnicodeUTF8));
        label_28->setText(QApplication::translate("UartSpiController", "LENGTH", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditBRCReadDataCount->setAccessibleName(QApplication::translate("UartSpiController", "WriteData", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditBRCReadDataCount->setInputMask(QString());
        lineEditBRCReadDataCount->setText(QApplication::translate("UartSpiController", "00", 0, QApplication::UnicodeUTF8));
        label_29->setText(QApplication::translate("UartSpiController", "0 - 31 B", 0, QApplication::UnicodeUTF8));
        label_30->setText(QApplication::translate("UartSpiController", "dec", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditBRC_2->setAccessibleName(QApplication::translate("UartSpiController", "BRC_SLOTS", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditBRC_2->setInputMask(QString());
        lineEditBRC_2->setText(QApplication::translate("UartSpiController", "6", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditBRC_3->setAccessibleName(QApplication::translate("UartSpiController", "BRC_LENGTH", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditBRC_3->setInputMask(QString());
        lineEditBRC_3->setText(QApplication::translate("UartSpiController", "6", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditBRC_4->setAccessibleName(QApplication::translate("UartSpiController", "BRC_BRC", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditBRC_4->setInputMask(QString());
        lineEditBRC_4->setText(QApplication::translate("UartSpiController", "6", 0, QApplication::UnicodeUTF8));
        label_31->setText(QApplication::translate("UartSpiController", "Number of BRC", 0, QApplication::UnicodeUTF8));
        label_33->setText(QApplication::translate("UartSpiController", "Number of slots", 0, QApplication::UnicodeUTF8));
        label_35->setText(QApplication::translate("UartSpiController", "Length of one slot", 0, QApplication::UnicodeUTF8));
        label_34->setText(QApplication::translate("UartSpiController", "1 - 63", 0, QApplication::UnicodeUTF8));
        label_36->setText(QApplication::translate("UartSpiController", "1 - 8", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditBRC_6->setAccessibleName(QApplication::translate("UartSpiController", "BRC_RESP_LENGTH", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditBRC_6->setInputMask(QString());
        lineEditBRC_6->setText(QApplication::translate("UartSpiController", "6", 0, QApplication::UnicodeUTF8));
        label_37->setText(QApplication::translate("UartSpiController", "Expected number of RX bytes", 0, QApplication::UnicodeUTF8));
        label_38->setText(QApplication::translate("UartSpiController", "dec", 0, QApplication::UnicodeUTF8));
        label_59->setText(QApplication::translate("UartSpiController", "0 - 65535 us", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditBRC_7->setAccessibleName(QApplication::translate("UartSpiController", "BRC_START", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditBRC_7->setInputMask(QString());
        lineEditBRC_7->setText(QApplication::translate("UartSpiController", "0", 0, QApplication::UnicodeUTF8));
        label_60->setText(QApplication::translate("UartSpiController", "Time between MEASURE and BRC", 0, QApplication::UnicodeUTF8));
        comboBoxS1R1_3->clear();
        comboBoxS1R1_3->insertItems(0, QStringList()
         << QApplication::translate("UartSpiController", "A", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("UartSpiController", "B", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_ACCESSIBILITY
        comboBoxS1R1_3->setAccessibleName(QApplication::translate("UartSpiController", "CHANNEL", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        groupBox_2->setTitle(QApplication::translate("UartSpiController", "Universal SPI command", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditSPIWriteData->setAccessibleName(QApplication::translate("UartSpiController", "WriteData", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditSPIWriteData->setInputMask(QString());
        lineEditSPIWriteData->setText(QApplication::translate("UartSpiController", "8fef0000", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditSPIReadData->setAccessibleName(QApplication::translate("UartSpiController", "ReadData", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditSPIReadData->setInputMask(QString());
        lineEditSPIReadData->setText(QString());
        label_11->setText(QApplication::translate("UartSpiController", "READ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButtonSPIWriteRead->setAccessibleName(QApplication::translate("UartSpiController", "SPIWriteRead()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButtonSPIWriteRead->setText(QApplication::translate("UartSpiController", "Write and Read", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("UartSpiController", "WRITE 6 BYTES", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("UartSpiController", "hex", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditSPIWriteDataCount->setAccessibleName(QApplication::translate("UartSpiController", "WriteData", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditSPIWriteDataCount->setInputMask(QString());
        lineEditSPIWriteDataCount->setText(QApplication::translate("UartSpiController", "4", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("UartSpiController", "DATA", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("UartSpiController", "LENGTH", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditSPIReadDataCount->setAccessibleName(QApplication::translate("UartSpiController", "WriteData", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditSPIReadDataCount->setInputMask(QString());
        lineEditSPIReadDataCount->setText(QApplication::translate("UartSpiController", "00", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("UartSpiController", "dec", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButtonSPICRC->setAccessibleName(QApplication::translate("UartSpiController", "SPICRC()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButtonSPICRC->setText(QApplication::translate("UartSpiController", "Add CRC to BYTE6", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditStatus->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[PDCM_RECEIVED_A]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditStatus->setInputMask(QString());
        lineEditStatus->setText(QApplication::translate("UartSpiController", "0x00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditStatus_2->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[PDCM_RECEIVED_B]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditStatus_2->setInputMask(QString());
        lineEditStatus_2->setText(QApplication::translate("UartSpiController", "0x00", 0, QApplication::UnicodeUTF8));
        label_39->setText(QApplication::translate("UartSpiController", "PDCMReceivedA", 0, QApplication::UnicodeUTF8));
        label_40->setText(QApplication::translate("UartSpiController", "PDCMReceivedB", 0, QApplication::UnicodeUTF8));
        label_41->setText(QApplication::translate("UartSpiController", "CRC-8", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditStatus_7->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[CRC8]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditStatus_7->setInputMask(QString());
        lineEditStatus_7->setText(QApplication::translate("UartSpiController", "0x00", 0, QApplication::UnicodeUTF8));
        label_42->setText(QApplication::translate("UartSpiController", "CLKIN_CNT", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditStatus_8->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[CLKIN_CNT]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditStatus_8->setInputMask(QString());
        lineEditStatus_8->setText(QString());
        label_43->setText(QApplication::translate("UartSpiController", "CRC_FAIL_B", 0, QApplication::UnicodeUTF8));
        label_44->setText(QApplication::translate("UartSpiController", "CRC_FAIL_A", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditStatus_9->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[CRC_FAIL_A]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditStatus_9->setInputMask(QString());
        lineEditStatus_9->setText(QApplication::translate("UartSpiController", "0x00", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditStatus_10->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[CRC_FAIL_B]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditStatus_10->setInputMask(QString());
        lineEditStatus_10->setText(QApplication::translate("UartSpiController", "0x00", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("UartSpiController", "hex", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("UartSpiController", "hex", 0, QApplication::UnicodeUTF8));
        label_45->setText(QApplication::translate("UartSpiController", "hex", 0, QApplication::UnicodeUTF8));
        label_46->setText(QApplication::translate("UartSpiController", "hex", 0, QApplication::UnicodeUTF8));
        label_47->setText(QApplication::translate("UartSpiController", "dec", 0, QApplication::UnicodeUTF8));
        label_48->setText(QApplication::translate("UartSpiController", "hex", 0, QApplication::UnicodeUTF8));
        label_49->setText(QApplication::translate("UartSpiController", "KAC_Master", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditStatus_11->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[KAC_MASTER]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditStatus_11->setInputMask(QString());
        lineEditStatus_11->setText(QString());
#ifndef QT_NO_ACCESSIBILITY
        lineEditStatus_12->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[KAC_SLAVE]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditStatus_12->setInputMask(QString());
        lineEditStatus_12->setText(QString());
        label_52->setText(QApplication::translate("UartSpiController", "KAC_Slave", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_22->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[CRM_RECEIVED_A]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_22->setText(QString());
#ifndef QT_NO_ACCESSIBILITY
        checkBox_23->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[CRM_RECEIVED_B]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_23->setText(QString());
#ifndef QT_NO_ACCESSIBILITY
        checkBox_24->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[TIMER_INT_RECEIVED_A]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_24->setText(QString());
#ifndef QT_NO_ACCESSIBILITY
        checkBox_25->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[TIMER_INT_RECEIVED_B]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_25->setText(QString());
#ifndef QT_NO_ACCESSIBILITY
        checkBox_26->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[SPI_ERROR]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_26->setText(QString());
#ifndef QT_NO_ACCESSIBILITY
        checkBox_27->setAccessibleName(QApplication::translate("UartSpiController", "STATUS[HARDWARE_ERROR]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_27->setText(QString());
        label_14->setText(QApplication::translate("UartSpiController", "STATUS", 0, QApplication::UnicodeUTF8));
        label_51->setText(QApplication::translate("UartSpiController", "CRMreceivedB", 0, QApplication::UnicodeUTF8));
        label_53->setText(QApplication::translate("UartSpiController", "CRMreceivedA", 0, QApplication::UnicodeUTF8));
        label_50->setText(QApplication::translate("UartSpiController", "dec", 0, QApplication::UnicodeUTF8));
        label_54->setText(QApplication::translate("UartSpiController", "dec", 0, QApplication::UnicodeUTF8));
        label_55->setText(QApplication::translate("UartSpiController", "Hardware_Error", 0, QApplication::UnicodeUTF8));
        label_56->setText(QApplication::translate("UartSpiController", "SPI_Error", 0, QApplication::UnicodeUTF8));
        label_57->setText(QApplication::translate("UartSpiController", "Timer_int_received_B", 0, QApplication::UnicodeUTF8));
        label_58->setText(QApplication::translate("UartSpiController", "Timer_int_received_A", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("UartSpiController", "Universal UART command", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditWriteData->setAccessibleName(QApplication::translate("UartSpiController", "WriteData", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditWriteData->setInputMask(QString());
        lineEditWriteData->setText(QApplication::translate("UartSpiController", "8fef0000", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditReadData->setAccessibleName(QApplication::translate("UartSpiController", "ReadData", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditReadData->setInputMask(QString());
        lineEditReadData->setText(QString());
        label_2->setText(QApplication::translate("UartSpiController", "READ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButtonWriteRead->setAccessibleName(QApplication::translate("UartSpiController", "WriteRead()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButtonWriteRead->setText(QApplication::translate("UartSpiController", "Write and Read", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("UartSpiController", "WRITE", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButtonWriteOnly->setAccessibleName(QApplication::translate("UartSpiController", "WriteOnly()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButtonWriteOnly->setText(QApplication::translate("UartSpiController", "Write only", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("UartSpiController", "hex", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditWriteDataCount->setAccessibleName(QApplication::translate("UartSpiController", "WriteData", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditWriteDataCount->setInputMask(QString());
        lineEditWriteDataCount->setText(QApplication::translate("UartSpiController", "6", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("UartSpiController", "DATA", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("UartSpiController", "LENGTH", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditReadDataCount->setAccessibleName(QApplication::translate("UartSpiController", "WriteData", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditReadDataCount->setInputMask(QString());
        lineEditReadDataCount->setText(QApplication::translate("UartSpiController", "00", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("UartSpiController", "dec", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButtonWriteOnly_2->setAccessibleName(QApplication::translate("UartSpiController", "ReadOnly()", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButtonWriteOnly_2->setText(QApplication::translate("UartSpiController", "Read only", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("UartSpiController", "UART Controller", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("UartSpiController", "Registers", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_21->setAccessibleName(QApplication::translate("UartSpiController", "U2SADDR", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEdit_21->setText(QApplication::translate("UartSpiController", "0", 0, QApplication::UnicodeUTF8));
        label_73->setText(QApplication::translate("UartSpiController", "ADDR[7:0]", 0, QApplication::UnicodeUTF8));
        label_74->setText(QApplication::translate("UartSpiController", "dec", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_22->setAccessibleName(QApplication::translate("UartSpiController", "WriteREGX(U2SADDR)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_22->setText(QApplication::translate("UartSpiController", "Write any reg", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_23->setAccessibleName(QApplication::translate("UartSpiController", "ReadREGX(U2SADDR)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_23->setText(QApplication::translate("UartSpiController", "Read any reg", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_27->setAccessibleName(QApplication::translate("UartSpiController", "U2SREGADDR[7:0]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEdit_27->setText(QApplication::translate("UartSpiController", "0", 0, QApplication::UnicodeUTF8));
        label_77->setText(QApplication::translate("UartSpiController", "DATA[7:0]", 0, QApplication::UnicodeUTF8));
        label_78->setText(QApplication::translate("UartSpiController", "dec", 0, QApplication::UnicodeUTF8));
        comboBoxS1R1_4->clear();
        comboBoxS1R1_4->insertItems(0, QStringList()
         << QApplication::translate("UartSpiController", "Internal FPGA master", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("UartSpiController", "External master via ribbon cable", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_ACCESSIBILITY
        comboBoxS1R1_4->setAccessibleName(QApplication::translate("UartSpiController", "U2SREGX03[bit7]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        pushButton_24->setAccessibleName(QApplication::translate("UartSpiController", "WriteREGX(U2SREGX03)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_24->setText(QApplication::translate("UartSpiController", "Write reg 0x03", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButton_25->setAccessibleName(QApplication::translate("UartSpiController", "ReadREGX(U2SREGX03)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButton_25->setText(QApplication::translate("UartSpiController", "Read reg 0x03", 0, QApplication::UnicodeUTF8));
        label_75->setText(QApplication::translate("UartSpiController", "Reg 0x03, bit 7", 0, QApplication::UnicodeUTF8));
        label_76->setText(QApplication::translate("UartSpiController", "Reg 0x03, bit 1", 0, QApplication::UnicodeUTF8));
        label_79->setText(QApplication::translate("UartSpiController", "Reg 0x03, bit 0", 0, QApplication::UnicodeUTF8));
        comboBoxS1R1_5->clear();
        comboBoxS1R1_5->insertItems(0, QStringList()
         << QApplication::translate("UartSpiController", "SPI pin IO2 - low", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("UartSpiController", "SPI pin IO2 - high", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_ACCESSIBILITY
        comboBoxS1R1_5->setAccessibleName(QApplication::translate("UartSpiController", "U2SREGX03[bit0]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        comboBoxS1R1_6->clear();
        comboBoxS1R1_6->insertItems(0, QStringList()
         << QApplication::translate("UartSpiController", "SPI pin IO1 - low", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("UartSpiController", "SPI pin IO1 - high", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_ACCESSIBILITY
        comboBoxS1R1_6->setAccessibleName(QApplication::translate("UartSpiController", "U2SREGX03[bit1]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("UartSpiController", "UART2SPI bridge regs", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UartSpiController: public Ui_UartSpiController {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UARTSPICONTROLLER_H
