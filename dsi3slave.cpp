#include "dsi3slave.h"
#include "ui_dsi3slave.h"
#include "dsi3slave_regmap.h"
#include "mainapp.h"
#include "dsi3master.h"
#include "measurement.h"
#include "uartuartcontroller.h"
#include "uartspicontroller.h"
#include "uartssicontroller.h"
#include "asicmaster.h"
#include "rdummaster.h"

#define LOG_TYPE_INFO       (4)
#define LOG_TYPE_DSI3_CMD   (5)
#define LOG_TYPE_ERROR      (6)
#define LOG_TYPE_BUTTON     (7)
#define LOG_TYPE_GPIO       (10)
#define LOG_TYPE_DSI3_TX    (12)
#define LOG_TYPE_DSI3_RX    (13)

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

DSI3Slave *gp_DSI3Slave;


DSI3Slave::DSI3Slave(QWidget *parent) :
    Applet(parent)
{
    ui = new Ui::DSI3Slave;
    ui->setupUi(this);
    createCustomUi();
    setWindowIcon(QIcon(":/board/onlogo.ico"));
    setWindowTitle("DSI3 Slave");
    m_PrevVERSION = VERSION_MOBE;
    m_block_update_version = false;
    m_memory = 0;
}

DSI3Slave::~DSI3Slave()
{
    delete ui;
}

bool DSI3Slave::initialize()
{
    int i, j;

    // COPY ACCESSIBLE NAMES//////////////////////////////////////////////

    CopyAccessNames(0);
    CopyAccessNames(2);

    // INIT REGISTER MAP//////////////////////////////////////////////

    InitRegisterMap(0);
    InitRegisterMap(2);

    // INIT SCALE MAP//////////////////////////////////////////////

    InitScaleMap();

    // INIT COMMAND REGISTERS //////////////////////////////////////////////

    SWriteREAD.makeSetup(SWriteREAD_INIT);
    addDataNode(&SWriteREAD);
    SReadREAD.makeSetup(SReadREAD_INIT);
    addDataNode(&SReadREAD);
    SWriteWRITERWP.makeSetup(SWriteWRITERWP_INIT);
    addDataNode(&SWriteWRITERWP);
    SReadWRITERWP.makeSetup(SReadWRITERWP_INIT);
    addDataNode(&SReadWRITERWP);
    SWriteWRITE8.makeSetup(SWriteWRITE8_INIT);
    addDataNode(&SWriteWRITE8);
    SReadWRITE8.makeSetup(SReadWRITE8_INIT);
    addDataNode(&SReadWRITE8);
    SWriteWRITE.makeSetup(SWriteWRITE_INIT);
    addDataNode(&SWriteWRITE);
    SReadWRITE.makeSetup(SReadWRITE_INIT);
    addDataNode(&SReadWRITE);
    SWriteWRITEP.makeSetup(SWriteWRITEP_INIT);
    addDataNode(&SWriteWRITEP);
    SReadWRITEP.makeSetup(SReadWRITEP_INIT);
    addDataNode(&SReadWRITEP);
    SWriteWRITEBLKP.makeSetup(SWriteWRITEBLKP_INIT);
    addDataNode(&SWriteWRITEBLKP);
    SWritePROGRAM.makeSetup(SWritePROGRAM_INIT);
    addDataNode(&SWritePROGRAM);
    SReadPROGRAM.makeSetup(SReadPROGRAM_INIT);
    addDataNode(&SReadPROGRAM);
    SWriteMEASURE.makeSetup(SWriteMEASURE_INIT);
    addDataNode(&SWriteMEASURE);
    SWriteMEASUREold.makeSetup(SWriteMEASUREold_INIT);
    addDataNode(&SWriteMEASUREold);
    SWriteMEASUREadd.makeSetup(SWriteMEASUREadd_INIT);
    addDataNode(&SWriteMEASUREadd);
    SWriteDISCOVERY.makeSetup(SWriteDISCOVERY_INIT);
    addDataNode(&SWriteDISCOVERY);
    SWriteSWRST.makeSetup(SWriteSWRST_INIT);
    addDataNode(&SWriteSWRST);
    SMODES.makeSetup(SMODES_INIT);
    addDataNode(&SMODES);
    // INIT OTHER REGISTERS //////////////////////////////////////////////

    SDBG.clear();
    Vardbg_out_sel.makeSetup("@variable=Vardbg_out_sel" "@value=0" "@min=0" "@max=63");
    addDataNode(&Vardbg_out_sel);
    SDBG.append(&Vardbg_out_sel);
    Vardbg_out_sel_MODE1.makeSetup("@variable=Vardbg_out_sel_MODE1" "@value=0" "@min=0" "@max=63");
    addDataNode(&Vardbg_out_sel_MODE1);
    SDBG.append(&Vardbg_out_sel_MODE1);
    Vardbg_out_sel_MODE2.makeSetup("@variable=Vardbg_out_sel_MODE2" "@value=0" "@min=0" "@max=63");
    addDataNode(&Vardbg_out_sel_MODE2);
    SDBG.append(&Vardbg_out_sel_MODE2);
    Vardbg_out_sel_MODE3.makeSetup("@variable=Vardbg_out_sel_MODE3" "@value=0" "@min=0" "@max=63");
    addDataNode(&Vardbg_out_sel_MODE3);
    SDBG.append(&Vardbg_out_sel_MODE3);
    Vardbg_out_sel_MODE4.makeSetup("@variable=Vardbg_out_sel_MODE4" "@value=0" "@min=0" "@max=63");
    addDataNode(&Vardbg_out_sel_MODE4);
    SDBG.append(&Vardbg_out_sel_MODE4);
    Vardbg_out_sel_MODE5.makeSetup("@variable=Vardbg_out_sel_MODE5" "@value=0" "@min=0" "@max=63");
    addDataNode(&Vardbg_out_sel_MODE5);
    SDBG.append(&Vardbg_out_sel_MODE5);

    SCOMP.clear();
    Varcomp_method.makeSetup("@variable=Varcomp_method" "@value=0" "@min=0" "@max=15");
    addDataNode(&Varcomp_method);
    SCOMP.append(&Varcomp_method);
    Varcomp_method_MODE1.makeSetup("@variable=Varcomp_method_MODE1" "@value=0" "@min=0" "@max=15");
    addDataNode(&Varcomp_method_MODE1);
    SCOMP.append(&Varcomp_method_MODE1);
    Varcomp_method_MODE2.makeSetup("@variable=Varcomp_method_MODE2" "@value=0" "@min=0" "@max=15");
    addDataNode(&Varcomp_method_MODE2);
    SCOMP.append(&Varcomp_method_MODE2);
    Varcomp_method_MODE3.makeSetup("@variable=Varcomp_method_MODE3" "@value=0" "@min=0" "@max=15");
    addDataNode(&Varcomp_method_MODE3);
    SCOMP.append(&Varcomp_method_MODE3);
    Varcomp_method_MODE4.makeSetup("@variable=Varcomp_method_MODE4" "@value=0" "@min=0" "@max=15");
    addDataNode(&Varcomp_method_MODE4);
    SCOMP.append(&Varcomp_method_MODE4);
    Varcomp_method_MODE5.makeSetup("@variable=Varcomp_method_MODE5" "@value=0" "@min=0" "@max=15");
    addDataNode(&Varcomp_method_MODE5);
    SCOMP.append(&Varcomp_method_MODE5); 

    VarSID.makeSetup("@variable=VarSID" "@value=0" "@min=0" "@max=15");
    addDataNode(&VarSID);
    piezo_model_shift.makeSetup("@variable=piezo_model_shift" "@value=1" "@min=0" "@max=15");
    addDataNode(&piezo_model_shift);

    VarCH1ID1.makeSetup("@variable=VarCH1ID1" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH1ID1);
    VarCH1ID2.makeSetup("@variable=VarCH1ID2" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH1ID2);
    VarCH1ID3.makeSetup("@variable=VarCH1ID3" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH1ID3);
    VarCH1ID4.makeSetup("@variable=VarCH1ID4" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH1ID4);
    VarCH1ID5.makeSetup("@variable=VarCH1ID5" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH1ID5);
    VarCH1ID6.makeSetup("@variable=VarCH1ID6" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH1ID6);
    VarCH2ID1.makeSetup("@variable=VarCH2ID1" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH2ID1);
    VarCH2ID2.makeSetup("@variable=VarCH2ID2" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH2ID2);
    VarCH2ID3.makeSetup("@variable=VarCH2ID3" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH2ID3);
    VarCH2ID4.makeSetup("@variable=VarCH2ID4" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH2ID4);
    VarCH2ID5.makeSetup("@variable=VarCH2ID5" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH2ID5);
    VarCH2ID6.makeSetup("@variable=VarCH2ID6" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarCH2ID6);

    VarDontUpdateCRC.makeSetup("@variable=VarDontUpdateCRC" "@value=0" "@min=0" "@max=1");
    addDataNode(&VarDontUpdateCRC);

    Bitstream2022.makeSetup("@variable=Bitstream2022" "@value=0" "@min=0" "@max=2");
    addDataNode(&Bitstream2022);

    VarSwitches.makeSetup("@variable=VarSwitches" "@value=0" "@min=0" "@max=1" "@descr=Enable addressing by switches");
    addDataNode(&VarSwitches);

    //ShadowComp_method.makeSetup("@variable=ShadowComp_method" "@value=0" "@min=0" "@max=5");

    for (i = 0; i < 6; i++)
    {
        m_slave_connected[0][i] = false;
        m_slave_connected[1][i] = false;
    }

    addDataNode(&gp_DSI3Master->MREG02);
    addDataNode(&gp_DSI3Master->MREG03);
    addDataNode(&gp_DSI3Master->MREG10);
    addDataNode(&gp_DSI3Master->CHANNEL);
    addDataNode(&gp_DSI3Master->MVar_version);

    CheckVersionConfig();

    slot_VersionUpdate();

    return true;
}

void DSI3Slave::createCustomUi()
{
    SLAVES_CONNECTED.clear();
    SLAVES_CONNECTED.append(ui->checkBoxCH1ID1);
    SLAVES_CONNECTED.append(ui->checkBoxCH1ID2);
    SLAVES_CONNECTED.append(ui->checkBoxCH1ID3);
    SLAVES_CONNECTED.append(ui->checkBoxCH1ID4);
    SLAVES_CONNECTED.append(ui->checkBoxCH1ID5);
    SLAVES_CONNECTED.append(ui->checkBoxCH1ID6);
    SLAVES_CONNECTED.append(ui->checkBoxCH2ID1);
    SLAVES_CONNECTED.append(ui->checkBoxCH2ID2);
    SLAVES_CONNECTED.append(ui->checkBoxCH2ID3);
    SLAVES_CONNECTED.append(ui->checkBoxCH2ID4);
    SLAVES_CONNECTED.append(ui->checkBoxCH2ID5);
    SLAVES_CONNECTED.append(ui->checkBoxCH2ID6);

    CHID_CONNECTED.clear();
    CHID_CONNECTED.append(&VarCH1ID1);
    CHID_CONNECTED.append(&VarCH1ID2);
    CHID_CONNECTED.append(&VarCH1ID3);
    CHID_CONNECTED.append(&VarCH1ID4);
    CHID_CONNECTED.append(&VarCH1ID5);
    CHID_CONNECTED.append(&VarCH1ID6);
    CHID_CONNECTED.append(&VarCH2ID1);
    CHID_CONNECTED.append(&VarCH2ID2);
    CHID_CONNECTED.append(&VarCH2ID3);
    CHID_CONNECTED.append(&VarCH2ID4);
    CHID_CONNECTED.append(&VarCH2ID5);
    CHID_CONNECTED.append(&VarCH2ID6);

    connect(ui->comboBoxVersion, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_VersionUpdate()));
    connect(this, SIGNAL(signal_RegisterChanged(QString)), this, SLOT(slot_UpdateLabels()));

    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidgetMODE->setCurrentIndex(0);
    ui->tabWidgetMODEv2->setCurrentIndex(0);

    //ui->tabWidget->widget(3)->setVisible(false);
    //ui->tabWidget->widget(3)->setEnabled(false);
    //ui->tabWidget->widget(3)->setHidden(true);
    ui->tabWidget->setTabEnabled(3, false);

    ui->ptGain->addGraph();
    ui->ptGain->graph(0)->setPen(QPen(QColor::fromRgb(84, 185, 72)));
    ui->ptGain->addGraph();
    ui->ptGain->graph(1)->setPen(QPen(Qt::red));
    ui->ptGain->xAxis->setRange(0, 60);
    ui->ptGain->yAxis->setRange(20, 130);
    ui->ptGain->xAxis->setLabel("time [ms]");
    ui->ptGain->replot();

    ui->ptGainv2->addGraph();
    ui->ptGainv2->graph(0)->setPen(QPen(QColor::fromRgb(84, 185, 72)));
    ui->ptGainv2->addGraph();
    ui->ptGainv2->graph(1)->setPen(QPen(Qt::red));
    ui->ptGainv2->xAxis->setRange(0, 60);
    ui->ptGainv2->yAxis->setRange(20, 130);
    ui->ptGainv2->xAxis->setLabel("time [ms]");
    ui->ptGainv2->replot();
}


/****************************************************************************
 *                              FUNCTIONALITY
*****************************************************************************/


// updates all RELAYS and GPIOs
bool DSI3Slave::updateHardware(Direction direction, const QString &item)
{

    return false;
}

QVariant DSI3Slave::FUNCTION_LIST()
{
        FUNCTION("Update", "This function updates hardware"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            return updateHardware(HARDWARE_GET,"*");
        }

        FUNCTION("PerformREAD", "This function performs READ command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Perform READ");
            return PerformREAD(true, 0, 0, 0, 0);
        }

        FUNCTION("PerformWRITE_RWP", "This function performs WRITE_RWP command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Perform WRITE_RWP");
            return PerformWRITE_RWP(true, 0, 0, 0, 0, 0, 0);
        }

        FUNCTION("PerformWRITE_8", "This function performs WRITE_8 command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Perform WRITE_8");
            return PerformWRITE_8(true, 0, 0, 0);
        }

        FUNCTION("PerformWRITE", "This function performs WRITE command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Perform WRITE");
            return PerformWRITE(true, 0, 0);
        }

        FUNCTION("PerformWRITE_P", "This function performs WRITE_P command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Perform WRITE_P");
            return PerformWRITE_P(true, 0, 0);
        }

        FUNCTION("PerformWRITE_BLK_P", "This function performs WRITE_BLK_P command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Perform WRITE_BLK_P");
            return PerformWRITE_BLK_P(true, 0, 0, 0, 0, 0, 0, 0);
        }

        FUNCTION("PerformPROGRAM", "This function performs PROGRAM command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Perform PROGRAM");
            return PerformPROGRAM(true, 0);
        }

        FUNCTION("PerformMEASURE", "This function performs MEASURE command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Perform MEASURE");
            return PerformMEASURE(false);
        }

        FUNCTION("PerformDISCOVERY", "This function performs DISCOVERY command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Perform DISCOVERY");
            return PerformDISCOVERY();
        }

        FUNCTION("PerformSWRST", "This function performs SW_RST command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Perform SW_RST");
            return PerformSWRST(true, 0);
        }

        FUNCTION("PerformUnlockAndSWRST", "This function performs unlock_sw_rst and SW_RST command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Perform Unlock and SW_RST");
            return PerformUnlockAndSWRST();
        }

        FUNCTION("AddressDISCOVERY", "This function performs DISCOVERY command on DSI3"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Address using DISCOVERY");
            return AddressDISCOVERY();
        }

        FUNCTION("AddressDEMUXES", "This function address slaves using demuxes"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Address using demuxes");
            return AddressDEMUXES();
        }

        FUNCTION("AddressSWITCHES", "This function address slaves using switches"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Address using demuxes");
            return AddressSWITCHES();
        }


        FUNCTION("ReadReadOnlyRegs", "This function reads the ReadOnly registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Read ReadOnly registers");
            return ReadSlaveRegs("ReadOnly");
        }

        FUNCTION("ReadCommonRegs", "This function reads the Common registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Read Common registers");
            return ReadSlaveRegs("Common");
        }

        FUNCTION("WriteCommonRegs", "This function writes the Common registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Write Common registers");
            return WriteSlaveRegs("Common");
        }

        FUNCTION("ReadGpcontrolReg", "This function reads the gp_control register"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Read gp_control register");
            return ReadSlaveRegs("Gpcontrol");
        }

        FUNCTION("WriteGpcontrolReg", "This function writes the gp_control register"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Write gp_control register");
            return WriteSlaveRegs("Gpcontrol");
        }

        FUNCTION("ReadTrimRegs", "This function reads the Frequency trimming registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Read Frequency trimming registers");
            return ReadSlaveRegs("Trim");
        }

        FUNCTION("WriteTrimRegs", "This function writes the Frequency trimming registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Write Frequency trimming registers");
            return WriteSlaveRegs("Trim");
        }

        FUNCTION("ReadDSI3Regs", "This function reads the DSI3 registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Read DSI3 registers");
            return ReadSlaveRegs("DSI3");
        }

        FUNCTION("WriteDSI3Regs", "This function writes the DSI3 registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Write DSI3 registers");
            return WriteSlaveRegs("DSI3");
        }

        FUNCTION("WriteDSI3RegsAll", "This function writes the DSI3 registers to all connected slaves"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Write DSI3 registers to all slaves");
            return WriteDSI3RegsAll();
        }

        FUNCTION("ReadModeRegs", "This function reads the Mode registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Read Mode registers");
            ReadSlaveRegs("CFAR");
            return ReadSlaveRegs("Mode");
        }

        FUNCTION("WriteModeRegs", "This function writes the Mode registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Write Mode registers");
            WriteSlaveRegs("CFAR");
            return WriteSlaveRegs("Mode");
        }

        FUNCTION("ReadSafetyRegs", "This function reads Safety registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Read Safety registers");
            return ReadSlaveRegs("Safety");
        }

        FUNCTION("ClearSafetyRegs", "This function cleares Safety registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Clear Safety registers");
            return ClearSafetyFlags(false);
        }

        FUNCTION("ClearSafetyFlags", "This function clears Safety flags"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Clear Safety Flags");
            return ClearSafetyFlags(false);
        }

        FUNCTION("CheckSlaves", "Check connected slaves"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Check slaves");
            CheckSlaves();
            return true;
        }

        FUNCTION("Synchronize", "Perform 8x WRITE_BLK_P to synchronize slaves"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Synchronize slaves");
            return Synchronize();
        }

        FUNCTION("Copy", "Copy content between selected MODEs"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Copy modes");
            CopyModes();

        }

        FUNCTION("CopyMemoryv1v2", "Copy content between selected MODEs"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Copy memory v1 to v2");
            CopyMem();
        }

        FUNCTION("QuickConfiguration", "Performs quick configuration"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Quick configuration");
            if (gp_Settings->mVersion == VERSION_UPID)
            {
                gp_ASICMaster->runAppCommand("QuickConfiguration","","","","","");
            }
            if ((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
            {
                gp_RDUMMaster->runAppCommand("QuickConfiguration","","","","","");
            }
            else
            {
                gp_DSI3Master->QuickConfiguration();
            }
        }
        FUNCTION("CalculateCustNVM", "Calculates Customer area NVM CRC"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            AddCRCNvmCust();
            return true;
        }

        FUNCTION("WriteNVMRegs", "Write Customer area NVM data"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Write Customer NVM Registers");
            if(ui->cb_CustNvmCRCAutoCalc->isChecked()) {
                AddCRCNvmCust();    //Calculate CRC before writing
            }
            //Writing regs
            WriteSlaveRegs("NVMCust");
            return true;
        }

        FUNCTION("ReadNVMRegs", "Read Customer area NVM data"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Read Customer NVM Registers");
            //Reading regs
            ReadSlaveRegs("NVMCust");
            bool readres = false;
            //Verify CRC of read data. Save read CRC, calculate CRC over read data, compare CRCs, return read CRC into GUI if there is a mismatch
            quint16 readcrc = SREGX[m_memory].at(m_map_sregs[m_memory][1][0x1A])->value(0,15);
            AddCRCNvmCust();    //Will replace read CRC in GUI with calculated one
            quint16 calccrc = SREGX[m_memory].at(m_map_sregs[m_memory][1][0x1A])->value(0,15);

            if(readcrc == calccrc) {
                readres = true;
            }
            else {
                SREGX[m_memory].at(m_map_sregs[m_memory][1][0x1A])->setValue(0, 15, readcrc);
            }
            ui->cb_CustNvmReadOK->setChecked(readres);

            return true;
        }
        FUNCTION("ReadPage0", "Read memory page 0 using PDCM"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Read page 0 using PDCM");
            return ReadPageUsingPDCM(0);
        }
        FUNCTION("ReadPage1", "Read memory page 1 using PDCM"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "DSI3 Slave - Read page 1 using PDCM");
            return ReadPageUsingPDCM(1);
        }



    return false;
}


void DSI3Slave::setupBoard()
{
}


void DSI3Slave::suspend()
{
}

void DSI3Slave::resume()
{
    slot_VersionUpdate();
    setupBoard();
    slot_UpdateLabels();
}
//****************************************************************************
// @function name: slot_UpdateLabels()
// @description: Update all labels in GUI with recalculated values
//****************************************************************************
void DSI3Slave::slot_UpdateLabels()
{
    const double rev_sh[4] = {1.4, 2, 2.8, 7};
    const quint32 gain_lna[4] = {20, 30, 40, 40};
    const double delta_gain_tim[] = {0.1, 0.2, 0.4, 0.8, 1.2, 1.6, 2, 2.5, 3.3, 4.5, 5.9, 8, 10, 12.7, 16, 20};
    QByteArray qba_temp;
    int meas_type;
    int type;
    int offset;

    if (m_memory == 0)
    {
        // common regs
        ui->labelovt_threshold->setText(QString("%1 dgC").arg(GET_REG("SREGX126[ovt_threshold]").toInt() - 55));
        ui->labeldie_temp->setText(QString("%1 dgC").arg(GET_REG("SREGX126[die_temp]").toInt() - 55));

        // dsi3 configuration
        ui->labelpdcm_size->setText(QString("%1 B").arg((GET_REG("SREGX120[pdcm_size]").toUInt() + 1) * 4));
        ui->labelt_chip->setText(QString("%1 us").arg(GET_REG("SREGX120[t_chip]").toUInt() + 3));
        ui->labelcrc_mode->setText(QString("%1 b").arg((GET_REG("SREGX120[crc_mode]").toUInt() + 1) * 8));

        if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUS) && (ui->comboBoxVersion->currentText() != "-"))
        {
            ui->labeldsi3pas_vpp->setText(QString("%1 Vpp").arg((1 + GET_REG("SREGX120[dsi3pas_vpp]").toUInt()) * 2));
            if (GET_REG("SREGX120[dsi3pas_ref]").toInt() == 1)
                ui->labeldsi3pas_ref->setText("Vbat/2");
            else
                ui->labeldsi3pas_ref->setText("GND");
        }
        else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUSDMX) && (ui->comboBoxVersion->currentText() != "-"))
        {
            ui->labeldsi3pas_vpp->setText(QString("%1 Vpp").arg((1 + GET_REG("SREGX120[dsi3pas_vpp]").toUInt()) * 2));
            if (GET_REG("SREGX120[dsi3pas_ref]").toInt() == 1)
                ui->labeldsi3pas_ref->setText("Vbat/2");
            else
                ui->labeldsi3pas_ref->setText("GND");
        }
        else
        {
            ui->labeldsi3pas_vpp->setText(QString("%1 Vpp").arg((2 - GET_REG("SREGX120[dsi3pas_vpp]").toUInt()) * 2));
            if (GET_REG("SREGX120[dsi3pas_ref]").toInt() == 0)
                ui->labeldsi3pas_ref->setText("Vbat/2");
            else
                ui->labeldsi3pas_ref->setText("GND");
        }

        // mode regs
        ui->labeltx_current_h->setText(QString("%1 mA").arg((GET_REG("SREGX048[tx_current_h]").toFloat()) * 7.9365 / 2, 0, 'f', 0));
        ui->labeltx_current_l->setText(QString("%1 mA").arg((GET_REG("SREGX048[tx_current_l]").toFloat()) * 7.9365 / 2, 0, 'f', 0));
        ui->labelmeas_duration->setText(QString("%1 ms").arg((GET_REG("SREGX058[meas_duration]").toFloat()) * 0.512, 0, 'f', 1));
        ui->labelInit_phase_dur->setText(QString("%1 ms").arg((GET_REG("SREGX04E[init_phase_dur]").toFloat()) * 0.512, 0, 'f', 1));
        ui->labelrev_end_timeout->setText(QString("%1 us").arg((GET_REG("SREGX04C[rev_end_timeout]").toFloat()) * 51.2, 0, 'f', 0));
        ui->labelrev_sh->setText(QString("%1 Ohm").arg(rev_sh[(GET_REG("SREGX050[rev_sh]").toUInt()) & 0x03], 0, 'f', 1));
        ui->labelgain_lna->setText(QString("%1 dB").arg(gain_lna[GET_REG("SREGX04C[gain_lna]").toUInt()]));
        ui->labeldyn_gain_offset->setText(QString("%1 dB").arg((GET_REG("SREGX04A[dyn_gain_offset]").toFloat() * 0.38), 0, 'f', 1));
        ui->labelvar_gain_start->setText(QString("%1 ms").arg((GET_REG("SREGX054[var_gain_start]").toFloat() * 0.4096), 0, 'f', 1));
        ui->labeldelta_gain_val0->setText(QString("%1 dB").arg((GET_REG("SREGX04E[delta_gain_val0]").toFloat() * 0.38), 0, 'f', 1));
        ui->labeldelta_gain_tim0->setText(QString("%1 ms").arg(delta_gain_tim[(GET_REG("SREGX04E[delta_gain_tim0]").toUInt())], 0, 'f', 1));
        ui->labeldelta_gain_val1->setText(QString("%1 dB").arg((GET_REG("SREGX050[delta_gain_val1]").toFloat() * 0.38), 0, 'f', 1));
        ui->labeldelta_gain_tim1->setText(QString("%1 ms").arg(delta_gain_tim[(GET_REG("SREGX050[delta_gain_tim1]").toUInt())], 0, 'f', 1));
        ui->labeldelta_gain_val2->setText(QString("%1 dB").arg((GET_REG("SREGX052[delta_gain_val2]").toFloat() * 0.38), 0, 'f', 1));
        ui->labeldelta_gain_tim2->setText(QString("%1 ms").arg(delta_gain_tim[(GET_REG("SREGX052[delta_gain_tim2]").toUInt())], 0, 'f', 1));
        ui->labeldelta_gain_tim3->setText(QString("%1 ms").arg(delta_gain_tim[(GET_REG("SREGX054[delta_gain_tim3]").toUInt())], 0, 'f', 1));
        ui->label_dir_cfar_der_size->setText(QString("%1").arg((GET_REG("SREGX05A[dir_cfar_der_size]").toUInt() + 1) * 2));
        ui->label_indir_cfar_der_size->setText(QString("%1").arg((GET_REG("SREGX05C[indir_cfar_der_size]").toUInt() + 1) * 2));

        meas_type = Getmeas_type_old(0);

        if (meas_type == 29)
        {
            ui->labeldelta_gain_val3->setText(QString("%1 Hz").arg((1/((GET_REG("SREGX054[delta_gain_val3]").toFloat()+0.00001) * 0.0000004)), 0, 'f', 0));

            ui->labeldelta_gain_val4->setText(QString("%1 Hz").arg((1/((GET_REG("SREGX056[delta_gain_val4]").toFloat()+0.00001) * 0.0000004)), 0, 'f', 0));

            type = GET_REG("SREGX056[delta_gain_tim4]").toInt();
            if ((type == 0) || (type == 4))
                ui->labeldelta_gain_tim4->setText(QString("up-chirp"));
            else if ((type == 1) || (type == 5))
                ui->labeldelta_gain_tim4->setText(QString("down-chirp"));
            else if ((type == 2) || (type == 6))
                ui->labeldelta_gain_tim4->setText(QString("updown-chirp"));
            else
                ui->labeldelta_gain_tim4->setText(QString("WRONG"));
        }
        else
        {
            ui->labeldelta_gain_val3->setText(QString("%1 dB").arg((GET_REG("SREGX054[delta_gain_val3]").toFloat() * 0.38), 0, 'f', 1));
            ui->labeldelta_gain_val4->setText(QString("%1 dB").arg((GET_REG("SREGX056[delta_gain_val4]").toFloat() * 0.38), 0, 'f', 1));
            ui->labeldelta_gain_tim4->setText(QString("%1 ms").arg(delta_gain_tim[(GET_REG("SREGX056[delta_gain_tim4]").toUInt())], 0, 'f', 1));
        }

        if (GET_REG("SREGX042[tx_per_h]").toInt() >= 10)
            ui->labeltx_per_h->setText(QString("%1 kHz").arg( 1 / (GET_REG("SREGX042[tx_per_h]").toFloat()*0.000025), 0, 'f', 1));
        else
            ui->labeltx_per_h->setText("0 kHz");
        if (GET_REG("SREGX040[tx_per_l]").toInt() >= 10)
            ui->labeltx_per_l->setText(QString("%1 kHz").arg( 1 / (GET_REG("SREGX040[tx_per_l]").toFloat()*0.000025), 0, 'f', 1));
        else
            ui->labeltx_per_l->setText("0 kHz");
    }
    else if (m_memory == 1)
    {
        // common regs
        ui->labelovt_thresholdv2->setText(QString("%1 dgC").arg(GET_REG("SREGXV2126[ovt_threshold]").toInt() - 55));
        ui->labeldie_tempv2->setText(QString("%1 dgC").arg(GET_REG("SREGXV2126[die_temp]").toInt() - 55));

        // dsi3 configuration
        ui->labelpdcm_sizev2->setText(QString("%1 B").arg((GET_REG("SREGXV2120[pdcm_size]").toUInt() + 1) * 4));

        if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUS) && (ui->comboBoxVersion->currentText() != "-"))
        {
            ui->labeldsi3pas_vppv2->setText(QString("%1 Vpp").arg((1 + GET_REG("SREGXV2120[dsi3pas_vpp]").toUInt()) * 2));
            if (GET_REG("SREGXV2120[dsi3pas_ref]").toInt() == 1)
                ui->labeldsi3pas_refv2->setText("Vbat/2");
            else
                ui->labeldsi3pas_refv2->setText("GND");
        }
        else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUSDMX) && (ui->comboBoxVersion->currentText() != "-"))
        {
            ui->labeldsi3pas_vppv2->setText(QString("%1 Vpp").arg((1 + GET_REG("SREGXV2120[dsi3pas_vpp]").toUInt()) * 2));
            if (GET_REG("SREGXV2120[dsi3pas_ref]").toInt() == 1)
                ui->labeldsi3pas_refv2->setText("Vbat/2");
            else
                ui->labeldsi3pas_refv2->setText("GND");
        }
        else
        {
            ui->labeldsi3pas_vppv2->setText(QString("%1 Vpp").arg((2 - GET_REG("SREGXV2120[dsi3pas_vpp]").toUInt()) * 2));
            if (GET_REG("SREGXV2120[dsi3pas_ref]").toInt() == 0)
                ui->labeldsi3pas_refv2->setText("Vbat/2");
            else
                ui->labeldsi3pas_refv2->setText("GND");
        }

        // mode regs
        ui->labeltx_current_hv2->setText(QString("%1 mA").arg((GET_REG("SREGXV2048[tx_current_h]").toFloat()) * 7.9365 / 2, 0, 'f', 0));
        ui->labeltx_current_lv2->setText(QString("%1 mA").arg((GET_REG("SREGXV2048[tx_current_l]").toFloat()) * 7.9365 / 2, 0, 'f', 0));

        ui->labelrev_end_timeoutv2->setText(QString("%1 us").arg((GET_REG("SREGXV204C[rev_end_timeout]").toFloat()) * 51.2, 0, 'f', 0));
        ui->labelrev_shv2->setText(QString("%1 Ohm").arg(rev_sh[(GET_REG("SREGXV2050[rev_sh]").toUInt()) & 0x03], 0, 'f', 1));
        ui->labelgain_lnav2->setText(QString("%1 dB").arg(gain_lna[GET_REG("SREGXV204C[gain_lna]").toUInt()]));
        //int offset = (GET_REG("SREGXV2040[dyn_gain_offset_6]").toInt() << 6) + GET_REG("SREGXV2042[dyn_gain_offset_50]").toInt();

        if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUSBSD) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBE2DMX))
        {
            ui->labelmeas_durationv2->setText(QString("%1 ms").arg((GET_REG("SREGXV204A[7:0]").toFloat()) * 0.512, 0, 'f', 1));
        }
        else
        {
            ui->labelmeas_durationv2->setText(QString("%1 ms").arg((GET_REG("SREGXV204A[meas_duration]").toFloat()) * 0.512, 0, 'f', 1));
        }


        if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEJUN) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBEDMX) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DCH)\
                 || (ui->comboBoxVersion->currentIndex() == VERSION_RDUSBSD) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DMX) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBE2DMX))
        {
            offset = GET_REG("SREGXV2042[15:9]").toInt();
        }
        else
        {
            offset = (GET_REG("SREGXV2040[dyn_gain_offset_6]").toInt() << 6) + GET_REG("SREGXV2042[dyn_gain_offset_50]").toInt();
        }

        ui->labeldyn_gain_offsetv2->setText(QString("%1 dB").arg((offset * 0.38), 0, 'f', 1));
        ui->labelvar_gain_startv2->setText(QString("%1 ms").arg((GET_REG("SREGXV2054[var_gain_start]").toFloat() * 0.4096), 0, 'f', 1));
        ui->labeldelta_gain_val0v2->setText(QString("%1 dB").arg((GET_REG("SREGXV204E[delta_gain_val0]").toFloat() * 0.38), 0, 'f', 1));
        ui->labeldelta_gain_tim0v2->setText(QString("%1 ms").arg(delta_gain_tim[(GET_REG("SREGXV204E[delta_gain_tim0]").toUInt())], 0, 'f', 1));
        ui->labeldelta_gain_val1v2->setText(QString("%1 dB").arg((GET_REG("SREGXV2050[delta_gain_val1]").toFloat() * 0.38), 0, 'f', 1));
        ui->labeldelta_gain_tim1v2->setText(QString("%1 ms").arg(delta_gain_tim[(GET_REG("SREGXV2050[delta_gain_tim1]").toUInt())], 0, 'f', 1));
        ui->labeldelta_gain_val2v2->setText(QString("%1 dB").arg((GET_REG("SREGXV2052[delta_gain_val2]").toFloat() * 0.38), 0, 'f', 1));
        ui->labeldelta_gain_tim2v2->setText(QString("%1 ms").arg(delta_gain_tim[(GET_REG("SREGXV2052[delta_gain_tim2]").toUInt())], 0, 'f', 1));
        ui->labeldelta_gain_tim3v2->setText(QString("%1 ms").arg(delta_gain_tim[(GET_REG("SREGXV2054[delta_gain_tim3]").toUInt())], 0, 'f', 1));

        meas_type = Getmeas_type_old(0);

        if (meas_type == 29)
        {
            ui->labeldelta_gain_val3v2->setText(QString("%1 Hz").arg((1/((GET_REG("SREGXV2054[delta_gain_val3]").toFloat()+0.00001) * 0.0000004)), 0, 'f', 0));

            ui->labeldelta_gain_val4v2->setText(QString("%1 Hz").arg((1/((GET_REG("SREGXV2056[delta_gain_val4]").toFloat()+0.00001) * 0.0000004)), 0, 'f', 0));

            type = GET_REG("SREGXV2056[delta_gain_tim4]").toInt();
            if (type == 0)
                ui->labeldelta_gain_tim4v2->setText(QString("up-chirp"));
            else if (type == 1)
                ui->labeldelta_gain_tim4v2->setText(QString("down-chirp"));
            else if (type == 2)
                ui->labeldelta_gain_tim4v2->setText(QString("updown-chirp"));
            else
                ui->labeldelta_gain_tim4v2->setText(QString("WRONG"));
        }
        else
        {
            ui->labeldelta_gain_val3v2->setText(QString("%1 dB").arg((GET_REG("SREGXV2054[delta_gain_val3]").toFloat() * 0.38), 0, 'f', 1));
            ui->labeldelta_gain_val4v2->setText(QString("%1 dB").arg((GET_REG("SREGXV2056[delta_gain_val4]").toFloat() * 0.38), 0, 'f', 1));
            ui->labeldelta_gain_tim4v2->setText(QString("%1 ms").arg(delta_gain_tim[(GET_REG("SREGXV2056[delta_gain_tim4]").toUInt())], 0, 'f', 1));
        }



        if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEJUN) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBEDMX) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DCH)\
                 || (ui->comboBoxVersion->currentIndex() == VERSION_RDUSBSD) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DMX) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBE2DMX))
        {
            if ((GET_REG("SREGXV2042[8:0]").toInt() != 0) || (GET_REG("SREGXV2040[15]").toInt() != 0))
                ui->labeltx_per_hv2->setText(QString("%1 kHz").arg( 1 / ((256 + GET_REG("SREGXV2042[8:0]").toFloat() + (GET_REG("SREGXV2040[15]").toInt() << 9))*0.000025), 0, 'f', 1));
            else
                ui->labeltx_per_hv2->setText("0 kHz");

            if (GET_REG("SREGXV2040[tx_per_l]").toInt() >= 10)
                ui->labeltx_per_lv2->setText(QString("%1 kHz").arg( 1 / ((256 + GET_REG("SREGXV2040[tx_per_l]").toFloat())*0.000025), 0, 'f', 1));
            else
                ui->labeltx_per_lv2->setText("0 kHz");
        }
        else
        {
            if (GET_REG("SREGXV2042[tx_per_h]").toInt() >= 10)
                ui->labeltx_per_hv2->setText(QString("%1 kHz").arg( 1 / ((GET_REG("SREGXV2042[tx_per_h]").toFloat())*0.000025), 0, 'f', 1));
            else
                ui->labeltx_per_hv2->setText("0 kHz");

            if (GET_REG("SREGXV2040[tx_per_l]").toInt() >= 10)
                ui->labeltx_per_lv2->setText(QString("%1 kHz").arg( 1 / ((GET_REG("SREGXV2040[tx_per_l]").toFloat())*0.000025), 0, 'f', 1));
            else
                ui->labeltx_per_lv2->setText("0 kHz");
        }
    }
    slot_GainGraphUpdate();
}

bool DSI3Slave::PerformREAD(bool inreg, int sid, int rd_pg, int rd_idx, int *rsp_data)
{
    bool result = false;

    if (!inreg)
    {
        SWriteREAD["sid"] = sid;
        SWriteREAD["rd_pg"] = rd_pg;
        SWriteREAD["rd_idx"] = rd_idx;
    }
    else
    {
        SWriteREAD["sid"] = VarSID.value();
        ui->checkBoxREAD->setChecked(result);
    }

    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS READ command - CH%1 SID%2 PG%3 IDX%4hex").arg((int)gp_DSI3Master->CHANNEL.value() + 1)\
                       .arg((int)SWriteREAD["sid"]).arg((int)SWriteREAD["rd_pg"]).arg((int)SWriteREAD["rd_idx"],1,16));

    if (GET_REG("VarDontUpdateCRC").toInt() == 0)
    {
        SWriteREAD["crc8"] = CalcSlaveCRCfromREG(SWriteREAD);
    }

    result = PerformCRM(SWriteREAD, &SReadREAD, 4);

    if(inreg)
    {
        ui->checkBoxREAD->setChecked(result);
    }
    else
    {
        if (result)
        {
            *rsp_data = (int)SReadREAD["rsp_data"];
        }
    }

    if ((result == false) && ((int)SWriteREAD["sid"] != 0x0F))
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "No response to DSI3PAS READ command");
    }

    slot_updateAllControls();
    return result;
}

bool DSI3Slave::PerformWRITE_RWP(bool inreg, int sid, int pdcm_upd, int new_pdcm_r_pg,\
                      int crm_upd, int new_crm_w_pg, int new_crm_w_idx)
{
    bool result = false;

    if (!inreg)
    {
        SWriteWRITERWP["sid"] = sid;
        SWriteWRITERWP["pdcm_upd"] = pdcm_upd;
        SWriteWRITERWP["new_pdcm_r_pg"] = new_pdcm_r_pg;
        SWriteWRITERWP["crm_upd"] = crm_upd;
        SWriteWRITERWP["new_crm_w_pg"] = new_crm_w_pg;
        SWriteWRITERWP["new_crm_w_idx"] = new_crm_w_idx;
    }
    else
    {
        SWriteWRITERWP["sid"] = VarSID.value();
        ui->checkBoxWRITERWP->setChecked(result);
    }

    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS WRITE_RWP command - CH%1 SID%2 PUP%3 PPG%4 CUP%5 CPG%6 CIDX%7hex")\
                       .arg((int)gp_DSI3Master->CHANNEL.value() + 1).arg((int)SWriteWRITERWP["sid"]).arg((int)SWriteWRITERWP["pdcm_upd"])\
            .arg((int)SWriteWRITERWP["new_pdcm_r_pg"]).arg((int)SWriteWRITERWP["crm_upd"]).arg((int)SWriteWRITERWP["new_crm_w_pg"])\
            .arg((int)SWriteWRITERWP["new_crm_w_idx"], 1, 16));

    if (GET_REG("VarDontUpdateCRC").toInt() == 0)
    {
        SWriteWRITERWP["crc8"] = CalcSlaveCRCfromREG(SWriteWRITERWP);
    }

    result = PerformCRM(SWriteWRITERWP, &SReadWRITERWP, 4);

    if(inreg)
        ui->checkBoxWRITERWP->setChecked(result);

    if ((result == false) && ((int)SWriteWRITERWP["sid"] != 0x0F))
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "No response to DSI3PAS WRITE_RWP command");
    }
    slot_updateAllControls();
    return result;
}

bool DSI3Slave::PerformWRITE_8(bool inreg, int sid, int w_idx, int data)
{
    bool result = false;   

    if (!inreg)
    {
        SWriteWRITE8["sid"] = sid;
        SWriteWRITE8["w_idx"] = w_idx;
        SWriteWRITE8["data"] = data;
    }
    else
    {
        SWriteWRITE8["sid"] = VarSID.value();
        ui->checkBoxWRITE8->setChecked(result);
    }

    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS WRITE_8 command - CH%1 SID%2")\
                       .arg((int)gp_DSI3Master->CHANNEL.value() + 1).arg((int)SWriteWRITE8["sid"]));

    if (GET_REG("VarDontUpdateCRC").toInt() == 0)
    {
        SWriteWRITE8["crc8"] = CalcSlaveCRCfromREG(SWriteWRITE8);
    }

    result = PerformCRM(SWriteWRITE8, &SReadWRITE8, 4);

    if(inreg)
        ui->checkBoxWRITE8->setChecked(result);

    if ((result == false) && ((int)SWriteWRITE8["sid"] != 0x0F))
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "No response to DSI3PAS WRITE_8 command");
    }
    slot_updateAllControls();
    return result;
}

bool DSI3Slave::PerformWRITE(bool inreg, int sid, int data)
{
    bool result = false;

    if (!inreg)
    {
        SWriteWRITE["sid"] = sid;
        SWriteWRITE["data"] = data;
    }
    else
    {
        SWriteWRITE["sid"] = VarSID.value();
        ui->checkBoxWRITE->setChecked(result);
    }

    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS WRITE command - CH%1 SID%2")\
                       .arg((int)gp_DSI3Master->CHANNEL.value() + 1).arg((int)SWriteWRITE["sid"]));

    if (GET_REG("VarDontUpdateCRC").toInt() == 0)
    {
        SWriteWRITE["crc8"] = CalcSlaveCRCfromREG(SWriteWRITE);
    }

    result = PerformCRM(SWriteWRITE, &SReadWRITE, 4);

    if(inreg)
        ui->checkBoxWRITE->setChecked(result);

    if ((result == false) && ((int)SWriteWRITE["sid"] != 0x0F))
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "No response to DSI3PAS WRITE command");
    }
    slot_updateAllControls();
    return result;
}

bool DSI3Slave::PerformWRITE_P(bool inreg, int sid, int data)
{
    bool result = false;

    if (!inreg)
    {
        SWriteWRITEP["sid"] = sid;
        SWriteWRITEP["data"] = data;
    }
    else
    {
        SWriteWRITEP["sid"] = VarSID.value();
        ui->checkBoxWRITEP->setChecked(result);
    }

    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS WRITE_P command - CH%1 SID%2")\
                       .arg((int)gp_DSI3Master->CHANNEL.value() + 1).arg((int)SWriteWRITEP["sid"]));

    if (GET_REG("VarDontUpdateCRC").toInt() == 0)
    {
        SWriteWRITEP["crc8"] = CalcSlaveCRCfromREG(SWriteWRITEP);
    }

    result = PerformCRM(SWriteWRITEP, &SReadWRITEP, 4);

    if(inreg)
        ui->checkBoxWRITEP->setChecked(result);

    if ((result == false) && ((int)SWriteWRITEP["sid"] != 0x0F))
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "No response to DSI3PAS WRITE_P command");
    }
    slot_updateAllControls();
    return result;
}

bool DSI3Slave::PerformWRITE_BLK_P(bool inreg, int sid, int data0, int data1, int data2,\
                        int data3, int data4, int data5)
{
    bool result = false;

    if (!inreg)
    {
        SWriteWRITEBLKP["sid"] = sid;
        SWriteWRITEBLKP["data0"] = data0;
        SWriteWRITEBLKP["data1"] = data1;
        SWriteWRITEBLKP["data2"] = data2;
        SWriteWRITEBLKP["data3"] = data3;
        SWriteWRITEBLKP["data4"] = data4;
        SWriteWRITEBLKP["data5"] = data5;
    }
    else
    {
        SWriteWRITEBLKP["sid"] = VarSID.value();
    }

    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS WRITE_BLK_P command - CH%1 SID%2")\
                       .arg((int)gp_DSI3Master->CHANNEL.value() + 1).arg((int)SWriteWRITEBLKP["sid"]));

    if (GET_REG("VarDontUpdateCRC").toInt() == 0)
    {
        SWriteWRITEBLKP["crc8"] = CalcSlaveCRCfromREG(SWriteWRITEBLKP);
    }

    QByteArray temp;
    result = PerformCOMMAND(SWriteWRITEBLKP, &temp, 1);

    return result;
}

bool DSI3Slave::PerformPROGRAM(bool inreg, int sid)
{    
    bool result = false;

    if (!inreg)
        SWritePROGRAM["sid"] = sid;
    else
    {
        SWritePROGRAM["sid"] = VarSID.value();
        ui->checkBoxPROGRAM->setChecked(result);
    }

    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS PROGRAM command - CH%1 SID%2")\
                       .arg((int)gp_DSI3Master->CHANNEL.value() + 1).arg((int)SWritePROGRAM["sid"]));

    if (GET_REG("VarDontUpdateCRC").toInt() == 0)
    {
        SWritePROGRAM["crc8"] = CalcSlaveCRCfromREG(SWritePROGRAM);
    }

    result = PerformCRM(SWritePROGRAM, &SReadPROGRAM, 4);

    if(inreg)
        ui->checkBoxPROGRAM->setChecked(result);

    if ((result == false) && ((int)SWritePROGRAM["sid"] != 0x0F))
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "No response to DSI3PAS PROGRAM command");
    }
    slot_updateAllControls();
    return result;
}

bool DSI3Slave::PerformMEASURE(bool brc)
{
    bool result = false;
    QByteArray rxdata;
    Register empty_reg;
    int brcs = 0;

    int num_of_pdcm_slots = GET_REG("MREG10[NUM_OF_PDCM_SLOTS]").toInt() + 1;

    if ((int)gp_DSI3Master->CHANNEL.value() == 0)
    {
        brcs = GET_REG("MREG02[BRC_CNT]").toInt();
    }
    else
    {
        brcs = GET_REG("MREG03[BRC_CNT]").toInt();
    }


    SWriteMEASURE["sid"] = 15;
    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS MEASURE command - CH%1").arg((int)gp_DSI3Master->CHANNEL.value() + 1));

    SWriteMEASUREold.fromByteArray(SWriteMEASURE.toByteArray());
    SWriteMEASUREold["doppler_tx_side"] = (uint)SWriteMEASURE["doppler_tx_side"];
    SWriteMEASUREold["doppler_tx_inner"] = (uint)SWriteMEASURE["doppler_tx_inner"];
    SWriteMEASUREold["doppler_tx_outer"] = (uint)SWriteMEASURE["doppler_tx_outer"];
    SWriteMEASUREold["doppler_rx_side"] = (uint)SWriteMEASURE["doppler_rx_side"];
    SWriteMEASUREold["doppler_rx_inner"] = (uint)SWriteMEASURE["doppler_rx_inner"];
    SWriteMEASUREold["doppler_rx_outer"] = (uint)SWriteMEASURE["doppler_rx_outer"];

    if ((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
    {
        SWriteMEASUREold["rev_sh_ena2"] = (uint)SWriteMEASURE["rev_sh_ena"];
        SWriteMEASUREold["rev_sh2"] = (uint)SWriteMEASURE["rev_sh"];
        SWriteMEASUREold.setValue(8,8,(uint)SWriteMEASUREadd["dyn_gain_restart_ena"]);
        SWriteMEASUREold.setValue(9, 14, (uint)SWriteMEASUREadd.value(1, 6));
        SWriteMEASUREold.setValue(16,23,(uint)SWriteMEASUREadd["beam_start_delay"]);
    }
    else
    {
        SWriteMEASUREold["rev_sh_ena"] = (uint)SWriteMEASURE["rev_sh_ena"];
        SWriteMEASUREold["rev_sh"] = (uint)SWriteMEASURE["rev_sh"];

        SWriteMEASUREold["rev_sh_ena2"] = (uint)0;
        SWriteMEASUREold["rev_sh2"] = (uint)0;
        SWriteMEASUREold.setValue(8,14,(uint)0);
        SWriteMEASUREold.setValue(16,23,(uint)0);
    }

    if (GET_REG("VarDontUpdateCRC").toInt() == 0)
    {
        SWriteMEASURE["crc8"] = CalcSlaveCRCfromREG(SWriteMEASURE);
        SWriteMEASUREold["crc8"] = CalcSlaveCRCfromREG(SWriteMEASUREold);
    }
    else
    {
        SWriteMEASUREold["crc8"] = SWriteMEASURE["crc8"];
    }

    if (brc == false)
    {
        QByteArray temp;
        result = PerformCOMMAND(SWriteMEASUREold, &temp, 1);
        return result;
    }
    else
    {
        //result = PerformCOMMAND(SWriteMEASUREold, &QByteArray(), -1);
        //result = PerformCOMMAND(empty_reg, &rxdata, 18*brcs*num_of_pdcm_slots + 1);

        result = PerformCOMMAND(SWriteMEASUREold, &rxdata, 18*brcs*num_of_pdcm_slots + 1);

        if (result)
        {
            gp_Measurement->SetBRCReadData(rxdata.toHex());
        }
        else
        {
            gp_Measurement->SetBRCReadData("");
        }
        return result;
    }

}


bool DSI3Slave::PrepareMEASUREUPID()
{
    bool result = false;
    QByteArray rxdata;

    SWriteMEASURE["sid"] = 15;
    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS MEASURE command prepare - CH%1").arg((int)gp_DSI3Master->CHANNEL.value() + 1));

    SWriteMEASUREold.fromByteArray(SWriteMEASURE.toByteArray());
    SWriteMEASUREold["doppler_tx_side"] = (uint)SWriteMEASURE["doppler_tx_side"];
    SWriteMEASUREold["doppler_tx_inner"] = (uint)SWriteMEASURE["doppler_tx_inner"];
    SWriteMEASUREold["doppler_tx_outer"] = (uint)SWriteMEASURE["doppler_tx_outer"];
    SWriteMEASUREold["doppler_rx_side"] = (uint)SWriteMEASURE["doppler_rx_side"];
    SWriteMEASUREold["doppler_rx_inner"] = (uint)SWriteMEASURE["doppler_rx_inner"];
    SWriteMEASUREold["doppler_rx_outer"] = (uint)SWriteMEASURE["doppler_rx_outer"];

    if ((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
    {
        SWriteMEASUREold["rev_sh_ena2"] = (uint)SWriteMEASURE["rev_sh_ena"];
        SWriteMEASUREold["rev_sh2"] = (uint)SWriteMEASURE["rev_sh"];
        SWriteMEASUREold.setValue(8,8,(uint)SWriteMEASUREadd["dyn_gain_restart_ena"]);
        SWriteMEASUREold.setValue(9, 14, (uint)SWriteMEASUREadd.value(1, 6));
        SWriteMEASUREold.setValue(16,23,(uint)SWriteMEASUREadd["beam_start_delay"]);
    }
    else
    {
        SWriteMEASUREold["rev_sh_ena"] = (uint)SWriteMEASURE["rev_sh_ena"];
        SWriteMEASUREold["rev_sh"] = (uint)SWriteMEASURE["rev_sh"];

        SWriteMEASUREold["rev_sh_ena2"] = (uint)0;
        SWriteMEASUREold["rev_sh2"] = (uint)0;
        SWriteMEASUREold.setValue(8,14,(uint)0);
        SWriteMEASUREold.setValue(16,23,(uint)0);
    }

    if (GET_REG("VarDontUpdateCRC").toInt() == 0)
    {
        SWriteMEASURE["crc8"] = CalcSlaveCRCfromREG(SWriteMEASURE);
        SWriteMEASUREold["crc8"] = CalcSlaveCRCfromREG(SWriteMEASUREold);
    }
    else
    {
        SWriteMEASUREold["crc8"] = SWriteMEASURE["crc8"];
    }

    if ((gp_Settings->mVersion == VERSION_UPID) || (gp_Settings->mVersion == VERSION_RDUM)|| (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
    {
        QByteArray temp;
        result = PerformCOMMAND(SWriteMEASUREold, &temp, -2);
    }

    return result;
}

bool DSI3Slave::FirstMEASURE()
{
    bool result = false;

    SWriteMEASURE["sid"] = 15;


    SWriteMEASUREold.fromByteArray(SWriteMEASURE.toByteArray());
    SWriteMEASUREold["doppler_tx_side"] = (uint)SWriteMEASURE["doppler_tx_side"];
    SWriteMEASUREold["doppler_tx_inner"] = (uint)SWriteMEASURE["doppler_tx_inner"];
    SWriteMEASUREold["doppler_tx_outer"] = (uint)SWriteMEASURE["doppler_tx_outer"];
    SWriteMEASUREold["doppler_rx_side"] = (uint)SWriteMEASURE["doppler_rx_side"];
    SWriteMEASUREold["doppler_rx_inner"] = (uint)SWriteMEASURE["doppler_rx_inner"];
    SWriteMEASUREold["doppler_rx_outer"] = (uint)SWriteMEASURE["doppler_rx_outer"];
    if ((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
    {
        SWriteMEASUREold["rev_sh_ena2"] = (uint)SWriteMEASURE["rev_sh_ena"];
        SWriteMEASUREold["rev_sh2"] = (uint)SWriteMEASURE["rev_sh"];
        SWriteMEASUREold.setValue(8,8,(uint)SWriteMEASUREadd["dyn_gain_restart_ena"]);
        SWriteMEASUREold.setValue(9, 14, (uint)SWriteMEASUREadd.value(1, 6));
        SWriteMEASUREold.setValue(16,23,(uint)SWriteMEASUREadd["beam_start_delay"]);
    }
    else
    {
        SWriteMEASUREold["rev_sh_ena"] = (uint)SWriteMEASURE["rev_sh_ena"];
        SWriteMEASUREold["rev_sh"] = (uint)SWriteMEASURE["rev_sh"];

        SWriteMEASUREold["rev_sh_ena2"] = (uint)0;
        SWriteMEASUREold["rev_sh2"] = (uint)0;
        SWriteMEASUREold.setValue(8,14,(uint)0);
        SWriteMEASUREold.setValue(16,23,(uint)0);
    }
    SWriteMEASURE["crc8"] = CalcSlaveCRCfromREG(SWriteMEASURE);
    SWriteMEASUREold["crc8"] = CalcSlaveCRCfromREG(SWriteMEASUREold);

    if (gp_Settings->mVersion != VERSION_UPID)
    {
        gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS First MEASURE - CH%1").arg((int)gp_DSI3Master->CHANNEL.value() + 1));
        QByteArray temp;
        result = PerformCOMMAND(SWriteMEASUREold, &temp, -1);
    }

    return result;
}

bool DSI3Slave::LastMEASURE()
{
    bool result = false;
    QByteArray rxdata;
    Register empty_reg;
    int brcs = 0;

    int num_of_pdcm_slots = GET_REG("MREG10[NUM_OF_PDCM_SLOTS]").toInt() + 1;

    if ((int)gp_DSI3Master->CHANNEL.value() == 0)
    {
        brcs = GET_REG("MREG02[BRC_CNT]").toInt();
    }
    else
    {
        brcs = GET_REG("MREG03[BRC_CNT]").toInt();
    }


    SWriteMEASURE["sid"] = 15;


    SWriteMEASUREold.fromByteArray(SWriteMEASURE.toByteArray());
    SWriteMEASUREold["doppler_tx_side"] = (uint)SWriteMEASURE["doppler_tx_side"];
    SWriteMEASUREold["doppler_tx_inner"] = (uint)SWriteMEASURE["doppler_tx_inner"];
    SWriteMEASUREold["doppler_tx_outer"] = (uint)SWriteMEASURE["doppler_tx_outer"];
    SWriteMEASUREold["doppler_rx_side"] = (uint)SWriteMEASURE["doppler_rx_side"];
    SWriteMEASUREold["doppler_rx_inner"] = (uint)SWriteMEASURE["doppler_rx_inner"];
    SWriteMEASUREold["doppler_rx_outer"] = (uint)SWriteMEASURE["doppler_rx_outer"];
    if ((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
    {
        SWriteMEASUREold["rev_sh_ena2"] = (uint)SWriteMEASURE["rev_sh_ena"];
        SWriteMEASUREold["rev_sh2"] = (uint)SWriteMEASURE["rev_sh"];
        SWriteMEASUREold.setValue(8,8,(uint)SWriteMEASUREadd["dyn_gain_restart_ena"]);
        SWriteMEASUREold.setValue(9, 14, (uint)SWriteMEASUREadd.value(1, 6));
        SWriteMEASUREold.setValue(16,23,(uint)SWriteMEASUREadd["beam_start_delay"]);
    }
    else
    {
        SWriteMEASUREold["rev_sh_ena"] = (uint)SWriteMEASURE["rev_sh_ena"];
        SWriteMEASUREold["rev_sh"] = (uint)SWriteMEASURE["rev_sh"];

        SWriteMEASUREold["rev_sh_ena2"] = (uint)0;
        SWriteMEASUREold["rev_sh2"] = (uint)0;
        SWriteMEASUREold.setValue(8,14,(uint)0);
        SWriteMEASUREold.setValue(16,23,(uint)0);
    }
    SWriteMEASURE["crc8"] = CalcSlaveCRCfromREG(SWriteMEASURE);
    SWriteMEASUREold["crc8"] = CalcSlaveCRCfromREG(SWriteMEASUREold);

    if (gp_Settings->mVersion != VERSION_UPID)
    {
        gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS Last MEASURE - CH%1").arg((int)gp_DSI3Master->CHANNEL.value() + 1));
        result = PerformCOMMAND(empty_reg, &rxdata, 18*brcs*num_of_pdcm_slots + 1);

        if (result)
        {
            gp_Measurement->SetBRCReadData(rxdata.toHex());
        }
        else
        {
            gp_Measurement->SetBRCReadData("");
        }
    }

    return result;
}

bool DSI3Slave::ContiMEASURE()
{
    bool result = false;
    QByteArray rxdata;
    Register empty_reg;
    int brcs = 0;

    int num_of_pdcm_slots = GET_REG("MREG10[NUM_OF_PDCM_SLOTS]").toInt() + 1;

    if ((int)gp_DSI3Master->CHANNEL.value() == 0)
    {
        brcs = GET_REG("MREG02[BRC_CNT]").toInt();
    }
    else
    {
        brcs = GET_REG("MREG03[BRC_CNT]").toInt();
    }

    SWriteMEASURE["sid"] = 15;
    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS MEASURE command - CH%1").arg((int)gp_DSI3Master->CHANNEL.value() + 1));

    SWriteMEASUREold.fromByteArray(SWriteMEASURE.toByteArray());
    SWriteMEASUREold["doppler_tx_side"] = (uint)SWriteMEASURE["doppler_tx_side"];
    SWriteMEASUREold["doppler_tx_inner"] = (uint)SWriteMEASURE["doppler_tx_inner"];
    SWriteMEASUREold["doppler_tx_outer"] = (uint)SWriteMEASURE["doppler_tx_outer"];
    SWriteMEASUREold["doppler_rx_side"] = (uint)SWriteMEASURE["doppler_rx_side"];
    SWriteMEASUREold["doppler_rx_inner"] = (uint)SWriteMEASURE["doppler_rx_inner"];
    SWriteMEASUREold["doppler_rx_outer"] = (uint)SWriteMEASURE["doppler_rx_outer"];
    if ((gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
    {
        SWriteMEASUREold["rev_sh_ena2"] = (uint)SWriteMEASURE["rev_sh_ena"];
        SWriteMEASUREold["rev_sh2"] = (uint)SWriteMEASURE["rev_sh"];
        SWriteMEASUREold.setValue(8,8,(uint)SWriteMEASUREadd["dyn_gain_restart_ena"]);
        SWriteMEASUREold.setValue(9, 14, (uint)SWriteMEASUREadd.value(1, 6));
        SWriteMEASUREold.setValue(16,23,(uint)SWriteMEASUREadd["beam_start_delay"]);
    }
    else
    {
        SWriteMEASUREold["rev_sh_ena"] = (uint)SWriteMEASURE["rev_sh_ena"];
        SWriteMEASUREold["rev_sh"] = (uint)SWriteMEASURE["rev_sh"];

        SWriteMEASUREold["rev_sh_ena2"] = (uint)0;
        SWriteMEASUREold["rev_sh2"] = (uint)0;
        SWriteMEASUREold.setValue(8,14,(uint)0);
        SWriteMEASUREold.setValue(16,23,(uint)0);
    }
    SWriteMEASURE["crc8"] = CalcSlaveCRCfromREG(SWriteMEASURE);
    SWriteMEASUREold["crc8"] = CalcSlaveCRCfromREG(SWriteMEASUREold);

    result = PerformCOMMAND(empty_reg, &rxdata, 18*brcs*num_of_pdcm_slots + 1);
    QByteArray temp;
    PerformCOMMAND(SWriteMEASUREold, &temp, -1);

    if (result)
    {
        gp_Measurement->SetBRCReadData(rxdata.toHex());
    }
    else
    {
        gp_Measurement->SetBRCReadData("");
    }
    return result;
}

bool DSI3Slave::PerformDISCOVERY()
{
    bool result = false;


    SWriteDISCOVERY["sid"] = 15;

    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS DISCOVERY command - CH%1 SID%2")\
                       .arg((int)gp_DSI3Master->CHANNEL.value() + 1).arg((int)SWriteDISCOVERY["sid"]));

    if (GET_REG("VarDontUpdateCRC").toInt() == 0)
    {
        SWriteDISCOVERY["crc8"] = CalcSlaveCRCfromREG(SWriteDISCOVERY);
    }

    Register temp;
    result = PerformCRM(SWriteDISCOVERY, &temp, 1);

    slot_updateAllControls();
    return result;
}

bool DSI3Slave::PerformSWRST(bool inreg, int sid)
{
    bool result = false;

    if (!inreg)
    {
        SWriteSWRST["sid"] = sid;
    }
    else
    {
        SWriteSWRST["sid"] = VarSID.value();
    }

    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS SW_RST command - CH%1 SID%2")\
                       .arg((int)gp_DSI3Master->CHANNEL.value() + 1).arg((int)SWriteSWRST["sid"]));

    if (GET_REG("VarDontUpdateCRC").toInt() == 0)
    {
        SWriteSWRST["crc8"] = CalcSlaveCRCfromREG(SWriteSWRST);
    }

    QByteArray temp;
    result = PerformCOMMAND(SWriteSWRST, &temp, 1);

    return result;
}

bool DSI3Slave::PerformUnlockAndSWRST()
{
    bool result = false;
    PerformRWPandWRITE(VarSID.value(), 0, 0x0A, 0x400);
    result = PerformSWRST(false, VarSID.value());
    return result;
}

bool DSI3Slave::AddressDISCOVERY()
{
    bool result = true;

    PerformRWPandWRITE(15, 0, 0x0A, 1);
    PerformDISCOVERY();
    PerformWRITE(false, 15, 0);
    result = result & CheckSlaves();

    return result;
}

bool DSI3Slave::AddressDEMUXES()
{
    bool result = true;
    QByteArray demux;
    int i;
    int rspdata;
    int shiftreg = 0;

    if (gp_Settings->mVersion == VERSION_UPID)
    {
        demux.clear();
        demux.append(0x3F);
        demux.append(0x3F);
        gp_UartSpiController->DEMUX.fromByteArray(demux);
        gp_UartSpiController->runAppCommand("SetDemux","","","","","");
        PerformRWPandWRITE(15, 0, 0x0A, 1);   // UNLOCK ID
        PerformWRITE_RWP(false, 15, 0, 0, 1, 0, 0x08);

        for (i = 1 ; i < 7; i++)
        {
            shiftreg = shiftreg << 1;
            shiftreg = shiftreg | 0x01;
            demux.clear();
            demux.append(shiftreg);
            demux.append(shiftreg);
            gp_UartSpiController->DEMUX.fromByteArray(demux);
            gp_UartSpiController->runAppCommand("SetDemux", "","","","","");

            PerformWRITE(false, 15, i);
            PerformREAD(false, i, 0, 0x08, &rspdata);
        }

    }
    else if ((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
    {
        shiftreg = 0x3F;

        gp_UartSsiController->SSIREGX040.setValue("[DEMUX_CHA]", shiftreg);
        gp_UartSsiController->SSIREGX040.setValue("[DEMUX_CHB]", shiftreg);
        gp_UartSsiController->runAppCommand("WriteREGX", "SSIREGX040", "", "", "", "");

        PerformRWPandWRITE(15, 0, 0x0A, 1);   // UNLOCK ID
        PerformWRITE_RWP(false, 15, 0, 0, 1, 0, 0x08);

        shiftreg = 0;
        for (i = 1 ; i < 7; i++)
        {
            shiftreg = shiftreg << 1;
            shiftreg = shiftreg | 0x01;

            gp_UartSsiController->SSIREGX040.setValue("[DEMUX_CHA]", shiftreg);
            gp_UartSsiController->SSIREGX040.setValue("[DEMUX_CHB]", shiftreg);
            gp_UartSsiController->runAppCommand("WriteREGX", "SSIREGX040", "", "", "", "");

            PerformWRITE(false, 15, i);
            PerformREAD(false, i, 0, 0x08, &rspdata);
        }
    }
    else
    {
        PerformRWPandWRITE(15, 0, 0x0A, 1);
        PerformRWPandWRITE(15, 0, 0x08, 1);
        PerformRWPandWRITE(15, 0, 0x0A, 0);
    }

    result = result & CheckSlaves();
    return result;
}

bool DSI3Slave::AddressSWITCHES()
{
    bool result = true;
    QByteArray demux;
    int i;
    int rspdata;
    int shiftreg = 0;

    if (gp_Settings->mVersion == VERSION_UPID)
    {
        demux.clear();
        demux.append(0x3F);
        demux.append(0x3F);
        gp_UartSpiController->DEMUX.fromByteArray(demux);
        gp_UartSpiController->runAppCommand("SetDemux","","","","","");     // Switch ON all demuxes

        for (i = 1 ; i < 7; i++)
        {
            PerformRWPandWRITE(0, 0, 0x0A, 1);      // UNLOCK ID
            PerformRWPandWRITE(0, 0, 0x08, i);      // WRITE_SID
            //PerformWRITE_RWP(false, 0, 0, 0, 1, 0, 0x08);
            //PerformWRITE(false, 0, i);
            PerformREAD(false, i, 0, 0x08, &rspdata);

            PerformRWPandWRITEP(i, 0, 0x0E, 0x1200);
        }

    }
    else if ((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
    {
        shiftreg = 0x3F;

        gp_UartSsiController->SSIREGX040.setValue("[DEMUX_CHA]", shiftreg);
        gp_UartSsiController->SSIREGX040.setValue("[DEMUX_CHB]", shiftreg);
        gp_UartSsiController->runAppCommand("WriteREGX", "SSIREGX040", "", "", "", "");

        PerformRWPandWRITE(15, 0, 0x0A, 1);   // UNLOCK ID
        PerformWRITE_RWP(false, 15, 0, 0, 1, 0, 0x08);

        shiftreg = 0;
        for (i = 1 ; i < 7; i++)
        {
            PerformWRITE(false, 15, i);
            PerformREAD(false, i, 0, 0x08, &rspdata);

            PerformRWPandWRITEP(i, 0, 0x0E, 0x1200);
        }
    }
    else
    {
        PerformRWPandWRITE(15, 0, 0x0A, 1);
        PerformRWPandWRITE(15, 0, 0x08, 1);
        PerformRWPandWRITE(15, 0, 0x0A, 0);
    }

    result = result & CheckSlaves();
    return result;
}

bool DSI3Slave::PerformRWPandWRITE(int sid, int new_crm_w_pg, int new_crm_w_idx, int data)
{
    bool result = false;
    QByteArray txdata;
    QByteArray rxdata;

    SWriteWRITERWP["sid"] = sid;
    SWriteWRITERWP["pdcm_upd"] = 0;
    SWriteWRITERWP["new_pdcm_r_pg"] = 0;
    SWriteWRITERWP["crm_upd"] = 1;
    SWriteWRITERWP["new_crm_w_pg"] = new_crm_w_pg;
    SWriteWRITERWP["new_crm_w_idx"] = new_crm_w_idx;
    SWriteWRITERWP["crc8"] = CalcSlaveCRCfromREG(SWriteWRITERWP);
    SWriteWRITE["sid"] = sid;
    SWriteWRITE["data"] = data;
    SWriteWRITE["crc8"] = CalcSlaveCRCfromREG(SWriteWRITE);

    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS WRITE_RWP and WRITE command - CH%1 SID%2 PG%3 IDX%4hex")\
                       .arg((int)gp_DSI3Master->CHANNEL.value() + 1).arg((int)SWriteWRITERWP["sid"])\
                       .arg((int)SWriteWRITERWP["new_crm_w_pg"]).arg((int)SWriteWRITERWP["new_crm_w_idx"], 1, 16));

    txdata.append(SWriteWRITERWP.toByteArray(Register::MSB, true));
    txdata.append(SWriteWRITE.toByteArray(Register::MSB, true));

    if(gp_Settings->mVersion == VERSION_UPID)
    {
        result = gp_ASICMaster->PerformCRM(txdata, &rxdata, 8);
    }
    else if((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
    {
        result = gp_RDUMMaster->PerformCRM(txdata, &rxdata, 8);
    }
    else
    {
        result = gp_DSI3Master->PerformCRM(txdata, &rxdata, 8);
    }

    if ((result == false) && (sid != 0x0F))
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "No response to DSI3PAS WRITE_RWP and/or WRITE command");
    }
    slot_updateAllControls();
    return result;
}

bool DSI3Slave::PerformRWPandWRITEP(int sid, int new_crm_w_pg, int new_crm_w_idx, int data)
{
    bool result = false;
    QByteArray txdata;
    QByteArray rxdata;

    SWriteWRITERWP["sid"] = sid;
    SWriteWRITERWP["pdcm_upd"] = 0;
    SWriteWRITERWP["new_pdcm_r_pg"] = 0;
    SWriteWRITERWP["crm_upd"] = 1;
    SWriteWRITERWP["new_crm_w_pg"] = new_crm_w_pg;
    SWriteWRITERWP["new_crm_w_idx"] = new_crm_w_idx;
    SWriteWRITERWP["crc8"] = CalcSlaveCRCfromREG(SWriteWRITERWP);
    SWriteWRITEP["sid"] = sid;
    SWriteWRITEP["data"] = data;
    SWriteWRITEP["crc8"] = CalcSlaveCRCfromREG(SWriteWRITEP);

    gp_DSI3Master->Log(LOG_TYPE_DSI3_CMD, QString("DSI3PAS WRITE_RWP and WRITE_P command - CH%1 SID%2 PG%3 IDX%4hex")\
                       .arg((int)gp_DSI3Master->CHANNEL.value() + 1).arg((int)SWriteWRITERWP["sid"])\
                       .arg((int)SWriteWRITERWP["new_crm_w_pg"]).arg((int)SWriteWRITERWP["new_crm_w_idx"], 1, 16));

    txdata.append(SWriteWRITERWP.toByteArray(Register::MSB, true));
    txdata.append(SWriteWRITEP.toByteArray(Register::MSB, true));

    if(gp_Settings->mVersion == VERSION_UPID)
    {
        result = gp_ASICMaster->PerformCRM(txdata, &rxdata, 8);
    }
    else if((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
    {
        result = gp_RDUMMaster->PerformCRM(txdata, &rxdata, 8);
    }
    else
    {
        result = gp_DSI3Master->PerformCRM(txdata, &rxdata, 8);
    }

    if ((result == false) && (sid != 0x0F))
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "No response to DSI3PAS WRITE_RWP and/or WRITE_P command");
    }
    slot_updateAllControls();
    return result;
}

bool DSI3Slave::PerformCRM(Register tx_reg, Register *rx_reg, int read_count)
{
    bool result = false;

    QByteArray txdata = tx_reg.toByteArray(Register::MSB, true);
    QByteArray rxdata;
    QByteArray crc_data;

    if(gp_Settings->mVersion == VERSION_UPID)
    {
          result = gp_ASICMaster->PerformCRM(txdata, &rxdata, read_count);
    }
    else if((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
    {
          result = gp_RDUMMaster->PerformCRM(txdata, &rxdata, read_count);
    }
    else
    {
          result = gp_DSI3Master->PerformCRM(txdata, &rxdata, read_count);
    }

    rx_reg->fromByteArray(rxdata, -1, Register::MSB, true);

    if (result)
    {
        QByteArray zeros;
        zeros.fill((char)0x00, read_count);

        if (rxdata == zeros)
        {
            result = false;
        }
        else
        {
            crc_data = rxdata;
            crc_data.resize(3);

            if (rxdata.at(3) != gp_UartSpiController->CalculateUPAECRC(crc_data))
            {
                LERROR("DSI3 RX CRC is not matching");
            }
            /*else
            {
                LWARNING(QString("DSI3 RX CRC checked %1").arg(rxdata.at(3)));
            }*/
        }
    }

    return result;
}

bool DSI3Slave::PerformCOMMAND(Register tx_reg, QByteArray *rxdata, int read_count)
{
    bool result = false;

    QByteArray data = tx_reg.toByteArray(Register::MSB, true);

    if (gp_Settings->mVersion == VERSION_UPID)
    {
        result = gp_ASICMaster->PerformCOMMAND(data, rxdata, read_count);
    }
    else if ((gp_Settings->mVersion == VERSION_RDUM) || (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER))
    {
        result = gp_RDUMMaster->PerformCOMMAND(data, rxdata, read_count);
    }
    else
    {
        result = gp_DSI3Master->PerformCOMMAND(data, rxdata, read_count);
    }


    return result;
}

bool DSI3Slave::WriteSlaveRegs(QString regtype)
{
    int page = 0;
    int addr = 0;
    int txdata = 0;
    bool result = true;
    int dbgoutsel;
    int comp_method;
    int mode;
    QString num;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (regtype == "Mode")
    {
        dbgoutsel = Vardbg_out_sel.value();
        SET_REG("SREGX04A[dbg_output_sel]", (dbgoutsel >> 2) & 0x07);
        SET_REG("SREGX048[dbg_output_sel]", dbgoutsel & 0x03);
        SET_REG("SREGXV2058[dbg_output_sel]", dbgoutsel);

        comp_method = Varcomp_method.value();
        SREGX[0].at(m_map_sregs[0][0][0x52])->setValue("[comp_method]", comp_method & 0x07);
        SREGX[0].at(m_map_sregs[0][0][0x58])->setValue("[comp_method_b3]", (comp_method >> 3) & 0x01);
        SREGX[1].at(m_map_sregs[1][0][0x52])->setValue("[comp_method]", comp_method & 0x07);
        SREGX[1].at(m_map_sregs[1][0][0x58])->setValue("[comp_method_b3]", (comp_method >> 3) & 0x01);



        for (mode = 1; mode < 6; mode++)
        {
            dbgoutsel = SDBG.at(mode)->value();   
            SREGX[0].at(m_map_sregs[0][1][0x4A - 0x40 + mode_start_addr[mode]])->setValue("[dbg_output_sel]", (dbgoutsel >> 2) & 0x07);
            SREGX[0].at(m_map_sregs[0][1][0x48 - 0x40 + mode_start_addr[mode]])->setValue("[dbg_output_sel]", (dbgoutsel & 0x03));
            SREGX[1].at(m_map_sregs[1][1][0x58 - 0x40 + mode_start_addr[mode]])->setValue("[dbg_output_sel]", dbgoutsel);


            /*num = QString("%1").arg(0x4A - 0x40 + mode_start_addr[mode],2,16,QChar('0')).toUpper();
            SET_REG(QString("SREGX1%1[dbg_output_sel]").arg(num), (dbgoutsel >> 2) & 0x07);
            num = QString("%1").arg(0x48 - 0x40 + mode_start_addr[mode],2,16,QChar('0')).toUpper();
            SET_REG(QString("SREGX1%1[dbg_output_sel]").arg(num), dbgoutsel & 0x03);*/

            comp_method = SCOMP.at(mode)->value();

            SREGX[0].at(m_map_sregs[0][1][0x52 - 0x40 + mode_start_addr[mode]])->setValue("[comp_method]", comp_method & 0x07);
            SREGX[0].at(m_map_sregs[0][1][0x58 - 0x40 + mode_start_addr[mode]])->setValue("[comp_method_b3]", (comp_method >> 3) & 0x01);
            SREGX[1].at(m_map_sregs[1][1][0x52 - 0x40 + mode_start_addr[mode]])->setValue("[comp_method]", comp_method & 0x07);
            SREGX[1].at(m_map_sregs[1][1][0x58 - 0x40 + mode_start_addr[mode]])->setValue("[comp_method_b3]", (comp_method >> 3) & 0x01);

            /*num = QString("%1").arg(0x52 - 0x40 + mode_start_addr[mode],2,16,QChar('0')).toUpper();
            SET_REG(QString("SREGX1%1[comp_method]").arg(num), comp_method & 0x07);
            num = QString("%1").arg(0x58 - 0x40 + mode_start_addr[mode],2,16,QChar('0')).toUpper();
            SET_REG(QString("SREGX1%1[comp_method_b3]").arg(num), (comp_method >> 3) & 0x01);*/
        }


        WriteModeRegs();

    }
    else
    {
        if (regtype == "DSI3")
        {
            if ((gp_DSI3Master->MVar_version.value() == VERSION_MOBEJUN) || (gp_DSI3Master->MVar_version.value() == VERSION_MOBEDMX)\
                    || (gp_DSI3Master->MVar_version.value() == VERSION_RDUS3DCH) || (gp_DSI3Master->MVar_version.value() == VERSION_RDUSBSD)\
                    || (gp_DSI3Master->MVar_version.value() == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version.value() == VERSION_MOBE2DMX))
            {
                SREGX[1].at(m_map_sregs[1][1][0x20])->setValue("[pdcm_size]", 3);
                SREGX[1].at(m_map_sregs[1][1][0x20])->setValue("[tx_filter_ena]", 1);
                SREGX[1].at(m_map_sregs[1][1][0x20])->setValue("[t_chip]", 0);
                SREGX[1].at(m_map_sregs[1][1][0x20])->setValue("[crc_mode]", 0);
                SREGX[1].at(m_map_sregs[1][1][0x20])->setValue("[dsi3pas_vpp]", 0);
            }

            AddCRCDSI3Config();
            //SET_REG("SREGX122[gain_clamp]", 255);
            //SREGX[m_memory].at(m_map_sregs[m_memory][1][0x22])->setValue("[gain_clamp]", 255);
            gp_Measurement->m_first_measurement[0] = true;
            gp_Measurement->m_first_measurement[1] = true;
        }

        for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
        {
            if (SREGX[m_memory].at(i)->extra("type").contains(regtype))
            {
                page = SREGX[m_memory].at(i)->extra("page").toInt();
                addr = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16);

                txdata = SREGX[m_memory].at(i)->value(0, 15);
                result = result & PerformRWPandWRITE(VarSID.value(), page, addr, txdata);
            }
        }
    }

    QApplication::restoreOverrideCursor();
    return result;
}

bool DSI3Slave::WriteModeRegs(void)
{
    int page = 0;
    int addr = 0;
    int txdata = 0;
    bool result = true;
    int sid;
    int mode = 0;
    int modes = SMODES.value(0, 5);

    QApplication::setOverrideCursor(Qt::WaitCursor);

    gp_DSI3Master->Log(LOG_TYPE_INFO, QString("mem is %1").arg(m_memory));

    if ((modes & 0x01) == 0x01)     // MODE 0
    {
        if ((GET_REG("VarSID").toInt() <= 6))
        {
            if (GET_REG("VarSID").toInt() == 0)
            {
                SREGX[m_memory].at(m_map_sregs[m_memory][0][0x44])->setValue(0, 14, SSCALE.at(m_map_sscale[0][0])->value(0, 14));
                SREGX[m_memory].at(m_map_sregs[m_memory][0][0x46])->setValue(0, 14, SSCALE.at(m_map_sscale[0][0])->value(15, 29));
            }
            else if ((GET_REG("VarSID").toInt() >= 1) && (GET_REG("VarSID").toInt() <= 6))
            {
                SREGX[m_memory].at(m_map_sregs[m_memory][0][0x44])->setValue(0, 14, SSCALE.at(m_map_sscale[GET_REG("VarSID").toInt() - 1][0])->value(0, 14));
                SREGX[m_memory].at(m_map_sregs[m_memory][0][0x46])->setValue(0, 14, SSCALE.at(m_map_sscale[GET_REG("VarSID").toInt() - 1][0])->value(15, 29));
            }
            AddCRCModeConfig();
            for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
            {
                if (SREGX[m_memory].at(i)->extra("type").contains("Mode"))
                {
                    page = 1;
                    addr = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16) - 0x40 + mode_start_addr[0];

                    txdata = SREGX[m_memory].at(i)->value(0, 15);
                    result = result & PerformRWPandWRITE(VarSID.value(), page, addr, txdata);
                }
            }
        }
        else if (GET_REG("VarSID").toInt() == 15)
        {
            for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
            {
                if (SREGX[m_memory].at(i)->extra("type").contains(("Mode")))
                {
                    page = 1;
                    addr = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16) - 0x40 + mode_start_addr[0];
                    txdata = SREGX[m_memory].at(i)->value(0, 15);
                    result = result & PerformRWPandWRITE(VarSID.value(), page, addr, txdata);
                }
            }

            for (sid = 1; sid < 7; sid++)
            {
                if (m_slave_connected[((int)gp_DSI3Master->CHANNEL.value())&0x01][sid-1] == true)
                {
                    VarSID.setValue(sid);

                    SREGX[m_memory].at(m_map_sregs[m_memory][0][0x44])->setValue(0, 14, SSCALE.at(m_map_sscale[sid - 1][0])->value(0, 14));
                    SREGX[m_memory].at(m_map_sregs[m_memory][0][0x46])->setValue(0, 14, SSCALE.at(m_map_sscale[sid - 1][0])->value(15, 29));

                    AddCRCModeConfig();

                    addr = 0x44 - 0x40 + mode_start_addr[0];
                    txdata = SREGX[m_memory].at(m_map_sregs[m_memory][0][0x44])->value(0, 15);
                    result = result & PerformRWPandWRITE(VarSID.value(), 1, addr, txdata);
                    addr = 0x46 - 0x40 + mode_start_addr[0];
                    txdata = SREGX[m_memory].at(m_map_sregs[m_memory][0][0x46])->value(0, 15);
                    result = result & PerformRWPandWRITE(VarSID.value(), 1, addr, txdata);
                    addr = 0x60 - 0x40 + mode_start_addr[0];
                    txdata = SREGX[m_memory].at(m_map_sregs[m_memory][0][0x60])->value(0, 15);
                    result = result & PerformRWPandWRITE(VarSID.value(), 1, addr, txdata);

                }
            }
            VarSID.setValue(15);
        }
    }

    for (mode = 1; mode < 6; mode++)
    {
        if (((modes >> mode) & 0x01) == 0x01)       //for selected MODEs only
        {
            if ((GET_REG("VarSID").toInt() <= 6))
            {
                if (GET_REG("VarSID").toInt() == 0)
                {
                    SREGX[m_memory].at(m_map_sregs[m_memory][1][0x44 - 0x40 + mode_start_addr[mode]])->setValue(0, 14, SSCALE.at(m_map_sscale[0][mode])->value(0, 14));
                    SREGX[m_memory].at(m_map_sregs[m_memory][1][0x46 - 0x40 + mode_start_addr[mode]])->setValue(0, 14, SSCALE.at(m_map_sscale[0][mode])->value(15, 29));
                }
                else if ((GET_REG("VarSID").toInt() >= 1) && (GET_REG("VarSID").toInt() <= 6))
                {
                    SREGX[m_memory].at(m_map_sregs[m_memory][1][0x44 - 0x40 + mode_start_addr[mode]])->setValue(0, 14, SSCALE.at(m_map_sscale[GET_REG("VarSID").toInt() - 1][mode])->value(0, 14));
                    SREGX[m_memory].at(m_map_sregs[m_memory][1][0x46 - 0x40 + mode_start_addr[mode]])->setValue(0, 14, SSCALE.at(m_map_sscale[GET_REG("VarSID").toInt() - 1][mode])->value(15, 29));
                }
                AddCRCModeConfig();
                for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
                {
                    if (SREGX[m_memory].at(i)->extra("type").contains("Mode"))
                    {
                        page = 1;
                        addr = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16) - 0x40 + mode_start_addr[mode];
                        txdata = SREGX[m_memory].at(m_map_sregs[m_memory][page][addr])->value(0, 15);
                        result = result & PerformRWPandWRITE(VarSID.value(), page, addr, txdata);
                    }
                }
            }
            else if (GET_REG("VarSID").toInt() == 15)
            {
                for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
                {
                    if (SREGX[m_memory].at(i)->extra("type").contains(("Mode")))
                    {
                        page = 1;
                        addr = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16) - 0x40 + mode_start_addr[mode];
                        txdata = SREGX[m_memory].at(m_map_sregs[m_memory][page][addr])->value(0, 15);
                        result = result & PerformRWPandWRITE(VarSID.value(), page, addr, txdata);
                    }
                }

                for (sid = 1; sid < 7; sid++)
                {
                    if (m_slave_connected[((int)gp_DSI3Master->CHANNEL.value())&0x01][sid-1] == true)
                    {
                        VarSID.setValue(sid);

                        SREGX[m_memory].at(m_map_sregs[m_memory][1][0x44 - 0x40 + mode_start_addr[mode]])->setValue(0, 14, SSCALE.at(m_map_sscale[sid - 1][mode])->value(0, 14));
                        SREGX[m_memory].at(m_map_sregs[m_memory][1][0x46 - 0x40 + mode_start_addr[mode]])->setValue(0, 14, SSCALE.at(m_map_sscale[sid - 1][mode])->value(15, 29));

                        AddCRCModeConfig();

                        addr = 0x44 - 0x40 + mode_start_addr[mode];
                        txdata = SREGX[m_memory].at(m_map_sregs[m_memory][1][addr])->value(0, 15);
                        result = result & PerformRWPandWRITE(VarSID.value(), 1, addr, txdata);
                        addr = 0x46 - 0x40 + mode_start_addr[mode];
                        txdata = SREGX[m_memory].at(m_map_sregs[m_memory][1][addr])->value(0, 15);
                        result = result & PerformRWPandWRITE(VarSID.value(), 1, addr, txdata);
                        addr = 0x60 - 0x40 + mode_start_addr[mode];
                        txdata = SREGX[m_memory].at(m_map_sregs[m_memory][1][addr])->value(0, 15);
                        result = result & PerformRWPandWRITE(VarSID.value(), 1, addr, txdata);

                    }
                }
                VarSID.setValue(15);
            }
        }
    }






    QApplication::restoreOverrideCursor();
    return result;
}

bool DSI3Slave::ReadSlaveRegs(QString regtype)
{
    int i;
    int page = 0;
    int addr = 0;
    int rxdata = 0;
    bool result = true;
    int dbgoutsel;
    int comp_method;
    int mode;
    QString num;

    QApplication::setOverrideCursor(Qt::WaitCursor);


    if (VarSID.value() > 6)
    {
        LERROR("Read from SID > 6 is not possible");
        QApplication::restoreOverrideCursor();
        return false;
    }
    else if (regtype == "Mode")
    {
        ReadModeRegs();
    }
    else
    {
        for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
        {
            if (SREGX[m_memory].at(i)->extra("type").contains(regtype))
            {
                page = SREGX[m_memory].at(i)->extra("page").toInt();
                addr = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16);
                result = result & PerformREAD(false, VarSID.value(), page, addr, &rxdata);
                SREGX[m_memory].at(i)->setValue(0, 15, rxdata);
            }
        }
    }

    QApplication::restoreOverrideCursor();
    return result;
}

bool DSI3Slave::ReadModeRegs(void)
{
    int mode = 0;
    int modes = SMODES.value(0, 5);
    int page = 0;
    int addr = 0;
    bool result;
    int rxdata;
    int dbgoutsel;
    int comp_method;
    QString num;

    if ((modes & 0x01) == 0x01)     // MODE 0
    {
        for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
        {
            if (SREGX[m_memory].at(i)->extra("type").contains("Mode"))
            {
                page = 1;
                addr = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16) - 0x40 + mode_start_addr[0];

                result = result & PerformREAD(false, VarSID.value(), page, addr, &rxdata);
                SREGX[m_memory].at(i)->setValue(0, 15, rxdata);
            }
        }

        if (m_memory == 0)
        {
            dbgoutsel = GET_REG("SREGX048[dbg_output_sel]").toInt() + (GET_REG("SREGX04A[dbg_output_sel]").toInt() << 2);
            Vardbg_out_sel.setValue(dbgoutsel);

            //comp_method = GET_REG("SREGX052[comp_method]").toInt() + (GET_REG("SREGX058[comp_method_b3]").toInt() << 3);
            //Varcomp_method.setValue(comp_method);
        }
        else if (m_memory == 1)
        {
            //dbgoutsel = GET_REG("SREGXV2058[dbg_output_sel]").toInt();
            dbgoutsel = SREGX[m_memory].at(m_map_sregs[m_memory][0][0x58])->value("[dbg_output_sel]");
            Vardbg_out_sel.setValue(dbgoutsel);

            //comp_method = GET_REG("SREGXV2052[comp_method]").toInt() + (GET_REG("SREGXV2058[comp_method_b3]").toInt() << 3);
            //Varcomp_method.setValue(comp_method);

        }
        comp_method = SREGX[m_memory].at(m_map_sregs[m_memory][0][0x52])->value("[comp_method]") + (SREGX[m_memory].at(m_map_sregs[m_memory][0][0x58])->value("[comp_method_b3]") << 3);
        Varcomp_method.setValue(comp_method);

        if (GET_REG("VarSID").toInt() == 0)
        {
            SSCALE.at(m_map_sscale[0][0])->setValue(0, 14, SREGX[m_memory].at(m_map_sregs[m_memory][0][0x44])->value(0, 14));
            SSCALE.at(m_map_sscale[0][0])->setValue(15, 29, SREGX[m_memory].at(m_map_sregs[m_memory][0][0x46])->value(0, 14));
        }
        else if ((GET_REG("VarSID").toInt() >= 1) || (GET_REG("VarSID").toInt() <= 6))
        {
            SSCALE.at(m_map_sscale[GET_REG("VarSID").toInt() - 1][0])->setValue(0, 14, SREGX[m_memory].at(m_map_sregs[m_memory][0][0x44])->value(0, 14));
            SSCALE.at(m_map_sscale[GET_REG("VarSID").toInt() - 1][0])->setValue(15, 29, SREGX[m_memory].at(m_map_sregs[m_memory][0][0x46])->value(0, 14));
        }
    }
    for (mode = 1; mode < 6; mode++)
    {
        if (((modes >> mode) & 0x01) == 0x01)       //for selected MODEs only
        {
            for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
            {
                if (SREGX[m_memory].at(i)->extra("type").contains("Mode"))
                {
                    page = 1;
                    addr = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16) - 0x40 + mode_start_addr[mode];

                    result = result & PerformREAD(false, VarSID.value(), page, addr, &rxdata);
                    SREGX[m_memory].at(m_map_sregs[m_memory][1][addr])->setValue(0, 15, rxdata);
                }
            }

            if (m_memory == 0)
            {
                num = QString("%1").arg(0x4A - 0x40 + mode_start_addr[mode],2,16,QChar('0')).toUpper();
                dbgoutsel = GET_REG(QString("SREGX1%1[dbg_output_sel]").arg(num)).toInt() << 2;
                num = QString("%1").arg(0x48 - 0x40 + mode_start_addr[mode],2,16,QChar('0')).toUpper();
                dbgoutsel = dbgoutsel + GET_REG(QString("SREGX1%1[dbg_output_sel]").arg(num)).toInt();
                SDBG.at(mode)->setValue(dbgoutsel);
            }
            else if (m_memory == 1)
            {
                /*num = QString("%1").arg(0x58 - 0x40 + mode_start_addr[mode],2,16,QChar('0')).toUpper();
                dbgoutsel = GET_REG(QString("SREGXV21%1[dbg_output_sel]").arg(num)).toInt();*/
                dbgoutsel = SREGX[m_memory].at(m_map_sregs[m_memory][1][0x58 - 0x40 + mode_start_addr[mode]])->value("dbg_output_sel");

                SDBG.at(mode)->setValue(dbgoutsel);
            }

            //num = QString("%1").arg(0x58 - 0x40 + mode_start_addr[mode],2,16,QChar('0')).toUpper();
            //comp_method = GET_REG(QString("SREGX1%1[comp_method_b3]").arg(num)).toInt() << 3;

            comp_method = SREGX[m_memory].at(m_map_sregs[m_memory][1][0x58 - 0x40 + mode_start_addr[mode]])->value("[comp_method_b3]") << 3;
            comp_method = comp_method + SREGX[m_memory].at(m_map_sregs[m_memory][1][0x52 - 0x40 + mode_start_addr[mode]])->value("[comp_method]");
            //num = QString("%1").arg(0x52 - 0x40 + mode_start_addr[mode],2,16,QChar('0')).toUpper();
            //comp_method = comp_method + GET_REG(QString("SREGX1%1[comp_method]").arg(num)).toInt();
            SCOMP.at(mode)->setValue(comp_method);



            if (GET_REG("VarSID").toInt() == 0)
            {
                SSCALE.at(m_map_sscale[0][mode])->setValue(0, 14, SREGX[m_memory].at(m_map_sregs[m_memory][1][0x44 - 0x40 + mode_start_addr[mode]])->value(0, 14));
                SSCALE.at(m_map_sscale[0][mode])->setValue(15, 29, SREGX[m_memory].at(m_map_sregs[m_memory][1][0x46 - 0x40 + mode_start_addr[mode]])->value(0, 14));
            }
            else if ((GET_REG("VarSID").toInt() >= 1) || (GET_REG("VarSID").toInt() <= 6))
            {
                SSCALE.at(m_map_sscale[GET_REG("VarSID").toInt() - 1][mode])->setValue(0, 14, SREGX[m_memory].at(m_map_sregs[m_memory][1][0x44 - 0x40 + mode_start_addr[mode]])->value(0, 14));
                SSCALE.at(m_map_sscale[GET_REG("VarSID").toInt() - 1][mode])->setValue(15, 29, SREGX[m_memory].at(m_map_sregs[m_memory][1][0x46 - 0x40 + mode_start_addr[mode]])->value(0, 14));
            }
        }
    }
    return result;
}

void DSI3Slave::slot_GainGraphUpdate(void)
{
    const quint32 gain_lna[4] = {20, 30, 40, 40};
    const double delta_gain_tim[] = {0.1, 0.2, 0.4, 0.8, 1.2, 1.6, 2, 2.5, 3.3, 4.5, 5.9, 8, 10, 12.7, 16, 20};
    QVector<double> x(8), y(8);
    QVector<double> xg(2), yg(2);
    int offset;


    if (m_memory == 0)
    {
        y[0] = gain_lna[GET_REG("SREGX04C[gain_lna]").toInt() & 0x03] + GET_REG("SREGX04A[dyn_gain_offset]").toFloat()*0.38;
    }
    else if (m_memory == 1)
    {
        if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEJUN) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBEDMX) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DCH)\
                 || (ui->comboBoxVersion->currentIndex() == VERSION_RDUSBSD) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DMX) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBE2DMX))
        {
            offset = GET_REG("SREGXV2042[15:9]").toInt();
        }
        else
        {
            offset = (GET_REG("SREGXV2040[dyn_gain_offset_6]").toInt() << 6) + GET_REG("SREGXV2042[dyn_gain_offset_50]").toInt();
        }

        y[0] = gain_lna[GET_REG("SREGXV204C[gain_lna]").toInt() & 0x03] + (float)offset * 0.38;
    }

    y[1] = y[0];

    x[0] = 0;
    x[1] = 0.4096 * ((float) SREGX[m_memory].at(m_map_sregs[m_memory][0][0x54])->value("[var_gain_start]"));
    x[2] = x[1] + delta_gain_tim[((quint8) SREGX[m_memory].at(m_map_sregs[m_memory][0][0x4E])->value("[delta_gain_tim0]"))];
    x[3] = x[2] + delta_gain_tim[((quint8) SREGX[m_memory].at(m_map_sregs[m_memory][0][0x50])->value("[delta_gain_tim1]"))];
    x[4] = x[3] + delta_gain_tim[((quint8) SREGX[m_memory].at(m_map_sregs[m_memory][0][0x52])->value("[delta_gain_tim2]"))];
    x[5] = x[4] + delta_gain_tim[((quint8) SREGX[m_memory].at(m_map_sregs[m_memory][0][0x54])->value("[delta_gain_tim3]"))];
    x[6] = x[5] + delta_gain_tim[((quint8) SREGX[m_memory].at(m_map_sregs[m_memory][0][0x56])->value("[delta_gain_tim4]"))];
    x[7] = 100;

    /*
    x[0] = 0;
    x[1] = GET_REG("SREGX054[var_gain_start]").toFloat()*0.4096;
    x[2] = x[1] + GET_REG("SREGX04E[delta_gain_tim0]").toFloat()*0.4096;
    x[3] = x[2] + GET_REG("SREGX050[delta_gain_tim1]").toFloat()*0.4096;
    x[4] = x[3] + GET_REG("SREGX052[delta_gain_tim2]").toFloat()*0.4096;
    x[5] = x[4] + GET_REG("SREGX054[delta_gain_tim3]").toFloat()*0.4096;
    x[6] = x[5] + GET_REG("SREGX056[delta_gain_tim4]").toFloat()*0.4096;
    x[7] = 100;*/

    //if (GET_REG("SREGX054[var_gain_enable]").toInt() == 1)
    if (SREGX[m_memory].at(m_map_sregs[m_memory][0][0x54])->value("[var_gain_enable]") == 1)
    {
        y[2] = y[1] + 0.38 * ((float) SREGX[m_memory].at(m_map_sregs[m_memory][0][0x4E])->value("[delta_gain_val0]"));
        y[3] = y[2] + 0.38 * ((float) SREGX[m_memory].at(m_map_sregs[m_memory][0][0x50])->value("[delta_gain_val1]"));
        y[4] = y[3] + 0.38 * ((float) SREGX[m_memory].at(m_map_sregs[m_memory][0][0x52])->value("[delta_gain_val2]"));
        y[5] = y[4] + 0.38 * ((float) SREGX[m_memory].at(m_map_sregs[m_memory][0][0x54])->value("[delta_gain_val3]"));
        y[6] = y[5] + 0.38 * ((float) SREGX[m_memory].at(m_map_sregs[m_memory][0][0x56])->value("[delta_gain_val4]"));
        y[7] = y[6];

        /*y[2] = y[1] + GET_REG("SREGX04E[delta_gain_val0]").toFloat()*0.38;
        y[3] = y[2] + GET_REG("SREGX050[delta_gain_val1]").toFloat()*0.38;
        y[4] = y[3] + GET_REG("SREGX052[delta_gain_val2]").toFloat()*0.38;
        y[5] = y[4] + GET_REG("SREGX054[delta_gain_val3]").toFloat()*0.38;
        y[6] = y[5] + GET_REG("SREGX056[delta_gain_val4]").toFloat()*0.38;
        y[7] = y[6];*/
    }
    else
    {
        y[2] = y[1];
        y[3] = y[2];
        y[4] = y[3];
        y[5] = y[4];
        y[6] = y[5];
        y[7] = y[6];
    }

    xg[0] = 0;      xg[1] = 200;      yg[0] = 58;      yg[1] = 58;

    if (m_memory == 0)
    {
        ui->ptGain->graph(0)->setData(x, y);
        ui->ptGain->graph(1)->setData(xg, yg);
        ui->ptGain->replot();
    }
    else if (m_memory == 1)
    {
        ui->ptGainv2->graph(0)->setData(x, y);
        ui->ptGainv2->graph(1)->setData(xg, yg);
        ui->ptGainv2->replot();
    }
}

char DSI3Slave::CalcSlaveCRCfromREG(Register preg)
{
    int i;
    const char generator = 0x2F;    // polynom
    char crc = 0xFF;                // start with 0xFF
    QByteArray data = preg.toByteArray(Register::MSB, true);
    data.remove(data.length()-1, 1);
    data.prepend((char)0x00);

    for (i = 0; i < data.length(); i++)
    {
        crc ^= data.at(i); /* XOR-in the next input byte */

        for (int i = 0; i < 8; i++)
        {
            if ((crc & 0x80) != 0)
            {
                crc = (char)((crc << 1) ^ generator);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}

bool DSI3Slave::ClearSafetyFlags(bool broadcast)
{
    bool result = true;  
    if (broadcast)
    {
        result = result & PerformRWPandWRITE(15, 1, 0x1C, 0xFFFF);
        result = result & PerformRWPandWRITE(15, 1, 0x1E, 0xFFFF);
    }
    else
    {
        result = result & PerformRWPandWRITE(VarSID.value(), 1, 0x1C, 0xFFFF);
        result = result & PerformRWPandWRITE(VarSID.value(), 1, 0x1E, 0xFFFF);
    }
    return result;
}


void DSI3Slave::AddCRCModeConfig()
{
    QByteArray data;
    QString regtype = "Mode";
    int mode;
    int addr;
    quint16 crc;
    QString num;


    for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
    {
        if (SREGX[m_memory].at(i)->extra("type").contains(regtype))
        {
            data.append(SREGX[m_memory].at(i)->toByteArray(Register::LSB, false));
        }
    }
    data.remove(data.length()-2, 2);    //remove CRC for calculation

    crc = gp_UartUartController->CalculateCRC16UPAEwithSEEDfromFPGA(0xFFFF, data);

    //SET_REG("SREGX060[crc_mode_config]", crc);
    SREGX[m_memory].at(m_map_sregs[m_memory][0][0x60])->setValue(0, 15, crc);

    for (mode = 1; mode < 6; mode++)
    {
        data.clear();

        for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
        {
            if (SREGX[m_memory].at(i)->extra("type").contains(regtype))
            {
                addr = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16);
                addr = addr - 0x40 + mode_start_addr[mode];
                data.append(SREGX[m_memory].at(m_map_sregs[m_memory][1][addr])->toByteArray(Register::LSB, false));
            }
        }
        data.remove(data.length()-2, 2);    //remove CRC for calculation

        crc = gp_UartUartController->CalculateCRC16UPAEwithSEEDfromFPGA(0xFFFF, data);
        //crc = gp_UartUartController->CalculateUPAECRC16withSEED(0xFFFF, data);

        //num = QString("%1").arg(0x60 - 0x40 + mode_start_addr[mode],2,16,QChar('0')).toUpper();
        addr = 0x60 - 0x40 + mode_start_addr[mode];
        //SET_REG(QString("SREGX1%1[crc_mode_config]").arg(num), crc);
        SREGX[m_memory].at(m_map_sregs[m_memory][1][addr])->setValue(0, 15, crc);
    }



    slot_updateAllControls();
}



void DSI3Slave::AddCRCNvmCust()
{
    QByteArray data;
    QString regtype = "NVMCust";
    quint16 crc;


    qDebug() << "SREGX[m_memory].size()" << SREGX[m_memory].size();
    qDebug() << "m_memory" << m_memory;


    for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
    {
        if (SREGX[m_memory].at(i)->extra("type").contains(regtype))
        {
            data.append(SREGX[m_memory].at(i)->toByteArray(Register::LSB, false));
            qDebug() << "data.append at i=" << i << ", data=" << data << ", register="
                     << SREGX[m_memory].at(i)->toByteArray(Register::LSB, false);
        }
    }
    data.remove(data.length()-2, 2);    //remove CRC for calculation

    crc = gp_UartUartController->CalculateCRC16UPAEwithSEEDfromFPGA(0xFFFF, data);

    qDebug() << "Calculated crc" << crc;

    //SET_REG("SREGX11A[crc_nvm_cust]", crc);
    SREGX[m_memory].at(m_map_sregs[m_memory][1][0x1A])->setValue(0, 15, crc);

    slot_updateAllControls();
}


void DSI3Slave::AddCRCDSI3Config()
{
    QByteArray data;
    QByteArray dsi3_reg;
    QString regtype = "DSI3";

    for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
    {
        if (SREGX[m_memory].at(i)->extra("type").contains(regtype))
        {
            data.append(SREGX[m_memory].at(i)->toByteArray(Register::LSB, false));
        }
    }
    data.remove(data.length()-2, 2);    //remove CRC for calculation
    dsi3_reg.append(data.at(1)).append(data.at(0));
    char crc = gp_UartUartController->CalculateUPAECRC(dsi3_reg);
    //SET_REG("SREGX122[crc_dsi_config]", crc);
    SREGX[m_memory].at(m_map_sregs[m_memory][1][0x22])->setValue("[crc_dsi_config]", crc);
    slot_updateAllControls();
}



bool DSI3Slave::CheckSlaves()
{
    int rsp_data = 0;
    bool result = false;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    int prev_channel = gp_DSI3Master->CHANNEL.value();

    for (int ch = 0; ch < 2; ch++)
    {
        gp_DSI3Master->CHANNEL.setValue(ch);
        for (int sid = 1; sid < 7; sid++)
        {
            m_slave_connected[ch][sid-1] = PerformREAD(false, sid, 0, 0, &rsp_data);
            if (prev_channel == ch)
            {
                result = result | m_slave_connected[ch][sid-1];
            }

            //SLAVES_CONNECTED.at(6*ch + sid - 1)->setChecked(m_slave_connected[ch][sid-1]);
            CHID_CONNECTED.at(6*ch + sid - 1)->setValue((int)m_slave_connected[ch][sid-1]);
        }
    }

    gp_DSI3Master->CHANNEL.setValue(prev_channel);

    QApplication::restoreOverrideCursor();

    return result;
}

void DSI3Slave::ClearSlaves()
{
    int comp_method;

    for (int ch = 0; ch < 2; ch++)
    {
        for (int sid = 1; sid < 7; sid++)
        {
            CHID_CONNECTED.at(6*ch + sid - 1)->setValue(0);
        }
    }

    if (Varcomp_method.value() == 0)
    {
        comp_method = GET_REG("SREGX052[comp_method]").toInt() + (GET_REG("SREGX058[comp_method_b3]").toInt() << 3);
        Varcomp_method.setValue(comp_method);
    }

    gp_Measurement->UPDATE_MASTER.setValue(1);

    SET_REG("SWriteSWRST[SWRST]", 2);
}

void DSI3Slave::ClearBitstream2022()
{
    SET_REG("Bitstream2022", 0);
}

bool DSI3Slave::WriteDSI3RegsAll()
{
    bool result = true;
    for (int sid = 1; sid < 7; sid++)
    {
        if (m_slave_connected[((int)gp_DSI3Master->CHANNEL.value())&0x01][sid-1] == true)
        {
            VarSID.setValue(sid);
            //SET_REG("SREGX120[mount_id]", sid);
            SREGX[m_memory].at(m_map_sregs[m_memory][1][0x20])->setValue("[mount_id]", sid);
            result = result & WriteSlaveRegs("DSI3");
        }
    }
    VarSID.setValue(15);
    return result;
}

bool DSI3Slave::Synchronize()
{
    bool result = true;

    PerformWRITE_RWP(false, 15, 0, 0, 1, 2, 0);
    //PerformWRITE_RWP(false, 15, 0, 0, 1, 1, 0xe0); - fail
    for (int i = 0; i < 8; i++)
    {
        result = result & PerformWRITE_BLK_P(false, 15, 0, 0, 0, 0, 0, 0);
    }

    return result;
}

void DSI3Slave::CopyModes()
{
    int mode_from = ui->comboBoxCopyFrom->currentIndex();
    int mode_to = ui->comboBoxCopyTo->currentIndex();
    int page_from;
    int page_to;
    int addr_from;
    int addr_to;

    if (m_memory == 0)
    {
        mode_from = ui->comboBoxCopyFrom->currentIndex();
        mode_to = ui->comboBoxCopyTo->currentIndex();
    }
    else
    {
        mode_from = ui->comboBoxCopyFromv2->currentIndex();
        mode_to = ui->comboBoxCopyTov2->currentIndex();
    }



    if (mode_from == mode_to)
    {
        return;
    }

    for (int i = 0; i < SREGX[m_memory].size(); i++) //go through all regs
    {
        if (SREGX[m_memory].at(i)->extra("type").contains("Mode"))
        {
            if (mode_from == 0)
            {
                page_from = 0;
                addr_from = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16);
            }
            else
            {
                page_from = 1;
                addr_from = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16) - 0x40 + mode_start_addr[mode_from];
            }

            if (mode_to == 0)
            {
                page_to = 0;
                addr_to = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16);
            }
            else
            {
                page_to = 1;
                addr_to = SREGX[m_memory].at(i)->extra("adr").toInt(0, 16) - 0x40 + mode_start_addr[mode_to];
            }

            SREGX[m_memory].at(m_map_sregs[m_memory][page_to][addr_to])->setValue(0, 15, SREGX[m_memory].at(m_map_sregs[m_memory][page_from][addr_from])->value(0, 15));
        }
    }

    SDBG.at(mode_to)->setValue(SDBG.at(mode_from)->value());
    SCOMP.at(mode_to)->setValue(SCOMP.at(mode_from)->value());

    for (int sid = 0; sid < 6; sid++)
    {
        SSCALE.at(m_map_sscale[sid][mode_to])->setValue(0, 29, SSCALE.at(m_map_sscale[sid][mode_from])->value(0, 29));
    }

    slot_updateAllControls();
}



void DSI3Slave::slot_VersionUpdate()
{
    if (m_block_update_version)
    {
        return;
    }
    bool hkmc = gp_Settings->mHKMC;


    static const QString meas_type_new[] = {"Ultra near range","Near range 1 period","Near range 2 periods","Near range 4 periods",
           "AM #0 unshaped","AM #1 unshaped","AM #2 unshaped","AM #3 unshaped","Dual AM #0 shaped","Dual AM #1 shaped","Dual AM #2 shaped",
           "Dual AM #3 shaped","Unused","Unused","Unused","Unused","Unused","Unused","Unused","Unused","Full chirp - up BEHL=2 - down BEHH=2",
           "Dual chirp - up","Dual chirp - down","Diagnostic burst","Diagnostic chirp up","Diagnostic chirp down",
           "Diagnostic Piezo model","Diag Ultra narrowband AM","Unused","Unused","Unused","Unused"};
    static const QString meas_type_JUN[] = {"Ultra near range","Near range 1 period","Near range 2 periods","Near range 4 periods",
           "AM #0 unshaped","AM #1 unshaped","AM #2 unshaped","AM #3 unshaped","Dual AM #0 shaped","Dual AM #1 shaped","Dual AM #2 shaped",
           "Dual AM #3 shaped","Doppler to carr","Doppler from carr","Unused","Unused","Up-chirp at H, AM at L","Up-chirp at L, AM at H","Unused","Unused","Full chirp - up BEHL=2 - down BEHH=2",
           "Dual chirp - up","Dual chirp - down","Diagnostic burst","Diagnostic chirp up","Diagnostic chirp down",
           "Diagnostic Piezo model","Diag Ultra narrowband AM","Unused","Unused","Unused","Unused"};
    static const QString meas_type_old[] = {"Ultra near range","Near range 1 period","Near range 2 periods","Near range 4 periods",
           "AM #0 unshaped","AM #1 unshaped","AM #2 unshaped","AM #3 unshaped","Dual AM #0 shaped","Dual AM #1 shaped","Dual AM #2 shaped",
           "Dual AM #3 shaped","Unused","Unused","Unused","Unused","Unused","Unused","Unused","Unused","Full chirp - up BEHL=2 - down BEHH=2",
           "Unused","Unused","Dual chirp - up","Dual chirp - down","Diagnostic burst","Diagnostic chirp up","Diagnostic chirp down","Unused",
           "Diagnostic Piezo model","Diag Ultra narrowband AM","Unused"};
    static const QString comp_method_new[] = {"0 Not implemented","1 5b log scale + 9 samp. + noise mon.","2 Uncompressed 8b TST3 and TST2",
           "3 Diag. chirp, differential coding","4 Not implemented","5 Diag Diff coding for Piezo model","6 5b log scale + 9 samp. + noise mon. + WBN",
           "7 Not implemented","8 Not implemented","9 Not implemented","10 Not implemented","11 Not implemented","12 Not implemented",
           "13 Not implemented","14 Not implemented","15 Not implemented"};
    static const QString comp_method_JUN[] = {"0 Not implemented","1 5b log scale + 9 samp. + noise mon.","2 Uncompressed 8b TST3 and TST2",
           "3 Diag. chirp, differential coding","4 5b log scale + 9 samp.","5 Diag Diff coding for Piezo model","6 5b log scale + 9 samp. + noise mon. + WBN",
           "7 5b log scale + 9 samp. + WBN","8 Not implemented","9 Not implemented","10 Not implemented","11 Not implemented","12 Not implemented",
           "13 Not implemented","14 Not implemented","15 Not implemented"};
    /*static const QString comp_method_MOBEv2[] = {"0 Not implemented","1 5b log scale + 9 samp. + noise mon.","2 Uncompr 51us TST3 and TST2",
           "3 Diag. chirp, differential coding","4 5b log scale + 9 samp.","5 Diag Diff coding for Piezo model","6 5b log scale + 9 samp. + noise mon. + WBN",
           "7 5b log scale + 9 samp. + WBN","8 Not implemented","9 Not implemented","10 Not implemented","11 Not implemented","12 Not implemented",
           "13 Not implemented","14 Not implemented","15 Not implemented"};*/
    static const QString comp_method_MOBEv2[] = {"0 Uncompr 102us TST3 TST2","1 5b log scale + 9 samp. + noise mon.","2 Uncompr 51us TST3 and TST2",
           "3 Diag. chirp, differential coding","4 5b log scale + 9 samp.","5 Diag Diff coding for Piezo model","6 5b log scale + 9 samp. + noise mon. + WBN",
           "7 5b log scale + 9 samp. + WBN","8 IQ uncompr 102.4 + WBN","9 IQ compr 102.4","10 Not implemented","11 Not implemented","12 Not implemented",
           "13 Not implemented","14 Not implemented","15 Not implemented"};
    static const QString comp_method_old[] = {"-","-","5b log scale + 9 samples (3b/sample)","5b log scale + 9 samp. + noise mon.",
           "Uncompressed 8b TST3 and TST2","Diag. chirp, differential coding","TBD CFAR ToF Data","Diag Diff coding for Piezo model",
           "-","-","5b log scale + 9 samples (3b/sample) + WBN","5b log scale + 9 samp. + noise mon. + WBN","-","-","-","-"};

    int i, mode;

    for(mode = 0; mode < 6; mode++)
    {
        for (i = 0; i < 32; i++)
        {
            if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBE) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LIST.at(mode)->setItemText(i, meas_type_old[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_ULSE) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LIST.at(mode)->setItemText(i, meas_type_new[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUS) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LIST.at(mode)->setItemText(i, meas_type_old[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUSDMX) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LIST.at(mode)->setItemText(i, meas_type_old[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEAPR) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LISTv2.at(mode)->setItemText(i, meas_type_new[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEJUN) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LISTv2.at(mode)->setItemText(i, meas_type_JUN[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUS2DMX) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LISTv2.at(mode)->setItemText(i, meas_type_new[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEDMX) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LISTv2.at(mode)->setItemText(i, meas_type_JUN[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DCH) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LISTv2.at(mode)->setItemText(i, meas_type_JUN[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUSBSD) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LISTv2.at(mode)->setItemText(i, meas_type_JUN[i]);
                MEAS_TYPE_LISTv2BSD.at(mode)->setItemText(i, meas_type_JUN[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DMX) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LISTv2.at(mode)->setItemText(i, meas_type_JUN[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBE2DMX) && (ui->comboBoxVersion->currentText() != "-"))
            {
                MEAS_TYPE_LISTv2.at(mode)->setItemText(i, meas_type_JUN[i]);
                MEAS_TYPE_LISTv2BSD.at(mode)->setItemText(i, meas_type_JUN[i]);
            }
            else
            {
                MEAS_TYPE_LIST.at(mode)->setItemText(i, "Incorrect version");
            }
        }
        for (i = 0; i < 16; i++)
        {
            if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBE) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LIST.at(mode)->setItemText(i, comp_method_old[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_ULSE) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LIST.at(mode)->setItemText(i, comp_method_new[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUS) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LIST.at(mode)->setItemText(i, comp_method_old[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUSDMX) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LIST.at(mode)->setItemText(i, comp_method_old[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEAPR) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LISTv2.at(mode)->setItemText(i, comp_method_new[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEJUN) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LISTv2.at(mode)->setItemText(i, comp_method_JUN[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUS2DMX) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LISTv2.at(mode)->setItemText(i, comp_method_new[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEDMX) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LISTv2.at(mode)->setItemText(i, comp_method_JUN[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DCH) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LISTv2.at(mode)->setItemText(i, comp_method_JUN[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUSBSD) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LISTv2.at(mode)->setItemText(i, comp_method_MOBEv2[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DMX) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LISTv2.at(mode)->setItemText(i, comp_method_JUN[i]);
            }
            else if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBE2DMX) && (ui->comboBoxVersion->currentText() != "-"))
            {
                COMP_METHOD_LISTv2.at(mode)->setItemText(i, comp_method_MOBEv2[i]);
            }
            else
            {
                COMP_METHOD_LIST.at(mode)->setItemText(i, "Incorrect version");
            }
        }
    }

    ui->groupBoxVersion->setEnabled(!hkmc);
    ui->comboBoxChannel->setEnabled(!hkmc);

    if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEAPR) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBEJUN)|| (ui->comboBoxVersion->currentIndex() == VERSION_RDUS2DMX)\
            || (ui->comboBoxVersion->currentIndex() == VERSION_MOBEDMX) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DCH) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUSBSD)\
            || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DMX)|| (ui->comboBoxVersion->currentIndex() == VERSION_MOBE2DMX))
    {
        m_memory = 1;
        ui->tabWidget->setTabEnabled(0, false);
        ui->tabWidget->setTabEnabled(1, true);

        ui->tabNvmMem->setTabEnabled(0, false);
        ui->tabNvmMem->setTabEnabled(1, true);


        if (ui->tabWidget->currentIndex() == 0)
        {
            ui->tabWidget->setCurrentIndex(1);
        }

        ui->groupBoxPredefActionsv2->setEnabled(!hkmc);
        ui->groupBoxSAFETYTO2v2->setEnabled(!hkmc);
        ui->groupBoxCommonRegsv2->setEnabled(!hkmc);
        ui->groupBoxDSI3regsv2->setEnabled(!hkmc);
        ui->groupBoxReadOnlyv2->setEnabled(!hkmc);
        ui->groupBoxFreqTrimv2->setEnabled(!hkmc);
        //ui->groupBoxModeRegsv2->setEnabled(true);
    }
    else
    {
        m_memory = 0;

        ui->tabWidget->setTabEnabled(0, true);
        if (ui->tabWidget->currentIndex() == 1)
        {
            ui->tabWidget->setCurrentIndex(0);
        }
        ui->tabWidget->setTabEnabled(1, false);

        ui->tabNvmMem->setTabEnabled(0, true);
        ui->tabNvmMem->setTabEnabled(1, false);

        ui->groupBoxPredefActions->setEnabled(!hkmc);
        ui->groupBoxSAFETYTO2->setEnabled(!hkmc);
        ui->groupBoxCommonRegs->setEnabled(!hkmc);
        ui->groupBoxDSI3regs->setEnabled(!hkmc);
        ui->groupBoxReadOnly->setEnabled(!hkmc);
        ui->groupBoxFreqTrim->setEnabled(!hkmc);
        //ui->groupBoxModeRegs->setEnabled(true);
    }

    if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUSBSD) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBE2DMX))
    {
        gp_Measurement->HideBSD(false);
    }
    else
    {
        gp_Measurement->HideBSD(true);
    }

    QList<QLineEdit *> allLineEdit;
    QList<QLabel *> allLabel;
    QList<QComboBox *> allComboBox;


    if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEAPR) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS2DMX))
    {

        allLineEdit.clear();
        allLineEdit = ui->tabWidgetMODEv2->findChildren<QLineEdit *>();
        foreach (QLineEdit *le, allLineEdit)
        {
            if (le->objectName().contains("APR"))
            {
                le->setVisible(true);
            }
            if (le->objectName().contains("JUN"))
            {
                le->setVisible(false);
            }
            if (le->objectName().contains("BSD"))
            {
                le->setVisible(false);
            }
            if (le->objectName().contains("ND"))
            {
                le->setVisible(true);
            }
        }
        allLabel.clear();
        allLabel = ui->tabWidgetMODEv2->findChildren<QLabel *>();
        foreach (QLabel *la, allLabel)
        {
            if (la->objectName().contains("APR"))
            {
                la->setVisible(true);
            }
            if (la->objectName().contains("JUN"))
            {
                la->setVisible(false);
            }
            if (la->objectName().contains("BSD"))
            {
                la->setVisible(false);
            }
            if (la->objectName().contains("ND"))
            {
                la->setVisible(true);
            }
        }
        allComboBox.clear();
        allComboBox = ui->tabWidgetMODEv2->findChildren<QComboBox *>();
        foreach (QComboBox *cb, allComboBox)
        {
            if (cb->objectName().contains("BSD"))
            {
                cb->setVisible(false);
            }
            if (cb->objectName().contains("ND"))
            {
                cb->setVisible(true);
            }
        }
    }
    else if ((ui->comboBoxVersion->currentIndex() == VERSION_MOBEJUN) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBEDMX) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DCH)\
             || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DMX))
    {
        allLineEdit.clear();
        allLineEdit = ui->tabWidgetMODEv2->findChildren<QLineEdit *>();
        foreach (QLineEdit *le, allLineEdit)
        {
            if (le->objectName().contains("JUN"))
            {
                le->setVisible(true);
            }
            if (le->objectName().contains("APR"))
            {
                le->setVisible(false);
            }
            if (le->objectName().contains("BSD"))
            {
                le->setVisible(false);
            }
            if (le->objectName().contains("ND"))
            {
                le->setVisible(true);
            }
        }
        allLabel.clear();
        allLabel = ui->tabWidgetMODEv2->findChildren<QLabel *>();
        foreach (QLabel *la, allLabel)
        {
            if (la->objectName().contains("JUN"))
            {
                la->setVisible(true);
            }
            if (la->objectName().contains("APR"))
            {
                la->setVisible(false);
            }
            if (la->objectName().contains("BSD"))
            {
                la->setVisible(false);
            }
            if (la->objectName().contains("ND"))
            {
                la->setVisible(true);
            }
        }
        allComboBox.clear();
        allComboBox = ui->tabWidgetMODEv2->findChildren<QComboBox *>();
        foreach (QComboBox *cb, allComboBox)
        {
            if (cb->objectName().contains("BSD"))
            {
                cb->setVisible(false);
            }
            if (cb->objectName().contains("ND"))
            {
                cb->setVisible(true);
            }
        }
    }
    else if ((ui->comboBoxVersion->currentIndex() == VERSION_RDUSBSD) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBE2DMX))
    {
        allLineEdit.clear();
        allLineEdit = ui->tabWidgetMODEv2->findChildren<QLineEdit *>();
        foreach (QLineEdit *le, allLineEdit)
        {
            if (le->objectName().contains("JUN"))
            {
                le->setVisible(true);
            }
            if (le->objectName().contains("APR"))
            {
                le->setVisible(false);
            }
            if (le->objectName().contains("BSD"))
            {
                le->setVisible(true);
            }
            if (le->objectName().contains("ND"))
            {
                le->setVisible(false);
            }
        }
        allLabel.clear();
        allLabel = ui->tabWidgetMODEv2->findChildren<QLabel *>();
        foreach (QLabel *la, allLabel)
        {
            if (la->objectName().contains("JUN"))
            {
                la->setVisible(true);
            }
            if (la->objectName().contains("APR"))
            {
                la->setVisible(false);
            }
            if (la->objectName().contains("BSD"))
            {
                la->setVisible(true);
            }
            if (la->objectName().contains("ND"))
            {
                la->setVisible(false);
            }
        }
        allComboBox.clear();
        allComboBox = ui->tabWidgetMODEv2->findChildren<QComboBox *>();
        foreach (QComboBox *cb, allComboBox)
        {
            if (cb->objectName().contains("BSD"))
            {
                cb->setVisible(true);
            }
            if (cb->objectName().contains("ND"))
            {
                cb->setVisible(false);
            }
        }
    }

    ui->tabWidget->setTabEnabled(2,!hkmc);
    ui->tabWidget->setTabEnabled(3,!hkmc);

    if (gp_Settings->mVersion == VERSION_UPID)
    {
        ui->tabWidget->setTabEnabled(4, true);
    }
    else
    {
        ui->tabWidget->setTabEnabled(4, false);
    }

    ChangeConfiguration();
}

void DSI3Slave::ChangeConfiguration()
{
    int comp_method;
    int meas_type;
    int mode;
    if(ui->comboBoxVersion->currentIndex() == m_PrevVERSION)
    {
        return;
    }

    int curr;
    int prev;

    if((m_PrevVERSION == VERSION_ULSE) || (m_PrevVERSION == VERSION_MOBEAPR) || (m_PrevVERSION == VERSION_MOBEJUN) || (m_PrevVERSION == VERSION_RDUS2DMX)\
            || (m_PrevVERSION == VERSION_MOBEDMX) || (m_PrevVERSION == VERSION_RDUS3DCH) || (m_PrevVERSION == VERSION_RDUSBSD) || (m_PrevVERSION == VERSION_RDUS3DMX) || (m_PrevVERSION == VERSION_MOBE2DMX))
    {
        prev = 1;
    }
    else
    {
        prev = 0;
    }

    if((ui->comboBoxVersion->currentIndex() == VERSION_ULSE) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBEAPR) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBEJUN)\
            || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS2DMX) || (ui->comboBoxVersion->currentIndex() == VERSION_MOBEDMX)\
            || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DCH) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUSBSD)\
            || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS3DMX)|| (ui->comboBoxVersion->currentIndex() == VERSION_MOBE2DMX))
    {
        curr = 1;
    }
    else
    {
        curr = 0;
    }


    //if ((ui->comboBoxVersion->currentIndex() == VERSION_ULSE) && (m_PrevVERSION != VERSION_ULSE))      // OldToNew
    if ((curr == 1) && (prev == 0))      // OldToNew
    {
        comp_method = Varcomp_method.value();
        comp_method = comp_method_recalc_OldToNew[comp_method];
        SREGX[0].at(m_map_sregs[0][0][0x52])->setValue(11, 13, comp_method & 0x07);
        SREGX[0].at(m_map_sregs[0][0][0x58])->setValue(7, 7, 0);
        Varcomp_method.setValue(comp_method);

        meas_type = SREGX[0].at(m_map_sregs[0][0][0x42])->value(10, 14);        // meas_type
        meas_type = meas_type_recalc_OldToNew[meas_type];
        SREGX[0].at(m_map_sregs[0][0][0x42])->setValue(10, 14, meas_type);

        for (mode = 1; mode < 6; mode++)
        {
            comp_method = SCOMP.at(mode)->value();
            comp_method = comp_method_recalc_OldToNew[comp_method];
            SREGX[0].at(m_map_sregs[0][1][0x52 - 0x40 + mode_start_addr[mode]])->setValue(11, 13, comp_method & 0x07);
            SREGX[0].at(m_map_sregs[0][1][0x58 - 0x40 + mode_start_addr[mode]])->setValue(7, 7, 0);
            SCOMP.at(mode)->setValue(comp_method);


            meas_type = SREGX[0].at(m_map_sregs[0][1][0x42 - 0x40 + mode_start_addr[mode]])->value(10, 14);        // meas_type
            meas_type = meas_type_recalc_OldToNew[meas_type];
            SREGX[0].at(m_map_sregs[0][1][0x42 - 0x40 + mode_start_addr[mode]])->setValue(10, 14, meas_type);
        }

    }
    //else if (((ui->comboBoxVersion->currentIndex() == VERSION_MOBE) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUS) || (ui->comboBoxVersion->currentIndex() == VERSION_RDUSDMX)) && (m_PrevVERSION == VERSION_ULSE))      // NewToOld
    else if ((curr == 0) && (prev == 1))      // NewToOld
    {
        comp_method = Varcomp_method.value();
        comp_method = comp_method_recalc_NewToOld[comp_method];
        SREGX[0].at(m_map_sregs[0][0][0x52])->setValue(11, 13, comp_method & 0x07);
        SREGX[0].at(m_map_sregs[0][0][0x58])->setValue(7, 7, 0);
        Varcomp_method.setValue(comp_method);

        meas_type = SREGX[0].at(m_map_sregs[0][0][0x42])->value(10, 14);        // meas_type
        meas_type = meas_type_recalc_NewToOld[meas_type];
        SREGX[0].at(m_map_sregs[0][0][0x42])->setValue(10, 14, meas_type);

        for (mode = 1; mode < 6; mode++)
        {
            comp_method = SCOMP.at(mode)->value();
            comp_method = comp_method_recalc_NewToOld[comp_method];
            SREGX[0].at(m_map_sregs[0][1][0x52 - 0x40 + mode_start_addr[mode]])->setValue(11, 13, comp_method & 0x07);
            SREGX[0].at(m_map_sregs[0][1][0x58 - 0x40 + mode_start_addr[mode]])->setValue(7, 7, 0);
            SCOMP.at(mode)->setValue(comp_method);


            meas_type = SREGX[0].at(m_map_sregs[0][1][0x42 - 0x40 + mode_start_addr[mode]])->value(10, 14);        // meas_type
            meas_type = meas_type_recalc_NewToOld[meas_type];
            SREGX[0].at(m_map_sregs[0][1][0x42 - 0x40 + mode_start_addr[mode]])->setValue(10, 14, meas_type);
        }

    }
    m_PrevVERSION = ui->comboBoxVersion->currentIndex();
}

int DSI3Slave::Getcomp_method_old(int mode)
{
    if ((gp_DSI3Master->MVar_version.value() == VERSION_ULSE) || (gp_DSI3Master->MVar_version.value() == VERSION_MOBEAPR) || (gp_DSI3Master->MVar_version.value() == VERSION_MOBEJUN)\
            || (gp_DSI3Master->MVar_version.value() == VERSION_RDUS2DMX) || (gp_DSI3Master->MVar_version.value() == VERSION_MOBEDMX)\
            || (gp_DSI3Master->MVar_version.value() == VERSION_RDUS3DCH) || (gp_DSI3Master->MVar_version.value() == VERSION_RDUSBSD)\
            || (gp_DSI3Master->MVar_version.value() == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version.value() == VERSION_MOBE2DMX))
    {
        return comp_method_recalc_NewToOld[(int)SCOMP.at(mode)->value()];
    }
    else
    {
        return SCOMP.at(mode)->value();
    }

}

int DSI3Slave::Getmeas_type_old(int mode)
{
    if ((gp_DSI3Master->MVar_version.value() == VERSION_ULSE) && (mode == 0))
    {
        return meas_type_recalc_NewToOld[SREGX[0].at(m_map_sregs[m_memory][0][0x42])->value(10, 14)];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_ULSE) && (mode != 0))
    {
        return meas_type_recalc_NewToOld[SREGX[0].at(m_map_sregs[m_memory][1][0x42 - 0x40 + mode_start_addr[mode]])->value(10, 14)];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_MOBEAPR) && (mode == 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][0][0x4A])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_MOBEAPR) && (mode != 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][1][0x4A - 0x40 + mode_start_addr[mode]])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_MOBEJUN) && (mode == 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][0][0x4A])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_MOBEJUN) && (mode != 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][1][0x4A - 0x40 + mode_start_addr[mode]])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_RDUS2DMX) && (mode == 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][0][0x4A])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_RDUS2DMX) && (mode != 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][1][0x4A - 0x40 + mode_start_addr[mode]])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_MOBEDMX) && (mode == 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][0][0x4A])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_MOBEDMX) && (mode != 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][1][0x4A - 0x40 + mode_start_addr[mode]])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_RDUS3DCH) && (mode == 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][0][0x4A])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_RDUS3DCH) && (mode != 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][1][0x4A - 0x40 + mode_start_addr[mode]])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_RDUSBSD) && (mode == 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][0][0x4A])->value(8,12)];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_RDUSBSD) && (mode != 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][1][0x4A - 0x40 + mode_start_addr[mode]])->value(8,12)];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_RDUS3DMX) && (mode == 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][0][0x4A])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_RDUS3DMX) && (mode != 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][1][0x4A - 0x40 + mode_start_addr[mode]])->value("[meas_type]")];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_MOBE2DMX) && (mode == 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][0][0x4A])->value(8,12)];
    }
    else if ((gp_DSI3Master->MVar_version.value() == VERSION_MOBE2DMX) && (mode != 0))
    {
        return meas_type_recalc_NewToOld[SREGX[m_memory].at(m_map_sregs[m_memory][1][0x4A - 0x40 + mode_start_addr[mode]])->value(8,12)];
    }

    else if (mode == 0)
    {
        return SREGX[m_memory].at(m_map_sregs[0][0][0x42])->value(10, 14);
    }
    else    // (mode != 0)
    {
        return SREGX[m_memory].at(m_map_sregs[0][1][0x42 - 0x40 + mode_start_addr[mode]])->value(10, 14);
    }


}
void DSI3Slave::CheckVersionConfig()
{
    QString filename = "dsi3pas.cfg";
    QFile file(filename);
    QString input;
    QString iq4D;
    QStringList qsl_input;

    ui->comboBoxVersion->setItemText(0, "-");
    ui->comboBoxVersion->setItemText(1, "-");
    ui->comboBoxVersion->setItemText(2, "-");
    ui->comboBoxVersion->setItemText(3, "-");
    ui->comboBoxVersion->setItemText(4, "-");
    ui->comboBoxVersion->setItemText(5, "-");
    ui->comboBoxVersion->setItemText(6, "-");
    ui->comboBoxVersion->setItemText(7, "-");
    ui->comboBoxVersion->setItemText(8, "-");
    ui->comboBoxVersion->setItemText(9, "-");
    ui->comboBoxVersion->setItemText(10, "-");
    ui->comboBoxVersion->setItemText(11, "-");
    gp_DSI3Master->Ver_setItemText(0, "-");
    gp_DSI3Master->Ver_setItemText(1, "-");
    gp_DSI3Master->Ver_setItemText(2, "-");
    gp_DSI3Master->Ver_setItemText(3, "-");
    gp_DSI3Master->Ver_setItemText(4, "-");
    gp_DSI3Master->Ver_setItemText(5, "-");
    gp_DSI3Master->Ver_setItemText(6, "-");
    gp_DSI3Master->Ver_setItemText(7, "-");
    gp_DSI3Master->Ver_setItemText(8, "-");
    gp_DSI3Master->Ver_setItemText(9, "-");
    gp_DSI3Master->Ver_setItemText(10, "-");
    gp_DSI3Master->Ver_setItemText(11, "-");
    gp_ASICMaster->Ver_setItemText(0, "-");
    gp_ASICMaster->Ver_setItemText(1, "-");
    gp_ASICMaster->Ver_setItemText(2, "-");
    gp_ASICMaster->Ver_setItemText(3, "-");
    gp_ASICMaster->Ver_setItemText(4, "-");
    gp_ASICMaster->Ver_setItemText(5, "-");
    gp_ASICMaster->Ver_setItemText(6, "-");
    gp_ASICMaster->Ver_setItemText(7, "-");
    gp_ASICMaster->Ver_setItemText(8, "-");
    gp_ASICMaster->Ver_setItemText(9, "-");
    gp_ASICMaster->Ver_setItemText(10, "-");
    gp_ASICMaster->Ver_setItemText(11, "-");
    gp_RDUMMaster->Ver_setItemText(0, "-");
    gp_RDUMMaster->Ver_setItemText(1, "-");
    gp_RDUMMaster->Ver_setItemText(2, "-");
    gp_RDUMMaster->Ver_setItemText(3, "-");
    gp_RDUMMaster->Ver_setItemText(4, "-");
    gp_RDUMMaster->Ver_setItemText(5, "-");
    gp_RDUMMaster->Ver_setItemText(6, "-");
    gp_RDUMMaster->Ver_setItemText(7, "-");
    gp_RDUMMaster->Ver_setItemText(8, "-");
    gp_RDUMMaster->Ver_setItemText(9, "-");
    gp_RDUMMaster->Ver_setItemText(10, "-");
    gp_RDUMMaster->Ver_setItemText(11, "-");
    if (!file.exists())
    {
        ui->comboBoxVersion->setItemText(0, "DSI3PAS 2021-");
        gp_DSI3Master->Ver_setItemText(0, "DSI3PAS 2021-");
        gp_ASICMaster->Ver_setItemText(0, "DSI3PAS 2021-");
        gp_RDUMMaster->Ver_setItemText(0, "DSI3PAS 2021-");
    }
    else if(!file.open(QIODevice::ReadOnly))
    {
        ui->comboBoxVersion->setItemText(0, "DSI3PAS 2021-");        
        gp_DSI3Master->Ver_setItemText(0, "DSI3PAS 2021-");
        gp_ASICMaster->Ver_setItemText(0, "DSI3PAS 2021-");
        gp_RDUMMaster->Ver_setItemText(0, "DSI3PAS 2021-");
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
                gp_Measurement->ShowIQtab();
            }
            if (qsl_input.at(1) == "4DULS"){
                gp_Measurement->Show4DULStab();
            }
        }

        qsl_input = input.split(",");
        if (qsl_input.size() < 1)
        {           
            return;
        }
        if (qsl_input.at(0) == "MOBE"){
            ui->comboBoxVersion->setItemText(0, "DSI3PAS 2021-");
            gp_DSI3Master->Ver_setItemText(0, "DSI3PAS 2021-");
            gp_ASICMaster->Ver_setItemText(0, "DSI3PAS 2021-");
            gp_RDUMMaster->Ver_setItemText(0, "DSI3PAS 2021-");
        }
        if (qsl_input.size() < 2)
        {          
            return;
        }
        if (qsl_input.at(1) == "ULSE"){
            ui->comboBoxVersion->setItemText(1, "DSI3PAS 2022 JAN");
            gp_DSI3Master->Ver_setItemText(1, "DSI3PAS 2022 JAN");
            gp_ASICMaster->Ver_setItemText(1, "DSI3PAS 2022 JAN");
            gp_RDUMMaster->Ver_setItemText(1, "DSI3PAS 2022 JAN");
        }
        if (qsl_input.size() < 3)
        {           
            return;
        }
        if (qsl_input.at(2) == "RDUS"){
            ui->comboBoxVersion->setItemText(2, "RDUS daisychain");
            gp_DSI3Master->Ver_setItemText(2, "RDUS daisychain");
            gp_ASICMaster->Ver_setItemText(2, "RDUS daisychain");
            gp_RDUMMaster->Ver_setItemText(2, "RDUS daisychain");
        }
        if (qsl_input.size() < 4)
        {
            return;
        }
        if (qsl_input.at(3) == "RDUSDMX"){
            ui->comboBoxVersion->setItemText(3, "RDUS demuxes");
            gp_DSI3Master->Ver_setItemText(3, "RDUS demuxes");
            gp_ASICMaster->Ver_setItemText(3, "RDUS demuxes");
            gp_RDUMMaster->Ver_setItemText(3, "RDUS demuxes");
        }
        if (qsl_input.size() < 5)
        {
            return;
        }
        if (qsl_input.at(4) == "MOBEAPR"){
            ui->comboBoxVersion->setItemText(4, "DSI3PAS 2022 APR");
            gp_DSI3Master->Ver_setItemText(4, "DSI3PAS 2022 APR");
            gp_ASICMaster->Ver_setItemText(4, "DSI3PAS 2022 APR");
            gp_RDUMMaster->Ver_setItemText(4, "DSI3PAS 2022 APR");
        }
        if (qsl_input.size() < 6)
        {
            return;
        }
        if (qsl_input.at(5) == "MOBEJUN"){
            ui->comboBoxVersion->setItemText(5, "DSI3PAS 2022 JUN");
            gp_DSI3Master->Ver_setItemText(5, "DSI3PAS 2022 JUN");
            gp_ASICMaster->Ver_setItemText(5, "DSI3PAS 2022 JUN");
            gp_RDUMMaster->Ver_setItemText(5, "DSI3PAS 2022 JUN");
        }
        if (qsl_input.size() < 7)
        {
            return;
        }
        if (qsl_input.at(6) == "RDUS2DMX"){
            ui->comboBoxVersion->setItemText(6, "RDUS 2 demuxes");
            gp_DSI3Master->Ver_setItemText(6, "RDUS 2 demuxes");
            gp_ASICMaster->Ver_setItemText(6, "RDUS 2 demuxes");
            gp_RDUMMaster->Ver_setItemText(6, "RDUS 2 demuxes");
        }
        if (qsl_input.size() < 8)
        {
            return;
        }
        if (qsl_input.at(7) == "MOBEDMX"){
            ui->comboBoxVersion->setItemText(7, "MOBE demuxes");
            gp_DSI3Master->Ver_setItemText(7, "MOBE demuxes");
            gp_ASICMaster->Ver_setItemText(7, "MOBE demuxes");
            gp_RDUMMaster->Ver_setItemText(7, "MOBE demuxes");
        }
        if (qsl_input.size() < 9)
        {
            return;
        }
        if (qsl_input.at(8) == "RDUS3DCH"){
            ui->comboBoxVersion->setItemText(8, "RDUS 3 daisychain");
            gp_DSI3Master->Ver_setItemText(8, "RDUS 3 daisychain");
            gp_ASICMaster->Ver_setItemText(8, "RDUS 3 daisychain");
            gp_RDUMMaster->Ver_setItemText(8, "RDUS 3 daisychain");
        }
        if (qsl_input.size() < 10)
        {
            return;
        }
        if (qsl_input.at(9) == "RDUSBSD"){
            ui->comboBoxVersion->setItemText(9, "MOBEv2 daisychain");
            gp_DSI3Master->Ver_setItemText(9, "MOBEv2 daisychain");
            gp_ASICMaster->Ver_setItemText(9, "MOBEv2 daisychain");
            gp_RDUMMaster->Ver_setItemText(9, "MOBEv2 daisychain");
        }
        if (qsl_input.size() < 11)
        {
            return;
        }
        if (qsl_input.at(10) == "RDUS3DMX"){
            ui->comboBoxVersion->setItemText(10, "RDUS 3 demuxes");
            gp_DSI3Master->Ver_setItemText(10, "RDUS 3 demuxes");
            gp_ASICMaster->Ver_setItemText(10, "RDUS 3 demuxes");
            gp_RDUMMaster->Ver_setItemText(10, "RDUS 3 demuxes");
        }
        if (qsl_input.size() < 12)
        {
            return;
        }
        if (qsl_input.at(11) == "MOBE2DMX"){
            ui->comboBoxVersion->setItemText(11, "MOBEv2 demuxes");
            gp_DSI3Master->Ver_setItemText(11, "MOBEv2 demuxes");
            gp_ASICMaster->Ver_setItemText(11, "MOBEv2 demuxes");
            gp_RDUMMaster->Ver_setItemText(11, "MOBEv2 demuxes");
        }
    }
}


void DSI3Slave::CopyAccessNames(int ver)
{
    QList<QLineEdit *> allLineEdit;
    QList<QComboBox *> allComboBox;
    QList<QCheckBox *> allCheckBox;
    QString accessname;
    QString regname;
    int mode;
    int idx;
    bool res;
    if (ver == 0)
    {
        COMP_METHOD_LIST.clear();
        COMP_METHOD_LIST.append(ui->comboBox_compmethod0);
        MEAS_TYPE_LIST.clear();
        MEAS_TYPE_LIST.append(ui->comboBox_meastype0);

        for (mode = 1; mode < 6; mode++)        //Memory v1
        {
            allLineEdit.clear();
            allLineEdit = ui->tabWidgetMODE->widget(mode)->findChildren<QLineEdit *>();
            foreach (QLineEdit *le, allLineEdit)
            {
                accessname = le->accessibleName();
                if (accessname != "")
                {
                    if (accessname.contains("SREGX010"))
                    {

                    }
                    else if (accessname.contains("SREGX0"))
                    {
                        accessname.remove("SREGX0");
                        regname = accessname.split("[").at(1);
                        regname.remove("]");
                        accessname = accessname.split("[").at(0);
                        idx = accessname.toInt(&res, 16);
                        idx = idx - 0x40 + mode_start_addr[mode];
                        accessname = QString("SREGX1%1").arg(idx,2,16,QChar('0')).toUpper();
                        accessname = QString("%1[%2]").arg(accessname).arg(regname);
                    }
                    else if(accessname.contains("SSCALE"))
                    {
                        accessname.insert(11, QString("_MODE%1").arg(mode));
                    }
                    else if(accessname.contains("Varcomp_method"))
                    {
                        accessname.insert(14, QString("_MODE%1").arg(mode));
                    }
                    else if(accessname.contains("Vardbg_out_sel"))
                    {
                        accessname.insert(14, QString("_MODE%1").arg(mode));
                    }

                    //else
                    //{
                    //    accessname = QString("%1_MODE%2").arg(accessname).arg(mode);
                    //}
                }
                le->setAccessibleName(accessname);
            }

            allComboBox.clear();
            allComboBox = ui->tabWidgetMODE->widget(mode)->findChildren<QComboBox *>();
            foreach (QComboBox *cb, allComboBox)
            {
                accessname = cb->accessibleName();
                if (accessname != "")
                {
                    if(accessname.contains("Varcomp_method"))
                    {
                        accessname.insert(14, QString("_MODE%1").arg(mode));
                        COMP_METHOD_LIST.append(cb);
                    }
                    else if(accessname.contains("Vardbg_out_sel"))
                    {
                        accessname.insert(14, QString("_MODE%1").arg(mode));
                    }
                    else if (accessname.contains("SREGX0"))
                    {
                        accessname.remove("SREGX0");
                        regname = accessname.split("[").at(1);
                        regname.remove("]");
                        accessname = accessname.split("[").at(0);
                        idx = accessname.toInt(&res, 16);
                        idx = idx - 0x40 + mode_start_addr[mode];
                        accessname = QString("SREGX1%1").arg(idx,2,16,QChar('0')).toUpper();
                        accessname = QString("%1[%2]").arg(accessname).arg(regname);
                        if (regname == "meas_type")
                        {
                            MEAS_TYPE_LIST.append(cb);
                        }
                    }
                }
                cb->setAccessibleName(accessname);
            }

            allCheckBox.clear();
            allCheckBox = ui->tabWidgetMODE->widget(mode)->findChildren<QCheckBox *>();
            foreach (QCheckBox *cbx, allCheckBox)
            {
                accessname = cbx->accessibleName();
                if (accessname != "")
                {
                    if (accessname.contains("SREGX0"))
                    {
                        accessname.remove("SREGX0");
                        regname = accessname.split("[").at(1);
                        regname.remove("]");
                        accessname = accessname.split("[").at(0);
                        idx = accessname.toInt(&res, 16);
                        idx = idx - 0x40 + mode_start_addr[mode];
                        accessname = QString("SREGX1%1").arg(idx,2,16,QChar('0')).toUpper();
                        accessname = QString("%1[%2]").arg(accessname).arg(regname);
                    }
                }
                cbx->setAccessibleName(accessname);
            }

        }
    }
    else if (ver == 2)
    {

        COMP_METHOD_LISTv2.clear();
        COMP_METHOD_LISTv2.append(ui->comboBox_compmethod0v2);
        MEAS_TYPE_LISTv2.clear();
        MEAS_TYPE_LISTv2.append(ui->comboBox_meastype0v2_ND);
        MEAS_TYPE_LISTv2BSD.clear();
        MEAS_TYPE_LISTv2BSD.append(ui->comboBox_meastype0v2_BSD);

        for (mode = 1; mode < 6; mode++)    // Memory v2
        {
            allLineEdit.clear();
            allLineEdit = ui->tabWidgetMODEv2->widget(mode)->findChildren<QLineEdit *>();
            foreach (QLineEdit *le, allLineEdit)
            {
                accessname = le->accessibleName();
                if (accessname != "")
                {
                    if (accessname.contains("SREGXV2010"))
                    {

                    }
                    else if (accessname.contains("SREGXV20"))
                    {
                        accessname.remove("SREGXV20");
                        regname = accessname.split("[").at(1);
                        regname.remove("]");
                        accessname = accessname.split("[").at(0);
                        idx = accessname.toInt(&res, 16);
                        idx = idx - 0x40 + mode_start_addr[mode];
                        accessname = QString("SREGXV21%1").arg(idx,2,16,QChar('0')).toUpper();
                        accessname = QString("%1[%2]").arg(accessname).arg(regname);
                    }
                    else if(accessname.contains("Varcomp_method"))
                    {
                        accessname.insert(14, QString("_MODE%1").arg(mode));
                    }
                    else if(accessname.contains("Vardbg_out_sel"))
                    {
                        accessname.insert(14, QString("_MODE%1").arg(mode));
                    }
                    else if(accessname.contains("SSCALE"))
                    {
                        accessname.insert(11, QString("_MODE%1").arg(mode));
                    }
                }
                le->setAccessibleName(accessname);
            }

            allComboBox.clear();
            allComboBox = ui->tabWidgetMODEv2->widget(mode)->findChildren<QComboBox *>();
            foreach (QComboBox *cb, allComboBox)
            {
                accessname = cb->accessibleName();
                if (accessname != "")
                {
                    if(accessname.contains("Varcomp_method"))
                    {
                        accessname.insert(14, QString("_MODE%1").arg(mode));
                        COMP_METHOD_LISTv2.append(cb);
                    }
                    else if(accessname.contains("Vardbg_out_sel"))
                    {
                        accessname.insert(14, QString("_MODE%1").arg(mode));
                    }
                    else if (accessname.contains("SREGXV20"))
                    {
                        accessname.remove("SREGXV20");
                        regname = accessname.split("[").at(1);
                        regname.remove("]");
                        accessname = accessname.split("[").at(0);
                        idx = accessname.toInt(&res, 16);
                        idx = idx - 0x40 + mode_start_addr[mode];
                        accessname = QString("SREGXV21%1").arg(idx,2,16,QChar('0')).toUpper();
                        accessname = QString("%1[%2]").arg(accessname).arg(regname);
                        if (regname == "meas_type")
                        {
                            MEAS_TYPE_LISTv2.append(cb);
                        }
                        if (regname == "12:8")
                        {
                            MEAS_TYPE_LISTv2BSD.append(cb);
                        }
                    }
                }
                cb->setAccessibleName(accessname);
            }

            allCheckBox.clear();
            allCheckBox = ui->tabWidgetMODEv2->widget(mode)->findChildren<QCheckBox *>();
            foreach (QCheckBox *cbx, allCheckBox)
            {
                accessname = cbx->accessibleName();
                if (accessname != "")
                {
                    if (accessname.contains("SREGXV20"))
                    {
                        accessname.remove("SREGXV20");
                        regname = accessname.split("[").at(1);
                        regname.remove("]");
                        accessname = accessname.split("[").at(0);
                        idx = accessname.toInt(&res, 16);
                        idx = idx - 0x40 + mode_start_addr[mode];
                        accessname = QString("SREGXV21%1").arg(idx,2,16,QChar('0')).toUpper();
                        accessname = QString("%1[%2]").arg(accessname).arg(regname);
                    }
                }
                cbx->setAccessibleName(accessname);
            }

        }
    }

}


void DSI3Slave::InitRegisterMap(int ver)
{
    int i, j, mode;
    int page;
    int adr;
    Register *preg0;

    if (ver == 0)
    {
        for (i = 0; i < 256; i++)
        {
            m_map_sregs[0][0][i]=0;
            m_map_sregs[0][1][i]=0;
        }
        for(i = 0; _REGS_INIT[i] != 0; i++)
        {
            preg0  = new Register;
            preg0->makeSetup(_REGS_INIT[i]);

            page = preg0->extra("page").toInt();
            adr = preg0->extra("adr").toInt(0,16);
            if(preg0->size()!=16) {
                qWarning()<<QString("!!! Error parsing register %1").arg(adr);
                continue;
            }

            preg0->setName(QString("SREGX%1%2").arg(page).arg(adr,2,16,QChar('0')).toUpper());
            SREGX[0].append(preg0);
            //m_map_sregs[page][adr] = i;
            m_map_sregs[0][page][adr] = SREGX[0].size() - 1;

            if (preg0->extra("type") == "Mode")
            {
                for (mode = 1; mode < 6; mode++)
                {
                    preg0  = new Register;
                    preg0->makeSetup(_REGS_INIT[i]);
                    preg0->setExtra("type", "copy");
                    page = 1;
                    adr = preg0->extra("adr").toInt(0,16) + mode_start_addr[mode] - 0x40;

                    if(preg0->size()!=16) {
                        qWarning()<<QString("!!! Error parsing register %1").arg(adr);
                        continue;
                    }

                    preg0->setName(QString("SREGX%1%2").arg(page).arg(adr,2,16,QChar('0')).toUpper());
                    SREGX[0].append(preg0);
                    //m_map_sregs[page][adr] = i;
                    m_map_sregs[0][page][adr] = SREGX[0].size() - 1;
                }
            }
        }
        for (j = 0; j < SREGX[0].size(); j++)
        {
            addDataNode(SREGX[0].at(j));
        }
    }
    else if (ver == 2)
    {
        for (i = 0; i < 256; i++)
        {
            m_map_sregs[1][0][i]=0;
            m_map_sregs[1][1][i]=0;
        }
        for(i = 0; _REGS_INITv2[i] != 0; i++)
        {
            preg0  = new Register;
            preg0->makeSetup(_REGS_INITv2[i]);

            page = preg0->extra("page").toInt();
            adr = preg0->extra("adr").toInt(0,16);
            if(preg0->size()!=16) {
                qWarning()<<QString("!!! Error parsing register %1").arg(adr);
                continue;
            }

            preg0->setName(QString("SREGXV2%1%2").arg(page).arg(adr,2,16,QChar('0')).toUpper());
            SREGX[1].append(preg0);
            //m_map_sregs[page][adr] = i;
            m_map_sregs[1][page][adr] = SREGX[1].size() - 1;

            if (preg0->extra("type") == "Mode")
            {
                for (mode = 1; mode < 6; mode++)
                {
                    preg0  = new Register;
                    preg0->makeSetup(_REGS_INITv2[i]);
                    preg0->setExtra("type", "copy");
                    page = 1;
                    adr = preg0->extra("adr").toInt(0,16) + mode_start_addr[mode] - 0x40;

                    if(preg0->size()!=16) {
                        qWarning()<<QString("!!! Error parsing register %1").arg(adr);
                        continue;
                    }

                    preg0->setName(QString("SREGXV2%1%2").arg(page).arg(adr,2,16,QChar('0')).toUpper());
                    SREGX[1].append(preg0);
                    m_map_sregs[1][page][adr] = SREGX[1].size() - 1;
                }
            }


        }
        for (j = 0; j < SREGX[1].size(); j++)
        {
            addDataNode(SREGX[1].at(j));
        }
    }
}

void DSI3Slave::InitScaleMap()
{
    Register *preg0;
    int i, j, sid, mode;

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 6; j++)
        {
            m_map_sscale[i][j] = 0;
        }
    }

    for(i = 0; _SCALE_INIT[i] != 0; i++)
    {
        preg0  = new Register;
        preg0->makeSetup(_SCALE_INIT[i]);

        sid = preg0->extra("sid").toInt();
        if(preg0->size()!=32) {
            qWarning()<<QString("!!! Error parsing register sid %1").arg(sid);
            continue;
        }
        preg0->setName(QString("SSCALE_SID%1").arg(sid));
        SSCALE.append(preg0);
        m_map_sscale[sid-1][0] = SSCALE.size() - 1;


        for (mode = 1; mode < 6; mode++)
        {
            preg0  = new Register;
            preg0->makeSetup(_SCALE_INIT[i]);

            sid = preg0->extra("sid").toInt();
            if(preg0->size()!=32) {
                qWarning()<<QString("!!! Error parsing register sid %1").arg(sid);
                continue;
            }
            preg0->setName(QString("SSCALE_SID%1_MODE%2").arg(sid).arg(mode));
            SSCALE.append(preg0);
            m_map_sscale[sid-1][mode] = SSCALE.size() - 1;
        }


    }
    for (i = 0; i < SSCALE.size(); i++)
    {
        addDataNode(SSCALE.at(i));
    }
}

void DSI3Slave::CopyMem()
{

    static const quint32 mode_start_address[]={0x40, 0x52, 0x74, 0x96, 0xB8, 0xDA, 0xDA, 0xDA};
    int mode;
    int page;
    int dyn_gain_offset;
    for (int i = 0; i < SREGX[0].size(); i++)
    {
        SREGX[1].at(i)->setValue(0, 15, SREGX[0].at(i)->value(0, 15));
    }

    for (mode = 0; mode < 6; mode++)
    {
        if (mode == 0)
        {
            page = 0;
        }
        else
        {
            page = 1;
        }

        dyn_gain_offset = SREGX[0].at(m_map_sregs[0][page][0x4A - 0x40 + mode_start_address[mode]])->value("[dyn_gain_offset]");
        SREGX[1].at(m_map_sregs[1][page][0x40 - 0x40 + mode_start_address[mode]])->setValue("[dyn_gain_offset_6]", dyn_gain_offset >> 6);
        SREGX[1].at(m_map_sregs[1][page][0x42 - 0x40 + mode_start_address[mode]])->setValue("[dyn_gain_offset_50]", dyn_gain_offset & 0x3F);

        SREGX[1].at(m_map_sregs[1][page][0x48 - 0x40 + mode_start_address[mode]])->setValue("[dbg_rx_ena]", SREGX[0].at(m_map_sregs[0][page][0x4A - 0x40 + mode_start_address[mode]])->value("[dbg_rx_ena]"));
        SREGX[1].at(m_map_sregs[1][page][0x48 - 0x40 + mode_start_address[mode]])->setValue("[dbg_ena]", SREGX[0].at(m_map_sregs[0][page][0x42 - 0x40 + mode_start_address[mode]])->value("[dbg_ena]"));
        SREGX[1].at(m_map_sregs[1][page][0x4A - 0x40 + mode_start_address[mode]])->setValue("[meas_duration]", SREGX[0].at(m_map_sregs[0][page][0x58 - 0x40 + mode_start_address[mode]])->value("[meas_duration]"));
        SREGX[1].at(m_map_sregs[1][page][0x4A - 0x40 + mode_start_address[mode]])->setValue("[meas_type]", SREGX[0].at(m_map_sregs[0][page][0x42 - 0x40 + mode_start_address[mode]])->value("[meas_type]"));
        SREGX[1].at(m_map_sregs[1][page][0x56 - 0x40 + mode_start_address[mode]])->setValue("[indir_cfar_scale]", SREGX[0].at(m_map_sregs[0][page][0x40 - 0x40 + mode_start_address[mode]])->value("[indir_cfar_scale]"));
        SREGX[1].at(m_map_sregs[1][page][0x58 - 0x40 + mode_start_address[mode]])->setValue("[dir_cfar_scale]", SREGX[0].at(m_map_sregs[0][page][0x40 - 0x40 + mode_start_address[mode]])->value("[dir_cfar_scale]"));
        SREGX[1].at(m_map_sregs[1][page][0x58 - 0x40 + mode_start_address[mode]])->setValue("[zif_scale]", SREGX[0].at(m_map_sregs[0][page][0x58 - 0x40 + mode_start_address[mode]])->value("[zif_scale]"));
    }

}

bool DSI3Slave::ReadPageUsingPDCM(int page)
{
    if (SREGX[m_memory].at(m_map_sregs[m_memory][1][0x20])->value("[vsds]") == 1)
    {
        LERROR("VSDS must be 0 for this command. Please set VSDS = 0");
        return false;
    }

    if (page == 0)
    {
        PerformWRITE_RWP(false, 15, 3, 0, 0, 0, 0);
        gp_ASICMaster->GenerateBRCsAndGetData(6, 8, page);
    }
    else if (page == 1)
    {
        PerformWRITE_RWP(false, 15, 3, 1, 0, 0, 0);
        gp_ASICMaster->GenerateBRCsAndGetData(6, 16, page);
    }
    else
    {
        LERROR("Page index out of range");
        return false;
    }



    return true;
}

void DSI3Slave::SetPDCMdata(int sid, int page, QString txt)
{
    if (page == 0)
    {
        if (sid == 1){
            ui->lineEditReadPage0S1->setText(txt);
        }
        else if (sid == 2){
            ui->lineEditReadPage0S2->setText(txt);
        }
        else if (sid == 3){
            ui->lineEditReadPage0S3->setText(txt);
        }
        else if (sid == 4){
            ui->lineEditReadPage0S4->setText(txt);
        }
        else if (sid == 5){
            ui->lineEditReadPage0S5->setText(txt);
        }
        else if (sid == 6){
            ui->lineEditReadPage0S6->setText(txt);
        }
    }
    else if (page == 1)
    {
        if (sid == 1){
            ui->lineEditReadPage1S1->setText(txt);
        }
        else if (sid == 2){
            ui->lineEditReadPage1S2->setText(txt);
        }
        else if (sid == 3){
            ui->lineEditReadPage1S3->setText(txt);
        }
        else if (sid == 4){
            ui->lineEditReadPage1S4->setText(txt);
        }
        else if (sid == 5){
            ui->lineEditReadPage1S5->setText(txt);
        }
        else if (sid == 6){
            ui->lineEditReadPage1S6->setText(txt);
        }
    }
}

bool DSI3Slave::IsAddressSwitches()
{
    if (VarSwitches.value() == 0){
        return false;
    }
    else{
        return true;
    }
}
