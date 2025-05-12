#include "measurement.h"
#include "ui_measurement.h"
#include "measurement_regmap.h"
#include "dsi3slave_regmap.h"
#include "dsi3slave.h"
#include "dsi3master.h"
#include "uartuartcontroller.h"
#include "uartspicontroller.h"
#include "uartssicontroller.h"
#include "mainapp.h"
#include "QTime"
#include "math.h"
#include "qclipboard.h"

#define LOG_TYPE_INFO       (4)
#define LOG_TYPE_ERROR      (6)
#define LOG_TYPE_BUTTON     (7)
#define LOG_TYPE_TIME       (11)

#define VERSION_MOBE        (0)
#define VERSION_ULSE        (1)
#define VERSION_RDUS        (2)
#define VERSION_RDUSDMX     (3)
#define VERSION_MOBEAPR     (4)
#define VERSION_MOBEJUN     (5)
#define VERSION_RDUS2DMX    (6)
#define VERSION_MOBEDMX     (7)
#define VERSION_RDUS3DCH    (8)
#define VERSION_RDUSBSD     (9)
#define VERSION_RDUS3DMX    (10)
#define VERSION_MOBE2DMX    (11)

Measurement *gp_Measurement;

QTimer *MEAStimer;

Measurement::Measurement(QWidget *parent) :
    Applet(parent)
{
    m_meas_slot_number = 0;
    m_first_measurement[0] = true;
    m_first_measurement[1] = true;
    m_meas_counter = -1;
    m_meas_tof_ena = false;
    m_counter = 0;
    m_history_pointer_start = 0;
    m_history_pointer_end = 0;
    m_packets = 0;
    m_PERerrors = 0;
    ui = new Ui::Measurement;
    ui->setupUi(this);
    createCustomUi();
    setWindowIcon(QIcon(":/board/onlogo.ico"));
    setWindowTitle("Measurement");
}

Measurement::~Measurement()
{
    delete ui;
}

bool Measurement::initialize()
{
    int i, j, k;

    for (i = 0; i < DIAGWORD_SIZE; i++)
    {
        DIAGWORD0[i].makeSetup(DIAGWORD0_INIT);
        DIAGWORD0[i].setName(QString("DIAGWORD0_%1").arg(i+1));
        DIAGWORD1[i].makeSetup(DIAGWORD1_INIT);
        DIAGWORD1[i].setName(QString("DIAGWORD1_%1").arg(i+1));
        DIAGWORD2[i].makeSetup(DIAGWORD2_INIT);
        DIAGWORD2[i].setName(QString("DIAGWORD2_%1").arg(i+1));
        addDataNode(&DIAGWORD0[i]);
        addDataNode(&DIAGWORD1[i]);
        addDataNode(&DIAGWORD2[i]);

    }

    for (i = 0; i < SCHEDULER_SIZE; i++)
    {
        SCHEDULER_MEASURE[i].makeSetup(SWriteMEASURE_INIT);
        SCHEDULER_MEASURE[i].setName(QString("SCHEDULER_MEASURE_%1").arg(i+1));
        addDataNode(&SCHEDULER_MEASURE[i]);
    }

    GRAPH_DETAIL_SEL[0].makeSetup("@variable=GRAPH_DETAIL_SEL_1" "@value=0" "@min=0" "@max=11");
    addDataNode(&GRAPH_DETAIL_SEL[0]);
    GRAPH_DETAIL_SEL[1].makeSetup("@variable=GRAPH_DETAIL_SEL_2" "@value=0" "@min=0" "@max=11");
    addDataNode(&GRAPH_DETAIL_SEL[1]);

    for (i = 0; i < 8; i++)
    {
        GRAPH_OVERVIEW_SEL[i].makeSetup("@variable=REG" "@value=0" "@min=0" "@max=11");
        GRAPH_OVERVIEW_SEL[i].setName(QString("GRAPH_OVERVIEW_SEL_%1").arg(i+1));
        addDataNode(&GRAPH_OVERVIEW_SEL[i]);
    }

    SCHEDULER_ENA.makeSetup(ENA_1_10_INIT);
    SCHEDULER_ENA.setName("SCHEDULER_ENA");
    addDataNode(&SCHEDULER_ENA);
    SUMMARYCHANNEL_ENA.makeSetup(ENA_1_10_INIT);
    SUMMARYCHANNEL_ENA.setName("SUMMARYCHANNEL_ENA");
    addDataNode(&SUMMARYCHANNEL_ENA);

    SUMMARY_STARTMS.makeSetup("@variable=SUMMARY_STARTMS" "@value=0" "@min=0" "@max=150");
    addDataNode(&SUMMARY_STARTMS);
    SUMMARY_STOPMS.makeSetup("@variable=SUMMARY_STOPMS" "@value=50" "@min=0" "@max=150");
    addDataNode(&SUMMARY_STOPMS);
    DETAIL_STARTMS.makeSetup("@variable=DETAIL_STARTMS" "@value=0" "@min=0" "@max=150");
    addDataNode(&DETAIL_STARTMS);
    DETAIL_STOPMS.makeSetup("@variable=DETAIL_STOPMS" "@value=50" "@min=0" "@max=150");
    addDataNode(&DETAIL_STOPMS);
    OVERVIEW_STARTMS.makeSetup("@variable=OVERVIEW_STARTMS" "@value=0" "@min=0" "@max=150");
    addDataNode(&OVERVIEW_STARTMS);
    OVERVIEW_STOPMS.makeSetup("@variable=OVERVIEW_STOPMS" "@value=50" "@min=0" "@max=150");
    addDataNode(&OVERVIEW_STOPMS);

    HISTORY_INDEX.makeSetup("@variable=HISTORY_INDEX" "@value=1" "@min=1" "@max=1000");
    addDataNode(&HISTORY_INDEX);

    NCYCLES.makeSetup("@variable=NCYCLES" "@value=1" "@min=1" "@max=1000");
    addDataNode(&NCYCLES);

    UPDATE_MASTER.makeSetup("@variable=UPDATE_MASTER" "@value=1" "@min=0" "@max=1");
    addDataNode(&UPDATE_MASTER);

    MEASPERIODMS.makeSetup("@variable=MEASPERIODMS" "@value=200" "@min=0" "@max=9999");
    addDataNode(&MEASPERIODMS);

    PDCMDATACOUNT.makeSetup("@variable=PDCMDATACOUNT" "@value=0" "@min=0" "@max=100000");
    addDataNode(&PDCMDATACOUNT);

    CFAR_SW_ENA.makeSetup("@variable=CFAR_SW_ENA" "@value=0" "@min=0" "@max=1");
    addDataNode(&CFAR_SW_ENA);
    CFAR_THR_OFF.makeSetup("@variable=CFAR_THR_OFF" "@value=30" "@min=1" "@max=255");
    addDataNode(&CFAR_THR_OFF);
    CFAR_SCALE.makeSetup("@variable=CFAR_SCALE" "@value=3" "@min=0" "@max=7");
    addDataNode(&CFAR_SCALE);
    CFAR_DER_THR.makeSetup("@variable=CFAR_DER_THR" "@value=8" "@min=1" "@max=63");
    addDataNode(&CFAR_DER_THR);
    CFAR_DER_SIZE.makeSetup("@variable=CFAR_DER_SIZE" "@value=3" "@min=0" "@max=3");
    addDataNode(&CFAR_DER_SIZE);
    CFAR_SNR.makeSetup("@variable=CFAR_SNR" "@value=12" "@min=1" "@max=63");
    addDataNode(&CFAR_SNR);
    CFAR_STARTMS.makeSetup("@variable=CFAR_STARTMS" "@value=0" "@min=0" "@max=99");
    addDataNode(&CFAR_STARTMS);
    MP_DET_THR_POS.makeSetup("@variable=MP_DET_THR_POS" "@value=20" "@min=0" "@max=255");
    addDataNode(&MP_DET_THR_POS);
    MP_DET_THR_NEG.makeSetup("@variable=MP_DET_THR_NEG" "@value=20" "@min=0" "@max=255");
    addDataNode(&MP_DET_THR_NEG);

    VarPHASE_DIFF_THR.makeSetup("@variable=VarPHASE_DIFF_THR" "@value=100" "@min=0" "@max=1023");
    addDataNode(&VarPHASE_DIFF_THR);
    VarPHASE_DIFF_12_ENA.makeSetup("@variable=VarPHASE_DIFF_12_ENA" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarPHASE_DIFF_12_ENA);
    VarPHASE_DIFF_34_ENA.makeSetup("@variable=VarPHASE_DIFF_34_ENA" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarPHASE_DIFF_34_ENA);
    VarPHASE_DIFF_12_INVERT.makeSetup("@variable=VarPHASE_DIFF_12_INVERT" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarPHASE_DIFF_12_INVERT);
    VarPHASE_DIFF_34_INVERT.makeSetup("@variable=VarPHASE_DIFF_34_INVERT" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarPHASE_DIFF_34_INVERT);

    APPLY_CFAR.makeSetup(ENA_1_8_INIT);
    APPLY_CFAR.setName("APPLY_CFAR");
    addDataNode(&APPLY_CFAR);

    TOF_OFFSET.makeSetup("@variable=TOF_OFFSET" "@value=0" "@min=0" "@max=10.1");
    addDataNode(&TOF_OFFSET);

    const int init_array[11] = {0x40, 0x42, 0x4A, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5A, 0x5C, 0x5E};
    const int init_mode_array[6] = {0x40, 0x42, 0x4A, 0x54, 0x56, 0x58};
    const int init_mode_addr_array[5] = {0x52, 0x74, 0x96, 0xB8, 0xDA};

    //CROSS REFERENCE
    addDataNode(&gp_DSI3Slave->SWriteMEASURE);
    addDataNode(&gp_DSI3Slave->SWriteMEASUREadd);
    addDataNode(&gp_DSI3Slave->piezo_model_shift);

    addDataNode(&gp_UartSsiController->SSI_XOR);

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 11; j++)
        {
            addDataNode(gp_DSI3Slave->SREGX[i].at(gp_DSI3Slave->m_map_sregs[i][0][init_array[j]]));
        }
    }


    /*addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[0][0x40]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[0][0x42]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[0][0x50]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[0][0x52]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[0][0x54]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[0][0x56]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[0][0x58]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[0][0x5A]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[0][0x5C]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[0][0x5E]));*/
    addDataNode(&gp_DSI3Master->MVar_version);

    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < 6; j++)
        {
            for (k = 0; k < 5; k++)
            {
                 addDataNode(gp_DSI3Slave->SREGX[i].at(gp_DSI3Slave->m_map_sregs[i][1][init_mode_array[j] - 0x40 + init_mode_addr_array[k]]));
            }
        }
    }
    /*
    //MODE1
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x40 - 0x40 + 0x52]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x42 - 0x40 + 0x52]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x54 - 0x40 + 0x52]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x56 - 0x40 + 0x52]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x58 - 0x40 + 0x52]));

    //MODE2
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x40 - 0x40 + 0x74]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x42 - 0x40 + 0x74]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x54 - 0x40 + 0x74]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x56 - 0x40 + 0x74]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x58 - 0x40 + 0x74]));

    //MODE3
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x40 - 0x40 + 0x96]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x42 - 0x40 + 0x96]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x54 - 0x40 + 0x96]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x56 - 0x40 + 0x96]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x58 - 0x40 + 0x96]));

    //MODE4
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x40 - 0x40 + 0xB8]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x42 - 0x40 + 0xB8]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x54 - 0x40 + 0xB8]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x56 - 0x40 + 0xB8]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x58 - 0x40 + 0xB8]));

    //MODE5
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x40 - 0x40 + 0xDA]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x42 - 0x40 + 0xDA]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x54 - 0x40 + 0xDA]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x56 - 0x40 + 0xDA]));
    addDataNode(gp_DSI3Slave->SREGX[0].at(gp_DSI3Slave->m_map_sregs[1][0x58 - 0x40 + 0xDA]));

*/
    addDataNode(&gp_DSI3Master->MREG02);
    addDataNode(&gp_DSI3Master->MREG03);
    addDataNode(&gp_DSI3Master->MREG10);
    addDataNode(&gp_DSI3Master->CHANNEL);

    MEAStimer = new QTimer();
    MEAStimer->setInterval(200);
    connect(MEAStimer,SIGNAL(timeout()),this,SLOT(slot_MEASTimer_timeout()));
    connect(ui->checkBoxEnableHistory, SIGNAL(clicked(bool)), this, SLOT(slot_EnableHistoryUpdate()));
    connect(ui->checkBoxPersistence, SIGNAL(clicked(bool)), this, SLOT(slot_PersistenceUpdate()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slot_TWUpdate()));
    connect(ui->cb_updatetableSamples, SIGNAL(clicked()), this, SLOT(slot_updateTable()));
    connect(ui->cb_updatetableTof, SIGNAL(clicked()), this, SLOT(slot_updateTable()));

    for (int i = 0; i < MAX_SLOT_NUMBER; i++)
    {
        m_diagword0[i].clear();
        m_diagword1[i].clear();
        m_diagword2[i].clear();

        for(int j = 0; j < MAX_BRC_NUMBER; j++)
        {
            m_sensorstatus[j][i] = 0x3F;
            m_kac[j][i] = -1;
            m_pdcm_crc[j][i] = -1;
        }
    }
    ui->tabWidget->setTabEnabled(4, false);     //4D ULS
    ui->tabWidget->setTabText(4,"");
    ui->tabWidget->setTabEnabled(5, false);     //IQ data
    ui->tabWidget->setTabText(5,"");

    CheckVersionConfig();

    slot_EnableHistoryUpdate();
    return true;
}

void Measurement::createCustomUi()
{
    GRAPH_DETAIL.clear();
    GRAPH_DETAIL.append(ui->ptDetail1);
    GRAPH_DETAIL.append(ui->ptDetail2);

    TABLE_DETAIL.clear();
    TABLE_DETAIL.append(ui->twDetail1);
    TABLE_DETAIL.append(ui->twDetail2);

    GRAPH_OVERVIEW.clear();
    GRAPH_OVERVIEW.append(ui->ptOverview1);
    GRAPH_OVERVIEW.append(ui->ptOverview2);
    GRAPH_OVERVIEW.append(ui->ptOverview3);
    GRAPH_OVERVIEW.append(ui->ptOverview4);
    GRAPH_OVERVIEW.append(ui->ptOverview5);
    GRAPH_OVERVIEW.append(ui->ptOverview6);
    GRAPH_OVERVIEW.append(ui->ptOverview7);
    GRAPH_OVERVIEW.append(ui->ptOverview8);

    LABEL_REVFREQ.clear();
    LABEL_REVFREQ.append(ui->label_RevFreq1);
    LABEL_REVFREQ.append(ui->label_RevFreq2);
    LABEL_REVFREQ.append(ui->label_RevFreq3);
    LABEL_REVFREQ.append(ui->label_RevFreq4);
    LABEL_REVFREQ.append(ui->label_RevFreq5);
    LABEL_REVFREQ.append(ui->label_RevFreq6);
    LABEL_REVFREQ.append(ui->label_RevFreq7);
    LABEL_REVFREQ.append(ui->label_RevFreq8);
    LABEL_REVFREQ.append(ui->label_RevFreq9);
    LABEL_REVFREQ.append(ui->label_RevFreq10);
    LABEL_REVFREQ.append(ui->label_RevFreq11);
    LABEL_REVFREQ.append(ui->label_RevFreq12);

    LABEL_REVDUR.clear();
    LABEL_REVDUR.append(ui->label_RevDur1);
    LABEL_REVDUR.append(ui->label_RevDur2);
    LABEL_REVDUR.append(ui->label_RevDur3);
    LABEL_REVDUR.append(ui->label_RevDur4);
    LABEL_REVDUR.append(ui->label_RevDur5);
    LABEL_REVDUR.append(ui->label_RevDur6);
    LABEL_REVDUR.append(ui->label_RevDur7);
    LABEL_REVDUR.append(ui->label_RevDur8);
    LABEL_REVDUR.append(ui->label_RevDur9);
    LABEL_REVDUR.append(ui->label_RevDur10);
    LABEL_REVDUR.append(ui->label_RevDur11);
    LABEL_REVDUR.append(ui->label_RevDur12);

    LABEL_REVFREQ_2.clear();
    LABEL_REVFREQ_2.append(ui->label_RevFreq1_2);
    LABEL_REVFREQ_2.append(ui->label_RevFreq2_2);
    LABEL_REVFREQ_2.append(ui->label_RevFreq3_2);
    LABEL_REVFREQ_2.append(ui->label_RevFreq4_2);
    LABEL_REVFREQ_2.append(ui->label_RevFreq5_2);
    LABEL_REVFREQ_2.append(ui->label_RevFreq6_2);
    LABEL_REVFREQ_2.append(ui->label_RevFreq7_2);
    LABEL_REVFREQ_2.append(ui->label_RevFreq8_2);
    LABEL_REVFREQ_2.append(ui->label_RevFreq9_2);
    LABEL_REVFREQ_2.append(ui->label_RevFreq10_2);
    LABEL_REVFREQ_2.append(ui->label_RevFreq11);
    LABEL_REVFREQ_2.append(ui->label_RevFreq12);

    LABEL_REVDUR_2.clear();
    LABEL_REVDUR_2.append(ui->label_RevDur1_2);
    LABEL_REVDUR_2.append(ui->label_RevDur2_2);
    LABEL_REVDUR_2.append(ui->label_RevDur3_2);
    LABEL_REVDUR_2.append(ui->label_RevDur4_2);
    LABEL_REVDUR_2.append(ui->label_RevDur5_2);
    LABEL_REVDUR_2.append(ui->label_RevDur6_2);
    LABEL_REVDUR_2.append(ui->label_RevDur7_2);
    LABEL_REVDUR_2.append(ui->label_RevDur8_2);
    LABEL_REVDUR_2.append(ui->label_RevDur9_2);
    LABEL_REVDUR_2.append(ui->label_RevDur10_2);
    LABEL_REVDUR_2.append(ui->label_RevDur11);
    LABEL_REVDUR_2.append(ui->label_RevDur12);

    ui->tabWidget->setCurrentIndex(0);

    InitGraphsAndTables();
    //UpdateGraphAndTable();
    //connect(ui->lineEditWriteData,SIGNAL(textEdited(QString)),this,SLOT(slot_WriteData_changed()));
    //connect(ui->lineEditWriteData,SIGNAL(textChanged(QString)),this,SLOT(slot_WriteData_changed()));

    ui->labelShortRangeData->setVisible(false);
    ui->ptShortRange->setVisible(false);

    connect(this, SIGNAL(signal_RegisterChanged(QString)), this, SLOT(slot_UpdateALL()));

    ui->labelCRCerror->setVisible(false);
    ui->labelCRCerror_2->setVisible(false);
    ui->labelCRCerror_3->setVisible(false);
    ui->labelCRCerror_4->setVisible(false);
    ui->labelearlylateerror->setVisible(false);
    ui->labelearlylateerror_2->setVisible(false);
    ui->labelearlylateerror_3->setVisible(false);
    ui->labelearlylateerror_4->setVisible(false);


}


/****************************************************************************
 *                              FUNCTIONALITY
*****************************************************************************/


// updates all RELAYS and GPIOs
bool Measurement::updateHardware(Direction direction, const QString &item)
{
    return false;
}

QVariant Measurement::FUNCTION_LIST()
{        
        FUNCTION("Update", "This function updates hardware"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            return updateHardware(HARDWARE_GET,"*");
        }

        FUNCTION("Measure", "This function performs one measurement"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Single MEASURE");
            return Measure(MEASURE_SINGLE);
        }

        FUNCTION("StartMeasurement", "This function starts continuous measurement"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Start Continuous Measurement");
            return StartContinuousMeasurement();
        }

        FUNCTION("StopMeasurement", "This function stops continuous measurement"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Stop measurement");
            return StopMeasurement();
        }

        FUNCTION("PerformNMeasurement", "This function performs N measurements"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Perform N cycles of measurement");
            return PerformNCycles();
        }

        FUNCTION("SaveGraph", "This function saves graph as PNG file"
                 ,"<i><br> parameter - graph name and path (optional), delimited by one space, example: Detail5"
                 "<i><br> example2: Overview2 c:/Users/Public/graph.png"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Save Graph");
            return SaveGraph(p1.toString());
        }

        FUNCTION("CopyGraph", "This function copys graph into clipboard"
                 ,"<i><br> parameter - graph name, example: Detail5"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Copy graph into clipboard");
            return CopyGraph(p1.toString());
        }

        FUNCTION("SaveCSV", "Save data of one graph to CSV"
                 ,"<i><br> parameter - graph name, example: Detail5"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Save graph into CSV");
            return SaveCSV(p1.toString());
        }

        FUNCTION("ShowResultsOfIndex", "This function shows results from history with defined index"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Show results of Index");
            return ShowResultsOfIndex();
        }

        FUNCTION("ClearHistory", "This function clears history memory"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Clear History");
            ClearHistory();
            return true;
        }

        FUNCTION("SaveHistory", "This function saves history into CSV"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Save History");
            SaveHistory();
            return true;
        }

        FUNCTION("Decode", "temporary"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Decode button - temporary");
            ui->labelCRCerror->setVisible(false);
            ui->labelCRCerror_2->setVisible(false);
            ui->labelCRCerror_3->setVisible(false);
            ui->labelCRCerror_4->setVisible(false);
            ui->labelearlylateerror->setVisible(false);
            ui->labelearlylateerror_2->setVisible(false);
            ui->labelearlylateerror_3->setVisible(false);
            ui->labelearlylateerror_4->setVisible(false);
            MEASUREdecode();
            UpdateMasterSettings();
            DecodeData();

            ShowPER();
            ShowData();
            return true;
        }

        FUNCTION("WriteModeRegs", "Write Mode registers into DSI3PAS slave"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "Measurement - Write Mode regs");
            gp_DSI3Slave->WriteSlaveRegs("Mode");
            return true;
        }

        FUNCTION("getSample", "Get one sample from measurement data"
                 ,"<i><br> two parameters, first parameter slot (start from 1) or time when 0, second parameter index (start from 0)"
                 ,"RETURN int sample value, -1 when slot and parameter are out of range")
        {
            int res = -1;
            int slot = p1.toInt();
            int index = p2.toInt();

            if (slot == 0)
            {
                if (m_meas_time.size() > index)
                {
                    res = 1000*m_meas_time.at(index);
                }
            }
            else if ((slot >= 1) && (slot <= 10))
            {
                if (m_meas_samples[slot-1].size() > index)
                {
                    res = m_meas_samples[slot-1].at(index);
                }
            }
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, QString("Function getSample, slot=%1, index=%2").arg(slot).arg(index));
            return res;
        }

        FUNCTION("getSamples", "Get all measurement data"
                 ,"<i><br> no parameter"
                 ,"RETURN string with measurement data")
        {
            int res = -1;
            int slot;
            int i;
            QString response;

            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                response.append(QString("_%1:[").arg(m_meas_slots.at(slot)));

                for(i = 0; i < m_meas_samples[slot].size(); i++)
                {
                    response.append(QString("%1,").arg(m_meas_samples[slot].at(i)));
                }

                response.append(QString("]"));
            }

            gp_DSI3Master->Log(LOG_TYPE_BUTTON, QString("Function getSamples"));
            return response;
        }

        FUNCTION("getPDCM_CRC", "Get if CRC of specified PDCM data was correct"
                 ,"<i><br> two parameters, first parameter brc (start from 1), second parameter slot (start from 1)"
                 ,"RETURN int value, 1 when CRC correct, 0 when CRC not correct, -1 when inputs out of range")
        {

            int res = -1;
            int brc = p1.toInt();
            int slot = p2.toInt();

            if ((brc >= 1) && (brc <= MAX_BRC_NUMBER))
            {
                if ((slot >= 1) && (slot <= MAX_SLOT_NUMBER))
                {
                    res = m_pdcm_crc[brc-1][slot-1];
                }
            }

            gp_DSI3Master->Log(LOG_TYPE_BUTTON, QString("Function getPDCM_CRC, brc=%1, slot=%2").arg(brc).arg(slot));
            return res;
        }

        FUNCTION("getSensorStatus", "Get sensor status of selected slot"
                 ,"<i><br> two parameters, first parameter brc (start from 1), second parameter slot (start from 1)"
                 ,"RETURN int sensor status value or -1 when inputs out of range")
        {
            int res = -1;
            int brc = p1.toInt();
            int slot = p2.toInt();

            if ((brc >= 1) && (brc <= MAX_BRC_NUMBER))
            {
                if ((slot >= 1) && (slot <= MAX_SLOT_NUMBER))
                {
                    if (m_sensorstatus[brc-1][slot-1] == 0x3F)
                    {
                        res = -1;
                    }
                    else
                    {
                        res = m_sensorstatus[brc-1][slot-1] & 0x1F;
                    }
                }
            }
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, QString("Function getSensorStatus, brc=%1, slot=%2").arg(brc).arg(slot));
            return res;
        }

        FUNCTION("getDiagWord0", "Get Diagnostic word0 for specified slot "
                 ,"<i><br> parameter slot (start from 1)"
                 ,"RETURN int diagword0 value or -1 when input out of range")
        {
            int res = -1;
            int slot = p1.toInt();

            if ((slot >= 1) && (slot <= MAX_SLOT_NUMBER))
            {
                if (m_diagword0[slot-1].length() < 2)
                {
                     res = -1;
                }
                else
                {
                    res = (m_diagword0[slot-1].at(1) + (m_diagword0[slot-1].at(0) << 8)) & 0xFFFF;
                }
            }
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, QString("Function getDiagWord0, slot=%1").arg(slot));
            return res;
        }

        FUNCTION("getDiagWord1", "Get Diagnostic word1 for specified slot "
                 ,"<i><br> parameter slot (start from 1)"
                 ,"RETURN int diagword1 value or -1 when input out of range")
        {
            int res = -1;
            int slot = p1.toInt();

            if ((slot >= 1) && (slot <= MAX_SLOT_NUMBER))
            {
                if (m_diagword1[slot-1].length() < 2)
                {
                     res = -1;
                }
                else
                {
                    res = (m_diagword1[slot-1].at(1) + (m_diagword1[slot-1].at(0) << 8)) & 0xFFFF;
                }
            }
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, QString("Function getDiagWord1, slot=%1").arg(slot));
            return res;
        }

        FUNCTION("getDiagWord2", "Get Diagnostic word2 for specified slot "
                 ,"<i><br> parameter slot (start from 1)"
                 ,"RETURN int diagword2 value or -1 when input out of range")
        {
            int res = -1;
            int slot = p1.toInt();

            if ((slot >= 1) && (slot <= MAX_SLOT_NUMBER))
            {
                if (m_diagword2[slot-1].length() < 2)
                {
                     res = -1;
                }
                else
                {
                    res = (m_diagword2[slot-1].at(1) + (m_diagword2[slot-1].at(0) << 8)) & 0xFFFF;
                }
            }
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, QString("Function getDiagWord2, slot=%1").arg(slot));
            return res;
        }

        FUNCTION("getKAC", "Get KAC value for selected slot"
                 ,"<i><br> two parameters, first parameter brc (start from 1), second parameter slot (start from 1)"
                 ,"RETURN int KAC value 0 - 7 , or -1 when inputs out of range")
        {

            int res = -1;
            int brc = p1.toInt();
            int slot = p2.toInt();

            if ((brc >= 1) && (brc <= MAX_BRC_NUMBER))
            {
                if ((slot >= 1) && (slot <= MAX_SLOT_NUMBER))
                {
                    res = m_kac[brc-1][slot-1];
                }
            }

            gp_DSI3Master->Log(LOG_TYPE_BUTTON, QString("Function getPDCM_CRC, brc=%1, slot=%2").arg(brc).arg(slot));
            return res;
        }

        FUNCTION("ResetPER", "Reset PER"
                 ,"<i><br> no parameter"
                 ,"no return")
        {
            m_packets = 0;
            m_PERerrors = 0;
            ShowPER();
            return true;
        }

    return false;
}


void Measurement::setupBoard()
{
}
void Measurement::suspend()
{       
}
void Measurement::resume()
{
    setupBoard();
}

bool Measurement::Measure(int type)
{
    bool result = false;
    int scheduler_next = 0;
    int scheduler_current = 0;
    int scheduler_ena = 0;
    int i;
    static QTime time;
    int time_diff;

    if (type == MEASURE_LAST)
    {
        if((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
        {
            if (ui->comboBoxMeasureSource->currentIndex() == 0)
            {
                gp_DSI3Slave->PrepareMEASUREUPID();
                gp_UartSsiController->SSI_CHANNEL.setValue(gp_DSI3Master->CHANNEL.value());
                gp_UartSsiController->SSI_MEAS.setValue(0);
                gp_UartSsiController->SSI_READ.setValue(1);
                gp_UartSsiController->runAppCommand("PerformMeasurement", "","","","","");
            }
        }
        else
        {
            result = gp_DSI3Slave->LastMEASURE();
        }

        return true;
    }
    else if ((type == MEASURE_SINGLE) && (MEAStimer->isActive() == true))
    {
        gp_DSI3Master->Log(LOG_TYPE_INFO, "Don't use Single measure when using Continuous measurement");
        return false;
    }

    //gp_DSI3Master->Log(LOG_TYPE_TIME, "Start of Measure function");

    ui->labelCRCerror->setVisible(false);
    ui->labelCRCerror_2->setVisible(false);
    ui->labelCRCerror_3->setVisible(false);
    ui->labelCRCerror_4->setVisible(false);
    ui->labelearlylateerror->setVisible(false);
    ui->labelearlylateerror_2->setVisible(false);
    ui->labelearlylateerror_3->setVisible(false);
    ui->labelearlylateerror_4->setVisible(false);

    for (i = 0; i < MAX_SLOT_NUMBER; i++)
    {
        m_hide_slot[i] = false;
    }

    if (ui->comboBoxMeasureSource->currentIndex() == 0) //measure CMD
    {
        result = true;
    }
    else    // SCHEDULER
    {
        //find next behavior and copy MEASURE cmd
        scheduler_current = ui->lineEditCurrentBehavior->text().toInt();
        scheduler_ena = SCHEDULER_ENA.toUInt();
        for (i = scheduler_current; i < 10; i++)
        {
            if (((scheduler_ena >> i) & 0x01) == 0x01)
            {
                scheduler_next = i + 1;
                break;
            }
        }
        if (scheduler_next == 0)
        {
            for (i = 0; i < scheduler_current + 1; i++)
            {
                if (((scheduler_ena >> i) & 0x01) == 0x01)
                {
                    scheduler_next = i + 1;
                    break;
                }
            }
        }

        if (scheduler_next == 0)
        {
            scheduler_current = 0;
            result = false;
        }
        else
        {
            scheduler_current = scheduler_next;
            gp_DSI3Slave->SWriteMEASURE.fromByteArray(SCHEDULER_MEASURE[scheduler_next-1].toByteArray());
            result = true;
        }
        ui->lineEditCurrentBehavior->setText(QString("%1").arg(scheduler_current));
    }
    if (result)
    {
        //gp_DSI3Master->Log(LOG_TYPE_TIME, "Start of MEASUREdecode");

        MEASUREdecode();

        //gp_DSI3Master->Log(LOG_TYPE_TIME, "End of MEASUREdecode, start of UpdateMasterSettings");

        UpdateMasterSettings();

        //gp_DSI3Master->Log(LOG_TYPE_TIME, "End of UpdateMasterSettings");

        if (m_first_measurement[((int)gp_DSI3Master->CHANNEL.value())&0x01])
        {
           //gp_DSI3Slave->PerformWRITE_RWP(false, 15, 3, 3, 0, 0, 0);
           gp_DSI3Slave->PerformWRITE_RWP(false, 15, 3, 2, 0, 0, 0);

           m_first_measurement[((int)gp_DSI3Master->CHANNEL.value())&0x01] = false;
        }

        //gp_DSI3Master->Log(LOG_TYPE_TIME, "Start of PerformMEASURE");

        if (type == MEASURE_SINGLE)
        {
            if(gp_Settings->mVersion == VERSION_UPID)
            {
                gp_DSI3Slave->PrepareMEASUREUPID();
                gp_UartSpiController->BRC_CHANNEL.setValue(gp_DSI3Master->CHANNEL.value());
                gp_UartSpiController->runAppCommand("PerformBRC", "","","","","");
            }
            else if((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
            {
                gp_DSI3Slave->PrepareMEASUREUPID();
                gp_UartSsiController->SSI_CHANNEL.setValue(gp_DSI3Master->CHANNEL.value());

                gp_UartSsiController->SSI_MEAS.setValue(1);
                gp_UartSsiController->SSI_READ.setValue(0);
                gp_UartSsiController->runAppCommand("PerformMeasurement", "","","","","");

                gp_UartSsiController->SSI_MEAS.setValue(0);
                gp_UartSsiController->SSI_READ.setValue(1);
                gp_UartSsiController->runAppCommand("PerformMeasurement", "","","","","");
            }
            else
            {
                ui->lineEditPeriod->setText("-");
                result = gp_DSI3Slave->PerformMEASURE(true);
            }

        }
        else if (type == MEASURE_CONTI)
        {
            if ((m_counter%10) == 0)
            {
                time = QTime::currentTime();
            }
            if ((m_counter%10) == 9)
            {
                time_diff=time.msecsTo(QTime::currentTime());
                ui->lineEditPeriod->setText(QString("%1").arg(time_diff / 9));
            }
            m_counter++;

            if(gp_Settings->mVersion == VERSION_UPID)
            {
                gp_DSI3Slave->PrepareMEASUREUPID();
                gp_UartSpiController->BRC_CHANNEL.setValue(gp_DSI3Master->CHANNEL.value());
                gp_UartSpiController->runAppCommand("PerformBRC", "","","","","");
            }
            else if((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
            {
                if (ui->comboBoxMeasureSource->currentIndex() == 0)
                {
                    gp_UartSsiController->SSI_MEAS.setValue(1);
                    gp_UartSsiController->SSI_READ.setValue(1);
                    gp_UartSsiController->runAppCommand("PerformMeasurement", "","","","","");
                }
                else
                {
                    gp_DSI3Slave->PrepareMEASUREUPID();
                    gp_UartSsiController->SSI_CHANNEL.setValue(gp_DSI3Master->CHANNEL.value());

                    gp_UartSsiController->SSI_MEAS.setValue(1);
                    gp_UartSsiController->SSI_READ.setValue(0);
                    gp_UartSsiController->runAppCommand("PerformMeasurement", "","","","","");

                    gp_UartSsiController->SSI_MEAS.setValue(0);
                    gp_UartSsiController->SSI_READ.setValue(1);
                    gp_UartSsiController->runAppCommand("PerformMeasurement", "","","","","");
                }
            }
            else
            {
                result = gp_DSI3Slave->ContiMEASURE();
            }

        }
        else if (type == MEASURE_FIRST)
        {
            result = gp_DSI3Slave->FirstMEASURE();

            if((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
            {
                if (ui->comboBoxMeasureSource->currentIndex() == 0)
                {
                    gp_DSI3Slave->PrepareMEASUREUPID();
                    gp_UartSsiController->SSI_CHANNEL.setValue(gp_DSI3Master->CHANNEL.value());
                    gp_UartSsiController->SSI_MEAS.setValue(1);
                    gp_UartSsiController->SSI_READ.setValue(0);
                    gp_UartSsiController->runAppCommand("PerformMeasurement", "","","","","");
                }
                else        // SCHEDULER
                {
                    gp_DSI3Slave->PrepareMEASUREUPID();
                    gp_UartSsiController->SSI_CHANNEL.setValue(gp_DSI3Master->CHANNEL.value());

                    gp_UartSsiController->SSI_MEAS.setValue(1);
                    gp_UartSsiController->SSI_READ.setValue(0);
                    gp_UartSsiController->runAppCommand("PerformMeasurement", "","","","","");

                    gp_UartSsiController->SSI_MEAS.setValue(0);
                    gp_UartSsiController->SSI_READ.setValue(1);
                    gp_UartSsiController->runAppCommand("PerformMeasurement", "","","","","");

                }



            }
            return true;
        }

        //gp_DSI3Master->Log(LOG_TYPE_TIME, "End of PerformMEASURE, Start of DecodeData");

        //if (result)
        DecodeData();

        //gp_DSI3Master->Log(LOG_TYPE_TIME, "End of DecodeData, Start of CFARinGUI");

        ShowPER();

        CFARinGUI();

        //gp_DSI3Master->Log(LOG_TYPE_TIME, "End of CFARinGUI, Start of CalculateTOF");

        CalculateTOF();//ToFinGUI();

        CalculatePHASE_DIFF();


        //gp_DSI3Master->Log(LOG_TYPE_TIME, "End of CalculateTOF, Start of AddToHistory");

        AddToHistory();

        //gp_DSI3Master->Log(LOG_TYPE_TIME, "End of AddToHistory, Start of ShowData");

        ShowData();

        //gp_DSI3Master->Log(LOG_TYPE_TIME, "End of ShowData");

    }
    else
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "SCHEDULER is empty");
    }

    return result;
}

bool Measurement::DecodeData(void)
{
    QByteArray slot_data[MAX_BRC_NUMBER][MAX_SLOT_NUMBER];
    QByteArray crc[MAX_BRC_NUMBER][MAX_SLOT_NUMBER];
    QByteArray compressed_data[MAX_SLOT_NUMBER];
    int length;
    QString num;
    QString txt;

    uint meas_type;
    uint comp_method;
    uint meas_duration;
    uint compr_timer;
    uint tx_per_l;
    uint tx_per_h;
    uint delta_gain_val3;
    uint delta_gain_val4;
    uint delta_gain_tim4;

    for (int i = 0; i < MAX_SLOT_NUMBER; i++)
    {
        m_diagword0[i].clear();
        m_diagword1[i].clear();
        m_diagword2[i].clear();

        for(int j = 0; j < MAX_BRC_NUMBER; j++)
        {
            m_sensorstatus[j][i] = 0x3F;
            m_pdcm_crc[j][i] = -1;
            m_kac[j][i] = -1;
        }
    }

    bool channel_low;

    int crc_length = 1;
    int slot_count = m_meas_slot_number;
    int brc_count = 0;

    int slot_duration = 0;
    if ((gp_Settings->mVersion == VERSION_UPID) || (gp_Settings->mVersion == VERSION_RDUM)|| (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
    {
        slot_duration = 2;
    }

    int crc_over_slot_dur = 0;
    if ((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
    {
        crc_over_slot_dur = 1;
    }

    int slot_data_length = 16;
    int slot_length = 1 + slot_data_length + crc_length + slot_duration;

    int DIAGWORD1_pos = 0;

    int brc, slot;


    if (gp_Settings->mVersion == VERSION_UPID)
    {
        brc_count = gp_UartSpiController->BRC_BRC.value();
    }
    else if ((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
    {
        brc_count = gp_UartSsiController->SSI_BRC_COUNT.value();
    }
    else
    {
        if ((int)gp_DSI3Master->CHANNEL.value() == 0)
        {
            brc_count = (int)gp_DSI3Master->MREG02["BRC_CNT"];
        }
        else
        {
            brc_count = (int)gp_DSI3Master->MREG03["BRC_CNT"];
        }
    }

    QByteArray uartdata = QByteArray::fromHex(ui->lineEditBRCReadData->text().toLatin1());
    ui->textEditSensorError->clear();


    if (GET_REG("SWriteMEASURE[mode]").toInt() == 0)
    {
        meas_type = gp_DSI3Slave->Getmeas_type_old(0);
        comp_method = gp_DSI3Slave->Getcomp_method_old(0);

        if (gp_DSI3Slave->m_memory == 0)
        {
            meas_duration = GET_REG("SREGX058[meas_duration]").toUInt();
            compr_timer = 0;
        }
        else if (gp_DSI3Slave->m_memory == 1)
        {
            if ((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
            {
                meas_duration = GET_REG("SREGXV204A[7:0]").toUInt();
            }
            else
            {
                meas_duration = GET_REG("SREGXV204A[meas_duration]").toUInt();
            }

            compr_timer = GET_REG("SREGXV205E[8:6]").toUInt();
        }

        if ((gp_DSI3Master->MVar_version == VERSION_MOBEJUN) || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH)\
                 || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
        {
            tx_per_h = 256 + GetSlaveReg(gp_DSI3Slave->m_memory,0, 0x42, "[8:0]") + (GetSlaveReg(gp_DSI3Slave->m_memory,0, 0x40, "[15]") << 9);
        }
        else
        {
            tx_per_h = GetSlaveReg(gp_DSI3Slave->m_memory,0, 0x42, "[tx_per_h]");
        }

        tx_per_l = GetSlaveReg(gp_DSI3Slave->m_memory,0, 0x40, "[tx_per_l]");
        delta_gain_val3 = GetSlaveReg(gp_DSI3Slave->m_memory,0, 0x54, "[delta_gain_val3]");
        delta_gain_val4 = GetSlaveReg(gp_DSI3Slave->m_memory,0, 0x56, "[delta_gain_val4]");
        delta_gain_tim4 = GetSlaveReg(gp_DSI3Slave->m_memory,0, 0x56, "[delta_gain_tim4]");

    }
    else
    {
        if (GET_REG("SWriteMEASURE[mode]").toInt() > 5)
        {
            SET_REG("SWriteMEASURE[mode]", 5);
        }

        num = QString("%1").arg(0x42 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()],2,16,QChar('0')).toUpper();
        meas_type = gp_DSI3Slave->Getmeas_type_old(GET_REG("SWriteMEASURE[mode]").toInt());

        if (gp_DSI3Slave->m_memory == 0)
        {
            meas_duration = GetSlaveReg(0, 1, 0x58 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[meas_duration]");
            compr_timer = 0;
        }
        else if (gp_DSI3Slave->m_memory == 1)
        {
            if ((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
            {
                meas_duration = GetSlaveReg(1, 1, 0x4A - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[7:0]");
            }
            else
            {
                meas_duration = GetSlaveReg(1, 1, 0x4A - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[meas_duration]");
            }

            compr_timer = GetSlaveReg(1, 1, 0x5E - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[8:6]");
        }

        if ((gp_DSI3Master->MVar_version == VERSION_MOBEJUN) || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH) \
                 || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
        {
            tx_per_h = 256 + GetSlaveReg(gp_DSI3Slave->m_memory, 1, 0x42 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[8:0]") +\
                    (GetSlaveReg(gp_DSI3Slave->m_memory, 1, 0x40 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[15]") << 9);
        }
        else
        {
            tx_per_h = GetSlaveReg(gp_DSI3Slave->m_memory, 1, 0x42 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[tx_per_h]");
        }



        tx_per_l = GetSlaveReg(gp_DSI3Slave->m_memory, 1, 0x40 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[tx_per_l]");
        tx_per_h = GetSlaveReg(gp_DSI3Slave->m_memory, 1, 0x42 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[tx_per_h]");
        delta_gain_val3 = GetSlaveReg(gp_DSI3Slave->m_memory, 1, 0x54 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[delta_gain_val3]");
        delta_gain_val4 = GetSlaveReg(gp_DSI3Slave->m_memory, 1, 0x56 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[delta_gain_val4]");
        delta_gain_tim4 = GetSlaveReg(gp_DSI3Slave->m_memory, 1, 0x56 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[delta_gain_tim4]");
        comp_method = gp_DSI3Slave->Getcomp_method_old(GET_REG("SWriteMEASURE[mode]").toInt());
    }

    int phase_shift = (int)gp_DSI3Slave->piezo_model_shift.value();

    float centerfreq;
    const int bw12 = 10071;
    int freq = 0;
    int i;

    if (tx_per_h != 0)
    {
        centerfreq = 1 / ((tx_per_h)*0.000000025);
    }

    if (uartdata.length() != (1 + brc_count*(slot_count*slot_length + crc_over_slot_dur)))
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "UART BRC RX DATA decoding failed - received data length differs from settings");
        return false;
    }

    uartdata.remove(0, 1);
    ui->textEdit_measdata->clear();
    for (brc = 0; brc < brc_count; brc++)
    {
        for (slot = 0; slot < slot_count; slot++)
        {
            if ((gp_UartSsiController->SSI_XOR.value() == 1) && ((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX)\
                                                                  || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX)))
            {
                slot_data[brc][slot] = Descramble(uartdata.mid(brc * (slot_count * slot_length + crc_over_slot_dur) + slot * slot_length, slot_length), slot);
            }
            else
            {
                slot_data[brc][slot] = uartdata.mid(brc * (slot_count * slot_length + crc_over_slot_dur) + slot * slot_length, slot_length);
            }
            ui->textEdit_measdata->append(slot_data[brc][slot].toHex());
            m_sensorstatus[brc][slot] = slot_data[brc][slot].at(0);
            m_kac[brc][slot] = (m_sensorstatus[brc][slot] >> 5) & 0x07;
            CheckKAC(m_sensorstatus[brc][slot], slot+1, brc+1);

            //crc[brc][slot] = uartdata.mid(brc * (slot_count * slot_length + crc_over_slot_dur) + slot * slot_length + slot_length - slot_duration - crc_length, crc_length);
            crc[brc][slot] = slot_data[brc][slot].mid(slot_length - slot_duration - crc_length, crc_length);


            m_pdcm_crc[brc][slot] = (int)CheckMOBEBRCCRC(slot, brc, slot_data[brc][slot].mid(0, slot_data_length+1), crc[brc][slot]);
        }
    }

    m_packets = m_packets + brc_count*slot_count;

    for (slot = 0; slot < slot_count; slot++)
    {
        for (brc = 0; brc < brc_count; brc++)
        {
            compressed_data[slot].append(slot_data[brc][slot].mid(1, slot_data_length));
        }

        DIAGWORD1_pos = get_diag_position(meas_type, comp_method,\
                                  meas_duration, true, 0, compr_timer) - 2;

        //ui->lineEditCount->setText(QString("%1").arg(DIAGWORD1_pos));
        /*if (gp_DSI3Master->MVar_version == VERSION_RDUSDMX)
        {
            DIAGWORD1_pos = DIAGWORD1_pos + 2;
        }*/

        if ((gp_DSI3Master->MVar_version != VERSION_RDUSDMX) && (gp_DSI3Master->MVar_version != VERSION_RDUS) && (gp_DSI3Master->MVar_version != VERSION_RDUS2DMX)\
                && (gp_DSI3Master->MVar_version != VERSION_MOBEDMX) && (gp_DSI3Master->MVar_version != VERSION_RDUS3DCH) && (gp_DSI3Master->MVar_version != VERSION_RDUSBSD)\
                && (gp_DSI3Master->MVar_version != VERSION_RDUS3DMX) && (gp_DSI3Master->MVar_version != VERSION_MOBE2DMX))
        {
            //diag word 1
            if (compressed_data[slot].length() >= (DIAGWORD1_pos + 2))
            {
                m_diagword1[slot] = compressed_data[slot].mid(DIAGWORD1_pos + 1, 1);
                m_diagword1[slot].append(compressed_data[slot].mid(DIAGWORD1_pos, 1));
                compressed_data[slot].remove(DIAGWORD1_pos, compressed_data[slot].length() - DIAGWORD1_pos - 1);
                m_diagword2[slot] = QByteArray(2, 0);
            }
            else
            {
                m_diagword1[slot] = QByteArray(2, 0);
                m_diagword2[slot] = QByteArray(2, 0);
            }
        }
        else
        {
            //diag word 1+2
            if (compressed_data[slot].length() >= (DIAGWORD1_pos + 4))
            {
                m_diagword1[slot] = compressed_data[slot].mid(DIAGWORD1_pos + 1, 1);
                m_diagword1[slot].append(compressed_data[slot].mid(DIAGWORD1_pos, 1));
                m_diagword2[slot] = compressed_data[slot].mid(DIAGWORD1_pos + 3, 1);
                m_diagword2[slot].append(compressed_data[slot].mid(DIAGWORD1_pos + 2, 1));

                compressed_data[slot].remove(DIAGWORD1_pos, compressed_data[slot].length() - DIAGWORD1_pos + 1);
            }
            else
            {
                m_diagword1[slot] = QByteArray(2, 0);
                m_diagword2[slot] = QByteArray(2, 0);
            }
        }



        //short range
        m_short_range[slot] = GetAndRemoveShortRange(&compressed_data[slot]);

        //diag word 0
        m_diagword0[slot] = compressed_data[slot].mid(0, 2);
        compressed_data[slot].remove(0, 2);
    }

    //compression = GET_REG("SREGX052[comp_method]").toInt();
    //compression = gp_DSI3Slave->Varcomp_method.value();

    int cnt = 0;

    m_meas_slots_iq.clear();

    if ((comp_method == 9) || (comp_method == 12))      // IQ data
    {
        for (i = 0; i < 12; i++)
        {
            m_meas_iq[i].clear();
        }


        for (slot = 0; slot < MAX_SLOT_NUMBER; slot++)
        {
            if (slot < slot_count)
            {
                /*if (m_meas_slots.at(slot).contains("L", Qt::CaseInsensitive))
                    channel_low = true;
                else
                    channel_low = false;*/

                txt = m_meas_slots.at(slot);
                txt.remove("H");
                txt.remove("L");


                m_meas_slots_iq.append(txt + "I");
                m_meas_slots_iq.append(txt + "Q");



                m_meas_iq[cnt++] = Decompress_data(comp_method, channel_low, compressed_data[slot], true);
                m_meas_iq[cnt++] = Decompress_data(comp_method, channel_low, compressed_data[slot], false);
            }
            else
            {
                m_diagword0[slot] = QByteArray(2, 0);
                m_diagword1[slot] = QByteArray(2, 0);
                m_diagword2[slot] = QByteArray(2, 0);
            }
        }
    }
    else    // magnitude
    {
        for (slot = 0; slot < MAX_SLOT_NUMBER; slot++)
        {
            if (slot < slot_count)
            {
                if (m_meas_slots.at(slot).contains("L", Qt::CaseInsensitive))
                    channel_low = true;
                else
                    channel_low = false;

                m_meas_samples[slot] = Decompress_data(comp_method, channel_low, compressed_data[slot], false);

                if (m_hide_slot[slot] == true)
                {
                    m_meas_samples[slot].fill(0, -1);
                }
            }
            else
            {
                m_meas_samples[slot].clear();
                m_diagword0[slot] = QByteArray(2, 0);
                m_diagword1[slot] = QByteArray(2, 0);
                m_diagword2[slot] = QByteArray(2, 0);
            }
        }
    }





    m_meas_freq.clear();
    m_meas_time.clear();




    if (((gp_DSI3Master->MVar_version == VERSION_RDUSBSD)|| (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX)) && ((comp_method == 9) || (comp_method == 12)))
    {
        for (i = 0; i < m_meas_iq[0].size(); i++)
        {
            if (i == 0)
            {
                m_meas_time.append(0);
            }
            else
            {
                if (i <= m_samplecount1_first102[0])
                {
                    m_meas_time.append(m_meas_time.at(i-1) + 0.1024);
                }
                else if (i <= m_samplecount2_first51[0])
                {
                    m_meas_time.append(m_meas_time.at(i-1) + 0.0512);
                }
                else
                {
                    m_meas_time.append(m_meas_time.at(i-1) + 0.1024);
                }
            }
        }
    }
    else if((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
    {
        for (i = 0; i < m_meas_samples[0].size(); i++)
        {
            if (i == 0)
            {
                m_meas_time.append(0);
            }
            else
            {
                if (i <= m_samplecount1_first102[0])
                {
                    m_meas_time.append(m_meas_time.at(i-1) + 0.1024);
                }
                else if (i <= m_samplecount2_first51[0])
                {
                    m_meas_time.append(m_meas_time.at(i-1) + 0.0512);
                }
                else
                {
                    m_meas_time.append(m_meas_time.at(i-1) + 0.1024);
                }
            }
        }
    }
    else
    {
        if (comp_method == 4)
        {
            for (i = 0; i < m_meas_samples[0].size(); i++)
            {
                m_meas_time.append(i * 0.0512);
            }
        }
        else
        {
            for (i = 0; i < m_meas_samples[0].size(); i++)
            {
                m_meas_time.append(i * 0.1024);
            }
        }
    }



    if ((meas_type == 29) && (comp_method == 7))
    {
        freq = 1/(delta_gain_val3 * 0.0000004);
        m_meas_freq.append(freq);
        m_meas_freq.append(freq);
        freq = 1/(delta_gain_val4 * 0.0000004);
        m_meas_freq.append(freq);
        m_meas_freq.append(freq);

        if ((delta_gain_tim4 == 0) || (delta_gain_tim4 == 4))       // UP chirp
        {
            for (i = 4; i < m_meas_samples[0].size(); i++)
            {
                freq = (i + 4 - phase_shift) / 78.0 * 2.0 * bw12 + centerfreq - bw12;
                m_meas_freq.append(freq);
            }
        }
        else if ((delta_gain_tim4 == 1) || (delta_gain_tim4 == 5))   // DOWN chirp
        {
            for (i = 4; i < m_meas_samples[0].size(); i++)
            {
                freq = centerfreq + bw12 - (i - 4 - phase_shift) / 78.0 * 2.0 * bw12;
                m_meas_freq.append(freq);
            }
        }
        else if ((delta_gain_tim4 == 2) || (delta_gain_tim4 == 6))   // UP+DOWN chirp
        {
            for (i = 4; i < m_meas_samples[0].size(); i++)
            {
                if (i <= 38)
                {
                    freq = (i - phase_shift) / 39.0 * 2.0 * bw12 + centerfreq - bw12;
                }
                else
                {
                    freq = -(i - 39 - phase_shift) / 39.0 * 2.0 * bw12 + centerfreq + bw12;
                }
                m_meas_freq.append(freq);
            }
        }
        else
        {
            for (i = 4; i < m_meas_samples[0].size(); i++)
            {
                m_meas_freq.append(0);
            }
        }
    }
}

void Measurement::ShowData(void)
{
    ui->textEditDiagError->clear();
    for (int slot = 0; slot < MAX_SLOT_NUMBER; slot++)
    {
        CheckDIAGWORD0(m_diagword0[slot], slot+1);
        DIAGWORD1[slot].fromByteArray(m_diagword1[slot]);
        DIAGWORD2[slot].fromByteArray(m_diagword2[slot]);
    }
    //gp_DSI3Master->Log(LOG_TYPE_TIME, "ShowData - 1");

    UpdateSummaryGraphDataAndFormat();

    //gp_DSI3Master->Log(LOG_TYPE_TIME, "ShowData - 2");

    UpdateSummaryTable();
    UpdateIQTable();

    //gp_DSI3Master->Log(LOG_TYPE_TIME, "ShowData - 3");

    UpdateDetailTable();

    //gp_DSI3Master->Log(LOG_TYPE_TIME, "ShowData - 4");

    slot_UpdateALL();   

    //gp_DSI3Master->Log(LOG_TYPE_TIME, "ShowData - 5");

    slot_updateAllControls();

    //gp_DSI3Master->Log(LOG_TYPE_TIME, "ShowData - 6");

}

QVector<double> Measurement::Decompress_data(int compression, bool channel_low, QByteArray compressed_data, bool I)
{
    int i;
    uint j;
    QVector<double> result;
    QByteArray compressed_data_swapped;

    for (i = 0; i < (compressed_data.size() / 2); i++)
    {
        compressed_data_swapped.append(compressed_data.at(2*i + 1));
        compressed_data_swapped.append(compressed_data.at(2*i + 0));
    }

    if ((compression == 2) || (compression == 3))
    {
            //QVectorAppend(&result, Decompress_Mode1(compressed_data_swapped));
            result = Decompress_Mode1(compressed_data_swapped);
    }
    else if (compression == 4)
    {
        for (i = 0; i < (compressed_data_swapped.length() / 2); i++)
        {
            result.append((double)((compressed_data_swapped.at(2*i) & 0xFF) << 2));
            result.append((double)((compressed_data_swapped.at(2*i + 1) & 0xFF) << 2));
        }
    }
    else if (compression == 5)
    {
        result = Decompress_Mode5(compressed_data_swapped, channel_low);
    }
    else if (compression == 7)
    {
        result = Decompress_Mode7(compressed_data_swapped, channel_low);
    }
    else if (compression == 8)
    {
        // uncompressed data 102.4us
        for (i = 0; i < (compressed_data_swapped.length() / 2); i++)
        {
            result.append((double)((compressed_data_swapped.at(2*i) & 0xFF) << 2));
            result.append((double)((compressed_data_swapped.at(2*i + 1) & 0xFF) << 2));
        }
    }
    else if (compression == 9)
    {
        // compressed IQ data 102.4us
        result = Decompress_Mode9(compressed_data_swapped, I);
    }
    else if ((compression == 14)|| (compression == 15))
    {
        result = Decompress_Mode14(compressed_data_swapped);
    }
    else if (compression == 12)
    {
        // uncompressed IQ 102.4us
        result = Decompress_Mode12(compressed_data_swapped, I);
    }
    return result;
}

QVector<double> Measurement::Decompress_Mode1(QByteArray slot_data)
{
    QVector<double> result;
    quint32 frame = 0;
    uint scaling;
    uint T;
    uint D;
    uint sample;
    const uint Div_lut[4] = {128, 108, 91, 76};
    int i, j;

    for (i = 0; i < (slot_data.size() / 4); i++)
    {
        frame = slot_data.at(i*4 + 3) & 0xFF;
        frame = frame << 8;
        frame = frame | ((uint)slot_data.at(i*4 + 2) & 0xFF);
        frame = frame << 8;
        frame = frame | ((uint)slot_data.at(i*4 + 1) & 0xFF);
        frame = frame << 8;
        frame = frame | ((uint)slot_data.at(i*4 + 0) & 0xFF);

        scaling = frame & 0x1F;
        D = Div_lut[scaling & 0x03];
        T = scaling >> 2;

        frame = frame >> 5;

        for (j = 0; j < 9; j++)
        {
            sample = frame & 0x07;

            if (sample == 0)
            {
                result.append(0);
            }
            else
            {
                sample = sample * D + D/2;
                result.append((double)(sample >> T));
            }
            frame = frame >> 3;
        }
    }
    return result;
}

QVector<double> Measurement::Decompress_Mode5(QByteArray slot_data, bool channel_low)
{
    QVector<double> result;
    QVector<int> Delta_lut(16);
    int i;
    int temp;
    Delta_lut[0] = 0;           Delta_lut[1] = 1;           Delta_lut[2] = 2;           Delta_lut[3] = 4;
    Delta_lut[4] = 7;           Delta_lut[5] = 11;          Delta_lut[6] = 19;          Delta_lut[7] = 31;
    Delta_lut[8] = 0;           Delta_lut[9] = -1;          Delta_lut[10] = -2;         Delta_lut[11] = -4;
    Delta_lut[12] = -7;         Delta_lut[13] = -11;        Delta_lut[14] = -19;        Delta_lut[15] = -31;

    quint8 uncompressed2 = 0;

    if (slot_data.length() >=2 )
    {
        result.append((slot_data.at(0) & 0xFF));
        uncompressed2 = slot_data.at(1);
        result.append((slot_data.at(1) & 0xFF));

        for (i = 1; i < slot_data.length(); i++)
        {
            if (channel_low)
            {
                temp = (int)uncompressed2 + Delta_lut[slot_data.at(i) & 0x0F];
                if (temp > 255)
                    temp = 255;
                if (temp < 0)
                     temp = 0;
                result.append(temp);
                uncompressed2 = temp;

                temp = (int)uncompressed2 + Delta_lut[(slot_data.at(i) >> 4) & 0x0F];
                if (temp > 255)
                    temp = 255;
                if (temp < 0)
                     temp = 0;
                result.append(temp);
                uncompressed2 = temp;
            }
            else
            {
                uncompressed2 = (quint8)(((int)uncompressed2 + Delta_lut[slot_data.at(i) & 0x0F]) & 0xFF);
                result.append(uncompressed2);
                uncompressed2 = (quint8)(((int)uncompressed2 + Delta_lut[(slot_data.at(i) >> 4) & 0x0F]) & 0xFF);
                result.append(uncompressed2);
            }
        }

        result.remove(78, result.size() - 78);
    }
    return result;
}

QVector<double> Measurement::Decompress_Mode7(QByteArray slot_data, bool channel_low)
{
    QVector<double> result;
    QVector<int> Delta_lut(16);
    int i;
    int temp;
    Delta_lut[0] = 0;           Delta_lut[1] = 1;           Delta_lut[2] = 2;           Delta_lut[3] = 4;
    Delta_lut[4] = 7;           Delta_lut[5] = 11;          Delta_lut[6] = 19;          Delta_lut[7] = 31;
    Delta_lut[8] = 0;           Delta_lut[9] = -1;          Delta_lut[10] = -2;         Delta_lut[11] = -4;
    Delta_lut[12] = -7;         Delta_lut[13] = -11;        Delta_lut[14] = -19;        Delta_lut[15] = -31;

    quint8 uncompressed2 = 0;

    if (slot_data.length() >=6 )
    {
        result.append((slot_data.at(0) & 0xFF));
        result.append((slot_data.at(1) & 0xFF));
        result.append((slot_data.at(2) & 0xFF));
        result.append((slot_data.at(3) & 0xFF));
        result.append((slot_data.at(4) & 0xFF));
        uncompressed2 = slot_data.at(5);
        result.append((slot_data.at(5) & 0xFF));

        for (i = 6; i < slot_data.length(); i++)
        {
            if (channel_low)
            {
                temp = (int)uncompressed2 + Delta_lut[slot_data.at(i) & 0x0F];
                if (temp > 255)
                    temp = 255;
                if (temp < 0)
                     temp = 0;
                result.append(temp);
                uncompressed2 = temp;

                temp = (int)uncompressed2 + Delta_lut[(slot_data.at(i) >> 4) & 0x0F];
                if (temp > 255)
                    temp = 255;
                if (temp < 0)
                     temp = 0;
                result.append(temp);
                uncompressed2 = temp;
            }
            else
            {
                uncompressed2 = (quint8)(((int)uncompressed2 + Delta_lut[slot_data.at(i) & 0x0F]) & 0xFF);
                result.append(uncompressed2);
                uncompressed2 = (quint8)(((int)uncompressed2 + Delta_lut[(slot_data.at(i) >> 4) & 0x0F]) & 0xFF);
                result.append(uncompressed2);
            }
        }

        result.remove(74, result.size() - 74);
    }
    return result;
}

QVector<double> Measurement::Decompress_Mode9(QByteArray slot_data, bool I)
{
    QVector<double> result;
    QVector<int> Div_lut(4);
    int sample;
    uint usample;
    uint frame;
    uint scaling;
    uint divider;
    uint D;
    uint T;
    bool neg;
    int i, j;

    Div_lut[0] = 186;           Div_lut[1] = 156;           Div_lut[2] = 132;           Div_lut[3] = 111;

    for (i = 0; i < slot_data.size() / 4; i++)
    {
        frame = slot_data.at(i*4 + 3) & 0xFF;
        frame = frame << 8;
        frame = frame | ((uint)slot_data.at(i*4 + 2) & 0xFF);
        frame = frame << 8;
        frame = frame | ((uint)slot_data.at(i*4 + 1) & 0xFF);
        frame = frame << 8;
        frame = frame | ((uint)slot_data.at(i*4 + 0) & 0xFF);

        scaling = frame & 0x0F;
        T = scaling >> 2;
        D = Div_lut[scaling & 0x03];

        frame = frame >> 4;
        for (j = 0; j < 4; j++)
        {
            if (I)
            {
                sample = frame & 0x7F;
                sample = sample % 11;
                if (sample < 5)
                {
                    neg = true;
                }
                else
                {
                    neg = false;
                }
                sample = sample - 5;
                sample = abs(sample);
                sample = sample * D;
                sample = sample >> (T+1);
                //sample = sample * 4;
                if (neg)
                {
                    sample = -1 * sample;
                }
                sample = sample + 511;
                usample = ((uint)sample & 0x3FF);
                result.append((uint)usample);
            }

            if (I == false)
            {
                sample = frame & 0x7F;
                sample = (int)floor(sample / 11.0);
                if (sample < 5)
                {
                    neg = true;
                }
                else
                {
                    neg = false;
                }
                sample = sample - 5;
                sample = abs(sample);
                sample = sample * D;
                sample = sample >> (T+1);
                //sample = sample * 4;
                if (neg)
                {
                    sample = -1 * sample;
                }
                sample = sample + 511;
                usample = ((uint)sample & 0x3FF);
                result.append((uint)usample);
            }
            frame = frame >> 7;


        }



/*        scaling = frame & 0x0F;
        T = scaling >> 2;
        D = Div_lut[scaling & 0x03];

        frame = frame >> 4;


        for (j = 0; j < 4; j++)
        {
            sample = frame & 0x7F;
            sample = sample % 11;
            sample = sample - 5;
            sample = sample * D;
            divider = (uint)pow(2.0, (int)T);
            //sample = sample / divider;
            sample = sample / divider / 2;
            //sample = sample + 1024;
            sample = sample + 512;
            usample = (uint)(sample & 0x3FF);

            if (I)
            {
                result.append((uint)usample);
            }




            sample = frame & 0x7F;
            sample = (int)floor(sample / 11.0);
            sample = sample - 5;
            sample = sample * D;
            divider = (uint)pow(2.0, (int)T);
            //sample = sample / divider;
            sample = sample / divider / 2;
            //sample = sample + 1024;
            sample = sample + 512;
            usample = (uint)(sample & 0x3FF);


            if (I == false)
            {
                result.append((uint)usample);
            }

            frame = frame >> 7;

        }*/

    }

    return result;
}

QVector<double> Measurement::Decompress_Mode14(QByteArray slot_data)
{
    int i, i_prol, k;
    uint frame;
    QVector <double> temp_result;
    QVector <double> result;

    for (k = 0; k < (slot_data.size() / 34); k++)
    {
        temp_result = Decompress_Mode1(slot_data.mid(34*k, 32));
        frame = slot_data.at(34*k + 33) & 0xFF;
        frame = frame << 8;
        frame = frame | ((uint)slot_data.at(34*k + 32) & 0xFF);

        for (i = 0; i < temp_result.size(); i++)
        {
            i_prol = (uint)((float)i / 4.5);
            if (((frame >> i_prol) & 0x01) == 0x01)
            {
                result.append(temp_result.at(i) + 1024);
            }
            else
            {
                result.append(temp_result.at(i));
            }
        }
    }
    int rest = (slot_data.size() % 34);
    if (rest != 0)
    {
        rest = rest - 2;
        temp_result = Decompress_Mode1(slot_data.mid(34*k, rest));
        frame = slot_data.at(34*k + rest + 1) & 0xFF;
        frame = frame << 8;
        frame = frame | ((uint)slot_data.at(34*k + rest) & 0xFF);

        for (i = 0; i < temp_result.size(); i++)
        {
            i_prol = (uint)((float)i / 4.5);
            if (((frame >> i_prol) & 0x01) == 0x01)
            {
                result.append(temp_result.at(i) + 1024);
            }
            else
            {
                result.append(temp_result.at(i));
            }
        }
    }
    return result;
}

QVector<double> Measurement::Decompress_Mode12(QByteArray slot_data, bool I)
{
    int i, i_prol, k;
    uint frame;
    QVector <double> temp_result;
    QVector <double> result;
    qint8 IQtemp;
    uint rest;

    i = slot_data.size();
    for (k = 0; k < (slot_data.size() / 130); k++)
    {
        temp_result.clear();
        for (i = 0; i < 64; i++)
        {
            if (I)
            {
                IQtemp = slot_data.at(130*k + 2*i);
                temp_result.append((IQtemp + 128) << 2);
            }
            else
            {
                IQtemp = slot_data.at(130*k + 2*i + 1);
                temp_result.append((IQtemp + 128) << 2);
            }
        }

        frame = slot_data.at(130*k + 129) & 0xFF;
        frame = frame << 8;
        frame = frame | ((uint)slot_data.at(130*k + 128) & 0xFF);

        for (i = 0; i < temp_result.size(); i++)
        {
            i_prol = (uint)((float)i / 4);
            if (((frame >> i_prol) & 0x01) == 0x01)
            {
                result.append(temp_result.at(i) + 1024);
            }
            else
            {
                result.append(temp_result.at(i));
            }
            //result.append(temp_result.at(i));
        }
    }

    rest = slot_data.size() % 130;

    if (rest != 0)
    {
        rest = rest - 2;
        temp_result.clear();
        for (i = 0; i < (rest / 2); i++)
        {
           if (I)
           {
               IQtemp = slot_data.at(130*k + 2*i);
               temp_result.append((IQtemp + 128) << 2);
           }
           else
           {
               IQtemp = slot_data.at(130*k + 2*i + 1);
               temp_result.append((IQtemp + 128) << 2);
           }
        }

        frame = slot_data.at(130*k + rest + 1) & 0xFF;
        frame = frame << 8;
        frame = frame | ((uint)slot_data.at(130*k + rest) & 0xFF);

        for (i = 0; i < temp_result.size(); i++)
        {
            i_prol = (uint)((float)i / 4);
            if (((frame >> i_prol) & 0x01) == 0x01)
            {
                result.append(temp_result.at(i) + 1024);
            }
            else
            {
                result.append(temp_result.at(i));
            }
            //result.append(temp_result.at(i));
        }
    }

    return result;
}

bool Measurement::StartContinuousMeasurement(void)
{
    m_meas_counter = -1;
    if(MEAStimer->isActive() == false)
    {
        Measure(MEASURE_FIRST);
        m_counter = 0;
        MEAStimer->start();
    }
    return true;
}

bool Measurement::PerformNCycles(void)
{
    int scheduler_count = 0;
    int scheduler_ena = 0;
    int i;

    if (ui->comboBoxMeasureSource->currentIndex() == 1)
    {
        scheduler_ena = SCHEDULER_ENA.toUInt();
        for (i = 0; i < 10; i++)
        {
            if (((scheduler_ena >> i) & 0x01) == 0x01)
            {
                scheduler_count++;
            }
        }
        m_meas_counter = scheduler_count * NCYCLES.toInt();
    }
    else
    {
        m_meas_counter = NCYCLES.toInt();
    }

    if(MEAStimer->isActive() == false)
    {
        Measure(MEASURE_FIRST);
    }

    MEAStimer->start();
    return true;
}

bool Measurement::StopMeasurement(void)
{
    m_meas_counter = 0;

    if (MEAStimer->isActive() == true)
    {
        MEAStimer->stop();
        Measure(MEASURE_LAST);
    }

    return true;
}

void Measurement::InitGraphsAndTables(void)
{
    int i = 0;

    for (i = 0; i < 10; i++)
    {
        ui->ptSummary->addGraph();
        ui->ptLegend->addGraph();
        ui->ptShortRange->addGraph();
        ui->ptIQ->addGraph();
        ui->ptLegendIQ->addGraph();
        ui->ptSummary->graph(i)->setPen(QPen(QColor::fromRgb(colors[i][0], colors[i][1], colors[i][2])));
        ui->ptLegend->graph(i)->setPen(QPen(QColor::fromRgb(colors[i][0], colors[i][1], colors[i][2])));
        ui->ptShortRange->graph(i)->setPen(QPen(QColor::fromRgb(colors[i][0], colors[i][1], colors[i][2])));
        ui->ptIQ->graph(i)->setPen(QPen(QColor::fromRgb(colors[i][0], colors[i][1], colors[i][2])));
        ui->ptLegendIQ->graph(i)->setPen(QPen(QColor::fromRgb(colors[i][0], colors[i][1], colors[i][2])));
    }

    for (i = 0; i < 10; i++)
    {
        ui->ptSummary->addGraph();
        ui->ptSummary->graph(i+10)->setPen(QPen(QColor::fromRgb(colors[i][0], colors[i][1], colors[i][2])));
        ui->ptIQ->addGraph();
        ui->ptIQ->graph(i+10)->setPen(QPen(QColor::fromRgb(colors[i][0], colors[i][1], colors[i][2])));
    }

    ui->ptSummary->xAxis->setRange(0, 10);
    //ui->ptSummary->yAxis->setRange(0, 1040);
    ui->ptSummary->yAxis->setRange(0, 1140);
    ui->ptSummary->xAxis2->setRange(0, 1.7);
    ui->ptSummary->xAxis2->setVisible(true);
    ui->ptSummary->xAxis->setLabel("time [ms]");
    ui->ptSummary->yAxis->setLabel("Samples [-]");
    ui->ptSummary->xAxis2->setLabel("Approx distance [m]");
    ui->ptLegend->xAxis->setVisible(false);
    ui->ptLegend->yAxis->setVisible(false);
    ui->ptLegend->graph(2)->setName("ECHO_DET");
    ui->ptLegend->legend->setVisible(true);
    ui->ptLegend->legend->setPositionStyle(QCPLegend::PositionStyle::psBottom);
    ui->ptShortRange->xAxis->setRange(0, 15);
    ui->ptShortRange->yAxis->setRange(-10000, 10000);
    ui->ptShortRange->xAxis->setLabel("Samples");
    ui->ptShortRange->yAxis->setLabel("Short range samples");

    ui->ptIQ->xAxis->setRange(0, 10);
    ui->ptIQ->yAxis->setRange(0, 1140);
    ui->ptIQ->xAxis->setLabel("time [ms]");
    ui->ptIQ->yAxis->setLabel("Samples [-]");
    ui->ptLegendIQ->xAxis->setVisible(false);
    ui->ptLegendIQ->yAxis->setVisible(false);
    ui->ptLegendIQ->legend->setVisible(true);
    ui->ptLegendIQ->legend->setPositionStyle(QCPLegend::PositionStyle::psBottom);

    for (i = 0; i < GRAPH_DETAIL.size(); i++)
    {
        GRAPH_DETAIL.at(i)->addGraph();
        GRAPH_DETAIL.at(i)->graph(0)->setPen(QColor::fromRgb(42, 162, 42));
        GRAPH_DETAIL.at(i)->xAxis->setRange(0, 10);
        GRAPH_DETAIL.at(i)->yAxis->setRange(0, 1040);
        GRAPH_DETAIL.at(i)->xAxis->setLabel("time [ms]");
        GRAPH_DETAIL.at(i)->yAxis->setLabel("Samples [-]");
        GRAPH_DETAIL.at(i)->legend->setVisible(true);
        GRAPH_DETAIL.at(i)->legend->setPositionStyle(QCPLegend::PositionStyle::psTopRight);
        GRAPH_DETAIL.at(i)->replot();
    }

    for (i = 0; i < GRAPH_OVERVIEW.size(); i++)
    {
        GRAPH_OVERVIEW.at(i)->addGraph();
        GRAPH_OVERVIEW.at(i)->graph(0)->setPen(QColor::fromRgb(42, 162, 42));
        GRAPH_OVERVIEW.at(i)->xAxis->setRange(0, 10);
        GRAPH_OVERVIEW.at(i)->yAxis->setRange(0, 1040);
        GRAPH_OVERVIEW.at(i)->xAxis->setLabel("time [ms]");
        GRAPH_OVERVIEW.at(i)->yAxis->setLabel("Samples [-]");
        GRAPH_OVERVIEW.at(i)->legend->setVisible(true);
        GRAPH_OVERVIEW.at(i)->legend->setPositionStyle(QCPLegend::PositionStyle::psTopRight);
        GRAPH_OVERVIEW.at(i)->replot();
    }

    ui->twSamples->setColumnCount(11);
    ui->twSamples->setRowCount(0);
    QStringList m_TableHeader;
    m_TableHeader<<"#"<<"Slot1"<<"Slot2"<<"Slot3"<<"Slot4"<<"Slot5"<<"Slot6"<<"Slot7"<<"Slot8"<<"Slot9"<<"Slot10";
    ui->twSamples->setHorizontalHeaderLabels(m_TableHeader);

    ui->twSamplesToF->setColumnCount(25);
    m_TableHeader.clear();
    m_TableHeader.append("E/S");
    for (i = 0; i < 8; i++)
    {
        m_TableHeader.append(QString("s%1ToF").arg(i+1));
        m_TableHeader.append(QString("s%1P").arg(i+1));
        m_TableHeader.append(QString("s%1W").arg(i+1));
    }
    ui->twSamplesToF->setHorizontalHeaderLabels(m_TableHeader);
    ui->twSamplesToF->setRowCount(0);

    ui->twIQ->setColumnCount(11);
    ui->twIQ->setRowCount(0);
    m_TableHeader<<"#"<<"Slot1"<<"Slot2"<<"Slot3"<<"Slot4"<<"Slot5"<<"Slot6"<<"Slot7"<<"Slot8"<<"Slot9"<<"Slot10";
    ui->twIQ->setHorizontalHeaderLabels(m_TableHeader);



}

void Measurement::UpdateSummaryGraphDataAndFormat(void)
{
    int i, j;
    QVector<double> short_range_x;
    QVector<double> m_meas_samples_wbn;
    QVector<double> m_meas_samples_clean;

    for (i = 0; i < 16; i++)
        short_range_x.append((double)i);

    /*if (m_meas_iq[0].size() > 0)
    {
        ui->ptIQ->graph(0)->setData(m_meas_time, m_meas_iq[0]);
    }
    if (m_meas_iq[1].size() > 0)
    {
        ui->ptIQ->graph(1)->setData(m_meas_time, m_meas_iq[1]);
    }*/

    for (i = 0; i < m_meas_slots_iq.size(); i++)
    {
        m_meas_samples_clean.clear();
        m_meas_samples_wbn.clear();

        for (j = 0; j < m_meas_iq[i].size(); j++)
        {
            if (m_meas_iq[i].at(j) > 1023)
            {
                m_meas_samples_wbn.append(1036+10*i);
                m_meas_samples_clean.append(((uint)m_meas_iq[i].at(j)) & 0x3FF);
            }
            else
            {
                m_meas_samples_wbn.append(1200);
                m_meas_samples_clean.append(m_meas_iq[i].at(j));
            }
        }

        ui->ptIQ->graph(i)->setData(m_meas_time, m_meas_samples_clean);
        ui->ptIQ->graph(i+10)->setData(m_meas_time, m_meas_samples_wbn);

    }


    for (i = 0; i < m_meas_slot_number; i++)
    {
        m_meas_samples_clean.clear();
        m_meas_samples_wbn.clear();
        for (j = 0; j < m_meas_samples[i].size(); j++)
        {
            if (m_meas_samples[i].at(j) > 1023)
            {
                m_meas_samples_wbn.append(1036+10*i);
                m_meas_samples_clean.append(((uint)m_meas_samples[i].at(j)) & 0x3FF);
            }
            else
            {
                m_meas_samples_wbn.append(1200);
                m_meas_samples_clean.append(m_meas_samples[i].at(j));
            }
        }

        if (m_meas_tof_ena)  //TOF recognition
        {
            ui->ptSummary->graph(i)->setData(m_meas_tof[i][0], m_meas_tof[i][1]);
            ui->ptSummary->graph(i)->setLineStyle(QCPGraph::lsNone);
            ui->ptSummary->graph(i)->setScatterStyle(QCPGraph::ScatterStyle::ssDisc);
            ui->ptSummary->graph(i)->setScatterSize(8);
            ui->ptSummary->graph(i+10)->setData(m_meas_tof[i][0], m_meas_tof[i][1]);
            ui->ptSummary->graph(i+10)->setLineStyle(QCPGraph::lsNone);
            ui->ptSummary->graph(i+10)->setScatterStyle(QCPGraph::ScatterStyle::ssDisc);
            ui->ptSummary->graph(i+10)->setScatterSize(8);
        }
        else
        {
            ui->ptSummary->graph(i)->setData(m_meas_time, m_meas_samples_clean);
            ui->ptSummary->graph(i)->setLineStyle(QCPGraph::lsLine);
            ui->ptSummary->graph(i)->setScatterStyle(QCPGraph::ScatterStyle::ssNone);
            ui->ptSummary->graph(i+10)->setData(m_meas_time, m_meas_samples_wbn);
            ui->ptSummary->graph(i+10)->setLineStyle(QCPGraph::lsLine);
            ui->ptSummary->graph(i+10)->setScatterStyle(QCPGraph::ScatterStyle::ssNone);

            ui->ptShortRange->graph(i)->setData(short_range_x, m_short_range[i]);
        }
    }




    UpdateSummaryGraphFormat();
}

void Measurement::UpdateSummaryGraphFormat(void)
{
    int i, j;
    int show = SUMMARYCHANNEL_ENA.toUInt();

    for (i = 0; i < m_meas_slot_number; i++)
    {
        if (((show >> i) & 0x01) == 0x01)
        {
            ui->ptSummary->graph(i)->setVisible(true);

            ui->ptSummary->graph(i+10)->setVisible(true);
            ui->ptShortRange->graph(i)->setVisible(true);
            ui->ptLegend->graph(i)->setName(QString("Slot %1 - %2").arg(i+1).arg(m_meas_slots.at(i)));

            /*ui->ptIQ->graph(2*i)->setVisible(true);
            ui->ptIQ->graph(2*i+1)->setVisible(true);*/
        }
        else
        {
            ui->ptSummary->graph(i)->setVisible(false);
            ui->ptSummary->graph(i+10)->setVisible(false);
            ui->ptShortRange->graph(i)->setVisible(false);
            ui->ptLegend->graph(i)->setName(QString("Slot %1 - hidden").arg(i+1));

            /*ui->ptIQ->graph(2*i)->setVisible(false);
            ui->ptIQ->graph(2*i+1)->setVisible(false);*/
        }
    }

    for (i = m_meas_slot_number; i < MAX_SLOT_NUMBER; i++)
    {
        ui->ptSummary->graph(i)->setVisible(false);
        ui->ptSummary->graph(i+10)->setVisible(false);
        ui->ptShortRange->graph(i)->setVisible(false);
        ui->ptLegend->graph(i)->setName("N/A");
    }
    ui->ptSummary->xAxis->setRange(SUMMARY_STARTMS.toInt(), SUMMARY_STOPMS.toInt());
    ui->ptSummary->xAxis2->setRange(SUMMARY_STARTMS.toDouble()*346.0/2000.0, SUMMARY_STOPMS.toDouble()*346.0/2000.0);
    ui->ptSummary->replot();
    ui->ptLegend->replot();
    ui->ptShortRange->replot();
    /*ui->ptIQ->xAxis->setRange(SUMMARY_STARTMS.toInt(), SUMMARY_STOPMS.toInt());
    ui->ptIQ->replot();*/


    if (m_meas_slots_iq.size() > 0)
    {
        for (i = 0; i < m_meas_slots_iq.size(); i++)
        {
            if (((show >> i) & 0x01) == 0x01)
            {
                ui->ptIQ->graph(i)->setVisible(true);
                ui->ptIQ->graph(i+10)->setVisible(true);
                ui->ptLegendIQ->graph(i)->setName(m_meas_slots_iq.at(i));
            }
            else
            {
                ui->ptIQ->graph(i)->setVisible(false);
                ui->ptIQ->graph(i+10)->setVisible(false);
                ui->ptLegendIQ->graph(i)->setName(m_meas_slots_iq.at(i) + " hidden");
            }
        }

        for (i = m_meas_slots_iq.size(); i < MAX_SLOT_NUMBER; i++)
        {
            ui->ptIQ->graph(i)->setVisible(false);
            ui->ptIQ->graph(i+10)->setVisible(false);
            ui->ptLegendIQ->graph(i)->setName("N/A");
        }

        ui->ptLegendIQ->replot();
        ui->ptShortRange->replot();
        ui->ptIQ->xAxis->setRange(SUMMARY_STARTMS.toInt(), SUMMARY_STOPMS.toInt());
        ui->ptIQ->replot();

    }
}

void Measurement::UpdateSummaryTable(void)
{
    // TABLE
    ui->twSamples->clearContents();
    ui->twSamplesToF->clearContents();
    ui->twSamples->setRowCount(0);
    ui->twSamplesToF->setRowCount(0);
    ui->twSamples->setColumnCount(0);
    ui->twSamplesToF->setColumnCount(0);

    int i, j;
    QStringList m_TableHeader;

    //uint comp_method = GET_REG("SREGX052[comp_method]").toUInt();
    //uint comp_method = gp_DSI3Slave->Varcomp_method.value();
    uint comp_method = gp_DSI3Slave->Getcomp_method_old(GET_REG("SWriteMEASURE[mode]").toInt());

    if(ui->cb_updatetableTof->isChecked() == true) {
        ui->twSamplesToF->setColumnCount(3*m_meas_slot_number + 1);
        m_TableHeader.append("E/S");
        for (i = 0; i < m_meas_slot_number; i++)
        {
            m_TableHeader.append(QString("S%1ToF").arg(i+1));
            m_TableHeader.append(QString("S%1P").arg(i+1));
            m_TableHeader.append(QString("S%1W").arg(i+1));
        }
        ui->twSamplesToF->setHorizontalHeaderLabels(m_TableHeader);

        for (int row = 0; row < 16; row++)
        {
            ui->twSamplesToF->setRowCount(row + 1);
            ui->twSamplesToF->setItem(row, 0, new QTableWidgetItem(QString("E%1").arg(row+1)));

            for (j = 0; j < MAX_SLOT_NUMBER; j++)
            {
                if(m_meas_tof[j][0].size() > row)
                {
                    ui->twSamplesToF->setItem(row, 3*j + 1, new QTableWidgetItem(QString("%1").arg(m_meas_tof[j][0].at(row))));
                    ui->twSamplesToF->setItem(row, 3*j + 2, new QTableWidgetItem(QString("%1").arg(m_meas_tof[j][1].at(row))));
                    ui->twSamplesToF->setItem(row, 3*j + 3, new QTableWidgetItem(QString("%1").arg(m_meas_tof[j][2].at(row))));
                }
                else
                {
                    ui->twSamplesToF->setItem(row, 3*j + 1, new QTableWidgetItem(QString("")));
                    ui->twSamplesToF->setItem(row, 3*j + 2, new QTableWidgetItem(QString("")));
                    ui->twSamplesToF->setItem(row, 3*j + 3, new QTableWidgetItem(QString("")));
                }
            }
        }
    }

    if(ui->cb_updatetableSamples->isChecked() == true) {
        m_TableHeader.clear();
        ui->twSamples->setColumnWidth(0, 100);
        ui->twSamples->setColumnCount(m_meas_slot_number + 1);
        if (comp_method == 7)
        {
            m_TableHeader.append("Freq [Hz]");
        }
        else
        {
            m_TableHeader.append("Time [ms]");
        }

        for (i = 0; i < m_meas_slot_number; i++)
        {
            m_TableHeader.append(QString("Slot%1").arg(i+1));

        }
        ui->twSamples->setHorizontalHeaderLabels(m_TableHeader);

        for (int row = 0; row < m_meas_time.size(); row++)
        {
            ui->twSamples->setRowCount(row + 1);
            //ui->twSamples->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(row)));

            if (comp_method == 7)
            {
                ui->twSamples->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(m_meas_freq.at(row))));
            }
            else
            {
                ui->twSamples->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(m_meas_time.at(row))));
            }

            for (j = 0; j < MAX_SLOT_NUMBER; j++)
            {
                if(m_meas_samples[j].size() > row)
                {
                    ui->twSamples->setItem(row, j + 1, new QTableWidgetItem(QString("%1").arg(m_meas_samples[j].at(row))));
                }
                else
                {
                    ui->twSamples->setItem(row, j + 1, new QTableWidgetItem(QString("")));
                }
            }
        }
    }
}

void Measurement::UpdateIQTable(void)
{
    // TABLE
    ui->twIQ->clearContents();
    ui->twIQ->setRowCount(0);
    ui->twIQ->setColumnCount(0);


    int i, j;
    QStringList m_TableHeader;


    if(ui->cb_updatetableIQ->isChecked() == true) {
        m_TableHeader.clear();
        ui->twIQ->setColumnWidth(0, 100);
        ui->twIQ->setColumnCount(m_meas_slots_iq.size() + 1);

            m_TableHeader.append("Time [ms]");


        for (i = 0; i < m_meas_slots_iq.size(); i++)
        {
            m_TableHeader.append(QString("%1").arg(m_meas_slots_iq.at(i)));
        }
        ui->twIQ->setHorizontalHeaderLabels(m_TableHeader);

        for (int row = 0; row < m_meas_time.size(); row++)
        {
            ui->twIQ->setRowCount(row + 1);
            //ui->twSamples->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(row)));


            ui->twIQ->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(m_meas_time.at(row))));


            for (j = 0; j < MAX_SLOT_NUMBER; j++)
            {
                if(m_meas_iq[j].size() > row)
                {
                    ui->twIQ->setItem(row, j + 1, new QTableWidgetItem(QString("%1").arg(m_meas_iq[j].at(row))));
                }
                else
                {
                    ui->twIQ->setItem(row, j + 1, new QTableWidgetItem(QString("")));
                }
            }
        }
    }
}

void Measurement::UpdateDetailGraph(void)
{
    int i, j;
    int idx;
    int slot = 0;
    int index = 0;
    static int slot_old[2] = {0, 0};
    static int graph_detail_sel_old[2] = {0, 0};
    bool update_table = false;

    if (ui->checkBoxPersistence->isChecked() == false)    // PERSISTENCE DISABLED
    {
        for (i = 0; i < 2; i++)
        {
            slot = m_graph_to_slot[GRAPH_DETAIL_SEL[i].toInt()];

            if (GRAPH_DETAIL.at(i)->graphCount() == 0)
            {
                GRAPH_DETAIL.at(i)->clearGraphs();
                GRAPH_DETAIL.at(i)->addGraph();
                GRAPH_DETAIL.at(i)->graph(0)->setPen(QColor::fromRgb(42, 162, 42));
                GRAPH_DETAIL.at(i)->legend->setVisible(true);
            }

            if (slot != slot_old[i])
            {
                update_table = true;
                slot_old[i] = slot;
            }
            if (GRAPH_DETAIL_SEL[i].toInt() != graph_detail_sel_old[i])
            {
                graph_detail_sel_old[i] = GRAPH_DETAIL_SEL[i].toInt();
            }
            if ((slot >= 0) && (slot < m_meas_slot_number))
            {
                if (m_meas_tof_ena)  //tof recognition
                {
                    GRAPH_DETAIL.at(i)->graph(0)->setData(m_meas_tof[slot][0], m_meas_tof[slot][1]);
                    GRAPH_DETAIL.at(i)->graph(0)->setLineStyle(QCPGraph::lsNone);
                    GRAPH_DETAIL.at(i)->graph(0)->setScatterStyle(QCPGraph::ScatterStyle::ssDisc);
                    GRAPH_DETAIL.at(i)->graph(0)->setScatterSize(8);
                }
                else
                {
                    GRAPH_DETAIL.at(i)->graph(0)->setData(m_meas_time, m_meas_samples[slot]);
                    GRAPH_DETAIL.at(i)->graph(0)->setLineStyle(QCPGraph::lsLine);
                    GRAPH_DETAIL.at(i)->graph(0)->setScatterStyle(QCPGraph::ScatterStyle::ssNone);
                }
                GRAPH_DETAIL.at(i)->graph(0)->setName(m_meas_slots.at(slot));
                GRAPH_DETAIL.at(i)->xAxis->setRange(DETAIL_STARTMS.toInt(), DETAIL_STOPMS.toInt());
                GRAPH_DETAIL.at(i)->replot();
                DIAGWORD1[10+i].setValue(0, 15, DIAGWORD1[slot].value(0, 15));
                DIAGWORD2[10+i].setValue(0, 15, DIAGWORD2[slot].value(0, 15));
            }
            else
            {
                GRAPH_DETAIL.at(i)->graph(0)->clearData();
                GRAPH_DETAIL.at(i)->graph(0)->setName("-");
                GRAPH_DETAIL.at(i)->xAxis->setRange(DETAIL_STARTMS.toInt(), DETAIL_STOPMS.toInt());
                GRAPH_DETAIL.at(i)->replot();
                DIAGWORD1[10+i].setValue(0, 15, 0);
                DIAGWORD2[10+i].setValue(0, 15, 0);
            }
        }
    }
    else    // PERSISTENCE ENABLED
    {

        for (i = 0; i < 2; i++)
        {
            if (GRAPH_DETAIL_SEL[i].toInt() != graph_detail_sel_old[i])
            {
                graph_detail_sel_old[i] = GRAPH_DETAIL_SEL[i].toInt();
                GRAPH_DETAIL.at(i)->clearGraphs();
            }

            DIAGWORD1[10+i].setValue(0, 15, 0);
            DIAGWORD2[10+i].setValue(0, 15, 0);

            while ((((m_history_pointer_end + 1000 - m_history_pointer_start)%1000) > GRAPH_DETAIL.at(i)->graphCount()))
            {
                index = (m_history_pointer_start + 1 + GRAPH_DETAIL.at(i)->graphCount()) % 1000;
                slot = m_graph_to_slothistory[index][GRAPH_DETAIL_SEL[i].toInt()];

                GRAPH_DETAIL.at(i)->addGraph();
                idx = GRAPH_DETAIL.at(i)->graphCount() - 1;
                if ((slot >= 0) && (slot < m_meas_slot_numberhistory[index]))
                {

                    if (m_meas_tof_ena)  //tof recognition
                    {
                        GRAPH_DETAIL.at(i)->graph(idx)->setData(m_meas_tofhistory[index][slot][0], m_meas_tofhistory[index][slot][1]);
                        GRAPH_DETAIL.at(i)->graph(idx)->setLineStyle(QCPGraph::lsNone);
                        GRAPH_DETAIL.at(i)->graph(idx)->setScatterStyle(QCPGraph::ScatterStyle::ssDisc);
                        GRAPH_DETAIL.at(i)->graph(idx)->setScatterSize(8);
                    }
                    else
                    {
                        GRAPH_DETAIL.at(i)->graph(idx)->setData(m_meas_timehistory[index], m_meas_sampleshistory[index][slot]);
                        GRAPH_DETAIL.at(i)->graph(idx)->setLineStyle(QCPGraph::lsLine);
                        GRAPH_DETAIL.at(i)->graph(idx)->setScatterStyle(QCPGraph::ScatterStyle::ssNone);     
                    }
                    GRAPH_DETAIL.at(i)->graph(idx)->setPen(QColor::fromRgb(percolors[idx%20][0], percolors[idx%20][1], percolors[idx%20][2]));

                    if ((((m_history_pointer_end + 1000 - m_history_pointer_start)%1000) - GRAPH_DETAIL.at(i)->graphCount()) == 0)
                    {
                        DIAGWORD1[10+i].setValue(0, 15, DIAGWORD1[slot].value(0, 15));
                        DIAGWORD2[10+i].setValue(0, 15, DIAGWORD2[slot].value(0, 15));
                    }
                }
                else
                {
                    GRAPH_DETAIL.at(i)->graph(GRAPH_DETAIL.at(i)->graphCount() - 1)->clearData();
                }
            }
            GRAPH_DETAIL.at(i)->xAxis->setRange(DETAIL_STARTMS.toInt(), DETAIL_STOPMS.toInt());
            GRAPH_DETAIL.at(i)->replot();
        }
    }



    slot_updateAllControls();

    if (update_table)
        UpdateDetailTable();
}

void Measurement::UpdateDetailTable(void)
{   
    int i, j;
    int slot = 0;
    QStringList m_TableHeader;

    if (ui->cb_updatetableSamples->isChecked() == false)
    {
        return;
    }

    // TABLE
    for (i = 0; i < 2; i++)
    {
        TABLE_DETAIL.at(i)->clearContents();
        TABLE_DETAIL.at(i)->setRowCount(0);

        slot = m_graph_to_slot[GRAPH_DETAIL_SEL[i].toInt()];

        if ((slot >= 0) && (slot < m_meas_slot_number))
        {
            if (m_meas_tof_ena) //TOF recognition
            {
                TABLE_DETAIL.at(i)->setColumnCount(4);
                m_TableHeader<<"ECHO"<<"ToF"<<"P"<<"W";
                TABLE_DETAIL.at(i)->setHorizontalHeaderLabels(m_TableHeader);

                for (int row = 0; row < 16; row++)
                {
                    TABLE_DETAIL.at(i)->setRowCount(row + 1);
                    TABLE_DETAIL.at(i)->setItem(row, 0, new QTableWidgetItem(QString("E%1").arg(row+1)));

                    if(m_meas_tof[i][0].size() > row)
                    {
                        TABLE_DETAIL.at(i)->setItem(row, 1, new QTableWidgetItem(QString("%1").arg(m_meas_tof[slot][0].at(row))));
                        TABLE_DETAIL.at(i)->setItem(row, 2, new QTableWidgetItem(QString("%1").arg(m_meas_tof[slot][1].at(row))));
                        TABLE_DETAIL.at(i)->setItem(row, 3, new QTableWidgetItem(QString("%1").arg(m_meas_tof[slot][2].at(row))));
                    }
                    else
                    {
                        TABLE_DETAIL.at(i)->setItem(row, 1, new QTableWidgetItem(QString("")));
                        TABLE_DETAIL.at(i)->setItem(row, 2, new QTableWidgetItem(QString("")));
                        TABLE_DETAIL.at(i)->setItem(row, 3, new QTableWidgetItem(QString("")));
                    }
                }
            }
            else
            {
                TABLE_DETAIL.at(i)->setColumnCount(2);
                m_TableHeader<<"#"<<"DATA";
                TABLE_DETAIL.at(i)->setHorizontalHeaderLabels(m_TableHeader);

                for (int row = 0; row < m_meas_time.size(); row++)
                {
                    TABLE_DETAIL.at(i)->setRowCount(row + 1);
                    TABLE_DETAIL.at(i)->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(row)));

                    if(m_meas_samples[i].size() > row)
                    {
                        TABLE_DETAIL.at(i)->setItem(row, 1, new QTableWidgetItem(QString("%1").arg(m_meas_samples[slot].at(row))));
                    }
                    else
                    {
                        TABLE_DETAIL.at(i)->setItem(row, 1, new QTableWidgetItem(QString("")));
                    }
                }
            }
        }
    }
}

void Measurement::UpdateOverviewGraph(void)
{
    int i, j;
    int slot = 0;
    int idx;
    int index = 0;
    static int graph_overview_sel_old[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    if (ui->checkBoxPersistence->isChecked() == false)  // PERSISTENCE DISABLED
    {
        for (i = 0; i < 8; i++)
        {
            slot = m_graph_to_slot[GRAPH_OVERVIEW_SEL[i].toInt()];

            if (GRAPH_OVERVIEW.at(i)->graphCount() == 0)
            {
                GRAPH_OVERVIEW.at(i)->clearGraphs();
                GRAPH_OVERVIEW.at(i)->addGraph();
                GRAPH_OVERVIEW.at(i)->graph(0)->setPen(QColor::fromRgb(42, 162, 42));
                GRAPH_OVERVIEW.at(i)->legend->setVisible(true);
            }

            if (GRAPH_OVERVIEW_SEL[i].toInt() != graph_overview_sel_old[i])
            {
                graph_overview_sel_old[i] = GRAPH_OVERVIEW_SEL[i].toInt();
            }

            if ((slot >= 0) && (slot < m_meas_slot_number))
            {
                if (m_meas_tof_ena)//tof recognition
                {
                    GRAPH_OVERVIEW.at(i)->graph(0)->setData(m_meas_tof[slot][0], m_meas_tof[slot][1]);
                    GRAPH_OVERVIEW.at(i)->graph(0)->setLineStyle(QCPGraph::lsNone);
                    GRAPH_OVERVIEW.at(i)->graph(0)->setScatterStyle(QCPGraph::ScatterStyle::ssDisc);
                    GRAPH_OVERVIEW.at(i)->graph(0)->setScatterSize(8);
                }
                else
                {
                    GRAPH_OVERVIEW.at(i)->graph(0)->setData(m_meas_time, m_meas_samples[slot]);
                    GRAPH_OVERVIEW.at(i)->graph(0)->setLineStyle(QCPGraph::lsLine);
                    GRAPH_OVERVIEW.at(i)->graph(0)->setScatterStyle(QCPGraph::ScatterStyle::ssNone);
                }
                GRAPH_OVERVIEW.at(i)->graph(0)->setName(m_meas_slots.at(slot));
                GRAPH_OVERVIEW.at(i)->xAxis->setRange(OVERVIEW_STARTMS.toInt(), OVERVIEW_STOPMS.toInt());
                GRAPH_OVERVIEW.at(i)->replot();
            }
            else
            {
                GRAPH_OVERVIEW.at(i)->graph(0)->clearData();
                GRAPH_OVERVIEW.at(i)->graph(0)->setName("-");
                GRAPH_OVERVIEW.at(i)->xAxis->setRange(OVERVIEW_STARTMS.toInt(), OVERVIEW_STOPMS.toInt());
                GRAPH_OVERVIEW.at(i)->replot();
            }
        }
    }
    else       // PERSISTENCE ENABLED
    {
        for (i = 0; i < 8; i++)
        {
            if (GRAPH_OVERVIEW_SEL[i].toInt() != graph_overview_sel_old[i])
            {
                graph_overview_sel_old[i] = GRAPH_OVERVIEW_SEL[i].toInt();
                GRAPH_OVERVIEW.at(i)->clearGraphs();
            }

            while ((((m_history_pointer_end + 1000 - m_history_pointer_start)%1000) > GRAPH_OVERVIEW.at(i)->graphCount()))
            {
                index = (m_history_pointer_start + 1 + GRAPH_OVERVIEW.at(i)->graphCount()) % 1000;
                slot = m_graph_to_slothistory[index][GRAPH_OVERVIEW_SEL[i].toInt()];

                GRAPH_OVERVIEW.at(i)->addGraph();
                idx = GRAPH_OVERVIEW.at(i)->graphCount() - 1;
                if ((slot >= 0) && (slot < m_meas_slot_numberhistory[index]))
                {

                    if (m_meas_tof_ena)  //tof recognition
                    {
                        GRAPH_OVERVIEW.at(i)->graph(idx)->setData(m_meas_tofhistory[index][slot][0], m_meas_tofhistory[index][slot][1]);
                        GRAPH_OVERVIEW.at(i)->graph(idx)->setLineStyle(QCPGraph::lsNone);
                        GRAPH_OVERVIEW.at(i)->graph(idx)->setScatterStyle(QCPGraph::ScatterStyle::ssDisc);
                        GRAPH_OVERVIEW.at(i)->graph(idx)->setScatterSize(8);
                    }
                    else
                    {
                        GRAPH_OVERVIEW.at(i)->graph(idx)->setData(m_meas_timehistory[index], m_meas_sampleshistory[index][slot]);
                        GRAPH_OVERVIEW.at(i)->graph(idx)->setLineStyle(QCPGraph::lsLine);
                        GRAPH_OVERVIEW.at(i)->graph(idx)->setScatterStyle(QCPGraph::ScatterStyle::ssNone);
                    }
                    GRAPH_OVERVIEW.at(i)->graph(idx)->setPen(QColor::fromRgb(percolors[idx%20][0], percolors[idx%20][1], percolors[idx%20][2]));
                }
                else
                {
                    GRAPH_OVERVIEW.at(i)->graph(GRAPH_OVERVIEW.at(i)->graphCount() - 1)->clearData();
                }
            }
            GRAPH_OVERVIEW.at(i)->xAxis->setRange(OVERVIEW_STARTMS.toInt(), OVERVIEW_STOPMS.toInt());
            GRAPH_OVERVIEW.at(i)->replot();
        }
    }

}

void Measurement::SwitchPersistence(bool on)
{
    for (int i = 0; i < 2; i++)
    {
        if (on)     //turn persistence ON
        {
            GRAPH_DETAIL.at(i)->clearGraphs();
            GRAPH_DETAIL.at(i)->legend->setVisible(false);
        }
        else
        {
            GRAPH_DETAIL.at(i)->clearGraphs();
            GRAPH_DETAIL.at(i)->addGraph();
            GRAPH_DETAIL.at(i)->graph(0)->setPen(QColor::fromRgb(42, 162, 42));
            GRAPH_DETAIL.at(i)->legend->setVisible(true);
        }
        GRAPH_DETAIL.at(i)->replot();
    }
    for (int i = 0; i < 8; i++)
    {
        if (on)     //turn persistence ON
        {
            GRAPH_OVERVIEW.at(i)->clearGraphs();
            GRAPH_OVERVIEW.at(i)->legend->setVisible(false);
        }
        else
        {
            GRAPH_OVERVIEW.at(i)->clearGraphs();
            GRAPH_OVERVIEW.at(i)->addGraph();
            GRAPH_OVERVIEW.at(i)->graph(0)->setPen(QColor::fromRgb(42, 162, 42));
            GRAPH_OVERVIEW.at(i)->legend->setVisible(true);
        }
        GRAPH_OVERVIEW.at(i)->replot();
    }
    slot_TWUpdate();

}

void Measurement::slot_UpdateALL(void)
{
    static const int tx_side[8] = {-457, -305, -152, 0, -1067, -915, -762, -610};
    static const int rx_side_inner_outer[8] = {152, 305, 457, 610, -457, -305, -152, 0};
    int i;
    uint per;

    for(i = 0; i < DIAGWORD_SIZE; i++)
    {
        per = DIAGWORD1[i]["period"].toUInt();
        if (per == 0)
        {
            LABEL_REVFREQ.at(i)->setText(QString("- kHz"));
        }
        else if ((gp_DSI3Master->MVar_version == VERSION_MOBEJUN) || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH)\
                  || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
        {
            LABEL_REVFREQ.at(i)->setText(QString("%1 kHz").arg(1/((per + 256) * 0.000025), 0,'f', 1));
        }
        else
        {
            LABEL_REVFREQ.at(i)->setText(QString("%1 kHz").arg(1/((per) * 0.000025), 0,'f', 1));
        }
        LABEL_REVDUR.at(i)->setText(QString("%1 us").arg(DIAGWORD1[i]["time"].toUInt() * 51.2, 0,'f', 0));


        if(ui->tabWidget->tabText(4) != "")      //ui->tabWidget->setTabText(4,"");
        {
            per = DIAGWORD1[i]["period"].toUInt();
            if (per == 0)
            {
                LABEL_REVFREQ_2.at(i)->setText(QString("- kHz"));
            }
            else if ((gp_DSI3Master->MVar_version == VERSION_MOBEJUN) || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH)\
                      || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
            {
                LABEL_REVFREQ_2.at(i)->setText(QString("%1 kHz").arg(1/((per + 256) * 0.000025), 0,'f', 1));
            }
            else
            {
                LABEL_REVFREQ_2.at(i)->setText(QString("%1 kHz").arg(1/((per) * 0.000025), 0,'f', 1));
            }
            LABEL_REVDUR_2.at(i)->setText(QString("%1 us").arg(DIAGWORD1[i]["time"].toUInt() * 51.2, 0,'f', 0));
        }
    }

    ui->textEditDiagError->append(DIAGWORD0[0]["CWNH"].extra("descr"));

    /*ui->labelDopplerSide->setText(QString("%1 / %2 Hz").arg(152.5 * TwosComplementToInt(3, 3 + GET_REG("SWriteMEASURE[doppler_tx_side]").toUInt()))\
                                                         .arg(152.5 * TwosComplementToInt(3, -3 + GET_REG("SWriteMEASURE[doppler_rx_side]").toUInt())));
    ui->labelDopplerOuter->setText(QString("%1 / %2 Hz").arg(152.5 * TwosComplementToInt(5, GET_REG("SWriteMEASURE[doppler_tx_outer]").toUInt()))\
                                                         .arg(152.5 * TwosComplementToInt(3, GET_REG("SWriteMEASURE[doppler_rx_outer]").toUInt())));
    ui->labelDopplerInner->setText(QString("%1 / %2 Hz").arg(152.5 * TwosComplementToInt(5, GET_REG("SWriteMEASURE[doppler_tx_inner]").toUInt()))\
                                                         .arg(152.5 * TwosComplementToInt(3, GET_REG("SWriteMEASURE[doppler_rx_inner]").toUInt())));
    */

    ui->labelDopplerSide->setText(QString("%1 / %2 Hz").arg(tx_side[1+2*GET_REG("SWriteMEASURE[doppler_tx_side]").toUInt()])\
                                                         .arg(rx_side_inner_outer[1+2*GET_REG("SWriteMEASURE[doppler_rx_side]").toUInt()]));
    ui->labelDopplerOuter->setText(QString("%1 / %2 Hz").arg(152.5 * TwosComplementToInt(5, 2*GET_REG("SWriteMEASURE[doppler_tx_outer]").toUInt()))\
                                                         .arg(rx_side_inner_outer[1+2*GET_REG("SWriteMEASURE[doppler_rx_outer]").toUInt()]));
    ui->labelDopplerInner->setText(QString("%1 / %2 Hz").arg(152.5 * TwosComplementToInt(5, 2*GET_REG("SWriteMEASURE[doppler_tx_inner]").toUInt()))\
                                                         .arg(rx_side_inner_outer[1+2*GET_REG("SWriteMEASURE[doppler_rx_inner]").toUInt()]));

    if (ui->tabWidget->currentIndex() == TW_SUMMARY)
    {
        UpdateSummaryGraphFormat();
    }
    else if (ui->tabWidget->currentIndex() == TW_DETAIL)
    {
        UpdateDetailGraph();
    }
    else if (ui->tabWidget->currentIndex() == TW_OVERVIEW)
    {
        UpdateOverviewGraph();
    }
    else
    {
        UpdateSummaryGraphFormat();
    }

    ui->label_bsdms->setText(QString("%1 ms").arg(0.512 * (uint)gp_DSI3Slave->SWriteMEASUREadd["beam_start_delay"]));

    /*QString txt = ui->comboBoxPeriod->currentText();
    QStringList qsl = txt.split(" ");
    int num = qsl.at(0).toInt();
    if (num < 50)
        num = num*1000;
    MEAStimer->setInterval(num - 100);*/

    int num = MEASPERIODMS.value();
    MEAStimer->setInterval(num + 5);

}
int Measurement::TwosComplementToInt(quint8 size, uint input)
{
    int output = 0;

    if ((input >> (size - 1)) == 0)     // positive
    {
        output = input;
    }
    else                                // negative
    {
        output = -1*(((~input) & ((1<<size))-1) + 1);
    }

    return output;
}
bool Measurement::SaveGraph(QString arg)
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
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "Filename is empty");
        result = false;
    }

    if (arg_list.size() > 0)
    {
        if (arg_list.at(0).contains("Summary"))
            ui->ptSummary->savePng(filename, 1000, 500);
        else if (arg_list.at(0).contains("Detail1"))
            ui->ptDetail1->savePng(filename, 1000, 500);
        else if (arg_list.at(0).contains("Detail2"))
            ui->ptDetail2->savePng(filename, 1000, 500);
        else if (arg_list.at(0).contains("IQ"))
            ui->ptIQ->savePng(filename, 1000, 500);
        else if (arg_list.at(0).contains("Overview"))
        {
            temp = arg_list.at(0);
            temp.remove("Overview");
            if ((temp.toInt() > 0) && (temp.toInt() < 9))
                GRAPH_OVERVIEW.at(temp.toInt()-1)->savePng(filename, 1000, 500);
            else
            {
                gp_DSI3Master->Log(LOG_TYPE_ERROR, "Graph name not recognized");
                result = false;
            }
        }
        else
        {
            gp_DSI3Master->Log(LOG_TYPE_ERROR, "Graph name not recognized");
            result = false;
        }
    }

    return result;
}

bool Measurement::CopyGraph(QString arg)
{
    QClipboard * clipboard = QApplication::clipboard();
    QPixmap pixmap;
    bool result = true;

    if (arg.contains("Summary"))
        pixmap = QPixmap::grabWidget(ui->ptSummary);
    else if (arg.contains("Detail1"))
        pixmap = QPixmap::grabWidget(ui->ptDetail1);
    else if (arg.contains("Detail2"))
        pixmap = QPixmap::grabWidget(ui->ptDetail2);
    else if (arg.contains("IQ"))
        pixmap = QPixmap::grabWidget(ui->ptIQ);
    else if (arg.contains("Overview"))
    {
        arg.remove("Overview");
        if ((arg.toInt() > 0) && (arg.toInt() < 9))
            pixmap = QPixmap::grabWidget(GRAPH_OVERVIEW.at(arg.toInt()-1));
        else
        {
            gp_DSI3Master->Log(LOG_TYPE_ERROR, "Graph name not recognized");
            result = false;
        }
    }
    else
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "Graph name not recognized");
        result = false;
    }

    clipboard->setPixmap(pixmap);
    return false;
}

void Measurement::MEASUREdecode(void)
{
    int i;
    int beh = 0;
    int slot = 0;
    m_meas_slots.clear();
    m_meas_slot_number = 0;

    uint compress;

    if (GET_REG("SWriteMEASURE[mode]").toInt() == 0)
    {
        compress = gp_DSI3Slave->Getcomp_method_old(0);
    }
    else
    {
        if (GET_REG("SWriteMEASURE[mode]").toInt() > 5)
        {
            SET_REG("SWriteMEASURE[mode]", 5);
        }
        compress = gp_DSI3Slave->Getcomp_method_old(GET_REG("SWriteMEASURE[mode]").toInt());
    }


    for (i = 0; i < MAX_SLOT_NUMBER; i++)
    {
        m_meas_dir[i] = true;
    }

    /*if (((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX)) &&\
            ((compress == 9) || (compress == 12)))          // IQ data with MOBEv2
    {
        for (i = 0; i < 6; i++)
        {

            m_graph_to_slot[2*i + 0] = -1;      // beh_h is ignored

            beh = (int)gp_DSI3Slave->SWriteMEASURE[QString("beh_l%1").arg(i+1)];
            if (beh != 3)
            {
                m_meas_slots.append(QString("%1L").arg(i+1));
                m_graph_to_slot[2*i + 1] = slot;
                if (beh != 2)
                    m_meas_dir[slot] = false;
                slot = slot + 1;
            }
            else
            {
                m_graph_to_slot[2*i + 1] = -1;
            }
        }
    }
    else
    {*/
        for (i = 0; i < 6; i++)
        {
            beh = (int)gp_DSI3Slave->SWriteMEASURE[QString("beh_h%1").arg(i+1)];
            if (beh != 3)
            {
                m_meas_slots.append(QString("%1H").arg(i+1));
                m_graph_to_slot[2*i + 0] = slot;
                if (beh != 2)
                    m_meas_dir[slot] = false;
                slot = slot + 1;
            }
            else
            {
                m_graph_to_slot[2*i + 0] = -1;
            }
            beh = (int)gp_DSI3Slave->SWriteMEASURE[QString("beh_l%1").arg(i+1)];
            if (beh != 3)
            {
                m_meas_slots.append(QString("%1L").arg(i+1));
                m_graph_to_slot[2*i + 1] = slot;
                if (beh != 2)
                    m_meas_dir[slot] = false;
                slot = slot + 1;
            }
            else
            {
                m_graph_to_slot[2*i + 1] = -1;
            }
        }
    //}

    if (slot > 8)
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "Too much slots in MEASURE command");
    else
        m_meas_slot_number = slot;
}

bool Measurement::ShowResultsOfIndex(void)
{
    bool result = false;
    int slot = 0;
    int j;
    int index = HISTORY_INDEX.toInt();

    if (index <= ((m_history_pointer_end + 1000 - m_history_pointer_start)%1000))
    {
        index = (m_history_pointer_start + index) % 1000;

        for (slot = 0; slot < MAX_SLOT_NUMBER; slot++)
        {
            m_meas_samples[slot] = m_meas_sampleshistory[index][slot];
            m_short_range[slot] = m_short_rangehistory[index][slot];
            m_diagword0[slot] = m_diagword0history[index][slot];
            m_diagword1[slot] = m_diagword1history[index][slot];
            m_diagword2[slot] = m_diagword2history[index][slot];
            for (j = 0; j < 3; j++)
            {
                m_meas_tof[slot][j] = m_meas_tofhistory[index][slot][j];
            }
        }
        for (slot = 0; slot < 12; slot++)
        {
            m_graph_to_slot[slot] = m_graph_to_slothistory[index][slot];
        }
        m_meas_time = m_meas_timehistory[index];
        m_meas_freq = m_meas_freqhistory[index];
        m_meas_slots = m_meas_slotshistory[index];
        m_meas_slot_number = m_meas_slot_numberhistory[index];
        m_meas_tof_ena = m_meas_tof_enahistory[index];
        ui->textEditSensorError->clear();
        ui->textEditSensorError->setText(m_sensorstatushistory[index]);
        ShowData();
        result = true;
    }
    else
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "Not enought data in the history");
    }

    return result;
}

void Measurement::ClearHistory(void)
{
    m_history_pointer_end = 0;
    m_history_pointer_start = m_history_pointer_end;
    ui->lineEditHistoryCounter->setText("0");
    gp_DSI3Master->Log(LOG_TYPE_INFO, "History was cleared");
    if (ui->checkBoxPersistence->isChecked())
    {
        GRAPH_DETAIL.at(0)->clearGraphs();
        GRAPH_DETAIL.at(1)->clearGraphs();
        GRAPH_DETAIL.at(0)->replot();
        GRAPH_DETAIL.at(1)->replot();
        for (int i = 0; i < 8; i++)
        {
            GRAPH_OVERVIEW.at(i)->clearGraphs();
            GRAPH_OVERVIEW.at(i)->replot();
        }
    }
}

bool Measurement::AddToHistory(void)
{
    bool result = false;
    int i, j, slot;
    if (ui->checkBoxEnableHistory->isChecked() == false)
    {
        return false;
    }
    if (((m_history_pointer_end - m_history_pointer_start + 1000) % 1000) == 999)
    {
        m_history_pointer_start = (m_history_pointer_start + 1) % 1000;
    }
    m_history_pointer_end = (m_history_pointer_end + 1) % 1000;

    for (slot = 0; slot < MAX_SLOT_NUMBER; slot++)
    {
        m_meas_sampleshistory[m_history_pointer_end][slot] = m_meas_samples[slot];
        m_short_rangehistory[m_history_pointer_end][slot] = m_short_range[slot];

        m_diagword0history[m_history_pointer_end][slot] = m_diagword0[slot];
        m_diagword1history[m_history_pointer_end][slot] = m_diagword1[slot];
        m_diagword2history[m_history_pointer_end][slot] = m_diagword2[slot];
        for (j = 0; j < 3; j++)
        {
            m_meas_tofhistory[m_history_pointer_end][slot][j] = m_meas_tof[slot][j];
        }
    }
    for (slot = 0; slot < 12; slot++)
    {
        m_graph_to_slothistory[m_history_pointer_end][slot] = m_graph_to_slot[slot];
    }
    m_meas_timehistory[m_history_pointer_end] = m_meas_time;
    m_meas_freqhistory[m_history_pointer_end] = m_meas_freq;
    m_meas_slotshistory[m_history_pointer_end] = m_meas_slots;
    m_meas_slot_numberhistory[m_history_pointer_end] = m_meas_slot_number;
    m_sensorstatushistory[m_history_pointer_end] = ui->textEditSensorError->toPlainText();
    m_meas_tof_enahistory[m_history_pointer_end] = m_meas_tof_ena;

    ui->lineEditHistoryCounter->setText(QString("%1").arg((m_history_pointer_end + 1000 - m_history_pointer_start)%1000));

    return result;
}

void Measurement::SetBRCReadData(QString data)
{
    ui->lineEditBRCReadData->setText(data);
    //ui->lineEditBRCReadDataCount->setText(QString("%1").arg(data.remove(' ').size()/2));
    SET_REG("PDCMDATACOUNT", data.remove(' ').size()/2);
}

void Measurement::QVectorAppend(QVector<double> *target, QVector<double> source)
{
    for (int i = 0; i < source.size(); i++)
    {
        target->append(source.at(i));
    }
}

void Measurement::CheckDIAGWORD0(QByteArray data, int slot)
{
    quint8 cwnvalue = 0;

    if (data.length() == 2)
    {
        if ((gp_DSI3Master->MVar_version == VERSION_ULSE) || (gp_DSI3Master->MVar_version == VERSION_MOBEAPR) || (gp_DSI3Master->MVar_version == VERSION_MOBEJUN)\
                || (gp_DSI3Master->MVar_version == VERSION_RDUS2DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH)\
                 || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD)|| (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
        {
            cwnvalue = (data.at(1) >> 2) & 0x0F;

            if (cwnvalue != 0)
            {
                ui->textEditDiagError->append(QString("ContiWave_noise_val_L=%1, slot=%2").arg(cwnvalue).arg(slot));
            }

            //cwnvalue = ((data.at(1) + (data.at(0) << 8)) >> 6) & 0x0F;
            cwnvalue =  (data.at(1) >> 6) & 0x03;
            cwnvalue = cwnvalue + ((data.at(0) & 0x03) << 2);

            if (cwnvalue != 0)
            {
                ui->textEditDiagError->append(QString("ContiWave_noise_val_H=%1, slot=%2").arg(cwnvalue).arg(slot));
            }

            if (data.at(0) != 0)        //bits 8 - 15
            {
                /*if (((data.at(0)) & 0x01) == 1)        //8
                {
                    ui->textEditDiagError->append(QString("TX overvolt. curr. red. OR TX undervolt., slot=%1").arg(slot));
                }*/
                if (((data.at(0) >> 2) & 0x01) == 1)        //10
                {
                    if (ui->checkBoxMaskSafety->isChecked() == false)
                    {
                        ui->textEditDiagError->append(QString("OR of all safety flags, slot=%1").arg(slot));
                    }
                }
                if (((data.at(0) >> 3) & 0x01) == 1)        //11
                {
                    ui->textEditDiagError->append(QString("TX overvolt. curr. red. OR TX undervolt., slot=%1").arg(slot));
                }

                if (((data.at(0) >> 4) & 0x01) == 1)        //12
                {
                    ui->textEditDiagError->append(QString("Wide band Noise Flag, slot=%1").arg(slot));
                }
                if (((data.at(0) >> 5) & 0x01) == 1)        //13
                {
                    ui->textEditDiagError->append(QString("Mode CRC Error, slot=%1").arg(slot));
                }
                if (((data.at(0) >> 6) & 0x01) == 1)        //14
                {
                    ui->textEditDiagError->append(QString("Thermal Flag, slot=%1").arg(slot));
                }
                if (((data.at(0) >> 7) & 0x01) == 1)        //15
                {
                    ui->textEditDiagError->append(QString("Thermal Shutdown, slot=%1").arg(slot));
                }
            }
        }
        else
        {
            cwnvalue = data.at(1) & 0x0F;

            if (cwnvalue != 0)
            {
                ui->textEditDiagError->append(QString("ContiWave_noise_val_L=%1, slot=%2").arg(cwnvalue).arg(slot));
            }

            cwnvalue = (data.at(1) >> 4) & 0x0F;

            if (cwnvalue != 0)
            {
                ui->textEditDiagError->append(QString("ContiWave_noise_val_H=%1, slot=%2").arg(cwnvalue).arg(slot));
            }

            if (data.at(0) != 0)        //bits 8 - 15
            {
                if (((data.at(0)) & 0x01) == 1)        //8
                {
                    ui->textEditDiagError->append(QString("TX overvolt. curr. red. OR TX undervolt., slot=%1").arg(slot));
                }
                if (((data.at(0) >> 2) & 0x01) == 1)        //10
                {
                    if (ui->checkBoxMaskSafety->isChecked() == false)
                    {
                        ui->textEditDiagError->append(QString("OR of all safety flags, slot=%1").arg(slot));
                    }
                }
                if (((data.at(0) >> 3) & 0x01) == 1)        //11
                {
                    ui->textEditDiagError->append(QString("Wide band Noise Flag, slot=%1").arg(slot));
                }

                if (((data.at(0) >> 4) & 0x01) == 1)        //12
                {
                    ui->textEditDiagError->append(QString("Mode CRC Error, slot=%1").arg(slot));
                }
                if (((data.at(0) >> 5) & 0x01) == 1)        //13
                {
                    ui->textEditDiagError->append(QString("Thermal Flag, slot=%1").arg(slot));
                }
                if (((data.at(0) >> 6) & 0x01) == 1)        //14
                {
                    ui->textEditDiagError->append(QString("Thermal Shutdown, slot=%1").arg(slot));
                }
                if (((data.at(0) >> 7) & 0x01) == 1)        //15
                {
                    ui->textEditDiagError->append(QString("Reset Flag, slot=%1").arg(slot));

                }
            }
        }

    }
}

void Measurement::CheckKAC(char data, int slot, int brc)
{
    if ((data & 0x1F) != 0)
    {
        if ((gp_DSI3Master->MVar_version == VERSION_ULSE) || (gp_DSI3Master->MVar_version == VERSION_MOBEAPR) || (gp_DSI3Master->MVar_version == VERSION_MOBEJUN)\
                || (gp_DSI3Master->MVar_version == VERSION_RDUS2DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH)\
                 || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
        {
            if ((data & 0x01) == 1)
            {
                if (ui->checkBoxMaskSafety->isChecked() == false)
                {
                    ui->textEditSensorError->append(QString("OR of all safety flags, brc=%1, slot=%2").arg(brc).arg(slot));
                }
            }

            if (((data >> 1) & 0x01) == 1)
                ui->textEditSensorError->append(QString("buff underflow (early), brc=%1, slot=%2").arg(brc).arg(slot));

            if (((data >> 2) & 0x01) == 1)
                ui->textEditSensorError->append(QString("buff overflow (late), brc=%1, slot=%2").arg(brc).arg(slot));

            if (((data >> 3) & 0x01) == 1)
                ui->textEditSensorError->append(QString("crc_err, brc=%1, slot=%2").arg(brc).arg(slot));

            if (((data >> 4) & 0x01) == 1)
            {
                if ((gp_DSI3Master->MVar_version == VERSION_MOBEJUN) || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH)\
                         || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
                {
                    ui->textEditSensorError->append(QString("dsi3_cfg_err, brc=%1, slot=%2").arg(brc).arg(slot));
                }
                else
                {
                    ui->textEditSensorError->append(QString("addr_cmd_err, brc=%1, slot=%2").arg(brc).arg(slot));
                }
            }

        }
        else
        {
            if ((data & 0x01) == 1)
            {
                if (ui->checkBoxMaskSafety->isChecked() == false)
                {
                    ui->textEditSensorError->append(QString("OR of all safety flags, brc=%1, slot=%2").arg(brc).arg(slot));
                }
            }

            if (((data >> 1) & 0x01) == 1)
                ui->textEditSensorError->append(QString("dsi3_cfg_err, brc=%1, slot=%2").arg(brc).arg(slot));

            if (((data >> 2) & 0x01) == 1)
                ui->textEditSensorError->append(QString("crc_err, brc=%1, slot=%2").arg(brc).arg(slot));

            if (((data >> 3) & 0x01) == 1)
                ui->textEditSensorError->append(QString("ctank_nok, brc=%1, slot=%2").arg(brc).arg(slot));

            if (((data >> 4) & 0x01) == 1)
            {
                ui->textEditSensorError->append(QString("early/late_err, brc=%1, slot=%2").arg(brc).arg(slot));
                ui->labelearlylateerror->setVisible(true);
                ui->labelearlylateerror_2->setVisible(true);
                ui->labelearlylateerror_3->setVisible(true);
                ui->labelearlylateerror_4->setVisible(true);
                m_hide_slot[slot-1] = true;
            }
        }

    }
}

bool Measurement::CheckMOBEBRCCRC(int slot, int brc, QByteArray data, QByteArray crc)
{
    bool result = false;
    quint16 crc16;
    char crc8;

    if (crc.size() == 2)
    {
        crc16 = (((quint16)crc.at(0)) << 8) & 0xFF00;
        crc16 = crc16 | ((quint16)(crc.at(1)) & 0x00FF);

        if (crc16 == gp_UartUartController->CalculateUPAECRC16withSEED((quint16)(slot+1), data))
            result = true;
        else
            result = false;

        if (result == false)
        {
            m_PERerrors = m_PERerrors + 1;
            ui->textEditSensorError->append(QString("crc error in BRC response, brc=%1, slot=%2").arg(brc + 1).arg(slot + 1));
            ui->labelCRCerror->setVisible(true);
            ui->labelCRCerror_2->setVisible(true);
            ui->labelCRCerror_3->setVisible(true);
            ui->labelCRCerror_4->setVisible(true);
            m_hide_slot[slot] = true;
        }

    }
    else if (crc.size() == 1)
    {
        crc8 = crc.at(0);

        if (crc8 == gp_UartUartController->CalculateUPAECRCwithSEED((char)(slot + 1), data))
            result = true;
        else
            result = false;

        if (result == false)
        {
            m_PERerrors = m_PERerrors + 1;
            ui->textEditSensorError->append(QString("crc error in BRC response, brc=%1, slot=%2").arg(brc + 1).arg(slot + 1));
            ui->labelCRCerror->setVisible(true);
            ui->labelCRCerror_2->setVisible(true);
            ui->labelCRCerror_3->setVisible(true);
            ui->labelCRCerror_4->setVisible(true);
            m_hide_slot[slot] = true;
        }

    }
    return result;
}

QVector<double> Measurement::GetAndRemoveShortRange(QByteArray *data)
{
    QVector<double> ShortRangeSamples;
    QByteArray ShortRangeData;
    uint i;
    uint compress;
    uint disabled;
    uint meas_type;
    QString num;

    if (GET_REG("SWriteMEASURE[mode]").toInt() == 0)
    {
        //compress = gp_DSI3Slave->Varcomp_method.value();
        compress = gp_DSI3Slave->Getcomp_method_old(0);
        //disabled = GET_REG("SREGX05E[short_range_thr]").toUInt();
        disabled = 0;
        //meas_type = GET_REG("SREGX042[meas_type]").toUInt();
        meas_type = gp_DSI3Slave->Getmeas_type_old(0);
    }
    else
    {
        if (GET_REG("SWriteMEASURE[mode]").toInt() > 5)
        {
            SET_REG("SWriteMEASURE[mode]", 5);
        }
        //compress = gp_DSI3Slave->SCOMP.at(GET_REG("SWriteMEASURE[mode]").toInt())->value();
        compress = gp_DSI3Slave->Getcomp_method_old(GET_REG("SWriteMEASURE[mode]").toInt());

        num = QString("%1").arg(0x42 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()],2,16,QChar('0')).toUpper();
        //meas_type = GET_REG(QString("SREGX1%1[meas_type]").arg(num)).toUInt();
        meas_type = gp_DSI3Slave->Getmeas_type_old(GET_REG("SWriteMEASURE[mode]").toInt());
        disabled = 0;
    }

    int testindex = 0;

    if (((disabled & 0x01) == 0) || (meas_type < 20) || (meas_type > 24))
    {
        return ShortRangeSamples;
    }

    switch (compress)
    {
    case 0:
        if (data->length() < 43)
            return ShortRangeSamples;
        ShortRangeData = data->mid(26 - testindex, 16);
        data->remove(26 - testindex, 16);
        break;
    case 1:
        if (data->length() < 47)
            return ShortRangeSamples;
        ShortRangeData = data->mid(30 - testindex, 16);
        data->remove(30 - testindex, 16);
        break;
    case 2:
        if (data->length() < 47)
            return ShortRangeSamples;
        ShortRangeData = data->mid(30 - testindex, 16);
        data->remove(30 - testindex, 16);
        break;
    case 3:
        if (data->length() < 67)
            return ShortRangeSamples;
        ShortRangeData = data->mid(50 - testindex, 16);
        data->remove(50 - testindex, 16);
        break;
    case 6:
        if (data->length() < 75)
            return ShortRangeSamples;
        ShortRangeData = data->mid(58 - testindex, 16);
        data->remove(58 - testindex, 16);
        break;

    }

    uint sample;
    for (i = 0; i < (ShortRangeData.length() / 2); i++)
    {
        sample = ShortRangeData.at(2*i + 1) & 0xFF;
        ShortRangeSamples.append((double)((((int)sample) - 128.0)*(1.0 / 0.0001024) / 128.0));
        sample = ShortRangeData.at(2*i + 0) & 0xFF;
        ShortRangeSamples.append((double)((((int)sample) - 128.0)*(1.0 / 0.0001024) / 128.0));
    }

    if ((ShortRangeData.at(14) & 0x01) == 0)
    {
        ShortRangeSamples.clear();
    }

    return ShortRangeSamples;
}

void Measurement::slot_MEASTimer_timeout()
{
    if (m_meas_counter == -1)
    {
        Measure(MEASURE_CONTI);
    }
    else if (m_meas_counter == 0)
    {
        MEAStimer->stop();
        Measure(MEASURE_LAST);
    }
    else if (m_meas_counter < -1)
    {
        m_meas_counter = 0;
        MEAStimer->stop();
        Measure(MEASURE_LAST);
    }
    else
    {
        Measure(MEASURE_CONTI);
        m_meas_counter = m_meas_counter - 1;
    }
}

bool Measurement::SaveCSV(QString arg)
{
    int slot = 0;
    int i, row;
    bool result = true;
    QString filename;
    QString delim = ui->cbDelim->currentText();     //CSV file delimiter choice - "," or ";"
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
    //uint comp_method = GET_REG("SREGX052[comp_method]").toUInt();
    //uint comp_method = gp_DSI3Slave->Varcomp_method.value();
    uint comp_method = gp_DSI3Slave->Getcomp_method_old(GET_REG("SWriteMEASURE[mode]").toInt());

    if (arg.contains("SummaryToF"))
    {
        filename = QFileDialog::getSaveFileName(this, "Save CSV file",QDir::currentPath(), "CSV file(*.csv)");
        QFile file(filename);
        if (file.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream data( &file );

            data << "Date and time" << delim << formattedTime << "\n";
            data << "\n";

            data << "Echo" << delim;

            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1 - ToF").arg(m_meas_slots.at(slot)) << delim;
                data << QString("%1 - Peak").arg(m_meas_slots.at(slot)) << delim;
                data << QString("%1 - Width").arg(m_meas_slots.at(slot)) << delim;
            }
            data << "\n";

            for (row = 0; row < 16; row++)
            {
                data << QString("Echo %1").arg(row) << delim;

                for (slot = 0; slot < m_meas_slot_number; slot++)
                {
                    if (m_meas_tof[slot][TOF].size() > row)
                        data << QString("%1").arg(m_meas_tof[slot][TOF].at(row), 0, 'g', 4) << delim;
                    else
                        data << "" << delim;

                    if (m_meas_tof[slot][PEAK].size() > row)
                        data << QString("%1").arg(m_meas_tof[slot][PEAK].at(row), 0, 'g', 0) << delim;
                    else
                        data << "" << delim;

                    if (m_meas_tof[slot][WIDTH].size() > row)
                        data << QString("%1").arg(m_meas_tof[slot][WIDTH].at(row), 0, 'g', 3) << delim;
                    else
                        data << "" << delim;
                }
                data << "\n";
            }
            data << "\n";

            data << "Reverb. period" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg(((((uint)m_diagword1[slot].at(1))&0xff) << 2) + ((((uint)m_diagword1[slot].at(0))>>6)&0x03)) << delim;
            }
            data << "\n" << "Reverb. length" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg(((uint)m_diagword1[slot].at(0))&0x3f) << delim;
            }
            data << "\n" << "Sensor errors" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << ui->textEditSensorError->toPlainText().replace(',', '-').remove('\n') << delim;
            }
            data << "\n" << "Diagword0" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg(m_diagword0[slot].at(1) + (m_diagword0[slot].at(0) << 8)) << delim;
            }
            data << "\n" << "Die temp in dgC" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg(((uint)(m_diagword2[slot].at(1))&0xFF) - 55) << delim;
            }
            data << "\n";
            file.close();
            return true;
        }
        return false;

    }
    else if (arg.contains("Summary"))
    {
        filename = QFileDialog::getSaveFileName(this, "Save CSV file",QDir::currentPath(), "CSV file(*.csv)");
        QFile file(filename);
        if (file.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream data( &file );

            data << "Date and time" << delim << formattedTime << "\n";
            data << "\n";
            if (comp_method == 7)
            {
                data << "Freq [Hz]" << delim;
            }
            else
            {
                data << "Time [ms]" << delim;
            }
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << m_meas_slots.at(slot) << delim;
            }
            data << "\n";

            for (row = 0; row < m_meas_time.size(); row++)
            {
                //data << QString("%1").arg(row) << delim;

                if (comp_method == 7)
                {
                    data << QString("%1").arg(m_meas_freq.at(row)) << delim;
                }
                else
                {
                    data << QString("%1").arg(m_meas_time.at(row)) << delim;
                }

                for (slot = 0; slot < m_meas_slot_number; slot++)
                {
                    if (m_meas_samples[slot].size() > row)
                        data << QString("%1").arg(m_meas_samples[slot].at(row), 0, 'g', 3) << delim;
                    else
                        data << "" << delim;
                }
                data << "\n";
            }
            data << "\n";
            data << "Reverb. period" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg(((((uint)m_diagword1[slot].at(1))&0xff) << 2) + ((((uint)m_diagword1[slot].at(0))>>6)&0x03)) << delim;
            }
            data << "\n" << "Reverb. length" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg(((uint)m_diagword1[slot].at(0))&0x3f) << delim;
            }
            data << "\n" << "Sensor errors" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << ui->textEditSensorError->toPlainText().replace(',', '-').remove('\n') << delim;
            }
            data << "\n" << "Diagword0" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg(m_diagword0[slot].at(1) + (m_diagword0[slot].at(0) << 8)) << delim;
            }
            data << "\n" << "Die temp in dgC" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg((((uint)(m_diagword2[slot].at(1)))& 0xFF) - 55) << delim;
            }
            data << "\n";
            file.close();
            return true;
        }
        return false;
    }
    else if (arg.contains("IQ"))
    {
        filename = QFileDialog::getSaveFileName(this, "Save CSV file",QDir::currentPath(), "CSV file(*.csv)");
        QFile file(filename);
        if (file.open(QFile::WriteOnly | QFile::Truncate))
        {
            delim = ui->cbDelimIQ->currentText();     //CSV file delimiter choice - "," or ";"

            QTextStream data( &file );

            data << "Date and time" << delim << formattedTime << "\n";
            data << "\n";
            data << "Time [ms]" << delim;


            for (slot = 0; slot < m_meas_slots_iq.size(); slot++)
            {
                data << m_meas_slots_iq.at(slot) << delim;
            }
            data << "\n";

            for (row = 0; row < m_meas_time.size(); row++)
            {
                //data << QString("%1").arg(row) << delim;


                data << QString("%1").arg(m_meas_time.at(row)) << delim;


                for (slot = 0; slot < m_meas_slots_iq.size(); slot++)
                {
                    if (m_meas_iq[slot].size() > row)
                        data << QString("%1").arg(m_meas_iq[slot].at(row), 0, 'g', 3) << delim;
                    else
                        data << "" << delim;
                }
                data << "\n";
            }
            data << "\n";
            data << "Reverb. period" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg(((((uint)m_diagword1[slot].at(1))&0xff) << 2) + ((((uint)m_diagword1[slot].at(0))>>6)&0x03)) << delim << delim;
            }
            data << "\n" << "Reverb. length" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg(((uint)m_diagword1[slot].at(0))&0x3f) << delim << delim;
            }
            data << "\n" << "Sensor errors" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << ui->textEditSensorError->toPlainText().replace(',', '-').remove('\n') << delim << delim;
            }
            data << "\n" << "Diagword0" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg(m_diagword0[slot].at(1) + (m_diagword0[slot].at(0) << 8)) << delim << delim;
            }
            data << "\n" << "Die temp in dgC" << delim;
            for (slot = 0; slot < m_meas_slot_number; slot++)
            {
                data << QString("%1").arg((((uint)(m_diagword2[slot].at(1)))& 0xFF) - 55) << delim << delim;
            }
            data << "\n";
            file.close();
            return true;
        }
        return false;
    }
    else if (arg.contains("Detail1"))
        slot = m_graph_to_slot[GRAPH_DETAIL_SEL[0].toInt()];
    else if (arg.contains("Detail2"))
        slot = m_graph_to_slot[GRAPH_DETAIL_SEL[1].toInt()];
    else if (arg.contains("Overview"))
    {
        arg.remove("Overview");
        if ((arg.toInt() > 0) && (arg.toInt() < 9))
            slot = m_graph_to_slot[GRAPH_OVERVIEW_SEL[arg.toInt()-1].toInt()];
        else
        {
            gp_DSI3Master->Log(LOG_TYPE_ERROR, "Graph name not recognized");
            result = false;
        }
    }
    else
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "Graph name not recognized");
        result = false;
    }

    filename = QFileDialog::getSaveFileName(this, "Save CSV file",QDir::currentPath(), "CSV file(*.csv)");
    QFile file(filename);
    if (file.open(QFile::WriteOnly | QFile::Truncate))
    {
        QTextStream data( &file );
        data << "Date and time" << delim << formattedTime << "\n";
        data << "\n";
        data << "#" << delim << m_meas_slots.at(slot) << "\n";

        for (row = 0; row < m_meas_samples[slot].size(); row++)
        {
            data << QString("%1").arg(m_meas_time.at(row)) << delim << QString("%1").arg(m_meas_samples[slot].at(row), 0, 'g', 3) << "\n";
        }

        data << "Reverb. period" << delim << QString("%1").arg(((((uint)m_diagword1[slot].at(1))&0xff) << 2) + ((((uint)m_diagword1[slot].at(0))>>6)&0x03)) << "\n";
        data << "Reverb. length" << delim << QString("%1").arg(((uint)m_diagword1[slot].at(0))&0x3f) << "\n";
        data << "Sensor errors" << delim << ui->textEditSensorError->toPlainText().replace(',', '-').remove('\n') << "\n";
        data << "Diagword0" << delim << QString("%1").arg(m_diagword0[slot].at(1) + (m_diagword0[slot].at(0) << 8)) << "\n";
        data << "Die temp in dgC" << delim << QString("%1").arg((((uint)m_diagword2[slot].at(1)) & 0xFF) - 55) << "\n";


        file.close();
        return true;
    }
    return false;
}

void Measurement::SaveHistory()
{
    int slot = 0;
    int i, row;
    int index = 0;
    int channel = 0;
    QString channel_des[12] = {"1H", "1L", "2H", "2L", "3H", "3L", "4H", "4L", "5H", "5L", "6H", "6L"};
    bool result = true;
    QString filename = QFileDialog::getSaveFileName(this, "Save CSV file",QDir::currentPath(), "CSV file(*.csv)");
    QString filename_mod;
    QString delim = ",";
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
    QVector<int> channel_history_indexes(1024);
    int channel_max_time = 0;

    for (channel = 0; channel < 12; channel++)
    {
        channel_history_indexes.clear();
        channel_max_time = 0;
        for (i = 0; i < ui->lineEditHistoryCounter->text().toInt(); i++)
        {
            index = (m_history_pointer_start+1+i) % 1000;
            slot = m_graph_to_slothistory[index][channel];
            if ((slot >= 0) && (slot <= MAX_SLOT_NUMBER))
            {
                channel_history_indexes.append(index);
                if (channel_max_time < m_meas_sampleshistory[index][slot].size())
                {
                    channel_max_time = m_meas_sampleshistory[index][slot].size();
                }
            }
        }

        if (channel_history_indexes.size() > 0)
        {
            filename_mod = filename;
            filename_mod.remove(".csv");
            filename_mod.append(QString("_%1.csv").arg(channel_des[channel]));
            QFile file(filename_mod);
            if (file.open(QFile::WriteOnly | QFile::Truncate))
            {
                QTextStream data( &file );

                data << "Date and time" << delim << formattedTime << "\n";
                data << "\n";
                data << "Time [ms]" << delim;

                for (i = 0; i < channel_history_indexes.size(); i++)
                {
                    data << QString("His%1").arg(i)<< delim;
                }
                data << "\n";
                for (row = 0; row < channel_max_time; row++)
                {
                    data << QString("%1").arg(m_meas_time.at(row)) << delim;
                    for (i = 0; i < channel_history_indexes.size(); i++)
                    {
                        slot = m_graph_to_slothistory[channel_history_indexes.at(i)][channel];
                        if (m_meas_sampleshistory[channel_history_indexes.at(i)][slot].size() > row)
                            data << QString("%1").arg(m_meas_sampleshistory[channel_history_indexes.at(i)][slot].at(row), 0, 'g', 3) << delim;
                        else
                            data << "" << delim;
                    }
                    data << "\n";
                }

                data << "\n"  << "Reverb. period" << delim;
                for (i = 0; i < channel_history_indexes.size(); i++)
                {
                    slot = m_graph_to_slothistory[channel_history_indexes.at(i)][channel];
                    data << QString("%1").arg(((((uint)m_diagword1history[channel_history_indexes.at(i)][slot].at(1))&0xff) << 2) + ((((uint)m_diagword1history[channel_history_indexes.at(i)][slot].at(0))>>6)&0x03)) << delim;
                }
                data << "\n" << "Reverb. length" << delim;
                for (i = 0; i < channel_history_indexes.size(); i++)
                {
                    slot = m_graph_to_slothistory[channel_history_indexes.at(i)][channel];
                    data << QString("%1").arg(((uint)m_diagword1history[channel_history_indexes.at(i)][slot].at(0))&0x3f) << delim;
                }
                data << "\n" << "Sensor errors" << delim;
                for (i = 0; i < channel_history_indexes.size(); i++)
                {
                    data << m_sensorstatushistory[channel_history_indexes.at(i)].replace(',', '-').remove('\n') << delim;
                }
                data << "\n" << "Diagword0" << delim;
                for (i = 0; i < channel_history_indexes.size(); i++)
                {
                    slot = m_graph_to_slothistory[channel_history_indexes.at(i)][channel];
                    data << QString("%1").arg(m_diagword0history[channel_history_indexes.at(i)][slot].at(1) + (m_diagword0history[channel_history_indexes.at(i)][slot].at(0) << 8)) << delim;
                }
                data << "\n";


                file.close();
            }
        }
    }
}
/*
void Measurement::SaveHistory()
{
    int slot = 0;
    int i, row;
    int index = 0;
    int channel = 0;
    QString channel_des[12] = {"1H", "1L", "2H", "2L", "3H", "3L", "4H", "4L", "5H", "5L", "6H", "6L"};
    bool result = true;
    QString filename = QFileDialog::getSaveFileName(this, "Save CSV file",QDir::currentPath(), "CSV file(*.csv)");
    QString filename_mod;
    QString delim = ",";
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
    QByteArray channel_history_indexes;
    int channel_max_time = 0;

    for (channel = 0; channel < 12; channel++)
    {
        channel_history_indexes.clear();
        channel_max_time = 0;
        for (i = 0; i < ui->lineEditHistoryCounter->text().toInt(); i++)
        {
            index = (m_history_pointer_start+1+i) % 1000;
            slot = m_graph_to_slothistory[index][channel];
            if ((slot >= 0) && (slot <= MAX_SLOT_NUMBER))
            {
                channel_history_indexes.append(index);
                if (channel_max_time < m_meas_sampleshistory[index][slot].size())
                {
                    channel_max_time = m_meas_sampleshistory[index][slot].size();
                }
            }
        }

        if (channel_history_indexes.size() > 0)
        {
            filename_mod = filename;
            filename_mod.remove(".csv");
            filename_mod.append(QString("_%1.csv").arg(channel_des[channel]));
            QFile file(filename_mod);
            if (file.open(QFile::WriteOnly | QFile::Truncate))
            {
                QTextStream data( &file );

                data << "Date and time" << delim << formattedTime << "\n";
                data << "\n";
                data << "Time [ms]" << delim;

                for (i = 0; i < channel_history_indexes.size(); i++)
                {
                    data << QString("His%1").arg(i)<< delim;
                }
                data << "\n";
                for (row = 0; row < channel_max_time; row++)
                {
                    data << QString("%1").arg(m_meas_time.at(row)) << delim;
                    for (i = 0; i < channel_history_indexes.size(); i++)
                    {
                        slot = m_graph_to_slothistory[channel_history_indexes.at(i)][channel];
                        if (m_meas_sampleshistory[channel_history_indexes.at(i)][slot].size() > row)
                            data << QString("%1").arg(m_meas_sampleshistory[channel_history_indexes.at(i)][slot].at(row), 0, 'g', 3) << delim;
                        else
                            data << "" << delim;
                    }
                    data << "\n";
                }

                data << "\n"  << "Reverb. period" << delim;
                for (i = 0; i < channel_history_indexes.size(); i++)
                {
                    slot = m_graph_to_slothistory[channel_history_indexes.at(i)][channel];
                    data << QString("%1").arg(((((uint)m_diagword1history[channel_history_indexes.at(i)][slot].at(1))&0xff) << 2) + ((((uint)m_diagword1history[channel_history_indexes.at(i)][slot].at(0))>>6)&0x03)) << delim;
                }
                data << "\n" << "Reverb. length" << delim;
                for (i = 0; i < channel_history_indexes.size(); i++)
                {
                    slot = m_graph_to_slothistory[channel_history_indexes.at(i)][channel];
                    data << QString("%1").arg(((uint)m_diagword1history[channel_history_indexes.at(i)][slot].at(0))&0x3f) << delim;
                }
                data << "\n" << "Sensor errors" << delim;
                for (i = 0; i < channel_history_indexes.size(); i++)
                {
                    data << m_sensorstatushistory[channel_history_indexes.at(i)].replace(',', '-').remove('\n') << delim;
                }
                data << "\n" << "Diagword0" << delim;
                for (i = 0; i < channel_history_indexes.size(); i++)
                {
                    slot = m_graph_to_slothistory[channel_history_indexes.at(i)][channel];
                    data << QString("%1").arg(m_diagword0history[channel_history_indexes.at(i)][slot].at(1) + (m_diagword0history[channel_history_indexes.at(i)][slot].at(0) << 8)) << delim;
                }
                data << "\n";


                file.close();
            }
        }
    }
}
*/
void Measurement::slot_EnableHistoryUpdate()
{
    bool check;
    if (ui->checkBoxEnableHistory->isChecked())
    {
        check = true;
    }
    else
    {
        ui->checkBoxPersistence->setChecked(false);
        check = false;
        slot_PersistenceUpdate();
        ClearHistory();
    }
    ui->checkBoxPersistence->setEnabled(check);
    ui->lineEditHistoryCounter->setEnabled(check);
    ui->lineEditHistoryIndex->setEnabled(check);
    ui->pushButtonClearHistory->setEnabled(check);
    ui->pushButtonSaveHistory->setEnabled(check);
    ui->pushButtonShowResultsOf->setEnabled(check);
}

void Measurement::slot_PersistenceUpdate()
{
    SwitchPersistence(ui->checkBoxPersistence->isChecked());
}

void Measurement::CFARinGUI()
{
    bool ena = GET_REG("CFAR_SW_ENA").toInt();

    if (ena == false) {
        return;
    }

    QVector<double> CFAR_MAG;   //CFAR_MAG(1024)
    QVector<double> ECHO_DET;
    QVector<double> DBG[10];

    int slot;
    uint slot_ena = APPLY_CFAR.toUInt();

    quint8 offset = GET_REG("CFAR_THR_OFF").toUInt();
    quint8 der_thr = GET_REG("CFAR_DER_THR").toUInt();
    quint8 der_size = (GET_REG("CFAR_DER_SIZE").toUInt() + 1) * 2;
    quint8 scale = GET_REG("CFAR_SCALE").toUInt();
    quint8 snr = GET_REG("CFAR_SNR").toUInt();
    quint8 MV_AVG_samples = 24;         //Moving average sample count
    quint8 SNR_debounce = 2;            //Number of samples under SNR threshold necessary to detect falling edge of echo
    m_TOF_DIFF.clear();
    m_TOF_DIFF.resize(m_meas_slot_number);
    m_TOF_SNR.clear();
    m_TOF_SNR.resize(m_meas_slot_number);
    m_TOF_ECHO_DET.clear();
    m_TOF_ECHO_DET.resize(m_meas_slot_number);
    m_TOF_SAMPLES.clear();
    m_TOF_SAMPLES.resize(m_meas_slot_number);

    if(m_meas_time.size() > 2) {
        m_sampling_period = (m_meas_time.at(1)-m_meas_time.at(0));
        qDebug() << "m_sampling_period:" << m_sampling_period;
    }
    else {
        return; //error, not enough data to calculate cfar
    }

    m_CFAR_START_SAMPLE = qRound(CFAR_STARTMS.toDouble()/m_sampling_period);

    for (slot = 0; slot < m_meas_slot_number; slot++)
    {
        CFAR_MAG.resize(m_meas_samples[slot].size());
        ECHO_DET.resize(m_meas_samples[slot].size());
        if (((slot_ena >> slot) & 0x01) == 0x01)        //if CFAR calculation is enabled
        {
            CalculateCFAR(m_meas_samples[slot], der_size, scale, offset, der_thr, snr, CFAR_MAG, ECHO_DET, MV_AVG_samples, SNR_debounce,
                        DBG[0], DBG[1], DBG[2], DBG[3], DBG[4], DBG[5], DBG[6], DBG[7], DBG[8], DBG[9], m_CFAR_START_SAMPLE);

            m_TOF_SAMPLES[slot] = m_meas_samples[slot]; //Backup the original input data for ToF detection
            m_meas_samples[slot] = ECHO_DET;            //Replace original input data with CFAR result
            m_TOF_ECHO_DET[slot] = ECHO_DET;
            m_TOF_DIFF[slot] = DBG[2];                  //ToF diff signal - always copy, used for multipeak TOF detection
            m_TOF_SNR[slot] = DBG[6];                   //SNR signal - always copy, used to report Peak SNR value (ToF detection)
        }
    }
}

/*****************************************************************************/
//Calculate TOF from samples
//06/2021 new version - using differentiation signal m_TOF_DIFF[channel]
void Measurement::CalculateTOF(void) {
    m_meas_tof_ena = false;     // DEBUG - VYRADI NEFUNGUJICI ZOBRAZOVANI, ALE MELO BY BYT TRUE
    for(int i=0; i<MAX_SLOT_NUMBER; i++) {
        m_meas_tof[i][TOF].clear();
        m_meas_tof[i][PEAK].clear();
        m_meas_tof[i][WIDTH].clear();
        m_meas_tof[i][PEAK_SNR].clear();
    }
    bool ena = GET_REG("CFAR_SW_ENA").toInt();
    if (ena == false)
    {
        return;
    }
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

    sample_period = m_sampling_period;

    //qDebug() << "m_TOF_DIFF" << m_TOF_DIFF;
    //qDebug() << "m_TOF_DIFF.size" << m_TOF_DIFF.size();
    //qDebug() << "sim_channel_count" << sim_channel_count;

    //new
    for(uint channel=0; channel < m_meas_slot_number; channel++) {
        bool max_det_shift = 0;
        sample = 0;
        maxi = 0;           //max. value of echo
        maxi_SNR = 0;       //value of SNR in peak maximum
        Redge_index = 0;    //when did the rising edge start
        Redge = -1;         //-1 = init before echo, 0 = falling edge detected, 1 = rising edge detected
        samplecount = m_TOF_ECHO_DET[channel].size(); //number of samples   //m_TOF_DIFF[channel].size()
        m_meas_tof[channel][TOF].clear();
        m_meas_tof[channel][PEAK].clear();
        m_meas_tof[channel][WIDTH].clear();
        m_meas_tof[channel][PEAK_SNR].clear();
        echocount = 0;
        qDebug() << "- - - Channel" << channel << "- - -";
        qDebug() << "m_meas_slot_number" << m_meas_slot_number;
        qDebug() << "m_TOF_ECHO_DET[" << channel << "].size =" << m_TOF_ECHO_DET[channel].size();
        qDebug() << "m_TOF_DIFF[" << channel << "].size =" << m_TOF_DIFF[channel].size();

        for(sample = 0; sample<samplecount; sample++) {
            if(echocount >= max_echo_cnt) {    //Reached Maximum number of reported echoes
                break;
            }
            if(m_TOF_ECHO_DET[channel].at(sample) == 0 && (Redge == 0 || Redge == 1)) {
                //report previous echo's width, peak etc... reporting width as: rising edge 1st echo to rising edge 2nd echo
                if(maxi == m_TOF_SAMPLES[channel].at(Redge_index) && max_det_shift == 1) {
                    //qDebug() << "Redge index -1";
                    Redge_index--;  //peak was at a point previous to Redge detection, shift by one sample
                    max_det_shift = 0;
                    //If we expand this echo width by one sample, we must subtract it from the previous one (only if we dont eliminate the previous echo completely)
                    if(m_meas_tof[channel][WIDTH].size()>0) {
                        if(m_meas_tof[channel][WIDTH].last()-sample_period >= sample_period) {
                           m_meas_tof[channel][WIDTH][m_meas_tof[channel][WIDTH].size()-1] = m_meas_tof[channel][WIDTH].last()-sample_period;
                        }
                    }
                }
                m_meas_tof[channel][TOF].append((Redge_index*sample_period)-TOF_offset);
                m_meas_tof[channel][PEAK].append(maxi);
                m_meas_tof[channel][WIDTH].append((sample-Redge_index)*sample_period);
                m_meas_tof[channel][PEAK_SNR].append(maxi_SNR);
                echocount++;
                Redge = -1;
                max_det_shift = 0;
                qDebug() << "Echo end at" << sample << "which is" << (sample*sample_period)-TOF_offset << "ms";
            }
            //Rising edge
            if(m_TOF_ECHO_DET[channel].at(sample) != 0) {
                if((m_TOF_DIFF[channel].at(sample) >= der_thr_pos) && Redge == 0) {        //multipeak echo
                    qDebug() << "TOF R.E. at" << sample << "which is" << (sample*sample_period)-TOF_offset << "ms";
                    //report previous echo's width, peak etc... reporting width as: rising edge 1st echo to rising edge 2nd echo
                    if(maxi == m_TOF_SAMPLES[channel].at(Redge_index) && max_det_shift == 1) {
                        //qDebug() << "Redge index -1";
                        Redge_index--;  //peak was at a point previous to Redge detection, shift by one sample
                        max_det_shift = 0;
                        //If we expand this echo width by one sample, we must subtract it from the previous one (only if we dont eliminate the previous echo completely)
                        if(m_meas_tof[channel][WIDTH].size()>0) {
                            if(m_meas_tof[channel][WIDTH].last()-sample_period >= sample_period) {
                               m_meas_tof[channel][WIDTH][m_meas_tof[channel][WIDTH].size()-1] = m_meas_tof[channel][WIDTH].last()-sample_period;
                            }
                        }
                    }
                    m_meas_tof[channel][TOF].append((Redge_index*sample_period)-TOF_offset);
                    m_meas_tof[channel][PEAK].append(maxi);
                    m_meas_tof[channel][WIDTH].append((sample-Redge_index)*sample_period);
                    m_meas_tof[channel][PEAK_SNR].append(maxi_SNR);
                    echocount++;
                    Redge = 1;
                    Redge_index = sample;
                    maxi = 0;
                    max_det_shift = 0;
                }
                else if(Redge == -1) {  //first rising edge of echo
                    qDebug() << "\nTOF first R.E. at" << sample << "which is" << (sample*sample_period)-TOF_offset << "ms";
                    Redge = 1;
                    Redge_index = sample;
                    maxi = 0;
                }

            }
            //Find peak value
            if(m_TOF_ECHO_DET[channel].at(sample) != 0 && m_TOF_SAMPLES[channel].at(sample) > maxi) {
                maxi = m_TOF_SAMPLES[channel].at(sample);
                maxi_SNR = m_TOF_SNR[channel].at(sample);

                if(sample > 0 && maxi == 0 && m_TOF_SAMPLES[channel].at(sample-1) > m_TOF_SAMPLES[channel].at(sample)) {    //very steep edges can cause 1 sample delay in Rising edge detection
                    maxi = m_TOF_SAMPLES[channel].at(sample-1);                                                             //when rising edge is detected, check if we didnt miss peak by 1 sample
                    maxi_SNR = m_TOF_SNR[channel].at(sample-1);
                    max_det_shift = 1;
                }
                else {
                    maxi = m_TOF_SAMPLES[channel].at(sample);
                    maxi_SNR = m_TOF_SNR[channel].at(sample);
                }
            }
            //Falling edge
            if((Redge == 1 && (m_TOF_DIFF[channel].at(sample) <= -der_thr_neg))) {             //diff signal crosses negative diff. threshold OR   - multipeak echo F.Edge
                Redge = 0;
            }
        }
    }
}

/*****************************************************************************/
void Measurement::ToFinGUI()
{
    int sample = 1;
    int slot, i;
    int maxi = 0;
    int samplecount = 0;
    int echocount = 0;
    double sample_period = 0.1024;
    double TOF_offset = GET_REG("TOF_OFFSET").toDouble();
    //double TOF_offset = 0;

    if (m_meas_time.size() > 2)
    {
        sample_period = m_meas_time.at(1);
    }

    //int echo_type = GET_REG("SREGX056[echo_type]").toInt();
    /*int echo_type = 0;
    if (echo_type <= 3)
    {
        m_meas_tof_ena = false;
        return;
    }*/
    //m_meas_tof_ena = true;
    m_meas_tof_ena = false;     // DEBUG - VYRADI NEFUNGUJICI ZOBRAZOVANI, ALE MELO BY BYT TRUE

    for (slot = 0; slot < m_meas_slot_number; slot++)
    {
        samplecount = m_meas_samples[slot].size();
        echocount = 0;
        sample = 1;
        m_meas_tof[slot][TOF].clear();
        m_meas_tof[slot][PEAK].clear();
        m_meas_tof[slot][WIDTH].clear();
        while (sample < samplecount)
        {
            if ((m_meas_samples[slot].at(sample-1) == 0) && (m_meas_samples[slot].at(sample) > 0))  //echo rising edge detection
            {
                maxi = 0;
                //qDebug() << "Echo begin at index: " << sample;
                for (i = 0; (sample+i+1) < samplecount; i++)
                {
                    if (m_meas_samples[slot].at(sample+i) > maxi)
                    {
                        maxi = m_meas_samples[slot].at(sample + i);
                    }
                    if ((m_meas_samples[slot].at(sample + i) > 0) && (m_meas_samples[slot].at(sample + i + 1) == 0) && (echocount < 16))  //echo falling edge detection
                    {                                                                                                                     //maximum 16 reported echoes
                        /*qDebug() << "Echo falling edge at index: " << sample + i + 1;
                        qDebug() << "TOF:   " << (sample*sample_period)-TOF_offset;
                        qDebug() << "PEAK:  " << maxi;
                        qDebug() << "WIDTH: " << (i+1)*sample_period;*/

                        if ((sample*sample_period - TOF_offset) > 0)
                        {
                            m_meas_tof[slot][TOF].append((sample*sample_period)-TOF_offset);
                            m_meas_tof[slot][PEAK].append(maxi);
                            m_meas_tof[slot][WIDTH].append((i+1)*sample_period);
                            echocount++;
                        }
                        sample = sample + i;
                        /*qDebug() << "ECHO COUNT: " << echocount;
                        qDebug() << "_____________________";*/
                        break;
                    }
                    else if((sample+i+2) >= samplecount && (echocount < 16)) {      //No falling edge until end of measurement
                        if ((sample*sample_period - TOF_offset) > 0)
                        {
                            m_meas_tof[slot][TOF].append((sample*sample_period)-TOF_offset);
                            m_meas_tof[slot][PEAK].append(maxi);
                            m_meas_tof[slot][WIDTH].append((i+1)*sample_period);
                            echocount++;
                        }
                        sample = sample + i;
                        break;
                    }
                }
            }
            sample++;
        }
    }
    return;
}

void Measurement::slot_TWUpdate(void)
{
    if (ui->tabWidget->currentIndex() == TW_SUMMARY)
    {
        UpdateSummaryGraphFormat();
    }
    else if (ui->tabWidget->currentIndex() == TW_DETAIL)
    {
        UpdateDetailGraph();
    }
    else if (ui->tabWidget->currentIndex() == TW_OVERVIEW)
    {
        UpdateOverviewGraph();
    }
}

/*****************************************************************************/
/*                 CFAR FUNCTION - new separated Mag/TOF CFAR                */
/*****************************************************************************/
void Measurement::CalculateCFAR(QVector<double> samples, quint8 der_size, double scale, quint8 offset, int der_thr, \
                            int CFAR_SNR_THRESHOLD, QVector<double> &CFAR_MAG, QVector<double> &ECHO_DET, quint8 MVA_samples,\
                            quint8 SNR_debounce, QVector<double> &DBG1, QVector<double> &DBG2, QVector<double> &DBG3, \
                            QVector<double> &DBG4, QVector<double> &DBG5, QVector<double> &DBG6, QVector<double> &DBG7, \
                            QVector<double> &DBG8, QVector<double> &DBG9, QVector<double> &DBG10, quint32 CFAR_START_SAMPLE)
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
        DBG1.append(min_cfar_thr_shifted.at(i));    //cfar threshold
        DBG2.append(MVAVG.at(i)*pow(10,(1.0*CFAR_SNR_THRESHOLD)/20));   //SNR threhold
        DBG3.append(DIFF_TOF_shifted.at(i));        //MP_DIFF
        DBG4.append(over_min_cfar_thr.at(i));       //over_thr
        DBG5.append(over_SNR_thr.at(i));            //over_snr
        DBG6.append(over_diff_thr.at(i));           //over_diff
        DBG7.append(SNR_curve.at(i));               //snr curve
        DBG8.append(DIFF_shifted.at(i));            //DIFF
        DBG9.append(DIFF_shifted.at(i));            //DIFF
        DBG10.append(DIFF_TOF_shifted.at(i));       //MP_DIFF
    }
    return;
}

/*************************************************/
double Measurement::QVectorMin(QVector<double> vec)
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
double Measurement::QVectorMax(QVector<double> vec)
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
double Measurement::QVectorAvg(QVector<double> vec)
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

void Measurement::UpdateMasterSettings()
{
    if (GET_REG("UPDATE_MASTER").toInt() == 1)
    {
        int num_of_pdcm_slots = m_meas_slots.length();
        int brcs = 0;
        bool update = false;
        quint16 mag_data_size;
        quint16 max_mag_data_size = 0;
        quint8 slot;
        QString num;
        uint meas_type;
        uint comp_method;
        uint meas_duration;
        uint compr_timer;
        QStringList regs;

        if (GET_REG("SWriteMEASURE[mode]").toInt() == 0)
        {
            //meas_type = GET_REG("SREGX042[meas_type]").toUInt();
            meas_type = gp_DSI3Slave->Getmeas_type_old(0);
            //comp_method = gp_DSI3Slave->Varcomp_method.value();
            comp_method = gp_DSI3Slave->Getcomp_method_old(0);

            if (gp_DSI3Slave->m_memory == 0)
            {
                meas_duration = GET_REG("SREGX058[meas_duration]").toUInt();
                compr_timer = 0;
            }
            else if (gp_DSI3Slave->m_memory == 1)
            {
                if ((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
                {
                    meas_duration = GET_REG("SREGXV204A[7:0]").toUInt();
                }
                else
                {
                    meas_duration = GET_REG("SREGXV204A[meas_duration]").toUInt();
                }

                compr_timer = GET_REG("SREGXV205E[8:6]").toUInt();
            }
        }
        else
        {
            if (GET_REG("SWriteMEASURE[mode]").toInt() > 5)
            {
                SET_REG("SWriteMEASURE[mode]", 5);
            }

            meas_type = gp_DSI3Slave->Getmeas_type_old(GET_REG("SWriteMEASURE[mode]").toInt());
            comp_method = gp_DSI3Slave->Getcomp_method_old(GET_REG("SWriteMEASURE[mode]").toInt());


            if (gp_DSI3Slave->m_memory == 0)
            {
                meas_duration = GetSlaveReg(0, 1, 0x58 - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[meas_duration]");
                compr_timer = 0;
            }
            else if (gp_DSI3Slave->m_memory == 1)
            {
                if ((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
                {
                    meas_duration = GetSlaveReg(1, 1, 0x4A - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[7:0]");
                }
                else
                {
                    meas_duration = GetSlaveReg(1, 1, 0x4A - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[meas_duration]");
                }

                compr_timer = GetSlaveReg(1, 1, 0x5E - 0x40 + mode_start_addr[GET_REG("SWriteMEASURE[mode]").toInt()], "[8:6]");
            }
        }

        for (slot = 0; slot < num_of_pdcm_slots; slot++)
        {
            mag_data_size = get_diag_position(meas_type, comp_method, meas_duration, true, 0, compr_timer);

            if ((gp_DSI3Master->MVar_version == VERSION_RDUSDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS) || (gp_DSI3Master->MVar_version == VERSION_RDUS2DMX)\
                    || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH) || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD)\
                    || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
            {
                mag_data_size = mag_data_size + 2;
            }


            if (mag_data_size > max_mag_data_size)
                max_mag_data_size = mag_data_size;
        }

        brcs = (max_mag_data_size + 15) / 16;



        if (gp_Settings->mVersion == VERSION_UPID)
        {
            gp_UartSpiController->BRC_BRC.setValue(brcs);
            gp_UartSpiController->BRC_SLOTS.setValue(num_of_pdcm_slots);
        }
        else if ((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
        {
            gp_UartSsiController->SSI_BRC_COUNT.setValue(brcs);
            gp_UartSsiController->SSI_SLOT_COUNT.setValue(num_of_pdcm_slots);
        }
        else
        {
            if ((int)gp_DSI3Master->CHANNEL.value() == 0)
            {
                if (brcs != (GET_REG("MREG02[BRC_CNT]").toInt()))
                {
                    SET_REG("MREG02[BRC_CNT]", brcs);
                    regs.append("MREG02");
                    update = true;
                }
            }
            else
            {
                if (brcs != (GET_REG("MREG03[BRC_CNT]").toInt()))
                {
                    SET_REG("MREG03[BRC_CNT]", brcs);
                    regs.append("MREG03");
                    update = true;
                }
            }

            if ((num_of_pdcm_slots - 1) != (GET_REG("MREG10[NUM_OF_PDCM_SLOTS]").toInt()))
            {
                SET_REG("MREG10[NUM_OF_PDCM_SLOTS]", num_of_pdcm_slots - 1);
                regs.append("MREG10");
                update = true;
            }

            if (update)
            {
                gp_DSI3Master->PerformWRITE(regs);
            }
        }


    }
}

/*************************************************/
void Measurement::get_mag_data_info(t_MAG_DATA_INFO *mag_data_info)
{
    const quint8 BEAMING_DELAY_DEF_TOF = 2;                     // 100 us default beaming delay
    const quint8 POWER_PHASE_TOF = 176 - BEAMING_DELAY_DEF_TOF; // 9 ms in TOF, unit [51.2us], minus Beaming Prepare state
    const quint8 COMP_EXT_START_TOF = 144;                      // Magnitude data logging starts this number of TOF before beaming end (beaming delay excluded)
    const quint8 COMP_EXT_START_PWR_TOF = 108;                  // Magnitude data logging starts this number of TOF before beaming end (beaming delay excluded) in case of Test compression method and power class configuration
    const quint8 TEMP_ABIST_TOF = 30;
    const quint8 DIAG_CHIRP_TOF = 157;                          // Diagnostic chirp length in TOF
    const quint8 DIAG_BURST_TOF = 162;                          // Diagnostic acoustic bursts length in TOF
    const quint8 DSP_DIAGW_SIZE = 6;

    const quint16 m_tof_lut[64] = {195, 210, 225, 239, 254, 269, 283, 298, 313, 327, 342, 356, 371, 386, 400, 415,\
                                   430, 444, 459, 474, 488, 503, 518, 532, 547, 562, 576, 591, 605, 620, 635, 649,\
                                   664, 679, 693, 708, 723, 737, 752, 767, 781, 796, 811, 825, 840, 854, 869, 884,\
                                   898, 913, 928, 942, 957, 972, 986, 1001, 1016, 1030, 1045, 1060, 1074, 1089, 1104, 1118};
    const quint16 beaming_tof_lut[32] = {2, 1, 1, 2, 7, 8, 9, 10, 7, 8, 9, 10, 0, 0, 0, 0,\
                                         49, 49, 0, 0, 49, 49, 49, 49, 49, 31, 157, 157, 157, 162, 49, 6};
    const quint16 beaming_tof_lut_RDUS2[32] = {4, 1, 2, 4, 7, 8, 9, 5, 7, 8, 9, 10, 49, 49, 0, 0,\
                                         49, 49, 0, 0, 49, 49, 49, 49, 49, 31, 157, 157, 157, 157, 49, 6};
    quint16 tank_charge_time_tof = 0;

    bool wbn_present = false;

    //defaults
    mag_data_info->block_size = 0;
    mag_data_info->block_sample_count = 0;
    mag_data_info->sample_count = 0;
    mag_data_info->block_count = 0;
    mag_data_info->meas_dur_tof = 0;
    mag_data_info->beaming_tof = 0;
    mag_data_info->mag_data_size = 0;

    //measurement duration
    //mag_data_info->meas_dur_tof = m_tof_lut[mag_data_info->meas_duration];
    mag_data_info->meas_dur_tof = (quint16) (mag_data_info->meas_duration * 10);


    // beaming size
    if ((gp_DSI3Master->MVar_version == VERSION_RDUS2DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH)\
             || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
    {
        mag_data_info->beaming_tof = beaming_tof_lut_RDUS2[mag_data_info->meas_type];
    }
    else
    {
        mag_data_info->beaming_tof = beaming_tof_lut[mag_data_info->meas_type];
    }

    // sample count (unit 51.2 us)
    mag_data_info->sample_count = mag_data_info->meas_dur_tof;
    if ((mag_data_info->selct_comp_res == 3) || (mag_data_info->selct_comp_res == 4)|| (mag_data_info->selct_comp_res == 15))
        mag_data_info->sample_count = mag_data_info->sample_count - mag_data_info->beaming_tof;

    if (mag_data_info->selct_comp_res == 0)
    {
        //102.4 us samples, 2.5 bit, compressed-concatenated
        mag_data_info->sample_count = (mag_data_info->sample_count - 1) / 2;        // conver to 102.4 us samples
        mag_data_info->block_sample_count = 8;
        mag_data_info->block_count = (mag_data_info->sample_count + 15) / 16;       // 16 samples per 2 blocks, round up to even number of blocks
        mag_data_info->block_count = mag_data_info->block_count * 2;
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;   // 8 samples per block
        mag_data_info->block_size = 3;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + 4;
    }
    else if ((mag_data_info->selct_comp_res >= 1) && (mag_data_info->selct_comp_res <= 3))
    {
        // 102.4 us samples, 3 bit, compressed
        if (mag_data_info->selct_comp_res == 3)
        {
            if (COMP_EXT_START_TOF > mag_data_info->beaming_tof)
                mag_data_info->sample_count = mag_data_info->sample_count + COMP_EXT_START_TOF;
            else
                mag_data_info->sample_count = mag_data_info->sample_count + mag_data_info->beaming_tof;
        }
        mag_data_info->sample_count = (mag_data_info->sample_count - 1) / 2;        // conver to 102.4 us samples     97
        mag_data_info->block_sample_count = 9;
        mag_data_info->block_count = (mag_data_info->sample_count + 8) / 9;         // 9 samples per block, round up
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;
        mag_data_info->block_size = 4;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + 4;
    }
    else if (mag_data_info->selct_comp_res == 4)
    {
        // 51.2 us samples, 8 bit, not compressed

        if((mag_data_info->meas_type >= 26) && (mag_data_info->meas_type <= 28))
        {
            mag_data_info->sample_count = mag_data_info->sample_count + DIAG_CHIRP_TOF;
        }
        else if(mag_data_info->meas_type == 29)
        {
            mag_data_info->sample_count = mag_data_info->sample_count + DIAG_BURST_TOF;
        }
        else
        {
            tank_charge_time_tof = mag_data_info->tank_charge_time;

            if ((mag_data_info->tank_charge_time > 6) && (mag_data_info->beaming_tof >= 32))
            {
                tank_charge_time_tof = 6;
            }

            tank_charge_time_tof = tank_charge_time_tof * 16;

            if (tank_charge_time_tof < TEMP_ABIST_TOF)
                tank_charge_time_tof = TEMP_ABIST_TOF;

            mag_data_info->sample_count = mag_data_info->sample_count + (POWER_PHASE_TOF - tank_charge_time_tof);
        }
        mag_data_info->block_count = (mag_data_info->sample_count + 1) / 2;         // 2 samples per block, round up
        mag_data_info->block_sample_count = 2;
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;
        mag_data_info->block_size = 2;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + 4;
    }
    else if ((mag_data_info->selct_comp_res == 5) || (mag_data_info->selct_comp_res == 7))
    {
        mag_data_info->block_size = 2;
        mag_data_info->block_sample_count = 4;
        mag_data_info->sample_count = 76 + 2;
        mag_data_info->block_count = 19;
        mag_data_info->meas_dur_tof = DIAG_CHIRP_TOF;
        mag_data_info->beaming_tof = DIAG_CHIRP_TOF;
        mag_data_info->mag_data_size = 40 + 4;
    }
    else if (mag_data_info->selct_comp_res == 6)
    {
        // 51.2 us samples, 3 bit, compressed
        mag_data_info->block_sample_count = 9;
        mag_data_info->block_count = (mag_data_info->sample_count + 8 - 1) / 9;
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;
        mag_data_info->block_size = 4;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + 4;
    }   
    else if ((mag_data_info->selct_comp_res >= 14) && (mag_data_info->selct_comp_res <= 15))
    {
        // 102.4 us samples, 3 bit, compressed
        if (mag_data_info->selct_comp_res == 15)
        {
            if (COMP_EXT_START_TOF > mag_data_info->beaming_tof)
                mag_data_info->sample_count = mag_data_info->sample_count + COMP_EXT_START_TOF;
            else
                mag_data_info->sample_count = mag_data_info->sample_count + mag_data_info->beaming_tof;
        }
        mag_data_info->sample_count = (mag_data_info->sample_count - 1) / 2;        // conver to 102.4 us samples     97
        mag_data_info->block_sample_count = 9;
        mag_data_info->block_count = (mag_data_info->sample_count + 8) / 9;         // 9 samples per block, round up
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;
        mag_data_info->block_size = 4;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + 4;

        mag_data_info->wbn_count = (mag_data_info->block_count + 7) / 8;
        mag_data_info->wbn_data_size = 8 * mag_data_info->block_size;
        mag_data_info->wbn_sample_count_2 = 9;

        mag_data_info->mag_data_size = mag_data_info->mag_data_size + 2 * mag_data_info->wbn_count;

    }

    if ((mag_data_info->meas_type >= 20) && (mag_data_info->meas_type <= 24))
    {
        if((GET_REG("SREGX05E[short_range_thr]").toUInt() & 0x01) != 0)
        {
            mag_data_info->mag_data_size = mag_data_info->mag_data_size + 16;
        }

    }

    //qDebug() << QString("%1").arg(mag_data_info->mag_data_size);

    /*if (GET_REG("NearRange").toInt() != 0)
    {
        if (((mag_data_info->selct_comp_res >= 0) && (mag_data_info->selct_comp_res <= 3)) || (mag_data_info->selct_comp_res == 6))
        {
            mag_data_info->mag_data_size = mag_data_info->mag_data_size + 16;
        }

    }*/
}

/*************************************************/
void Measurement::get_mag_data_info3(t_MAG_DATA_INFO *mag_data_info)
{
    const quint8 BEAMING_DELAY_DEF_TOF = 2;                     // 100 us default beaming delay
    const quint8 POWER_PHASE_TOF = 176 - BEAMING_DELAY_DEF_TOF; // 9 ms in TOF, unit [51.2us], minus Beaming Prepare state
    const quint8 COMP_EXT_START_TOF = 144;                      // Magnitude data logging starts this number of TOF before beaming end (beaming delay excluded)
    const quint8 COMP_EXT_START_PWR_TOF = 108;                  // Magnitude data logging starts this number of TOF before beaming end (beaming delay excluded) in case of Test compression method and power class configuration
    const quint8 TEMP_ABIST_TOF = 30;
    const quint8 DIAG_CHIRP_TOF = 157;                          // Diagnostic chirp length in TOF
    const quint8 DIAG_BURST_TOF = 162;                          // Diagnostic acoustic bursts length in TOF
    //const quint8 DSP_DIAGW_SIZE = 6;
    const quint8 DSP_DIAGW_SIZE = 4;

    const quint16 m_tof_lut[64] = {195, 210, 225, 239, 254, 269, 283, 298, 313, 327, 342, 356, 371, 386, 400, 415,\
                                   430, 444, 459, 474, 488, 503, 518, 532, 547, 562, 576, 591, 605, 620, 635, 649,\
                                   664, 679, 693, 708, 723, 737, 752, 767, 781, 796, 811, 825, 840, 854, 869, 884,\
                                   898, 913, 928, 942, 957, 972, 986, 1001, 1016, 1030, 1045, 1060, 1074, 1089, 1104, 1118};
    const quint16 beaming_tof_lut[32] = {2, 1, 1, 2, 7, 8, 9, 10, 7, 8, 9, 10, 0, 0, 0, 0,\
                                         49, 49, 0, 0, 49, 49, 49, 49, 49, 31, 157, 157, 157, 162, 49, 6};
    const quint16 beaming_tof_lut_RDUS2[32] = {4, 1, 2, 4, 7, 8, 9, 5, 7, 8, 9, 10, 49, 49, 0, 0,\
                                         49, 49, 0, 0, 49, 49, 49, 49, 49, 31, 157, 157, 157, 157, 49, 6};

    qint16 tank_charge_time_tof = 0;
    qint16 k, x, b_tof;
    quint32 t;
    bool pre_fast;
    quint8 r;
    //bool wbn_present = false;
    bool compr_res_diag;
    //quint16 selct_compr_res;
    quint16 compr_timer_lut[8];

    mag_data_info->block_size = 0;
    mag_data_info->block_sample_count = 0;
    mag_data_info->block_count = 0;
    mag_data_info->block2_size = 0;
    mag_data_info->block2_sample_count = 0;
    mag_data_info->block2_count = 0;
    mag_data_info->sample_count = 0;
    mag_data_info->sample_count_fast = 0;
    mag_data_info->sample_count_pre_rx = 0;
    mag_data_info->meas_dur_tof = 0;
    mag_data_info->beaming_tof = 0;
    mag_data_info->mag_data_size = 0;
    mag_data_info->wbn_count = 0;
    mag_data_info->wbn_data_size = 0;
    mag_data_info->wbn_sample_count_2 = 0;

    for (k = 0; k < 8; k++)             // To be replaced by a constant look-up table in C++ language.
    {
        t = 2000000 + k * 1000000;      // unit [ns]
        t = (t + (51200 / 2)) / 51200;
        compr_timer_lut[k] = t;
    }


    //measurement duration
    //mag_data_info->meas_dur_tof = m_tof_lut[mag_data_info->meas_duration];
    mag_data_info->meas_dur_tof = (quint16) (mag_data_info->meas_duration * 10);


    // beaming size
    if ((gp_DSI3Master->MVar_version == VERSION_RDUS2DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH)\
             || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
    {
        mag_data_info->beaming_tof = beaming_tof_lut_RDUS2[mag_data_info->meas_type];
    }
    else
    {
        mag_data_info->beaming_tof = beaming_tof_lut[mag_data_info->meas_type];
    }



    //sample count (unit 51.2 us)
    pre_fast = false;
    b_tof = mag_data_info->beaming_tof;
    mag_data_info->sample_count_fast = 0;
    mag_data_info->sample_count = mag_data_info->meas_dur_tof;
    if ((mag_data_info->selct_comp_res == 3) || (mag_data_info->selct_comp_res == 4) || (mag_data_info->selct_comp_res == 8) || (mag_data_info->selct_comp_res == 11))
    {
        mag_data_info->sample_count = mag_data_info->sample_count - mag_data_info->beaming_tof;
    }

    //Diagnostic compression mode
    compr_res_diag = false;
    if ((mag_data_info->meas_type >= 24) && (mag_data_info->meas_type <= 26))
    {
        compr_res_diag = true;
    }

    if (mag_data_info->selct_comp_res == 0)
    {
        //102.4 us samples, 2.5 bit, compressed-concatenated
        mag_data_info->sample_count = (mag_data_info->sample_count - 1) / 2;        // conver to 102.4 us samples
        mag_data_info->block_sample_count = 8;
        mag_data_info->block_count = (mag_data_info->sample_count + 15) / 16;       // 16 samples per 2 blocks, round up to even number of blocks
        mag_data_info->block_count = mag_data_info->block_count * 2;
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;   // 8 samples per block
        mag_data_info->block_size = 3;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + 4;
    }
    else if (((mag_data_info->selct_comp_res >= 1) && (mag_data_info->selct_comp_res <= 3)) || (mag_data_info->selct_comp_res == 14) || (mag_data_info->selct_comp_res == 15))
    {
        // 102.4 us samples, 3 bit, compressed
        if ((mag_data_info->selct_comp_res == 3) || (mag_data_info->selct_comp_res == 15))      // noise monitoring
        {
            if (COMP_EXT_START_TOF > mag_data_info->beaming_tof)
            {
                b_tof = COMP_EXT_START_TOF;
            }

            mag_data_info->sample_count = mag_data_info->sample_count + b_tof;

        }

        if ((mag_data_info->compr_timer > 0) && (mag_data_info->compr_timer < 7))
        {
            k = compr_timer_lut[mag_data_info->compr_timer];
            if (mag_data_info->sample_count < (b_tof + k))
            {
                k = mag_data_info->sample_count - b_tof;
            }

            // 3 intervals: 1. beaming  @ 102.4 us, .sample_count_pre_rx  samples
            //              2. RX       @  51.2 us, .sample_count_fast    samples
            //              3. RX       @ 102.4 us,  remaining            samples

            r = (mag_data_info->sample_count - k) % 9;

            x = 1;
            if ((k & 1) == 0)
            {
                //even fast sampling time
                if ((r & 3) != 2)
                {
                    x = 0;
                }
                if ((b_tof & 1) == 0)
                {
                    if ((r & 2) == 0)
                    {
                        k = k - 1;
                    }
                }
            }
            else
            {
                //odd fast sampling time
                if ((r == 7) || ((r & 3) != 3))
                {
                    x = 0;
                }
                if((b_tof & 1) == 0)
                {
                    if ((r == 1) || (r == 2) || (r == 7))
                    {
                        k = k - 1;
                    }
                }
            }
            mag_data_info->sample_count_fast = k;
            mag_data_info->sample_count = k + (mag_data_info->sample_count - k + x) / 2;
        }
        else if(mag_data_info->compr_timer == 7)
        {
            if (mag_data_info->sample_count < (b_tof))
            {
                k = mag_data_info->sample_count - b_tof;
            }
            else
            {
                k = mag_data_info->sample_count;
            }

            if ((mag_data_info->selct_comp_res == 3) || (mag_data_info->selct_comp_res == 15))      // noise monitoring
            {
                k = k - (b_tof/2);

            }

            mag_data_info->sample_count_fast = k;
            mag_data_info->sample_count = k;
        }
        else
        {
            mag_data_info->sample_count = (mag_data_info->sample_count - 1) / 2;
        }

        mag_data_info->block_sample_count = 9;
        mag_data_info->block_count = (mag_data_info->sample_count + 8) / 9;
        mag_data_info->block_size = 4;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + DSP_DIAGW_SIZE;

        if ((mag_data_info->selct_comp_res == 14) || (mag_data_info->selct_comp_res == 15)) // WBN present
        {
            mag_data_info->wbn_count = (mag_data_info->block_count + 7) / 8;
            mag_data_info->wbn_data_size = 8 * mag_data_info->block_size;
            mag_data_info->wbn_sample_count_2 = 9;
            mag_data_info->mag_data_size = mag_data_info->mag_data_size + mag_data_info->wbn_count * 2;
        }
    }
    else if ((mag_data_info->selct_comp_res == 4) || (mag_data_info->selct_comp_res == 8))
    {
        // .block  ... uncompressed data
        // .block2 ... compressed data from noise monitoring and beaming in case of power class

        pre_fast = (mag_data_info->selct_comp_res == 4);

        //51.2 / 102.4 us samples, 8 bit, not compressed
        if (compr_res_diag)
        {
            b_tof = DIAG_CHIRP_TOF;
        }
        else
        {
            tank_charge_time_tof = mag_data_info->tank_charge_time;
            if ((mag_data_info->tank_charge_time > 6) && (mag_data_info->beaming_tof >= 32))
            {
                tank_charge_time_tof = 6;
            }
            tank_charge_time_tof = tank_charge_time_tof * 16;
            if(tank_charge_time_tof < TEMP_ABIST_TOF)
            {
                tank_charge_time_tof = TEMP_ABIST_TOF;
            }
            b_tof = POWER_PHASE_TOF - tank_charge_time_tof;
        }

        mag_data_info->sample_count = mag_data_info->sample_count + b_tof;

        if ((mag_data_info->compr_timer != 0) && (mag_data_info->selct_comp_res == 8))
        {
            if (compr_res_diag)
            {
                mag_data_info->sample_count = mag_data_info->sample_count + 1;
                b_tof = b_tof + 1;
            }
            k = compr_timer_lut[mag_data_info->compr_timer];

            if (mag_data_info->sample_count < (b_tof + k))
            {
                k = mag_data_info->sample_count - b_tof;
            }

            // 3 intervals: 1. beaming  @ 102.4 us, .sample_count_pre_rx  samples
            //              2. RX       @  51.2 us, .sample_count_fast    samples
            //              3. RX       @ 102.4 us,  remaining            samples

            mag_data_info->sample_count_fast = k;
            mag_data_info->sample_count = k + (mag_data_info->sample_count - k + 1) / 2;

        }
        else if (mag_data_info->selct_comp_res == 8)
        {
            mag_data_info->sample_count = (mag_data_info->sample_count + 1) / 2;
        }

        mag_data_info->block_count = (mag_data_info->sample_count + 1) / 2;
        mag_data_info->block_sample_count = 2;
        mag_data_info->block_size = 2;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + DSP_DIAGW_SIZE;

    }
    else if (mag_data_info->selct_comp_res == 5)
    {
        mag_data_info->block_size = 2;
        mag_data_info->block_sample_count = 4;
        mag_data_info->sample_count = 76 + 2;
        mag_data_info->block_count = 19;
        mag_data_info->meas_dur_tof = DIAG_CHIRP_TOF;
        mag_data_info->beaming_tof = DIAG_CHIRP_TOF;
        b_tof = DIAG_CHIRP_TOF;
        mag_data_info->mag_data_size = 38 + 2 + DSP_DIAGW_SIZE;
    }

    else if (mag_data_info->selct_comp_res == 6)
    {
        // 51.2 us samples, 3 bit, compressed
        pre_fast = true;
        mag_data_info->block_sample_count = 9;
        mag_data_info->block_count = (mag_data_info->sample_count + 8 - 1) / 9;
        mag_data_info->block_size = 4;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + DSP_DIAGW_SIZE;
    }

    else if (mag_data_info->selct_comp_res == 7)
    {
        mag_data_info->block_size = 2;
        mag_data_info->block_sample_count = 4;
        mag_data_info->sample_count = 68 + 2 + 4;
        mag_data_info->block_count = 17;
        mag_data_info->meas_dur_tof = DIAG_CHIRP_TOF;
        mag_data_info->beaming_tof = DIAG_CHIRP_TOF;
        b_tof = DIAG_CHIRP_TOF;
        mag_data_info->mag_data_size = 34 + 2 + 4 + DSP_DIAGW_SIZE;
    }
    else if ((mag_data_info->selct_comp_res == 9))
    {
        // .block       ... IQ signal (total)
        // .block2_size ... number of samples @ 51.2 us

        // 51.2 / 102.4 us samples, 3.5 bit, compressed-concatenated IQ signals

        pre_fast = (mag_data_info->selct_comp_res == 10);

        if (mag_data_info->selct_comp_res == 9)
        {
            if (mag_data_info->compr_timer != 0)
            {
                k = compr_timer_lut[mag_data_info->compr_timer];
                if (mag_data_info->sample_count < (mag_data_info->beaming_tof + k))
                {
                    k = mag_data_info->sample_count - mag_data_info->beaming_tof;
                }
                // 3 intervals: 1. beaming  @ 102.4 us, .sample_count_pre_rx  samples
                //              2. RX       @  51.2 us, .sample_count_fast    samples
                //              3. RX       @ 102.4 us,  remaining            samples
                r = (mag_data_info->sample_count - k) & 3;
                x = 0;

                if (((mag_data_info->beaming_tof ^ k) & 1) == 0)
                {
                    if (r == 2)
                    {
                        k = k + 1;
                    }
                }
                else
                {
                    if (r == 0)
                    {
                        x = 1;
                    }
                }

                mag_data_info->sample_count_fast = k;
                mag_data_info->sample_count = k + (mag_data_info->sample_count - k + x + 1) / 2;
            }
            else
            {
                mag_data_info->sample_count = (mag_data_info->sample_count + 1) / 2;
            }
        }
        mag_data_info->block_sample_count = 4;
        mag_data_info->block_count = (mag_data_info->sample_count + 3) / 4;
        mag_data_info->block_size = 4;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + DSP_DIAGW_SIZE;
    }

    else if (mag_data_info->selct_comp_res == 12)
    {
        // 102.4 us samples, 16 bit, IQ uncompressed
        if (compr_res_diag)
        {
            mag_data_info->sample_count = mag_data_info->beaming_tof;
        }

        if (mag_data_info->compr_timer != 0)
        {
            k = compr_timer_lut[mag_data_info->compr_timer];

            if (mag_data_info->sample_count < (mag_data_info->beaming_tof + k))
            {
                k = mag_data_info->sample_count - mag_data_info->beaming_tof;
            }
            // 3 intervals: 1. beaming  @ 102.4 us, .sample_count_pre_rx  samples
            //              2. RX       @  51.2 us, .sample_count_fast    samples
            //              3. RX       @ 102.4 us,  remaining            samples

            r = (mag_data_info->sample_count - k) & 3;

            x = 0;
            if (((mag_data_info->beaming_tof ^ k) & 1) == 0)
            {
                if (r == 2)
                {
                    k = k + 1;
                }
            }
            else
            {
                if (r == 0)
                {
                    x = 1;
                }
            }
            mag_data_info->sample_count_fast = k;
            mag_data_info->sample_count = k + (mag_data_info->sample_count - k + x + 1) / 2;
        }
        else
        {
            mag_data_info->sample_count = (mag_data_info->sample_count + 1) / 2;
        }

        mag_data_info->block_count = (mag_data_info->sample_count + 3) / 4;
        mag_data_info->block_count = mag_data_info->block_count * 4;

        mag_data_info->block_sample_count = 1;
        mag_data_info->block_size = 2;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + DSP_DIAGW_SIZE;

        mag_data_info->wbn_count = (mag_data_info->block_count + (4 * 16 - 1)) / (4 * 16);
        mag_data_info->wbn_data_size = 4 * 16 * mag_data_info->block_size;
        mag_data_info->wbn_sample_count_2 = 8;
        mag_data_info->mag_data_size = mag_data_info->mag_data_size + mag_data_info->wbn_count * 2;
    }

    if (pre_fast == false)
    {
        b_tof = b_tof / 2;
    }
    mag_data_info->sample_count_pre_rx = b_tof;


    /*if ((mag_data_info->meas_type >= 20) && (mag_data_info->meas_type <= 24))
    {
        if((GET_REG("SREGX05E[short_range_thr]").toUInt() & 0x01) != 0)
        {
            mag_data_info->mag_data_size = mag_data_info->mag_data_size + 16;
        }

    }*/

    //qDebug() << QString("%1").arg(mag_data_info->mag_data_size);

    /*if (GET_REG("NearRange").toInt() != 0)
    {
        if (((mag_data_info->selct_comp_res >= 0) && (mag_data_info->selct_comp_res <= 3)) || (mag_data_info->selct_comp_res == 6))
        {
            mag_data_info->mag_data_size = mag_data_info->mag_data_size + 16;
        }

    }*/
}


/*************************************************/
void Measurement::get_mag_data_info4(t_MAG_DATA_INFO *mag_data_info)
{
    const quint8 BEAMING_DELAY_DEF_TOF = 2;                     // 100 us default beaming delay
    const quint8 POWER_PHASE_TOF = 176 - BEAMING_DELAY_DEF_TOF; // 9 ms in TOF, unit [51.2us], minus Beaming Prepare state
    const quint8 COMP_EXT_START_TOF = 144;                      // Magnitude data logging starts this number of TOF before beaming end (beaming delay excluded)
    const quint8 COMP_EXT_START_PWR_TOF = 108;                  // Magnitude data logging starts this number of TOF before beaming end (beaming delay excluded) in case of Test compression method and power class configuration
    const quint8 TEMP_ABIST_TOF = 30;
    const quint8 DIAG_CHIRP_TOF = 157;                          // Diagnostic chirp length in TOF
    //const quint8 DSP_DIAGW_SIZE = 6;
    const quint8 DSP_DIAGW_SIZE = 4;

    // Compression Methods
    const quint8 DSP_COMP_MAG_3_LOG = 2;
    const quint8 DSP_COMP_MAG_3_LOG_EXT = 3;       // extended magnitude sampling
    const quint8 DSP_COMP_TST_FAST_SMP = 4;
    const quint8 DSP_COMP_DIAG_CHIRP = 5;
    const quint8 DSP_COMP_DIAG_CHIRP_MODEL = 7;

    const quint8 DSP_COMP_TST = 8;

    // IQ signal
    const quint8 DSP_COMP_IQ = 1 + 8;
    const quint8 DSP_COMP_IQ_FAST_SMP = 2 + 8;
    const quint8 DSP_COMP_IQ_UNCOMP = 3 + 8;       // uncompressed IQ signal

    // wideband noise flags included
    const quint8 WBN_BASE = 0xC;
    const quint8 DSP_COMP_IQ_UNCOMP_WBN = 0 | WBN_BASE;
    const quint8 DSP_COMP_MAG_3_LOG_WBN = DSP_COMP_MAG_3_LOG | WBN_BASE;
    const quint8 DSP_COMP_MAG_3_LOG_EXT_WBN = DSP_COMP_MAG_3_LOG_EXT | WBN_BASE;

    const quint16 STEP_TIME_NS = 51200;

    // not implemented, the highest bit is intentionaly set
    //Public Const DSP_COMP_MAG_25 = 0 Or &H10
    //Public Const DSP_COMP_MAG_3_LINEAR = 1 Or &H10
    //Public Const DSP_COMP_MAG_3_LINEAR_WBN = (DSP_COMP_MAG_3_LINEAR Or &HC) Or &H10
    //Public Const DSP_COMP_MAG_3_LOG_FAST_SMP = 6 Or &H10


    const quint16 beaming_tof_lut[32] = {2, 1, 1, 2, 7, 8, 9, 5, 7, 8, 9, 10, 49, 49, 0, 0,\
                                         49, 49, 0, 0, 49, 49, 49, 49, 49, 31, 157, 157, 157, 157, 49, 6};
    /*const quint16 beaming_tof_lut_RDUS2[32] = {4, 1, 2, 4, 7, 8, 9, 5, 7, 8, 9, 10, 49, 49, 0, 0,\
                                         49, 49, 0, 0, 49, 49, 49, 49, 49, 31, 157, 157, 157, 157, 49, 6};*/

    qint16 tank_charge_time_tof = 0;
    qint16 k, n, rx_smp_count, x, x1, b_tof;
    quint32 t;
    bool pre_fast;
    quint8 r;
    bool wbn_present = false;
    bool compr_res_diag;
    //quint16 selct_compr_res;
    quint16 compr_timer_lut;

    mag_data_info->block_size = 0;
    mag_data_info->block_sample_count = 0;
    mag_data_info->block_count = 0;
    mag_data_info->block2_size = 0;
    mag_data_info->block2_sample_count = 0;
    mag_data_info->block2_count = 0;
    mag_data_info->sample_count = 0;
    mag_data_info->sample_count_fast = 0;
    mag_data_info->sample_count_pre_rx = 0;
    mag_data_info->meas_dur_tof = 0;
    mag_data_info->beaming_tof = 0;
    mag_data_info->mag_data_size = 0;
    mag_data_info->wbn_count = 0;
    mag_data_info->wbn_data_size = 0;
    mag_data_info->wbn_sample_count_2 = 0;



    /*for (k = 0; k < 7; k++)             // To be replaced by a constant look-up table in C++ language.
    {
        t = 2000000 + k * 1000000;      // unit [ns]
        t = (t + (51200 / 2)) / 51200;
        compr_timer_lut[k] = t;
    }
        compr_timer_lut[7] = 0xFFF;
*/

    if (mag_data_info->compr_timer == 7)
    {
        compr_timer_lut = 0xFFF;
    }
    else if ((mag_data_info->compr_timer > 0) && (mag_data_info->compr_timer < 7))
    {
        t = 2000 + 1000 * mag_data_info->compr_timer;
        t = t * 1000;
        t = (t + (STEP_TIME_NS / 2)) / STEP_TIME_NS;
        compr_timer_lut = t;
    }

    //measurement duration
    //mag_data_info->meas_dur_tof = m_tof_lut[mag_data_info->meas_duration];
    mag_data_info->meas_dur_tof = (quint16) (mag_data_info->meas_duration * 10);


    // beaming size
    mag_data_info->beaming_tof = beaming_tof_lut[mag_data_info->meas_type];


    //sample count (unit 51.2 us)
    b_tof = mag_data_info->beaming_tof;
    mag_data_info->sample_count = mag_data_info->meas_dur_tof;
    if ((mag_data_info->selct_comp_res == 3) || (mag_data_info->selct_comp_res == 4) || (mag_data_info->selct_comp_res == 8)/* || (mag_data_info->selct_comp_res == 11)*/)  //51.2us
    {
        mag_data_info->sample_count = mag_data_info->sample_count - mag_data_info->beaming_tof;
    }

    //Diagnostic compression mode
    compr_res_diag = false;
    if ((mag_data_info->meas_type >= 24) && (mag_data_info->meas_type <= 26))
    {
        compr_res_diag = true;
    }


    if (mag_data_info->selct_comp_res == 0)
    {
        //102.4 us samples, 2.5 bit, compressed-concatenated
        mag_data_info->sample_count = (mag_data_info->sample_count - 1) / 2;        // conver to 102.4 us samples
        mag_data_info->block_sample_count = 8;
        mag_data_info->block_count = (mag_data_info->sample_count + 15) / 16;       // 16 samples per 2 blocks, round up to even number of blocks
        mag_data_info->block_count = mag_data_info->block_count * 2;
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;   // 8 samples per block
        mag_data_info->block_size = 3;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + 4;
    }
    else if (((mag_data_info->selct_comp_res >= 1) && (mag_data_info->selct_comp_res <= 3)) || (mag_data_info->selct_comp_res == 14) || (mag_data_info->selct_comp_res == 15))
    {
        // 102.4 us samples, 3 bit, compressed
        if ((mag_data_info->selct_comp_res == 3) || (mag_data_info->selct_comp_res == 15))      // noise monitoring
        {
            if (COMP_EXT_START_TOF > mag_data_info->beaming_tof)
            {
                b_tof = COMP_EXT_START_TOF;
            }
            mag_data_info->sample_count = mag_data_info->sample_count + b_tof;
        }


        if ((mag_data_info->compr_timer > 0) && (mag_data_info->compr_timer < 8))
        {
            // 3 intervals: 1. beaming  @ 102.4 us, .sample_count_pre_rx  samples
            //              2. RX       @  51.2 us, .sample_count_fast    samples
            //              3. RX       @ 102.4 us,  remaining            samples

            k = compr_timer_lut;
            if (mag_data_info->sample_count >= (b_tof + k))
            {
                r = (mag_data_info->sample_count - k) % 9;
                x = 1;
                if ((k & 1)== 0)
                {
                    //even fast sampling time
                    if ((r & 3) != 2)
                    {
                        x = 0;
                    }
                    if ((b_tof & 1) == 0)
                    {
                        if ((r & 2) == 0)
                        {
                            k = k - 1;
                        }
                    }
                    else
                    {
                        if (r  == 5)
                        {
                            x = 1;
                        }
                    }
                }
                else
                {
                    // odd fast sampling time
                    if (r != 3)
                    {
                        x = 0;
                    }
                    if ((b_tof & 1) == 0)
                    {
                        if ((r == 1) || (r == 2) || (r == 7))
                        {
                            k = k - 1;
                        }
                    }
                    else
                    {
                        if (r  == 5)
                        {
                            x = 1;
                        }
                    }
                }
                //mag_data_info->sample_count_fast = k;
                mag_data_info->sample_count = k + (mag_data_info->sample_count - k + x) / 2;
            }
            x = (b_tof + 1) / 2;
            if (mag_data_info->sample_count > b_tof)
            {
                if (((mag_data_info->sample_count - b_tof + x) % 9) == 1)
                {
                    mag_data_info->sample_count = mag_data_info->sample_count - 1;
                }
                mag_data_info->sample_count = mag_data_info->sample_count - x;
            }
            else
            {
                mag_data_info->sample_count = (mag_data_info->sample_count - 1) / 2;
            }
        }
        else
        {
            mag_data_info->sample_count = (mag_data_info->sample_count - 1) / 2;
        }

        mag_data_info->block_sample_count = 9;
        mag_data_info->block_count = (mag_data_info->sample_count + 8) / 9;
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;
        mag_data_info->block_size = 4;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + DSP_DIAGW_SIZE; //?????????


        if ((mag_data_info->selct_comp_res == 14) || (mag_data_info->selct_comp_res == 15)) // WBN present
        {
            mag_data_info->wbn_count = (mag_data_info->block_count + 7) / 8;
            mag_data_info->wbn_data_size = 8 * mag_data_info->block_size;
            mag_data_info->wbn_sample_count_2 = 9;
            //mag_data_info->mag_data_size = mag_data_info->mag_data_size + mag_data_info->wbn_count * 2;
        }

    }
    else if ((mag_data_info->selct_comp_res == 4) || (mag_data_info->selct_comp_res == 8))
    {
        // .block  ... uncompressed data
        // .block2 ... compressed data from noise monitoring and beaming in case of power class

        rx_smp_count = mag_data_info->sample_count;

        b_tof = DIAG_CHIRP_TOF;
        if (compr_res_diag == false)
        {
            tank_charge_time_tof = mag_data_info->tank_charge_time;
            if ((mag_data_info->tank_charge_time > 6) && (mag_data_info->beaming_tof >= 32))
            {
                tank_charge_time_tof = 6;
            }
            tank_charge_time_tof = tank_charge_time_tof * 16;
            if(tank_charge_time_tof < TEMP_ABIST_TOF)
            {
                tank_charge_time_tof = TEMP_ABIST_TOF;
            }
            b_tof = POWER_PHASE_TOF - tank_charge_time_tof;
        }
        mag_data_info->sample_count = mag_data_info->sample_count + b_tof;


        if (mag_data_info->selct_comp_res == 8)
        {
            if (mag_data_info->compr_timer != 0)
            {
                k = compr_timer_lut;

                if (rx_smp_count >= k)
                {
                    r = (mag_data_info->sample_count - k) & 3;
                    x = 0;
                    if (((b_tof ^ k) & 1) != 0)
                    {
                        if (r == 0)
                        {
                            x = 1;
                        }
                    }
                    mag_data_info->sample_count = k + (mag_data_info->sample_count - k + x + 1) / 2;
                }
                else
                {
                    if (rx_smp_count >= 0)
                    {
                        mag_data_info->sample_count = rx_smp_count + (mag_data_info->sample_count - rx_smp_count + 1) / 2;
                    }
                    else
                    {
                        mag_data_info->sample_count = (mag_data_info->sample_count + 1) / 2;
                    }

                }

            }
            else
            {
                mag_data_info->sample_count = (mag_data_info->sample_count + 1) / 2;
            }
        }
        mag_data_info->block_count = (mag_data_info->sample_count + 1) / 2;
        mag_data_info->block_sample_count = 2;
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;
        mag_data_info->block_size = 2;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + DSP_DIAGW_SIZE;

    }
    else if (mag_data_info->selct_comp_res == 5)
    {
        mag_data_info->block_size = 2;
        mag_data_info->block_sample_count = 4;
        mag_data_info->sample_count = 76 + 2;
        mag_data_info->block_count = 19;
        mag_data_info->meas_dur_tof = DIAG_CHIRP_TOF;
        mag_data_info->beaming_tof = DIAG_CHIRP_TOF;
        b_tof = DIAG_CHIRP_TOF;
        mag_data_info->mag_data_size = 38 + 2 + DSP_DIAGW_SIZE;
    }

    else if (mag_data_info->selct_comp_res == 6)
    {
        // 51.2 us samples, 3 bit, compressed
        pre_fast = true;
        mag_data_info->block_sample_count = 9;
        mag_data_info->block_count = (mag_data_info->sample_count + 8 - 1) / 9;
        mag_data_info->block_size = 4;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + DSP_DIAGW_SIZE;
    }

    else if (mag_data_info->selct_comp_res == 7)
    {
        mag_data_info->block_size = 2;
        mag_data_info->block_sample_count = 4;
        mag_data_info->sample_count = 68 + 2 + 4;
        mag_data_info->block_count = 17;
        mag_data_info->meas_dur_tof = DIAG_CHIRP_TOF;
        mag_data_info->beaming_tof = DIAG_CHIRP_TOF;
        b_tof = DIAG_CHIRP_TOF;
        mag_data_info->mag_data_size = 34 + 2 + 4 + DSP_DIAGW_SIZE;
    }
    else if ((mag_data_info->selct_comp_res == 9))
    {
        // .block       ... IQ signal (total)
        // .block2_size ... number of samples @ 51.2 us

        // 51.2 / 102.4 us samples, 3.5 bit, compressed-concatenated IQ signals

        if (mag_data_info->selct_comp_res == 9)
        {
            if (mag_data_info->compr_timer != 0)
            {
                rx_smp_count = mag_data_info->sample_count - mag_data_info->beaming_tof;
                k = compr_timer_lut;
                if (rx_smp_count >= k)
                {
                   r = (mag_data_info->sample_count - k ) & 3;
                   x = 0;
                   if (((mag_data_info->beaming_tof ^ k) & 1) != 0)
                   {
                       if (r == 0)
                       {
                           x = 1;
                       }
                   }
                   mag_data_info->sample_count = k + (mag_data_info->sample_count - k + x + 1) / 2;
                }
                else
                {
                    if (rx_smp_count > 0)
                    {
                        mag_data_info->sample_count = rx_smp_count + (mag_data_info->beaming_tof + 1) / 2;
                    }
                    else
                    {
                        mag_data_info->sample_count = (mag_data_info->sample_count + 1) / 2;
                    }
                }

            }
            else
            {
                mag_data_info->sample_count = (mag_data_info->sample_count + 1) / 2;
            }
        }

        mag_data_info->block_sample_count = 4;
        mag_data_info->block_count = (mag_data_info->sample_count + 3) / 4;
        mag_data_info->block_size = 4;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + DSP_DIAGW_SIZE;

    }

    else if (mag_data_info->selct_comp_res == 12)
    {
        // 102.4 us samples, 16 bit, IQ uncompressed
        if (compr_res_diag)
        {
            mag_data_info->sample_count = mag_data_info->beaming_tof;
        }

        if (mag_data_info->compr_timer != 0)
        {
            rx_smp_count = mag_data_info->sample_count - mag_data_info->beaming_tof;

            k = compr_timer_lut;

            if (rx_smp_count >= k)
            {
                r = (mag_data_info->sample_count - k) & 3;
                x = 0;
                if (((mag_data_info->beaming_tof ^ k) & 1) != 0)
                {
                    if (r == 0)
                    {
                        x = 1;
                    }
                }
                mag_data_info->sample_count = k + (mag_data_info->sample_count - k + x + 1) / 2;
            }
            else
            {
                if (rx_smp_count >= 0)
                {
                    mag_data_info->sample_count = rx_smp_count + (mag_data_info->beaming_tof + 1) / 2;
                }
                else
                {
                    mag_data_info->sample_count = (mag_data_info->sample_count + 1) / 2;
                }
            }
        }
        else
        {
            mag_data_info->sample_count = (mag_data_info->sample_count + 1) / 2;
        }
        mag_data_info->block_count = (mag_data_info->sample_count + 3) / 4;
        mag_data_info->block_count = mag_data_info->block_count * 4;

        mag_data_info->block_sample_count = 1;
        mag_data_info->block_size = 2;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + DSP_DIAGW_SIZE;

        mag_data_info->wbn_count = (mag_data_info->block_count + (4 * 16 - 1)) / (4 * 16);
        mag_data_info->wbn_data_size = 4 * 16 * mag_data_info->block_size;
        mag_data_info->wbn_sample_count_2 = 8;
        mag_data_info->mag_data_size = mag_data_info->mag_data_size + mag_data_info->wbn_count * 2;

    }
}




/*
void Measurement::get_mag_data_info_mobis(t_MAG_DATA_INFO *mag_data_info)
{
    const quint8 BEAMING_DELAY_DEF_TOF = 2;                     // 100 us default beaming delay
    const quint8 POWER_PHASE_TOF = 176 - BEAMING_DELAY_DEF_TOF; // 9 ms in TOF, unit [51.2us], minus Beaming Prepare state
    const quint8 COMP_EXT_START_TOF = 144;                      // Magnitude data logging starts this number of TOF before beaming end (beaming delay excluded)
    const quint8 TEMP_ABIST_TOF = 30;
    const quint8 DIAG_CHIRP_TOF = 157;                          // Diagnostic chirp length in TOF   

    const quint16 beaming_tof_lut[32] = {4, 1, 2, 4, 7, 8, 9, 5, 7, 8, 9, 10, 0, 0, 0, 0,\
                                         49, 49, 0, 0, 49, 49, 49, 31, 157, 157, 157, 49, 0, 0, 0, 0};

    //defaults
    mag_data_info->block_size = 0;
    mag_data_info->block_sample_count = 0;
    mag_data_info->sample_count = 0;
    mag_data_info->block_count = 0;
    mag_data_info->meas_dur_tof = 0;
    mag_data_info->beaming_tof = 0;
    mag_data_info->mag_data_size = 0;

    //measurement duration
    mag_data_info->meas_dur_tof = (quint16) (mag_data_info->meas_duration * 10);


    // beaming size
    mag_data_info->beaming_tof = beaming_tof_lut[mag_data_info->meas_type];


    // sample count (unit 51.2 us) for modes with noise monitoring
    mag_data_info->sample_count = mag_data_info->meas_dur_tof;
    if ((mag_data_info->selct_comp_res == 1) || (mag_data_info->selct_comp_res == 2)|| (mag_data_info->selct_comp_res == 6))
        mag_data_info->sample_count = mag_data_info->sample_count - mag_data_info->beaming_tof;


    if ((mag_data_info->selct_comp_res == 1) || (mag_data_info->selct_comp_res == 4))
    {
        // 102.4 us samples, 3 bit, compressed
        if (mag_data_info->selct_comp_res == 1)
        {
            if (COMP_EXT_START_TOF > mag_data_info->beaming_tof)
                mag_data_info->sample_count = mag_data_info->sample_count + COMP_EXT_START_TOF;
            else
                mag_data_info->sample_count = mag_data_info->sample_count + mag_data_info->beaming_tof;
        }
        mag_data_info->sample_count = (mag_data_info->sample_count - 1) / 2;        // conver to 102.4 us samples
        mag_data_info->block_sample_count = 9;
        mag_data_info->block_count = (mag_data_info->sample_count + 8) / 9;         // 9 samples per block, round up
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;
        mag_data_info->block_size = 4;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + 4;
    }
    else if (mag_data_info->selct_comp_res == 2)
    {
        // 51.2 us samples, 8 bit, not compressed

        if((mag_data_info->meas_type >= 24) && (mag_data_info->meas_type <= 26))
        {
            mag_data_info->sample_count = mag_data_info->sample_count + DIAG_CHIRP_TOF;
        }
        else
        {
            mag_data_info->sample_count = mag_data_info->sample_count + (POWER_PHASE_TOF - TEMP_ABIST_TOF);
        }
        mag_data_info->block_count = (mag_data_info->sample_count + 1) / 2;         // 2 samples per block, round up
        mag_data_info->block_sample_count = 2;
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;
        mag_data_info->block_size = 2;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + 4;
    }
    else if ((mag_data_info->selct_comp_res == 3) || (mag_data_info->selct_comp_res == 5))
    {
        // diagnostics
        mag_data_info->block_size = 2;
        mag_data_info->block_sample_count = 4;
        mag_data_info->sample_count = 76 + 2;
        mag_data_info->block_count = 19;
        mag_data_info->meas_dur_tof = DIAG_CHIRP_TOF;
        mag_data_info->beaming_tof = DIAG_CHIRP_TOF;
        mag_data_info->mag_data_size = 40 + 4;
    }
    else if ((mag_data_info->selct_comp_res == 6) || (mag_data_info->selct_comp_res == 7))
    {
        // 102.4 us samples, 3 bit, compressed, with WBN
        if (mag_data_info->selct_comp_res == 6)
        {
            if (COMP_EXT_START_TOF > mag_data_info->beaming_tof)
                mag_data_info->sample_count = mag_data_info->sample_count + COMP_EXT_START_TOF;
            else
                mag_data_info->sample_count = mag_data_info->sample_count + mag_data_info->beaming_tof;
        }
        mag_data_info->sample_count = (mag_data_info->sample_count - 1) / 2;        // conver to 102.4 us samples     97
        mag_data_info->block_sample_count = 9;
        mag_data_info->block_count = (mag_data_info->sample_count + 8) / 9;         // 9 samples per block, round up
        mag_data_info->sample_count = mag_data_info->block_count * mag_data_info->block_sample_count;
        mag_data_info->block_size = 4;
        mag_data_info->mag_data_size = mag_data_info->block_count * mag_data_info->block_size + 4;

        mag_data_info->wbn_count = (mag_data_info->block_count + 7) / 8;
        mag_data_info->wbn_data_size = 8 * mag_data_info->block_size;
        mag_data_info->wbn_sample_count_2 = 9;

        mag_data_info->mag_data_size = mag_data_info->mag_data_size + 2 * mag_data_info->wbn_count;
    }
}
*/
/*************************************************/
quint16 Measurement::get_diag_position(quint8 meas_type, quint8 selct_compr_res, quint8 meas_duration, quint8 direct, quint8 tank_charge_time, quint8 compr_timer)
{
    t_MAG_DATA_INFO mag_data_info;

    mag_data_info.meas_type = meas_type;
    mag_data_info.selct_comp_res = selct_compr_res;
    mag_data_info.meas_duration = meas_duration;
    mag_data_info.direct = direct;
    mag_data_info.tank_charge_time = tank_charge_time;
    mag_data_info.compr_timer = compr_timer;

    m_samplecount1_first102[0] = 0;
    m_samplecount2_first51[0] = 0;
    m_samplecount3_second102[0] = 2300;

    if ((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
    {
        get_mag_data_info3(&mag_data_info);


        switch(mag_data_info.selct_comp_res)
        {
            case 1:
            case 2:
            case 3:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            m_samplecount1_first102[0] = mag_data_info.sample_count_pre_rx;
            m_samplecount2_first51[0] = mag_data_info.sample_count_pre_rx + mag_data_info.sample_count_fast;
            m_samplecount3_second102[0] = 2300;
            break;

            case 4:
            m_samplecount1_first102[0] = 0;
            m_samplecount2_first51[0] = 2300;
            m_samplecount3_second102[0] = 0;
            break;

            case 6:
            m_samplecount1_first102[0] = 0;
            m_samplecount2_first51[0] = 2300;
            m_samplecount3_second102[0] = 0;
            break;

            case 8:
            /*compression[0] = 2;
            compression[1] = 4;
            block_count[0] = mag_data_info.block2_count;
            block_count[1] = mag_data_info.block_count;
            block_sample_count[0] = mag_data_info.block2_sample_count;
            block_sample_count[1] = mag_data_info.block_sample_count;
            byte_shift[1] = mag_data_info.block2_count * mag_data_info.block2_size;*/


            m_samplecount1_first102[0] = mag_data_info.sample_count_pre_rx;
            m_samplecount2_first51[0] = mag_data_info.sample_count_pre_rx + mag_data_info.sample_count_fast;
            m_samplecount3_second102[0] = 2300;
            break;

            case 9:
            m_samplecount1_first102[0] = mag_data_info.sample_count_pre_rx;
            m_samplecount2_first51[0] = mag_data_info.sample_count_pre_rx + mag_data_info.sample_count_fast;
            m_samplecount3_second102[0] = 2300;
            break;

            case 10:
            m_samplecount1_first102[0] = 0;
            m_samplecount2_first51[0] = 2300;
            m_samplecount3_second102[0] = 0;
            break;
        }

        if (GET_REG("SREGXV205E[8:6]").toUInt() == 7)
        {
            m_samplecount2_first51[0] = 2300;
            m_samplecount3_second102[0] = 0;
        }


        if (m_samplecount1_first102[0] > 0)
        {
            m_samplecount1_first102[0] = m_samplecount1_first102[0] - 1;
        }

        if (m_samplecount2_first51[0] > 0)
        {
            m_samplecount2_first51[0] = m_samplecount2_first51[0] - 1;
        }
    }
    else
    {
        get_mag_data_info(&mag_data_info);

    }

    return mag_data_info.mag_data_size;
}

/*************************************************/
void Measurement::slot_updateTable(void) {
    UpdateSummaryTable();
    UpdateIQTable();
}

void Measurement::SetSlaveReg(int memory, int page, int index, QString name, int value)
{
    gp_DSI3Slave->SREGX[memory].at(gp_DSI3Slave->m_map_sregs[memory][page][index])->setValue(name, value);
}

quint32 Measurement::GetSlaveReg(int memory, int page, int index, QString name)
{
    return gp_DSI3Slave->SREGX[memory].at(gp_DSI3Slave->m_map_sregs[memory][page][index])->value(name);
}

void Measurement::HideBSD(bool hide)
{
    ui->label_bsd->setVisible(!hide);
    ui->label_bsden->setVisible(!hide);
    ui->label_bsden1->setVisible(!hide);
    ui->label_bsden6->setVisible(!hide);
    ui->label_bsdms->setVisible(!hide);
    ui->checkBox_bsden1->setVisible(!hide);
    ui->checkBox_bsden2->setVisible(!hide);
    ui->checkBox_bsden3->setVisible(!hide);
    ui->checkBox_bsden4->setVisible(!hide);
    ui->checkBox_bsden5->setVisible(!hide);
    ui->checkBox_bsden6->setVisible(!hide);
    ui->lineEdit_bsd->setVisible(!hide);
    ui->checkBox_dgre->setVisible(!hide);
}

QByteArray Measurement::Descramble(QByteArray data, int slot)
{
    quint8 inpchar;
    QByteArray output;
    quint8 descr_table[8][19] = {\
        {0x67, 0x3d, 0x66, 0xdd, 0xae, 0x9d, 0xf8, 0xa1, 0x4e, 0xb0, 0xb0, 0x74, 0x74, 0x27, 0x2b, 0x5a, 0x5b, 0xbb, 0x73},\
        {0x86, 0xb2, 0xe6, 0xf9, 0xc6, 0x86, 0xd2, 0xee, 0xd9, 0xc8, 0x96, 0x5a, 0xee, 0xbf, 0xc0, 0x34, 0x54, 0x1f, 0x3f},\
        {0x40, 0x41, 0x30, 0xf4, 0x14, 0x4f, 0x03, 0x74, 0x45, 0x23, 0xfb, 0x55, 0x0b, 0xf3, 0x43, 0x01, 0x71, 0xc4, 0xe0},\
        {0x93, 0x6c, 0x29, 0x2d, 0xde, 0xd9, 0xd8, 0x9a, 0x5a, 0xeb, 0xbf, 0x03, 0x34, 0x45, 0x13, 0xff, 0x45, 0x04, 0xf3},\
        {0x72, 0x43, 0xa1, 0x79, 0xb4, 0xe2, 0x7b, 0xc5, 0x67, 0x9b, 0x6e, 0x2f, 0xac, 0x5c, 0x79, 0x31, 0xe2, 0xd8, 0xcd},\
        {0xd3, 0xcb, 0x11, 0x1b, 0xcc, 0x47, 0x55, 0x7e, 0xf7, 0xa0, 0x0a, 0x70, 0x83, 0x20, 0x29, 0x18, 0xda, 0x0a, 0xd7},\
        {0x22, 0xc9, 0x91, 0x9a, 0xac, 0xef, 0x7d, 0x00, 0xe5, 0x08, 0x83, 0x06, 0x21, 0x8a, 0xd0, 0xeb, 0x58, 0x0b, 0x36},\
        {0xb1, 0x95, 0xbc, 0xa7, 0x7d, 0x36, 0xe1, 0x9a, 0x8c, 0xe7, 0x6d, 0x0a, 0xe5, 0x8f, 0x83, 0x24, 0x29, 0x1b, 0xda}};\

    for (int i = 0; i < data.length(); i++)
    {
        inpchar = (quint8)data.at(i) & 0xFF;
        output.append(inpchar ^ descr_table[slot][i]);
    }


    return output;
}

void Measurement::ShowPER(void)
{
    if (m_packets == 0)
    {
        ui->labelPER->setText("Total PER =");
        ui->labelPACKETS->setText("Total packets = 0");
    }
    else
    {
        ui->labelPER->setText(QString("Total PER = %1").arg((double)m_PERerrors/m_packets, 0, 'E', 3));
        ui->labelPACKETS->setText(QString("Total packets = %1").arg(m_packets));
    }
}

void Measurement::CalculatePHASE_DIFF(void)
{
    int i;
    double phase_diff_temp;
    QVector<double> phase1, phase2;
    phase1.clear();
    phase2.clear();

    if (VarPHASE_DIFF_12_ENA.value() == 1)
    {
        if ((m_meas_slot_number >= 4) && (m_meas_slot_number <= 8))
        {
            if ((m_meas_slots.at(0) == "1H") && (m_meas_slots.at(1) == "1L") && (m_meas_slots.at(2) == "2H") && (m_meas_slots.at(3) == "2L"))
            {
                if (m_meas_slot_number == 4)
                {
                    m_meas_slot_number = m_meas_slot_number + 2;
                    m_meas_slots.append("THR");   
                    m_meas_slots.append("PD12");            
                    m_meas_samples[m_meas_slot_number - 2].clear();   
                    m_meas_samples[m_meas_slot_number - 1].clear();  

                    for (i = 0; i < m_meas_samples[0].size(); i++)
                    {
                        m_meas_samples[m_meas_slot_number - 2].append(VarPHASE_DIFF_THR.value());
                    }
                }
                else
                {
                    m_meas_slot_number = m_meas_slot_number + 1;
                    m_meas_slots.append("PD12");            
                    m_meas_samples[m_meas_slot_number - 1].clear();                    
                }

                
                for (i = 0; i < m_meas_samples[0].size(); i++)
                {
                    if ((m_meas_samples[1].at(i) > VarPHASE_DIFF_THR.value()) && (m_meas_samples[3].at(i) > VarPHASE_DIFF_THR.value()))
                    {
                        phase_diff_temp = m_meas_samples[2].at(i) - m_meas_samples[0].at(i) + 512.0;

                        if (phase_diff_temp > 1023)
                        {
                            phase_diff_temp = phase_diff_temp - 1023;
                        }
                        if (phase_diff_temp < 0)
                        {
                            phase_diff_temp = phase_diff_temp + 1023;
                        }

                        if (VarPHASE_DIFF_12_INVERT.value() == 1)
                        {
                        phase_diff_temp = 1023 - phase_diff_temp;
                        }
                        m_meas_samples[m_meas_slot_number - 1].append(phase_diff_temp);
                        phase1.append(m_meas_samples[0].at(i));
                        phase2.append(m_meas_samples[2].at(i));
                    }
                    else
                    {
                        phase1.append(0);
                        phase2.append(0);
                        m_meas_samples[m_meas_slot_number - 1].append(512.0);
                    }
                }
                m_meas_samples[0] = phase1;
                m_meas_samples[2] = phase2;
            }
        }
    }


    if (VarPHASE_DIFF_34_ENA.value() == 1)
    {
        if ((m_meas_slot_number >= 8) && (m_meas_slot_number <= 9))
        {
            phase1.clear();
            phase2.clear();
            if ((m_meas_slots.at(4) == "3H") && (m_meas_slots.at(5) == "3L") && (m_meas_slots.at(6) == "4H") && (m_meas_slots.at(7) == "4L"))
            {
                m_meas_slot_number = m_meas_slot_number + 1;
                m_meas_slots.append("PD34");
                m_meas_samples[m_meas_slot_number - 1].clear();

                for (i = 0; i < m_meas_samples[0].size(); i++)
                {
                    if ((m_meas_samples[5].at(i) > VarPHASE_DIFF_THR.value()) && (m_meas_samples[7].at(i) > VarPHASE_DIFF_THR.value()))
                    {
                        phase_diff_temp = m_meas_samples[6].at(i) - m_meas_samples[4].at(i) + 512.0;

                        if (phase_diff_temp > 1023)
                        {
                            phase_diff_temp = phase_diff_temp - 1023;
                        }
                        if (phase_diff_temp < 0)
                        {
                            phase_diff_temp = phase_diff_temp + 1023;
                        }

                        if (VarPHASE_DIFF_34_INVERT.value() == 1)
                        {
                        phase_diff_temp = 1023 - phase_diff_temp;
                        }
                        m_meas_samples[m_meas_slot_number - 1].append(phase_diff_temp);
                        phase1.append(m_meas_samples[4].at(i));
                        phase2.append(m_meas_samples[6].at(i));
                    }
                    else
                    {
                        phase1.append(0);
                        phase2.append(0);
                        m_meas_samples[m_meas_slot_number - 1].append(512.0);
                    }
                }
                m_meas_samples[4] = phase1;
                m_meas_samples[6] = phase2;
            }
        }


    }
}
void Measurement::ShowIQtab(void)
{
    ui->tabWidget->setTabEnabled(5, true);     //IQ data
    ui->tabWidget->setTabText(5,"I/Q data");
}

void Measurement::Show4DULStab(void)
{
    ui->tabWidget->setTabEnabled(4, true);     //4D ULS
    ui->tabWidget->setTabText(4,"4D ULS");
}
void Measurement::CheckVersionConfig()
{
    QString filename = "dsi3pas.cfg";
    QFile file(filename);
    QString input;
    QString iq4D;
    QStringList qsl_input;


    if (!file.exists())
    {

    }
    else if(!file.open(QIODevice::ReadOnly))
    {

    }
    else
    {
        QTextStream in(&file);
        input = in.readLine();  // slave version
        iq4D = in.readLine();   // master version
        iq4D = in.readLine();   // IQ data, 4DULS

        qsl_input = iq4D.split(",");

        if (qsl_input.size() == 2)
        {
            if (qsl_input.at(0) == "IQDATA"){
                ShowIQtab();
            }
            if (qsl_input.at(1) == "4DULS"){
                Show4DULStab();
            }
        }
    }
}
