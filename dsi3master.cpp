#include "dsi3master.h"
#include "ui_dsi3master.h"
#include "mainapp.h"
#include "uartuartcontroller.h"
#include "dsi3slave.h"

#define LOG_TYPE_UART1_TX   (1)
#define LOG_TYPE_UART1_RX   (2)
#define LOG_TYPE_MOBD       (3)
#define LOG_TYPE_INFO       (4)
#define LOG_TYPE_DSI3_CMD   (5)
#define LOG_TYPE_ERROR      (6)
#define LOG_TYPE_BUTTON     (7)
#define LOG_TYPE_UART2_TX   (8)
#define LOG_TYPE_UART2_RX   (9)
#define LOG_TYPE_GPIO       (10)
#define LOG_TYPE_TIME       (11)
#define LOG_TYPE_DSI3_TX    (12)
#define LOG_TYPE_DSI3_RX    (13)
#define LOG_TYPE_SPI_TX     (14)
#define LOG_TYPE_SPI_RX     (15)
#define LOG_TYPE_SPI_CMD    (16)
#define LOG_TYPE_SSI_TX     (17)
#define LOG_TYPE_SSI_RX     (18)
#define LOG_TYPE_SSI_CMD    (19)

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

DSI3Master *gp_DSI3Master;

DSI3Master::DSI3Master(QWidget *parent) :
    Applet(parent)
{
    ui = new Ui::DSI3Master;
    ui->setupUi(this);
    createCustomUi();
    setWindowIcon(QIcon(":/board/onlogo.ico"));
    setWindowTitle("DSI3 Master");
}

DSI3Master::~DSI3Master()
{
    delete ui;
}

bool DSI3Master::initialize()
{
    MREG00.makeSetup("@register=MREG00 @addr=0 @length=1 @purpose=Status;"
                    "REVID[7:0]#Revision ID, unique identification;"
                    );
    MREG01.makeSetup("@register=MREG01 @addr=1  @length=1 @purpose=Config;"
                    "INT_PDCM_ENA#Generates interrupt after receiving a complete PDCM slot;"
                    "INT_BRC_FRAME_ENA#Generates interrupt after receiving a complete BRC frame;"
                    "INT_BRC_MESS_ENA#Generates interrupt after receiving the programmed number of BRC messages;"
                    "INT_SEND_CRM_ENA#Generates interrupt after sending of CRM message is finished;"
                    "INT_REC_CRM_ENA#Generates interrupt after receiving CRM response is finished;"
                    "INT_HW_ERR_ENA#Generates interrupt after HW_ERROR detected;"
                    "INT_CRC_FAIL_ENA#Generates interrupt in case of UART CRC fail;"
                    "UNUSED;"
                    );
    MREG02.makeSetup("@register=MREG02 @addr=2 @length=3 @purpose=Config;"
                    "BRC_CNT[6:0]#Count of BRC pulses;"
                    "UNUSED;"
                    "DELAY_BEFORE_BRC[15:8]#Delay time before first BRC pulse in multiple of 100us;"
                    "TIME_BETWEEN_BRC[23:16]#Timing between BRC pulses in multiple of 100us;"
                    );
    MREG03.makeSetup("@register=MREG03 @addr=3 @length=3 @purpose=Config;"
                    "BRC_CNT[6:0]#Count of BRC pulses;"
                    "UNUSED;"
                    "DELAY_BEFORE_BRC[15:8]#Delay time before first BRC pulse in multiple of 100us;"
                    "TIME_BETWEEN_BRC[23:16]#Timing between BRC pulses in multiple of 100us;"
                    );
    MREG04.makeSetup("@register=MREG04 @addr=4 @length=1 @purpose=Status;"
                    "SYMBOL_ERROR_CH1#Received symbol(3 chips) cant be decoded;"
                    "SYMBOL_ERROR_CH2#Received symbol(3 chips) cant be decoded;"
                    "FRAME_TIMEOUT_CH1#No slave response detected;"
                    "FRAME_TIMEOUT_CH2#No slave response detected;"
                    "RX_TIMEOUT_CH1#More than 4xidentical chips detected in row;"
                    "RX_TIMEOUT_CH2#More than 4xidentical chips detected in row;"
                    "HW_ERROR#HW_ERROR detected (OR of all HW error bits);"
                    "UNUSED;"
                    );
    MREG05.makeSetup("@register=MREG05 @addr=5 @length=1 @purpose=Config;"
                    "CLR_SYMBOL_ERROR_CH1#Received symbol(3 chips) cant be decoded;"
                    "CLR_SYMBOL_ERROR_CH2#Received symbol(3 chips) cant be decoded;"
                    "CLR_FRAME_TIMEOUT_CH1#No slave response detected;"
                    "CLR_FRAME_TIMEOUT_CH2#No slave response detected;"
                    "CLR_RX_TIMEOUT_CH1#More than 4xidentical chips detected in row;"
                    "CLR_RX_TIMEOUT_CH2#More than 4xidentical chips detected in row;"
                    "CLR_HW_ERROR#HW_ERROR detected (OR of all HW error bits);"
                    "CLR_UNUSED;"
                    );
    MREG06.makeSetup("@register=MREG06 @addr=6 @length=2 @purpose=Status;"
                    "OSCLCK_FAIL#Locking to CLKIN failed;"
                    "UNUSED[15:1];"
                    );
    MREG07.makeSetup("@register=MREG07 @addr=7 @length=2 @purpose=Config;"
                    "CLR_OSCLCK_FAIL#Locking to CLKIN failed;"
                    "CLR_UNUSED[15:1];"
                    );
    MREG08.makeSetup("@register=MREG08 @addr=8 @length=1 @purpose=Status;"
                    "INT_PDCM_DET_CH1#Generates interrupt after receiving a complete PDCM slot;"
                    "INT_BRC_FRAME_DET_CH1#Generates interrupt after receiving a complete BRCA frame;"
                    "INT_BRC_MESS_DET_CH1#Generates interrupt after receiving the programmed number of BRC messages;"
                    "INT_SEND_CRM_DET_CH1#Generates interrupt after sending of CRM message is finished;"
                    "INT_REC_CRM_DET_CH1#Generates interrupt after receiving CRM response is finished;"
                    "INT_HW_ERR_DET_CH1#Generates interrupt after HW_ERROR detected;"
                    "INT_CRC_FAIL_DET_CH1#Generates interrupt in case of UART CRC fail;"
                    "UNUSED;"
                    );
    MREG09.makeSetup("@register=MREG09 @addr=9 @length=1 @purpose=Status;"
                    "INT_PDCM_DET_CH2#Generates interrupt after receiving a complete PDCM slot;"
                    "INT_BRC_FRAME_DET_CH2#Generates interrupt after receiving a complete BRCA frame;"
                    "INT_BRC_MESS_DET_CH2#Generates interrupt after receiving the programmed number of BRC messages;"
                    "INT_SEND_CRM_DET_CH2#Generates interrupt after sending of CRM message is finished;"
                    "INT_REC_CRM_DET_CH2#Generates interrupt after receiving CRM response is finished;"
                    "INT_HW_ERR_DET_CH2#Generates interrupt after HW_ERROR detected;"
                    "INT_CRC_FAIL_DET_CH2#Generates interrupt in case of UART CRC fail;"
                    "UNUSED;"
                    );
    MREG10.makeSetup("@register=MREG10 @addr=10 @length=5 @purpose=Config;"
                    "PDCM_RESP_FRAME_LEN[4:0]#Size of complete PDCM response frame with header and CRC;"
                    "DATA_AMP#0 is 4V, 1 is 3V;"
                    "SCRAMBLER_ENA#Enable scrambler;"
                    "REF#0 is reference to 1/2VBAT, 1 is reference to GND;"
                    "PDCM_RESP_START_TIME[13:8]#Time offset in TCHIPS measured from Master DSI3PAS BRC TX start;"
                    "BUS_DRIVER_ENA#0 is 4V, 1 is 2V;"
                    "UNUSED1;"
                    "NUM_OF_PDCM_SLOTS[19:16]#Number of PDCM slots to be received, 0 is 1 slot, 1 is 2 slots, ...;"
                    "MAX_CRM_RESP_START_TIME[30:20]#Max CRM response start time, measured in TCHIPS from Master CRM TX start;"
                    "UNUSED2;"
                    "CRM_RESP_FRAME_LEN[36:32]#Size fo complete CRM response frame with header and CRC;"
                    "RX_FILTER_ENA[38:37]#Enabling RX filter;"
                    "UNUSED3;"
                    );
    MREG11.makeSetup("@register=MREG11 @addr=11 @length=2 @purpose=Clock;"
                    "CLKIN_FREQ[3:0]#See MOBD_CLOCK_CONFIG.xlsm for details;"
                    "CLKIN_FREQ_DET[7:4]#See MOBD_CLOCK_CONFIG.xlsm for details;"
                    "BAUD_RATE[15:8]#See MOBD_CLOCK_CONFIG.xlsm for details;"
                    );



    MWriteREAD.makeSetup("@register=MWriteREAD;"
                    "@field=CMD[2:0]   @value=0    @readonly=1     @descr=Command READ constant;"
                    "ADDR[7:3]#Address;"
                    "@field=CRC[15:8]      @readonly=1     @descr=CRC8;"
                    );

    MReadREAD.makeSetup("@register=MReadREAD;"
                    "@field=CMD[2:0]   @value=0    @readonly=1     @descr=Command READ constant;"
                    "@field=ADDR[7:3]   @value=0    @readonly=1     @descr=Address;"
                    "@field=CRC[15:8]      @readonly=1     @descr=CRC8;"
                    );

    MWriteWRITE.makeSetup("@register=MWriteWRITE;"
                    "@field=CMD[2:0]   @value=1    @readonly=1     @descr=Command WRITE constant;"
                    "ADDR[7:3]#Address;"
                    "@field=CRC[15:8]      @readonly=1     @descr=CRC8;"
                    );

    MReadWRITE.makeSetup("@register=MReadWRITE;"
                    "@field=CMD[2:0]   @value=1    @readonly=1     @descr=Command WRITE constant;"
                    "@field=ADDR[7:3]   @value=0    @readonly=1     @descr=Address;"
                    "@field=CRC[15:8]      @readonly=1     @descr=CRC8;"
                    );

    MWriteCOMMAND.makeSetup("@register=MWriteCOMMAND;"
                    "@field=CMD[2:0]   @value=2    @readonly=1     @descr=Command COMMAND constant;"
                    "LEN[6:3]#DATA length;"
                    "NONE;"
                    "DATA[103:8]#DATA to be transfered;"
                    "@field=CRC[111:104]     @readonly=1     @descr=CRC8;"
                    );
    MWriteCRM.makeSetup("@register=MWriteCRM;"
                    "@field=CMD[2:0]   @value=3    @readonly=1     @descr=Command CRM constant;"
                    "LEN[4:3]#Address;"
                    "NONE[7:5];"
                    "CRM1[39:8]#DATA to be transfered;"
                    "CRM2[71:40]#DATA to be transfered;"
                    "CRM3[103:72]#DATA to be transfered;"
                    "@field=CRC[111:104]      @readonly=1     @descr=CRC8;"
                    );
    MReadCRM.makeSetup("@register=MReadCRM;"
                    "CRM1[31:0]#received DATA;"
                    "CRM2[63:32]#received DATA;"
                    "CRM3[95:64]#received DATA;"
                    );

    VarStartOfBRC.makeSetup("@variable=VarStartOfBRC" "@value=14000" "@min=0" "@max=20000");

    CHANNEL.makeSetup("@variable=CHANNEL" "@value=0" "@min=0" "@max=1");


    MVar_version.makeSetup("@variable=MVar_version" "@value=0" "@min=0" "@max=11");
    addDataNode(&MVar_version);


    MREGS.append(&MREG00);
    MREGS.append(&MREG01);
    MREGS.append(&MREG02);
    MREGS.append(&MREG03);
    MREGS.append(&MREG04);
    MREGS.append(&MREG05);
    MREGS.append(&MREG06);
    MREGS.append(&MREG07);
    MREGS.append(&MREG08);
    MREGS.append(&MREG09);
    MREGS.append(&MREG10);
    MREGS.append(&MREG11);
    for(int i = 0; i < MREGS.count(); i++){
        addDataNode(MREGS.at(i));
    }
    MWriteREAD["CRC"].setReadOnly(true);
    addDataNode(&MWriteREAD);
    addDataNode(&MReadREAD);
    addDataNode(&MWriteWRITE);
    addDataNode(&MReadWRITE);
    addDataNode(&MWriteCOMMAND);
    addDataNode(&MWriteCRM);
    addDataNode(&MReadCRM);
    addDataNode(&VarStartOfBRC);
    addDataNode(&CHANNEL);

    return true;
}

void DSI3Master::createCustomUi()
{
    //    void signal_RegisterChanged(void);
    connect(this, SIGNAL(signal_RegisterChanged(QString)), this, SLOT(slot_UpdateLabels()));
    //connect(ui->lineEditWriteData,SIGNAL(textEdited(QString)),this,SLOT(slot_WriteData_changed()));
    //connect(ui->lineEditWriteData,SIGNAL(textChanged(QString)),this,SLOT(slot_WriteData_changed()));
    ui->tabWidget->setCurrentIndex(0);
}


/****************************************************************************
 *                              FUNCTIONALITY
*****************************************************************************/


// updates all RELAYS and GPIOs
bool DSI3Master::updateHardware(Direction direction, const QString &item)
{

    return false;
}

QVariant DSI3Master::FUNCTION_LIST()
{        

        FUNCTION("Update", "This function updates hardware"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            return updateHardware(HARDWARE_GET,"*");
        }
        FUNCTION("WriteAllConfig", "This function writes data into all master's config registers"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            Log(LOG_TYPE_BUTTON, "DSI3 Master - Write ALL Config regs");
            return WriteAllConfig();
        }
        FUNCTION("ReadAllConfig", "This function reads data from all master's config register"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            Log(LOG_TYPE_BUTTON, "DSI3 Master - Read ALL Config regs");
            return ReadAllConfig();
        }
        FUNCTION("ReadAllStatus", "This function reads data from all master's status register"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            Log(LOG_TYPE_BUTTON, "DSI3 Master - Write ALL Status and Flags registers");
            return ReadAllStatus();
        }
        FUNCTION("ReadREG11", "This function reads register 11 - clock and baudrate register"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            Log(LOG_TYPE_BUTTON, "DSI3 Master - Read REG11");
            return ReadClockBaudrate();
        }
        FUNCTION("WriteREG11", "This function writes register 11 - clock and baudrate register"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            Log(LOG_TYPE_BUTTON, "DSI3 Master - Write REG11");
            return WriteClockBaudrate();
        }
        FUNCTION("PerformREAD", "This function perform READ command with values in register MWriteREAD"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            Log(LOG_TYPE_BUTTON, "DSI3 Master - Perform READ");
            return PerformREAD();
        }
        FUNCTION("PerformWRITE", "This function perform WRITE command with values in register MWriteWRITE"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            Log(LOG_TYPE_BUTTON, "DSI3 Master - Perform WRITE");
            return PerformWRITE();
        }
        FUNCTION("PerformCOMMAND", "This function perform DSI3PAS COMMAND"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            Log(LOG_TYPE_BUTTON, "DSI3 Master - Perform COMMAND");
            return PerformCOMMAND();
        }
        FUNCTION("PerformCRM", "This function perform DSI3PAS CRM"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            Log(LOG_TYPE_BUTTON, "DSI3 Master - Perform CRM");
            return PerformCOMMANDCRM();
        }
        FUNCTION("QuickConfiguration", "Quick configuration of Master and Slaves"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            Log(LOG_TYPE_BUTTON, "DSI3 Master - Quick Configuration");
            return QuickConfiguration();
        }
    return false;
}


void DSI3Master::setupBoard()
{
}


void DSI3Master::suspend()
{       
}

void DSI3Master::resume()
{
    if (gp_Settings->mVersion == VERSION_UPID)
    {
        ui->tabWidget->setEnabled(false);
    }
    else
    {
        ui->tabWidget->setEnabled(true);
    }
    setupBoard();
}
//****************************************************************************
// @function name: slot_UpdateLabels()
// @description: Update all labels in GUI with recalculated values
//****************************************************************************
void DSI3Master::slot_UpdateLabels()
{
    ui->labelDELAY_BEFORE_BRC_CH1->setText(QString("%1 us").arg(100*(int)MREG02["DELAY_BEFORE_BRC"]));
    ui->labelTIMING_BETWEEN_BRC_PULSES_CH1->setText(QString("%1 us").arg(100*(int)MREG02["TIME_BETWEEN_BRC"]));
    ui->labelDELAY_BEFORE_BRC_CH2->setText(QString("%1 us").arg(100*(int)MREG03["DELAY_BEFORE_BRC"]));
    ui->labelTIMING_BETWEEN_BRC_PULSES_CH2->setText(QString("%1 us").arg(100*(int)MREG03["TIME_BETWEEN_BRC"]));
    ui->labelPDCM_RESP_START_TIME->setText(QString("%1 us").arg(3*(int)MREG10["PDCM_RESP_START_TIME"]));
    ui->labelPDCM_RESP_FRAME_LEN->setText(QString("%1 bytes").arg((int)MREG10["PDCM_RESP_FRAME_LEN"]));
    ui->labelMAX_CRM_RESP_START_TIME->setText(QString("%1 us").arg(3*(int)MREG10["MAX_CRM_RESP_START_TIME"]));
    //ui->labelCRM_RESP_FRAME_LEN->setText(QString("%1 us").arg(3*(int)MREG04["CRM_RESP_FRAME_LEN"]));
    ui->labelCRM_RESP_FRAME_LEN->setText(QString("-"));
    ui->labelDATA_AMP->setText(QString("%1 Vpp").arg(4-(int)MREG10["DATA_AMP"]));

    if((int)MREG10["NUM_OF_PDCM_SLOTS"] == 0)
        ui->labelNUM_OF_PDCM_SLOTS->setText(QString("1 slot"));
    else if((int)MREG10["NUM_OF_PDCM_SLOTS"] < 10)
        ui->labelNUM_OF_PDCM_SLOTS->setText(QString("%1 slots").arg(1 + (int)MREG10["NUM_OF_PDCM_SLOTS"]));
    else
        ui->labelNUM_OF_PDCM_SLOTS->setText("not Allowed");

    if ((int)MREG10["REF"] == 0)
        ui->labelREF->setText(QString("Vbat/2"));
    else
        ui->labelREF->setText(QString("GND"));

    if ((int)MREG10["RX_FILTER_ENA"] == 0)
        ui->labelRX_FILTER_ENA->setText(QString("Disabled"));
    else if ((int)MREG10["RX_FILTER_ENA"] == 1)
        ui->labelRX_FILTER_ENA->setText(QString("Enabled"));
    else
        ui->labelRX_FILTER_ENA->setText(QString("Reserved"));


}


//****************************************************************************
// @function name: WriteAllConfig()
// @description: Write all config registers
//****************************************************************************
bool DSI3Master::WriteAllConfig()
{
    QStringList regs;
    regs.append("MREG02");
    regs.append("MREG03");
    regs.append("MREG10");
    regs.append("MREG07");
    regs.append("MREG05");
    regs.append("MREG01");
    return PerformWRITE(regs);
}

//****************************************************************************
// @function name: ReadAllConfig()
// @description: Read all config registers
//****************************************************************************
bool DSI3Master::ReadAllConfig()
{
    QStringList regs;
    regs.append("MREG02");
    regs.append("MREG03");
    regs.append("MREG10");
    regs.append("MREG07");
    regs.append("MREG05");
    regs.append("MREG01");
    return PerformREAD(regs);
}

//****************************************************************************
// @function name: ReadAllStatus()
// @description: Read all status registers
//****************************************************************************
bool DSI3Master::ReadAllStatus(void)
{
    QStringList regs;
    regs.append("MREG06");
    regs.append("MREG04");
    regs.append("MREG08");
    regs.append("MREG09");
    regs.append("MREG00");
    return PerformREAD(regs);
}

//****************************************************************************
// @function name: ReadClockBaudrate()
// @description: Read register 11
//****************************************************************************
bool DSI3Master::ReadClockBaudrate(void)
{
    QStringList regs;
    regs.append("MREG11");
    return PerformREAD(regs);
}

//****************************************************************************
// @function name: WriteClockBaudrate()
// @description: Write register 11
//****************************************************************************
bool DSI3Master::WriteClockBaudrate(void)
{
    QByteArray txcommand;
    QByteArray txdata;
    int i, j, k;
    bool result = false;

    txcommand = MREG11.toByteArray();
    txcommand.append(gp_UartUartController->CalculateMOBDCRC(txcommand));

    Log(LOG_TYPE_MOBD, QString("WRITE command - baudrate, addr=11"));

    for (i = 0; i < txcommand.length(); i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (((txcommand.at(i) >> j)&0x01) == 0x01)
            {
                txdata.append((char)0xFF);
            }
            else
            {
                txdata.append((char)0x00);
            }
        }
    }
    gp_UartUartController->SetMDSEL(0);

    QByteArray temp;
    result = gp_UartUartController->WriteReadUART(false, txdata, &temp, true, false, RX1_BUF, 0, 0, false);

    gp_UartUartController->SetMDSEL(1);

    return result;
}

//***************************************************************************
// @function name: findREGX()
// @param:
//      const QString &regname
// @description: finds register in list by its name
//***************************************************************************/
Register *DSI3Master::findREG(const QString &regname)
{
    foreach(Register *preg, MREGS){
        if(preg->name() == regname ){
            return preg;
        }
    }
    return NULL;
}

//****************************************************************************
// @function name: PerformREAD()
// @description: Perform READ command
//****************************************************************************
bool DSI3Master::PerformREAD(void)
{
    QByteArray txcommand;
    QByteArray rxdata;
    bool result;
    int addr = (int)MWriteREAD["ADDR"];
    int length = ui->lineEditPerformReadLength->text().toInt();

    Log(LOG_TYPE_MOBD, QString("READ from MASTER Reg %1").arg(addr));

    txcommand.append((char)addr);
    txcommand.append(gp_UartUartController->CalculateMOBDCRC(txcommand));
    MWriteREAD["CRC"] = txcommand.at(1);
    result = gp_UartUartController->WriteReadUART(false, txcommand, &rxdata, true, true, RX1_BUF, length + 2, 500, true);

    if (result)
    {
        MReadREAD["ADDR"] = rxdata.at(0);
        MReadREAD["CRC"] = rxdata.at(rxdata.length() - 1);
        ui->lineEditPerformReadData->setText(rxdata.mid(1, length).toHex());
    }
    else
    {
        Log(LOG_TYPE_ERROR, "No response in PerformREAD");
    }
    slot_updateAllControls();
    return false;
}

//****************************************************************************
// @function name: PerformREAD()
// @description: Perform READ command
//****************************************************************************
bool DSI3Master::PerformREAD(QStringList regnames)
{
    QByteArray txcommand;
    QByteArray rxcommand;
    QByteArray onecommand;
    Register *preg;
    int addr;
    int length;
    int read_count = 0;
    int i, j, k;
    bool result = false;

    //gp_UartUartController->SetMDSEL(101);

    for (i = 0; i < regnames.length(); i++)     // CREATE COMMAND
    {
        preg = findREG(regnames.at(i));
        if (preg != NULL)
        {
            addr = preg->extra("addr").toInt();
            length = preg->extra("length").toInt();
            onecommand.clear();
            onecommand.append(addr&0x1F);
            onecommand.append(gp_UartUartController->CalculateMOBDCRC(onecommand));
            for (j = 0; j < (length + 3); j++)
            {
                onecommand.append((char)0xFF);
            }
            read_count = read_count + length + 2;
            txcommand.append(onecommand);
            Log(LOG_TYPE_MOBD, QString("READ command, addr=%1").arg(addr));
        }
        else
        {
            Log(LOG_TYPE_ERROR, "Error in parsing regnames, function PerformREAD");
        }
    }

    result = gp_UartUartController->WriteReadUART(false, txcommand, &rxcommand, true, true, RX1_BUF, read_count, 500, true);

    if (result)
    {
        read_count = 0;
        for (i = 0; i < regnames.length(); i++)     // PARSE RESPONSE
        {
            preg = findREG(regnames.at(i));
            if (preg != NULL)
            {
                addr = preg->extra("addr").toInt();
                length = preg->extra("length").toInt();
                onecommand.clear();
                onecommand = rxcommand.mid(read_count, length + 2);

                if (onecommand.size() == (length + 2))
                {
                    if ((onecommand.at(0) == (addr&0x1F)) && (onecommand.at(length+1) == gp_UartUartController->CalculateMOBDCRC(onecommand.mid(0, length+1))))
                    {
                        preg->fromByteArray(onecommand.mid(1, length));
                    }
                    else
                    {
                        Log(LOG_TYPE_ERROR, "Error in received data (command byte or crc), function PerformREAD");
                        return false;
                    }
                }
                else
                {
                    Log(LOG_TYPE_ERROR, "Error in received data, function PerformREAD");
                    return false;
                }
                read_count = read_count + length + 2;
            }
        }
    }
    else
    {
        Log(LOG_TYPE_ERROR, "No response, function PerformREAD");
        return false;
    }

    slot_updateAllControls();
    return result;
}

//****************************************************************************
// @function name: PerformWRITE()
// @description: Perform WRITE command
//****************************************************************************
bool DSI3Master::PerformWRITE()
{
    QByteArray txcommand;
    QByteArray rxdata;
    bool result;
    int addr = (int)MWriteWRITE["ADDR"];
    int length;

    Log(LOG_TYPE_MOBD, QString("WRITE to MASTER Reg %1").arg(addr));

    txcommand.append((char)((addr & 0x1F) + 0x20));
    txcommand.append(QByteArray::fromHex(ui->lineEditMWriteWRITEDATA->text().toLatin1().replace(" ","")));
    txcommand.append(gp_UartUartController->CalculateMOBDCRC(txcommand));
    length = txcommand.length() - 2;
    MWriteWRITE["CRC"] = txcommand.at(txcommand.length() - 1);

    result = gp_UartUartController->WriteReadUART(false, txcommand, &rxdata, true, true, RX1_BUF, length + 2, 500, true);

    if (result)
    {
        MReadWRITE["ADDR"] = rxdata.at(0);
        MReadWRITE["CRC"] = rxdata.at(rxdata.length() - 1);
        ui->lineEditMReadWRITEDATA->setText(rxdata.mid(1, length).toHex());
    }
    else
    {
        Log(LOG_TYPE_ERROR, "No response in PerformWRITE");
    }
    slot_updateAllControls();
    return false;
}

//****************************************************************************
// @function name: PerformWRITE()
// @description: Perform WRITE command
//****************************************************************************
bool DSI3Master::PerformWRITE(QStringList regnames)
{
    QByteArray txcommand;
    QByteArray rxcommand;
    QByteArray onecommand;
    Register *preg;
    int addr;
    int length;
    int read_count = 0;
    int i, j, k;
    bool result = false;

    //gp_UartUartController->SetMDSEL(101);

    for (i = 0; i < regnames.length(); i++)     // CREATE COMMAND
    {
        preg = findREG(regnames.at(i));
        if (preg != NULL)
        {
            addr = preg->extra("addr").toInt();
            length = preg->extra("length").toInt();
            onecommand.clear();
            onecommand.append((addr&0x1F) + (0x20));
            onecommand.append(preg->toByteArray());
            onecommand.append(gp_UartUartController->CalculateMOBDCRC(onecommand));
            for (j = 0; j < (length + 3); j++)
            {
                onecommand.append((char)0xFF);
            }
            read_count = read_count + length + 2;
            txcommand.append(onecommand);
            Log(LOG_TYPE_MOBD, QString("WRITE command, addr=%1").arg(addr));
        }
        else
        {
            Log(LOG_TYPE_ERROR, "Error in parsing regnames, function PerformWRITE");
        }
    }

    result = gp_UartUartController->WriteReadUART(false, txcommand, &rxcommand, true, true, RX1_BUF, read_count, 500, true);

    if (result)
    {
        read_count = 0;
        for (i = 0; i < regnames.length(); i++)     // PARSE RESPONSE
        {
            preg = findREG(regnames.at(i));
            if (preg != NULL)
            {
                addr = preg->extra("addr").toInt();
                length = preg->extra("length").toInt();
                onecommand.clear();
                onecommand = rxcommand.mid(read_count, length + 2);

                if (onecommand.size() == (length + 2))
                {
                    if ((onecommand.at(0) == ((addr&0x1F)+0x20)) && (onecommand.at(length+1) == gp_UartUartController->CalculateMOBDCRC(onecommand.mid(0, length+1))))
                    {
                        /*if (onecommand.mid(1, length) != preg->toByteArray())
                        {
                            Log(LOG_TYPE_ERROR, "Received value of register is not as expected, function PerformWRITE");
                            return false;
                        }*/
                    }
                    else
                    {
                        Log(LOG_TYPE_ERROR, "Error in received data (command byte or crc), function PerformWRITE");
                        return false;
                    }
                }
                else
                {
                    Log(LOG_TYPE_ERROR, "Error in received data, function PerformWRITE");
                    return false;
                }
                read_count = read_count + length + 2;
            }
        }
    }
    else
    {
        Log(LOG_TYPE_ERROR, "No response, function PerformWRITE");
        return false;
    }

    slot_updateAllControls();
    return result;
}

//****************************************************************************
// @function name: PerformCOMMAND()
// @description: Perform DSI3PAS COMMAND
//****************************************************************************
bool DSI3Master::PerformCOMMAND()
{
    QByteArray txcommand;
    QByteArray rxdata;
    bool result;
    int length = (int)MWriteCOMMAND["LEN"];

    Log(LOG_TYPE_MOBD, QString("DSI3PAS COMMAND"));

    txcommand.append((char)((length & 0x1F) + 0x40));
    txcommand.append(QByteArray::fromHex(ui->lineEditMWriteCOMMANDDATA->text().toLatin1().replace(" ","")));
    txcommand.append(gp_UartUartController->CalculateMOBDCRC(txcommand));

    if (length != (txcommand.length() - 2))
    {
        Log(LOG_TYPE_ERROR, "Wrong data length in Perform COMMAND");
        return false;
    }

    MWriteCOMMAND["CRC"] = txcommand.at(txcommand.length() - 1);

    result = gp_UartUartController->WriteReadUART(false, txcommand, &rxdata, true, false, GET_REG("CHANNEL").toInt() & 0x01, 0, 500, true);

    slot_updateAllControls();
    return result;
}

//****************************************************************************
// @function name: PerformCOMMAND()
// @description: Perform DSI3PAS COMMAND
//****************************************************************************
bool DSI3Master::PerformCOMMAND(QByteArray txdata, QByteArray *rxdata, int read_count)
{
    bool result = false;

    Log(LOG_TYPE_MOBD, QString("DSI3PAS COMMAND command"));
    int length = txdata.length();

    if ((length > 0) && (length < 16))
    {
        Log(LOG_TYPE_DSI3_TX, txdata.toHex());
        if ((read_count > 1) || (read_count < 0))
        {
            txdata.prepend((char)(0x50 | (length)));
        }
        else
        {
            txdata.prepend((char)(0x40 | (length)));
        }
        txdata.append(gp_UartUartController->CalculateMOBDCRC(txdata));     //crc

        result = gp_UartUartController->WriteReadUART(false, txdata, rxdata, true, read_count>0, GET_REG("CHANNEL").toInt() & 0x01, read_count, 500, true);
        if (rxdata->length() > 1)
        {
            //Log(LOG_TYPE_DSI3_RX, rxdata->toHex());
            Log(LOG_TYPE_DSI3_RX, rxdata->mid(1, rxdata->length() - 1).toHex());
        }

    }
    else if (length == 0)
    {
        result = gp_UartUartController->WriteReadUART(false, txdata, rxdata, false, read_count>0, GET_REG("CHANNEL").toInt() & 0x01, read_count, 500, true);
    }
    else
    {
        Log(LOG_TYPE_ERROR, "PerformCOMMAND, not enough data or wrong data LENGTH");
    }

    slot_updateAllControls();
    return result;
}

//****************************************************************************
// @function name: PerformCRM()
// @description: Perform DSI3PAS CRM
//****************************************************************************
bool DSI3Master::PerformCRM(QByteArray txdata, QByteArray *rxdata, int read_count)
{
    bool result = false;
    QByteArray txcommand;
    QString cmd;

    Log(LOG_TYPE_MOBD, QString("DSI3PAS CRM command"));

    int length = txdata.length();

    if ((length == 4) || (length == 8) || (length == 12))
    {
        Log(LOG_TYPE_DSI3_TX, txdata.toHex());
        txcommand.append((char)(((length / 4) & 0x1F) + 0x60));
        txcommand.append(txdata);
        txcommand.append(gp_UartUartController->CalculateMOBDCRC(txcommand));
        result = gp_UartUartController->WriteReadUART(false, txcommand, rxdata, true, read_count>0, GET_REG("CHANNEL").toInt() & 0x01, read_count, 500, true);
        Log(LOG_TYPE_DSI3_RX, rxdata->toHex());
    }
    else
    {
        Log(LOG_TYPE_ERROR, "PerformCRM, not enough data or wrong data LENGTH");
    }
    slot_updateAllControls();
    return result;
}


//****************************************************************************
// @function name: PerformCOMMANDCRM()
// @description: Perform DSI3PAS CRM
//****************************************************************************
bool DSI3Master::PerformCOMMANDCRM()
{
    QByteArray txcommand;
    QByteArray rxdata;
    bool result;
    int length = (int)MWriteCRM["LEN"];

    Log(LOG_TYPE_MOBD, QString("DSI3PAS CRM"));

    txcommand.append((char)((length & 0x1F) + 0x60));
    txcommand.append(QByteArray::fromHex(ui->lineEditMWriteCRMDATA->text().toLatin1().replace(" ","")));
    txcommand.append(gp_UartUartController->CalculateMOBDCRC(txcommand));

    if ((4*length) != (txcommand.length() - 2))
    {
        Log(LOG_TYPE_ERROR, "Wrong data length in Perform COMMAND");
        return false;
    }

    MWriteCRM["CRC"] = txcommand.at(txcommand.length() - 1);

    result = gp_UartUartController->WriteReadUART(false, txcommand, &rxdata, true, true, GET_REG("CHANNEL").toInt() & 0x01, 4*length, 500, true);

    if (result)
    {
        MReadCRM["LEN"] = (rxdata.at(0)>>5)&0x07;
        MReadCRM["CRC"] = rxdata.at(rxdata.length() - 1);
        ui->lineEditMReadCRMDATA->setText(rxdata.mid(0, 4*length).toHex());
    }
    else
    {
        Log(LOG_TYPE_ERROR, "No response in PerformCRM");
    }

    slot_updateAllControls();
    return result;
}

//***************************************************************************
// @function name: CalculateMasterCRC()
// @param:
//      QByteArray data
// @description: Calculate and return CRC from input data
//***************************************************************************/
char DSI3Master::CalculateMasterCRC(QByteArray data)
{
    int i;
    const char generator = 0x9B;    // polynom
    char crc = 0xFF;                // start with 0xFF
    //data.prepend((char)0x00);

    for (i = 0; i < data.length(); i++)
    {
        crc ^= data.at(i); // XOR-in the next input byte

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


void DSI3Master::Log(int type, QString message)
{
    static QTime time = QTime::currentTime();
    const char LOG_TIME_FORMAT[]="hh:mm:ss.zzz";
    int color=-1000;
    int time_diff=time.msecsTo(QTime::currentTime());
    QString text;
    QString unit = "ms";
    if(time_diff>1000) {time_diff/=1000;unit="s";}

    if(type == LOG_TYPE_UART1_TX)
    {
        color =LOG_COLOR(Qt::black);

        text = QString("UART TX(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(QString(message).replace("\n","&lt;cr&gt; "));
    }
    else if(type == LOG_TYPE_UART1_RX)
    {
        color =LOG_COLOR(Qt::black);
        text = QString("UART RX(+%1%2):%3")
                .arg(time_diff)
                .arg(unit)
                .arg(QString(message).replace("\n","&lt;cr&gt; "));
    }
    else if(type == LOG_TYPE_UART2_TX)
    {
        color =LOG_COLOR(Qt::black);

        text = QString("UART+ESC TX(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(QString(message).replace("\n","&lt;cr&gt; "));
    }
    else if(type == LOG_TYPE_UART2_RX)
    {
        color =LOG_COLOR(Qt::black);
        text = QString("UART+ESC RX(+%1%2):%3")
                .arg(time_diff)
                .arg(unit)
                .arg(QString(message).replace("\n","&lt;cr&gt; "));
    }
    else if(type == LOG_TYPE_MOBD)
    {
        color = LOG_COLOR(Qt::blue);
        text = QString("MASTER CMD(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(message);
    }
    else if(type == LOG_TYPE_DSI3_CMD)
    {
        color = LOG_COLOR(Qt::darkGreen);
        text = QString("SLAVE CMD(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(message);
    }
    else if(type == LOG_TYPE_INFO)
    {
        color = LOG_COLOR(Qt::darkGray);
        text = QString("INF(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(message);
    }
    else if(type == LOG_TYPE_ERROR)
    {
        color = LOG_COLOR(Qt::red);
        text = QString("ERROR(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(message);
    }
    else if(type == LOG_TYPE_BUTTON)
    {
        emit signal_log(LOG_COLOR(Qt::black), "");
        color = LOG_COLOR(Qt::darkYellow);
        text = QString("BUTTON(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(message);
    }
    else if(type == LOG_TYPE_GPIO)
    {
        color = LOG_COLOR(Qt::darkGray);
        text = QString("GPIO(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(message);
    }
    else if(type == LOG_TYPE_TIME)
    {
        return;
        color = LOG_COLOR(Qt::darkCyan);
        text = QString("TIME(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(message);
    }
    else if(type == LOG_TYPE_DSI3_TX)
    {
        color =LOG_COLOR(Qt::darkCyan);
        text = QString("DSI3 CMD(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(QString(message).replace("\n","&lt;cr&gt; "));
    }
    else if(type == LOG_TYPE_DSI3_RX)
    {
        color =LOG_COLOR(Qt::darkCyan);
        text = QString("DSI3 RSP(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(QString(message).replace("\n","&lt;cr&gt; "));
    }
    else if(type == LOG_TYPE_SPI_TX)
    {
        color =LOG_COLOR(Qt::black);

        text = QString("SPI TX(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(QString(message).replace("\n","&lt;cr&gt; "));
    }
    else if(type == LOG_TYPE_SPI_RX)
    {
        color =LOG_COLOR(Qt::black);
        text = QString("SPI RX(+%1%2):%3")
                .arg(time_diff)
                .arg(unit)
                .arg(QString(message).replace("\n","&lt;cr&gt; "));
    }
    else if(type == LOG_TYPE_SPI_CMD)
    {
        color = LOG_COLOR(Qt::darkMagenta);
        text = QString("SPI CMD(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(message);
    }
    else if(type == LOG_TYPE_SSI_TX)
    {
        color =LOG_COLOR(Qt::black);

        text = QString("SSI TX(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(QString(message).replace("\n","&lt;cr&gt; "));
    }
    else if(type == LOG_TYPE_SSI_RX)
    {
        color =LOG_COLOR(Qt::black);
        text = QString("SSI RX(+%1%2):%3")
                .arg(time_diff)
                .arg(unit)
                .arg(QString(message).replace("\n","&lt;cr&gt; "));
    }
    else if(type == LOG_TYPE_SSI_CMD)
    {
        color = LOG_COLOR(Qt::darkMagenta);
        text = QString("SSI CMD(%1):%2")
                .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                .arg(message);
    }
    emit signal_log(color,text);
    time = QTime::currentTime();
}

bool DSI3Master::QuickConfiguration()
{
    bool result = false;
    bool connected = false;

    result = WriteClockBaudrate();

    if (result == false){
        Log(LOG_TYPE_ERROR, "Error during Write Reg11");
        return false;}

    result = WriteAllConfig();

    if (result == false){
        Log(LOG_TYPE_ERROR, "Error during Write Reg11");
        return false;}

    result = ReadAllStatus();

    if (result == false){
        Log(LOG_TYPE_ERROR, "Error during Write Reg11");
        return false;}

    for (int ch = 0; ch < 2; ch++)
    {
        gp_DSI3Master->CHANNEL.setValue(ch);
        connected = false;

        if (MVar_version.value() == VERSION_RDUS)
        {
            if (gp_DSI3Slave->IsAddressSwitches())
            {
                gp_DSI3Slave->AddressSWITCHES();
            }
            else
            {
                gp_DSI3Slave->AddressDISCOVERY();
            }

            for (int sid = 1; sid < 7; sid++)
            {
                if (gp_DSI3Slave->m_slave_connected[ch][sid-1] == true)
                {
                    connected = true;
                }
            }

        }
        else if ((MVar_version.value() == VERSION_RDUSDMX) || (MVar_version.value() == VERSION_RDUS2DMX) || (MVar_version.value() == VERSION_MOBEDMX) || \
                 (MVar_version.value() == VERSION_RDUS3DCH) || (MVar_version.value() == VERSION_RDUSBSD) || (MVar_version.value() == VERSION_RDUS3DMX) || (MVar_version.value() == VERSION_MOBE2DMX))
        {
            if (gp_DSI3Slave->IsAddressSwitches())
            {
                gp_DSI3Slave->AddressSWITCHES();
            }
            else
            {
                gp_DSI3Slave->AddressDEMUXES();
            }

            for (int sid = 1; sid < 7; sid++)
            {
                if (gp_DSI3Slave->m_slave_connected[ch][sid-1] == true)
                {
                    connected = true;
                }
            }
        }
        else
        {
            gp_DSI3Slave->CheckSlaves();
            for (int sid = 1; sid < 7; sid++)
            {
                if (gp_DSI3Slave->m_slave_connected[ch][sid-1] == true)
                {
                    connected = true;
                }
            }

            if (connected == false)
            {
                if (gp_DSI3Slave->IsAddressSwitches())
                {
                    gp_DSI3Slave->AddressSWITCHES();
                }
                else
                {
                    gp_DSI3Slave->AddressDISCOVERY();
                }
                for (int sid = 1; sid < 7; sid++)
                {
                    if (gp_DSI3Slave->m_slave_connected[ch][sid-1] == true)
                    {
                        connected = true;
                    }
                }
            }
        }

        if (connected)
        {
            result = gp_DSI3Slave->WriteDSI3RegsAll();

            if (result == false){
                Log(LOG_TYPE_ERROR, "Error during Write Reg11");
                return false;}

            result = gp_DSI3Slave->WriteSlaveRegs("Mode");

            if (result == false){
                Log(LOG_TYPE_ERROR, "Error during Write Reg11");
                return false;}

            result = gp_DSI3Slave->WriteSlaveRegs("Common");

            if (result == false){
                Log(LOG_TYPE_ERROR, "Error during Write Common");
                return false;}

            result = gp_DSI3Slave->Synchronize();

            if (result == false){
                Log(LOG_TYPE_ERROR, "Error during Synchronize");
                return false;}

            result = gp_DSI3Slave->ClearSafetyFlags(true);

            if (result == false){
                Log(LOG_TYPE_ERROR, "Error during Clearing safety flags");
                return false;}
        }
    }
    gp_DSI3Master->CHANNEL.setValue(0);

    return true;
}

void DSI3Master::Ver_setItemText(int index, QString text)
{
    ui->comboBoxVersionM->setItemText(index, text);
}
