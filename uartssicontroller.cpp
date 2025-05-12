#include "uartssicontroller.h"
#include "ui_uartssicontroller.h"
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
#define LOG_TYPE_SPI_CMD    (16)
#define LOG_TYPE_SSI_TX     (17)
#define LOG_TYPE_SSI_RX     (18)
#define LOG_TYPE_SSI_CMD    (19)

UartSsiController *gp_UartSsiController;


UartSsiController::UartSsiController(QWidget *parent) :
    Applet(parent)
{
    ui = new Ui::UartSsiController;
    ui->setupUi(this);
    createCustomUi();
    setWindowIcon(QIcon(":/board/onlogo.ico"));
    setWindowTitle("UART-SSI Controller");

    m_escapedcharsRDUM[0] = 0x10;
    m_escapedreplacementRDUM[0] = 0x00;
    m_escapedcharsRDUM[1] = 0x11;
    m_escapedreplacementRDUM[1] = 0x02;
    m_escapedcharsRDUM[2] = 0x13;
    m_escapedreplacementRDUM[2] = 0x03;
    m_escapedcharsRDUM[3] = 0x01;
    m_escapedreplacementRDUM[3] = 0x05;
    m_escapedcharsRDUM[4] = 0x06;
    m_escapedreplacementRDUM[4] = 0x07;
    m_escapedcharsRDUM[5] = 0x15;
    m_escapedreplacementRDUM[5] = 0x08;
    m_escapedcharsRDUM[6] = 0x04;
    m_escapedreplacementRDUM[6] = 0x09;

}

UartSsiController::~UartSsiController()
{
    delete ui;
}

bool UartSsiController::initialize()
{
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

    SSIREGX000.makeSetup("@register=SSIREGX000 @addr=0;"
                  "DBG_SLP_SLP[15:0];"
                  "FREE1[31:16];"
            );
    addDataNode(&SSIREGX000);

    SSIREGX004.makeSetup("@register=SSIREGX004 @addr=4;"
                  "DBG_SLP_SLP1[15:0];"
                  "FREE1[31:16];"
            );
    addDataNode(&SSIREGX004);

    SSIREGX008.makeSetup("@register=SSIREGX008 @addr=8;"
                  "DBG_SLP_SLP2[15:0];"
                  "FREE1[31:16];"
            );
    addDataNode(&SSIREGX008);

    SSIREGX00C.makeSetup("@register=SSIREGX00C @addr=12;"
                  "DBG_SLP_SLP3[15:0];"
                  "FREE1[31:16];"
            );
    addDataNode(&SSIREGX00C);

    SSIREGX010.makeSetup("@register=SSIREGX010 @addr=16;"
                  "DBG_SLP_END[15:0];"
                  "FREE1[31:16];"
            );
    addDataNode(&SSIREGX010);

    SSIREGX014.makeSetup("@register=SSIREGX014 @addr=20;"
                  "DBG_SLP_START[15:0];"
                  "FREE1[31:16];"
            );
    addDataNode(&SSIREGX014);

    SSIREGX018.makeSetup("@register=SSIREGX018 @addr=24;"
                  "PDM1_SEL[7:0];"
                  "PDM2_SEL[15:8];"
                  "FREE1[31:16];"
            );
    addDataNode(&SSIREGX018);

    SSIREGX01C.makeSetup("@register=SSIREGX01C @addr=28;"
                  "DBG1_SEL[7:0];"
                  "DBG2_SEL[15:8];"
                  "DBG3_SEL[23:16];"
                  "DBG4_SEL[31:24];"
            );
    addDataNode(&SSIREGX01C);

    SSIREGX020.makeSetup("@register=SSIREGX020 @addr=32;"
                  "FAST_SDM[1:0];"
                  "FREE1[31:2];"
            );
    addDataNode(&SSIREGX020);

    SSIREGX024.makeSetup("@register=SSIREGX024 @addr=36;"
                  "PULSE_DELAY[23:0];"
                  "FREE1[31:24];"
            );
    addDataNode(&SSIREGX024);

    SSIREGX028.makeSetup("@register=SSIREGX028 @addr=40;"
                  "PULSE_PERIOD[23:0];"
                  "FREE1[31:24];"
            );
    addDataNode(&SSIREGX028);

    SSIREGX02C.makeSetup("@register=SSIREGX02C @addr=44;"
                  "PULSE_CNT[7:0];"
                  "FREE1[31:8];"
            );
    addDataNode(&SSIREGX02C);

    SSIREGX030.makeSetup("@register=SSIREGX030 @addr=48;"
                  "PULSE_WIDTH[15:0];"
                  "FREE1[31:16];"
            );
    addDataNode(&SSIREGX030);

    SSIREGX034.makeSetup("@register=SSIREGX034 @addr=52;"
                  "CLKIN_OSC_F[11:0];"
                  "FREE1[22:12];"
                  "CLKIN_2MHZ;"
                  "FREE2[30:24];"
                  "CLKIN_OSC_ENA;"
            );
    addDataNode(&SSIREGX034);

    SSIREGX038.makeSetup("@register=SSIREGX038 @addr=56;"
                  "NRES;"
                  "FREE1[31:1];"
            );
    addDataNode(&SSIREGX038);

    SSIREGX03C.makeSetup("@register=SSIREGX03C @addr=60;"
                  "INTERFACE_SEL[3:0];"
                  "FREE1[31:4];"
            );
    addDataNode(&SSIREGX03C);

    SSIREGX040.makeSetup("@register=SSIREGX040 @addr=64;"
                  "DEMUX_CHA[5:0];"
                  "FREE1[15:6];"
                  "DEMUX_CHB[21:16];"
                  "FREE1[31:22];"
            );
    addDataNode(&SSIREGX040);

    SSIREGX1EC.makeSetup("@register=SSIREGX1EC @addr=492;"
                  "SVN_REV[15:0];"
                  "FREE1[31:16];"
            );
    addDataNode(&SSIREGX1EC);

    SSIREGX1F0.makeSetup("@register=SSIREGX1F0 @addr=496;"
                  "SYNTH_TIME[31:0];"
            );
    addDataNode(&SSIREGX1F0);

    SSIREGX1F4.makeSetup("@register=SSIREGX1F4 @addr=500;"
                  "USER[31:0];"
            );
    addDataNode(&SSIREGX1F4);

    SSIREGX1F8.makeSetup("@register=SSIREGX1F8 @addr=504;"
                  "USER[15:0];"
                  "FREE1[31:16];"
            );
    addDataNode(&SSIREGX1F8);

    SSIREGX1FC.makeSetup("@register=SSIREGX1FC @addr=508;"
                  "DUMMY[1:0];"
                  "FREE1[31:2];"
            );
    addDataNode(&SSIREGX1FC);




    SSIREGADDR.makeSetup("@register=SSIREGADDR @purpose=Config;" //addr:reg ADDR
                  "TEST[31:0];"
            );
    addDataNode(&SSIREGADDR);
    SSIADDR.makeSetup("@variable=SSIADDR" "@value=0" "@min=0 @max=2147483647");
    addDataNode(&SSIADDR);

    SSIREGADDRMEM.makeSetup("@register=SSIREGADDRMEM @purpose=Config;" //addr:reg ADDR
                  "TEST[31:0];"
            );
    addDataNode(&SSIREGADDRMEM);
    SSIADDRMEM.makeSetup("@register=SSIADDRMEM @purpose=Config;" //addr:reg ADDR
                  "TEST[31:0];"
            );
    addDataNode(&SSIADDRMEM);

    /*BRC_CHANNEL.makeSetup("@variable=BRC_CHANNEL" "@value=0" "@min=0 @max=1");
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
    addDataNode(&REPEAT_TIME);*/

    SSI_CHANNEL.makeSetup("@variable=SSI_CHANNEL" "@value=0" "@min=0 @max=1");
    addDataNode(&SSI_CHANNEL);
    SSI_MEAS.makeSetup("@variable=SSI_MEAS" "@value=0" "@min=0 @max=1");
    addDataNode(&SSI_MEAS);
    SSI_READ.makeSetup("@variable=SSI_READ" "@value=0" "@min=0 @max=1");
    addDataNode(&SSI_READ);
    SSI_BRC_COUNT.makeSetup("@variable=SSI_BRC_COUNT" "@value=0" "@min=0 @max=255");
    addDataNode(&SSI_BRC_COUNT);
    SSI_SLOT_COUNT.makeSetup("@variable=SSI_SLOT_COUNT" "@value=0" "@min=0 @max=8");
    addDataNode(&SSI_SLOT_COUNT);
    SSI_MEAS_BRC_DELAY.makeSetup("@variable=SSI_MEAS_BRC_DELAY" "@value=0" "@min=0 @max=65535");
    addDataNode(&SSI_MEAS_BRC_DELAY);
    SSI_BRC_BRC_DELAY.makeSetup("@variable=SSI_BRC_BRC_DELAY" "@value=0" "@min=0 @max=65535");
    addDataNode(&SSI_BRC_BRC_DELAY);
    SSI_BRC_GET_DELAY.makeSetup("@variable=SSI_BRC_GET_DELAY" "@value=0" "@min=0 @max=65535");
    addDataNode(&SSI_BRC_GET_DELAY);
    SSI_GET_COUNT.makeSetup("@variable=SSI_GET_COUNT" "@value=0" "@min=0 @max=255");
    addDataNode(&SSI_GET_COUNT);
    SSI_GET_BYTES.makeSetup("@variable=SSI_GET_BYTES" "@value=0" "@min=0 @max=65535");
    addDataNode(&SSI_GET_BYTES);
    SSI_GET_GET_DELAY.makeSetup("@variable=SSI_GET_GET_DELAY" "@value=0" "@min=0 @max=65535");
    addDataNode(&SSI_GET_GET_DELAY);

    SSI_XOR.makeSetup("@variable=SSI_XOR" "@value=1" "@min=0 @max=1");
    addDataNode(&SSI_XOR);

    addDataNode(&gp_ASICMaster->REGX02);

    return true;
}

void UartSsiController::createCustomUi()
{
    ui->tabWidget->setCurrentIndex(0);
}


/****************************************************************************
 *                              FUNCTIONALITY
*****************************************************************************/


// updates all RELAYS and GPIOs
bool UartSsiController::updateHardware(Direction direction, const QString &item)
{
    return false;
}

QVariant UartSsiController::FUNCTION_LIST()
{        

    if(!isActivated()) return false;

    QByteArray data;
    QByteArray data_send;
    QByteArray data_read;
    QString run_rdum = "run RDUM=";
    QString rdum = "RDUM ";


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

                gp_DSI3Master->Log(LOG_TYPE_UART1_TX, data.toHex());

                QBAtoTXRDUM(data);

                ui->lineEditWriteDataESC->setText(data.toHex());
                ui->lineEditWriteDataCountESC->setText(QString("%1").arg(data.length()));

                /*if (gp_Settings->mVersion == VERSION_RDUM_SPIMASTER)
                {
                    data = data.toHex();
                    data_send.append(data);
                    data_send.prepend(run_rdum.toLatin1());
                    gp_DSI3Master->Log(LOG_TYPE_UART2_TX, data_send.toHex());
                    gp_DSI3Master->Log(LOG_TYPE_UART2_TX, QString(data_send));
                }
                else
                {
                    data_send.append(data);
                    gp_DSI3Master->Log(LOG_TYPE_UART2_TX, data_send.toHex());
                }*/

                //data = data.toHex();
                data_send.append(data);
                //data_send.prepend(run_rdum.toLatin1());
                gp_DSI3Master->Log(LOG_TYPE_UART2_TX, data_send.toHex());
                //gp_DSI3Master->Log(LOG_TYPE_UART2_TX, QString(data_send));





                result = BENCH()->board()->writereadDataRDUM(data_send, &data_read, true, true, 800);       //transfer data to UART

                if (result == true)
                {
                    ui->lineEditReadDataESC->setText(data_read.toHex());
                    ui->lineEditReadDataCountESC->setText(QString("%1").arg(data_read.length()));

                    gp_DSI3Master->Log(LOG_TYPE_UART2_RX, data_read.toHex());

                    QBAfromRXRDUM(data_read);

                    gp_DSI3Master->Log(LOG_TYPE_UART1_RX, data_read.toHex());

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

        FUNCTION("SPIWriteRead", "Write and read SSI command to UART Interface "
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QString data_read_string;
            char kac = 0;
            char first_byte;
            int crc_yes = 0;
            QByteArray rdata;
            QByteArray data_crc;
            QByteArray header;
            int tx_length;
            int rx_length;

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

            tx_length = data.length();

            if(!params.get("rx_length", &rx_length))      // not parsed
            {
                rx_length = ui->lineEditSPIReadDataCount->text().toInt();
            }
            else                                // parsed
            {
                ui->lineEditSPIReadDataCount->setText(QString("%1").arg(rx_length));
            }


            params.get("crc_yes", &crc_yes);

            if (data != "")
            {
                if (crc_yes == 1)               // add CRC
                {
                    //runAppCommand("SPICRC","","","","","");
                    //data = QByteArray::fromHex(ui->lineEditSSIWriteData->text().toLatin1().replace(" ",""));
                }

                gp_DSI3Master->Log(LOG_TYPE_SPI_TX, data.toHex());

                header.clear();
                header.append(char(0x01));  //SOH
                header.append(char(0x04));  //SPI command
                header.append(char((tx_length >> 24) & 0xFF));
                header.append(char((tx_length >> 16) & 0xFF));
                header.append(char((tx_length >> 8) & 0xFF));
                header.append(char((tx_length >> 0) & 0xFF));

                data_send.clear();
                data_send.append(header);
                data_send.append(data);


                QString test = data_send.toHex();

                data_read_string = runAppCommand("WriteRead", QString("data=%1").arg(test),"","","","").toString();      // transfer data

                if (data_read_string != "")     // if result is OK
                {
                    data_read = QByteArray::fromHex(data_read_string.toLatin1());

                    if ((data_read.at(0) == 0x01) && (data_read.at(1) == 0x06) && (data_read.at(data_read.length()-1) == 0x04))
                    {
                        data_read.remove(0, 2);
                        data_read.remove(data_read.length()-1, 1);
                        ui->lineEditSPIReadData->setText(data_read.toHex());
                        ui->lineEditSPIReadDataCount->setText(QString("%1").arg(data_read.length()));

                        gp_DSI3Master->Log(LOG_TYPE_SPI_RX, data_read.toHex());

                        /*data_crc = data_read;
                        data_crc.resize(6);

                        if (data_read.at(6) != CalculateCRC(data_crc))
                        {
                            LERROR("ASIC master RX SPI CRC is not matching");
                        }*/


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

        FUNCTION("SSIWriteRead", "Write and read SSI command to UART Interface "
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QString data_read_string;
            char kac = 0;
            char first_byte;
            int crc_yes = 0;
            QByteArray rdata;
            QByteArray data_crc;
            QByteArray header;
            int tx_length;
            int rx_length;

            // custom for write
            //if can't parse from parameter will take from LineEdit control

            if(!params.get("data", &data))      // not parsed
            {
                if ((ui->lineEditSSIWriteData->text().toLatin1().replace(" ","").length() % 2) == 0)
                    data = QByteArray::fromHex(ui->lineEditSSIWriteData->text().toLatin1().replace(" ",""));
                else
                    data.clear();
            }
            else                                // parsed
                ui->lineEditSSIWriteData->setText(data.toHex());

            tx_length = data.length();

            if(!params.get("rx_length", &rx_length))      // not parsed
            {
                rx_length = ui->lineEditSSIReadDataCount->text().toInt();

            }
            else                                // parsed
            {
                ui->lineEditSSIReadDataCount->setText(QString("%1").arg(rx_length));
            }


            params.get("crc_yes", &crc_yes);

            if (data != "")
            {
                if (crc_yes == 1)               // add CRC
                {
                    //runAppCommand("SPICRC","","","","","");
                    //data = QByteArray::fromHex(ui->lineEditSSIWriteData->text().toLatin1().replace(" ",""));
                }

                gp_DSI3Master->Log(LOG_TYPE_SSI_TX, data.toHex());

                header.clear();
                header.append(char(0x01));  //SOH
                header.append(char(0x03));  //SSI command
                header.append(char((tx_length >> 24) & 0xFF));
                header.append(char((tx_length >> 16) & 0xFF));
                header.append(char((tx_length >> 8) & 0xFF));
                header.append(char((tx_length >> 0) & 0xFF));
                header.append(char((rx_length >> 24) & 0xFF));
                header.append(char((rx_length >> 16) & 0xFF));
                header.append(char((rx_length >> 8) & 0xFF));
                header.append(char((rx_length >> 0) & 0xFF));

                data_send.clear();
                data_send.append(header);
                data_send.append(data);


                QString test = data_send.toHex();

                data_read_string = runAppCommand("WriteRead", QString("data=%1").arg(test),"","","","").toString();      // transfer data

                if (data_read_string != "")     // if result is OK
                {
                    data_read = QByteArray::fromHex(data_read_string.toLatin1());

                    if ((data_read.at(0) == 0x01) && (data_read.at(1) == 0x06) && (data_read.at(data_read.length()-1) == 0x04))
                    {
                        data_read.remove(0, 2);
                        data_read.remove(data_read.length()-1, 1);
                        ui->lineEditSSIReadData->setText(data_read.toHex());
                        ui->lineEditSSIReadDataCount->setText(QString("%1").arg(data_read.length()));

                        gp_DSI3Master->Log(LOG_TYPE_SSI_RX, data_read.toHex());

                        /*data_crc = data_read;
                        data_crc.resize(6);

                        if (data_read.at(6) != CalculateCRC(data_crc))
                        {
                            LERROR("ASIC master RX SPI CRC is not matching");
                        }*/


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


            ui->lineEditSSIReadData->setText("");
            ui->lineEditSSIReadDataCount->setText(QString("0"));
            return QString("");
        }

        /*FUNCTION("PerformBRC", "Perform BRC command to UART interface"
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
        }*/

        FUNCTION("PerformMeasurement", "Perform measurement and BRCs command"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QString result = "";
            QString str_command;

            int ssi_channel;
            int ssi_meas;
            int ssi_read;
            int ssi_select;
            int ssi_slots;
            int ssi_brc_count;
            int ssi_meas_brc_delay;
            int ssi_brc_brc_delay;
            int ssi_brc_get_delay;
            int ssi_get_count;
            int ssi_get_bytes;
            int ssi_get_get_delay;
            int byte;

            QByteArray command;
            //QByteArray data_read;

            ssi_channel = SSI_CHANNEL.value();
            ssi_meas = SSI_MEAS.value();
            ssi_read = SSI_READ.value();
            ssi_brc_count = SSI_BRC_COUNT.value();
            ssi_slots = SSI_SLOT_COUNT.value();
            ssi_meas_brc_delay = SSI_MEAS_BRC_DELAY.value();
            ssi_brc_brc_delay = SSI_BRC_BRC_DELAY.value();
            ssi_brc_get_delay = SSI_BRC_GET_DELAY.value();
            ssi_get_count = SSI_GET_COUNT.value();
            ssi_get_bytes = SSI_GET_BYTES.value();
            ssi_get_get_delay = SSI_GET_GET_DELAY.value();


            if (ui->checkBoxUpdateGABauto->isChecked())
            {
                ssi_brc_get_delay = ssi_meas_brc_delay + ssi_brc_brc_delay + 1000;
                ssi_get_get_delay = ssi_brc_brc_delay + 100;
                ssi_get_count = ssi_brc_count;
                ssi_get_bytes = (ssi_slots * (18 + 2)) + 1;

                SSI_BRC_GET_DELAY.setValue(ssi_brc_get_delay);
                SSI_GET_GET_DELAY.setValue(ssi_get_get_delay);
                SSI_GET_COUNT.setValue(ssi_get_count);
                SSI_GET_BYTES.setValue(ssi_get_bytes);
            }


            if (gp_UartSsiController->SSIREGX03C.value("[INTERFACE_SEL]") <= 2)     // SSI
            {
                ssi_select = 1;
            }
            else
            {
                ssi_select = 0;
            }

            command.clear();
            command.append(char(0x01));     // SOH
            command.append(char(0x06));     // MEAS command
            command.append((ssi_meas_brc_delay >> 8) & 0xFF);
            command.append((ssi_meas_brc_delay >> 0) & 0xFF);
            command.append((ssi_brc_count) & 0xFF);
            command.append((ssi_brc_brc_delay >> 8) & 0xFF);
            command.append((ssi_brc_brc_delay >> 0) & 0xFF);
            command.append((ssi_brc_get_delay >> 8) & 0xFF);
            command.append((ssi_brc_get_delay >> 0) & 0xFF);
            command.append((ssi_get_count) & 0xFF);
            command.append((ssi_get_bytes >> 8) & 0xFF);
            command.append((ssi_get_bytes >> 0) & 0xFF);
            command.append((ssi_get_get_delay >> 8) & 0xFF);
            command.append((ssi_get_get_delay >> 0) & 0xFF);
            command.append(char(0x0A));
            byte = ((ssi_select & 0x01) << 0) | ((ssi_channel & 0x01) << 1) | ((ssi_read & 0x01) << 2) | ((ssi_meas & 0x01) << 3);
            command.append(byte & 0xFF);

            str_command = command.toHex();
            result = runAppCommand("WriteRead", QString("data=%1").arg(str_command),"","","","").toString();       //transfer data to UART

            //RX_Length = Brc * (Slots) * (Length + 2);
            //SET_REG("BRC_RESP_LENGTH", RX_Length);

            if (result != "")   //if result is OK
            {
                data_read = QByteArray::fromHex(result.toLatin1());

                if ((data_read.at(0) == 0x01) && (data_read.at(1) == 0x06))
                {
                    data_read.remove(0, 2);
                    data_read.remove(data_read.length()-1, 1);
                    ui->lineEditBRCReadData->setText(data_read.toHex());
                    ui->lineEditBRCReadDataCount->setText(QString("%1").arg(data_read.length()));
                    //gp_BRC->runAppCommand("SetBRCData", QString("data=%1").arg(QString(data_read.toHex())),"","","","");

                    if (ssi_read == 1)
                    {
                        data_read.prepend(char(0x01));
                        gp_Measurement->SetBRCReadData(data_read.toHex());
                    }

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
            SSIREGX040.fromByteArray(DEMUX.toByteArray());
            runAppCommand("WriteREGX", "SSIREGX040", "", "", "", "");
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
            quint32 addr_wr;

            if(GetSSIREGXPointer(p1.toString(), &preg))
            {
                QByteArray regData = preg->toByteArray();       // BYTE3,BYTE2,BYTE1,BYTE0

                if ( preg->extra("addr") != "")
                {
                    addr_wr = (quint32)preg->extra("addr").toUInt(0,10);
                    addr_wr = addr_wr + 0x30000000;
                }
                else if (p1.toString().contains("SSIADDRMEM"))
                {
                    addr_wr = GET_REG("SSIADDRMEM[31:0]").toInt();
                }
                else
                {
                    addr_wr = GET_REG("SSIADDR").toInt();
                    addr_wr = addr_wr + 0x30000000;
                }

                command.clear();
                command.append(char(0x01));     // SOH
                command.append(char(0x02));     // WRITE command
                command.append((addr_wr >> 24) & 0xFF);
                command.append((addr_wr >> 16) & 0xFF);
                command.append((addr_wr >> 8) & 0xFF);
                command.append((addr_wr >> 0) & 0xFF);
                command.append(regData.at(3));
                command.append(regData.at(2));
                command.append(regData.at(1));
                command.append(regData.at(0));

                cmd = command.toHex();
                resp = runAppCommand("WriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();

                if (resp != "")
                {
                    resp = QByteArray::fromHex(resp);
                    if (resp.at(1) == 0x01)
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
            quint32 addr;

            if (GetSSIREGXPointer(p1.toString(), &preg))
            {
                if ( preg->extra("addr") != "")
                {
                    addr = (quint32)preg->extra("addr").toUInt(0,10);
                    addr = addr + 0x30000000;
                }
                else if (p1.toString().contains("SSIADDRMEM"))
                {
                    addr = GET_REG("SSIADDRMEM[31:0]").toInt();
                }
                else
                {
                    addr = GET_REG("SSIADDR").toInt();
                    addr = addr + 0x30000000;
                }




                command.clear();
                command.append(char(0x01));     // SOH
                command.append(char(0x01));     // READ command
                command.append((addr >> 24) & 0xFF);
                command.append((addr >> 16) & 0xFF);
                command.append((addr >> 8) & 0xFF);
                command.append((addr >> 0) & 0xFF);

                cmd = command.toHex();

                regdata = runAppCommand("WriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();

                if (regdata.size() >= 14)
                {
                    regdata = QByteArray::fromHex(regdata);

                    rdata.clear();
                    rdata.append(regdata.at(5));
                    rdata.append(regdata.at(4));
                    rdata.append(regdata.at(3));
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
        FUNCTION("WriteREGXs", "Write into register in UART2SSI bridge"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            runAppCommand("WriteREGX", "SSIREGX000", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX004", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX008", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX00C", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX010", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX014", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX018", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX01C", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX020", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX024", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX028", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX02C", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX030", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX034", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX038", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX03C", "", "", "", "");
            runAppCommand("WriteREGX", "SSIREGX040", "", "", "", "");
        }

        FUNCTION("ReadREGXs", "Read register from UART2SSI bridge"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            uint timestamp_unix;
            QByteArray user;
            QDateTime myDateTime;
            QString temp;

            runAppCommand("ReadREGX", "SSIREGX000", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX004", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX008", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX00C", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX010", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX014", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX018", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX01C", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX020", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX024", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX028", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX02C", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX030", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX034", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX038", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX03C", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX040", "", "", "", "");

            runAppCommand("ReadREGX", "SSIREGX1EC", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX1F0", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX1F4", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX1F8", "", "", "", "");
            runAppCommand("ReadREGX", "SSIREGX1FC", "", "", "", "");

            timestamp_unix = SSIREGX1F0.value(0, 31);
            myDateTime.setTime_t(timestamp_unix);
            ui->labelSYNTH_TIME->setText(myDateTime.toString());

            user.append(SSIREGX1F4.toByteArray());
            user.append(SSIREGX1F8.toByteArray());
            ui->labelUSER->setText(QString::fromAscii(user));

            ui->labelSVN_REV->setText(QString("%1").arg(SSIREGX1EC.value(0,15)));
        }
        FUNCTION("CRCtst", "Read register from UART2SSI bridge"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QByteArray tst;
            uint cr;
            tst.clear();



            tst.append(0x7F);
            tst.append(0xC8);
            tst.append(0x47);
            tst.append(0x13);
            tst.append(0x88);
            tst.append(0xEB);
            tst.append(0xBE);
            tst.append(0x68);
            tst.append(0x64);
            tst.append(0x6D);
            tst.append(0x34);
            tst.append(0x86);
            tst.append(0x2A);
            tst.append(0xE0);
            tst.append(0x66);
            tst.append(0x7D);
            tst.append(0x8C);
            tst.append(0x0A);
            tst.append(0xA4);
            tst.append(0xA8);
            tst.append(0x19);
            tst.append(0x68);
            tst.append(0xFA);
            tst.append(0xB5);



            tst.clear();

            tst.append(char(0x00));
            tst.append(char(0x00));

            tst.append(0xC8);
            tst.append(0x7F);

            tst.append(0x13);
            tst.append(0x47);


            tst.append(0xEB);
            tst.append(0x88);


            tst.append(0x68);
            tst.append(0xBE);


            tst.append(0x6D);
            tst.append(0x64);


            tst.append(0x86);
            tst.append(0x34);


            tst.append(0xE0);
            tst.append(0x2A);


            tst.append(0x7D);
            tst.append(0x66);


            tst.append(0x0A);
            tst.append(0x8C);


            tst.append(0xA8);
            tst.append(0xA4);


            tst.append(0x68);
            tst.append(0x19);


            tst.append(0xB5);
            tst.append(0xFA);

            cr = CalculateCRC16UPAEwithSEEDfromFPGA(0xFFFF, tst);
            //ui->lineEditCRCres->setText(QString("%1").arg(cr));


        }
        FUNCTION("CalcScram", "Read register from UART2SSI bridge"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {

            Calculate_scramble();
        }

    return false;
}


void UartSsiController::setupBoard()
{
}


void UartSsiController::suspend()
{       
}

void UartSsiController::resume()
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


char UartSsiController::CalculateCRC(QByteArray data)
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

char UartSsiController::CalculateUPAECRC(QByteArray data)
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

char UartSsiController::CalculateUPAECRCwithSEED(char seed, QByteArray data)
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

quint16 UartSsiController::CalculateUPAECRC16withSEED(quint16 seed, QByteArray data)
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



quint16 UartSsiController::CalculateCRC16UPAEwithSEEDfromFPGA(quint16 seed, QByteArray data)
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
bool UartSsiController::GetSSIREGXPointer(QString name, Register **preg)
{
    uint address;
    bool result = false;

    if (name.contains("SSIREGX"))
    {
        result = true;

        name.remove("SSIREGX");
        address = name.toUInt(0, 16);

        switch (address)
        {
        case 0:
           *preg = &SSIREGX000;
            break;
        case 4:
           *preg = &SSIREGX004;
            break;
        case 8:
           *preg = &SSIREGX008;
            break;
        case 12:
           *preg = &SSIREGX00C;
            break;
        case 16:
           *preg = &SSIREGX010;
            break;
        case 20:
           *preg = &SSIREGX014;
            break;
        case 24:
           *preg = &SSIREGX018;
            break;
        case 28:
           *preg = &SSIREGX01C;
            break;
        case 32:
           *preg = &SSIREGX020;
            break;
        case 36:
           *preg = &SSIREGX024;
            break;
        case 40:
           *preg = &SSIREGX028;
            break;
        case 44:
           *preg = &SSIREGX02C;
            break;
        case 48:
           *preg = &SSIREGX030;
            break;
        case 52:
           *preg = &SSIREGX034;
            break;
        case 56:
           *preg = &SSIREGX038;
            break;
        case 60:
           *preg = &SSIREGX03C;
            break;
        case 64:
           *preg = &SSIREGX040;
            break;
        case 492:
           *preg = &SSIREGX1EC;
            break;
        case 496:
           *preg = &SSIREGX1F0;
            break;
        case 500:
           *preg = &SSIREGX1F4;
            break;
        case 504:
           *preg = &SSIREGX1F8;
            break;
        case 508:
           *preg = &SSIREGX1FC;
            break;
        default:
            result = false;
            break;
        }
    }
    else if (name.contains("SSIADDRX"))
    {
        name.remove("SSIADDRX");
        address = name.toUInt(0, 16);
        SET_REG("SSIADDR", address & 0xFF);
        *preg = &SSIREGADDR;
        result = true;
    }
    else if (name.contains("SSIADDRMEM"))
    {
        *preg = &SSIREGADDRMEM;
        result = true;
    }
    else if (name.contains("SSIADDR"))
    {
        *preg = &SSIREGADDR;
        result = true;
    }
    return result;
}

void UartSsiController::QBAtoTXRDUM(QByteArray &data)
{
    int i;
    QByteArray replacement;
    QByteArray header;
    replacement[0] = 0x10;

    header = data.mid(0, 2);
    data.remove(0, 2);
    for (i = 0; i < m_escapedcharsRDUM.length(); i++)
    {
        replacement[1] = m_escapedreplacementRDUM.at(i);
        data.replace(m_escapedcharsRDUM.at(i), replacement);
    }
    data.prepend(header);
}

void UartSsiController::QBAfromRXRDUM(QByteArray &data)
{
    int i;
    QByteArray newvalue;
    QByteArray replacement;
    QByteArray header;
    replacement[0] = 0x10;

    header = data.mid(0, 2);
    data.remove(0, 2);
    for (i = (m_escapedcharsRDUM.length() - 1); i > - 1; i--)
    {
        replacement[1] = m_escapedreplacementRDUM.at(i);
        newvalue[0] = m_escapedcharsRDUM.at(i);
        data.replace(replacement, newvalue);
    }
    data.prepend(header);
}

bool UartSsiController::WriteNVMreg(quint32 address, quint32 data)
{
    quint32 addr_wr;
    QByteArray command;
    QString cmd;
    QByteArray response;

    addr_wr = (4 * address) + 0x31000000;

    command.clear();
    command.append(char(0x01));     // SOH
    command.append(char(0x02));     // WRITE command
    command.append((addr_wr >> 24) & 0xFF);
    command.append((addr_wr >> 16) & 0xFF);
    command.append((addr_wr >> 8) & 0xFF);
    command.append((addr_wr >> 0) & 0xFF);
    command.append((data >> 24) & 0xFF);
    command.append((data >> 16) & 0xFF);
    command.append((data >> 8) & 0xFF);
    command.append((data >> 0) & 0xFF);

    cmd = command.toHex();
    response = runAppCommand("WriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();

    if (response != "")
    {
        response = QByteArray::fromHex(response);
        if (response.at(1) == 0x01)
        {
            //LERROR("Write command was not successful, address may be wrong");
            return false;
        }
        return true;
    }

    else
        return false;

}

bool UartSsiController::ReadNVMreg(quint32 address, quint32 *data)
{
    QByteArray command;
    QByteArray response;
    QString cmd;
    quint32 addr;

    addr = (4*address) + 0x31000000;

    command.clear();
    command.append(char(0x01));     // SOH
    command.append(char(0x01));     // READ command
    command.append((addr >> 24) & 0xFF);
    command.append((addr >> 16) & 0xFF);
    command.append((addr >> 8) & 0xFF);
    command.append((addr >> 0) & 0xFF);

    cmd = command.toHex();

    response = runAppCommand("WriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();

    if (response != "")
    {
        response = QByteArray::fromHex(response);
        if (response.length() == 7)
        {
            *data = ((quint32)response.at(5))&0xFF;
            *data = *data + ((((quint32)response.at(4)) & 0xFF) << 8);
            *data = *data + ((((quint32)response.at(3)) & 0xFF) << 16);
            *data = *data + ((((quint32)response.at(2)) & 0xFF) << 24);
        }

        //qDebug()<< response.toHex();
        return true;
    }
    return false;
}


quint16 polynom_0 = 0x8011;
quint16 polynom_1 = 0x8081;
quint16 polynom_2 = 0x8003;

quint16 scrambler_lut[256+11] = {
  0x2145, 0x3625, 0x7501, 0x5481, 0x1CC3, 0x2140, 0x5D41, 0x045F,
  0x4915, 0x72B6, 0x0EB3, 0x727E, 0x1588, 0x7CF3, 0x2985, 0x5875,
  0x36E4, 0x21F5, 0x0698, 0x3A88, 0x5658, 0x5A7E, 0x400E, 0x7303,
  0x0C5C, 0x07A6, 0x63AD, 0x193B, 0x0653, 0x55E9, 0x4FDD, 0x6933,
  0x653E, 0x5F65, 0x5D0C, 0x11B2, 0x6BC9, 0x6150, 0x4244, 0x0F3F,
  0x2D52, 0x6750, 0x5F56, 0x0D2C, 0x48FB, 0x3A62, 0x38FA, 0x5EFD,
  0x3785, 0x4960, 0x1CF6, 0x78BB, 0x33E7, 0x79D9, 0x0EAC, 0x4EE3,
  0x6D59, 0x058B, 0x7397, 0x30AE, 0x09C0, 0x6EDD, 0x309C, 0x039C,
  0x2F76, 0x04EA, 0x0DA6, 0x7A30, 0x18DD, 0x07A4, 0x0426, 0x3C99,
  0x4065, 0x758C, 0x44B7, 0x45D9, 0x73AD, 0x15C0, 0x0C40, 0x58EB,
  0x39C5, 0x0D20, 0x6C3F, 0x0CDB, 0x36EB, 0x3094, 0x72D8, 0x454F,
  0x477A, 0x530A, 0x1F0F, 0x02D4, 0x05D5, 0x7449, 0x5FA2, 0x0084,
  0x748E, 0x70AD, 0x3AF7, 0x5281, 0x0847, 0x6FA1, 0x09D4, 0x138D,
  0x256D, 0x498C, 0x2675, 0x2944, 0x7C5F, 0x0883, 0x6215, 0x5899,
  0x0F5A, 0x4B8F, 0x0848, 0x1203, 0x6149, 0x5E81, 0x3BF1, 0x4F55,
  0x1CC0, 0x285A, 0x2E75, 0x5318, 0x1DED, 0x0DC0, 0x34C5, 0x024A,
  0x3093, 0x210B, 0x605D, 0x5814, 0x5214, 0x3F11, 0x1976, 0x303A,
  0x4296, 0x048D, 0x4332, 0x7380, 0x5ECE, 0x3DE1, 0x1B47, 0x4EF0,
  0x4EDD, 0x375A, 0x5AED, 0x1A62, 0x2F35, 0x4D80, 0x622E, 0x02D5,
  0x5441, 0x29AD, 0x26BD, 0x33A7, 0x6CD4, 0x6347, 0x6658, 0x28BC,
  0x3F6B, 0x445D, 0x5963, 0x18B0, 0x1E93, 0x49A7, 0x40DA, 0x07A9,
  0x69F3, 0x5A92, 0x5583, 0x0BBD, 0x5714, 0x5B0B, 0x0AD9, 0x46BC,
  0x0F42, 0x3A35, 0x0E5A, 0x5317, 0x4E6A, 0x0A85, 0x3633, 0x7CC2,
  0x1077, 0x47C3, 0x1EFD, 0x543D, 0x19C1, 0x7B92, 0x7DA4, 0x7490,
  0x548A, 0x61D6, 0x43C6, 0x5F4F, 0x3133, 0x387D, 0x0B14, 0x5B20,
  0x42E5, 0x2269, 0x0834, 0x410B, 0x4DBD, 0x78D8, 0x6C4A, 0x0AA7,
  0x00DB, 0x39DC, 0x44DB, 0x2540, 0x4338, 0x6A3D, 0x47B6, 0x4140,
  0x7144, 0x3E70, 0x4F13, 0x73BB, 0x1959, 0x5D95, 0x7467, 0x2113,
  0x0CCA, 0x4FF2, 0x6EC3, 0x314E, 0x7BDF, 0x5CC3, 0x4DD7, 0x1D34,
  0x7BE0, 0x24C4, 0x7DCF, 0x13E8, 0x3F20, 0x3D70, 0x3422, 0x6DEA,
  0x5560, 0x18AD, 0x3A0F, 0x217F, 0x0A78, 0x45B7, 0x4561, 0x518A,
  0x13A5, 0x076F, 0x23C5, 0x2771, 0x1E6A, 0x6EB5, 0x0522, 0x52E9,

  0x0000,

  0x3625, 0x7501, 0x5481, 0x1CC3, 0x2140, 0x4C3D, 0x5D41, 0x045F,
  0x718A, 0x2145,
};

void UartSsiController::xor_init(int seed_id, int crc) {
  int idx_add = 0;

  if ((crc & 0x04) == 0x04 ) idx_add |= 0x40;
  if ((crc & 0x01) == 0x01 ) idx_add |= 0x10;
  if ((crc & 0x80) == 0x80 ) idx_add |= 0x04;
  if ((crc & 0x01) == 0x01 ) idx_add |= 0x01;

  if (seed_id < 256)
    scrambler = scrambler_lut[(seed_id + idx_add)%256];
  else
    scrambler = scrambler_lut[seed_id];

  int poly = 0;

  if ((crc & 0x40) == 0x40) polynom = polynom_2;
  if ((crc & 0x40) != 0x40 && (crc & 0x02) == 0x02) polynom = polynom_1;
  if ((crc & 0x40) != 0x40 && (crc & 0x02) != 0x02) polynom = polynom_0;
}

void UartSsiController::xor_tick() {
  int new_bit = 0;

  // Process copied byte. In first cycle compute seed (append 0x00 before message)
  for(int b = 0; b < 15; b++)
    if( (scrambler & polynom & 1<<b) == 1<<b)
      new_bit ^= 1;

  scrambler = scrambler >> 1;
  if (new_bit) scrambler |= 1<<14;
}

int UartSsiController::next_xor_data(int crc) {
  int result = 0;
  int steps = 4;

  if ((crc & 0x08) != 0x08 && (crc & 0x20) == 0x20) steps = 6;
  if ((crc & 0x08) != 0x08 && (crc & 0x20) != 0x20) steps = 5;

  result = (scrambler>>11);
  result <<= 4;


  for(int i = 0; i < steps; i++)
    xor_tick();

  result |= (scrambler>>11);

  for(int i = 0; i < steps; i++)
    xor_tick();

  return result;
}

// Test it
int UartSsiController::Calculate_scramble(void) {
  //uint8_t *input;
  quint8 output[48];
  quint16 a;

  int cnt = 48;

  quint8 input[48] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  //a = ui->lineEdit_index->text().toInt();

  xor_init(a, 0x48);

  for(int i = 0; i< cnt; i++) {
      output[i] = input[i] ^ next_xor_data(0x48);
  }

  cnt = 5;

  return 0;
}

