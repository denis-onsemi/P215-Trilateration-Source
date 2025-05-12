#include "uartuartcontroller.h"
#include "ui_uartuartcontroller.h"
#include "mainapp.h"
#include "dsi3master.h"
#include "dsi3slave.h"

#define LOG_TYPE_UART1_TX    (1)
#define LOG_TYPE_UART1_RX    (2)
#define LOG_TYPE_INFO       (4)
#define LOG_TYPE_ERROR      (6)
#define LOG_TYPE_BUTTON     (7)
#define LOG_TYPE_UART2_TX   (8)
#define LOG_TYPE_UART2_RX   (9)
#define LOG_TYPE_GPIO       (10)

UartUartController *gp_UartUartController;


UartUartController::UartUartController(QWidget *parent) :
    Applet(parent)
{
    ui = new Ui::UartUartController;
    ui->setupUi(this);
    createCustomUi();
    setWindowIcon(QIcon(":/board/onlogo.ico"));
    setWindowTitle("UART-UART Controller");
}

UartUartController::~UartUartController()
{
    delete ui;
}

bool UartUartController::initialize()
{
    MReadNINT.makeSetup("@register=MReadNINT;"
                    "NINT1#Interrupt NINT1;"
                    "NINT2#Interrupt NINT2;"
                    "ZEROS[3:2]#zeros;"
                    "CONTROL_NIBBLE[7:4]#Control nibble is 7;"
                    );
    addDataNode(&MReadNINT);

    UREG00.makeSetup("@register=UREG00 @addr=0;"
                     "FPGAversion[7:0]#FPGA version (8bit number);"
                    );
    addDataNode(&UREG00);

    UREG01.makeSetup("@register=UREG01 @addr=1;"
                    "UART_INTERNAL=0#0 - External UART interface, 1 - PC <-> UART-UART bridge;"
                    "UNUSED[3:1]#unused;"
                    "PDM_SEL#0 - UART1 on PDM7/8,1 - UART2 on PDM7/8;"
                    "UNUSED2[6:5]#unused;"
                    "UART_BRIDGE=0#0 - UART2UART bridge to DSI3 master, 1 - UART2UART bridge to J_UART;"
                    );
    addDataNode(&UREG01);

    UREGANY.makeSetup("@register=UREGANY @addr=255;"
                    "DATA[7:0]#ANY register data;"
                    "ADDR[15:8]#ANY register address;"
                    );
    addDataNode(&UREGANY);

    addDataNode(&gp_DSI3Master->MREG00);

    return true;
}

void UartUartController::createCustomUi()
{
    //connect(ui->lineEditWriteData,SIGNAL(textEdited(QString)),this,SLOT(slot_WriteData_changed()));
    //connect(ui->lineEditWriteData,SIGNAL(textChanged(QString)),this,SLOT(slot_WriteData_changed()));
}


/****************************************************************************
 *                              FUNCTIONALITY
*****************************************************************************/


// updates all RELAYS and GPIOs
bool UartUartController::updateHardware(Direction direction, const QString &item)
{
    return false;
}

QVariant UartUartController::FUNCTION_LIST()
{        
        FUNCTION("Update", "This function updates hardware"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            return updateHardware(HARDWARE_GET,"*");
        }

        FUNCTION("SetMDSELhigh", "This function sets MDSEL pin high"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Set MDSEL high");
            return SetMDSEL(1);
        }

        FUNCTION("SetMDSELlow", "This function sets MDSEL pin low"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Set MDSEL low");
            return SetMDSEL(0);
        }

        FUNCTION("SetMDSELpulse", "This function creates negative pulse at MDSEL pin"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Set MDSEL low and high");
            return SetMDSEL(101);
        }

        FUNCTION("GetNINTx", "This function reads interrupts"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Get NINTx values");
            return GetNINTx();
        }

        FUNCTION("SetBaudrate", "This function sets baudrate"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Set baudrate");
            return SetBaudrate();
        }

        FUNCTION("ReadReg00", "This function reads data from Reg00"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Read Reg00");
            return ReadBridgeReg("UREG00");
        }

        FUNCTION("WriteReg01", "This function writes data to Reg01"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Write Reg01");
            return WriteBridgeReg("UREG01");
        }

        FUNCTION("ReadReg01", "This function reads data from Reg01"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Read Reg01");
            return ReadBridgeReg("UREG01");
        }

        FUNCTION("WriteANYReg", "This function writes data to register with specified address"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Write ANY Reg");
            return WriteBridgeReg("ANY");
        }

        FUNCTION("ReadANYReg", "This function reads data from register with specified address"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Read ANY Reg");
            return ReadBridgeReg("ANY");
        }

        FUNCTION("WriteReadUART1", "This function writes and reads data via UART1"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Write and Read UART1");
            QByteArray temp1;
            QByteArray temp2;
            return WriteReadUART(true, temp1, &temp2, true, true, RX1_BUF, 0, 500, true);
        }

        FUNCTION("WriteReadUART1RICC", "This function writes and reads data via UART1 from RICC"
                 ,"<i><br> two parameters, first parameter: ByteArray to be send, second parameter: number bytes to read"
                 ,"RETURN ByteArray of read data")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Write and Read UART1 from RICC");
            QByteArray txdata;
            QByteArray rxdata;
            int rxcount;

            txdata = p1.toByteArray();
            rxcount = p2.toInt();

            if (WriteReadUART(false, txdata, &rxdata, true, true, RX1_BUF, rxcount, 500, true) == true)
            {
                return rxdata;
            }
            else
            {
                return QByteArray();
            }
        }

        FUNCTION("WriteReadUART2RICC", "This function writes and reads data via UART2 from RICC"
                 ,"<i><br> two parameters, first parameter: ByteArray to be send, second parameter: number bytes to read"
                 ,"RETURN ByteArray of read data")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Write and Read UART2 from RICC");
            QByteArray txdata;
            QByteArray rxdata;
            int rxcount;

            txdata = p1.toByteArray();
            rxcount = p2.toInt();

            if (WriteReadUART(false, txdata, &rxdata, true, true, RX2_BUF, rxcount, 500, true) == true)
            {
                return rxdata;
            }
            else
            {
                return QByteArray();
            }
        }

        FUNCTION("AddUART1CRC", "This function add one UART crc byte to the end"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Add CRC to UART1 command");
            QByteArray datadata;
            datadata = QByteArray::fromHex(ui->lineEditWriteDataUART1->text().toLatin1().replace(" ",""));
            datadata.append(CalculateMOBDCRC(datadata));
            ui->lineEditWriteDataUART1->setText(datadata.toHex());
            return true;
        }

        FUNCTION("WriteReadUART2", "This function writes and reads data via UART2"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Write and Read UART2");
            QByteArray temp1;
            QByteArray temp2;
            return WriteReadUART(true, temp1, &temp2, true, true, RX2_BUF, 0, 500, true);
        }

        FUNCTION("AddUART2CRC", "This function add one UART crc byte to the end"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Add CRC to UART1 command");
            QByteArray datadata;
            datadata = QByteArray::fromHex(ui->lineEditWriteDataUART2->text().toLatin1().replace(" ",""));
            datadata.append(CalculateMOBDCRC(datadata));
            ui->lineEditWriteDataUART2->setText(datadata.toHex());
            return true;
        }

        FUNCTION("WriteReadBRIDGE", "This function writes and reads data to BRIDGE"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_DSI3Master->Log(LOG_TYPE_BUTTON, "UART-UART Controller - Write and Read RAW command to UART bridge");
            QByteArray temp1;
            QByteArray temp2;
            return WriteReadBRIDGE(true, temp1, &temp2, true, true, 0, 500);
        }

        FUNCTION("LoadConfigFile", "This function loads configuration file with specified name, placed in the location of DSI3PAS.exe"
                 ,"<i><br> parameter filename(e.g. config.csv)"
                 ,"RETURN true/false")
        {
            g_BenchApplication->loadConfig(p1.toString());
            return true;
        }

        FUNCTION("CheckCRC", ""
                 ,"<i><br> parameter"
                 ,"RETURN true/false")
        {
            CheckCRC();
            return true;
        }

    return false;
}


void UartUartController::setupBoard()
{
}


void UartUartController::suspend()
{       
}

void UartUartController::resume()
{
    if (gp_Settings->mVersion == VERSION_UPID)
    {
        ui->groupBox->setEnabled(false);
        ui->groupBox_2->setEnabled(false);
        ui->groupBox_4->setEnabled(false);
    }
    else
    {
        ui->groupBox->setEnabled(true);
        ui->groupBox_2->setEnabled(true);
        ui->groupBox_4->setEnabled(true);
    }
    setupBoard();
}

bool UartUartController::WriteReadUART(bool inLineEdit, QByteArray &data_write, QByteArray *pdata_read, bool write, bool read, int channel, int read_length, int timeout, bool reset)
{
    bool result = false;

    if (inLineEdit)
    {
        if (channel == RX1_BUF)
        {
            data_write = QByteArray::fromHex(ui->lineEditWriteDataUART1->text().toLatin1().replace(" ",""));
            read_length = ui->lineEditReadDataCountUART1->text().toInt();
        }
        else if(channel == RX2_BUF)
        {
            data_write = QByteArray::fromHex(ui->lineEditWriteDataUART2->text().toLatin1().replace(" ",""));
            read_length = ui->lineEditReadDataCountUART2->text().toInt();
        }
        else
            return false;
    }
    if (reset)
        gp_DSI3Master->Log(LOG_TYPE_GPIO, "MDSEL set low and high");

    if (channel == RX2_BUF)
        gp_DSI3Master->Log(LOG_TYPE_UART2_TX, data_write.toHex());
    else
        gp_DSI3Master->Log(LOG_TYPE_UART1_TX, data_write.toHex());

    QBAtoTX(data_write, channel, reset);

    result = BENCH()->board()->writereadData(data_write, pdata_read, write, read, channel, 2*read_length, timeout);

    if (result && (read_length>0))
    {
        QBAfromRX(channel, *pdata_read);
    }
    else
    {
        *pdata_read = QByteArray();
    }
    if (read_length > 0)
    {
        if (channel == RX2_BUF)
            gp_DSI3Master->Log(LOG_TYPE_UART2_RX, pdata_read->toHex());
        else
            gp_DSI3Master->Log(LOG_TYPE_UART1_RX, pdata_read->toHex());
    }

    if (inLineEdit)
    {
        if (channel == RX1_BUF)
            ui->lineEditReadDataUART1->setText(pdata_read->toHex());
        else if(channel == RX2_BUF)
            ui->lineEditReadDataUART2->setText(pdata_read->toHex());
    }
    return result;
}

bool UartUartController::WriteReadBRIDGE(bool inLineEdit, QByteArray &data_write, QByteArray *pdata_read, bool write, bool read, int read_length, int timeout)
{
    bool result = false;

    if (inLineEdit)
    {
        data_write = QByteArray::fromHex(ui->lineEditWriteDataBRIDGE->text().toLatin1().replace(" ",""));
        read_length = ui->lineEditReadDataCountBRIDGE->text().toInt();
    }

    result = BENCH()->board()->writereadData(data_write, pdata_read, write, read, RXBRIDGE_BUF, read_length, timeout);

    if (result && inLineEdit)
    {
        ui->lineEditReadDataBRIDGE->setText(pdata_read->toHex());
    }
    else
    {
        ui->lineEditReadDataBRIDGE->setText("");
    }
    return result;
}

bool UartUartController::SetMDSEL(int type)
{
    QByteArray command;
    bool result;

    if (type == 0)
    {
        command.append((char)0x00);
        gp_DSI3Master->Log(LOG_TYPE_GPIO, "MDSEL set low");
    }
    else if (type == 1)
    {
        command.append((char)0x01);
        gp_DSI3Master->Log(LOG_TYPE_GPIO, "MDSEL set high");
    }
    else if (type == 101)
    {
        command.append((char)0x01);           command.append((char)0x00);           command.append((char)0x01);
        gp_DSI3Master->Log(LOG_TYPE_GPIO, "MDSEL set low and high");
    }
    else
    {
        return false;
    }

    QByteArray temp;
    result = BENCH()->board()->writereadData(command, &temp, true, false, MDSEL, 0, 0);
    return result;
}

bool UartUartController::GetNINTx()
{
    QByteArray txdata;
    QByteArray rxdata;
    bool result;

    gp_DSI3Master->Log(LOG_TYPE_GPIO, "Get NINTx");

    txdata.append((char)0x70);
    result = BENCH()->board()->writereadData(txdata, &rxdata, true, true, RXNINT_BUF, 1, 500);
    if (result)
    {
        MReadNINT.fromByteArray(rxdata);
    }
    else
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "No response to Get NINT command");
    }
    slot_updateAllControls();
    return result;
}

bool UartUartController::SetBaudrate()
{
    bool result;
    QByteArray command;
    command.append((char)0xFF).append((char)0xFF).append((char)0xFF).append((char)0xFF);
    command.append((char)0xFF).append((char)0xFF).append((char)0xFF).append((char)0xFF);
    command.append((char)0xFF).append((char)0xFF).append((char)0xFF).append((char)0x00);
    command.append((char)0x00).append((char)0xFF).append((char)0xFF).append((char)0x00);
    command.append((char)0x00).append((char)0xFF).append((char)0x00).append((char)0x00);
    command.append((char)0xFF).append((char)0xFF).append((char)0x00).append((char)0x00);

    SetMDSEL(0);
    QByteArray temp;
    result = WriteReadUART(false, command, &temp, true, false, RX1_BUF, 0, 0, true);
    SetMDSEL(1);

    return result;
}

void UartUartController::QBAtoTX(QByteArray &data, int channel, bool reset)
{
    int i;
    QByteArray data_expanded;
    char control_nibble;

    switch (channel) {
    case RX1_BUF:
        control_nibble = 0x20;
        break;
    case RX2_BUF:
        control_nibble = 0x30;
        break;
    case RXNINT_BUF:
        control_nibble = 0x70;
        break;
    default:
        control_nibble = 0x10;
    }
    if (channel == RXNINT_BUF)
    {
        for (i = 0; i < data.length(); i++)
        {
            data_expanded.append(control_nibble);
        }
    }
    else
    {
        for (i = 0; i < data.length(); i++)
        {
            data_expanded.append(0x40 + (data.at(i)&0x0F));   // low nibble
            data_expanded.append(control_nibble + ((data.at(i)>>4)&0x0F));  //high nibble
        }
    }

    data.clear();
    if (reset)
    {
        data_expanded.prepend((char)0x01);
        data_expanded.prepend((char)0x00);
        data_expanded.prepend((char)0x01);
    }
    data = data_expanded;
}

void UartUartController::QBAfromRX(int channel, QByteArray &data)
{
    int i;
    QByteArray data_shorted;
    char control_nibble_low = 0x80;
    char control_nibble_high = 0x90;

    switch (channel) {
    case RX1_BUF:
        control_nibble_low = 0x00;
        control_nibble_high = 0x20;
        break;
    case RX2_BUF:
        control_nibble_low = 0x10;
        control_nibble_high = 0x30;
        break;
    case RXNINT_BUF:
        control_nibble_low = 0x70;
        break;
    }

    if (channel == RXNINT_BUF)
    {
        for (i = 0; i < data.size(); i++)
        {
            if ((data.at(i)&0xF0) == control_nibble_low)
            {
                data_shorted.append(data.at(i) & 0x0F);
            }
        }
    }
    else
    {
        for (i = 0; i < (data.size() / 2); i++)
        {
            if (((data.at(2*i)&0xF0) == control_nibble_low) && ((data.at(2*i+1)&0xF0) == control_nibble_high))
            {
                data_shorted.append((data.at(2*i)&0x0F) + ((data.at(2*i+1)&0x0F)<<4));
            }
        }
    }
    data.clear();
    data = data_shorted;
}

char UartUartController::CalculateMOBDCRC(QByteArray data)
{
    int i;
    const char generator = 0x07;    // polynom
    char crc = 0xFF;                // start with 0xFF


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


char UartUartController::CalculateCRC(QByteArray data)
{
    int i;
    const char generator = 0x2F;    // polynom
    char crc = 0xFF;                // start with 0xFF


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

char UartUartController::CalculateUPAECRC(QByteArray data)
{
    int i;
    const char generator = 0x2F;    // polynom
    char crc = 0xFF;                // start with 0xFF

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

char UartUartController::CalculateUPAECRCwithSEED(char seed, QByteArray data)
{
    int i;
    const char generator = 0x2F;    // polynom
    char crc = seed;                // start with 0xFF

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

quint16 UartUartController::CalculateUPAECRC16withSEED(quint16 seed, QByteArray data)
{
    int i;
    const quint16 generator = 0x1021;    // polynom
    quint16 crc = seed;                // start with 0xFF

    data.prepend((char)0x00).prepend((char)0x00);

    for (i = 0; i < data.length(); i++)
    {
        crc ^= (((quint16)data.at(i)) << 8);

        for (int i = 0; i < 8; i++)
        {
            if ((crc & 0x8000) != 0)
            {
                crc = (quint16)((crc << 1) ^ generator);
            }
            else
            {
                crc <<= 1;
            }
        }
    }

    return crc;
}



quint16 UartUartController::CalculateCRC16UPAEwithSEEDfromFPGA(quint16 seed, QByteArray data)
{
    QVector<quint16> CRC16_TAB(16);
    quint16 CRC16_INIT = 0x1D0F;
    quint16 crc;
    quint16 upd_mask;
    int i;

    CRC16_TAB[0] = 0x0000;    CRC16_TAB[1] = 0x1021;    CRC16_TAB[2] = 0x2042;    CRC16_TAB[3] = 0x3063;
    CRC16_TAB[4] = 0x4084;    CRC16_TAB[5] = 0x50A5;    CRC16_TAB[6] = 0x60C6;    CRC16_TAB[7] = 0x70E7;
    CRC16_TAB[8] = 0x8108;    CRC16_TAB[9] = 0x9129;    CRC16_TAB[10] = 0xA14A;    CRC16_TAB[11] = 0xB16B;
    CRC16_TAB[12] = 0xC18C;    CRC16_TAB[13] = 0xD1AD;    CRC16_TAB[14] = 0xE1CE;    CRC16_TAB[15] = 0xF1EF;


    if (seed == 0xFFFF)
    {
        crc = CRC16_INIT;
    }
    else if ((seed > 0) && (seed < 9))
    {
        crc = CRC16_TAB[seed];
    }


    for (i = 0; i < data.length() / 2; i++)
    {
        upd_mask = CRC16_TAB[((crc >> 12) & 0x0F) ^ ((data.at(2*i + 1) >> 4) & 0x0F)];
        crc = (crc << 4) ^ (upd_mask);
        upd_mask = CRC16_TAB[((crc >> 12) & 0x0F) ^ ((data.at(2*i + 1)) & 0x0F)];
        crc = (crc << 4) ^ (upd_mask);
        upd_mask = CRC16_TAB[((crc >> 12) & 0x0F) ^ ((data.at(2*i) >> 4) & 0x0F)];
        crc = (crc << 4) ^ (upd_mask);
        upd_mask = CRC16_TAB[((crc >> 12) & 0x0F) ^ ((data.at(2*i)) & 0x0F)];
        crc = (crc << 4) ^ (upd_mask);
    }

    return crc;
}

bool UartUartController::WriteBridgeReg(QString regname)
{
    quint8 addr = 0;
    quint8 data = 0;
    QByteArray txcommand;
    QByteArray rxcommand;
    bool result = false;

    if (regname == "ANY")
    {
        data = GET_REG("UREGANY[DATA]").toUInt();
        addr = GET_REG("UREGANY[ADDR]").toUInt();
    }
    else if (regname == "UREG00")
    {
        addr = 0;
        data = GET_REG("UREG00[7:0]").toUInt();
    }
    else if (regname == "UREG01")
    {
        addr = 1;
        data = GET_REG("UREG01[7:0]").toUInt();
    }
    else
    {
        return false;
    }

    txcommand.append((char)(((addr >> 0)&0x0F) | 0x40));
    txcommand.append((char)(((addr >> 4)&0x0F) | 0x50));
    txcommand.append((char)(((data >> 0)&0x0F) | 0x40));
    txcommand.append((char)(((data >> 4)&0x0F) | 0x80));

    result = WriteReadBRIDGE(false, txcommand, &rxcommand, true, false, 0, 500);
    return result;
}

bool UartUartController::ReadBridgeReg(QString regname)
{
    quint8 addr = 0;
    quint8 data = 0;
    QByteArray txcommand;
    QByteArray rxcommand;
    bool result = false;
    Register *reg;

    if (regname == "ANY")
    {
        addr = GET_REG("UREGANY[ADDR]").toUInt();
        reg = &UREGANY;
    }
    else if (regname == "UREG00")
    {
        addr = 0;
        reg = &UREG00;
    }
    else if (regname == "UREG01")
    {
        addr = 1;
        reg = &UREG01;
    }
    else
    {
        return false;
    }

    txcommand.append((char)(((addr >> 0)&0x0F) | 0x40));
    txcommand.append((char)(((addr >> 4)&0x0F) | 0x50));
    txcommand.append((char)(0x90));

    result = WriteReadBRIDGE(false, txcommand, &rxcommand, true, true, 2, 500);
    if ((result) && (rxcommand.size() == 2))
    {
        if (((rxcommand.at(0)&0xF0) == 0x40) && ((rxcommand.at(1)&0xF0) == 0x50))
        {
            data = (rxcommand.at(0)&0x0F) + ((rxcommand.at(1)&0x0F) << 4);
            reg->setValue(0, 7, data);
        }
        else
        {
            result = false;
        }
    }
    slot_updateAllControls();
    return result;
}

void UartUartController::SwitchUART2UART(void)
{
    SET_REG("UREG01[UART_INTERNAL]", 1);
    SET_REG("UREG01[UART_BRIDGE]", 0);
    WriteBridgeReg("UREG01");
}

void UartUartController::CheckCRC()
{
    QByteArray PDCMdata = QByteArray::fromHex(ui->lineEditPDCMdataCRC->text().toLatin1().replace(" ",""));
    int PDCMslot = ui->lineEditPDCMslot->text().toInt();
    char crc;
    char calccrc;
    bool result = false;

    if (PDCMdata.length() == 18)
    {
        crc = PDCMdata.at(17);
        calccrc = CalculateUPAECRCwithSEED(PDCMslot, PDCMdata.mid(0, 17));

        if (crc == calccrc)
        {
            result = true;
        }
    }

    if (result)
    {
        ui->lineEditPDCMcrcres->setText("ok");
    }
    else
    {
        ui->lineEditPDCMcrcres->setText("fail");
    }

    ui->lineEditPDCMcrccorrect->setText(QString("%1").arg(calccrc,2,16,QChar(0)));

}
