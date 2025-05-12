/********************************************************************************
** Form generated from reading UI file 'CFARsim.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CFARSIM_H
#define UI_CFARSIM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>
#include <customcontrols_checkbox.h>
#include "customcontrols.h"
#include "customcontrols_combobox.h"
#include "customcontrols_lineedit.h"
#include "customcontrols_radiobutton.h"
#include "customcontrols_realedit.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_CFARsim
{
public:
    QTabWidget *tabWidget;
    QWidget *tab_3;
    CustomControls::PushButton *pushButtonUpdate_49;
    CustomControls::PushButton *pushButtonUpdate_17;
    QCustomPlot *ptSimulation;
    QCustomPlot *ptLegend;
    QLabel *label_57;
    QLabel *label_61;
    CustomControls::LineEdit *lineEditRange_start;
    QLabel *label_56;
    QLabel *label_62;
    CustomControls::LineEdit *lineEditRange_stop;
    QTabWidget *tabWidget_2;
    QWidget *tab;
    QTableWidget *twSamples;
    QWidget *tab_2;
    QTableWidget *twSamplesToF;
    QLabel *label_65;
    QLabel *label_69;
    CustomControls::RealEdit *lineEditRange_31;
    QWidget *tab_4;
    QTableWidget *twCompr;
    CustomControls::ComboBox *cb_plot1;
    QLabel *label_3;
    QLabel *label_4;
    CustomControls::ComboBox *cb_plot2;
    CustomControls::ComboBox *cb_plot3;
    QLabel *label_5;
    QLabel *label_6;
    CustomControls::ComboBox *cb_plot4;
    QLabel *label_7;
    CustomControls::ComboBox *cb_plot5;
    QLabel *label_8;
    CustomControls::ComboBox *cb_plot6;
    QLabel *label_9;
    CustomControls::ComboBox *cb_plot7;
    CustomControls::ComboBox *cb_plot8;
    QLabel *label_10;
    CustomControls::PushButton *pbSaveCSV;
    CustomControls::PushButton *pbSaveTOF;
    CustomControls::PushButton *pbSaveTOF_2;
    CustomControls::CheckBox *cb_updatetable;
    QWidget *DynamicScaling;
    QGroupBox *groupBox;
    QCustomPlot *ptDynScale;
    CustomControls::CheckBox *checkBox;
    QFrame *frame_3;
    QLabel *label_148;
    CustomControls::RealEdit *le_dyn_sc_dt0;
    QLabel *label_142;
    CustomControls::RealEdit *le_dyn_sc_dt5;
    QLabel *label_131;
    CustomControls::RealEdit *le_dyn_sc_dt4;
    CustomControls::RealEdit *le_dyn_sc_dt1;
    CustomControls::RealEdit *le_dyn_sc_dt2;
    QLabel *label_60;
    CustomControls::RealEdit *le_dyn_sc_val5;
    CustomControls::RealEdit *le_dyn_sc_val2;
    CustomControls::RealEdit *le_dyn_sc_val4;
    QLabel *label_126;
    QLabel *label_129;
    CustomControls::RealEdit *le_dyn_sc_dt3;
    QLabel *label_141;
    QLabel *label_146;
    QLabel *label_144;
    QLabel *label_143;
    CustomControls::RealEdit *le_dyn_sc_val0;
    CustomControls::RealEdit *le_dyn_sc_val3;
    QLabel *label_147;
    QLabel *label_145;
    QLabel *label_63;
    QLabel *label_130;
    CustomControls::RealEdit *le_dyn_sc_val1;
    QLabel *label_66;
    CustomControls::ComboBox *cbDynScChA;
    QLabel *label_64;
    QFrame *frame_4;
    QLabel *label_149;
    CustomControls::RealEdit *le_dyn_sc_dt0_2;
    QLabel *label_150;
    CustomControls::RealEdit *le_dyn_sc_dt5_2;
    QLabel *label_151;
    CustomControls::RealEdit *le_dyn_sc_dt4_2;
    CustomControls::RealEdit *le_dyn_sc_dt1_2;
    CustomControls::RealEdit *le_dyn_sc_dt2_2;
    QLabel *label_67;
    CustomControls::RealEdit *le_dyn_sc_val5_2;
    CustomControls::RealEdit *le_dyn_sc_val2_2;
    CustomControls::RealEdit *le_dyn_sc_val4_2;
    QLabel *label_152;
    QLabel *label_153;
    CustomControls::RealEdit *le_dyn_sc_dt3_2;
    QLabel *label_154;
    QLabel *label_155;
    QLabel *label_156;
    QLabel *label_157;
    CustomControls::RealEdit *le_dyn_sc_val0_2;
    CustomControls::RealEdit *le_dyn_sc_val3_2;
    QLabel *label_158;
    QLabel *label_159;
    QLabel *label_68;
    QLabel *label_160;
    CustomControls::RealEdit *le_dyn_sc_val1_2;
    QLabel *label_70;
    CustomControls::ComboBox *cbDynScChB;
    QFrame *line;
    QGroupBox *groupBoxCFAR;
    QLabel *label_43;
    QLabel *label_124;
    CustomControls::LineEdit *lineEdit_199;
    QLabel *label_127;
    QLabel *label_132;
    CustomControls::LineEdit *lineEdit_200;
    CustomControls::LineEdit *lineEdit_201;
    CustomControls::LineEdit *lineEdit_202;
    QLabel *label_133;
    CustomControls::LineEdit *lineEdit_203;
    QLabel *label_128;
    CustomControls::RealEdit *lineEdit_204;
    CustomControls::LineEdit *lineEdit_205;
    QLabel *label_134;
    CustomControls::PushButton *pbCalcCFAR;
    CustomControls::LineEdit *lineEdit_206;
    QLabel *label_138;
    QLabel *label_135;
    CustomControls::LineEdit *lineEdit_207;
    QLabel *label_139;
    CustomControls::LineEdit *lineEdit_208;
    QLabel *label_55;
    QLabel *label_58;
    QLabel *label_59;
    QLabel *label_140;
    CustomControls::LineEdit *lineEdit_209;
    CustomControls::LineEdit *lineEdit_210;
    QLabel *label_125;
    CustomControls::LineEdit *lineEdit_211;
    QLabel *label_44;
    QFrame *frame;
    QLabel *label_11;
    CustomControls::RadioButton *radioButtonCFARv1;
    CustomControls::RadioButton *radioButtonCFARv2;
    QFrame *frame_2;
    CustomControls::RadioButton *rbSourceMag;
    CustomControls::RadioButton *rbSourceToF;
    QLabel *label_18;
    CustomControls::CheckBox *checkBox_2;
    QGroupBox *groupBox_IO;
    CustomControls::PushButton *pbLoadCSV;
    CustomControls::PushButton *pbLoadMeas;
    CustomControls::ComboBox *cb_delim;
    QLabel *label;
    CustomControls::LineEdit *le_LegendLine;
    QLabel *label_2;
    QGroupBox *groupBoxCompr;
    CustomControls::CheckBox *cb_cmpr_cfaren_1;
    CustomControls::CheckBox *cb_cmpr_cpren_1;
    CustomControls::CheckBox *cb_cmpr_cpren_2;
    CustomControls::CheckBox *cb_cmpr_cfaren_2;
    CustomControls::CheckBox *cb_cmpr_cpren_3;
    CustomControls::CheckBox *cb_cmpr_cfaren_3;
    QLabel *label_136;
    QLabel *label_12;
    QLabel *label_13;
    QLabel *label_14;
    CustomControls::PushButton *pbUpDBG;
    CustomControls::ComboBox *cb_dbgin_1;
    CustomControls::ComboBox *cb_dbgin_2;
    CustomControls::ComboBox *cb_dbgin_3;
    QLabel *label_137;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    CustomControls::ComboBox *cb_sampling_1;
    CustomControls::ComboBox *cb_sampling_2;
    CustomControls::ComboBox *cb_sampling_3;

    void setupUi(QWidget *CFARsim)
    {
        if (CFARsim->objectName().isEmpty())
            CFARsim->setObjectName(QString::fromUtf8("CFARsim"));
        CFARsim->resize(1574, 1120);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(CFARsim->sizePolicy().hasHeightForWidth());
        CFARsim->setSizePolicy(sizePolicy);
        CFARsim->setStyleSheet(QString::fromUtf8("QTabWidget{\n"
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
        tabWidget = new QTabWidget(CFARsim);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 160, 1551, 971));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        tabWidget->setFont(font);
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        pushButtonUpdate_49 = new CustomControls::PushButton(tab_3);
        pushButtonUpdate_49->setObjectName(QString::fromUtf8("pushButtonUpdate_49"));
        pushButtonUpdate_49->setGeometry(QRect(1370, 330, 101, 21));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        pushButtonUpdate_49->setFont(font1);
        pushButtonUpdate_17 = new CustomControls::PushButton(tab_3);
        pushButtonUpdate_17->setObjectName(QString::fromUtf8("pushButtonUpdate_17"));
        pushButtonUpdate_17->setGeometry(QRect(1370, 350, 101, 21));
        pushButtonUpdate_17->setFont(font1);
        ptSimulation = new QCustomPlot(tab_3);
        ptSimulation->setObjectName(QString::fromUtf8("ptSimulation"));
        ptSimulation->setGeometry(QRect(10, 20, 1271, 701));
        ptSimulation->setMinimumSize(QSize(450, 140));
        ptSimulation->setMaximumSize(QSize(16777215, 16777215));
        ptLegend = new QCustomPlot(tab_3);
        ptLegend->setObjectName(QString::fromUtf8("ptLegend"));
        ptLegend->setGeometry(QRect(1290, 20, 241, 251));
        ptLegend->setMinimumSize(QSize(150, 200));
        ptLegend->setMaximumSize(QSize(16777215, 16777215));
        label_57 = new QLabel(tab_3);
        label_57->setObjectName(QString::fromUtf8("label_57"));
        label_57->setGeometry(QRect(1460, 300, 31, 21));
        label_57->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_61 = new QLabel(tab_3);
        label_61->setObjectName(QString::fromUtf8("label_61"));
        label_61->setGeometry(QRect(1340, 280, 51, 21));
        label_61->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEditRange_start = new CustomControls::LineEdit(tab_3);
        lineEditRange_start->setObjectName(QString::fromUtf8("lineEditRange_start"));
        lineEditRange_start->setGeometry(QRect(1400, 280, 50, 21));
        lineEditRange_start->setMinimumSize(QSize(50, 0));
        label_56 = new QLabel(tab_3);
        label_56->setObjectName(QString::fromUtf8("label_56"));
        label_56->setGeometry(QRect(1340, 300, 51, 21));
        label_56->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_62 = new QLabel(tab_3);
        label_62->setObjectName(QString::fromUtf8("label_62"));
        label_62->setGeometry(QRect(1460, 280, 31, 21));
        label_62->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        lineEditRange_stop = new CustomControls::LineEdit(tab_3);
        lineEditRange_stop->setObjectName(QString::fromUtf8("lineEditRange_stop"));
        lineEditRange_stop->setGeometry(QRect(1400, 300, 50, 21));
        lineEditRange_stop->setMinimumSize(QSize(50, 0));
        tabWidget_2 = new QTabWidget(tab_3);
        tabWidget_2->setObjectName(QString::fromUtf8("tabWidget_2"));
        tabWidget_2->setGeometry(QRect(20, 730, 1261, 221));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        twSamples = new QTableWidget(tab);
        if (twSamples->columnCount() < 9)
            twSamples->setColumnCount(9);
        if (twSamples->rowCount() < 5)
            twSamples->setRowCount(5);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        twSamples->setItem(0, 0, __qtablewidgetitem);
        twSamples->setObjectName(QString::fromUtf8("twSamples"));
        twSamples->setGeometry(QRect(10, 10, 1231, 171));
        twSamples->setRowCount(5);
        twSamples->setColumnCount(9);
        twSamples->horizontalHeader()->setDefaultSectionSize(65);
        twSamples->horizontalHeader()->setMinimumSectionSize(27);
        twSamples->verticalHeader()->setVisible(false);
        twSamples->verticalHeader()->setDefaultSectionSize(23);
        twSamples->verticalHeader()->setHighlightSections(true);
        tabWidget_2->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        twSamplesToF = new QTableWidget(tab_2);
        if (twSamplesToF->columnCount() < 9)
            twSamplesToF->setColumnCount(9);
        if (twSamplesToF->rowCount() < 5)
            twSamplesToF->setRowCount(5);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        twSamplesToF->setItem(0, 0, __qtablewidgetitem1);
        twSamplesToF->setObjectName(QString::fromUtf8("twSamplesToF"));
        twSamplesToF->setGeometry(QRect(10, 10, 1161, 171));
        twSamplesToF->setRowCount(5);
        twSamplesToF->setColumnCount(9);
        twSamplesToF->horizontalHeader()->setDefaultSectionSize(60);
        twSamplesToF->horizontalHeader()->setMinimumSectionSize(27);
        twSamplesToF->verticalHeader()->setVisible(false);
        twSamplesToF->verticalHeader()->setDefaultSectionSize(23);
        twSamplesToF->verticalHeader()->setHighlightSections(true);
        label_65 = new QLabel(tab_2);
        label_65->setObjectName(QString::fromUtf8("label_65"));
        label_65->setGeometry(QRect(1170, 10, 91, 21));
        label_65->setAlignment(Qt::AlignCenter);
        label_69 = new QLabel(tab_2);
        label_69->setObjectName(QString::fromUtf8("label_69"));
        label_69->setGeometry(QRect(1200, 50, 31, 21));
        label_69->setAlignment(Qt::AlignCenter);
        lineEditRange_31 = new CustomControls::RealEdit(tab_2);
        lineEditRange_31->setObjectName(QString::fromUtf8("lineEditRange_31"));
        lineEditRange_31->setGeometry(QRect(1190, 30, 50, 21));
        lineEditRange_31->setMinimumSize(QSize(50, 0));
        tabWidget_2->addTab(tab_2, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        twCompr = new QTableWidget(tab_4);
        if (twCompr->columnCount() < 9)
            twCompr->setColumnCount(9);
        if (twCompr->rowCount() < 5)
            twCompr->setRowCount(5);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        twCompr->setItem(0, 0, __qtablewidgetitem2);
        twCompr->setObjectName(QString::fromUtf8("twCompr"));
        twCompr->setGeometry(QRect(10, 10, 1231, 171));
        twCompr->setRowCount(5);
        twCompr->setColumnCount(9);
        twCompr->horizontalHeader()->setDefaultSectionSize(65);
        twCompr->horizontalHeader()->setMinimumSectionSize(27);
        twCompr->verticalHeader()->setVisible(false);
        twCompr->verticalHeader()->setDefaultSectionSize(23);
        twCompr->verticalHeader()->setHighlightSections(true);
        tabWidget_2->addTab(tab_4, QString());
        cb_plot1 = new CustomControls::ComboBox(tab_3);
        cb_plot1->setObjectName(QString::fromUtf8("cb_plot1"));
        cb_plot1->setGeometry(QRect(1320, 400, 201, 22));
        label_3 = new QLabel(tab_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(1400, 380, 46, 21));
        label_3->setFont(font);
        label_4 = new QLabel(tab_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(1400, 430, 46, 21));
        label_4->setFont(font);
        cb_plot2 = new CustomControls::ComboBox(tab_3);
        cb_plot2->setObjectName(QString::fromUtf8("cb_plot2"));
        cb_plot2->setGeometry(QRect(1318, 450, 201, 22));
        cb_plot3 = new CustomControls::ComboBox(tab_3);
        cb_plot3->setObjectName(QString::fromUtf8("cb_plot3"));
        cb_plot3->setGeometry(QRect(1318, 500, 201, 22));
        label_5 = new QLabel(tab_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(1400, 480, 46, 21));
        label_5->setFont(font);
        label_6 = new QLabel(tab_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(1400, 530, 46, 21));
        label_6->setFont(font);
        cb_plot4 = new CustomControls::ComboBox(tab_3);
        cb_plot4->setObjectName(QString::fromUtf8("cb_plot4"));
        cb_plot4->setGeometry(QRect(1318, 550, 201, 22));
        label_7 = new QLabel(tab_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(1400, 580, 46, 21));
        label_7->setFont(font);
        cb_plot5 = new CustomControls::ComboBox(tab_3);
        cb_plot5->setObjectName(QString::fromUtf8("cb_plot5"));
        cb_plot5->setGeometry(QRect(1320, 600, 201, 22));
        label_8 = new QLabel(tab_3);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(1400, 630, 46, 21));
        label_8->setFont(font);
        cb_plot6 = new CustomControls::ComboBox(tab_3);
        cb_plot6->setObjectName(QString::fromUtf8("cb_plot6"));
        cb_plot6->setGeometry(QRect(1318, 650, 201, 22));
        label_9 = new QLabel(tab_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(1400, 680, 46, 21));
        label_9->setFont(font);
        cb_plot7 = new CustomControls::ComboBox(tab_3);
        cb_plot7->setObjectName(QString::fromUtf8("cb_plot7"));
        cb_plot7->setGeometry(QRect(1320, 700, 201, 22));
        cb_plot8 = new CustomControls::ComboBox(tab_3);
        cb_plot8->setObjectName(QString::fromUtf8("cb_plot8"));
        cb_plot8->setGeometry(QRect(1318, 750, 201, 22));
        label_10 = new QLabel(tab_3);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(1400, 730, 46, 21));
        label_10->setFont(font);
        pbSaveCSV = new CustomControls::PushButton(tab_3);
        pbSaveCSV->setObjectName(QString::fromUtf8("pbSaveCSV"));
        pbSaveCSV->setGeometry(QRect(1350, 800, 141, 31));
        pbSaveCSV->setCheckable(false);
        pbSaveTOF = new CustomControls::PushButton(tab_3);
        pbSaveTOF->setObjectName(QString::fromUtf8("pbSaveTOF"));
        pbSaveTOF->setGeometry(QRect(1350, 840, 141, 31));
        pbSaveTOF->setCheckable(false);
        pbSaveTOF_2 = new CustomControls::PushButton(tab_3);
        pbSaveTOF_2->setObjectName(QString::fromUtf8("pbSaveTOF_2"));
        pbSaveTOF_2->setGeometry(QRect(1350, 880, 141, 31));
        pbSaveTOF_2->setCheckable(false);
        cb_updatetable = new CustomControls::CheckBox(tab_3);
        cb_updatetable->setObjectName(QString::fromUtf8("cb_updatetable"));
        cb_updatetable->setGeometry(QRect(400, 730, 171, 17));
        tabWidget->addTab(tab_3, QString());
        DynamicScaling = new QWidget();
        DynamicScaling->setObjectName(QString::fromUtf8("DynamicScaling"));
        groupBox = new QGroupBox(DynamicScaling);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 20, 681, 631));
        ptDynScale = new QCustomPlot(groupBox);
        ptDynScale->setObjectName(QString::fromUtf8("ptDynScale"));
        ptDynScale->setGeometry(QRect(390, 20, 271, 531));
        checkBox = new CustomControls::CheckBox(groupBox);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(130, 560, 151, 20));
        checkBox->setLayoutDirection(Qt::LeftToRight);
        frame_3 = new QFrame(groupBox);
        frame_3->setObjectName(QString::fromUtf8("frame_3"));
        frame_3->setGeometry(QRect(0, 20, 381, 261));
        frame_3->setFrameShape(QFrame::StyledPanel);
        frame_3->setFrameShadow(QFrame::Raised);
        label_148 = new QLabel(frame_3);
        label_148->setObjectName(QString::fromUtf8("label_148"));
        label_148->setGeometry(QRect(180, 230, 111, 21));
        label_148->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_dt0 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_dt0->setObjectName(QString::fromUtf8("le_dyn_sc_dt0"));
        le_dyn_sc_dt0->setGeometry(QRect(120, 80, 61, 20));
        le_dyn_sc_dt0->setReadOnly(true);
        label_142 = new QLabel(frame_3);
        label_142->setObjectName(QString::fromUtf8("label_142"));
        label_142->setGeometry(QRect(180, 140, 111, 21));
        label_142->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_dt5 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_dt5->setObjectName(QString::fromUtf8("le_dyn_sc_dt5"));
        le_dyn_sc_dt5->setGeometry(QRect(120, 230, 61, 20));
        label_131 = new QLabel(frame_3);
        label_131->setObjectName(QString::fromUtf8("label_131"));
        label_131->setGeometry(QRect(180, 110, 111, 21));
        label_131->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_dt4 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_dt4->setObjectName(QString::fromUtf8("le_dyn_sc_dt4"));
        le_dyn_sc_dt4->setGeometry(QRect(120, 200, 61, 20));
        le_dyn_sc_dt1 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_dt1->setObjectName(QString::fromUtf8("le_dyn_sc_dt1"));
        le_dyn_sc_dt1->setGeometry(QRect(120, 110, 61, 20));
        le_dyn_sc_dt2 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_dt2->setObjectName(QString::fromUtf8("le_dyn_sc_dt2"));
        le_dyn_sc_dt2->setGeometry(QRect(120, 140, 61, 20));
        label_60 = new QLabel(frame_3);
        label_60->setObjectName(QString::fromUtf8("label_60"));
        label_60->setGeometry(QRect(30, 40, 161, 31));
        label_60->setFont(font);
        label_60->setStyleSheet(QString::fromUtf8("color: rgb(233, 125, 46);"));
        label_60->setAlignment(Qt::AlignCenter);
        le_dyn_sc_val5 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_val5->setObjectName(QString::fromUtf8("le_dyn_sc_val5"));
        le_dyn_sc_val5->setGeometry(QRect(300, 230, 61, 20));
        le_dyn_sc_val2 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_val2->setObjectName(QString::fromUtf8("le_dyn_sc_val2"));
        le_dyn_sc_val2->setGeometry(QRect(300, 140, 61, 20));
        le_dyn_sc_val4 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_val4->setObjectName(QString::fromUtf8("le_dyn_sc_val4"));
        le_dyn_sc_val4->setGeometry(QRect(300, 200, 61, 20));
        label_126 = new QLabel(frame_3);
        label_126->setObjectName(QString::fromUtf8("label_126"));
        label_126->setGeometry(QRect(180, 80, 111, 21));
        label_126->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_129 = new QLabel(frame_3);
        label_129->setObjectName(QString::fromUtf8("label_129"));
        label_129->setGeometry(QRect(0, 80, 111, 21));
        label_129->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_dt3 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_dt3->setObjectName(QString::fromUtf8("le_dyn_sc_dt3"));
        le_dyn_sc_dt3->setGeometry(QRect(120, 170, 61, 20));
        label_141 = new QLabel(frame_3);
        label_141->setObjectName(QString::fromUtf8("label_141"));
        label_141->setGeometry(QRect(0, 140, 111, 21));
        label_141->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_146 = new QLabel(frame_3);
        label_146->setObjectName(QString::fromUtf8("label_146"));
        label_146->setGeometry(QRect(180, 200, 111, 21));
        label_146->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_144 = new QLabel(frame_3);
        label_144->setObjectName(QString::fromUtf8("label_144"));
        label_144->setGeometry(QRect(180, 170, 111, 21));
        label_144->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_143 = new QLabel(frame_3);
        label_143->setObjectName(QString::fromUtf8("label_143"));
        label_143->setGeometry(QRect(0, 170, 111, 21));
        label_143->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_val0 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_val0->setObjectName(QString::fromUtf8("le_dyn_sc_val0"));
        le_dyn_sc_val0->setGeometry(QRect(300, 80, 61, 20));
        le_dyn_sc_val3 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_val3->setObjectName(QString::fromUtf8("le_dyn_sc_val3"));
        le_dyn_sc_val3->setGeometry(QRect(300, 170, 61, 20));
        label_147 = new QLabel(frame_3);
        label_147->setObjectName(QString::fromUtf8("label_147"));
        label_147->setGeometry(QRect(0, 230, 111, 21));
        label_147->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_145 = new QLabel(frame_3);
        label_145->setObjectName(QString::fromUtf8("label_145"));
        label_145->setGeometry(QRect(0, 200, 111, 21));
        label_145->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_63 = new QLabel(frame_3);
        label_63->setObjectName(QString::fromUtf8("label_63"));
        label_63->setGeometry(QRect(210, 40, 161, 31));
        label_63->setFont(font);
        label_63->setStyleSheet(QString::fromUtf8("color: rgb(233, 125, 46);"));
        label_63->setAlignment(Qt::AlignCenter);
        label_130 = new QLabel(frame_3);
        label_130->setObjectName(QString::fromUtf8("label_130"));
        label_130->setGeometry(QRect(0, 110, 111, 21));
        label_130->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_val1 = new CustomControls::RealEdit(frame_3);
        le_dyn_sc_val1->setObjectName(QString::fromUtf8("le_dyn_sc_val1"));
        le_dyn_sc_val1->setGeometry(QRect(300, 110, 61, 20));
        label_66 = new QLabel(frame_3);
        label_66->setObjectName(QString::fromUtf8("label_66"));
        label_66->setGeometry(QRect(0, 0, 291, 51));
        QFont font2;
        font2.setPointSize(14);
        font2.setBold(true);
        font2.setWeight(75);
        label_66->setFont(font2);
        label_66->setStyleSheet(QString::fromUtf8("color: rgb(84, 185, 72);"));
        label_66->setAlignment(Qt::AlignCenter);
        cbDynScChA = new CustomControls::ComboBox(frame_3);
        cbDynScChA->setObjectName(QString::fromUtf8("cbDynScChA"));
        cbDynScChA->setGeometry(QRect(200, 15, 161, 22));
        label_64 = new QLabel(groupBox);
        label_64->setObjectName(QString::fromUtf8("label_64"));
        label_64->setGeometry(QRect(10, 580, 361, 41));
        label_64->setFont(font);
        label_64->setStyleSheet(QString::fromUtf8("color: rgb(233, 125, 46);"));
        label_64->setAlignment(Qt::AlignCenter);
        frame_4 = new QFrame(groupBox);
        frame_4->setObjectName(QString::fromUtf8("frame_4"));
        frame_4->setGeometry(QRect(0, 290, 381, 261));
        frame_4->setFrameShape(QFrame::StyledPanel);
        frame_4->setFrameShadow(QFrame::Raised);
        label_149 = new QLabel(frame_4);
        label_149->setObjectName(QString::fromUtf8("label_149"));
        label_149->setGeometry(QRect(180, 230, 111, 21));
        label_149->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_dt0_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_dt0_2->setObjectName(QString::fromUtf8("le_dyn_sc_dt0_2"));
        le_dyn_sc_dt0_2->setGeometry(QRect(120, 80, 61, 20));
        le_dyn_sc_dt0_2->setReadOnly(true);
        label_150 = new QLabel(frame_4);
        label_150->setObjectName(QString::fromUtf8("label_150"));
        label_150->setGeometry(QRect(180, 140, 111, 21));
        label_150->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_dt5_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_dt5_2->setObjectName(QString::fromUtf8("le_dyn_sc_dt5_2"));
        le_dyn_sc_dt5_2->setGeometry(QRect(120, 230, 61, 20));
        label_151 = new QLabel(frame_4);
        label_151->setObjectName(QString::fromUtf8("label_151"));
        label_151->setGeometry(QRect(180, 110, 111, 21));
        label_151->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_dt4_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_dt4_2->setObjectName(QString::fromUtf8("le_dyn_sc_dt4_2"));
        le_dyn_sc_dt4_2->setGeometry(QRect(120, 200, 61, 20));
        le_dyn_sc_dt1_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_dt1_2->setObjectName(QString::fromUtf8("le_dyn_sc_dt1_2"));
        le_dyn_sc_dt1_2->setGeometry(QRect(120, 110, 61, 20));
        le_dyn_sc_dt2_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_dt2_2->setObjectName(QString::fromUtf8("le_dyn_sc_dt2_2"));
        le_dyn_sc_dt2_2->setGeometry(QRect(120, 140, 61, 20));
        label_67 = new QLabel(frame_4);
        label_67->setObjectName(QString::fromUtf8("label_67"));
        label_67->setGeometry(QRect(30, 40, 161, 31));
        label_67->setFont(font);
        label_67->setStyleSheet(QString::fromUtf8("color: rgb(233, 125, 46);"));
        label_67->setAlignment(Qt::AlignCenter);
        le_dyn_sc_val5_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_val5_2->setObjectName(QString::fromUtf8("le_dyn_sc_val5_2"));
        le_dyn_sc_val5_2->setGeometry(QRect(300, 230, 61, 20));
        le_dyn_sc_val2_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_val2_2->setObjectName(QString::fromUtf8("le_dyn_sc_val2_2"));
        le_dyn_sc_val2_2->setGeometry(QRect(300, 140, 61, 20));
        le_dyn_sc_val4_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_val4_2->setObjectName(QString::fromUtf8("le_dyn_sc_val4_2"));
        le_dyn_sc_val4_2->setGeometry(QRect(300, 200, 61, 20));
        label_152 = new QLabel(frame_4);
        label_152->setObjectName(QString::fromUtf8("label_152"));
        label_152->setGeometry(QRect(180, 80, 111, 21));
        label_152->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_153 = new QLabel(frame_4);
        label_153->setObjectName(QString::fromUtf8("label_153"));
        label_153->setGeometry(QRect(0, 80, 111, 21));
        label_153->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_dt3_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_dt3_2->setObjectName(QString::fromUtf8("le_dyn_sc_dt3_2"));
        le_dyn_sc_dt3_2->setGeometry(QRect(120, 170, 61, 20));
        label_154 = new QLabel(frame_4);
        label_154->setObjectName(QString::fromUtf8("label_154"));
        label_154->setGeometry(QRect(0, 140, 111, 21));
        label_154->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_155 = new QLabel(frame_4);
        label_155->setObjectName(QString::fromUtf8("label_155"));
        label_155->setGeometry(QRect(180, 200, 111, 21));
        label_155->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_156 = new QLabel(frame_4);
        label_156->setObjectName(QString::fromUtf8("label_156"));
        label_156->setGeometry(QRect(180, 170, 111, 21));
        label_156->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_157 = new QLabel(frame_4);
        label_157->setObjectName(QString::fromUtf8("label_157"));
        label_157->setGeometry(QRect(0, 170, 111, 21));
        label_157->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_val0_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_val0_2->setObjectName(QString::fromUtf8("le_dyn_sc_val0_2"));
        le_dyn_sc_val0_2->setGeometry(QRect(300, 80, 61, 20));
        le_dyn_sc_val3_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_val3_2->setObjectName(QString::fromUtf8("le_dyn_sc_val3_2"));
        le_dyn_sc_val3_2->setGeometry(QRect(300, 170, 61, 20));
        label_158 = new QLabel(frame_4);
        label_158->setObjectName(QString::fromUtf8("label_158"));
        label_158->setGeometry(QRect(0, 230, 111, 21));
        label_158->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_159 = new QLabel(frame_4);
        label_159->setObjectName(QString::fromUtf8("label_159"));
        label_159->setGeometry(QRect(0, 200, 111, 21));
        label_159->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_68 = new QLabel(frame_4);
        label_68->setObjectName(QString::fromUtf8("label_68"));
        label_68->setGeometry(QRect(210, 40, 161, 31));
        label_68->setFont(font);
        label_68->setStyleSheet(QString::fromUtf8("color: rgb(233, 125, 46);"));
        label_68->setAlignment(Qt::AlignCenter);
        label_160 = new QLabel(frame_4);
        label_160->setObjectName(QString::fromUtf8("label_160"));
        label_160->setGeometry(QRect(0, 110, 111, 21));
        label_160->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_dyn_sc_val1_2 = new CustomControls::RealEdit(frame_4);
        le_dyn_sc_val1_2->setObjectName(QString::fromUtf8("le_dyn_sc_val1_2"));
        le_dyn_sc_val1_2->setGeometry(QRect(300, 110, 61, 20));
        label_70 = new QLabel(frame_4);
        label_70->setObjectName(QString::fromUtf8("label_70"));
        label_70->setGeometry(QRect(0, 0, 291, 51));
        label_70->setFont(font2);
        label_70->setStyleSheet(QString::fromUtf8("color: red;"));
        label_70->setAlignment(Qt::AlignCenter);
        cbDynScChB = new CustomControls::ComboBox(frame_4);
        cbDynScChB->setObjectName(QString::fromUtf8("cbDynScChB"));
        cbDynScChB->setGeometry(QRect(200, 15, 161, 22));
        line = new QFrame(groupBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(7, 280, 371, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        tabWidget->addTab(DynamicScaling, QString());
        groupBoxCFAR = new QGroupBox(CFARsim);
        groupBoxCFAR->setObjectName(QString::fromUtf8("groupBoxCFAR"));
        groupBoxCFAR->setEnabled(true);
        groupBoxCFAR->setGeometry(QRect(230, 10, 801, 151));
        groupBoxCFAR->setStyleSheet(QString::fromUtf8(""));
        label_43 = new QLabel(groupBoxCFAR);
        label_43->setObjectName(QString::fromUtf8("label_43"));
        label_43->setGeometry(QRect(10, 50, 111, 21));
        label_43->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_124 = new QLabel(groupBoxCFAR);
        label_124->setObjectName(QString::fromUtf8("label_124"));
        label_124->setGeometry(QRect(10, 30, 111, 21));
        label_124->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_199 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_199->setObjectName(QString::fromUtf8("lineEdit_199"));
        lineEdit_199->setGeometry(QRect(130, 90, 41, 20));
        label_127 = new QLabel(groupBoxCFAR);
        label_127->setObjectName(QString::fromUtf8("label_127"));
        label_127->setGeometry(QRect(10, 70, 111, 21));
        label_127->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_132 = new QLabel(groupBoxCFAR);
        label_132->setObjectName(QString::fromUtf8("label_132"));
        label_132->setGeometry(QRect(10, 90, 111, 21));
        label_132->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_200 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_200->setObjectName(QString::fromUtf8("lineEdit_200"));
        lineEdit_200->setGeometry(QRect(130, 70, 41, 20));
        lineEdit_201 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_201->setObjectName(QString::fromUtf8("lineEdit_201"));
        lineEdit_201->setGeometry(QRect(130, 50, 41, 20));
        lineEdit_202 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_202->setObjectName(QString::fromUtf8("lineEdit_202"));
        lineEdit_202->setGeometry(QRect(130, 30, 41, 20));
        label_133 = new QLabel(groupBoxCFAR);
        label_133->setObjectName(QString::fromUtf8("label_133"));
        label_133->setGeometry(QRect(10, 110, 111, 21));
        label_133->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_203 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_203->setObjectName(QString::fromUtf8("lineEdit_203"));
        lineEdit_203->setGeometry(QRect(130, 110, 41, 20));
        label_128 = new QLabel(groupBoxCFAR);
        label_128->setObjectName(QString::fromUtf8("label_128"));
        label_128->setGeometry(QRect(580, 105, 111, 21));
        label_128->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_204 = new CustomControls::RealEdit(groupBoxCFAR);
        lineEdit_204->setObjectName(QString::fromUtf8("lineEdit_204"));
        lineEdit_204->setGeometry(QRect(700, 105, 41, 20));
        lineEdit_205 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_205->setObjectName(QString::fromUtf8("lineEdit_205"));
        lineEdit_205->setGeometry(QRect(330, 110, 41, 20));
        label_134 = new QLabel(groupBoxCFAR);
        label_134->setObjectName(QString::fromUtf8("label_134"));
        label_134->setGeometry(QRect(190, 110, 131, 21));
        label_134->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pbCalcCFAR = new CustomControls::PushButton(groupBoxCFAR);
        pbCalcCFAR->setObjectName(QString::fromUtf8("pbCalcCFAR"));
        pbCalcCFAR->setGeometry(QRect(600, 10, 171, 26));
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(true);
        font3.setWeight(75);
        pbCalcCFAR->setFont(font3);
        pbCalcCFAR->setCheckable(false);
        lineEdit_206 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_206->setObjectName(QString::fromUtf8("lineEdit_206"));
        lineEdit_206->setGeometry(QRect(330, 70, 41, 20));
        label_138 = new QLabel(groupBoxCFAR);
        label_138->setObjectName(QString::fromUtf8("label_138"));
        label_138->setGeometry(QRect(190, 70, 131, 21));
        label_138->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_135 = new QLabel(groupBoxCFAR);
        label_135->setObjectName(QString::fromUtf8("label_135"));
        label_135->setGeometry(QRect(380, 30, 131, 21));
        label_135->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_207 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_207->setObjectName(QString::fromUtf8("lineEdit_207"));
        lineEdit_207->setGeometry(QRect(520, 30, 41, 20));
        label_139 = new QLabel(groupBoxCFAR);
        label_139->setObjectName(QString::fromUtf8("label_139"));
        label_139->setGeometry(QRect(380, 50, 131, 21));
        label_139->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_208 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_208->setObjectName(QString::fromUtf8("lineEdit_208"));
        lineEdit_208->setGeometry(QRect(520, 50, 41, 20));
        label_55 = new QLabel(groupBoxCFAR);
        label_55->setObjectName(QString::fromUtf8("label_55"));
        label_55->setGeometry(QRect(310, 10, 81, 21));
        label_55->setFont(font);
        label_55->setStyleSheet(QString::fromUtf8("color: rgb(233, 125, 46);"));
        label_55->setAlignment(Qt::AlignCenter);
        label_58 = new QLabel(groupBoxCFAR);
        label_58->setObjectName(QString::fromUtf8("label_58"));
        label_58->setGeometry(QRect(100, 10, 141, 21));
        label_58->setFont(font);
        label_58->setStyleSheet(QString::fromUtf8("color: rgb(233, 125, 46);"));
        label_58->setAlignment(Qt::AlignCenter);
        label_59 = new QLabel(groupBoxCFAR);
        label_59->setObjectName(QString::fromUtf8("label_59"));
        label_59->setGeometry(QRect(500, 10, 81, 21));
        label_59->setFont(font);
        label_59->setStyleSheet(QString::fromUtf8("color: rgb(233, 125, 46);"));
        label_59->setAlignment(Qt::AlignCenter);
        label_140 = new QLabel(groupBoxCFAR);
        label_140->setObjectName(QString::fromUtf8("label_140"));
        label_140->setGeometry(QRect(190, 90, 131, 21));
        label_140->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_209 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_209->setObjectName(QString::fromUtf8("lineEdit_209"));
        lineEdit_209->setGeometry(QRect(330, 90, 41, 20));
        lineEdit_210 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_210->setObjectName(QString::fromUtf8("lineEdit_210"));
        lineEdit_210->setGeometry(QRect(330, 50, 41, 20));
        label_125 = new QLabel(groupBoxCFAR);
        label_125->setObjectName(QString::fromUtf8("label_125"));
        label_125->setGeometry(QRect(190, 30, 131, 21));
        label_125->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        lineEdit_211 = new CustomControls::LineEdit(groupBoxCFAR);
        lineEdit_211->setObjectName(QString::fromUtf8("lineEdit_211"));
        lineEdit_211->setGeometry(QRect(330, 30, 41, 20));
        label_44 = new QLabel(groupBoxCFAR);
        label_44->setObjectName(QString::fromUtf8("label_44"));
        label_44->setGeometry(QRect(190, 50, 131, 21));
        label_44->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        frame = new QFrame(groupBoxCFAR);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(580, 40, 211, 61));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        label_11 = new QLabel(frame);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(0, 0, 201, 21));
        label_11->setFont(font);
        radioButtonCFARv1 = new CustomControls::RadioButton(frame);
        radioButtonCFARv1->setObjectName(QString::fromUtf8("radioButtonCFARv1"));
        radioButtonCFARv1->setGeometry(QRect(0, 20, 211, 17));
        radioButtonCFARv1->setLayoutDirection(Qt::LeftToRight);
        radioButtonCFARv1->setChecked(true);
        radioButtonCFARv2 = new CustomControls::RadioButton(frame);
        radioButtonCFARv2->setObjectName(QString::fromUtf8("radioButtonCFARv2"));
        radioButtonCFARv2->setGeometry(QRect(0, 40, 211, 17));
        frame_2 = new QFrame(groupBoxCFAR);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setGeometry(QRect(400, 70, 171, 61));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        rbSourceMag = new CustomControls::RadioButton(frame_2);
        rbSourceMag->setObjectName(QString::fromUtf8("rbSourceMag"));
        rbSourceMag->setGeometry(QRect(10, 40, 161, 17));
        rbSourceToF = new CustomControls::RadioButton(frame_2);
        rbSourceToF->setObjectName(QString::fromUtf8("rbSourceToF"));
        rbSourceToF->setGeometry(QRect(10, 20, 161, 17));
        rbSourceToF->setChecked(true);
        label_18 = new QLabel(frame_2);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 0, 161, 21));
        label_18->setFont(font);
        checkBox_2 = new CustomControls::CheckBox(groupBoxCFAR);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(600, 125, 151, 20));
        checkBox_2->setLayoutDirection(Qt::LeftToRight);
        groupBox_IO = new QGroupBox(CFARsim);
        groupBox_IO->setObjectName(QString::fromUtf8("groupBox_IO"));
        groupBox_IO->setGeometry(QRect(9, 9, 201, 141));
        pbLoadCSV = new CustomControls::PushButton(groupBox_IO);
        pbLoadCSV->setObjectName(QString::fromUtf8("pbLoadCSV"));
        pbLoadCSV->setGeometry(QRect(20, 20, 161, 21));
        pbLoadCSV->setCheckable(false);
        pbLoadMeas = new CustomControls::PushButton(groupBox_IO);
        pbLoadMeas->setObjectName(QString::fromUtf8("pbLoadMeas"));
        pbLoadMeas->setGeometry(QRect(20, 50, 161, 21));
        pbLoadMeas->setCheckable(false);
        cb_delim = new CustomControls::ComboBox(groupBox_IO);
        cb_delim->setObjectName(QString::fromUtf8("cb_delim"));
        cb_delim->setGeometry(QRect(130, 80, 41, 21));
        label = new QLabel(groupBox_IO);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 80, 111, 20));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        le_LegendLine = new CustomControls::LineEdit(groupBox_IO);
        le_LegendLine->setObjectName(QString::fromUtf8("le_LegendLine"));
        le_LegendLine->setGeometry(QRect(130, 110, 41, 21));
        le_LegendLine->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_2 = new QLabel(groupBox_IO);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 110, 111, 21));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        groupBoxCompr = new QGroupBox(CFARsim);
        groupBoxCompr->setObjectName(QString::fromUtf8("groupBoxCompr"));
        groupBoxCompr->setGeometry(QRect(1050, 10, 511, 141));
        cb_cmpr_cfaren_1 = new CustomControls::CheckBox(groupBoxCompr);
        cb_cmpr_cfaren_1->setObjectName(QString::fromUtf8("cb_cmpr_cfaren_1"));
        cb_cmpr_cfaren_1->setGeometry(QRect(140, 40, 71, 21));
        cb_cmpr_cpren_1 = new CustomControls::CheckBox(groupBoxCompr);
        cb_cmpr_cpren_1->setObjectName(QString::fromUtf8("cb_cmpr_cpren_1"));
        cb_cmpr_cpren_1->setGeometry(QRect(210, 40, 111, 21));
        cb_cmpr_cpren_2 = new CustomControls::CheckBox(groupBoxCompr);
        cb_cmpr_cpren_2->setObjectName(QString::fromUtf8("cb_cmpr_cpren_2"));
        cb_cmpr_cpren_2->setGeometry(QRect(210, 70, 111, 21));
        cb_cmpr_cfaren_2 = new CustomControls::CheckBox(groupBoxCompr);
        cb_cmpr_cfaren_2->setObjectName(QString::fromUtf8("cb_cmpr_cfaren_2"));
        cb_cmpr_cfaren_2->setGeometry(QRect(140, 70, 71, 21));
        cb_cmpr_cpren_3 = new CustomControls::CheckBox(groupBoxCompr);
        cb_cmpr_cpren_3->setObjectName(QString::fromUtf8("cb_cmpr_cpren_3"));
        cb_cmpr_cpren_3->setGeometry(QRect(210, 100, 111, 21));
        cb_cmpr_cfaren_3 = new CustomControls::CheckBox(groupBoxCompr);
        cb_cmpr_cfaren_3->setObjectName(QString::fromUtf8("cb_cmpr_cfaren_3"));
        cb_cmpr_cfaren_3->setGeometry(QRect(140, 100, 71, 21));
        label_136 = new QLabel(groupBoxCompr);
        label_136->setObjectName(QString::fromUtf8("label_136"));
        label_136->setGeometry(QRect(400, 20, 101, 21));
        label_136->setFont(font);
        label_136->setAlignment(Qt::AlignCenter);
        label_12 = new QLabel(groupBoxCompr);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(90, 40, 51, 21));
        label_13 = new QLabel(groupBoxCompr);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(90, 70, 51, 21));
        label_14 = new QLabel(groupBoxCompr);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(90, 100, 51, 21));
        pbUpDBG = new CustomControls::PushButton(groupBoxCompr);
        pbUpDBG->setObjectName(QString::fromUtf8("pbUpDBG"));
        pbUpDBG->setGeometry(QRect(210, 5, 181, 26));
        QFont font4;
        font4.setPointSize(8);
        font4.setBold(true);
        font4.setWeight(75);
        pbUpDBG->setFont(font4);
        pbUpDBG->setCheckable(false);
        cb_dbgin_1 = new CustomControls::ComboBox(groupBoxCompr);
        cb_dbgin_1->setObjectName(QString::fromUtf8("cb_dbgin_1"));
        cb_dbgin_1->setGeometry(QRect(10, 40, 71, 22));
        cb_dbgin_2 = new CustomControls::ComboBox(groupBoxCompr);
        cb_dbgin_2->setObjectName(QString::fromUtf8("cb_dbgin_2"));
        cb_dbgin_2->setGeometry(QRect(10, 70, 71, 22));
        cb_dbgin_3 = new CustomControls::ComboBox(groupBoxCompr);
        cb_dbgin_3->setObjectName(QString::fromUtf8("cb_dbgin_3"));
        cb_dbgin_3->setGeometry(QRect(10, 100, 71, 22));
        label_137 = new QLabel(groupBoxCompr);
        label_137->setObjectName(QString::fromUtf8("label_137"));
        label_137->setGeometry(QRect(0, 20, 121, 21));
        label_137->setFont(font);
        label_137->setAlignment(Qt::AlignCenter);
        label_15 = new QLabel(groupBoxCompr);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(390, 40, 111, 21));
        label_16 = new QLabel(groupBoxCompr);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(390, 70, 111, 21));
        label_17 = new QLabel(groupBoxCompr);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(390, 100, 111, 21));
        cb_sampling_1 = new CustomControls::ComboBox(groupBoxCompr);
        cb_sampling_1->setObjectName(QString::fromUtf8("cb_sampling_1"));
        cb_sampling_1->setGeometry(QRect(320, 40, 71, 22));
        cb_sampling_2 = new CustomControls::ComboBox(groupBoxCompr);
        cb_sampling_2->setObjectName(QString::fromUtf8("cb_sampling_2"));
        cb_sampling_2->setGeometry(QRect(320, 70, 71, 22));
        cb_sampling_3 = new CustomControls::ComboBox(groupBoxCompr);
        cb_sampling_3->setObjectName(QString::fromUtf8("cb_sampling_3"));
        cb_sampling_3->setGeometry(QRect(320, 100, 71, 22));

        retranslateUi(CFARsim);

        tabWidget->setCurrentIndex(0);
        tabWidget_2->setCurrentIndex(0);

    } // setupUi

    void retranslateUi(QWidget *CFARsim)
    {
        CFARsim->setWindowTitle(QApplication::translate("CFARsim", "UartController2", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButtonUpdate_49->setAccessibleName(QApplication::translate("CFARsim", "CopyGraph(sim)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButtonUpdate_49->setText(QApplication::translate("CFARsim", "Copy graph", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pushButtonUpdate_17->setAccessibleName(QApplication::translate("CFARsim", "SaveGraph(sim)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pushButtonUpdate_17->setText(QApplication::translate("CFARsim", "Save graph", 0, QApplication::UnicodeUTF8));
        label_57->setText(QApplication::translate("CFARsim", "ms", 0, QApplication::UnicodeUTF8));
        label_61->setText(QApplication::translate("CFARsim", "Start:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditRange_start->setAccessibleName(QApplication::translate("CFARsim", "SUMMARY_STARTMS", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditRange_start->setInputMask(QString());
        lineEditRange_start->setText(QApplication::translate("CFARsim", "0", 0, QApplication::UnicodeUTF8));
        label_56->setText(QApplication::translate("CFARsim", "End:", 0, QApplication::UnicodeUTF8));
        label_62->setText(QApplication::translate("CFARsim", "ms", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditRange_stop->setAccessibleName(QApplication::translate("CFARsim", "SUMMARY_STOPMS", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditRange_stop->setInputMask(QString());
        lineEditRange_stop->setText(QApplication::translate("CFARsim", "10", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled = twSamples->isSortingEnabled();
        twSamples->setSortingEnabled(false);
        twSamples->setSortingEnabled(__sortingEnabled);

        tabWidget_2->setTabText(tabWidget_2->indexOf(tab), QApplication::translate("CFARsim", "Samples", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled1 = twSamplesToF->isSortingEnabled();
        twSamplesToF->setSortingEnabled(false);
        twSamplesToF->setSortingEnabled(__sortingEnabled1);

        label_65->setText(QApplication::translate("CFARsim", "TOF_CALIB", 0, QApplication::UnicodeUTF8));
        label_69->setText(QApplication::translate("CFARsim", "ms", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEditRange_31->setAccessibleName(QApplication::translate("CFARsim", "TOF_OFFSET", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEditRange_31->setInputMask(QString());
        lineEditRange_31->setText(QApplication::translate("CFARsim", "0", 0, QApplication::UnicodeUTF8));
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_2), QApplication::translate("CFARsim", "ToF (for CFAR only)", 0, QApplication::UnicodeUTF8));

        const bool __sortingEnabled2 = twCompr->isSortingEnabled();
        twCompr->setSortingEnabled(false);
        twCompr->setSortingEnabled(__sortingEnabled2);

        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("CFARsim", "Compr./Decompr.", 0, QApplication::UnicodeUTF8));
        cb_plot1->clear();
        cb_plot1->insertItems(0, QStringList()
         << QString()
        );
        label_3->setText(QApplication::translate("CFARsim", "Plot 1", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CFARsim", "Plot 2", 0, QApplication::UnicodeUTF8));
        cb_plot2->clear();
        cb_plot2->insertItems(0, QStringList()
         << QString()
        );
        cb_plot3->clear();
        cb_plot3->insertItems(0, QStringList()
         << QString()
        );
        label_5->setText(QApplication::translate("CFARsim", "Plot 3", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("CFARsim", "Plot 4", 0, QApplication::UnicodeUTF8));
        cb_plot4->clear();
        cb_plot4->insertItems(0, QStringList()
         << QString()
        );
        label_7->setText(QApplication::translate("CFARsim", "Plot 5", 0, QApplication::UnicodeUTF8));
        cb_plot5->clear();
        cb_plot5->insertItems(0, QStringList()
         << QString()
        );
        label_8->setText(QApplication::translate("CFARsim", "Plot 6", 0, QApplication::UnicodeUTF8));
        cb_plot6->clear();
        cb_plot6->insertItems(0, QStringList()
         << QString()
        );
        label_9->setText(QApplication::translate("CFARsim", "Plot 7", 0, QApplication::UnicodeUTF8));
        cb_plot7->clear();
        cb_plot7->insertItems(0, QStringList()
         << QString()
        );
        cb_plot8->clear();
        cb_plot8->insertItems(0, QStringList()
         << QString()
        );
        label_10->setText(QApplication::translate("CFARsim", "Plot 8", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pbSaveCSV->setAccessibleName(QApplication::translate("CFARsim", "SaveCSV(samples)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pbSaveCSV->setText(QApplication::translate("CFARsim", "Save samples to CSV", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pbSaveTOF->setAccessibleName(QApplication::translate("CFARsim", "SaveCSV(tof)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pbSaveTOF->setText(QApplication::translate("CFARsim", "Save ToF to CSV", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pbSaveTOF_2->setAccessibleName(QApplication::translate("CFARsim", "SaveCSV(compr)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pbSaveTOF_2->setText(QApplication::translate("CFARsim", "Save Compr. to CSV", 0, QApplication::UnicodeUTF8));
        cb_updatetable->setText(QApplication::translate("CFARsim", "Show results in table", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("CFARsim", "CFAR Simulation", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("CFARsim", "Dynamic Scaling", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_ENA", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox->setText(QApplication::translate("CFARsim", "Dynamic Scaling Enable", 0, QApplication::UnicodeUTF8));
        label_148->setText(QApplication::translate("CFARsim", "dyn_dscale5 [dB]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt0->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_0", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_142->setText(QApplication::translate("CFARsim", "dyn_dscale2 [dB]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt5->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_5", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_131->setText(QApplication::translate("CFARsim", "dyn_dscale1 [dB]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt4->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_4", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt1->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_1", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_2", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_60->setText(QApplication::translate("CFARsim", "Time (delta)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val5->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_5", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_2", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val4->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_4", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_126->setText(QApplication::translate("CFARsim", "dyn_dscale0 [dB]", 0, QApplication::UnicodeUTF8));
        label_129->setText(QApplication::translate("CFARsim", "dyn_dt0 [ms]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt3->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_3", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_141->setText(QApplication::translate("CFARsim", "dyn_dt2 [ms]", 0, QApplication::UnicodeUTF8));
        label_146->setText(QApplication::translate("CFARsim", "dyn_dscale4 [dB]", 0, QApplication::UnicodeUTF8));
        label_144->setText(QApplication::translate("CFARsim", "dyn_dscale3 [dB]", 0, QApplication::UnicodeUTF8));
        label_143->setText(QApplication::translate("CFARsim", "dyn_dt3 [ms]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val0->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_0", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val3->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_3", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_147->setText(QApplication::translate("CFARsim", "dyn_dt5 [ms]", 0, QApplication::UnicodeUTF8));
        label_145->setText(QApplication::translate("CFARsim", "dyn_dt4 [ms]", 0, QApplication::UnicodeUTF8));
        label_63->setText(QApplication::translate("CFARsim", "Scaling factor (delta) \n"
"[-60.0 to +60.0 dB]", 0, QApplication::UnicodeUTF8));
        label_130->setText(QApplication::translate("CFARsim", "dyn_dt1 [ms]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val1->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_1", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_66->setText(QApplication::translate("CFARsim", "Input A: ", 0, QApplication::UnicodeUTF8));
        label_64->setText(QApplication::translate("CFARsim", "Scaling is applied to the \n"
"output of 1st stage CFAR / input of 2nd stage CFAR", 0, QApplication::UnicodeUTF8));
        label_149->setText(QApplication::translate("CFARsim", "dyn_dscale5 [dB]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt0_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_0_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_150->setText(QApplication::translate("CFARsim", "dyn_dscale2 [dB]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt5_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_5_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_151->setText(QApplication::translate("CFARsim", "dyn_dscale1 [dB]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt4_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_4_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt1_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_1_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt2_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_2_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_67->setText(QApplication::translate("CFARsim", "Time (delta)", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val5_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_5_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val2_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_2_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val4_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_4_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_152->setText(QApplication::translate("CFARsim", "dyn_dscale0 [dB]", 0, QApplication::UnicodeUTF8));
        label_153->setText(QApplication::translate("CFARsim", "dyn_dt0 [ms]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_dt3_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_TIME_3_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_154->setText(QApplication::translate("CFARsim", "dyn_dt2 [ms]", 0, QApplication::UnicodeUTF8));
        label_155->setText(QApplication::translate("CFARsim", "dyn_dscale4 [dB]", 0, QApplication::UnicodeUTF8));
        label_156->setText(QApplication::translate("CFARsim", "dyn_dscale3 [dB]", 0, QApplication::UnicodeUTF8));
        label_157->setText(QApplication::translate("CFARsim", "dyn_dt3 [ms]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val0_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_0_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val3_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_3_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_158->setText(QApplication::translate("CFARsim", "dyn_dt5 [ms]", 0, QApplication::UnicodeUTF8));
        label_159->setText(QApplication::translate("CFARsim", "dyn_dt4 [ms]", 0, QApplication::UnicodeUTF8));
        label_68->setText(QApplication::translate("CFARsim", "Scaling factor (delta) \n"
"[-60.0 to +60.0 dB]", 0, QApplication::UnicodeUTF8));
        label_160->setText(QApplication::translate("CFARsim", "dyn_dt1 [ms]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_dyn_sc_val1_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_VAL_1_B", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_70->setText(QApplication::translate("CFARsim", "Input B: ", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(DynamicScaling), QApplication::translate("CFARsim", "Dynamic Scaling", 0, QApplication::UnicodeUTF8));
        groupBoxCFAR->setTitle(QApplication::translate("CFARsim", "CFAR simulation", 0, QApplication::UnicodeUTF8));
        label_43->setText(QApplication::translate("CFARsim", "cfar_scale [3b]", 0, QApplication::UnicodeUTF8));
        label_124->setText(QApplication::translate("CFARsim", "cfar_thr_off [8b]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_199->setAccessibleName(QApplication::translate("CFARsim", "CFAR_DER_SIZE", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_127->setText(QApplication::translate("CFARsim", "cfar_der_thr [6b]", 0, QApplication::UnicodeUTF8));
        label_132->setText(QApplication::translate("CFARsim", "cfar_der_size [2b]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_200->setAccessibleName(QApplication::translate("CFARsim", "CFAR_DER_THR", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_201->setAccessibleName(QApplication::translate("CFARsim", "CFAR_SCALE", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_202->setAccessibleName(QApplication::translate("CFARsim", "CFAR_THR_OFF", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_133->setText(QApplication::translate("CFARsim", "cfar_snr [6b]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_203->setAccessibleName(QApplication::translate("CFARsim", "CFAR_SNR", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_128->setText(QApplication::translate("CFARsim", "CFAR start time [ms]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_204->setAccessibleName(QApplication::translate("CFARsim", "CFAR_STARTMS", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        lineEdit_204->setText(QApplication::translate("CFARsim", "0", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_205->setAccessibleName(QApplication::translate("CFARsim", "CFAR_SNR_TOF", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_134->setText(QApplication::translate("CFARsim", "cfar_snr_tof [6b]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pbCalcCFAR->setAccessibleName(QApplication::translate("CFARsim", "CalcCFAR(sim)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pbCalcCFAR->setText(QApplication::translate("CFARsim", "Calculate CFAR", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_206->setAccessibleName(QApplication::translate("CFARsim", "DER_THR_TOF", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_138->setText(QApplication::translate("CFARsim", "cfar_der_thr_tof [6b]", 0, QApplication::UnicodeUTF8));
        label_135->setText(QApplication::translate("CFARsim", "mp_det_thr_pos [8b]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_207->setAccessibleName(QApplication::translate("CFARsim", "MP_DET_THR_POS", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_139->setText(QApplication::translate("CFARsim", "mp_det_thr_neg [8b]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_208->setAccessibleName(QApplication::translate("CFARsim", "MP_DET_THR_NEG", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_55->setText(QApplication::translate("CFARsim", "ToF CFAR", 0, QApplication::UnicodeUTF8));
        label_58->setText(QApplication::translate("CFARsim", "Magnitude CFAR", 0, QApplication::UnicodeUTF8));
        label_59->setText(QApplication::translate("CFARsim", "Multipeak", 0, QApplication::UnicodeUTF8));
        label_140->setText(QApplication::translate("CFARsim", "cfar_der_size_tof [2b]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_209->setAccessibleName(QApplication::translate("CFARsim", "DER_SIZE_TOF", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_210->setAccessibleName(QApplication::translate("CFARsim", "CFAR_SCALE_TOF", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_125->setText(QApplication::translate("CFARsim", "cfar_thr_off_tof [8b]", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        lineEdit_211->setAccessibleName(QApplication::translate("CFARsim", "THR_OFF_TOF", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label_44->setText(QApplication::translate("CFARsim", "cfar_scale_tof [3b]", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("CFARsim", "CFAR version", 0, QApplication::UnicodeUTF8));
        radioButtonCFARv1->setText(QApplication::translate("CFARsim", "New: Separated Mag/ToF CFAR", 0, QApplication::UnicodeUTF8));
        radioButtonCFARv2->setText(QApplication::translate("CFARsim", "Old: Combined Mag+ToF CFAR", 0, QApplication::UnicodeUTF8));
        rbSourceMag->setText(QApplication::translate("CFARsim", "Magnitude CFAR output", 0, QApplication::UnicodeUTF8));
        rbSourceToF->setText(QApplication::translate("CFARsim", "ToF CFAR output", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("CFARsim", "ToF Extraction source", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        checkBox_2->setAccessibleName(QApplication::translate("CFARsim", "DYN_SC_ENA", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        checkBox_2->setText(QApplication::translate("CFARsim", "Dynamic Scaling Enable", 0, QApplication::UnicodeUTF8));
        groupBox_IO->setTitle(QApplication::translate("CFARsim", "Data Input/output", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pbLoadCSV->setAccessibleName(QApplication::translate("CFARsim", "LoadCSV(sim)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pbLoadCSV->setText(QApplication::translate("CFARsim", "Load data from CSV", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pbLoadMeas->setAccessibleName(QApplication::translate("CFARsim", "LoadMeas(sim)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pbLoadMeas->setText(QApplication::translate("CFARsim", "Load data from Meas.", 0, QApplication::UnicodeUTF8));
        cb_delim->clear();
        cb_delim->insertItems(0, QStringList()
         << QApplication::translate("CFARsim", ",", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFARsim", ";", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_ACCESSIBILITY
        cb_delim->setAccessibleName(QApplication::translate("CFARsim", "CSV_DELIM", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        label->setText(QApplication::translate("CFARsim", "CSV delimiter", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        le_LegendLine->setAccessibleName(QApplication::translate("CFARsim", "CSVLegendLine", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        le_LegendLine->setText(QApplication::translate("CFARsim", "3", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CFARsim", "Data legend at line", 0, QApplication::UnicodeUTF8));
        groupBoxCompr->setTitle(QApplication::translate("CFARsim", "Compression/Decompression", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        cb_cmpr_cfaren_1->setAccessibleName(QApplication::translate("CFARsim", "CMPR_DBG_ENA[EN_CFAR1]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        cb_cmpr_cfaren_1->setText(QApplication::translate("CFARsim", "CFAR ->", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        cb_cmpr_cpren_1->setAccessibleName(QApplication::translate("CFARsim", "CMPR_DBG_ENA[EN_CMPR1]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        cb_cmpr_cpren_1->setText(QApplication::translate("CFARsim", "Compr./Decom.", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        cb_cmpr_cpren_2->setAccessibleName(QApplication::translate("CFARsim", "CMPR_DBG_ENA[EN_CMPR2]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        cb_cmpr_cpren_2->setText(QApplication::translate("CFARsim", "Compr./Decom.", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        cb_cmpr_cfaren_2->setAccessibleName(QApplication::translate("CFARsim", "CMPR_DBG_ENA[EN_CFAR2]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        cb_cmpr_cfaren_2->setText(QApplication::translate("CFARsim", "CFAR ->", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        cb_cmpr_cpren_3->setAccessibleName(QApplication::translate("CFARsim", "CMPR_DBG_ENA[EN_CMPR3]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        cb_cmpr_cpren_3->setText(QApplication::translate("CFARsim", "Compr./Decom.", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        cb_cmpr_cfaren_3->setAccessibleName(QApplication::translate("CFARsim", "CMPR_DBG_ENA[EN_CFAR3]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        cb_cmpr_cfaren_3->setText(QApplication::translate("CFARsim", "CFAR ->", 0, QApplication::UnicodeUTF8));
        label_136->setText(QApplication::translate("CFARsim", "Debug output", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("CFARsim", "Input ->", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("CFARsim", "Input ->", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("CFARsim", "Input ->", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_ACCESSIBILITY
        pbUpDBG->setAccessibleName(QApplication::translate("CFARsim", "UpdateDBG(sim)", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        pbUpDBG->setText(QApplication::translate("CFARsim", "Update debug output", 0, QApplication::UnicodeUTF8));
        cb_dbgin_1->clear();
        cb_dbgin_1->insertItems(0, QStringList()
         << QString()
        );
#ifndef QT_NO_ACCESSIBILITY
        cb_dbgin_1->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        cb_dbgin_2->clear();
        cb_dbgin_2->insertItems(0, QStringList()
         << QString()
        );
#ifndef QT_NO_ACCESSIBILITY
        cb_dbgin_2->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        cb_dbgin_3->clear();
        cb_dbgin_3->insertItems(0, QStringList()
         << QString()
        );
#ifndef QT_NO_ACCESSIBILITY
        cb_dbgin_3->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        label_137->setText(QApplication::translate("CFARsim", "Channel selection", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("CFARsim", " --> CMPR_DBG1_xx", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("CFARsim", " --> CMPR_DBG2_xx", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("CFARsim", " --> CMPR_DBG3_xx", 0, QApplication::UnicodeUTF8));
        cb_sampling_1->clear();
        cb_sampling_1->insertItems(0, QStringList()
         << QApplication::translate("CFARsim", "51.2 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFARsim", "102.4 us", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_ACCESSIBILITY
        cb_sampling_1->setAccessibleName(QApplication::translate("CFARsim", "CMPR_DBG_ENA[EN_UNDR1]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        cb_sampling_2->clear();
        cb_sampling_2->insertItems(0, QStringList()
         << QApplication::translate("CFARsim", "51.2 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFARsim", "102.4 us", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_ACCESSIBILITY
        cb_sampling_2->setAccessibleName(QApplication::translate("CFARsim", "CMPR_DBG_ENA[EN_UNDR2]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        cb_sampling_3->clear();
        cb_sampling_3->insertItems(0, QStringList()
         << QApplication::translate("CFARsim", "51.2 us", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("CFARsim", "102.4 us", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_ACCESSIBILITY
        cb_sampling_3->setAccessibleName(QApplication::translate("CFARsim", "CMPR_DBG_ENA[EN_UNDR3]", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
    } // retranslateUi

};

namespace Ui {
    class CFARsim: public Ui_CFARsim {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CFARSIM_H
