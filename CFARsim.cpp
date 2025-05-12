#include "CFARsim.h"
#include "ui_CFARsim.h"
#include "mainapp.h"
#include "QTime"
#include "qclipboard.h"
#include <QString>
#include "measurement.h"
#include <algorithm>
#include <math.h>


CFARsim *gp_CFARsim;


CFARsim::CFARsim(QWidget *parent) :
Applet(parent)
{
    ui = new Ui::CFARsim;
    ui->setupUi(this);
    createCustomUi();
    setWindowIcon(QIcon(":/board/onlogo.ico"));
    setWindowTitle("CFAR simulation");
}


CFARsim::~CFARsim()
{
    delete ui;
}

bool CFARsim::initialize()
{
    CSVLegendLine.makeSetup("@variable=CSVLegendLine" "@value=3" "@min=0" "@max=9999999");
    addDataNode(&CSVLegendLine);
    CFAR_THR_OFF.makeSetup("@variable=CFAR_THR_OFF" "@value=30" "@min=1" "@max=255");
    addDataNode(&CFAR_THR_OFF);
    CFAR_SCALE.makeSetup("@variable=CFAR_SCALE" "@value=3" "@min=0" "@max=7");
    addDataNode(&CFAR_SCALE);
    CFAR_DER_THR.makeSetup("@variable=CFAR_DER_THR" "@value=11" "@min=1" "@max=63");
    addDataNode(&CFAR_DER_THR);
    CFAR_DER_SIZE.makeSetup("@variable=CFAR_DER_SIZE" "@value=3" "@min=0" "@max=3");
    addDataNode(&CFAR_DER_SIZE);
    CFAR_SNR.makeSetup("@variable=CFAR_SNR" "@value=6" "@min=1" "@max=63");
    addDataNode(&CFAR_SNR);
    CFAR_SNR_TOF.makeSetup("@variable=CFAR_SNR_TOF" "@value=6" "@min=1" "@max=63");
    addDataNode(&CFAR_SNR_TOF);
    SUMMARY_STARTMS.makeSetup("@variable=SUMMARY_STARTMS" "@value=0" "@min=0" "@max=150");
    addDataNode(&SUMMARY_STARTMS);
    SUMMARY_STOPMS.makeSetup("@variable=SUMMARY_STOPMS" "@value=50" "@min=0" "@max=150");
    addDataNode(&SUMMARY_STOPMS);
    CFAR_STARTMS.makeSetup("@variable=CFAR_STARTMS" "@value=0" "@min=0" "@max=50");
    addDataNode(&CFAR_STARTMS);
    TOF_REPORTING.makeSetup("@variable=TOF_REPORTING" "@value=0" "@min=0" "@max=50");
    addDataNode(&TOF_REPORTING);
    DER_THR_TOF.makeSetup("@variable=DER_THR_TOF" "@value=11" "@min=1" "@max=63");
    addDataNode(&DER_THR_TOF);
    MP_DET_THR_POS.makeSetup("@variable=MP_DET_THR_POS" "@value=20" "@min=0" "@max=255");
    addDataNode(&MP_DET_THR_POS);
    MP_DET_THR_NEG.makeSetup("@variable=MP_DET_THR_NEG" "@value=20" "@min=0" "@max=255");
    addDataNode(&MP_DET_THR_NEG);
    DER_SIZE_TOF.makeSetup("@variable=DER_SIZE_TOF" "@value=3" "@min=0" "@max=3");
    addDataNode(&DER_SIZE_TOF);
    THR_OFF_TOF.makeSetup("@variable=THR_OFF_TOF" "@value=30" "@min=1" "@max=255");
    addDataNode(&THR_OFF_TOF);
    CFAR_SCALE_TOF.makeSetup("@variable=CFAR_SCALE_TOF" "@value=3" "@min=0" "@max=7");
    addDataNode(&CFAR_SCALE_TOF);

    DYN_SC_VAL_0.makeSetup("@variable=DYN_SC_VAL_0" "@value=0" "@min=-120" "@max=120");
    DYN_SC_VAL_1.makeSetup("@variable=DYN_SC_VAL_1" "@value=0" "@min=-120" "@max=120");
    DYN_SC_VAL_2.makeSetup("@variable=DYN_SC_VAL_2" "@value=0" "@min=-120" "@max=120");
    DYN_SC_VAL_3.makeSetup("@variable=DYN_SC_VAL_3" "@value=0" "@min=-120" "@max=120");
    DYN_SC_VAL_4.makeSetup("@variable=DYN_SC_VAL_4" "@value=0" "@min=-120" "@max=120");
    DYN_SC_VAL_5.makeSetup("@variable=DYN_SC_VAL_5" "@value=0" "@min=-120" "@max=120");
    DYN_SC_TIME_0.makeSetup("@variable=DYN_SC_TIME_0" "@value=0" "@min=0" "@max=0");
    DYN_SC_TIME_1.makeSetup("@variable=DYN_SC_TIME_1" "@value=10" "@min=1" "@max=65");
    DYN_SC_TIME_2.makeSetup("@variable=DYN_SC_TIME_2" "@value=10" "@min=1" "@max=65");
    DYN_SC_TIME_3.makeSetup("@variable=DYN_SC_TIME_3" "@value=10" "@min=1" "@max=65");
    DYN_SC_TIME_4.makeSetup("@variable=DYN_SC_TIME_4" "@value=10" "@min=1" "@max=65");
    DYN_SC_TIME_5.makeSetup("@variable=DYN_SC_TIME_5" "@value=10" "@min=1" "@max=65");
    DYN_SC_VAL_0_B.makeSetup("@variable=DYN_SC_VAL_0_B" "@value=0" "@min=-120" "@max=120");
    DYN_SC_VAL_1_B.makeSetup("@variable=DYN_SC_VAL_1_B" "@value=0" "@min=-120" "@max=120");
    DYN_SC_VAL_2_B.makeSetup("@variable=DYN_SC_VAL_2_B" "@value=0" "@min=-120" "@max=120");
    DYN_SC_VAL_3_B.makeSetup("@variable=DYN_SC_VAL_3_B" "@value=0" "@min=-120" "@max=120");
    DYN_SC_VAL_4_B.makeSetup("@variable=DYN_SC_VAL_4_B" "@value=0" "@min=-120" "@max=120");
    DYN_SC_VAL_5_B.makeSetup("@variable=DYN_SC_VAL_5_B" "@value=0" "@min=-120" "@max=120");
    DYN_SC_TIME_0_B.makeSetup("@variable=DYN_SC_TIME_0_B" "@value=0" "@min=0" "@max=0");
    DYN_SC_TIME_1_B.makeSetup("@variable=DYN_SC_TIME_1_B" "@value=10" "@min=1" "@max=65");
    DYN_SC_TIME_2_B.makeSetup("@variable=DYN_SC_TIME_2_B" "@value=10" "@min=1" "@max=65");
    DYN_SC_TIME_3_B.makeSetup("@variable=DYN_SC_TIME_3_B" "@value=10" "@min=1" "@max=65");
    DYN_SC_TIME_4_B.makeSetup("@variable=DYN_SC_TIME_4_B" "@value=10" "@min=1" "@max=65");
    DYN_SC_TIME_5_B.makeSetup("@variable=DYN_SC_TIME_5_B" "@value=10" "@min=1" "@max=65");
    DYN_SC_ENA.makeSetup("@variable=DYN_SC_ENA" "@value=0" "@min=0" "@max=1");
    addDataNode(&DYN_SC_VAL_0);
    addDataNode(&DYN_SC_VAL_1);
    addDataNode(&DYN_SC_VAL_2);
    addDataNode(&DYN_SC_VAL_3);
    addDataNode(&DYN_SC_VAL_4);
    addDataNode(&DYN_SC_VAL_5);
    addDataNode(&DYN_SC_TIME_0);
    addDataNode(&DYN_SC_TIME_1);
    addDataNode(&DYN_SC_TIME_2);
    addDataNode(&DYN_SC_TIME_3);
    addDataNode(&DYN_SC_TIME_4);
    addDataNode(&DYN_SC_TIME_5);
    addDataNode(&DYN_SC_VAL_0_B);
    addDataNode(&DYN_SC_VAL_1_B);
    addDataNode(&DYN_SC_VAL_2_B);
    addDataNode(&DYN_SC_VAL_3_B);
    addDataNode(&DYN_SC_VAL_4_B);
    addDataNode(&DYN_SC_VAL_5_B);
    addDataNode(&DYN_SC_TIME_0_B);
    addDataNode(&DYN_SC_TIME_1_B);
    addDataNode(&DYN_SC_TIME_2_B);
    addDataNode(&DYN_SC_TIME_3_B);
    addDataNode(&DYN_SC_TIME_4_B);
    addDataNode(&DYN_SC_TIME_5_B);
    addDataNode(&DYN_SC_ENA);


    CMPR_DBG_ENA.makeSetup(ENA_1_4x3_INIT);
    CMPR_DBG_ENA.setName("CMPR_DBG_ENA");
    addDataNode(&CMPR_DBG_ENA);

    addDataNode(&gp_Measurement->TOF_OFFSET);

    PreparePlot();  //prepare plot - axis, range etc.

    connect(ui->cb_plot1, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_PlotData()));
    connect(ui->cb_plot2, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_PlotData()));
    connect(ui->cb_plot3, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_PlotData()));
    connect(ui->cb_plot4, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_PlotData()));
    connect(ui->cb_plot5, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_PlotData()));
    connect(ui->cb_plot6, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_PlotData()));
    connect(ui->cb_plot7, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_PlotData()));
    connect(ui->cb_plot8, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_PlotData()));
    connect(ui->lineEditRange_start, SIGNAL(editingFinished()), this, SLOT(slot_PlotData()));
    connect(ui->lineEditRange_stop, SIGNAL(editingFinished()), this, SLOT(slot_PlotData()));
    connect(ui->cb_dbgin_1, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_updateCB()));
    connect(ui->cb_dbgin_2, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_updateCB()));
    connect(ui->cb_dbgin_3, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_updateCB()));
    connect(ui->radioButtonCFARv1, SIGNAL(clicked()), this, SLOT(slot_updateCFARcontrols()));
    connect(ui->radioButtonCFARv2, SIGNAL(clicked()), this, SLOT(slot_updateCFARcontrols()));
    connect(ui->cb_updatetable, SIGNAL(clicked()), this, SLOT(slot_updateTable()));

    connect(ui->le_dyn_sc_val0, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_val1, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_val2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_val3, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_val4, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_val5, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt0, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt1, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt3, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt4, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt5, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_val0_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_val1_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_val2_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_val3_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_val4_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_val5_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt0_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt1_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt2_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt3_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt4_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));
    connect(ui->le_dyn_sc_dt5_2, SIGNAL(editingFinished()), this, SLOT(slot_updateDynScaling()));

    return true;
}

void CFARsim::createCustomUi()
{
    ui->twSamples->setColumnCount(MAX_CHANNEL_NUMBER+1);
    ui->twSamples->clearContents();
    ui->twSamples->setRowCount(0);
    QStringList m_TableHeader;
    m_TableHeader<<"#"<<"CH1"<<"CH2"<<"CH3"<<"CH4"<<"CH5"<<"CH6"<<"CH7"<<"CH8"<<"CH9"<<"CH10"<<"CH11"<<"CH12";
    ui->twSamples->setHorizontalHeaderLabels(m_TableHeader);

    ui->twSamplesToF->setColumnCount((MAX_CHANNEL_NUMBER*3)+1);
    m_TableHeader.clear();
    m_TableHeader.append("E/S");
    for (int i = 0; i < MAX_CHANNEL_NUMBER; i++) {
        m_TableHeader.append(QString("Ch%1ToF").arg(i+1));
        m_TableHeader.append(QString("Ch%1P").arg(i+1));
        m_TableHeader.append(QString("Ch%1W").arg(i+1));
    }
    ui->twSamplesToF->setHorizontalHeaderLabels(m_TableHeader);
    ui->twSamplesToF->clearContents();
    ui->twSamplesToF->setRowCount(0);

    m_TableHeader.clear();
    ui->twCompr->setColumnCount(6);
    ui->twCompr->setRowCount(0);
    m_TableHeader<<"Time 1 [ms]"<<"Dbg out 1"<<"Time 2 [ms]"<<"Dbg out 2"<<"Time 3 [ms]"<<"Dbg out 3";
    ui->twCompr->setHorizontalHeaderLabels(m_TableHeader);

    //Dynamic Scaling Plot
    ui->ptDynScale->addGraph();
    ui->ptDynScale->graph(0)->setPen(QPen(QColor::fromRgb(84, 185, 72)));
    ui->ptDynScale->addGraph();
    ui->ptDynScale->graph(1)->setPen(QPen(Qt::red));
    ui->ptDynScale->xAxis->setRange(0, 65);
    ui->ptDynScale->yAxis->setRange(-65, 65);
    ui->ptDynScale->xAxis->setLabel("time [ms]");
    //ui->ptDynScale->graph(0)->setName("Input A");
    //ui->ptDynScale->graph(1)->setName("Input B");
    //ui->ptDynScale->legend->setVisible(true);
    //ui->ptDynScale->legend->setPositionStyle(QCPLegend::PositionStyle::psBottom);
    ui->ptDynScale->replot();
    slot_updateDynScaling();

    return;
}

void CFARsim::setupBoard()
{
}


void CFARsim::suspend()
{
}

void CFARsim::resume()
{
    setupBoard();
}

/****************************************************************************
 *                              FUNCTIONALITY
*****************************************************************************/

bool CFARsim::updateHardware(Direction direction, const QString &item) {
    return false;
}

QVariant CFARsim::FUNCTION_LIST()
{
    FUNCTION("LoadCSV", "This functions loads data from selected CSV file. Required format = Time in first column, select correct line where the data legend is ","<i><br> no parameter", "RETURN always true") {
        qDebug() << "Load CSV from file";
        QVector<QVector<double>> output;    //table format - [channel 0-11] [samples 0-...]
        QVector<QString> channel_name;
        float sampling_period = 0;
        uint sample_count = 0;
        quint8 channel_count = 0;
        LoadCSV(output, sampling_period, channel_count, channel_name, sample_count);
        //qDebug() <<"\noutput table\n" << output;
        qDebug() <<"\nsampling_period" << sampling_period;
        qDebug() <<"channel_count" << channel_count;
        qDebug() <<"channel name" << channel_name;
        qDebug() <<"sample_count" << sample_count;
        PrepareData(output, sampling_period, channel_count, sample_count, channel_name);
        /*qDebug() <<"\nsim_samples\n" << sim_samples;
        qDebug() <<"sim_channel_count" << sim_channel_count;
        qDebug() <<"sim_time" << sim_time;*/
        SetupComprComboBox(false);
        SetupPlotComboBox(true);
        CFARinGUI();
        qDebug() << "Load CSV complete";
        return true;
    }

    FUNCTION("LoadMeas", "This functions loads current data from Measurement window","<i><br> no parameter", "RETURN always true") {
        qDebug() << "Load from measurement";
        QVector<QVector<double>> output;    //table format - [channel 0-11] [samples 0-...]
        QVector<QString> channel_name;
        float sampling_period = 0;
        uint sample_count = 0;
        quint8 channel_count = 0;
        LoadFromMeasurement(output, sampling_period, channel_count, channel_name, sample_count);
        /*qDebug() <<"\noutput table\n" << output;
        qDebug() <<"sampling_period" << sampling_period;
        qDebug() <<"channel_count" << channel_count;
        qDebug() <<"channel name" << channel_name;*/
        qDebug() <<"sample_count" << sample_count;
        PrepareData(output, sampling_period, channel_count, sample_count, channel_name);
        /*qDebug() <<"\nsim_samples\n" << sim_samples;
        qDebug() <<"sim_channel_count" << sim_channel_count;
        qDebug() <<"sim_time" << sim_time;*/
        SetupComprComboBox(false);
        SetupPlotComboBox(true);
        CFARinGUI();
        qDebug() << "Load from Measurement complete";
        return true;
    }

    FUNCTION("SaveCSV", "This functions saves Samples table to CSV file","<i><br> filename and path", "RETURN always true") {
        qDebug() << "Save samples to CSV";
        return SaveCSV(p1.toString());
    }

    FUNCTION("SaveTOF", "This functions saves ToF table to CSV file","<i><br> no parameter", "RETURN always true") {
        qDebug() << "Save ToF to CSV";
        return true;
    }

    FUNCTION("CalcCFAR", "This function starts CFAR data processing","<i><br> no parameter", "RETURN always true") {
        qDebug() << "Calculate CFAR";
        CFARinGUI();
        return true;
    }

    FUNCTION("CopyGraph", "This function copies graph to clipboard","<i><br> no parameter", "RETURN always true") {
        qDebug() << "Save ToF to CSV";
        CopyGraph();
        return true;
    }

    FUNCTION("SaveGraph", "This function saves graph as PNG file"
             ,"<i><br> parameter - graph name and path (optional), delimited by one space, example: Simulation5"
             "<i><br> example2: Simulation2 c:/Users/Public/graph.png"
             ,"RETURN always true") {
        //gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Simulation - Save Graph");
        return SaveGraph(p1.toString());
    }

    FUNCTION("UpdateDBG", "This function updates debug outputs","<i><br> no parameter", "RETURN always true") {
        qDebug() << "Update debug";
        slot_updateCB();
        return true;
    }

    return false;
}

/****************************************************************************
 *                            OTHER FUNCTIONS
*****************************************************************************/
//Load data from CSV - parse into samples, sampling rate [ms], channel count [1-12], channel names (from legend) and number of samples
bool CFARsim::LoadCSV(QVector<QVector<double>> &output, float &sampling_period, quint8 &channel_count, QVector<QString> &channel_name, uint &sample_count) {
    //qDebug()<<"Opening CSV";
    QString filename = QFileDialog::getOpenFileName(this, "Open CSV file",QDir::currentPath(), "CSV file(*.csv)");
    QFile file(filename);

    QString input;
    QVector<QVector<double>> csv_out;       //format [radek] [sloupce]
    QStringList onerow;
    int ColumnCount = 0;
    int row_cnt = 0;
    int legend_row = CSVLegendLine.toInt();
    float samplerate = 0;


    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
        return false;
    }
    else {
        QTextStream in(&file);
        while(!in.atEnd()) {
            row_cnt++;
            input = in.readLine();

            onerow = input.split(ui->cb_delim->currentText());      //split by delimiter

            if(row_cnt == legend_row) {                             //first line of real data (legend)
                //qDebug() <<"pocet sloupcu:"<< onerow.length();
                ColumnCount = onerow.length();
                if(ColumnCount > MAX_CHANNEL_NUMBER+1) {
                    QMessageBox::information(0, "error", QString("Maximum number of columns in CSV file is").arg(MAX_CHANNEL_NUMBER+1));
                    return false;
                }
                if(onerow.at(ColumnCount-1) == "") {                //last column is empty. ignore it
                    ColumnCount = ColumnCount-1;
                    //qDebug() <<"pocet sloupcu (korekce):"<< ColumnCount;
                }
                //Parse Legend and insert data choices into combo boxes
                for(int i=1; i<ColumnCount; i++) {
                    channel_name.append(onerow.at(i));
                }
            }
            else if(row_cnt > legend_row) {
                if(onerow.length() == ColumnCount || onerow.length() == ColumnCount+1) {
                    QVector<double> temp(ColumnCount);
                    for(int i=1; i<ColumnCount; i++){
                        temp[i] = onerow.at(i).toDouble();
                    }
                    csv_out.append(temp);

                    if(row_cnt == legend_row+2) {
                        samplerate = onerow.at(0).toDouble();
                    }
                }
                else {
                  row_cnt--;
                  break;
                }
            }
            //qDebug() <<"row:"<< row_cnt << "="<< onerow;
        }
        //qDebug() <<"csv_out table";
        //qDebug() << csv_out;
        //qDebug() << "\ncsv_out(1,2)" << csv_out[1][2];

        //Create output table in a suitable format - //output [channel 1-12] [samples]
        for(int j=1; j<ColumnCount; j++) {           //begin at second column - ignore timestep
            QVector<double> temp(row_cnt-legend_row);
            for(int i=0; i<row_cnt-legend_row; i++) {
                temp[i] = csv_out[i][j];
            }
            output.append(temp);
        }
        //qDebug() <<"output table";
        //qDebug() << output;

        channel_count = ColumnCount-1;      //minus timestamp
        sample_count = row_cnt-legend_row;  //ignore data until legend line
        sampling_period = samplerate;       //sampling rate from timestamp in ms
    }//end if CSV loaded successfully
    return true;
}

/*****************************************************************************/
//Get current samples etc. from Measurement window
bool CFARsim::LoadFromMeasurement(QVector<QVector<double>> &output, float &sampling_period, quint8 &channel_count, QVector<QString> &channel_name, uint &sample_count) {
    channel_count = gp_Measurement->m_meas_slot_number;             //number of slots
    for(int i=0; i < channel_count; i++) {
        channel_name.append(gp_Measurement->m_meas_slots.at(i));    //slot names
        output.append(gp_Measurement->m_meas_samples[i]);           //measurement samples
    }
    sampling_period = gp_Measurement->m_meas_time.at(1);            //time stamps
    sample_count = gp_Measurement->m_meas_samples[0].size();        //number of samples

    //qDebug() << "m_meas_slot_number" << gp_Measurement->m_meas_slot_number;
    //qDebug() << "m_meas_slots" << gp_Measurement->m_meas_slots;
    //qDebug() << "\n\nm_meas_time" << gp_Measurement->m_meas_time;
    //qDebug() << "\nm_meas_samples[0]" << gp_Measurement->m_meas_samples[0];
    //qDebug() << "\n\nm_meas_samples[1]" << gp_Measurement->m_meas_samples[1];

    return true;
}
/*****************************************************************************/
//Setup GUI Combo boxes - compressin debugging input signal choice
bool CFARsim::SetupComprComboBox(bool remember_state) {
    QVector<quint8> prev_selection(CMPR_DBG_CNT);
    //remember selected indexes
    prev_selection[0]  = ui->cb_dbgin_1->currentIndex();
    prev_selection[1]  = ui->cb_dbgin_2->currentIndex();
    prev_selection[2] = ui->cb_dbgin_3->currentIndex();
    qDebug() << "compr prev_selection" << prev_selection << "remember_state" << remember_state;

    ui->cb_dbgin_1->blockSignals(true);             //we dont want to emit signal and interrupt this function...
    ui->cb_dbgin_2->blockSignals(true);
    ui->cb_dbgin_3->blockSignals(true);

    for(int i=MAX_CHANNEL_NUMBER+10; i>0; i--) {    //delete previous choices from combobox, add a few to be safe
        //compression debug checkboxes
        ui->cb_dbgin_1->removeItem(i);
        ui->cb_dbgin_2->removeItem(i);
        ui->cb_dbgin_3->removeItem(i);
    }

    for(int i=0; i<sim_channel_count; i++) { //insert new choices to combo boxes
        //compression debug comboboxes - only choice from original input data
        ui->cb_dbgin_1->addItem(sim_channel_names[i]);
        ui->cb_dbgin_2->addItem(sim_channel_names[i]);
        ui->cb_dbgin_3->addItem(sim_channel_names[i]);
    }

    if(remember_state == true) {
        //compression debug checkboxes
        if(prev_selection[0] < sim_channel_count)
            ui->cb_dbgin_1->setCurrentIndex(prev_selection[0]);
        else
            ui->cb_dbgin_1->setCurrentIndex(0);
        if(prev_selection[1] < sim_channel_count)
            ui->cb_dbgin_2->setCurrentIndex(prev_selection[1]);
        else
            ui->cb_dbgin_2->setCurrentIndex(0);
        if(prev_selection[2] < sim_channel_count)
            ui->cb_dbgin_3->setCurrentIndex(prev_selection[2]);
        else
            ui->cb_dbgin_3->setCurrentIndex(0);
    }
    qDebug() << "compr indexes" << ui->cb_dbgin_1->currentIndex() << ui->cb_dbgin_2->currentIndex() << ui->cb_dbgin_3->currentIndex();

    ui->cb_dbgin_1->blockSignals(false);
    ui->cb_dbgin_2->blockSignals(false);
    ui->cb_dbgin_3->blockSignals(false);

    return true;
}


/*****************************************************************************/
//Setup GUI Combo boxes to allow choice between plotted channels
bool CFARsim::SetupPlotComboBox(bool remember_state) {
    QVector<quint8> prev_selection(PLOT_CNT);
    prev_selection[0] = ui->cb_plot1->currentIndex();   //remember selected indexes
    prev_selection[1] = ui->cb_plot2->currentIndex();
    prev_selection[2] = ui->cb_plot3->currentIndex();
    prev_selection[3] = ui->cb_plot4->currentIndex();
    prev_selection[4] = ui->cb_plot5->currentIndex();
    prev_selection[5] = ui->cb_plot6->currentIndex();
    prev_selection[6] = ui->cb_plot7->currentIndex();
    prev_selection[7] = ui->cb_plot8->currentIndex();
    for(int i=0; i<PLOT_CNT; i++) {                     //ignore compression debug outputs
        if(prev_selection[i] > sim_channel_names.size() && remember_state == false) {
           prev_selection[i] = 0;
        }
    }

    ui->cb_plot1->blockSignals(true);
    ui->cb_plot2->blockSignals(true);
    ui->cb_plot3->blockSignals(true);
    ui->cb_plot4->blockSignals(true);
    ui->cb_plot5->blockSignals(true);
    ui->cb_plot6->blockSignals(true);
    ui->cb_plot7->blockSignals(true);
    ui->cb_plot8->blockSignals(true);

    for(int i=MAX_CHANNEL_NUMBER*((MEM_SIZE_MULT-1)*sim_channel_count)+50; i>0; i--) { //delete previous choices from combobox, add a few to be safe
        ui->cb_plot1->removeItem(i);
        ui->cb_plot2->removeItem(i);
        ui->cb_plot3->removeItem(i);
        ui->cb_plot4->removeItem(i);
        ui->cb_plot5->removeItem(i);
        ui->cb_plot6->removeItem(i);
        ui->cb_plot7->removeItem(i);
        ui->cb_plot8->removeItem(i);
    }

    for(int i=0; i<sim_channel_names.size(); i++) {    //insert new choices to combo boxes
        ui->cb_plot1->addItem(sim_channel_names[i]);
        ui->cb_plot2->addItem(sim_channel_names[i]);
        ui->cb_plot3->addItem(sim_channel_names[i]);
        ui->cb_plot4->addItem(sim_channel_names[i]);
        ui->cb_plot5->addItem(sim_channel_names[i]);
        ui->cb_plot6->addItem(sim_channel_names[i]);
        ui->cb_plot7->addItem(sim_channel_names[i]);
        ui->cb_plot8->addItem(sim_channel_names[i]);
    }

    ui->cbDynScChA->clear();
    ui->cbDynScChB->clear();
    ui->cbDynScChA->addItem("- none -");
    ui->cbDynScChB->addItem("- none -");
    for(int i=0; i<sim_channel_count; i++) {            //Dynamic scaling - insert input channels into combo boxes
        ui->cbDynScChA->addItem(QString("MAG_CFAR" + sim_channel_names[i]));
        ui->cbDynScChB->addItem(QString("MAG_CFAR" + sim_channel_names[i]));
    }

    //compression debug outputs
    for(int i=0; i<sim_cmpr_dbg_names.size(); i++) {
        ui->cb_plot1->addItem(sim_cmpr_dbg_names[i]);
        ui->cb_plot2->addItem(sim_cmpr_dbg_names[i]);
        ui->cb_plot3->addItem(sim_cmpr_dbg_names[i]);
        ui->cb_plot4->addItem(sim_cmpr_dbg_names[i]);
        ui->cb_plot5->addItem(sim_cmpr_dbg_names[i]);
        ui->cb_plot6->addItem(sim_cmpr_dbg_names[i]);
        ui->cb_plot7->addItem(sim_cmpr_dbg_names[i]);
        ui->cb_plot8->addItem(sim_cmpr_dbg_names[i]);
    }

    if(remember_state == true) {
        for(int i=0; i<PLOT_CNT; i++) {                     //recall previously selected indexes
            if(prev_selection[i] > ui->cb_plot1->count()) {
                prev_selection[i] = 0;
            }
        }
        ui->cb_plot1->setCurrentIndex(prev_selection[0]);
        ui->cb_plot2->setCurrentIndex(prev_selection[1]);
        ui->cb_plot3->setCurrentIndex(prev_selection[2]);
        ui->cb_plot4->setCurrentIndex(prev_selection[3]);
        ui->cb_plot5->setCurrentIndex(prev_selection[4]);
        ui->cb_plot6->setCurrentIndex(prev_selection[5]);
        ui->cb_plot7->setCurrentIndex(prev_selection[6]);
        ui->cb_plot8->setCurrentIndex(prev_selection[7]);
    }

    ui->cb_plot1->blockSignals(false);
    ui->cb_plot2->blockSignals(false);
    ui->cb_plot3->blockSignals(false);
    ui->cb_plot4->blockSignals(false);
    ui->cb_plot5->blockSignals(false);
    ui->cb_plot6->blockSignals(false);
    ui->cb_plot7->blockSignals(false);
    ui->cb_plot8->blockSignals(false);

    return true;
}

/*****************************************************************************/
//Prepare data for CFAR calculation
void CFARsim::PrepareData(QVector<QVector<double>> samples, float sampling_period, uint channel_count, uint sample_count, QVector<QString> channel_name) {
    sim_channel_count = channel_count;
    sim_time.resize(sample_count);          //resize timestamp array
    sim_samples.resize(channel_count*(MEM_SIZE_MULT*2)-channel_count);        //resize sample array
    sim_channel_names.resize(channel_count*(MEM_SIZE_MULT*2)-channel_count);  //resize channel names
    sim_TOF_DIFF.resize(channel_count);                     //resize array for DIFF signal
    sim_TOF_SNR.resize(channel_count);
    sim_TOF_ECHO_DET.resize(channel_count);
    sim_cmpr_en_ch_cnt = 0;                                 //all compression debug outputs are turned off
    sim_cmpr_samples.clear();
    sim_cmpr_dbg_names.clear();
    sim_cmpr_time.clear();
    sim_sampling_period = sampling_period;
    for(int i=0; i<sample_count; i++) {     //timestamps for plot
        sim_time[i] = sampling_period*i;
    }
    for(int i=0; i<channel_count; i++) {    //copy samples to global memory
        sim_samples[i] = samples[i];
        quint8 CB_LINE_CNT = 2;
        sim_channel_names[i] = channel_name[i];
        sim_channel_names[(channel_count*CFAR_OUT_MEM_MULTIPLIER)+i] = QString("%1 MAG_CFAR %2").arg(CB_LINE_CNT).arg(channel_name[i]);
        sim_channel_names[(channel_count*CFAR_OUT_MEM_MULTIPLIER)+i+(MEM_SIZE_MULT-1)*channel_count] = QString("%1 TOF_CFAR %2").arg(MEM_SIZE_MULT-1+CB_LINE_CNT++).arg(channel_name[i]);
        #ifdef DBG1_MEM_MULT
            sim_channel_names[(channel_count*DBG1_MEM_MULT)+i] = QString("%1 MAG_THR %2").arg(CB_LINE_CNT).arg(channel_name[i]);
            sim_channel_names[(channel_count*DBG1_MEM_MULT)+i+(MEM_SIZE_MULT-1)*channel_count] = QString("%1 TOF_THR %2").arg(MEM_SIZE_MULT-1+CB_LINE_CNT++).arg(channel_name[i]);
        #endif
        #ifdef DBG2_MEM_MULT
            sim_channel_names[(channel_count*DBG2_MEM_MULT)+i] = QString("%1 MAG_SNR_THR %2").arg(CB_LINE_CNT).arg(channel_name[i]);
            sim_channel_names[(channel_count*DBG2_MEM_MULT)+i+(MEM_SIZE_MULT-1)*channel_count] = QString("%1 TOF_SNR_THR %2").arg(MEM_SIZE_MULT-1+CB_LINE_CNT++).arg(channel_name[i]);
        #endif
        #ifdef DBG3_MEM_MULT
            sim_channel_names[(channel_count*DBG3_MEM_MULT)+i] = QString("%1 MAG_MP_DIFF %2").arg(CB_LINE_CNT).arg(channel_name[i]);
            sim_channel_names[(channel_count*DBG3_MEM_MULT)+i+(MEM_SIZE_MULT-1)*channel_count] = QString("%1 TOF_MP_DIFF %2").arg(MEM_SIZE_MULT-1+CB_LINE_CNT++).arg(channel_name[i]);
        #endif
        #ifdef DBG4_MEM_MULT
            sim_channel_names[(channel_count*DBG4_MEM_MULT)+i] = QString("%1 MAG_over_THR %2").arg(CB_LINE_CNT).arg(channel_name[i]);
            sim_channel_names[(channel_count*DBG4_MEM_MULT)+i+(MEM_SIZE_MULT-1)*channel_count] = QString("%1 TOF_over_THR %2").arg(MEM_SIZE_MULT-1+CB_LINE_CNT++).arg(channel_name[i]);
        #endif
        #ifdef DBG5_MEM_MULT
            sim_channel_names[(channel_count*DBG5_MEM_MULT)+i] = QString("%1 MAG_over_SNR %2").arg(CB_LINE_CNT).arg(channel_name[i]);
            sim_channel_names[(channel_count*DBG5_MEM_MULT)+i+(MEM_SIZE_MULT-1)*channel_count] = QString("%1 TOF_over_SNR %2").arg(MEM_SIZE_MULT-1+CB_LINE_CNT++).arg(channel_name[i]);
        #endif
        #ifdef DBG6_MEM_MULT
            sim_channel_names[(channel_count*DBG6_MEM_MULT)+i] = QString("%1 MAG_over_DIFF %2").arg(CB_LINE_CNT).arg(channel_name[i]);
            sim_channel_names[(channel_count*DBG6_MEM_MULT)+i+(MEM_SIZE_MULT-1)*channel_count] = QString("%1 TOF_over_DIFF %2").arg(MEM_SIZE_MULT-1+CB_LINE_CNT++).arg(channel_name[i]);
        #endif
        #ifdef DBG7_MEM_MULT
            sim_channel_names[(channel_count*DBG7_MEM_MULT)+i] = QString("%1 MAG_SNR %2").arg(CB_LINE_CNT).arg(channel_name[i]);
            sim_channel_names[(channel_count*DBG7_MEM_MULT)+i+(MEM_SIZE_MULT-1)*channel_count] = QString("%1 TOF_SNR %2").arg(MEM_SIZE_MULT-1+CB_LINE_CNT++).arg(channel_name[i]);
        #endif
        #ifdef DBG8_MEM_MULT
            sim_channel_names[(channel_count*DBG8_MEM_MULT)+i] = QString("%1 MAG_DIFF %2").arg(CB_LINE_CNT).arg(channel_name[i]);
            sim_channel_names[(channel_count*DBG8_MEM_MULT)+i+(MEM_SIZE_MULT-1)*channel_count] = QString("%1 TOF_DIFF %2").arg(MEM_SIZE_MULT-1+CB_LINE_CNT++).arg(channel_name[i]);
        #endif
        #ifdef DBG9_MEM_MULT
            sim_channel_names[(channel_count*DBG9_MEM_MULT)+i] = QString("%1 MAG_dbg %2").arg(CB_LINE_CNT).arg(channel_name[i]);
            sim_channel_names[(channel_count*DBG9_MEM_MULT)+i+(MEM_SIZE_MULT-1)*channel_count] = QString("%1 TOF_dbg %2").arg(MEM_SIZE_MULT-1+CB_LINE_CNT++).arg(channel_name[i]);
        #endif
        #ifdef DBG10_MEM_MULT
            sim_channel_names[(channel_count*DBG10_MEM_MULT)+i] = QString("%1 MAG_dbg %2").arg(CB_LINE_CNT).arg(channel_name[i]);
            sim_channel_names[(channel_count*DBG10_MEM_MULT)+i+(MEM_SIZE_MULT-1)*channel_count] = QString("%1 TOF_dbg %2").arg(MEM_SIZE_MULT-1+CB_LINE_CNT++).arg(channel_name[i]);
        #endif

    }
}

/*****************************************************************************/
//Insert input data into CFAR algorithm and calculate results
void CFARsim::CFAR(void) {
    QVector<double> CFAR_MAG;   //CFAR_MAG(1024)
    QVector<double> ECHO_DET;
    QVector<double> ECHO_DET_TOF;
    QVector<double> DBG[DEBUG_CNT]; //debug outputs

    quint8 offset = GET_REG("CFAR_THR_OFF").toUInt();
    quint8 TOF_der_thr = GET_REG("DER_THR_TOF").toUInt();
    quint8 der_thr = GET_REG("CFAR_DER_THR").toUInt();
    quint8 der_size = (GET_REG("CFAR_DER_SIZE").toUInt() + 1) * 2;  //2, 4, 6, 8 for Mag. CFAR
    quint8 der_size_tof_det = 4 ;                                   //2, 4, 6, 8 for ToF detection/calculation
    quint8 scale = GET_REG("CFAR_SCALE").toUInt();
    quint8 snr = GET_REG("CFAR_SNR").toUInt();          //Magnitude CFAR SNR threshold
    quint8 TOF_snr = GET_REG("CFAR_SNR_TOF").toUInt();  //TOF CFAR SNR threshold
    quint8 MV_AVG_samples = 24;         //Moving average sample count
    quint8 SNR_debounce = 2;            //Number of samples under SNR threshold necessary to detect falling edge of echo
    quint8 TOF_scale = GET_REG("CFAR_SCALE_TOF").toUInt();
    quint8 TOF_offset = GET_REG("THR_OFF_TOF").toUInt();
    quint8 TOF_der_size = (GET_REG("DER_SIZE_TOF").toUInt()+1)*2;

    sim_CFAR_START_SAMPLE = qRound(CFAR_STARTMS.toDouble()/sim_sampling_period);
    //qDebug() << "sim_CFAR_START_SAMPLE" << sim_CFAR_START_SAMPLE;

    for(int ch = 0; ch < sim_channel_count; ch++)
    {
        CFAR_MAG.resize(sim_samples[ch].size());
        ECHO_DET.resize(sim_samples[ch].size());
        ECHO_DET_TOF.resize(sim_samples[ch].size());
        for(int j=0; j<DEBUG_CNT; j++) {
          DBG[j].clear();
        }

        if (ui->radioButtonCFARv1->isChecked()) {   //Separated sequential Mag/TOF CFAR
            //Magnitude CFAR from original samples data
            CalculateCFAR_new(sim_samples[ch], der_size, scale, offset, der_thr, snr, CFAR_MAG, ECHO_DET, MV_AVG_samples, SNR_debounce,
                              DBG[0], DBG[1], DBG[2], DBG[3], DBG[4], DBG[5], DBG[6], DBG[7], DBG[8], DBG[9], sim_CFAR_START_SAMPLE, CFAR_TYPE_ALLDBG);

            //Scaling of 2nd stage CFAR data (if enabled)
            if(DYN_SC_ENA.toInt() != 0) {
                DynScaleData(ch, sim_time, ECHO_DET);
            }

            //ToF CFAR from Magnitude CFAR output data
            CalculateCFAR_new(ECHO_DET, TOF_der_size, TOF_scale, TOF_offset, TOF_der_thr, TOF_snr, CFAR_MAG, ECHO_DET_TOF, MV_AVG_samples, SNR_debounce,
                              DBG[10], DBG[11], DBG[12], DBG[13], DBG[14], DBG[15], DBG[16], DBG[17], DBG[18], DBG[19], sim_CFAR_START_SAMPLE, CFAR_TYPE_ALLDBG);
        }
        else if(ui->radioButtonCFARv2->isChecked()) {  //Old version: Mag+TOF CFAR calculated from input data
            CalculateCFARv2(sim_samples[ch], der_size, der_size_tof_det, scale, offset, der_thr, TOF_der_thr, snr, CFAR_MAG, ECHO_DET, ECHO_DET_TOF, MV_AVG_samples, SNR_debounce,\
                          DBG[0], DBG[1], DBG[2], DBG[3], DBG[4], DBG[5], DBG[6], DBG[7], DBG[8], DBG[9], sim_CFAR_START_SAMPLE);
            DBG[10] = DBG[0];   //cfar thr
            DBG[11] = DBG[8];   //TOF SNR thr
            DBG[12] = DBG[2];   //mp diff
            DBG[16] = DBG[6];   //snr curve
            DBG[17] = DBG[7];   //DIFF
        }

        sim_samples[(sim_channel_count*CFAR_OUT_MEM_MULTIPLIER)+ch] = ECHO_DET;
        sim_samples[(sim_channel_count*CFAR_OUT_MEM_MULTIPLIER)+ch+(MEM_SIZE_MULT-1)*sim_channel_count] = ECHO_DET_TOF;
        //qDebug()<<"sim_samples[" << (sim_channel_count*CFAR_OUT_MEM_MULTIPLIER)+ch << "] = " << ECHO_DET << "";
        #ifdef DBG1_MEM_MULT
            sim_samples[(sim_channel_count*DBG1_MEM_MULT)+ch] = DBG[0];
            sim_samples[(sim_channel_count*DBG1_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count] = DBG[10];
            //qDebug()<<"\nDBG1_MEM_MULT";
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG1_MEM_MULT)+ch << "]" << "=" << sim_samples[(sim_channel_count*DBG1_MEM_MULT)+ch];
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG1_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count << "]" << "=" << sim_samples[(sim_channel_count*DBG1_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count];
        #endif
        #ifdef DBG2_MEM_MULT
            sim_samples[(sim_channel_count*DBG2_MEM_MULT)+ch] = DBG[1];
            sim_samples[(sim_channel_count*DBG2_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count] = DBG[11];
            //qDebug()<<"\nDBG2_MEM_MULT";
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG2_MEM_MULT)+ch << "]" << "=" << sim_samples[(sim_channel_count*DBG2_MEM_MULT)+ch];
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG2_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count << "]" << "=" << sim_samples[(sim_channel_count*DBG2_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count];
        #endif
        #ifdef DBG3_MEM_MULT
            sim_samples[(sim_channel_count*DBG3_MEM_MULT)+ch] = DBG[2];
            sim_samples[(sim_channel_count*DBG3_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count] = DBG[12];
            //qDebug()<<"\nDBG3_MEM_MULT";
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG3_MEM_MULT)+ch << "]" << "=" << sim_samples[(sim_channel_count*DBG3_MEM_MULT)+ch];
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG3_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count << "]" << "=" << sim_samples[(sim_channel_count*DBG3_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count];
        #endif
        #ifdef DBG4_MEM_MULT
            sim_samples[(sim_channel_count*DBG4_MEM_MULT)+ch] = DBG[3];
            sim_samples[(sim_channel_count*DBG4_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count] = DBG[13];
            //qDebug()<<"\nDBG4_MEM_MULT";
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG4_MEM_MULT)+ch << "]" << "=" << sim_samples[(sim_channel_count*DBG4_MEM_MULT)+ch];
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG4_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count << "]" << "=" << sim_samples[(sim_channel_count*DBG4_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count];
        #endif
        #ifdef DBG5_MEM_MULT
            sim_samples[(sim_channel_count*DBG5_MEM_MULT)+ch] = DBG[4];
            sim_samples[(sim_channel_count*DBG5_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count] = DBG[14];
            //qDebug()<<"\nDBG5_MEM_MULT";
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG5_MEM_MULT)+ch << "]" << "=" << sim_samples[(sim_channel_count*DBG5_MEM_MULT)+ch];
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG5_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count << "]" << "=" << sim_samples[(sim_channel_count*DBG5_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count];
        #endif
        #ifdef DBG6_MEM_MULT
            sim_samples[(sim_channel_count*DBG6_MEM_MULT)+ch] = DBG[5];
            sim_samples[(sim_channel_count*DBG6_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count] = DBG[15];
            //qDebug()<<"\nDBG6_MEM_MULT";
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG6_MEM_MULT)+ch << "]" << "=" << sim_samples[(sim_channel_count*DBG6_MEM_MULT)+ch];
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG6_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count << "]" << "=" << sim_samples[(sim_channel_count*DBG6_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count];
        #endif
        #ifdef DBG7_MEM_MULT
            sim_samples[(sim_channel_count*DBG7_MEM_MULT)+ch] = DBG[6];
            sim_samples[(sim_channel_count*DBG7_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count] = DBG[16]; //sim_samples[(sim_channel_count*DBG7_MEM_MULT)+ch+(MEM_SIZE_MULT-1)] = DBG[6];
            //qDebug()<<"\nDBG7_MEM_MULT";
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG7_MEM_MULT)+ch << "]" << "=" << sim_samples[(sim_channel_count*DBG7_MEM_MULT)+ch];
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG7_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count << "]" << "=" << sim_samples[(sim_channel_count*DBG7_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count];
        #endif
        #ifdef DBG8_MEM_MULT
            sim_samples[(sim_channel_count*DBG8_MEM_MULT)+ch] = DBG[7];
            sim_samples[(sim_channel_count*DBG8_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count] = DBG[17];
            //qDebug()<<"\nDBG8_MEM_MULT";
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG8_MEM_MULT)+ch << "]" << "=" << sim_samples[(sim_channel_count*DBG8_MEM_MULT)+ch];
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG8_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count << "]" << "=" << sim_samples[(sim_channel_count*DBG8_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count];
        #endif
        #ifdef DBG9_MEM_MULT
            sim_samples[(sim_channel_count*DBG9_MEM_MULT)+ch] = DBG[8];
            sim_samples[(sim_channel_count*DBG9_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count] = DBG[18];
            //qDebug()<<"\nDBG9_MEM_MULT";
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG9_MEM_MULT)+ch << "]" << "=" << sim_samples[(sim_channel_count*DBG9_MEM_MULT)+ch];
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG9_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count << "]" << "=" << sim_samples[(sim_channel_count*DBG9_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count];
        #endif
        #ifdef DBG10_MEM_MULT
            sim_samples[(sim_channel_count*DBG10_MEM_MULT)+ch] = DBG[9];
            sim_samples[(sim_channel_count*DBG10_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count] = DBG[19];
            //qDebug()<<"\nDBG10_MEM_MULT";
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG10_MEM_MULT)+ch << "]" << "=" << sim_samples[(sim_channel_count*DBG10_MEM_MULT)+ch];
            //qDebug()<<"sim_samples[" << (sim_channel_count*DBG10_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count << "]" << "=" << sim_samples[(sim_channel_count*DBG10_MEM_MULT)+ch+(MEM_SIZE_MULT-1)*sim_channel_count];
        #endif

        //qDebug() << "sim_TOF_DIFF[ch] = DBG[4].size =" << DBG[4].size();

        //ToF extraction source
        if(ui->rbSourceToF->isChecked()) {
            sim_TOF_DIFF[ch] = DBG[12];             //ToF diff signal - always copy, used for multipeak TOF detection
            sim_TOF_SNR[ch] = DBG[16];              //SNR signal - always copy, used to report Peak SNR value (ToF detection)
            sim_TOF_ECHO_DET[ch] = ECHO_DET_TOF;    //ToF extraction from 2nd level (ToF CFAR)
        }
        else {
            sim_TOF_DIFF[ch] = DBG[2];              //ToF diff signal - always copy, used for multipeak TOF detection
            sim_TOF_SNR[ch] = DBG[6];               //SNR signal - always copy, used to report Peak SNR value (ToF detection)
            sim_TOF_ECHO_DET[ch] = ECHO_DET;        //ToF extraction from 1st level (Mag CFAR)
        }

    }
    //qDebug() <<"ECHO DET" << sim_samples;
    /*qDebug() << "---PRINTING SIM SAMPLES ARRAY---";
    qDebug() << "sim_samples.size()" << sim_samples.size();
    for(int m=0;m<sim_samples.size();m++){
        qDebug() <<"sim_samples[" << m << "].size() =" << sim_samples[m].size();
        qDebug() <<"sim_samples[" << m << "]" << sim_samples[m] << "\n";
    }
    qDebug() << "--------------------------------";*/
}

/*****************************************************************************/
//CFAR in GUI necessary steps
void CFARsim::CFARinGUI(void) {
    if(sim_samples.size() < 1)
        return;
    CFAR();
    CalculateTOF();
    slot_PlotData();
    UpdateTable();
}

/*****************************************************************************/
//Plot selected data
void CFARsim::PreparePlot(void) {
    for (int i = 0; i < 8; i++) {
        ui->ptSimulation->addGraph();
        ui->ptLegend->addGraph();
        ui->ptSimulation->graph(i)->setPen(QPen(QColor::fromRgb(plot_colors[i][0], plot_colors[i][1], plot_colors[i][2])));
        ui->ptLegend->graph(i)->setPen(QPen(QColor::fromRgb(plot_colors[i][0], plot_colors[i][1], plot_colors[i][2])));
        ui->ptLegend->graph(i)->setName(QString("Plot %1").arg(i+1));
    }
    ui->ptSimulation->xAxis->setRange(0, 10);
    ui->ptSimulation->yAxis->setRange(-100, 1040);  //(0, 1040)
    ui->ptSimulation->xAxis2->setRange(0, 1.7);
    ui->ptSimulation->xAxis2->setVisible(true);
    ui->ptSimulation->xAxis->setLabel("time [ms]");
    ui->ptSimulation->yAxis->setLabel("Samples [-]");
    ui->ptSimulation->xAxis2->setLabel("Approx distance [m]");
    ui->ptLegend->xAxis->setVisible(false);
    ui->ptLegend->yAxis->setVisible(false);
    ui->ptLegend->legend->setVisible(true);
    ui->ptLegend->legend->setPositionStyle(QCPLegend::PositionStyle::psBottom);
}

/*****************************************************************************/
//Plot selected data
void CFARsim::slot_PlotData(void) {
    qDebug() << " Plotting data";
    //qDebug() << "sim_channel_names.size" << sim_channel_names.size();
    //qDebug() << "sim_channel_names" << sim_channel_names;
    //qDebug() << "sim_samples.size" << sim_samples.size();
    QVector<quint8> plot_index(PLOT_CNT);
    plot_index[0] = ui->cb_plot1->currentIndex();
    plot_index[1] = ui->cb_plot2->currentIndex();
    plot_index[2] = ui->cb_plot3->currentIndex();
    plot_index[3] = ui->cb_plot4->currentIndex();
    plot_index[4] = ui->cb_plot5->currentIndex();
    plot_index[5] = ui->cb_plot6->currentIndex();
    plot_index[6] = ui->cb_plot7->currentIndex();
    plot_index[7] = ui->cb_plot8->currentIndex();

    for(int i=0; i<PLOT_CNT; i++) {
        //normal signals
        if(plot_index.at(i) > 0 && plot_index.at(i) <= sim_channel_names.size()) {
            ui->ptSimulation->graph(i)->setData(sim_time,sim_samples.at(plot_index.at(i)-1));
            ui->ptSimulation->graph(i)->setVisible(true);
            ui->ptLegend->graph(i)->setName(QString("Plot %1 - %2").arg(i+1).arg(sim_channel_names.at(plot_index.at(i)-1)));
            qDebug() << "Plot:"<< i <<", Index(-1):" << plot_index.at(i)-1 << "Data:" << sim_channel_names.at(plot_index.at(i)-1);
            //qDebug() << "Data =" << sim_samples.at(plot_index.at(i)-1) << "\n";
        }
        //compression debug signals
        else if(plot_index.at(i) > 0 && plot_index.at(i) < 256) {

            quint8 cmpr_idx = plot_index.at(i)-sim_channel_names.size()-1;
            //qDebug() << "\ncmpr_idx = " << plot_index.at(i) << "-" << sim_channel_names.size() << "-1" << " = " << cmpr_idx;
            //qDebug() << "sim_cmpr_samples.size()" << sim_cmpr_samples.size();
            //qDebug() << "sim_cmpr_dbg_names.size()" << sim_cmpr_dbg_names.size();
            if(cmpr_idx <= sim_cmpr_dbg_names.size()) {
                ui->ptSimulation->graph(i)->setData(sim_cmpr_time.at(cmpr_idx), sim_cmpr_samples.at(cmpr_idx));
                ui->ptSimulation->graph(i)->setVisible(true);
                ui->ptLegend->graph(i)->setName(QString("Plot %1 - %2").arg(i+1).arg(sim_cmpr_dbg_names.at(cmpr_idx)));
            }
            else {
                qDebug() << "plotting index out of bounds";
                cmpr_idx = 0;
                ui->ptSimulation->graph(i)->setVisible(false);
                ui->ptLegend->graph(i)->setName("N/A");
            }
        }
        else {
            ui->ptSimulation->graph(i)->setVisible(false);
            ui->ptLegend->graph(i)->setName("N/A");
        }
    }
    ui->ptSimulation->xAxis->setRange(SUMMARY_STARTMS.toInt(), SUMMARY_STOPMS.toInt());
    ui->ptSimulation->xAxis2->setRange(SUMMARY_STARTMS.toDouble()*346.0/2000.0, SUMMARY_STOPMS.toDouble()*346.0/2000.0);
    ui->ptSimulation->replot();
    ui->ptLegend->replot();
}

/*****************************************************************************/
//Calculate TOF from samples
//06/2021 new version - using differentiation signal sim_TOF_DIFF[channel]
void CFARsim::CalculateTOF(void) {
    //New version using differentiation signal
    int der_thr_pos = GET_REG("MP_DET_THR_POS").toInt();
    int der_thr_neg = GET_REG("MP_DET_THR_NEG").toInt();
    int sample = 1;
    int maxi = 0;
    float maxi_SNR = 0;
    int samplecount = 0;
    int echocount = 0;
    //int diff_tof_thr = ui->le_tofthr->text().toInt();   //ToF detection only differentiation threshold
    //int diff_tof_thr = 5;
    int Redge = -1;         //-1 = init before echo, 0 = falling edge detected, 1 = rising edge detected
    int Redge_index = 0;

    double sample_period = 0.1024;  //[ms]
    double TOF_offset = GET_REG("TOF_OFFSET").toDouble();
    quint8 max_echo_cnt = 12;
    /*if(GET_REG("TOF_REPORTING").toInt() == 0) {
        max_echo_cnt = 8;   //Report 8 Peaks and ToFs
    }
    else {
        max_echo_cnt = 12;  //Report 12 ToFs
    }*/

    //double TOF_offset = 0;

    if (sim_time.size() > 2) {
        sample_period = sim_time.at(1);
    }

    //qDebug() << "sim_TOF_ECHO_DET" << sim_TOF_ECHO_DET;
    //qDebug() << "sim_TOF_ECHO_DET.size" << sim_TOF_ECHO_DET.size();
    //qDebug() << "sim_TOF_DIFF" << sim_TOF_DIFF;
    //qDebug() << "sim_TOF_DIFF.size" << sim_TOF_DIFF.size();
    //qDebug() << "sim_channel_count" << sim_channel_count;

    //new
    for(uint channel=0; channel < sim_channel_count; channel++) {
        bool max_det_shift = 0;
        sample = 0;
        maxi = 0;           //max. value of echo
        maxi_SNR = 0;       //value of SNR in peak maximum
        Redge_index = 0;    //when did the rising edge start
        Redge = -1;         //-1 = init before echo, 0 = falling edge detected, 1 = rising edge detected
        //qDebug() << "sim_TOF_DIFF[channel].size" << sim_TOF_DIFF[channel].size();
        //qDebug() << "sim_TOF_ECHO_DET[channel].size" << sim_TOF_ECHO_DET[channel].size();
        samplecount = sim_TOF_DIFF[channel].size(); //number of samples
        sim_tof[channel][TOF].clear();
        sim_tof[channel][PEAK].clear();
        sim_tof[channel][WIDTH].clear();
        sim_tof[channel][PEAK_SNR].clear();
        echocount = 0;
        //qDebug() << "- - - Channel" << channel << "- - -";

        for(sample = 0; sample<samplecount; sample++) {
            if(echocount >= max_echo_cnt) {    //Reached Maximum number of reported echoes
                break;
            }
            if(sim_TOF_ECHO_DET[channel].at(sample) == 0 && (Redge == 0 || Redge == 1)) {   //Echo ended
                //report previous echo's width, peak etc... reporting width as: rising edge 1st echo to rising edge 2nd echo
                if(maxi == sim_samples[channel].at(Redge_index-1) && max_det_shift == 1) {
                    //qDebug() << "Redge index -1";
                    Redge_index--;  //peak was at a point previous to Redge detection, shift by one sample
                    max_det_shift = 0;
                    //If we expand this echo width by one sample, we must subtract it from the previous one (only if we dont eliminate the previous echo completely)
                    if(sim_tof[channel][WIDTH].size()>0) {
                        if(sim_tof[channel][WIDTH].last()-sample_period >= sample_period) {
                           sim_tof[channel][WIDTH][sim_tof[channel][WIDTH].size()-1] = sim_tof[channel][WIDTH].last()-sample_period;
                        }
                    }
                }
                sim_tof[channel][TOF].append((Redge_index*sample_period)-TOF_offset);
                sim_tof[channel][PEAK].append(maxi);
                sim_tof[channel][WIDTH].append((sample-Redge_index)*sample_period);
                sim_tof[channel][PEAK_SNR].append(maxi_SNR);
                echocount++;
                Redge = -1;
                max_det_shift = 0;
                //qDebug() << "Echo end at" << sample << "which is" << (sample*sample_period)-TOF_offset << "ms";
            }
            //Rising edge
            if(sim_TOF_ECHO_DET[channel].at(sample) != 0) {
                if((sim_TOF_DIFF[channel].at(sample) >= der_thr_pos) && Redge == 0) {        //multipeak echo
                    //qDebug() << "TOF R.E. at" << sample << "which is" << (sample*sample_period)-TOF_offset << "ms";
                    //report previous echo's width, peak etc... reporting width as: rising edge 1st echo to rising edge 2nd echo
                    if(maxi == sim_samples[channel].at(Redge_index-1) && max_det_shift == 1 && Redge_index>0) {
                        //qDebug() << "Redge index -1";
                        Redge_index--;  //peak was at a point previous to Redge detection, shift by one sample
                        max_det_shift = 0;
                        //If we expand this echo width by one sample, we must subtract it from the previous one (only if we dont eliminate the previous echo completely)
                        if(sim_tof[channel][WIDTH].size()>0) {
                            if(sim_tof[channel][WIDTH].last()-sample_period >= sample_period) {
                               sim_tof[channel][WIDTH][sim_tof[channel][WIDTH].size()-1] = sim_tof[channel][WIDTH].last()-sample_period;
                            }
                        }
                    }
                    sim_tof[channel][TOF].append((Redge_index*sample_period)-TOF_offset);
                    sim_tof[channel][PEAK].append(maxi);
                    sim_tof[channel][WIDTH].append((sample-Redge_index)*sample_period);
                    sim_tof[channel][PEAK_SNR].append(maxi_SNR);
                    echocount++;
                    Redge = 1;
                    Redge_index = sample;
                    maxi = 0;
                    max_det_shift = 0;
                }
                else if(Redge == -1) {  //first rising edge of echo
                    //qDebug() << "\nTOF first R.E. at" << sample << "which is" << (sample*sample_period)-TOF_offset << "ms";
                    Redge = 1;
                    Redge_index = sample;
                    maxi = 0;
                }

            }
            //Find peak value
            if(sim_TOF_ECHO_DET[channel].at(sample) != 0 && sim_samples[channel].at(sample) > maxi) {
                if(sample > 0 && maxi == 0 && sim_samples[channel].at(sample-1) > sim_samples[channel].at(sample)) {    //very steep edges can cause 1 sample delay in Rising edge detection
                    maxi = sim_samples[channel].at(sample-1);                                                           //when rising edge is detected, check if we didnt miss peak by 1 sample
                    maxi_SNR = sim_TOF_SNR[channel].at(sample-1);
                    max_det_shift = 1;
                }
                else {
                    maxi = sim_samples[channel].at(sample);
                    maxi_SNR = sim_TOF_SNR[channel].at(sample);
                }
            }
            //Falling edge
            if(Redge == 1 && (sim_TOF_DIFF[channel].at(sample) <= -der_thr_neg)) {  //diff signal crosses negative diff. threshold OR   - multipeak echo F.Edge
                //qDebug() << "TOF F.E. at" << sample << "which is" << (sample*sample_period)-TOF_offset << "ms";
                Redge = 0;
            }
        }
    }
}

/*****************************************************************************/
//Update Sample/ToF data table
void CFARsim::UpdateTable(void) {    
    ui->twSamplesToF->clearContents();
    ui->twSamplesToF->setRowCount(0);
    ui->twSamplesToF->setColumnCount(0);
    ui->twSamples->clearContents();
    ui->twSamples->setRowCount(0);
    ui->twSamples->setColumnCount(0);
    if(ui->cb_updatetable->checkState() == 0) {
        return;
    }

    uint i, j;
    QStringList m_TableHeader;
    //------------------------------------------------------------------------------------------------
    //TOF TABLE
    quint8 max_echo_cnt = 12;
    /*if(GET_REG("TOF_REPORTING").toInt() == 0) {
        max_echo_cnt = 8;   //Report 8 Peaks and ToFs
    }
    else {
        max_echo_cnt = 12;  //Report 12 ToFs
    }*/

    quint8 m = 2;
    #ifdef REPORT_WIDTH
        m++;    //m=3
    #endif

    #ifdef REPORT_PEAK_SNR
        m++;    //m=3 (no width) or 4 (report width)
    #endif

    ui->twSamplesToF->setColumnCount(m*sim_channel_count + 1);
    m_TableHeader.append("E/S");
    for (i = 0; i < sim_channel_count; i++)
    {
        m_TableHeader.append(QString("%1 ToF").arg(sim_channel_names.at(i)));
        m_TableHeader.append(QString("%1 P").arg(sim_channel_names.at(i)));
        #ifdef REPORT_WIDTH
            m_TableHeader.append(QString("%1 W").arg(sim_channel_names.at(i)));
        #endif
        #ifdef REPORT_PEAK_SNR
            m_TableHeader.append(QString("%1 P_SNR").arg(sim_channel_names.at(i)));
        #endif
    }
    ui->twSamplesToF->setHorizontalHeaderLabels(m_TableHeader);

    for (int row = 0; row < max_echo_cnt; row++)
    {
        ui->twSamplesToF->setRowCount(row + 1);
        ui->twSamplesToF->setItem(row, 0, new QTableWidgetItem(QString("E%1").arg(row+1)));

        for (j = 0; j < MAX_CHANNEL_NUMBER; j++)
        {
            if(sim_tof[j][0].size() > row)
            {
                quint8 column = 1;
                ui->twSamplesToF->setItem(row, m*j + column, new QTableWidgetItem(QString("%1").arg(sim_tof[j][column-1].at(row),0,'f',4)));
                column++;
                //if(GET_REG("TOF_REPORTING").toInt() == 0) { //report ToF+peak
                ui->twSamplesToF->setItem(row, m*j + column, new QTableWidgetItem(QString("%1").arg(sim_tof[j][column-1].at(row))));
                column++;
                /*}
                else {                                      //report only ToF
                    ui->twSamplesToF->setItem(row, m*j + column, new QTableWidgetItem(QString("")));
                    column++;
                }*/
                #ifdef REPORT_WIDTH
                    ui->twSamplesToF->setItem(row, m*j + column, new QTableWidgetItem(QString("%1").arg(sim_tof[j][column-1].at(row))));
                    column++;
                #endif
                #ifdef REPORT_PEAK_SNR
                    ui->twSamplesToF->setItem(row, m*j + column, new QTableWidgetItem(QString("%1").arg(sim_tof[j][column-1].at(row),0,'f',2)));
                    column++;
                #endif
            }
            else
            {
                quint8 column = 1;
                ui->twSamplesToF->setItem(row, m*j + column++, new QTableWidgetItem(QString("")));
                ui->twSamplesToF->setItem(row, m*j + column++, new QTableWidgetItem(QString("")));
                #ifdef REPORT_WIDTH
                    ui->twSamplesToF->setItem(row, m*j + column++, new QTableWidgetItem(QString("")));
                #endif
                #ifdef REPORT_PEAK_SNR
                    ui->twSamplesToF->setItem(row, m*j + column++, new QTableWidgetItem(QString("")));
                #endif
            }
        }
    }
    //------------------------------------------------------------------------------------------------
    // SAMPLES
    m_TableHeader.clear();
    ui->twSamples->setColumnCount(sim_channel_count*MEM_SIZE_MULT+(MEM_SIZE_MULT-1)*sim_channel_count + 1);
    ui->twSamples->setColumnWidth(0, 65);
    m_TableHeader.append("Time [ms]");

    for (i = 0; i < (sim_channel_count*MEM_SIZE_MULT+(MEM_SIZE_MULT-1)*sim_channel_count); i++) {
        m_TableHeader.append(QString("%1").arg(sim_channel_names.at(i)));
    }
    ui->twSamples->setHorizontalHeaderLabels(m_TableHeader);

    for (int row = 0; row < sim_time.size(); row++) {
        ui->twSamples->setRowCount(row + 1);
        ui->twSamples->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(sim_time.at(row),0,'f',4)));

        for (j = 0; j < (sim_channel_count*MEM_SIZE_MULT+(MEM_SIZE_MULT-1)*sim_channel_count); j++) {
            if(sim_samples[j].size() > row) {
                ui->twSamples->setItem(row, j + 1, new QTableWidgetItem(QString("%1").arg(sim_samples[j].at(row))));
            }
            else {
                ui->twSamples->setItem(row, j + 1, new QTableWidgetItem(QString("")));
            }
        }
    }
    //------------------------------------------------------------------------------------------------
    // COMPR./DECOMPR. data
    ui->twCompr->clearContents();
    ui->twCompr->setRowCount(0);
    m_TableHeader.clear();
    ui->twCompr->setColumnCount(sim_cmpr_dbg_names.size()*2);               //Two columns per debug output
    ui->twCompr->setColumnWidth(0, 100);

    for (i = 0; i < sim_cmpr_dbg_names.size(); i++) {                       //Timestep1 - out1 --- Timestep2 - out2 --- Timestep3 - out3
        m_TableHeader.append(QString("Time %1 [ms]").arg(sim_cmpr_dbg_names.at(i)));    // Timestep
        m_TableHeader.append(QString("%1").arg(sim_cmpr_dbg_names.at(i)));              // Dbg Out
    }

    ui->twCompr->setHorizontalHeaderLabels(m_TableHeader);

    //Find out what is the longest timebase (102.4 undersampled samples vs 2 times as much 51.2 uncompressed samples)
    int maxlen = 0;
    for(int i = 0; i<sim_cmpr_dbg_names.size(); i++) {
        if(maxlen <  sim_cmpr_time[i].size())
            maxlen =  sim_cmpr_time[i].size();
    }
    qDebug() << "maxlen" << maxlen;
    qDebug() << "sim_cmpr_dbg_names.size" << sim_cmpr_dbg_names.size();

    for (int row = 0; row < maxlen; row++) {
        ui->twCompr->setRowCount(row + 1);  //add new row
        //ui->twCompr->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(sim_cmpr_time[dbg_out].at(row),0,'f',4)));

        for (j = 0; j < sim_cmpr_dbg_names.size()*2; j=j+2) {
            ui->twCompr->setColumnWidth(j, 200);
            ui->twCompr->setColumnWidth(j+1, 200);

            if(sim_cmpr_samples[int(j/2)].size() > row) {
                ui->twCompr->setItem(row, j,    new QTableWidgetItem(QString("%1").arg(sim_cmpr_time[int(j/2)].at(row),0,'f',4)));
                ui->twCompr->setItem(row, j + 1,new QTableWidgetItem(QString("%1").arg(sim_cmpr_samples[int(j/2)].at(row))));
            }
            else {
                ui->twCompr->setItem(row, j,    new QTableWidgetItem(QString("")));
                ui->twCompr->setItem(row, j + 1,new QTableWidgetItem(QString("")));
            }
        }

    }

}

/*****************************************************************************/
//Copy graph to clipboard
bool CFARsim::CopyGraph(void) {
    QClipboard * clipboard = QApplication::clipboard();
    QPixmap pixmap;
    bool result = true;
    pixmap = QPixmap::grabWidget(ui->ptSimulation);
    clipboard->setPixmap(pixmap);
    return result;
}

/*****************************************************************************/
//Save graph to PNG
bool CFARsim::SaveGraph(QString arg)
{
    bool result = true;
    QStringList arg_list = arg.split(" ");
    QString filename;
    QString temp;

    if (arg_list.size() > 1)
    {
        filename = arg_list.at(1);
    }
    else
    {
        filename = QFileDialog::getSaveFileName(this, "Save PNG file",QDir::currentPath(), "PNG file(*.png)");
    }

    if (filename == "")
    {
        //gp_DSI3Master->Log(LOG_TYPE_ERROR, "Filename is empty");
        result = false;
        return result;
    }

    ui->ptSimulation->savePng(filename, 1000, 500);

    return result;
}

/*****************************************************************************/
//Save table to CSV (Sample or ToF)
bool CFARsim::SaveCSV(QString arg) {
    int slot = 0;
    int i, row;
    QString filename;
    QString delim = ui->cb_delim->currentText();     //CSV file delimiter choice - "," or ";"
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");

    //Save TOF table to CSV
    if (arg.contains("tof"))
    {
        filename = QFileDialog::getSaveFileName(this, "Save CSV file",QDir::currentPath(), "CSV file(*.csv)");
        QFile file(filename);
        if (file.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream data( &file );

            data << "Date and time" << delim << formattedTime << "\n";
            data << "\n";

            data << "Echo" << delim;

            for (slot = 0; slot < sim_channel_count; slot++)
            {
                data << QString("%1 - ToF").arg(sim_channel_names.at(slot)) << delim;
                data << QString("%1 - Peak").arg(sim_channel_names.at(slot)) << delim;
                data << QString("%1 - Width").arg(sim_channel_names.at(slot)) << delim;
            }
            data << "\n";

            for (row = 0; row < 16; row++)
            {
                data << QString("Echo %1").arg(row) << delim;

                for (slot = 0; slot < sim_channel_count; slot++)
                {
                    if (sim_tof[slot][TOF].size() > row)
                        data << QString("%1").arg(sim_tof[slot][TOF].at(row), 0, 'f', 4) << delim;
                    else
                        data << "" << delim;

                    if (sim_tof[slot][PEAK].size() > row)
                        data << QString("%1").arg(sim_tof[slot][PEAK].at(row), 0, 'f', 0) << delim;
                    else
                        data << "" << delim;

                    if (sim_tof[slot][WIDTH].size() > row)
                        data << QString("%1").arg(sim_tof[slot][WIDTH].at(row), 0, 'f', 3) << delim;
                    else
                        data << "" << delim;
                }
                data << "\n";
            }
            data << "\n";
            file.close();
            return true;
        }
        return false;

    }
    //Save Samples table to CSV
    else if (arg.contains("samples"))
    {
        filename = QFileDialog::getSaveFileName(this, "Save CSV file",QDir::currentPath(), "CSV file(*.csv)");
        QFile file(filename);
        if (file.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream data( &file );

            data << "Date and time" << delim << formattedTime << "\n";
            data << "\n";
            data << "Time [ms]" << delim;

            for (slot = 0; slot < (sim_channel_count*MEM_SIZE_MULT+(MEM_SIZE_MULT-1)*sim_channel_count); slot++)
            {
                data << sim_channel_names.at(slot) << delim;
            }
            data << "\n";

            for (row = 0; row < sim_time.size(); row++)
            {
                data << QString("%1").arg(sim_time.at(row)) << delim;

                for (slot = 0; slot < (sim_channel_count*MEM_SIZE_MULT+(MEM_SIZE_MULT-1)*sim_channel_count); slot++)
                {
                    if (sim_samples[slot].size() > row)
                        data << QString("%1").arg(sim_samples[slot].at(row), 0, 'g', 3) << delim;
                    else
                        data << "" << delim;
                }
                data << "\n";
            }
            data << "\n";
            file.close();
            return true;
        }
        return false;
    }
    // Save COMPR./DECOMPR. data to CSV
    else if(arg.contains("compr"))
    {
        qDebug() << " -> Save compr.";
        filename = QFileDialog::getSaveFileName(this, "Save CSV file",QDir::currentPath(), "CSV file(*.csv)");
        QFile file(filename);
        if (file.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream data( &file );

            data << "Date and time" << delim << formattedTime << "\n";
            data << "\n";

            for (slot = 0; slot < sim_cmpr_dbg_names.size(); slot++) //Timestep1 - out1 --- Timestep2 - out2 --- Timestep3 - out3
            {
                data << QString("Time %1 [ms]").arg(sim_cmpr_dbg_names.at(slot)) << delim;  // Timestep
                data << sim_cmpr_dbg_names.at(slot) << delim;                               // Dbg Out
            }
            data << "\n";

            //Find out what is the longest timebase (102.4 undersampled samples vs 2 times as much 51.2 uncompressed samples)
            int maxlen = 0;
            for(int i = 0; i<sim_cmpr_dbg_names.size(); i++) {
                if(maxlen <  sim_cmpr_time[i].size())
                    maxlen =  sim_cmpr_time[i].size();
            }

            for (row = 0; row < maxlen; row++)
            {
                for (slot = 0; slot < sim_cmpr_dbg_names.size()*2; slot = slot+2)
                {
                    if (sim_cmpr_samples[int(slot/2)].size() > row) {
                        data << QString("%1").arg(sim_cmpr_time[int(slot/2)].at(row), 0, 'f', 4) << delim;
                        data << QString("%1").arg(sim_cmpr_samples[int(slot/2)].at(row), 0, 'g', 3) << delim;
                    }
                    else {
                        data << "" << delim;
                        data << "" << delim;
                    }
                }
                data << "\n";
            }
            data << "\n";
            file.close();
            return true;
        }
        return false;
    }
    else {
        //gp_DSI3Master->Log(LOG_TYPE_ERROR, "Graph name not recognized");
        return false;
    }
    return false;
}

/*****************************************************************************/
/*****************************************************************************/
/*             CFARv2 FUNCTION - new CFAR THR calculation                    */
/*****************************************************************************/
/*****************************************************************************/
//Calculate CFARv2
void CFARsim::CalculateCFARv2(QVector<double> samples, quint8 der_size, quint8 TOF_der_size, double scale, quint8 offset, int der_thr, int TOF_der_thr, \
                            int CFAR_SNR_THRESHOLD, QVector<double> &CFAR_MAG, QVector<double> &ECHO_DET, QVector<double> &ECHO_DET_TOF, \
                            quint8 MVA_samples, quint8 SNR_debounce, QVector<double> &DBG1, QVector<double> &DBG2, \
                            QVector<double> &DBG3, QVector<double> &DBG4, QVector<double> &DBG5, QVector<double> &DBG6, \
                            QVector<double> &DBG7, QVector<double> &DBG8, QVector<double> &DBG9, QVector<double> &DBG10, quint32 CFAR_START_SAMPLE)
{
    QVector<double> min_cfar_thr;
    QVector<double> diff;
    QVector<double> diff_TOF;
    QVector<double> diff_differences;
    QVector<double> diff_differences_TOF;
    QVector<double> mins;
    QVector<double> min_cfar_thr_shifted;                   //Time shifted cfar_thr to better match FPGA CFAR implementation
    QVector<double> over_diff_thr;                          //Debug signals - high if condition is fulfilled (diff threshold, SNR threshold, cfar threshold)
    QVector<double> over_SNR_thr;
    QVector<double> over_min_cfar_thr;
    int i = 0, j = 0;
    quint8 valueS = 2;
    quint8 MVA_MIN_VAL = 2;                                 //Minimum value of moving average
    quint16 SNR_THR_TOF = GET_REG("CFAR_SNR_TOF").toInt();
    //quint32 CFAR_START_SAMPLE = 176;

    min_cfar_thr.resize(samples.size());
    diff.resize(samples.size());                            //Differentiation signal for Magnitude CFAR
    diff_TOF.resize(samples.size());                        //Differentiation signal for ToF Detection CFAR
    diff_differences.resize(der_size -1);
    diff_differences_TOF.resize(TOF_der_size -1);
    mins.resize(der_size / valueS);
    min_cfar_thr_shifted.resize(samples.size());
    over_diff_thr.resize(samples.size());
    over_SNR_thr.resize(samples.size());
    over_min_cfar_thr.resize(samples.size());

    double multiplier = scale/2.5;                          //Convert new scale to old multiplier

    if(der_size < TOF_der_size) {
        TOF_der_size = der_size;
        qDebug() << "TOF_der_size value adjusted to match der_size";
    }

    //Generate CFAR_MAG and DIFF arrays
    for (i = 0; i < (samples.size() - der_size); i++)
    {
        if(i < CFAR_START_SAMPLE)                           //delayed start of CFAR calculation
            continue;

        for (j = 0; j < (der_size - 1); j++)                //Calculate differentiation
        {
            diff_differences[j] = samples[i+j+1] - samples[i+j];
        }
        diff[i] = QVectorAvg(diff_differences);

        for(j = 0; j < (TOF_der_size - 1); j++) {
            diff_differences_TOF[j] = samples[i+j+1] - samples[i+j];
        }
        diff_TOF[i] = QVectorAvg(diff_differences_TOF);

        for (j = 0; j < der_size / valueS; j++)              //Calculate CFAR thr
        {
            mins[j] = QVectorMax(samples.mid(i + j*valueS, valueS));
        }

        //min_cfar_thr[i] = QVectorMin(mins) * multiplier + offset;     //Change of cfar threshold calculation based on MOBIS request, 14/05/2021
        if(offset > QVectorMin(mins) * multiplier) {
            min_cfar_thr[i] = offset;
        }
        else {
            min_cfar_thr[i] = QVectorMin(mins) * multiplier;
        }

        //Time shift the min_cfar_thr signal FORWARD to match FPGA implementation of CFAR
        #ifdef SHIFT_CFAR_THR
        if(i < der_size+1) {
            min_cfar_thr_shifted[i] = offset;               //Dont shift the first samples to avoid array index out of range
        }
        else {
            min_cfar_thr_shifted[i] = min_cfar_thr[i-(der_size+1)];
        }
        #else
        min_cfar_thr_shifted[i] = min_cfar_thr[i];
        #endif
        /*********************************/
        // CFAR_MAG only above CFAR thr
        if (samples[i] >= min_cfar_thr_shifted[i])
        {
            CFAR_MAG[i] = samples[i];
            over_min_cfar_thr[i] = 500;
        }
        else
        {
            CFAR_MAG[i] = 0;
            over_min_cfar_thr[i] = 0;
        }
    }

    //Find edges of possible echoes
    QVector<double> DIFF_shifted;           //Time shifted differentiation signal - Mag.
    QVector<double> DIFF_TOF_shifted;       //Time shifted differentiation signal - ToF
    QVector<double> MVAVG;                  //Moving average of input signal except potential echoes
    QVector<double> ConfirmedEchoes;        //Echoes confirmed all by 3 conditions - magnitude, differentiation threshold, SNR threshold
    QVector<double> SNR_curve;              //Signal to Noise ratio curve

    DIFF_shifted.resize(samples.size());
    DIFF_TOF_shifted.resize(samples.size());
    MVAVG.resize(samples.size());
    ConfirmedEchoes.resize(samples.size());
    SNR_curve.resize(samples.size());

    //Time shift FOWRWARD the DIFF signal used to calculate edges of echoes
    int sh = 0;
/*    switch(der_size) {  //(GUI: 0,1,2,3) -> 2,4,6,8
        case 8: sh = 1; break;
        case 6: sh = 2; break;
        default: break;
    }*/
    qDebug()<<"der_size =" << der_size << "shift coef. sh =" << sh;

    #ifdef SHIFT_DIFF
    for(i=0; i<(samples.size() - der_size); i++) {
        if(i < CFAR_START_SAMPLE)
            continue;
        if(i < (der_size/2)+sh) {
            DIFF_shifted[i] = 0;
        }
        else {
            DIFF_shifted[i] = diff[i-(der_size/2)+sh];
        }
        if(i < (TOF_der_size/2)+sh) {
            DIFF_TOF_shifted[i] = 0;
        }
        else {
            DIFF_TOF_shifted[i] = diff_TOF[i-(TOF_der_size/2)+sh];
        }
    }
    #else
    for(i=0; i<samples.size(); i++){
        DIFF_shifted[i] = diff[i];
        DIFF_TOF_shifted[i] = diff_TOF[i];
    }
    #endif
    //qDebug() << "Shifted by: " << der_size/2 << " samples forward";

    //####################################################
    //Sweep input data and find edges of echoes
    //Rising edge of echo - 3 conditions need to be fulfilled - data above CFAR_THR (CFAR_MAG>0), differentiation above differentiation THR (DIFF_shifted[i] > der_thr), SNR above SNR threshold
    //Falling edge of echo - when SNR falls below SNR Threshold
    //First step - find when at least one condition (CFAR_MAG or differentiation) is fulfilled
    //Until it is fulfilled, calculate Moving average of "noise"
    QVector<double> prev_inputs;                        //Buffer of previous X samples
    //prev_inputs.resize(MVA_samples);
    quint8 under_snr_cnt = 0;
    quint8 TOF_under_snr_cnt = 0;
    bool echo_start_flag = 0;

    for(i = 0; i < (samples.size() - der_size); i++) {
        if(i < CFAR_START_SAMPLE) {
            //ECHO_DET[i] = samples[i];                   //keep raw correlator output prior to CFAR start time
            continue;
        }

        //Calculate moving average - calculation is performed in the end of loop if echo is not started
        if(i>0){
             MVAVG[i] =  MVAVG[i-1];    //Temporarily get the previous sample to calculate current SNR
        }

        //Calculate SNR
        if(samples[i] > 0) {        //Prevent undefined operations - Log(0) or log(negative number)
            SNR_curve[i] = 20*log10((float(samples[i]))/float(MVAVG[i-1]));
        }
        else {
            SNR_curve[i] = 0.01;    //Replacement value
        }

        // End of echo - Echo already started but its magnitude dropped below SNR THR
        if((SNR_curve[i] < CFAR_SNR_THRESHOLD && echo_start_flag == 1) || (i == (samples.size() - der_size -1) && echo_start_flag == 1)) {     //changed from: DIFF_shifted[i] < der_thr and CFAR_MAG[i]==0 and echo_start_flag == 1
            if(i >= (samples.size() - der_size -1)) {
                //qDebug() << " Echo ended because it reached end of data @i = " << i;
                 echo_start_flag = 0;
            }
            under_snr_cnt++;
            if(under_snr_cnt >= SNR_debounce) {                         //Debounce - Echo has to stay under SNR for 'SNR_debounce' number of samples
                echo_start_flag = 0;
                //qDebug() << " Echo ended @i = " << i << " - " << i*sim_sampling_period << "ms";
            }
        }
        // Debouncer reset
        else if(SNR_curve[i] >= CFAR_SNR_THRESHOLD && echo_start_flag == 1) {
            under_snr_cnt = 0;
        }
        // Start of echo - Both conditions 1, Echo over SNR THR and echo didnt start yet
        else if(DIFF_shifted[i] >= der_thr && CFAR_MAG[i]>0 && echo_start_flag == 0 && SNR_curve[i] >= CFAR_SNR_THRESHOLD) {
            under_snr_cnt = 0;
            echo_start_flag = 1;         //Set echo start flag
            //qDebug() << "   Echo started @i = " << i << " - " << i*sim_sampling_period << "ms";
        }
        /****************************/
        //Signal for TOF detection
        //Start of echo, Rising edge
        if(DIFF_shifted[i] >= TOF_der_thr && samples[i] >= min_cfar_thr_shifted[i] && SNR_curve[i] >= SNR_THR_TOF && (i > 0 && ECHO_DET_TOF[i-1] == 0)) {
            TOF_under_snr_cnt = 0;
            ECHO_DET_TOF[i] = 1;
        }
        //End of echo, Falling edge
        else if((SNR_curve[i] < SNR_THR_TOF && (i > 0 && ECHO_DET_TOF[i-1] == 1)) || \
                (i == (samples.size() - der_size -1) && (i > 0 && ECHO_DET_TOF[i-1] == 1))) {
            TOF_under_snr_cnt++;
            if(TOF_under_snr_cnt >= SNR_debounce || (i >= (samples.size() - der_size -1))) {    //Debounce - Echo has to stay under SNR for 'SNR_debounce' number of samples
                ECHO_DET_TOF[i] = 0;
            }
            else {
                ECHO_DET_TOF[i] = ECHO_DET_TOF[i-1];
            }

        }
        else if(i>0) {
            ECHO_DET_TOF[i] = ECHO_DET_TOF[i-1];
        }
        //Debouncer reset
        if(SNR_curve[i] >= SNR_THR_TOF) {
            TOF_under_snr_cnt = 0;
        }

        /****************************/
        //Pass data to CFAR output
        if(echo_start_flag == 1) {
            ECHO_DET[i] = samples[i];
        }
        else {
            ECHO_DET[i] = 0;
        }

        //Calculate Moving Average - Every loop except if there is confirmed echo, then keep constant value under echo
        if(echo_start_flag == 0) {
            prev_inputs.prepend(samples[i]);            //prepend the newest sample to FIFO buffer
            if(prev_inputs.size() > MVA_samples) {      //if FIFO buffer is full
                prev_inputs.remove(MVA_samples);        //delete oldest sample
            }
            float temp = 0;
            for(j=0; j<prev_inputs.size(); j++) {
                temp = temp + prev_inputs[j];           //sum all samples in buffer
            }
            MVAVG[i] = temp/prev_inputs.size();         //divide the sum by the buffer length
            if(MVAVG[i] < MVA_MIN_VAL)
                MVAVG[i] = MVA_MIN_VAL;
        }
    }

    //Debug signals
    QVector<double> TOF_cfar_out;                       //CFAR output for ToF detection
    TOF_cfar_out.resize(samples.size());
    for(i = 0; i<samples.size()-der_size; i++) {
        if(DIFF_shifted[i] > der_thr) {
            over_diff_thr[i] = 400;
        }
        else {
            over_diff_thr[i] = 0;
        }
        if(SNR_curve[i] >= CFAR_SNR_THRESHOLD) {
            over_SNR_thr[i] = 600;
        }
        else {
            over_SNR_thr[i] = 0;
        }
        //ToF debug
        if(ECHO_DET_TOF[i] != 0) {
            ECHO_DET_TOF[i] = samples.at(i);
        }
        else {
            ECHO_DET_TOF[i] = 0;
        }
    }

    for(i = 0; i<samples.size(); i++) {
        DBG1.append(min_cfar_thr_shifted.at(i));    //min_cfar_thr
        DBG2.append(MVAVG.at(i)*pow(10,(1.0*CFAR_SNR_THRESHOLD)/20));   //Magnitude SNR threshold curve
        DBG3.append(DIFF_TOF_shifted.at(i));        //MP_DIFF
        DBG4.append(over_min_cfar_thr.at(i));       //over_thr
        DBG5.append(over_SNR_thr.at(i));            //over_snr
        DBG6.append(over_diff_thr.at(i));           //over_diff
        DBG7.append(SNR_curve.at(i));               //snr curve
        DBG8.append(DIFF_shifted.at(i));            //DIFF
        DBG9.append(MVAVG.at(i)*pow(10,(1.0*SNR_THR_TOF)/20));          //TOF SNR threshold curve
        DBG10.append(TOF_cfar_out.at(i));           //TOF CFAR output

    }
    return;
}

/*****************************************************************************/
/*****************************************************************************/
/*             CFAR_new FUNCTION - new separated Mag/TOF CFAR                */
/*****************************************************************************/
/*****************************************************************************/
//Calculate CFAR_new
void CFARsim::CalculateCFAR_new(QVector<double> samples, quint8 der_size, double scale, quint8 offset, int der_thr, \
                            int CFAR_SNR_THRESHOLD, QVector<double> &CFAR_MAG, QVector<double> &ECHO_DET, quint8 MVA_samples,\
                            quint8 SNR_debounce, QVector<double> &DBG1, QVector<double> &DBG2, QVector<double> &DBG3, \
                            QVector<double> &DBG4, QVector<double> &DBG5, QVector<double> &DBG6, QVector<double> &DBG7, \
                            QVector<double> &DBG8, QVector<double> &DBG9, QVector<double> &DBG10, quint32 CFAR_START_SAMPLE, quint8 CFAR_TYPE)
{
    QVector<double> min_cfar_thr;
    QVector<double> diff;
    QVector<double> diff_TOF;
    QVector<double> diff_differences;
    QVector<double> diff_differences_TOF;
    QVector<double> mins;
    QVector<double> min_cfar_thr_shifted;                   //Time shifted cfar_thr to better match FPGA CFAR implementation
    QVector<double> over_diff_thr;                          //Debug signals - high if condition is fulfilled (diff threshold, SNR threshold, cfar threshold)
    QVector<double> over_SNR_thr;
    QVector<double> over_min_cfar_thr;
    int i = 0, j = 0;
    quint8 valueS = 2;
    quint8 MVA_MIN_VAL = 2;                                 //Minimum value of moving average
    quint8 TOF_der_size = 4;                                //DER_SIZE for ToF edge detection, der_size = 1 -> 4 samples differentiation (0,1,2,3 -> 2,4,6,8)
    if(der_size < TOF_der_size) {
        TOF_der_size = der_size;
        qDebug() << "TOF_der_size value adjusted to match der_size";
    }

    min_cfar_thr.resize(samples.size());
    diff.resize(samples.size());                            //Differentiation signal for Magnitude CFAR
    diff_TOF.resize(samples.size());                        //Differentiation signal for ToF Detection CFAR
    diff_differences.resize(der_size -1);
    diff_differences_TOF.resize(TOF_der_size -1);
    mins.resize(der_size / valueS);
    min_cfar_thr_shifted.resize(samples.size());
    over_diff_thr.resize(samples.size());
    over_SNR_thr.resize(samples.size());
    over_min_cfar_thr.resize(samples.size());

    double multiplier = scale/2.5;                          //Convert new scale to old multiplier

    //Generate CFAR_MAG and DIFF arrays
    for (i = 0; i < (samples.size() - der_size); i++)
    {
        if(i < CFAR_START_SAMPLE)                           //delayed start of CFAR calculation
            continue;

        for (j = 0; j < (der_size - 1); j++)                //Calculate differentiation
        {
            diff_differences[j] = samples[i+j+1] - samples[i+j];
        }
        diff[i] = QVectorAvg(diff_differences);

        for(j = 0; j < (TOF_der_size - 1); j++) {
            diff_differences_TOF[j] = samples[i+j+1] - samples[i+j];
        }
        diff_TOF[i] = QVectorAvg(diff_differences_TOF);

        for (j = 0; j < der_size / valueS; j++)              //Calculate CFAR thr
        {
            mins[j] = QVectorMax(samples.mid(i + j*valueS, valueS));
        }

        //min_cfar_thr[i] = QVectorMin(mins) * multiplier + offset;     //Change of cfar threshold calculation based on MOBIS request, 14/05/2021
        if(offset > QVectorMin(mins) * multiplier) {
            min_cfar_thr[i] = offset;
        }
        else {
            min_cfar_thr[i] = QVectorMin(mins) * multiplier;
        }

        //Time shift the min_cfar_thr signal FORWARD to match FPGA implementation of CFAR
        #ifdef SHIFT_CFAR_THR
        if(i < der_size+1) {
            min_cfar_thr_shifted[i] = offset;               //Dont shift the first samples to avoid array index out of range
        }
        else {
            min_cfar_thr_shifted[i] = min_cfar_thr[i-(der_size+1)];
        }
        #else
        min_cfar_thr_shifted[i] = min_cfar_thr[i];
        #endif
        /*********************************/
        // CFAR_MAG only above CFAR thr
        if (samples[i] >= min_cfar_thr_shifted[i])
        {
            CFAR_MAG[i] = samples[i];
            over_min_cfar_thr[i] = 500;
        }
        else
        {
            CFAR_MAG[i] = 0;
            over_min_cfar_thr[i] = 0;
        }
    }

    //Find edges of possible echoes
    QVector<double> DIFF_shifted;           //Time shifted differentiation signal - Mag.
    QVector<double> DIFF_TOF_shifted;       //Time shifted differentiation signal - ToF CFAR
    QVector<double> MVAVG;                  //Moving average of input signal except potential echoes
    QVector<double> ConfirmedEchoes;        //Echoes confirmed all by 3 conditions - magnitude, differentiation threshold, SNR threshold
    QVector<double> SNR_curve;              //Signal to Noise ratio curve

    DIFF_shifted.resize(samples.size());
    DIFF_TOF_shifted.resize(samples.size());
    MVAVG.resize(samples.size());
    ConfirmedEchoes.resize(samples.size());
    SNR_curve.resize(samples.size());

    //Time shift FOWRWARD the DIFF signal used to calculate edges of echoes
    int sh = 0;
    /*switch(der_size) {  //(GUI: 0,1,2,3) -> 2,4,6,8
        case 8: sh = 1; break;
        case 6: sh = 2; break;
        default: break;
    }
    qDebug()<<"der_size =" << der_size << "shift coef. sh =" << sh;*/

    #ifdef SHIFT_DIFF
    for(i=0; i<(samples.size() - der_size); i++) {
        if(i < CFAR_START_SAMPLE)
            continue;
        if(i < (der_size/2)+sh) {
            DIFF_shifted[i] = 0;
        }
        else {
            DIFF_shifted[i] = diff[i-(der_size/2)+sh];
        }
        if(i < (TOF_der_size/2)+sh) {
            DIFF_TOF_shifted[i] = 0;
        }
        else {
            DIFF_TOF_shifted[i] = diff_TOF[i-(TOF_der_size/2)+sh];
        }
    }
    #else
    for(i=0; i<samples.size(); i++){
        DIFF_shifted[i] = diff[i];
        DIFF_TOF_shifted[i] = diff_TOF[i];
    }
    #endif
    //qDebug() << "Shifted by: " << der_size/2 << " samples forward";

    //####################################################
    //Sweep input data and find edges of echoes
    //Rising edge of echo - 3 conditions need to be fulfilled - data above CFAR_THR (CFAR_MAG>0), differentiation above differentiation THR (DIFF_shifted[i] > der_thr), SNR above SNR threshold
    //Falling edge of echo - when SNR falls below SNR Threshold
    //First step - find when at least one condition (CFAR_MAG or differentiation) is fulfilled
    //Until it is fulfilled, calculate Moving average of "noise"
    QVector<double> prev_inputs;                        //Buffer of previous X samples
    //prev_inputs.resize(MVA_samples);
    quint8 under_snr_cnt = 0;
    bool echo_start_flag = 0;

    for(i = 0; i < (samples.size() - der_size); i++) {
        if(i < CFAR_START_SAMPLE) {
            //ECHO_DET[i] = samples[i];                   //keep raw correlator output prior to CFAR start time
            continue;
        }

        //Calculate moving average - calculation is performed in the end of loop if echo is not started
        if(i>0){
            MVAVG[i] =  MVAVG[i-1];    //Temporarily get the previous sample to calculate current SNR
        }
        else {
            MVAVG[i] = MVA_MIN_VAL;
        }

        //Calculate SNR
        if(samples[i] > 0 && i>0) {        //Prevent undefined operations - Log(0) or log(negative number)
            SNR_curve[i] = 20*log10((float(samples[i]))/float(MVAVG[i-1]));
        }
        else {
            SNR_curve[i] = 0.01;    //Replacement value
        }

        // End of echo - Echo already started but its magnitude dropped below SNR THR
        if((SNR_curve[i] < CFAR_SNR_THRESHOLD && echo_start_flag == 1) || (i == (samples.size() - der_size -1) && echo_start_flag == 1)) {     //changed from: DIFF_shifted[i] < der_thr and CFAR_MAG[i]==0 and echo_start_flag == 1
            if(i >= (samples.size() - der_size -1)) {
                //qDebug() << " Echo ended because it reached end of data @i = " << i;
                 echo_start_flag = 0;
            }
            under_snr_cnt++;
            if(under_snr_cnt >= SNR_debounce) {                         //Debounce - Echo has to stay under SNR for 'SNR_debounce' number of samples
                echo_start_flag = 0;
                //qDebug() << " Echo ended @i = " << i << " - " << i*sim_sampling_period << "ms";
            }
        }
        // Debouncer reset
        else if(SNR_curve[i] >= CFAR_SNR_THRESHOLD && echo_start_flag == 1) {
            under_snr_cnt = 0;
        }
        // Start of echo - Both conditions 1, Echo over SNR THR and echo didnt start yet
        else if(DIFF_shifted[i] >= der_thr && CFAR_MAG[i]>0 && echo_start_flag == 0 && SNR_curve[i] >= CFAR_SNR_THRESHOLD) {
            under_snr_cnt = 0;
            echo_start_flag = 1;         //Set echo start flag
            //qDebug() << "   Echo started @i = " << i << " - " << i*sim_sampling_period << "ms";
        }
        /****************************/
        //Pass data to CFAR output
        if(echo_start_flag == 1) {
            ECHO_DET[i] = samples[i];
        }
        else {
            ECHO_DET[i] = 0;
        }

        //Calculate Moving Average - Every loop except if there is confirmed echo, then keep constant value under echo
        if(echo_start_flag == 0) {
            prev_inputs.prepend(samples[i]);            //prepend the newest sample to FIFO buffer
            if(prev_inputs.size() > MVA_samples) {      //if FIFO buffer is full
                prev_inputs.remove(MVA_samples);        //delete oldest sample
            }
            float temp = 0;
            for(j=0; j<prev_inputs.size(); j++) {
                temp = temp + prev_inputs[j];           //sum all samples in buffer
            }
            MVAVG[i] = temp/prev_inputs.size();         //divide the sum by the buffer length
            if(MVAVG[i] < MVA_MIN_VAL) {
                MVAVG[i] = MVA_MIN_VAL;
            }
        }
    }

    //Debug signals
    for(i = 0; i<samples.size()-der_size; i++) {
        if(DIFF_shifted[i] > der_thr) {
            over_diff_thr[i] = 400;
        }
        else {
            over_diff_thr[i] = 0;
        }
        if(SNR_curve[i] >= CFAR_SNR_THRESHOLD) {
            over_SNR_thr[i] = 600;
        }
        else {
            over_SNR_thr[i] = 0;
        }
    }

    //qDebug() << "samples.size()" << samples.size();
    //qDebug() << "CFAR type" << CFAR_TYPE;
    for(i = 0; i<samples.size(); i++) {
        switch(CFAR_TYPE) {
        case CFAR_TYPE_ALLDBG:
            DBG1.append(min_cfar_thr_shifted.at(i));    //cfar threshold
            DBG2.append(MVAVG.at(i)*pow(10,(1.0*CFAR_SNR_THRESHOLD)/20));   //SNR threshold
            DBG3.append(DIFF_TOF_shifted.at(i));        //MP_DIFF
            DBG4.append(over_min_cfar_thr.at(i));       //over_thr
            DBG5.append(over_SNR_thr.at(i));            //over_snr
            DBG6.append(over_diff_thr.at(i));           //over_diff
            DBG7.append(SNR_curve.at(i));               //snr curve
            DBG8.append(DIFF_shifted.at(i));            //DIFF
            DBG9.append(DIFF_shifted.at(i));            //DIFF
            DBG10.append(DIFF_TOF_shifted.at(i));       //MP_DIFF
            break;
        case CFAR_TYPE_NODBG: break;
        default: break;
        }
    }
    return;
}

/*************************************************/
double CFARsim::QVectorMin(QVector<double> vec)
{
    /*uint i;
    double min = 1024;
    for (i = 0; i < vec.size(); i++)
    {
        if (vec[i] < min)
        {
            min = vec[i];
        }
    }
    return min;*/
    return *std::min_element(vec.begin(), vec.end());   //changed to std::... algorithm thanks to M.Navratil suggestion 2021-06-29
}
/*************************************************/
double CFARsim::QVectorMax(QVector<double> vec)
{
    /*uint i;
    double max = 0;
    for (i = 0; i < vec.size(); i++)
    {
        if (vec[i] > max)
        {
            max = vec[i];
        }
    }
    return max;*/
    return *std::max_element(vec.begin(), vec.end());
}
/*************************************************/
double CFARsim::QVectorAvg(QVector<double> vec)
{
    uint i;
    double avg = 0;
    for (i = 0; i < vec.size(); i++)
    {
        avg = avg + vec[i];
    }
    return avg/vec.size();                              //changed thanks to M.Navratil suggestion
}

/*************************************************/
//Update CFAR control checkboxes when switching between versions
void CFARsim::slot_updateCFARcontrols(void) {
    if(ui->radioButtonCFARv2->isChecked()) {        //Mag+TOF cfar from original input data [original version]
        ui->lineEdit_211->setEnabled(false);
        ui->lineEdit_210->setEnabled(false);
        ui->lineEdit_209->setEnabled(false);
    }
    else if(ui->radioButtonCFARv1->isChecked()) {   //Separated Mag/ToF CFAR
        ui->lineEdit_211->setEnabled(true);
        ui->lineEdit_210->setEnabled(true);
        ui->lineEdit_209->setEnabled(true);
    }
    return;
}

/*****************************************************************************/
/*****************************************************************************/
/*                       COMPRESSION/DECOMPRESSION                           */
/*****************************************************************************/
/*****************************************************************************/
//
bool CFARsim::CompressDebug(void) {
    //init data arrays
    if(sim_channel_count < 1 || sim_samples.size()<1) {
        qDebug("no input data");
        return false;
    }

    //read state of checkboxes
    bool checkbox_state[CMPR_DBG_CNT][4];   //0 = cfar en, 1 = undersampling en, 2 = compr./decompr en, 3 = dbg output en.
    uint en_reg = CMPR_DBG_ENA.toUInt();
    for(int i=0; i<3; i++){
        for(int j=0; j<CMPR_DBG_CNT; j++) {
            bool bit = ((en_reg >> (j+(i*CMPR_DBG_CNT))) & 1U);
            checkbox_state[j][i] = bit;
        }
    }
    checkbox_state[0][3] = ui->cb_dbgin_1->currentIndex()>0? 1 : 0; //inline if zapis - je vetsi nez 0? ano - zapise 1, ne - zapise 0
    checkbox_state[1][3] = ui->cb_dbgin_2->currentIndex()>0? 1 : 0;
    checkbox_state[2][3] = ui->cb_dbgin_3->currentIndex()>0? 1 : 0;
    quint8 ena_cnt = checkbox_state[0][3]+checkbox_state[1][3]+checkbox_state[2][3];
    sim_cmpr_dbg_names.clear();
    sim_cmpr_en_ch_cnt = 0;

    if(ena_cnt == 0) {
        //qDebug() << "sim_cmpr_dbg_names.size" << sim_cmpr_dbg_names.size();
        //qDebug() << "sim_cmpr_en_ch_cnt" << sim_cmpr_en_ch_cnt;
        return false;   //no debug channel is enabled, end
    }

    if(ui->cb_dbgin_1->currentIndex() > 0) {
        sim_cmpr_dbg_names.append(QString("CMPR_DBG1_%1").arg(ui->cb_dbgin_1->currentText()));
        sim_cmpr_en_ch_cnt++;
    }
    if(ui->cb_dbgin_2->currentIndex() > 0) {
        sim_cmpr_dbg_names.append(QString("CMPR_DBG2_%1").arg(ui->cb_dbgin_2->currentText()));
        sim_cmpr_en_ch_cnt++;
    }
    if(ui->cb_dbgin_3->currentIndex() > 0) {
        sim_cmpr_dbg_names.append(QString("CMPR_DBG3_%1").arg(ui->cb_dbgin_3->currentText()));
        sim_cmpr_en_ch_cnt++;
    }
    //qDebug() << "sim_cmpr_dbg_names" << sim_cmpr_dbg_names;
    //qDebug() << "sim_cmpr_dbg_names.size" << sim_cmpr_dbg_names.size();
    //qDebug() << "sim_cmpr_en_ch_cnt" << sim_cmpr_en_ch_cnt;


    sim_cmpr_samples.clear();                   //clear previous results
    sim_cmpr_time.clear();                      //clear previous results

    CFAR(); //start CFAR calculation - data could be needed in step 1

    //sim_cmpr_samples[debug channel]
    //step-by-step process
    for(int dbg_ch = 0; dbg_ch < CMPR_DBG_CNT; dbg_ch++) {
        //debug enable
        if(checkbox_state[dbg_ch][3] == 0) {     //debug output disabled
            continue;
        }

        //Load timebase
        QVector<double> timebase;               //input data sampling time steps
        timebase = sim_time;                    //timebase of original data (plot X axis)

        //Load selected input data
        quint8 data_index = 0;
        switch(dbg_ch) {
            case 0: data_index = ui->cb_dbgin_1->currentIndex()-1; break;
            case 1: data_index = ui->cb_dbgin_2->currentIndex()-1; break;
            case 2: data_index = ui->cb_dbgin_3->currentIndex()-1; break;
            default: break;
        }

        QVector<QVector<double>> results(2);    //old data from previous step / results from current step
        results[0] = sim_samples[data_index];   //load original data
        /////////////////////////////////////////////////////////////////////
        //cfar
        if(checkbox_state[dbg_ch][0] != 0) {
            results[1] = sim_samples[data_index+(sim_channel_count*CFAR_OUT_MEM_MULTIPLIER)];   //copy results for selected channel
            //qDebug() << "cfar ch" << dbg_ch+1;
        }
        else {
           results[1] = results[0];             //disabled - no change in data
        }
        results[0].clear();
        results[0] = results[1];
        results[1].clear();

        /////////////////////////////////////////////////////////////////////
        //undersampling - now only if compression/decompression is enabled
        if(checkbox_state[dbg_ch][1] != 0 && checkbox_state[dbg_ch][2] != 0) {
            //qDebug() << "undersampling ch" << dbg_ch+1;
            float sim_cmpr_smpl_per = 0;
            results[1] = Undersampling(results[0], sim_cmpr_smpl_per);
            timebase.resize(results[1].size());         //undersampling changes X axis (time steps)
            for(int j=0; j<results[1].size(); j++) {
                timebase[j] = sim_cmpr_smpl_per*j;
            }
            //qDebug() << "original period" << sim_sampling_period << "ms";
            //qDebug() << "undersampling period" << sim_cmpr_smpl_per << "ms";
            //qDebug() << "undersampling result" << results[1];
        }
        else {
           results[1] = results[0];             //disabled - no change in data
        }
        results[0].clear();
        results[0] = results[1];
        results[1].clear();

        /////////////////////////////////////////////////////////////////////
        //compression/decompression
        if(checkbox_state[dbg_ch][2] != 0) {
            //qDebug() << "compr./decompr. ch" << dbg_ch+1;
            results[1] = ComprDecompr(results[0]);
        }
        else {
           results[1] = results[0];             //disabled - no change in data
        }
        results[0].clear();
        results[0] = results[1];
        results[1].clear();

        /////////////////////////////////////////////////////////////////////
        //debug enable
        if(checkbox_state[dbg_ch][3] != 0) {     //debug output enabled
            //qDebug() << "dbg ch" << dbg_ch+1;
        }

        /////////////////////////////////////////////////////////////////////
        sim_cmpr_time.append(timebase);
        sim_cmpr_samples.append(results.at(0));  //append to final results array -> plot it
    }
    return true;
}

/*************************************************/
//Undersampling - based on VHDL/VBA FPGA code from T. Suchy
QVector<double> CFARsim::Undersampling(QVector<double> samples_in, float &sim_undr_sampling_per) {
    QVector<double> samples_out;
    int mag = 0;
    struct t_REG reg_s;
    float og_s_period = sim_time.at(1);     //sample period of original data
    sim_undr_sampling_per = og_s_period*2;  //sample period of undersampled data

    for(int i=0; i<samples_in.size(); i++) {
        mag = samples_in.at(i);
        adv_smp(reg_s, mag);        //reg_s contains modified magnitude data which needs to be undersampled
        if(i % 2 == 0) {            //every even sample
            samples_out.append(reg_s.adv_mag);
        }
    }
    return samples_out;
}

/*************************************************/
//Advanced Sampling - based on VHDL/VBA FPGA code from T. Suchy
void CFARsim::adv_smp(struct t_REG &reg_s, int mag) {
    struct t_REG v;
    v = reg_s;

    if(reg_s.adv_mag > mag) {
        v.adv_mag_falling_dly = 1;
    }
    else {
        v.adv_mag_falling_dly = 0;
    } 

    if(reg_s.adv_mag_falling_dly == 1 && v.adv_mag_falling_dly == 0 && reg_s.adv_mag_kept == 0) {
        v.adv_mag_kept = 1;
    }
    else {
        v.adv_mag_kept = 0;
        v.adv_mag = mag;
    }
    reg_s = v;
}

/*************************************************/
//Update compress. debug output signals after changing selection of input signal
void CFARsim::slot_updateCB(void) {
    //qDebug() << "slot_updateCB";
    CompressDebug();
    SetupPlotComboBox(true);
    slot_PlotData();
    UpdateTable();
    //qDebug() << "sim_cmpr_samples" << sim_cmpr_samples;
    //qDebug() << "sim_cmpr_time" << sim_cmpr_time;
}

/*************************************************/
//Update results table
void CFARsim::slot_updateTable(void) {
    UpdateTable();
}

/*************************************************/
//Compression algorithm - Mode 2,3,6
QVector<double> CFARsim::ComprDecompr(QVector<double> samples_in) {
    QVector<double> result;
    QVector<double> C_result;
    if(samples_in.size() < 1) {
        return result;
    }

    //Spec page 76
    //5bit logarithmic scale encoding, 3bits per sample
    static quint8 Divider_lut[4] = {128, 108, 91, 76};

    for(int i=0; i<samples_in.size(); i=i+9) {    //9 samples at a time
        if(i+9 > samples_in.size()) {
            break;
        }
        quint16 Max = 0;
        //Find maximum encoded value
        for(int j=i; j<i+9; j++) {
            if (samples_in[j] > Max) {
                Max = samples_in[j];
            }
        }
        //Scale bit shift
        quint8 Scale_shift = 0;
        if((Max & 0x3C0) == 0) {
            Max = Max << 4;
            Scale_shift = Scale_shift + 4;
        }
        if((Max & 0x300) == 0) {
            Max = Max << 2;
            Scale_shift = Scale_shift + 2;
        }
        if((Max & 0x200) == 0) {
            Max = Max << 1;
            Scale_shift = Scale_shift + 1;
        }
        //Scale divider
        quint8 Scale_div = 0;
        if(Max < (Divider_lut[1]*8)) {
            Scale_div = 1;
        }
        if(Max < (Divider_lut[2]*8)) {
            Scale_div = 2;
        }
        if(Max < (Divider_lut[3]*8)) {
            Scale_div = 3;
        }

        quint8 Scale = Scale_shift*4 + Scale_div;  //Scale is 5 bit value

        //Sample encoding
        quint16 S = 0;              //sample
        double C_sample = 0;        //compressed sample
        quint8 D = Divider_lut[(Scale & 3)];
        quint16 T = Scale >> 2;
        for(int k=i; k<i+9; k++) {
            S = samples_in[k];
            if((Scale & 0x10) != 0) {
                S = S << 4;
            }
            if((Scale & 0x08) != 0) {
                S = S << 2;
            }
            if((Scale & 0x04) != 0) {
                S = S << 1;
            }
            C_sample = floor(S*1.0/Divider_lut[(Scale & 3)]);
            C_result.append(C_sample);    //append to compression   result array

            //Sample Decompression
            S = C_sample;
            if(S == 0) {
                S = 0;
            }
            else {
                S = S*D+(D/2);
            }
            result.append(S >> T);
        }
    }
    return result;
}

/*************************************************/
//Update Dynamic Scaling Plot
void CFARsim::slot_updateDynScaling(void) {
    /*Dynamic scaling table structure
    *   scale_arr[0]    - scaling for channel A, abs. time [ms]
    *   scale_arr[1]    - scaling for channel A, scaling factors [dB]
    *   scale_arr[2]    - abs. time for channel B scaling   [ms]
    *   scale_arr[3]    - abs. scaling factor for channel B [dB]
    */
    QVector<QVector<double>> scale_arr;
    DynScaleCaclulate(scale_arr);

    ui->ptDynScale->graph(0)->setData(scale_arr.at(0), scale_arr.at(1));
    ui->ptDynScale->graph(1)->setData(scale_arr.at(2), scale_arr.at(3));
    ui->ptDynScale->replot();

}

/*************************************************/
//Scale Input data using Dynamic Scaling Curve
bool CFARsim::DynScaleData(int channel, QVector<double> time, QVector<double> &data) {
    qDebug() << "DynScaleData";

    /*Dynamic scaling table structure
    *   scale_arr[0]    - scaling for channel A, abs. time [ms]
    *   scale_arr[1]    - scaling for channel A, scaling factors [dB]
    *   scale_arr[2]    - abs. time for channel B scaling   [ms]
    *   scale_arr[3]    - abs. scaling factor for channel B [dB]
    */
    QVector<QVector<double>> scale_arr;
    DynScaleCaclulate(scale_arr);
    //qDebug() << "scale_arr" << scale_arr;

    //Check if Channel is selected for scaling
    int scale_arr_idx = 0;
    if(ui->cbDynScChA->currentIndex()-1 == channel) {
        scale_arr_idx = 0;
    }
    else if (ui->cbDynScChB->currentIndex()-1 == channel) {
        scale_arr_idx = 2;
    }
    else {
        return false;
    }


    for(int i=0; i<time.size(); i++) {
        for(int k=1; k<scale_arr.at(0).size(); k++) {
            //Find interval to do interpolation, using abs. time
            if(time.at(i) >= scale_arr[scale_arr_idx].at(k-1)  &&  time.at(i) < scale_arr[scale_arr_idx].at(k)) {
                //     y                      x                x0                              y0                                      x1                              y1
                double y = LinInterpolate(time.at(i), scale_arr[scale_arr_idx].at(k-1), scale_arr[scale_arr_idx+1].at(k-1), scale_arr[scale_arr_idx].at(k), scale_arr[scale_arr_idx+1].at(k));
                //y - scaling factor in dB, convert it to abs. value and multiply 'data' by this factor
                //qDebug() << "x" << time.at(i) << ", y [dB] =" << y << ", -> =" << pow(10, y/20);
                data[i] = data[i] * pow(10, y/20);
            }
        }
    }

    return true;
}

/*************************************************/
//Caclulate scaling factors
bool CFARsim::DynScaleCaclulate(QVector<QVector<double>> &scale_tb) {
    qDebug() << "DynScaleCaclulate";
    QVector<double> abst_A, abst_B;     //absolute time
    QVector<double> deltat_A, deltat_B; //delta time
    QVector<double> deltay_C, deltay_D, y_A, y_B;

    deltat_A << DYN_SC_TIME_0.toDouble() << DYN_SC_TIME_1.toDouble() << DYN_SC_TIME_2.toDouble()
       << DYN_SC_TIME_3.toDouble() << DYN_SC_TIME_4.toDouble() << DYN_SC_TIME_5.toDouble();


    deltat_B << DYN_SC_TIME_0_B.toDouble() << DYN_SC_TIME_1_B.toDouble() << DYN_SC_TIME_2_B.toDouble()
       << DYN_SC_TIME_3_B.toDouble() << DYN_SC_TIME_4_B.toDouble() << DYN_SC_TIME_5_B.toDouble();

    deltay_C << DYN_SC_VAL_0.toDouble() << DYN_SC_VAL_1.toDouble() << DYN_SC_VAL_2.toDouble()
       << DYN_SC_VAL_3.toDouble() << DYN_SC_VAL_4.toDouble() << DYN_SC_VAL_5.toDouble();

    deltay_D << DYN_SC_VAL_0_B.toDouble() << DYN_SC_VAL_1_B.toDouble() << DYN_SC_VAL_2_B.toDouble()
       << DYN_SC_VAL_3_B.toDouble() << DYN_SC_VAL_4_B.toDouble() << DYN_SC_VAL_5_B.toDouble();

    double temp_A = 0, temp_B = 0;
    double temp_yC = 0, temp_yD = 0;
    for(int i=0; i<deltat_A.size(); i++) {
        temp_A = temp_A + deltat_A.at(i);
        abst_A << temp_A;

        temp_B = temp_B + deltat_B.at(i);
        abst_B << temp_B;

        const int up_lim = 60;

        temp_yC = temp_yC + deltay_C.at(i);
        if(temp_yC > up_lim) temp_yC = up_lim;
        if(temp_yC < -up_lim) temp_yC = -up_lim;
        y_A << temp_yC;

        temp_yD = temp_yD + deltay_D.at(i);
        if(temp_yD > up_lim) temp_yD = up_lim;
        if(temp_yD < -up_lim) temp_yD = -up_lim;
        y_B << temp_yD;
    }

    //Prolong graph until max possible measurement duration
    abst_A << 65.024;
    abst_B << 65.024;
    y_A << y_A.last();
    y_B << y_B.last();

    //Prepare scaling factor table for both inputs
    scale_tb.clear();
    scale_tb << abst_A << y_A << abst_B << y_B;

    return true;
}

/*************************************************/
//Linear interpolation
//https://cs.wikipedia.org/wiki/Line%C3%A1rn%C3%AD_interpolace
//Returns calculated value from interpolation Y
double CFARsim::LinInterpolate(double x, double x0, double y0, double x1, double y1) {
    double y = y0 + (x-x0) * (y1-y0)/(x1-x0);
    return y;
}
