#include "uartspicontroller.h"
#include "ui_uartspicontroller.h"
#include "mainapp.h"
#include "dsi3master.h"
#include "asicmaster.h"
#include "dsi3slave.h"
#include "measurement.h"

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

UartSpiController *gp_UartSpiController;


UartSpiController::UartSpiController(QWidget *parent) :
    Applet(parent)
{
    ui = new Ui::UartSpiController;
    ui->setupUi(this);
    createCustomUi();
    setWindowIcon(QIcon(":/board/onlogo.ico"));
    setWindowTitle("UART-SPI Controller");
}

UartSpiController::~UartSpiController()
{
    delete ui;
}

bool UartSpiController::initialize()
{
    STATUS.makeSetup("@register=STATUS;"
              "CRC8[7:0];"
              "CRC_FAIL_B[15:8];"
              "CRC_FAIL_A[23:16];"
              "CLKIN_CNT[29:24];"
              "KAC_SLAVE[31:30];"
              "KAC_MASTER[33:32];"
              "HARDWARE_ERROR;"
              "SPI_ERROR;"
              "TIMER_INT_RECEIVED_A;"
              "TIMER_INT_RECEIVED_B;"
              "CRM_RECEIVED_B;"
              "CRM_RECEIVED_A;"
              "PDCM_RECEIVED_B[47:40];"
              "PDCM_RECEIVED_A[55:48];"
    );
    addDataNode(&STATUS);

    DEMUX.makeSetup("@register=DEMUX;"
              "M1DEMUX1;"
              "M1DEMUX2;"
              "M1DEMUX3;"
              "M1DEMUX4;"
              "M1DEMUX5;"
              "M1DEMUX6;"
              "_1[7:6];"
              "M2DEMUX1;"
              "M2DEMUX2;"
              "M2DEMUX3;"
              "M2DEMUX4;"
              "M2DEMUX5;"
              "M2DEMUX6;"
              "_2[15:14];"
    );
    addDataNode(&DEMUX);

    U2SREGX00.makeSetup("@register=U2SREGX00 @addr=0 @purpose=Config;"     //addr:0x00
                  "TEST[7:0];"
            );
    addDataNode(&U2SREGX00);
    U2SREGX01.makeSetup("@register=U2SREGX01 @addr=1 @purpose=Config;"     //addr:0x01
                  "Dbg_Pulse_Delay[7:0];"
            );
    addDataNode(&U2SREGX01);
    U2SREGX02.makeSetup("@register=U2SREGX02 @addr=2 @purpose=Config;"     //addr:0x02
                  "Dbg_Pulse_Delay[7:0];"
            );
    addDataNode(&U2SREGX02);
    U2SREGX03.makeSetup("@register=U2SREGX03 @addr=3 @purpose=Config;"     //addr:0x03
                  "bit0;"
                  "bit1;"
                  "bit2;"
                  "bit3;"
                  "bit4;"
                  "bit5;"
                  "bit6;"
                  "bit7;"
            );
    addDataNode(&U2SREGX03);
    U2SREGADDR.makeSetup("@register=U2SREGADDR @purpose=Config;" //addr:reg ADDR
                  "TEST[7:0];"
            );
    addDataNode(&U2SREGADDR);
    U2SADDR.makeSetup("@variable=U2SADDR" "@value=0" "@min=0 @max=255");
    addDataNode(&U2SADDR);
    BRC_CHANNEL.makeSetup("@variable=BRC_CHANNEL" "@value=0" "@min=0 @max=1");
    addDataNode(&BRC_CHANNEL);
    BRC_BRC.makeSetup("@variable=BRC_BRC" "@value=1" "@min=1 @max=63");
    addDataNode(&BRC_BRC);
    BRC_SLOTS.makeSetup("@variable=BRC_SLOTS" "@value=1" "@min=1 @max=8");
    addDataNode(&BRC_SLOTS);
    BRC_LENGTH.makeSetup("@variable=BRC_LENGTH" "@value=0" "@min=0 @max=31");
    addDataNode(&BRC_LENGTH);
    BRC_PERIOD.makeSetup("@variable=BRC_PERIOD" "@value=0" "@min=0 @max=8192");
    addDataNode(&BRC_PERIOD);
    BRC_RESP_LENGTH.makeSetup("@variable=BRC_RESP_LENGTH" "@value=0" "@min=0 @max=11000");
    addDataNode(&BRC_RESP_LENGTH);
    BRC2_RESP_LENGTH.makeSetup("@variable=BRC2_RESP_LENGTH" "@value=0" "@min=0 @max=11000" "@readonly=1");
    addDataNode(&BRC2_RESP_LENGTH);
    BRC3_RESP_LENGTH.makeSetup("@variable=BRC3_RESP_LENGTH" "@value=0" "@min=0 @max=11000" "@readonly=1");
    addDataNode(&BRC3_RESP_LENGTH);
    BRC_START.makeSetup("@variable=BRC_START" "@value=0" "@min=0 @max=65535");
    addDataNode(&BRC_START);
    REPEAT_TIME.makeSetup("@variable=REPEAT_TIME" "@value=40" "@min=40 @max=2000");
    addDataNode(&REPEAT_TIME);

    addDataNode(&gp_ASICMaster->REGX02);

    return true;
}

void UartSpiController::createCustomUi()
{
    ui->tabWidget->setCurrentIndex(0);
}


/****************************************************************************
 *                              FUNCTIONALITY
*****************************************************************************/


// updates all RELAYS and GPIOs
bool UartSpiController::updateHardware(Direction direction, const QString &item)
{
    return false;
}

QVariant UartSpiController::FUNCTION_LIST()
{        

    if(!isActivated()) return false;

    QByteArray data;
    QByteArray data_send;
    QByteArray data_read;

    ParameterParser params(p1.toString(),",");

        FUNCTION("Update", "This function updates hardware"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            return updateHardware(HARDWARE_GET,"*");
        }
        FUNCTION("WriteRead", "Write and read data to UART Interface "
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            bool result = false;
            // custom for write
            //if can't parse from parameter will take from LineEdit control

            if(!params.get("data", &data))
            {
                if ((ui->lineEditWriteData->text().toLatin1().replace(" ","").length() % 2) == 0)
                    data = QByteArray::fromHex(ui->lineEditWriteData->text().toLatin1().replace(" ",""));
                else
                    data.clear();
                data.toUInt(0, 10);
            }
            else
                ui->lineEditWriteData->setText(data.toHex());

            if (data != "")
            {
                data_send.append(data);

                result = BENCH()->board()->writereadDataUPID(data_send, &data_read, true, true, 500);       //transfer data to UART

                if (result == true)
                {
                    ui->lineEditReadData->setText(data_read.toHex());
                    ui->lineEditReadDataCount->setText(QString("%1").arg(data_read.length()));

                    return QString(data_read.toHex());
                }
                else
                {
                    ui->lineEditReadData->setText("");
                    ui->lineEditReadDataCount->setText(QString("0"));

                    return QString("");
                }
            }
            else
            {
                LERROR("No input data or number of input data is not even");
            }


            return result;
        }
        FUNCTION("WriteOnly", "Write data to UART Interface "
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            bool result = false;
            // custom for write
            //if can't parse from parameter will take from LineEdit control

            if(!params.get("data", &data))
                    data = QByteArray::fromHex(ui->lineEditWriteData->text().toLatin1().replace(" ",""));
            else
                ui->lineEditWriteData->setText(data.toHex());

            data_send.append(data);

            result = BENCH()->board()->writereadDataUPID(data_send, &data_read, true, false, 500);       //transfer data to UART

            ui->lineEditReadData->setText("");
            ui->lineEditReadDataCount->setText(QString("0"));

            return result;
        }

        FUNCTION("ReadOnly", "Read data from UART Interface "
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            bool result = false;

            result = BENCH()->board()->writereadDataUPID(data_send, &data_read, false, true, 1500);       //transfer data to UART

            if (p1.toString() == "brc2=yes")
            {
                if (result == true)
                {
                    //if (data_read.length() > 2)
                    //{
                    //    data_read.remove(0, 2);
                    //}
                    return data_read.toHex();
                }
                else
                {
                    return QString("");
                }
            }
            else
            {
                if (result == true)
                {
                    ui->lineEditReadData->setText(data_read.toHex());
                    ui->lineEditReadDataCount->setText(QString("%1").arg(data_read.length()));

                    return QString(data_read.toHex());
                }
                else
                {
                    ui->lineEditReadData->setText("");
                    ui->lineEditReadDataCount->setText(QString("0"));

                    return QString("");
                }
            }
        }

        FUNCTION("SPIWriteRead", "Write and read SPI command to UART Interface "
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QString data_read_string;
            char kac = 0;
            char first_byte;
            int crc_yes = 0;
            QByteArray rdata;
            QByteArray data_crc;

            // custom for write
            //if can't parse from parameter will take from LineEdit control

            if(!params.get("data", &data))      // not parsed
            {
                if ((ui->lineEditSPIWriteData->text().toLatin1().replace(" ","").length() % 2) == 0)
                    data = QByteArray::fromHex(ui->lineEditSPIWriteData->text().toLatin1().replace(" ",""));
                else
                    data.clear();
            }
            else                                // parsed
                ui->lineEditSPIWriteData->setText(data.toHex());

            params.get("crc_yes", &crc_yes);

            if (data != "")
            {
                if (crc_yes == 1)               // add CRC
                {
                    runAppCommand("SPICRC","","","","","");
                    data = QByteArray::fromHex(ui->lineEditSPIWriteData->text().toLatin1().replace(" ",""));
                }

                gp_DSI3Master->Log(LOG_TYPE_SPI_TX, data.toHex());

                data_send.append(data);

                if (data_send.length() < 30)
                {
                    first_byte = data_send.length() & 0x1F;     //ADD UART HEADER
                    first_byte += (kac & 0x03) << 5;
                    data_send.prepend(first_byte);

                }
                else
                {
                    kac = 3;
                    first_byte = (kac & 0x03) << 5;
                    data_send.prepend(data_send.length() & 0xFF);
                    data_send.prepend(first_byte);
                }

                QString test = data_send.toHex();

                data_read_string = runAppCommand("WriteRead", QString("data=%1").arg(test),"","","","").toString();      // transfer data

                if (data_read_string != "")     // if result is OK
                {
                    data_read = QByteArray::fromHex(data_read_string.toLatin1());

                    char checkchar = ((first_byte & 0x70) + 0x88);

                    if ((data_read.at(0) == 0x10) && (data_read.at(1) == checkchar))
                    {
                        data_read.remove(0, 2);
                        ui->lineEditSPIReadData->setText(data_read.toHex());
                        ui->lineEditSPIReadDataCount->setText(QString("%1").arg(data_read.length()));

                        gp_DSI3Master->Log(LOG_TYPE_SPI_RX, data_read.toHex());

                        data_crc = data_read;
                        data_crc.resize(6);

                        if (data_read.at(6) != CalculateCRC(data_crc))
                        {
                            LERROR("ASIC master RX SPI CRC is not matching");
                        }

                        if (m_last_command_read == false)
                        {
                            rdata.append(data_read.at(6)).append(data_read.at(5)).append(data_read.at(4)).append(data_read.at(3))\
                                    .append(data_read.at(2)).append(data_read.at(1)).append(data_read.at(0));
                            STATUS.fromByteArray(rdata);
                            slot_updateAllControls();
                        }
                        else
                        {
                            SET_REG("STATUS[31:30]", data_read.at(1) & 0x03);            //KAC_SLAVE
                            SET_REG("STATUS[33:32]", (data_read.at(1) >> 2) & 0x03);     //KAC_MASTER
                            SET_REG("STATUS[34]", (data_read.at(1) >> 4) & 0x01);        //HARDWARE_ERROR
                            SET_REG("STATUS[35]", (data_read.at(1) >> 5) & 0x01);        //SPI_ERROR

                            if (((data_send.at(1) >> 5) & 0x01) == 0x00)                // CHANNEL A
                            {
                                SET_REG("STATUS[37]", (data_read.at(1) >> 6) & 0x01);      // TIMER_INT_B
                                SET_REG("STATUS[39]", (data_read.at(1) >> 7) & 0x01);      // CRM_RECEIVED_A
                                SET_REG("STATUS[55:48]", (data_read.at(0)));                // PDCM_RECEIVED_A
                            }
                            else
                            {
                                SET_REG("STATUS[36]", (data_read.at(1) >> 6) & 0x01);      // TIMER_INT_B
                                SET_REG("STATUS[38]", (data_read.at(1) >> 7) & 0x01);      // CRM_RECEIVED_B
                                SET_REG("STATUS[47:40]", (data_read.at(0)));                // PDCM_RECEIVED_B
                            }
                        }


                        if((data_send.at(1) & 0x1F) == 0x03)
                        {
                            m_last_command_read = true;
                        }
                        else
                        {
                            m_last_command_read = false;
                        }

                        return QString(data_read.toHex());
                    }
                    else
                    {
                        LERROR("Unexpected result");
                    }
                }
                else
                {
                    LERROR("No response");
                }
            }
            else
            {
                LERROR("No input data or number of input data is not even");
            }


            ui->lineEditSPIReadData->setText("");
            ui->lineEditSPIReadDataCount->setText(QString("0"));
            return QString("");
        }

        FUNCTION("PerformBRC", "Perform BRC command to UART interface"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QString result = "";
            QString str_command;
            int Channel = 0;
            int Brc = 0;
            int Slots = 0;
            int Length = 0;
            int Period = 0;
            int RX_Length = 0;
            int Start = 0;

            QByteArray command;
            //QByteArray data_read;

            Channel = GET_REG("BRC_CHANNEL").toInt();

            if(!params.get("BRC", &Brc))      // not parsed
                Brc = GET_REG("BRC_BRC").toInt();
            else                                // parsed
                SET_REG("BRC_BRC", Brc);

            if(!params.get("SLOTS", &Slots))      // not parsed
                Slots = GET_REG("BRC_SLOTS").toInt();
            else                                // parsed
                SET_REG("BRC_SLOTS", Slots);

            if(!params.get("LENGTH", &Length))      // not parsed
                Length = GET_REG("BRC_LENGTH").toInt();
            else                                // parsed
                SET_REG("BRC_LENGTH", Length);

            if(!params.get("PERIOD", &Period))      // not parsed
                Period = GET_REG("BRC_PERIOD").toInt();
            else                                // parsed
                SET_REG("BRC_PERIOD", Period);

            if(!params.get("START", &Period))      // not parsed
                Start = GET_REG("BRC_START").toInt();
            else                                // parsed
                SET_REG("BRC_START", Start);

            //int Scrambler_ena = GET_REG("REGX02[SCRAMBLER_ENA]").toInt();
            int Scrambler_ena = gp_ASICMaster->REGX02.value("[SCRAMBLER_ENA]");

            command[0] = 0x80 + (Brc & 0x3F);
            command[1] = (((Slots - 1) & 0x07) << 5) + (Length & 0x1F);
            command[2] = (((Period & 0x1FFF) >> 8) & 0x1F) + ((Channel & 0x01) << 5) + ((Scrambler_ena & 0x01) << 6);
            command[3] = Period & 0xFF;
            command[4] = (Start >> 8) & 0xFF;
            command[5] = (Start) & 0xFF;


            str_command = command.toHex();

            result = runAppCommand("WriteRead", QString("data=%1").arg(str_command),"","","","").toString();       //transfer data to UART

            RX_Length = Brc * (Slots) * (Length + 2);
            SET_REG("BRC_RESP_LENGTH", RX_Length);

            if (result != "")   //if result is OK
            {
                data_read = QByteArray::fromHex(result.toLatin1());

                char checkchar = ((command[0] & 0x70) + 0x08);

                if ((data_read.at(0) == 0x10) && (data_read.at(1) == checkchar))
                {
                    data_read.remove(0, 2);
                    ui->lineEditBRCReadData->setText(data_read.toHex());
                    ui->lineEditBRCReadDataCount->setText(QString("%1").arg(data_read.length()));
                    //gp_BRC->runAppCommand("SetBRCData", QString("data=%1").arg(QString(data_read.toHex())),"","","","");

                    data_read.prepend(char(0x01));
                    gp_Measurement->SetBRCReadData(data_read.toHex());

                    //gp_UPIDMemory->runAppCommand("ReadREGX", "REGX12").toBool();
                    //gp_UPIDMemory->runAppCommand("GetCRMResp", "REGX20").toBool();

                    //return QString(data_read.toHex());
                    return true;
                }
                else
                {
                    LERROR("Unexpected data received");
                }
            }
            return "";
        }

        FUNCTION("SetDemux", "Set demultiplexer"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            data = DEMUX.toByteArray();
            data.prepend((char)0xC0);
            QString str_data = data.toHex();
            runAppCommand("WriteRead", QString("data=%1").arg(str_data),"","","","");

            return true;
        }

        FUNCTION("SPICRC", "Calculate CRC for SPI frame"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {

            if ((ui->lineEditSPIWriteData->text().toLatin1().replace(" ", "").length() % 2) == 0)
            {
                data = QByteArray::fromHex(ui->lineEditSPIWriteData->text().toLatin1().replace(" ",""));
                QByteArray data_crc;

                /*if (data.length() > 6)
                    data.resize(6);*/

                while (data.length() < 7)
                {
                    data.append(QChar(0));
                }

                data.remove(6, 1);

                data_crc = data;
                data_crc.resize(6);

                data.insert(6, CalculateCRC(data_crc));


                ui->lineEditSPIWriteData->setText(data.toHex());

                return true;
            }
            else
            {
                LERROR("Number of input data needs to be even");
                return false;
            }

        }

        FUNCTION("WriteREGX", "Write into register in UART2SPI bridge"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            Register *preg;
            QByteArray command;
            QString cmd;
            QByteArray resp;
            quint8 addr_wr;

            if(GetU2SREGXPointer(p1.toString(), &preg))
            {
                QByteArray regData = preg->toByteArray();       // BYTE3,BYTE2,BYTE1,BYTE0

                if ( preg->extra("addr") != "")
                {
                    addr_wr = (quint8)preg->extra("addr").toUInt(0,10);
                }
                else
                {
                    addr_wr = GET_REG("U2SADDR").toInt();
                }

                command.append((char)0x20).append((char)0x01).append((char)addr_wr).append((char)regData.at(0));
                cmd = command.toHex();
                resp = runAppCommand("WriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();

                if (resp != "")
                {
                    resp = QByteArray::fromHex(resp);
                    if (resp.at(2) == 0x01)
                    {
                        //LERROR("Write command was not successful, address may be wrong");
                        return false;
                    }
                    return true;
                }

                else
                    return false;
            }
            else
            {
                LERROR("The parameter of the RUN command is not corrent! E.g.: Use REGX01 as a parameter");
                return false;
            }

            return true;
        }

        FUNCTION("ReadREGX", "Read register from UART2SPI bridge"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            Register *preg;
            QByteArray command;
            QByteArray regdata;
            QByteArray rdata;
            QString cmd;
            quint8 addr;

            if (GetU2SREGXPointer(p1.toString(), &preg))
            {
                if ( preg->extra("addr") != "")
                {
                    addr = (quint8)preg->extra("addr").toUInt(0,10);
                }
                else
                {
                    addr = GET_REG("U2SADDR").toInt();
                }

                command.append((char)0x20).append((char)0x00).append((char)addr);
                cmd = command.toHex();

                regdata = runAppCommand("WriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();

                if (regdata != "")
                {
                    regdata = QByteArray::fromHex(regdata);

                    rdata.append(regdata.at(2));

                    preg->fromByteArray(rdata);
                    slot_updateAllControls();
                    return true;
                }
                return false;

            }
            else
            {
                LERROR("The parameter of the RUN command is not corrent! E.g.: Use REGX01 as a parameter");
                return false;
            }
        }
    return false;
}


void UartSpiController::setupBoard()
{
}


void UartSpiController::suspend()
{       
}

void UartSpiController::resume()
{
    if (gp_Settings->mVersion == VERSION_MOBD)
    {
        ui->tabWidget->setEnabled(false);
    }
    else
    {
        ui->tabWidget->setEnabled(true);
    }
    setupBoard();
}


char UartSpiController::CalculateCRC(QByteArray data)
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

char UartSpiController::CalculateUPAECRC(QByteArray data)
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

char UartSpiController::CalculateUPAECRCwithSEED(char seed, QByteArray data)
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

quint16 UartSpiController::CalculateUPAECRC16withSEED(quint16 seed, QByteArray data)
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



quint16 UartSpiController::CalculateCRC16UPAEwithSEEDfromFPGA(quint16 seed, QByteArray data)
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
bool UartSpiController::GetU2SREGXPointer(QString name, Register **preg)
{
    uint address;
    bool result = false;

    if (name.contains("U2SREGX"))
    {
        result = true;

        name.remove("U2SREGX");
        address = name.toUInt(0, 16);

        switch (address)
        {
        case 0:
           *preg = &U2SREGX00;
            break;
        case 1:
            *preg = &U2SREGX01;
            break;
        case 2:
            *preg = &U2SREGX02;
            break;
        case 3:
            *preg = &U2SREGX03;
            break;

        default:
            result = false;
            break;
        }
    }
    else if (name.contains("U2SADDRX"))
    {
        name.remove("U2SADDRX");
        address = name.toUInt(0, 16);
        SET_REG("U2SADDR", address & 0xFF);
        *preg = &U2SREGADDR;
        result = true;
    }
    else if (name.contains("U2SADDR"))
    {
        *preg = &U2SREGADDR;
        result = true;
    }

    return result;
}
