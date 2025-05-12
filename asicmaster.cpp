#include "asicmaster.h"
#include "ui_asicmaster.h"
#include "mainapp.h"
#include "uartuartcontroller.h"
#include "uartspicontroller.h"
#include "dsi3slave.h"
#include "dsi3master.h"

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

#define NO_OPERATION        (1)
#define WRITE_BYADDR        (2)
#define READ_BYADDR         (3)
#define GET_CRM_RESP        (4)
#define GET_PDCM_RESP       (5)
#define SPI_CMD_COLUMN      (2)
#define SPI_ADDRESS_COLUMN  (3)


ASICMaster *gp_ASICMaster;

ASICMaster::ASICMaster(QWidget *parent) :
    Applet(parent)
{
    ui = new Ui::ASICMaster;
    ui->setupUi(this);
    createCustomUi();
    setWindowTitle("ASIC Master");
    setWindowIcon(QIcon(":/board/onlogo.ico"));
    m_current.clear();
    m_x.clear();
    m_y.clear();
    m_y_uint.clear();
    m_timesample = 0;
}

ASICMaster::~ASICMaster()
{
    delete ui;
}

bool ASICMaster::initialize()
{
    REGX00.makeSetup("@register=REGX00 @addr=0 @purpose=Config;"     //addr:0x00
                  "CRM_RESP_END_TIME[7:0]#Minimum time between end of one CRM transfer and start of next one (in Tchips);"
                  "_1[11:8];"
                  "PDCM_RESP_FRAME_LEN[16:12]#Number of bytes in each BRC slot;"
                  "_2[18:17];"
                  "CLKIN_FREQ_DET[22:19]#Information about clock connected to CLKIN;"
                  "_3;"
                  "PDCM_RESP_START_TIME[29:24]#Time between 1st edge of BRC pulse and start of response in 1st slot (in Tchips);"
                  "TCHIP#Select slave to master communication speed (0 is 3us, 1 is 4us);"
                  "CRC16#0 - CRC8 used, 1 - CRC16 used for PDCM data;"
            );
    addDataNode(&REGX00);
    REGX01.makeSetup("@register=REGX01 @addr=1 @purpose=Config;"     //addr:0x01
                  "MAX_CRM_RESP_START_TIME[11:0]#Time between 1st edge of CRM request and start of CRM response (in Tchips);"
                  "CLKIN_FREQ[17:12]#Information about clock connected to CLKIN;"
                  "HW_INT_MASK#Masking of interrupt source;"
                  "DATA_AMP#Selects amplitude of master to slave communication (0 is 3Vpp, 1 is 4Vpp);"
                  "DATA_REF#Selects reference voltage for DSI3 communication (0 is GND, 1 isVBAT/2);"
                  "SLAVE_RX_FILTER_ENA[22:21]#Select filter for de-shaping of slave signal;"
                  "VSUPOV_MASK#Disabling of VSUPOV detection;"
                  "REVISION_ID[31:24]#Revision ID of ASIC;"
            );
    addDataNode(&REGX01);
    REGX02.makeSetup("@register=REGX02 @addr=2 @purpose=Config;"     //addr:0x02
                  "N_FRAMES[3:0];"
                  "SCRAMBLER_ENA#1 - Apply descrambler on received data form slave nodes;"
                  "BUS_DRIVER_WEAK_ENA;"
                  "APPNRM;"
                  "TIMER[15:7];"
                  "NUM_OF_PDCM_SLOTS[18:16]#Number of slots to receive in BRC transfer;"
                  "CRM_INT_MASK#Masking of interrupt source;"
                  "PDCM_INT_MASK[27:20]#Masking of interrupt source;"
                  "BUS_DRIVER_ENA#Switches bus driver on/off;"
                  "BURST_ENA#Trigger BURST transfer;"
                  "BRC_ENA#Trigger BRC transfer;"
                  "TX_START#Edge sensitive bit to trigger operation specified by (BRC_ENA, BURST_ENA, APPNRM, APPDLY);"
            );
    addDataNode(&REGX02);
    REGX03.makeSetup("@register=REGX03 @addr=3 @purpose=TXData;"
                  "DATA[31:0]#CRM/BURST transmit data buffer;"
            );
    addDataNode(&REGX03);
    REGX04.makeSetup("@register=REGX04 @addr=4 @purpose=TXData;"
                  "DATA[31:0]#CRM/BURST transmit data buffer;"
            );
    addDataNode(&REGX04);
    REGX05.makeSetup("@register=REGX05 @addr=5 @purpose=TXData;"
                  "DATA[31:0]#CRM/BURST transmit data buffer;"
            );
    addDataNode(&REGX05);
    REGX06.makeSetup("@register=REGX06 @addr=6 @purpose=TXData;"
                  "DATA[31:0]#CRM/BURST transmit data buffer;"
            );
    addDataNode(&REGX06);
    REGX07.makeSetup("@register=REGX07 @addr=7 @purpose=TXData;"
                  "DATA[31:0]#CRM/BURST transmit data buffer;"
            );
    addDataNode(&REGX07);
    REGX0A.makeSetup("@register=REGX0A @addr=10 @purpose=CTM;"
                  "CTM_START[11:0]#Sets delay between SPI write command with TX_start is 1 to start of current sampling. 1LSB is 6.4us at typical oscillator;"
                  "_1[15:12];"
                  "CTM_DIV[17:16]#Selects downsampling of observed current;"
                  "_2[23:18];"
                  "CTM_SEL[27:24]#Selects data for CTM module to sample;"
                  "_3[29:28];"
                  "CTM_ENA_CHB#Enables sampling of CHB current into CHA shadow memory;"
                  "CTM_ENA_CHA#Enables sampling of CHA current into CHB shadow memory;"
            );
    addDataNode(&REGX0A);
    REGX0D.makeSetup("@register=REGX0D @addr=13 @purpose=Status;"
                  "PDCM_SLOT1_DUR[15:0];"
                  "PDCM_SLOT2_DUR[31:16];"
            );
    addDataNode(&REGX0D);
    REGX0E.makeSetup("@register=REGX0E @addr=14 @purpose=Status;"
                  "PDCM_SLOT3_DUR[15:0];"
                  "PDCM_SLOT4_DUR[31:16];"
            );
    addDataNode(&REGX0E);
    REGX0F.makeSetup("@register=REGX0F @addr=15 @purpose=Status;"
                  "PDCM_SLOT5_DUR[15:0];"
                  "PDCM_SLOT6_DUR[31:16];"
            );
    addDataNode(&REGX0F);
    REGX10.makeSetup("@register=REGX10 @addr=16 @purpose=Status;"
                  "PDCM_SLOT7_DUR[15:0];"
                  "PDCM_SLOT8_DUR[31:16];"
            );
    addDataNode(&REGX10);
    REGX11.makeSetup("@register=REGX11 @addr=17 @purpose=Status;"
                  "HWR#ASIC went through reset. All configuration was set to POR values;"
                  "GNDL#Deglitcher on GNDL comparator;"
                  "OSCFAIL#Deglitcher on OSC FAIL comparator;"
                  "OSCLCK_FAIL#Lock to external clock reference is not successful;"
                  "TSD#Set when TEMP > TSD_THR;"
                  "TW#Set when TEMP > TW_THR;"
                  "BSTFAIL#Or of all flags in BSTFAIL register;"
                  "_1;"
                  "OVC1B#Comparison of ADC result with calculated threshold;"
                  "OVC1T#Comparison of ADC result with calculated threshold;"
                  "OVC2B#Comparison of ADC result with calculated threshold;"
                  "OVC2T#Comparison of ADC result with calculated threshold;"
                  "VSUPUV#VBSTUV in UPIDv2<br>VSUPUV in UPIDv1<br>Comparison of ADC result with threshold from EEPROM;"
                  "VSUPOV#VBSTOV in UPIDv2<br>VSUPOV in UPIDv1<br>Comparison of ADC result with threshold from EEPROM;"
                  "VIOUV#Comparison of ADC result with threshold from EEPROM;"
                  "VIOOV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDUV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDOV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDA1UV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDA1OV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDA2UV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDA2OV#Comparison of ADC result with threshold from EEPROM;"
                  "VDIGUV#VDRVUV in UPIDv2<br>VDIGUV in UPIDv1<br>Comparison of ADC result with threshold from EEPROM;"
                  "VDIGOV#VDRVOV in UPIDv2<br>VDIGOV in UPIDv1<br>Comparison of ADC result with threshold from EEPROM;"
                  "VDDAUV;"
                  "OVCR1B#Bus overcurrent towards GND  detected by receiver ADC;"
                  "OVCR1T#Bus overcurrent towards VSUP  detected by receiver ADC;"
                  "OVCR2B#Bus overcurrent towards GND  detected by receiver ADC;"
                  "OVCR2T#Bus overcurrent towards VSUP  detected by receiver ADC;"
                  "OVCPW1#Overcurrent detection on power phase switch;"
                  "OVCPW2#Overcurrent detection on power phase switch;"
                  "HW_ERROR;"
            );
    addDataNode(&REGX11);
    REGX12.makeSetup("@register=REGX12 @addr=18 @purpose=Status;"
                  "PDCM_OR_CRM_CRC_FAIL[7:0];"
                  "PDCM_RECEIVED[15:8];"
                  "CRM_RECEIVED#All sheduled BRCs were performed;"
                  "TIMER_INT_RECEIVED;"
                  "BURST_DONE#Whole BURST command was sent to bus;"
                  "HW_ERROR#Copy of REGX11.HW_ERROR;"
                  "RX_TO#There was at least 5 chips withou edge detected on bus;"
                  "FRAME_TO#Response for CRM command was not detected on bus;"
                  "SYMBOL_ERR#Unknown symbol on bus detected;"
                  "CLKIN_CNT[31:23]#Counter running on external clock - CLKIN;"
            );
    addDataNode(&REGX12);
    REGX13.makeSetup("@register=REGX13 @addr=19 @purpose=Status;"
                  "VDIG[8:0]#VDRV in UPIDv2<br>VDIG in UPIDv1<br>Result of ADC measurement;"
                  "_1;"
                  "VDD[18:10]#Result of ADC measurement;"
                  "_2;"
                  "VSUP[28:20]#VBST in UPIDv2<br>VSUP in UPIDv1<br>Result of ADC measurement;"
                  "_3[31:29];"
            );
    addDataNode(&REGX13);
    REGX14.makeSetup("@register=REGX14 @addr=20 @purpose=Status;"
                  "VIO[8:0]#Result of ADC measurement;"
                  "_1;"
                  "VDATA2[18:10]#Result of ADC measurement;"
                  "_2;"
                  "VDATA1[28:20]#Result of ADC measurement;"
                  "_3[31:29];"
            );
    addDataNode(&REGX14);
    REGX15.makeSetup("@register=REGX15 @addr=21 @purpose=Status;"
                  "VDDA_D2[8:0]#Result of ADC measurement;"
                  "_1;"
                  "VDDA_D1[18:10]#Result of ADC measurement;"
                  "_2;"
                  "TEMPERATURE_TJ[28:20]#Result of ADC measurement;"
                  "_3[31:29];"
            );
    addDataNode(&REGX15);
    REGX16.makeSetup("@register=REGX16 @addr=22 @purpose=Status;"
                  "IDATA2[7:0]#Result of ADC measurement;"
                  "IDATA1[15:8]#Result of ADC measurement;"
                  "_1[31:16];"
            );
    addDataNode(&REGX16);
    REGX20.makeSetup("@register=REGX20 @addr=32 @purpose=RXData;"
                  "DATA[31:0]#RX data;"
            );
    addDataNode(&REGX20);
    REGX21.makeSetup("@register=REGX21 @addr=33 @purpose=RXData;"
                  "DATA[31:0]#RX data;"
            );
    addDataNode(&REGX21);
    REGX22.makeSetup("@register=REGX22 @addr=34 @purpose=RXData;"
                  "DATA[31:0]#RX data;"
            );
    addDataNode(&REGX22);
    REGX23.makeSetup("@register=REGX23 @addr=35 @purpose=RXData;"
                  "DATA[31:0]#RX data;"
            );
    addDataNode(&REGX23);
    REGX50.makeSetup("@register=REGX50 @addr=80 @purpose=ClearStatusErrors;"
                  "CLR_HWR;"
                  "CLR_GNDL;"
                  "CLR_OSCFAIL;"
                  "CLR_OSCLCK_FAIL;"
                  "CLR_TSD;"
                  "CLR_TW;"
                  "CLR_BSTFAIL;"
                  "ZERO;"
                  "CLR_OVC1B;"
                  "CLR_OVC1T;"
                  "CLR_OVC2B;"
                  "CLR_OVC2T;"
                  "CLR_VSUPUV#CLR_VBSTUV in UPIDv2<br>CLR_VSUPUV in UPIDv1<br>Clear by write bit;"
                  "CLR_VSUPOV#CLR_VBSTOV in UPIDv2<br>CLR_VSUPOV in UPIDv1<br>Clear by write bit;"
                  "CLR_VIOUV;"
                  "CLR_VIOOV;"
                  "CLR_VDDUV;"
                  "CLR_VDDOV;"
                  "CLR_VDDA1UV;"
                  "CLR_VDDA1OV;"
                  "CLR_VDDA2UV;"
                  "CLR_VDDA2OV;"
                  "CLR_VDIGUV#CLR_VDRVUV in UPIDv2<br>CLR_VDIGUV in UPIDv1<br>Clear by write bit;"
                  "CLR_VDIGOV#CLR_VDRVOV in UPIDv2<br>CLR_VDIGOV in UPIDv1<br>Clear by write bit;"
                  "CLR_VDDAUV;"
                  "CLR_OVCR1B;"
                  "CLR_OVCR1T;"
                  "CLR_OVCR2B;"
                  "CLR_OVCR2T;"
                  "CLR_OVCPW1#CLR_Overcurrent detection on power phase switch;"
                  "CLR_OVCPW2#CLR_Overcurrent detection on power phase switch;"
                  "ZERO;"
            );
    addDataNode(&REGX50);
    REGX51.makeSetup("@register=REGX51 @addr=81 @purpose=ClearStatusErrors;"
                  "ZERO[15:0];"
                  "CLR_CRM_RECEIVED;"
                  "ZERO;"
                  "CLR_BURST_DONE;"
                  "ZERO;"
                  "CLR_RX_TO;"
                  "CLR_FRAME_TO;"
                  "CLR_SYMBOL_ERR;"
                  "ZERO[31:23];"
            );
    addDataNode(&REGX51);

    REGADDR.makeSetup("@register=REGADDR;"
                  "DATA[31:0];"
            );
    addDataNode(&REGADDR);


    //CHANNEL.makeSetup("@variable=CHANNEL" "@value=0" "@min=0 @max=1");
    addDataNode(&gp_DSI3Master->CHANNEL);
    ADDR.makeSetup("@variable=ADDR" "@value=0" "@min=0 @max=255");
    addDataNode(&ADDR);
    SLOT_NUMBER.makeSetup("@variable=SLOT_NUMBER" "@value=1" "@min=0 @max=255");
    addDataNode(&SLOT_NUMBER);
    SLOT_COUNT.makeSetup("@variable=SLOT_COUNT" "@value=1" "@min=1 @max=8");
    addDataNode(&SLOT_COUNT);
    SLOT_LENGTH.makeSetup("@variable=SLOT_LENGTH" "@value=1" "@min=0 @max=31");
    addDataNode(&SLOT_LENGTH);
    SLOT_LENGTH_ALL.makeSetup("@variable=SLOT_LENGTH_ALL" "@value=1" "@min=0 @max=255");
    addDataNode(&SLOT_LENGTH_ALL);
    ctSID.makeSetup("@variable=ctSID" "@value=1" "@min=1 @max=6");
    addDataNode(&ctSID);
    /*UPIDTO.makeSetup("@variable=UPIDTO" "@value=3" "@min=0 @max=7" "@descr=POCL FPGA - versions 10.dec.2019 or older, UPID_v2 - versions 1.mar.2020 or newer");
    addDataNode(&UPIDTO);*/
    CLKIN.makeSetup("@variable=CLKIN" "@value=10" "@min=0 @max=100.1");
    addDataNode(&CLKIN);
    RecCLKIN_FREQ.makeSetup("@variable=RecCLKIN_FREQ" "@value=0" "@min=0 @max=63");
    addDataNode(&RecCLKIN_FREQ);
    RecCLKIN_FREQ_DET.makeSetup("@variable=RecCLKIN_FREQ_DET" "@value=0" "@min=0 @max=15");
    addDataNode(&RecCLKIN_FREQ_DET);
    CLKIN_ERR.makeSetup("@variable=CLKIN_ERR" "@value=0" "@min=-10000.1 @max=10000.1");
    addDataNode(&CLKIN_ERR);
    CHECK_CRC.makeSetup("@variable=CHECK_CRC" "@value=0" "@min=0 @max=255");
    addDataNode(&CHECK_CRC);
    CHECK_CRC_SHADOW.makeSetup("@variable=CHECK_CRC_SHADOW" "@value=0" "@min=0 @max=255");
    addDataNode(&CHECK_CRC_SHADOW);
    DATA_LENGTH.makeSetup("@variable=DATA_LENGTH" "@value=0" "@min=0 @max=10000");
    addDataNode(&DATA_LENGTH);
    VarCRC.makeSetup("@variable=VarCRC" "@value=0" "@min=0 @max=65535");
    addDataNode(&VarCRC);

    addDataNode(&gp_DSI3Master->MVar_version);
    return true;
}

void ASICMaster::createCustomUi()
{
    ui->tabWidget->setCurrentIndex(0);
}


/****************************************************************************
 *                              FUNCTIONALITY
*****************************************************************************/


// updates all RELAYS and GPIOs
bool ASICMaster::updateHardware(Direction direction, const QString &item)
{

    return false;
}

QVariant ASICMaster::FUNCTION_LIST()
{        

        FUNCTION("Update", "This function updates hardware"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            return updateHardware(HARDWARE_GET,"*");
        }
        FUNCTION("WriteREGX","Write one register"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            Register *preg;
            QByteArray command;
            QString cmd;
            QString resp;
            uint first_byte;
            quint8 addr_wr;

            if(GetREGXPointer(p1.toString(), &preg))
            {
                QByteArray regData = preg->toByteArray();       // BYTE3,BYTE2,BYTE1,BYTE0

                if ( preg->extra("addr") != "")
                {
                    addr_wr = (quint8)preg->extra("addr").toUInt(0,10);
                }
                else
                {
                    addr_wr = GET_REG("ADDR").toInt();
                }

                gp_DSI3Master->Log(LOG_TYPE_SPI_CMD, QString("WRITE_BYADDR, addr: 0x%1").arg(addr_wr, 0, 16));

                first_byte = (((uint)gp_DSI3Master->CHANNEL.value() << 5)) | 0x02;
                command.append((char)first_byte).append(addr_wr).append(regData.at(3))\
                        .append(regData.at(2)).append(regData.at(1)).append(regData.at(0));
                command.append(gp_UartSpiController->CalculateCRC(command));   //CRC

                cmd = command.toHex();


                //if (GET_REG("UPIDTO").toInt() < 3)
                //{
                resp = gp_UartSpiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","").toString();
                //}
                //else
               // {
               //     resp = gp_UartController->runAppCommand("longSPIWriteRead", QString("data=%1").arg(cmd)).toString();
               // }
                if (resp != "")
                    return true;
                else
                    return false;
            }
            else
            {
                LERROR("The parameter of the RUN command is not corrent! E.g.: Use REGX01 as a parameter");
                return false;
            }


        }

        FUNCTION("ReadREGX","Read one register"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            Register *preg;
            QByteArray command;
            QByteArray regdata;
            QByteArray rdata;
            QString cmd;
            uint first_byte;
            quint8 addr;

            if (GetREGXPointer(p1.toString(), &preg))
            {
                if ( preg->extra("addr") != "")
                {
                    addr = (quint8)preg->extra("addr").toUInt(0,10);
                }
                else
                {
                    addr = GET_REG("ADDR").toInt();
                }

                gp_DSI3Master->Log(LOG_TYPE_SPI_CMD, QString("READ_BYADDR, addr: 0x%1").arg(addr, 0, 16));

                first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x03;
                command.append((char)first_byte).append(addr).append((char)0x00)\
                        .append((char)0x00).append((char)0x00).append((char)0x00);

                command.append(gp_UartSpiController->CalculateCRC(command));   //CRC
                cmd = command.toHex();

                //if (GET_REG("UPIDTO").toInt() < 3)
                //{
                    gp_UartSpiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","");              // READ_BY_ADDR
                //}
               // else
                //{
                //    gp_UartController->runAppCommand("longSPIWriteRead", QString("data=%1").arg(cmd));              // READ_BY_ADDR
               // }

                first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x01;
                command.remove(0, 1);
                command.prepend((char)first_byte);
                command.remove((int)6, (int)1);
                command.append(gp_UartSpiController->CalculateCRC(command));   //CRC
                cmd = command.toHex();

                gp_DSI3Master->Log(LOG_TYPE_SPI_CMD, QString("NO_OPERATION"));

                //if (GET_REG("UPIDTO").toInt() < 3)
                //{
                    regdata = gp_UartSpiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();      // NO_OPERATION
                //}
                //else
                //{
                //    regdata = gp_UartController->runAppCommand("longSPIWriteRead", QString("data=%1").arg(cmd)).toByteArray();      // NO_OPERATION
                //}

                if (regdata != "")
                {
                    regdata = QByteArray::fromHex(regdata);

                    rdata.append(regdata.at(5)).append(regdata.at(4)).append(regdata.at(3)).append(regdata.at(2));

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

        FUNCTION("ReadREGXs", "Read more registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QString regs;
            regs = p1.toString();

            if(regs == "REGX0D-REGX10")
            {
                runAppCommand("ReadREGX", "REGX0D","","","","");
                runAppCommand("ReadREGX", "REGX0E","","","","");
                runAppCommand("ReadREGX", "REGX0F","","","","");
                runAppCommand("ReadREGX", "REGX10","","","","");
                UpdateSlotDurations();
            }
            else if (regs == "REGX13-REGX15")
            {
                runAppCommand("ReadREGX", "REGX13","","","","");
                runAppCommand("ReadREGX", "REGX14","","","","");
                runAppCommand("ReadREGX", "REGX15","","","","");
                UpdateVoltages();
            }
            else if (regs == "REGX13-REGX16")
            {
                runAppCommand("ReadREGX", "REGX13","","","","");
                runAppCommand("ReadREGX", "REGX14","","","","");
                runAppCommand("ReadREGX", "REGX15","","","","");
                runAppCommand("ReadREGX", "REGX16","","","","");
                UpdateVoltages();
            }
        }

        FUNCTION("WriteConfig", "Write all configuration registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            runAppCommand("WriteREGX", "REGX00","","","","");
            runAppCommand("WriteREGX", "REGX01","","","","");
            runAppCommand("WriteREGX", "REGX02","","","","");

            return true;
        }


        FUNCTION("ReadConfig", "Read all configuration registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
           runAppCommand("ReadREGX", "REGX00","","","","");
           runAppCommand("ReadREGX", "REGX01","","","","");
           runAppCommand("ReadREGX", "REGX02","","","","");
           return true;
        }


        FUNCTION("ReadStatus", "Read all status registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            runAppCommand("ReadREGX", "REGX0D","","","","");
            runAppCommand("ReadREGX", "REGX0E","","","","");
            runAppCommand("ReadREGX", "REGX0F","","","","");
            runAppCommand("ReadREGX", "REGX10","","","","");
            runAppCommand("ReadREGX", "REGX11","","","","");
            runAppCommand("ReadREGX", "REGX12","","","","");
            runAppCommand("ReadREGX", "REGX13","","","","");
            runAppCommand("ReadREGX", "REGX14","","","","");
            runAppCommand("ReadREGX", "REGX15","","","","");
            runAppCommand("ReadREGX", "REGX16","","","","");

            UpdateVoltages();
            return true;
        }

        FUNCTION("WriteTX", "Write all TX registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            runAppCommand("WriteREGX", "REGX03","","","","");
            runAppCommand("WriteREGX", "REGX04","","","","");
            runAppCommand("WriteREGX", "REGX05","","","","");
            runAppCommand("WriteREGX", "REGX06","","","","");
            runAppCommand("WriteREGX", "REGX07","","","","");
            return true;
        }

        FUNCTION("ReadTX", "Read all TX registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            runAppCommand("ReadREGX", "REGX03","","","","");
            runAppCommand("ReadREGX", "REGX04","","","","");
            runAppCommand("ReadREGX", "REGX05","","","","");
            runAppCommand("ReadREGX", "REGX06","","","","");
            runAppCommand("ReadREGX", "REGX07","","","","");
            return true;
        }

        FUNCTION("ReadRX", "Read all TX registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            runAppCommand("ReadREGX", "REGX20","","","","");
            return true;
        }

        FUNCTION("GetCRMResp","Get CRM response"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QByteArray command;
            QByteArray regdata;
            QByteArray rdata;
            QString cmd;
            uint first_byte;
            int registers;
            int i;

            if(p1.toString() == "REGX20")
                registers = 1;
            else if(p1.toString() == "REGX20-REGX21")
                registers = 2;
            else if(p1.toString() == "REGX20-REGX22")
                registers = 3;
            else
                registers = 4;


            gp_DSI3Master->Log(LOG_TYPE_SPI_CMD, QString("GET_CRM_RESP, %1B").arg(registers*4));

            first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x04;
            command.append((char)first_byte).append((char)0x00).append((char)0x00)\
                    .append((char)0x00).append((char)0x00).append((char)0x00);

            command.append(gp_UartSpiController->CalculateCRC(command));   //CRC

            for (i = 0; i < registers; i++)
            {
                command.append((char)0x00).append((char)0x00).append((char)0x00).append((char)0x00);
            }

            cmd = command.toHex();


            //if (GET_REG("UPIDTO").toInt() < 3)
            //{
                regdata = gp_UartSpiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();
            //}
            //else
            //{
            //    regdata = gp_UartController->runAppCommand("longSPIWriteRead", QString("data=%1").arg(cmd)).toByteArray();
            //}

            if (regdata != "")
            {
                regdata = QByteArray::fromHex(regdata);

                if (regdata.length() == (7 + registers * 4))
                {
                    switch (registers)
                    {
                    case 4:
                        rdata.clear();
                        rdata.append(regdata.at(22)).append(regdata.at(21)).append(regdata.at(20)).append(regdata.at(19));
                        REGX23.fromByteArray(rdata);
                    case 3:
                        rdata.clear();
                        rdata.append(regdata.at(18)).append(regdata.at(17)).append(regdata.at(16)).append(regdata.at(15));
                        REGX22.fromByteArray(rdata);
                    case 2:
                        rdata.clear();
                        rdata.append(regdata.at(14)).append(regdata.at(13)).append(regdata.at(12)).append(regdata.at(11));
                        REGX21.fromByteArray(rdata);
                    case 1:
                        rdata.clear();
                        rdata.append(regdata.at(10)).append(regdata.at(9)).append(regdata.at(8)).append(regdata.at(7));
                        REGX20.fromByteArray(rdata);
                    }

                    slot_updateAllControls();
                    return true;
                }
                else
                {
                    LERROR("Unexpected number of bytes received");
                    return false;
                }
            }
            return false;
        }

        FUNCTION("GetPDCMresp", "Perform GET_PDCM_RESP command"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QByteArray command;
            QByteArray regdata;
            QString cmd;
            QString str_data;
            uint first_byte;
            int slot_length;
            int i;

            slot_length = GET_REG("SLOT_LENGTH").toInt();

            first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x05;
            command.append((char)first_byte).append((char)GET_REG("SLOT_NUMBER").toInt()).append((char)0x00)\
                    .append((char)0x00).append((char)0x00).append((char)0x00);

            command.append(gp_UartSpiController->CalculateCRC(command));   //CRC

            for (i = 0; i < (slot_length + 2); i++)
            {
                command.append((char)0x00);
            }

            cmd = command.toHex();


            //if (GET_REG("UPIDTO").toInt() < 3)
            //{
                regdata = gp_UartSpiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();
            //}
            //else
            //{
            //    regdata = gp_UartController->runAppCommand("longSPIWriteRead", QString("data=%1").arg(cmd)).toByteArray();
            //}

            if (regdata != "")
            {
                regdata = QByteArray::fromHex(regdata);

                if (regdata.length() == (7 + slot_length + 2))
                {

                    regdata.remove(0, 7);
                    str_data = regdata.toHex();
                    ui->lineEditPDCMresp->setText(str_data);
                    return str_data;
                }
                else
                {
                    LERROR("Unexpected number of bytes received");
                    return QString("");
                }
            }
            return QString("");

        }

        FUNCTION("GetAllPDCMresp", "Perform GET_ALL_PDCM_RESP command"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QByteArray command;
            QByteArray regdata;
            QString cmd;
            QString str_data;
            uint first_byte;
            int slot_length;
            int slot_count;
            int i;

            slot_length = GET_REG("SLOT_LENGTH_ALL").toInt();
            slot_count = GET_REG("SLOT_COUNT").toInt();

            first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x07;
            command.append((char)first_byte).append((char)0x00).append((char)0x00)\
                    .append((char)0x00).append((char)0x00).append((char)0x00);

            command.append(gp_UartSpiController->CalculateCRC(command));   //CRC

            for (i = 0; i < (((slot_length + 2) * slot_count) + 1); i++)
            {
                command.append((char)0x00);
            }

            cmd = command.toHex();

            gp_DSI3Master->Log(LOG_TYPE_SPI_CMD, QString("GET_ALL_PDCM_RESP"));

            regdata = gp_UartSpiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();

            if (regdata != "")
            {
                regdata = QByteArray::fromHex(regdata);

                if (regdata.length() == (7 + ((slot_length + 2) * slot_count) + 1))
                {

                    regdata.remove(0, 7);
                    str_data = regdata.toHex();


                    //char crctst = gp_UartController->CalculateUPAECRC(regdata.mid(slot_length*slot_count, slot_count*2));

                    //str_data.append("  ");
                    //str_data.append(crctst);
                    ui->lineEditAllPDCMresp->setText(str_data);
                    return str_data;
                }
                else
                {
                    LERROR("Unexpected number of bytes received");
                    return QString("");
                }
            }
            return QString("");

        }
        FUNCTION("GetAllPDCMrespShadow", "Perform GET_ALL_PDCM_RESP_SHADOW command"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QByteArray command;
            QByteArray regdata;
            QString cmd;
            QString str_data;
            uint first_byte;
            int slot_length;
            int slot_count;
            int i;

            slot_length = GET_REG("SLOT_LENGTH_ALL").toInt();
            slot_count = GET_REG("SLOT_COUNT").toInt();

            first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x08;
            command.append((char)first_byte).append((char)0x00).append((char)0x00)\
                    .append((char)0x00).append((char)0x00).append((char)0x00);

            command.append(gp_UartSpiController->CalculateCRC(command));   //CRC

            for (i = 0; i < (((slot_length + 2) * slot_count) + 1); i++)
            {
                command.append((char)0x00);
            }

            cmd = command.toHex();

            gp_DSI3Master->Log(LOG_TYPE_SPI_CMD, QString("GET_ALL_PDCM_RESP_SHADOW"));

            regdata = gp_UartSpiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();

            if (regdata != "")
            {
                regdata = QByteArray::fromHex(regdata);

                if (regdata.length() == (7 + ((slot_length + 2) * slot_count) + 1))
                {

                    regdata.remove(0, 7);
                    str_data = regdata.toHex();


                    //char crctst = gp_UartController->CalculateUPAECRC(regdata.mid(slot_length*slot_count, slot_count*2));

                    //str_data.append("  ");
                    //str_data.append(crctst);
                    ui->lineEditAllPDCMrespShadow->setText(str_data);
                    return str_data;
                }
                else
                {
                    LERROR("Unexpected number of bytes received");
                    return QString("");
                }
            }
            return QString("");

        }


        FUNCTION("AddCRC", "Add CRC to last byte"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            Register *preg;
            QByteArray temp;


            if (GetREGXPointer(p1.toString(), &preg))
            {
                temp = preg->toByteArray();

                QByteArray data_crc;
                data_crc.append(temp.at(3)).append(temp.at(2)).append(temp.at(1));

                char res = gp_UartSpiController->CalculateUPAECRC(data_crc);

                temp.remove(0, 1);
                temp.prepend(res);
                preg->fromByteArray(temp);

                slot_updateAllControls();

                return true;
            }
            else
            {
                LERROR("The parameter of the RUN command is not corrent! E.g.: Use REGX01 as a parameter");
                return false;
            }

        }
        FUNCTION("QuickConfiguration", "Quick configuration of Master and Slaves"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            bool res;
            gp_UartSpiController->runAppCommand("WriteREGX", "U2SREGX03", "", "", "", "");
            runAppCommand("WriteREGX", "REGX50","","","","");
            runAppCommand("WriteREGX", "REGX50","","","","");
            runAppCommand("ReadStatus", "","","","","");
            SET_REG("REGX02[TX_START]", 0);
            SET_REG("REGX02[BUS_DRIVER_ENA]", 1);
            runAppCommand("WriteConfig", "","","","","");

            QByteArray demux;
            demux.clear();
            demux.append(0x3F);
            demux.append(0x3F);
            gp_UartSpiController->DEMUX.fromByteArray(demux);
            gp_UartSpiController->runAppCommand("SetDemux","","","","","");



            if ((gp_DSI3Master->MVar_version == VERSION_RDUSDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS2DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX) || \
                    (gp_DSI3Master->MVar_version == VERSION_RDUS3DCH) || (gp_DSI3Master->MVar_version == VERSION_RDUSBSD) || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
            {
                if (gp_DSI3Slave->IsAddressSwitches())
                {
                    gp_DSI3Slave->AddressSWITCHES();
                    gp_DSI3Slave->Synchronize();
                    res = gp_DSI3Slave->CheckSlaves();
                }
                else
                {
                    gp_DSI3Slave->Synchronize();
                    res = gp_DSI3Slave->AddressDEMUXES();
                }

            }
            else
            {
                if (gp_DSI3Slave->IsAddressSwitches())
                {
                    gp_DSI3Slave->AddressSWITCHES();
                    gp_DSI3Slave->Synchronize();
                    res = gp_DSI3Slave->CheckSlaves();
                }
                else
                {
                    gp_DSI3Slave->Synchronize();
                    res = gp_DSI3Slave->AddressDISCOVERY();
                }
            }

            if (res)
            {
                gp_DSI3Slave->ClearSafetyFlags(true);
                gp_DSI3Slave->WriteDSI3RegsAll();
                gp_DSI3Slave->WriteSlaveRegs("Common");
                gp_DSI3Slave->WriteModeRegs();
            }

            QApplication::restoreOverrideCursor();

        }
        FUNCTION("SPIDecoder", ""
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            SPIDecoder();
        }



    return false;
}


void ASICMaster::setupBoard()
{
}


void ASICMaster::suspend()
{       
}

void ASICMaster::resume()
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


bool ASICMaster::GetREGXPointer(QString name, Register **preg)
{
    uint address;
    bool result = false;

    if (name.contains("REGX"))
    {
        result = true;

        name.remove("REGX");
        address = name.toUInt(0, 16);

        switch (address)
        {
        case 0:
           *preg = &REGX00;
            break;
        case 1:
            *preg = &REGX01;
            break;
        case 2:
            *preg = &REGX02;
            break;
        case 3:
            *preg = &REGX03;
            break;
        case 4:
            *preg = &REGX04;
            break;
        case 5:
            *preg = &REGX05;
            break;
        case 6:
            *preg = &REGX06;
            break;
        case 7:
            *preg = &REGX07;
            break;
        case 10:
            *preg = &REGX0A;
            break;
        case 13:
            *preg = &REGX0D;
            break;
        case 14:
            *preg = &REGX0E;
            break;
        case 15:
            *preg = &REGX0F;
            break;
        case 16:
            *preg = &REGX10;
            break;
        case 17:
            *preg = &REGX11;
            break;
        case 18:
            *preg = &REGX12;
            break;
        case 19:
            *preg = &REGX13;
            break;
        case 20:
            *preg = &REGX14;
            break;
        case 21:
            *preg = &REGX15;
            break;
        case 22:
            *preg = &REGX16;
            break;
        case 32:
            *preg = &REGX20;
            break;
        case 33:
            *preg = &REGX21;
            break;
        case 34:
            *preg = &REGX22;
            break;
        case 35:
            *preg = &REGX23;
            break;
        case 80:
            *preg = &REGX50;
            break;
        case 81:
            *preg = &REGX51;
            break;

        default:
            result = false;
            break;
        }
    }
    else if (name.contains("ADDRX"))
    {
        name.remove("ADDRX");
        address = name.toUInt(0, 16);
        SET_REG("ADDR", address & 0xFF);
        *preg = &REGADDR;
        result = true;
    }
    else if (name.contains("ADDR"))
    {
        *preg = &REGADDR;
        result = true;
    }

    return result;
}


quint16 ASICMaster::CalcEEPROMCRC(QByteArray data)
{
    quint16 crc = 0;
    int i, j, w;
    //QByteArray testdata;
    int step = 0;
    //testdata.append((char)0x00);    testdata.append((char)0x00);    testdata.append((char)0x09);    testdata.append((char)0x24);
    //testdata.append((char)0x00);    testdata.append((char)0x00);    testdata.append((char)0xCF);    testdata.append((char)0x03);
    //testdata.append((char)0xFF);    testdata.append((char)0x98);    testdata.append((char)0xF8);    testdata.append((char)0x7C);
    //testdata.append((char)0x95);    testdata.append((char)0x6F);    testdata.append((char)0xF8);    testdata.append((char)0xBA);
    //testdata.append((char)0x7D);    testdata.append((char)0x19);    testdata.append((char)0xFE);    testdata.append((char)0x00);

    //data = testdata;

    static bool poly[9] = {true, true, false, true, true, true, true, true, false};
    bool work[9] = {true, true, true, true, true, true, true, true, true};
    //bool din[152];
    bool din[168];

    //if (data.size() != 20)
    if (data.size() != 22)
    {
        return 0;
    }
    //for (i = 18; i >=0; i--)
    for (i = 20; i >=0; i--)
    {
        for (j = 0; j < 8; j++)
        {
            //din[8*(18-i) + j] = (bool)((data.at(i) >> j) & 0x01);
            din[8*(20-i) + j] = (bool)((data.at(i) >> j) & 0x01);
        }
    }

    //for (i = 151; i >= 0; i--)
    for (i = 167; i >= 0; i--)
    {
        if (work[8] != din[i])
        {
            for (w = 8; w >= 1; w--)
            {
                work[w] = work[w - 1] ^ poly[w];
            }
            work[0] = poly[0];
        }
        else
        {
            for (w = 8; w >= 1; w--)
            {
                work[w] = work[w - 1];
            }
            work[0] = 0;
        }

        step = step + 1;

        //if(step >= 151)
        if(step >= 167)
        {
            break;
        }
    }

    for (i = 0; i < 9; i++)
    {
        crc = crc | ((((uint)work[i]) & 0x01) << i);
    }

    return crc;
}
void ASICMaster::UpdateVoltages()
{
    ui->labelVSUP->setText(QString("%1 V").arg(GET_REG("REGX13[VSUP]").toFloat() * 23.69 / 512, 0, 'f', 2));
    ui->labelVDD->setText(QString("%1 V").arg(GET_REG("REGX13[VDD]").toFloat() * 6.18 / 512, 0, 'f', 2));
    ui->labelVDIG->setText(QString("%1 V").arg(GET_REG("REGX13[VDIG]").toFloat() * 3.605 / 512, 0, 'f', 2));

    ui->labelVIO->setText(QString("%1 V").arg(GET_REG("REGX14[VIO]").toFloat() * 6.18 / 512, 0, 'f', 2));
    ui->labelVDATA2->setText(QString("%1 V").arg(GET_REG("REGX14[VDATA2]").toFloat() * 23.69 / 512, 0, 'f', 2));
    ui->labelVDATA1->setText(QString("%1 V").arg(GET_REG("REGX14[VDATA1]").toFloat() * 23.69 / 512, 0, 'f', 2));

    ui->labelVDDA_D1->setText(QString("%1 V").arg(GET_REG("REGX15[VDDA_D1]").toFloat() * 6.18 / 512, 0, 'f', 2));
    ui->labelVDDA_D2->setText(QString("%1 V").arg(GET_REG("REGX15[VDDA_D2]").toFloat() * 6.18 / 512, 0, 'f', 2));
    ui->labelTEMPERATURE_TJ->setText(QString("%1 C").arg((GET_REG("REGX15[TEMPERATURE_TJ]").toFloat() - 249.0) / 0.959, 0, 'f', 2));

}

void ASICMaster::UpdateSlotDurations()
{
    ui->labelSLOTDUR_1->setText((QString("%1 us").arg(GET_REG("REGX0D[PDCM_SLOT1_DUR]").toFloat() * 0.1, 0, 'f', 2)));
    ui->labelSLOTDUR_2->setText((QString("%1 us").arg(GET_REG("REGX0D[PDCM_SLOT2_DUR]").toFloat() * 0.1, 0, 'f', 2)));
    ui->labelSLOTDUR_3->setText((QString("%1 us").arg(GET_REG("REGX0E[PDCM_SLOT3_DUR]").toFloat() * 0.1, 0, 'f', 2)));
    ui->labelSLOTDUR_4->setText((QString("%1 us").arg(GET_REG("REGX0E[PDCM_SLOT4_DUR]").toFloat() * 0.1, 0, 'f', 2)));
    ui->labelSLOTDUR_5->setText((QString("%1 us").arg(GET_REG("REGX0F[PDCM_SLOT5_DUR]").toFloat() * 0.1, 0, 'f', 2)));
    ui->labelSLOTDUR_6->setText((QString("%1 us").arg(GET_REG("REGX0F[PDCM_SLOT6_DUR]").toFloat() * 0.1, 0, 'f', 2)));
    ui->labelSLOTDUR_7->setText((QString("%1 us").arg(GET_REG("REGX10[PDCM_SLOT7_DUR]").toFloat() * 0.1, 0, 'f', 2)));
    ui->labelSLOTDUR_8->setText((QString("%1 us").arg(GET_REG("REGX10[PDCM_SLOT8_DUR]").toFloat() * 0.1, 0, 'f', 2)));
}

//****************************************************************************
// @function name: PerformCOMMAND()
// @description: Perform DSI3PAS COMMAND
//****************************************************************************
bool ASICMaster::PerformCOMMAND(QByteArray txdata, QByteArray *rxdata, int read_count)
{
    bool result = false;

    gp_DSI3Master->Log(LOG_TYPE_MOBD, QString("DSI3PAS COMMAND command"));
    int length = txdata.length();

    if ((length > 0) && (length < 16))
    {
        gp_DSI3Master->Log(LOG_TYPE_DSI3_TX, txdata.toHex());
        if((read_count == 1) || (read_count == -2))
        {
            if (length > 0){
                REGX03.setValue(24, 31, txdata.at(0));
            }
            if (length > 1){
                REGX03.setValue(16, 23, txdata.at(1));
            }
            if (length > 2){
                REGX03.setValue(8, 15, txdata.at(2));
            }
            if (length > 3){
                REGX03.setValue(0, 7, txdata.at(3));
            }
            if (length > 4){
                REGX04.setValue(24, 31, txdata.at(4));
            }
            if (length > 5){
                REGX04.setValue(16, 23, txdata.at(5));
            }
            if (length > 6){
                REGX04.setValue(8, 15, txdata.at(6));
            }
            if (length > 7){
                REGX04.setValue(0, 7, txdata.at(7));
            }
            if (length > 8){
                REGX05.setValue(24, 31, txdata.at(8));
            }
            if (length > 9){
                REGX05.setValue(16, 23, txdata.at(9));
            }
            if (length > 10){
                REGX05.setValue(8, 15, txdata.at(10));
            }
            if (length > 11){
                REGX05.setValue(0, 7, txdata.at(11));
            }
            if (length > 12){
                REGX06.setValue(24, 31, txdata.at(12));
            }
            if (length > 13){
                REGX06.setValue(16, 23, txdata.at(13));
            }
            if (length > 14){
                REGX06.setValue(8, 15, txdata.at(14));
            }

            if(read_count == 1)
            {
                SET_REG("REGX02[TX_START]", 1);
            }
            else if(read_count == -2)
            {
                SET_REG("REGX02[TX_START]", 0);
            }
            SET_REG("REGX02[BURST_ENA]", 1);
            SET_REG("REGX02[BRC_ENA]", 0);
            SET_REG("REGX02[N_FRAMES]", length);

            if (length > 0){
            result = result & runAppCommand("WriteREGX", "REGX03","","","","").toBool();
            }
            if (length > 4){
            result = result & runAppCommand("WriteREGX", "REGX04","","","","").toBool();
            }
            if (length > 8){
            result = result & runAppCommand("WriteREGX", "REGX05","","","","").toBool();
            }
            if (length > 12){
            result = result & runAppCommand("WriteREGX", "REGX06","","","","").toBool();
            }
            result = result & runAppCommand("WriteREGX", "REGX02","","","","").toBool();
            result = result & runAppCommand("ReadREGX", "REGX12","","","","").toBool();
        }


    }
    else
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "PerformCOMMAND, not enough data or wrong data LENGTH");
    }

    slot_updateAllControls();
    return result;
}

//****************************************************************************
// @function name: PerformCRM()
// @description: Perform DSI3PAS CRM
//****************************************************************************
bool ASICMaster::PerformCRM(QByteArray txdata, QByteArray *rxdata, int read_count)
{
    bool result = true;
    int SYMBOL_ERR;
    int CRM_RECEIVED;
    int RX_TO;
    int FRAME_TO;
    int PDCM_OR_CRM_CRC_FAIL;

    gp_DSI3Master->Log(LOG_TYPE_MOBD, QString("DSI3PAS CRM command"));

    int length = txdata.length();

    if ((length == 4) || (length == 8) || (length == 12))
    {
        gp_DSI3Master->Log(LOG_TYPE_DSI3_TX, txdata.toHex());

        if (length >= 4)
        {
            REGX03.setValue(24, 31, txdata.at(0));
            REGX03.setValue(16, 23, txdata.at(1));
            REGX03.setValue(8, 15, txdata.at(2));
            REGX03.setValue(0, 7, txdata.at(3));
            runAppCommand("AddCRC", "REGX03","","","","");
            runAppCommand("WriteREGX", "REGX03","","","","").toBool();
            SET_REG("REGX02[N_FRAMES]", 1);
        }
        if (length >= 8)
        {
            REGX04.setValue(24, 31, txdata.at(4));
            REGX04.setValue(16, 23, txdata.at(5));
            REGX04.setValue(8, 15, txdata.at(6));
            REGX04.setValue(0, 7, txdata.at(7));
            runAppCommand("AddCRC", "REGX04","","","","");
            runAppCommand("WriteREGX", "REGX04","","","","").toBool();
            SET_REG("REGX02[N_FRAMES]", 2);
        }
        if (length >= 12)
        {
            REGX05.setValue(24, 31, txdata.at(8));
            REGX05.setValue(16, 23, txdata.at(9));
            REGX05.setValue(8, 15, txdata.at(10));
            REGX05.setValue(0, 7, txdata.at(11));
            runAppCommand("AddCRC", "REGX05","","","","");
            runAppCommand("WriteREGX", "REGX05","","","","").toBool();
            SET_REG("REGX02[N_FRAMES]", 3);
        }

        SET_REG("REGX02[TX_START]", 1);
        SET_REG("REGX02[BURST_ENA]", 0);
        SET_REG("REGX02[BRC_ENA]", 0);

        result = result & runAppCommand("WriteREGX", "REGX50","","","","").toBool();
        result = result & runAppCommand("WriteREGX", "REGX51","","","","").toBool();
        result = result & runAppCommand("WriteREGX", "REGX02","","","","").toBool();
        result = result & runAppCommand("ReadREGX", "REGX12","","","","").toBool();

        if (result)
        {
            SYMBOL_ERR = GET_REG("REGX12[SYMBOL_ERR]").toInt();
            CRM_RECEIVED = GET_REG("REGX12[CRM_RECEIVED]").toInt();
            RX_TO = GET_REG("REGX12[RX_TO]").toInt();
            FRAME_TO = GET_REG("REGX12[FRAME_TO]").toInt();
            PDCM_OR_CRM_CRC_FAIL = GET_REG("REGX12[PDCM_OR_CRM_CRC_FAIL]").toInt() & 0x01;

            if ((SYMBOL_ERR == 0) && (RX_TO == 0) && (FRAME_TO == 0) && (CRM_RECEIVED == 1))
            {
                if (length >= 12)
                {
                    result = result & runAppCommand("GetCRMResp", "REGX20-REGX22","","","","").toBool();
                }
                else if (length >= 8)
                {
                    result = result & runAppCommand("GetCRMResp", "REGX20-REGX21","","","","").toBool();
                }
                else if (length >= 4)
                {
                    result = result & runAppCommand("GetCRMResp", "REGX20","","","","").toBool();
                }


                if (result)
                {
                    rxdata->clear();

                    if (length >= 4)
                    {
                        rxdata->append(REGX20.value(24, 31));
                        rxdata->append(REGX20.value(16, 23));
                        rxdata->append(REGX20.value(8, 15));
                        rxdata->append(REGX20.value(0, 7));
                    }
                    if (length >= 8)
                    {
                        rxdata->append(REGX21.value(24, 31));
                        rxdata->append(REGX21.value(16, 23));
                        rxdata->append(REGX21.value(8, 15));
                        rxdata->append(REGX21.value(0, 7));
                    }
                    if (length >= 12)
                    {
                        rxdata->append(REGX22.value(24, 31));
                        rxdata->append(REGX22.value(16, 23));
                        rxdata->append(REGX22.value(8, 15));
                        rxdata->append(REGX22.value(0, 7));
                    }

                    gp_DSI3Master->Log(LOG_TYPE_DSI3_RX, rxdata->toHex());

                    //check crc
                    //temp = ReadCOMMAND4B.toByteArray();
                    //data_crc.append(temp.at(3)).append(temp.at(2)).append(temp.at(1));
                    //crc_res = gp_UartController->CalculateUPAECRC(data_crc);

                    //if (crc_res != temp.at(0))
                    //{
                    //    LERROR("CRC of CRM response is not correct");
                    //}
                }
            }
            else
            {
                result = false;
            }
            slot_updateAllControls();
        }

    }
    else
    {
        gp_DSI3Master->Log(LOG_TYPE_ERROR, "PerformCRM, not enough data or wrong data LENGTH");
    }

    slot_updateAllControls();
    return result;
}

void ASICMaster::Ver_setItemText(int index, QString text)
{
    ui->comboBoxVersionM->setItemText(index, text);
}

bool ASICMaster::GenerateBRCsAndGetData(int slot_count, int BRC_count, int page)
{
    QString data_sid[6];
    int sid;
    int brc;
    QByteArray data;
    QByteArray temp;

    for(sid = 0; sid < 6; sid++)
    {
        data_sid[sid].clear();
    }

    if (page == 0)
    {
        runAppCommand("WriteREGX", "REGX51","","","","");

        for (brc = 0; brc < BRC_count; brc++)
        {
            SET_REG("REGX02[N_FRAMES]", 1);
            SET_REG("REGX02[TIMER]", 0);
            SET_REG("REGX02[NUM_OF_PDCM_SLOTS]", slot_count - 1);

            SET_REG("REGX02[BURST_ENA]", 0);
            SET_REG("REGX02[BRC_ENA]", 1);
            SET_REG("REGX02[TX_START]", 1);

            gp_DSI3Master->Log(LOG_TYPE_MOBD, "Generate 1 BRC:");

            runAppCommand("WriteREGX", "REGX02","","","","");

            runAppCommand("ReadREGX", "REGX12","","","","");

            SET_REG("SLOT_LENGTH_ALL", 16);
            SET_REG("SLOT_COUNT", slot_count);
            runAppCommand("GetAllPDCMresp","","","","","");

            data = QByteArray::fromHex(ui->lineEditAllPDCMresp->text().toLatin1());

            for(sid = 0; sid < 6; sid++)
            {
                temp = data.mid(18*sid + 1, 16);
                data_sid[sid].append(temp.toHex());
            }
        }
    }
    else if (page == 1)
    {
        runAppCommand("WriteREGX", "REGX51","","","","");

        for (brc = 0; brc < (BRC_count / 2); brc++)
        {
            SET_REG("REGX02[N_FRAMES]", 2);
            SET_REG("REGX02[TIMER]", 30);
            SET_REG("REGX02[NUM_OF_PDCM_SLOTS]", slot_count - 1);

            SET_REG("REGX02[BURST_ENA]", 0);
            SET_REG("REGX02[BRC_ENA]", 1);
            SET_REG("REGX02[TX_START]", 1);

            gp_DSI3Master->Log(LOG_TYPE_MOBD, "Generate 2 BRCs:");
            gp_DSI3Master->Log(LOG_TYPE_INFO, "Delay between 2 BRCs defined in TIMER");

            runAppCommand("WriteREGX", "REGX02","","","","");

            runAppCommand("ReadREGX", "REGX12","","","","");

            SET_REG("SLOT_LENGTH_ALL", 16);
            SET_REG("SLOT_COUNT", slot_count);

            runAppCommand("GetAllPDCMrespShadow","","","","","");

            data = QByteArray::fromHex(ui->lineEditAllPDCMrespShadow->text().toLatin1());

            for(sid = 0; sid < 6; sid++)
            {
                temp = data.mid(18*sid + 1, 16);
                data_sid[sid].append(temp.toHex());
            }

            runAppCommand("GetAllPDCMresp","","","","","");

            data = QByteArray::fromHex(ui->lineEditAllPDCMresp->text().toLatin1());

            for(sid = 0; sid < 6; sid++)
            {
                temp = data.mid(18*sid + 1, 16);
                data_sid[sid].append(temp.toHex());
            }
        }
    }

    for(sid = 0; sid < 6; sid++)
    {
        gp_DSI3Slave->SetPDCMdata(sid+1, page, data_sid[sid]);
    }

    return true;
}

void ASICMaster::SPIDecoder()
{
    QString filenameMOSI = QFileDialog::getOpenFileName(this, "Open MOSI TXT file",QDir::currentPath(), "MOSI TXT file(*.txt)");
    QString filenameMISO;
    QString filenameMOSIout = QFileDialog::getSaveFileName(this, "Save MOSI TXT file", QDir::currentPath(), "MOSI TXT file(*.txt)");
    //QString filenameMISOout;

    filenameMISO = filenameMOSI;
    filenameMISO.replace("MOSI", "MISO");
    //filenameMISOout.replace("MISO", "MISOcommented");


    QFile fileMOSI(filenameMOSI);
    QFile fileMISO(filenameMISO);
    QFile fileMOSIout(filenameMOSIout);
    //QFile fileMISOout(filenameMISOout);

    if(!fileMOSI.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", fileMOSI.errorString());
        return;
    }
    if(!fileMISO.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", fileMISO.errorString());
        return;
    }

    if (!fileMOSIout.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::information(0, "error", fileMOSIout.errorString());
        return;
    }

    /*if (!fileMISOout.open(QFile::WriteOnly | QFile::Truncate))
    {
        QMessageBox::information(0, "error", fileMISOout.errorString());
        return;
    }*/

    QTextStream tsMOSI(&fileMOSI);
    QTextStream tsMISO(&fileMISO);
    QTextStream tsMOSIout(&fileMOSIout);
    //QTextStream tsMISOout(&fileMISOout);


    QString lineMOSI;
    QString lineMISO;
    QString outMOSI;
    QString outMISO;
    QStringList qslMOSI;
    QStringList qslMISO;
    int num;

    while ((!tsMOSI.atEnd()) && (!tsMISO.atEnd()))
    {
        lineMOSI = tsMOSI.readLine();
        lineMISO = tsMISO.readLine();
        outMOSI = lineMOSI;
        outMISO = lineMISO;
        qslMOSI = lineMOSI.split(",");
        qslMISO = lineMISO.split(",");

        num = qslMOSI.at(SPI_CMD_COLUMN).toInt();
        if (qslMOSI.at(SPI_CMD_COLUMN).toInt() == NO_OPERATION)
        {
            outMOSI.append("  NO_OP  ");
        }
        if (qslMOSI.at(SPI_CMD_COLUMN).toInt() == WRITE_BYADDR)
        {
            outMOSI.append("  WRITE  ");
            if ((qslMOSI.at(SPI_ADDRESS_COLUMN).toInt() < 3) || (qslMOSI.at(SPI_ADDRESS_COLUMN).toInt(0, 16) > 7))
            {
                outMOSI.append(DecodeUPIDWrite(qslMOSI.at(SPI_ADDRESS_COLUMN).toInt(0, 16), (qslMOSI.at(4).toUInt(0, 16) << 24) + (qslMOSI.at(5).toUInt(0, 16) << 16) + (qslMOSI.at(6).toUInt(0, 16) << 8) + qslMOSI.at(7).toUInt(0, 16)));
            }
        }
        if (qslMOSI.at(SPI_CMD_COLUMN).toInt() == READ_BYADDR)
        {
            outMOSI.append("  READ  ");
        }
        if (qslMOSI.at(SPI_CMD_COLUMN).toInt() == GET_CRM_RESP)
        {
            outMOSI.append("  CRM  ");
        }
        tsMOSIout << outMOSI;
        tsMOSIout << "\n";
        //tsMISOout << "\n";
    }
    fileMOSI.close();
    fileMISO.close();
    fileMOSIout.close();
    //fileMISOout.close();
}

QString ASICMaster::DecodeUPIDWrite(int addr, uint data)
{
    QString comment;
    QString temp;
    Register *preg;
    Register reg;
    int i;

    if(GetREGXPointer(QString("REGX%1").arg(addr, 0, 16), &preg))
    {
        reg.makeSetup(preg->getSetup());
        reg.setValue(0, 31, data);
    }
    else
    {
        return "";
    }

    for (i = 0; i < reg.m_items.size(); i++)
    {
        temp = QString("[%1]").arg(reg.items().at(i));
        if (reg.value(temp) != 0)
        {
            comment.append(QString("%1=%2, ").arg(reg.m_items.at(i)).arg(reg.value(temp), 0, 16));
        }
    }


    return comment;
}
