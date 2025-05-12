#include "rdummaster.h"
#include "ui_rdummaster.h"
#include "mainapp.h"
#include "uartuartcontroller.h"
#include "uartssicontroller.h"
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

#define NO_OPERATION        (1)
#define WRITE_BYADDR        (2)
#define READ_BYADDR         (3)
#define GET_CRM_RESP        (4)
#define GET_PDCM_RESP       (5)
#define SPI_CMD_COLUMN      (2)
#define SPI_ADDRESS_COLUMN  (3)


RDUMMaster *gp_RDUMMaster;

RDUMMaster::RDUMMaster(QWidget *parent) :
    Applet(parent)
{
    ui = new Ui::RDUMMaster;
    ui->setupUi(this);
    createCustomUi();
    setWindowIcon(QIcon(":/board/onlogo.ico"));
    setWindowTitle("RDUM Master");
    m_current.clear();
    m_x.clear();
    m_y.clear();
    m_y_uint.clear();
    m_timesample = 0;
}

RDUMMaster::~RDUMMaster()
{
    delete ui;
}

bool RDUMMaster::initialize()
{
    MREGX00.makeSetup("@register=MREGX00 @addr=0 @purpose=Config;"     //addr:0x00
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
    addDataNode(&MREGX00);
    MREGX01.makeSetup("@register=MREGX01 @addr=1 @purpose=Config;"     //addr:0x01
                  "MAX_CRM_RESP_START_TIME[11:0]#Time between 1st edge of CRM request and start of CRM response (in Tchips);"
                  "CLKIN_FREQ[17:12]#Information about clock connected to CLKIN;"
                  "HW_INT_MASK#Masking of interrupt source;"
                  "DATA_AMP#Selects amplitude of master to slave communication (0 is 3Vpp, 1 is 4Vpp);"
                  "DATA_REF#Selects reference voltage for DSI3 communication (0 is GND, 1 isVBAT/2);"
                  "SLAVE_RX_FILTER_ENA[22:21]#Select filter for de-shaping of slave signal;"
                  "VSUPOV_MASK#Disabling of VSUPOV detection;"
                  "REVISION_ID[31:24]#Revision ID of ASIC;"
            );
    addDataNode(&MREGX01);
    MREGX02.makeSetup("@register=MREGX02 @addr=2 @purpose=Config;"     //addr:0x02
                  "N_FRAMES[3:0]#--;"
                  "SCRAMBLER_ENA#1 - Apply descrambler on received data form slave nodes;"
                  "APPPWR[6:5]#--;"
                  "TIMER[15:7]#--;"
                  "NUM_OF_PDCM_SLOTS[18:16]#Number of slots to receive in BRC transfer;"
                  "CRM_INT_MASK#Masking of interrupt source;"
                  "PDCM_INT_MASK[27:20]#Masking of interrupt source;"
                  "BUS_DRIVER_ENA#Switches bus driver on/off;"
                  "BURST_ENA#Trigger BURST transfer;"
                  "BRC_ENA#Trigger BRC transfer;"
                  "TX_START#Edge sensitive bit to trigger operation specified by (BRC_ENA, BURST_ENA, APPNRM, APPDLY);"
            );
    addDataNode(&MREGX02);
    MREGX03.makeSetup("@register=MREGX03 @addr=3 @purpose=TXData;"
                  "DATA[31:0]#CRM/BURST transmit data buffer;"
            );
    addDataNode(&MREGX03);
    MREGX04.makeSetup("@register=MREGX04 @addr=4 @purpose=TXData;"
                  "DATA[31:0]#CRM/BURST transmit data buffer;"
            );
    addDataNode(&MREGX04);
    MREGX05.makeSetup("@register=MREGX05 @addr=5 @purpose=TXData;"
                  "DATA[31:0]#CRM/BURST transmit data buffer;"
            );
    addDataNode(&MREGX05);
    MREGX06.makeSetup("@register=MREGX06 @addr=6 @purpose=TXData;"
                  "DATA[31:0]#CRM/BURST transmit data buffer;"
            );
    addDataNode(&MREGX06);
    MREGX07.makeSetup("@register=MREGX07 @addr=7 @purpose=TXData;"
                  "DATA[31:0]#CRM/BURST transmit data buffer;"
            );
    addDataNode(&MREGX07);

    MREGX08.makeSetup("@register=MREGX08 @addr=8 @purpose=PCR;"
                  "DELAY_CHB[3:0]#Selects delay 8..18ms for channel B;"
                  "DURATION_CHB[6:4]#Select length of delayed power phase (0ms, 1ms, 1.5ms, 2ms, 2.5ms, 3ms, 3.5ms, 4ms at typical oscillator) for channel B;"
                  "PWRENA_CHB#Allows generation of power phase on channel B;"
                  "DELAY_CHA[11:8]#Selects delay 8..18ms for channel A;"
                  "DURATION_CHA[14:12]#Select length of delayed power phase (0ms, 1ms, 1.5ms, 2ms, 2.5ms, 3ms, 3.5ms, 4ms at typical oscillator) for channel A;"
                  "PWRENA_CHA#Allows generation of power phase on channel A;"
                  "FREE1[29:16];"
                  "PSW_SLP[31:30]#Controls slope of power pulse;"
            );
    addDataNode(&MREGX08);

    MREGX09.makeSetup("@register=MREGX09 @addr=9 @purpose=ADVconf;"
                  "N_FRAMES_MSB[1:0]#Extra bits for executing more frames;"
                  "FREE1[7:2]#CRM/BURST transmit data buffer;"
                  "BRC_FORCE_SLOT_ERROR[15:8]#Force data corrution in received slot by BRC command. Bit mask - bit n (7..0) will cause corruption of slot (8..1);"
                  "DSI_RX_THR[24:16]#Threshold for accepting communication premble;"
                  "FREE2[31:25]#CRM/BURST transmit data buffer;"
            );
    addDataNode(&MREGX09);
    MREGX0A.makeSetup("@register=MREGX0A @addr=10 @purpose=CTM;"
                  "CTM_START[11:0]#Sets delay between SPI write command with TX_start is 1 to start of current sampling. 1LSB is 6.4us at typical oscillator;"
                  "_1[15:12];"
                  "CTM_DIV[17:16]#Selects downsampling of observed current;"
                  "_2[23:18];"
                  "CTM_SEL[27:24]#Selects data for CTM module to sample;"
                  "_3;"
                  "CTM_BSTOSC_EXT#Enable external input for bst_osc_clk_20m. ECU (or FPGA) have to turn booster off and CLKIN to switch setting of this bit. If using PLL BSTSYNC have to has fixed frequency;"
                  "CTM_ENA_CHB#Enables sampling of CHB current into CHA shadow memory;"
                  "CTM_ENA_CHA#Enables sampling of CHA current into CHB shadow memory;"
            );
    addDataNode(&MREGX0A);
    MREGX0B.makeSetup("@register=MREGX0B @addr=11 @purpose=Config;"
                  "BST_VLIM_THR[7:0]#Peak current during TON phase;"
                  "BST_VSETPOINT[14:8]#Target voltage for booster;"
                  "BST_FREQ[19:15]#Set booster operation frequency;"
                  "VBST_UV_THR[27:20]#Set threshold for VBSTUV detection. 1LSB is 2 ADC LSBs;"
                  "NONE[30:28]#;"
                  "BST_ENA#0 to disable booster operation;"
            );
    addDataNode(&MREGX0B);
    MREGX0C.makeSetup("@register=MREGX0C @addr=12 @purpose=Config;"
                  "BST_COMP_DIV[2:0]#Division of COMP on the current comparator input;"
                  "NONE1;"
                  "BST_MIN_TOFF[6:4]#Minimum TOFF time for booster;"
                  "NONE2;"
                  "BST_MIN_TON[9:8]#Minimum TON time for booster;"
                  "NONE3[11:10];"
                  "BST_OTA_GAIN[13:12]#Trans-conductance of the error amplifier;"
                  "NONE4[15:14];"
                  "BST_SKCL_THR[17:16]#Booster skip-cycle threshold;"
                  "BST_DRV_SLP[19:18]#Booster VGATE driver slope setting - Typ Ron 0 - 4 Ω, 1 - 6 Ω, 2 - 8 Ω, 3 - 16 Ω;"
                  "BST_SLP_CTRL[22:20]#Booster slope compensation ;"
                  "NONE6;"
                  "BST_OV_SD[26:24]#Booster overvoltage detection limit;"
                  "NONE7;"
                  "BST_OV_REACT[29:28]#Booster overvoltage reactivation limit;"
                  "NONE8[31:30];"
            );
    addDataNode(&MREGX0C);
    MREGX0D.makeSetup("@register=MREGX0D @addr=13 @purpose=Config;"
                  "STOP_BRC#Stop current sequence of BRCs after this BRC command finishes. After sequence is stopped this bit is cleared;"
                  "_1[7:1];"
                  "BRC_NUM_OF_RET_TRIAL[11:8]#Number of allowed retransmission trials inside one BRC command;"
                  "_2[15:12];"
                  "RET_LIMIT_PER_MEAS#Apply BRC_NUM_OF_RET_TRIAL limit per all sheduled BRCs;"
                  "GEN_EXT_BRC_ENA#Allows generation of extra BRC when all received slots are empty (without current). Maximum of 1 extra BRC is generated per measurement;"
                  "_4[23:18];"
                  "CRC_SEL[25:24]=1#--;"
                  "_3[31:26];"
            );
    addDataNode(&MREGX0D);
    MREGX0E.makeSetup("@register=MREGX0E @addr=14 @purpose=Config;"
                  "BSTSS_PROFILE[1:0]#Select of profile for booster spread spectrum. 0 - triangle, 1 - sawtooth, 2 - reserved (Random?), 3 - reserved ;"
                  "_1[3:2];"
                  "BSTSS_PERIOD#Booster spread specturm modulation wave period. 0 - +-8.33us, 1 - +- 111us;"
                  "_2[7:5];"
                  "BSTSS_SEL[9:8]#Enable of Booster spread spectrum 1 - 5%, 2 - 10%, 3 - 20%;"
                  "_3[15:10];"
                  "BST_SOFTSTART[18:16]#Enable of booster soft-start feature. After booster enable the voltage will slowly rump-up to desired value.;"
                  "_4[23:19];"
                  "BST_SKCL_BEH[26:24]#See spec;"
                  "_5[31:27];"
            );
    addDataNode(&MREGX0E);

    MREGX10.makeSetup("@register=MREGX10 @addr=16 @purpose=Status;"
                  "BST_SKCL#Booster skip-cycle active;"
                  "BST_OSCFAIL#Booster oscillator stopped working;"
                  "BST_FBFAIL#Feedback network disconnected;"
                  "NONE2;"
                  "BST_VDRV_UV#VDRV supply undervoltage detected;"
                  "NONE3[15:5];"
                  "BST_REGSTATUS[17:16]#Current limitation status. 0 is normal operation, 2 is TOFF min limitation, 1 is ILIM comparator liminataion;"
                  "BST_RUNNING#Booster is enabled and regulating;"
                  "NONE4[31:19];"
            );
    addDataNode(&MREGX10);


    MREGX11.makeSetup("@register=MREGX11 @addr=17 @purpose=Status;"
                  "HWR#ASIC went through reset. All configuration was set to POR values;"
                  "GNDL#Deglitcher on GNDL comparator;"
                  "OSCFAIL#Deglitcher on OSC FAIL comparator;"
                  "OSCLCK_FAIL#Lock to external clock reference is not successful;"
                  "TSD#Set when TEMP > TSD_THR;"
                  "TW#Set when TEMP > TW_THR;"
                  "BFAIL#Or of all flags in BSTFAIL register;"
                  "NONE4[9:7];"
                  "TSD1#Comparison of ADC result with calculated threshold;"
                  "TSD2#Comparison of ADC result with calculated threshold;"
                  "VSUPUV#Comparison of ADC result with threshold from EEPROM;"
                  "VSUPOV#Comparison of ADC result with threshold from EEPROM;"
                  "VIOUV#Comparison of ADC result with threshold from EEPROM;"
                  "VIOOV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDUV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDOV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDA1UV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDA1OV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDA2UV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDA2OV#Comparison of ADC result with threshold from EEPROM;"
                  "VDRVUV#Comparison of ADC result with threshold from EEPROM;"
                  "VDRVOV#Comparison of ADC result with threshold from EEPROM;"
                  "VDDAUV#--;"
                  "OVCR1B#Bus overcurrent towards GND  detected by receiver ADC;"
                  "OVCR1T#Bus overcurrent towards VSUP  detected by receiver ADC;"
                  "OVCR2B#Bus overcurrent towards GND  detected by receiver ADC;"
                  "OVCR2T#Bus overcurrent towards VSUP  detected by receiver ADC;"
                  "OVCPW1#Overcurrent detection on power phase switch;"
                  "OVCPW2#Overcurrent detection on power phase switch;"
                  "HW_ERROR#--;"
            );
    addDataNode(&MREGX11);
    MREGX12.makeSetup("@register=MREGX12 @addr=18 @purpose=Status;"
                     "BUS_AVAIL#No power phase is being generated - bus is available;"
                     "RETRAN_DONE#Retransmission event occurred in current BRC group. Cleared by GET_ALL_BRCS;"
                     "NUM_RETRAN_DONE[5:2]#Number of retransmission event occurred in current BRC group. Cleared by GET_ALL_BRCS;"
                     "EXTRA_BRC_DONE#Extra BRC was done due to one regular BRC having all slots zero. Maximum of 1 extra BRC is added during measurement;"
                     "_1;"
                     "BRC_RCVD[15:8]#--;"
                     "CRM_RCVD#All sheduled BRCs were performed;"
                     "TIMER_INT#--;"
                     "BURST_DONE#Whole BURST command was sent to bus;"
                     "HW_ERROR#Copy of REGX11.HW_ERROR;"
                     "RX_TO#There was at least 5 chips withou edge detected on bus;"
                     "FRAME_TO#Response for CRM command was not detected on bus;"
                     "SYMBOL_ERR#Unknown symbol on bus detected;"
                     "CLKIN_CNT[31:23]#Counter running on external clock - CLKIN;"
            );
    addDataNode(&MREGX12);
    MREGX13.makeSetup("@register=MREGX13 @addr=19 @purpose=Status;"
                  "VDRV[8:0]#Result of ADC measurement;"
                  "_1;"
                  "VDD[18:10]#Result of ADC measurement;"
                  "_2;"
                  "VSUP[28:20]#Result of ADC measurement;"
                  "_3[31:29];"
            );
    addDataNode(&MREGX13);
    MREGX14.makeSetup("@register=MREGX14 @addr=20 @purpose=Status;"
                  "VIO[8:0]#Result of ADC measurement;"
                  "_1;"
                  "VDATA2[18:10]#Result of ADC measurement;"
                  "_2;"
                  "VDATA1[28:20]#Result of ADC measurement;"
                  "_3[31:29];"
            );
    addDataNode(&MREGX14);
    MREGX15.makeSetup("@register=MREGX15 @addr=21 @purpose=Status;"
                  "VDDA_D2[8:0]#Result of ADC measurement;"
                  "_1;"
                  "VDDA_D1[18:10]#Result of ADC measurement;"
                  "_2;"
                  "TEMP[28:20]#Result of ADC measurement;"
                  "_3[31:29];"
            );
    addDataNode(&MREGX15);
    MREGX16.makeSetup("@register=MREGX16 @addr=22 @purpose=Status;"
                  "IDATA2[7:0]#Result of ADC measurement;"
                  "IDATA1[15:8]#Result of ADC measurement;"
                  "_1[31:16];"
            );
    addDataNode(&MREGX16);
    MREGX17.makeSetup("@register=MREGX17 @addr=23 @purpose=Status;"
                  "DEVID_X[7:0]#X coordinate of the die on the wafer;"
                  "DEVID_Y[15:8]#Y coordinate of the die on the wafer;"
                  "DEVID_LOT[27:16]#Fabrication lot number;"
                  "DEVID_FAB[29:28]#Wafer fabrication facility code;"
                  "FREE[31:30];"
            );
    addDataNode(&MREGX17);
    MREGX18.makeSetup("@register=MREGX18 @addr=24 @purpose=Status;"
                  "DEVID_WEEK[5:0]#Calendar week when the wafers were started in the fab;"
                  "DEVID_YEAR[10:6]#Alphanumeric coding of the wafer production year;"
                  "DEVID_WAF[15:11]#Wafer number used during wafer lot production ;"
                  "FREE[31:16];"
            );
    addDataNode(&MREGX18);
    MREGX20.makeSetup("@register=MREGX20 @addr=32 @purpose=RXData;"
                  "DATA[31:0]#RX data;"
            );
    addDataNode(&MREGX20);
    MREGX21.makeSetup("@register=MREGX21 @addr=33 @purpose=RXData;"
                  "DATA[31:0]#RX data;"
            );
    addDataNode(&MREGX21);
    MREGX22.makeSetup("@register=MREGX22 @addr=34 @purpose=RXData;"
                  "DATA[31:0]#RX data;"
            );
    addDataNode(&MREGX22);
    MREGX23.makeSetup("@register=MREGX23 @addr=35 @purpose=RXData;"
                  "DATA[31:0]#RX data;"
            );
    addDataNode(&MREGX23);

    MREGX3C.makeSetup("@register=MREGX3C @addr=60 @purpose=SpreadSpectrum;"
                  "BSTSS_STEPS[4:0]#Number of spread spectrum steps, 0 is 1step, 1 is 2steps,31 is 32 (all) steps;"
                  "EMPTY[31:5];"
            );
    addDataNode(&MREGX3C);

    MREGX3D.makeSetup("@register=MREGX3D @addr=61 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ0[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME0[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ1[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME1[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX3D);

    MREGX3E.makeSetup("@register=MREGX3E @addr=62 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ2[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME2[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ3[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME3[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX3E);

    MREGX3F.makeSetup("@register=MREGX3F @addr=63 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ4[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME4[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ5[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME5[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX3F);

    MREGX40.makeSetup("@register=MREGX40 @addr=64 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ6[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME6[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ7[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME7[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX40);

    MREGX41.makeSetup("@register=MREGX41 @addr=65 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ8[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME8[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ9[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME9[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX41);

    MREGX42.makeSetup("@register=MREGX42 @addr=66 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ10[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME10[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ11[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME11[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX42);

    MREGX43.makeSetup("@register=MREGX43 @addr=67 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ12[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME12[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ13[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME13[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX43);

    MREGX44.makeSetup("@register=MREGX44 @addr=68 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ14[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME14[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ15[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME15[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX44);

    MREGX45.makeSetup("@register=MREGX45 @addr=69 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ16[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME16[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ17[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME17[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX45);

    MREGX46.makeSetup("@register=MREGX46 @addr=70 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ18[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME18[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ19[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME19[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX46);

    MREGX47.makeSetup("@register=MREGX47 @addr=71 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ20[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME20[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ21[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME21[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX47);

    MREGX48.makeSetup("@register=MREGX48 @addr=72 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ22[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME22[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ23[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME23[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX48);

    MREGX49.makeSetup("@register=MREGX49 @addr=73 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ24[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME24[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ25[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME25[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX49);

    MREGX4A.makeSetup("@register=MREGX4A @addr=74 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ26[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME26[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ27[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME27[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX4A);

    MREGX4B.makeSetup("@register=MREGX4B @addr=75 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ28[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME28[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ29[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME29[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX4B);

    MREGX4C.makeSetup("@register=MREGX4C @addr=76 @purpose=SpreadSpectrum;"
                  "BSTSS_FREQ30[4:0]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME30[15:5]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
                  "BSTSS_FREQ31[20:16]#Frequency of spread specturm step - same codeing as BST_FREQ;"
                  "BSTSS_TIME31[31:21]#Time of step - 0..64ms in 31us steps. 0 is 31us, 1 is 62us, 63 is 64ms;"
            );
    addDataNode(&MREGX4C);

    MREGX4F.makeSetup("@register=MREGX4F @addr=79 @purpose=ClearStatusErrors;"
                  "CLR_BST_SKCL#Clear trigger for clear by read register;"
                  "CLR_BST_OSCFAIL#Clear trigger for clear by read register;"
                  "CLR_BST_FBFAIL#Clear trigger for clear by read register;"
                  "NONE2;"
                  "CLR_BST_VDRV_UV#Clear trigger for clear by read register;"
                  "NONE3[31:5];"
            );
    addDataNode(&MREGX4F);

    MREGX50.makeSetup("@register=MREGX50 @addr=80 @purpose=ClearStatusErrors;"
                  "CLR_HWR#--;"
                  "CLR_GNDL#--;"
                  "CLR_OSCFAIL#--;"
                  "CLR_OSCLCK_FAIL#--;"
                  "CLR_TSD#--;"
                  "CLR_TW#--;"
                  "NONE3[9:6];"
                  "CLR_TSD1#--;"
                  "CLR_TSD2#--;"
                  "CLR_VSUPUV#--;"
                  "CLR_VSUPOV#--;"
                  "CLR_VIOUV#--;"
                  "CLR_VIOOV#--;"
                  "CLR_VDDUV#--;"
                  "CLR_VDDOV#--;"
                  "CLR_VDDA1UV#--;"
                  "CLR_VDDA1OV#--;"
                  "CLR_VDDA2UV#--;"
                  "CLR_VDDA2OV#--;"
                  "CLR_VDRVUV#--;"
                  "CLR_VDRVOV#--;"
                  "CLR_VDDAUV#--;"
                  "CLR_OVCR1B#--;"
                  "CLR_OVCR1T#--;"
                  "CLR_OVCR2B#--;"
                  "CLR_OVCR2T#--;"
                  "CLR_OVCPW1#CLR_Overcurrent detection on power phase switch;"
                  "CLR_OVCPW2#CLR_Overcurrent detection on power phase switch;"
                  "ZERO;"
            );
    addDataNode(&MREGX50);
    MREGX51.makeSetup("@register=MREGX51 @addr=81 @purpose=ClearStatusErrors;"
                  "ZERO[15:0];"
                  "CLR_CRM_RECEIVED#--;"
                  "ZERO;"
                  "CLR_BURST_DONE#--;"
                  "ZERO;"
                  "CLR_RX_TO#--;"
                  "CLR_FRAME_TO#--;"
                  "CLR_SYMBOL_ERR#--;"
                  "ZERO[31:23];"
            );
    addDataNode(&MREGX51);


    EEX00.makeSetup("@register=EEX00 @addr=0 @purpose=NVM;"     //addr:0x00
                  "BG_TRIM[4:0]#NVM reg;"
                  "RDUS_COMPAT_ENA#NVM reg;"
                  "BST_OSC_TRIM_FREQ4#NVM reg;"
                  "OSC_TRIM_IDIO[10:7]#NVM reg;"
                  "OSC_TRIM_FREQ[15:11]#NVM reg;"
                  "ECC[25:16]#ECC;"
                  "_1[31:26]#empty;"
            );
    addDataNode(&EEX00);
    EEX01.makeSetup("@register=EEX01 @addr=1 @purpose=NVM;"     //addr:0x01
                  "DATAREC1_BIAS_TRIM[5:0]#NVM reg;"
                  "DATAREC2_BIAS_TRIM[11:6]#NVM reg;"
                  "DATADRV_AMP_HGM#NVM reg;"
                  "OSC_TRIM_TEMP[15:13]#NVM reg;"
                  "ECC[25:16]#ECC;"
                  "_1[31:26]#empty;"
            );
    addDataNode(&EEX01);
    EEX02.makeSetup("@register=EEX02 @addr=2 @purpose=NVM;"     //addr:0x02
                  "BST_PPOR[5:0]#NVM reg;"
                  "BST_SLP[10:6]#NVM reg;"
                  "BST_OTA_RSET[15:11]#NVM reg;"
                  "ECC[25:16]#ECC;"
                  "_1[31:26]#empty;"
            );
    addDataNode(&EEX02);
    EEX03.makeSetup("@register=EEX03 @addr=3 @purpose=NVM;"     //addr:0x03
                  "DSI_IDLE_LEVEL[2:0]#NVM reg;"
                  "DSI_IDLE_LEVEL_LOW[5:3]#NVM reg;"
                  "AUTH_DIS#NVM reg;"
                  "VAR45_ENA#NVM reg;"
                  "TSD1_TRIM[11:8]#NVM reg;"
                  "TSD2_TRIM[15:12]#NVM reg;"
                  "ECC[25:16]#ECC;"
                  "_1[31:26]#empty;"
            );
    addDataNode(&EEX03);
    EEX04.makeSetup("@register=EEX04 @addr=4 @purpose=NVM;"     //addr:0x04
                  "TSD_TRIM[7:0]#NVM reg;"
                  "TW_TRIM[15:8]#NVM reg;"
                  "ECC[25:16]#ECC;"
                  "_1[31:26]#empty;"
            );
    addDataNode(&EEX04);
    EEX05.makeSetup("@register=EEX05 @addr=5 @purpose=NVM;"     //addr:0x05
                  "BST_OSC_TRIM_IDIO[3:0]#NVM reg;"
                  "BST_OSC_TRIM_FREQ[7:4]#NVM reg;"
                  "VBST_OV[15:8]#NVM reg;"
                  "ECC[25:16]#ECC;"
                  "_1[31:26]#empty;"
            );
    addDataNode(&EEX05);
    EEX06.makeSetup("@register=EEX06 @addr=6 @purpose=NVM;"     //addr:0x05
                  "VIO_UV[7:0]#NVM reg;"
                  "VIO_OV[15:8]#NVM reg;"
                  "ECC[25:16]#ECC;"
                  "_1[31:26]#empty;"
            );
    addDataNode(&EEX06);
    EEX07.makeSetup("@register=EEX07 @addr=7 @purpose=NVM;"     //addr:0x06
                  "VDDX_UV[7:0]#NVM reg;"
                  "VDDX_OV[15:8]#NVM reg;"
                  "ECC[25:16]#ECC;"
                  "_1[31:26]#empty;"
            );
    addDataNode(&EEX07);
    EEX08.makeSetup("@register=EEX08 @addr=8 @purpose=NVM;"     //addr:0x07
                  "VDRV_UV[7:0]#NVM reg;"
                  "VDRV_OV[15:8]#NVM reg;"
                  "ECC[25:16]#ECC;"
                  "_1[31:26]#empty;"
            );
    addDataNode(&EEX08);
    EEX09.makeSetup("@register=EEX09 @addr=9 @purpose=NVM;"     //addr:0x08
                  "DT_SHORT[6:0]#NVM reg;"
                  "SPI_SEL#NVM reg;"
                  "DT_LONG[15:8]#NVM reg;"
                  "ECC[25:16]#ECC;"
                  "_1[31:26]#empty;"
            );
    addDataNode(&EEX09);
    EEX0A.makeSetup("@register=EEX0A @addr=10 @purpose=NVM;"     //addr:0x0A
                  "CRC[8:0]#CRC of NVM regs;"
                  "OVCD_ADC[15:9]#NVM reg;"
                  "ECC[25:16]#ECC;"
                  "_1[31:26]#empty;"
            );
    addDataNode(&EEX0A);

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
    SLOT_LENGTH_ALL.makeSetup("@variable=SLOT_LENGTH_ALL" "@value=1" "@min=0 @max=3000");
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

    connect(ui->checkBox_x50ALL, SIGNAL(clicked(bool)), this, SLOT(slot_REGX50ALLUpdate()));
    connect(ui->checkBox_x51ALL, SIGNAL(clicked(bool)), this, SLOT(slot_REGX51ALLUpdate()));

    ui->ptCurrent->addGraph();
    ui->ptCurrent->graph(0)->setPen(QPen(QColor::fromRgb(42, 162, 42)));
    ui->ptCurrent->addGraph();
    ui->ptCurrent->graph(1)->setPen(QPen(QColor::fromRgb(255, 0, 0)));
    ui->ptCurrent->addGraph();
    ui->ptCurrent->graph(2)->setPen(QPen(QColor::fromRgb(0, 255, 255)));
    ui->ptCurrent->xAxis->setRange(0, 200);
    ui->ptCurrent->yAxis->setRange(0, 220);
    ui->ptCurrent->xAxis->setLabel("time [us]");
    ui->ptCurrent->yAxis->setLabel("current [mA]");

    ui->tabWidget->setTabEnabled(2, false);
    ui->tabWidget->setTabEnabled(3, false);
    ui->tabWidget->setTabEnabled(5, false);
    ui->tabWidget->setTabEnabled(6, false);
    ui->tabWidget->setTabText(2,"");
    ui->tabWidget->setTabText(3, "");
    return true;
}

void RDUMMaster::createCustomUi()
{
    ui->tabWidget->setCurrentIndex(0);
}


/****************************************************************************
 *                              FUNCTIONALITY
*****************************************************************************/


// updates all RELAYS and GPIOs
bool RDUMMaster::updateHardware(Direction direction, const QString &item)
{

    return false;
}

QVariant RDUMMaster::FUNCTION_LIST()
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

                if (gp_UartSsiController->SSIREGX03C.value("[INTERFACE_SEL]") <= 2)     // SSI
                {
                    addr_wr = addr_wr + 1;
                    gp_DSI3Master->Log(LOG_TYPE_SSI_CMD, QString("WRITE_BYADDR, addr: 0x%1").arg(addr_wr, 0, 16));

                    first_byte = (((uint)gp_DSI3Master->CHANNEL.value() << 5)) | 0x05;
                    command.append((char)first_byte).append(addr_wr).append(regData.at(3))\
                            .append(regData.at(2)).append(regData.at(1)).append(regData.at(0));
                    command.append(gp_UartSsiController->CalculateCRC(command));   //CRC
                    cmd = command.toHex();

                    resp = gp_UartSsiController->runAppCommand("SSIWriteRead", QString("data=%1, rx_length=%2").arg(cmd).arg(7),"","","","").toString();

                }
                else            // SPI
                {
                    gp_DSI3Master->Log(LOG_TYPE_SPI_CMD, QString("WRITE_BYADDR, addr: 0x%1").arg(addr_wr, 0, 16));

                    first_byte = (((uint)gp_DSI3Master->CHANNEL.value() << 5)) | 0x02;
                    command.append((char)first_byte).append(addr_wr).append(regData.at(3))\
                            .append(regData.at(2)).append(regData.at(1)).append(regData.at(0));
                    command.append(gp_UartSsiController->CalculateCRC(command));   //CRC
                    cmd = command.toHex();

                    resp = gp_UartSsiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","").toString();

                }

                if (resp != "")
                    return true;
                else
                    return false;
            }
            else
            {
                LERROR("The parameter of the RUN command is not corrent! E.g.: Use MREGX01 as a parameter");
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


                if (gp_UartSsiController->SSIREGX03C.value("[INTERFACE_SEL]") <= 2)     // SSI
                {
                    addr = addr + 1;
                    gp_DSI3Master->Log(LOG_TYPE_SSI_CMD, QString("READ_BYADDR, addr: 0x%1").arg(addr, 0, 16));

                    first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x04;
                    command.append((char)first_byte).append(addr).append((char)0x00)\
                            .append((char)0x00).append((char)0x00).append((char)0x00);
                    command.append(gp_UartSsiController->CalculateCRC(command));   //CRC
                    cmd = command.toHex();

                    regdata = gp_UartSsiController->runAppCommand("SSIWriteRead", QString("data=%1, rx_length=%2").arg(cmd).arg(7),"","","","").toByteArray();  // READ_BYADDR

                    /*first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x01;
                    command.remove(0, 1);
                    command.prepend((char)first_byte);
                    command.remove((int)6, (int)1);
                    command.append(gp_UartSsiController->CalculateCRC(command));   //CRC
                    cmd = command.toHex();

                    gp_DSI3Master->Log(LOG_TYPE_SSI_CMD, QString("NO_OPERATION"));


                    regdata = gp_UartSsiController->runAppCommand("SSIWriteRead", QString("data=%1, rx_length=%2").arg(cmd).arg(7),"","","","").toByteArray();      // NO_OPERATION*/

                }
                else            // SPI
                {
                    gp_DSI3Master->Log(LOG_TYPE_SPI_CMD, QString("READ_BYADDR, addr: 0x%1").arg(addr, 0, 16));

                    first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x03;
                    command.append((char)first_byte).append(addr).append((char)0x00)\
                            .append((char)0x00).append((char)0x00).append((char)0x00);
                    command.append(gp_UartSsiController->CalculateCRC(command));   //CRC
                    cmd = command.toHex();

                    gp_UartSsiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","").toString();  // READ_BYADDR

                    first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x01;
                    command.remove(0, 1);
                    command.prepend((char)first_byte);
                    command.remove((int)6, (int)1);
                    command.append(gp_UartSsiController->CalculateCRC(command));   //CRC
                    cmd = command.toHex();

                    gp_DSI3Master->Log(LOG_TYPE_SPI_CMD, QString("NO_OPERATION"));


                    regdata = gp_UartSsiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();      // NO_OPERATION
                }

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

            if(regs == "MREGX0D-MREGX10")
            {
                runAppCommand("ReadREGX", "MREGX0D","","","","");
                runAppCommand("ReadREGX", "MREGX0E","","","","");
                runAppCommand("ReadREGX", "MREGX0F","","","","");
                runAppCommand("ReadREGX", "MREGX10","","","","");
                UpdateSlotDurations();
            }
            else if (regs == "MREGX13-MREGX15")
            {
                runAppCommand("ReadREGX", "MREGX13","","","","");
                runAppCommand("ReadREGX", "MREGX14","","","","");
                runAppCommand("ReadREGX", "MREGX15","","","","");
                UpdateVoltages();
            }
            else if (regs == "MREGX13-MREGX16")
            {
                runAppCommand("ReadREGX", "MREGX13","","","","");
                runAppCommand("ReadREGX", "MREGX14","","","","");
                runAppCommand("ReadREGX", "MREGX15","","","","");
                runAppCommand("ReadREGX", "MREGX16","","","","");
                UpdateVoltages();
            }
            else if (regs == "MREGX17-MREGX18")
            {
                runAppCommand("ReadREGX", "MREGX17","","","","");
                runAppCommand("ReadREGX", "MREGX18","","","","");
            }
        }

        FUNCTION("WriteConfig", "Write all configuration registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            runAppCommand("WriteREGX", "MREGX00","","","","");
            runAppCommand("WriteREGX", "MREGX01","","","","");
            runAppCommand("WriteREGX", "MREGX09","","","","");
            runAppCommand("WriteREGX", "MREGX02","","","","");

            return true;
        }


        FUNCTION("ReadConfig", "Read all configuration registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
           runAppCommand("ReadREGX", "MREGX00","","","","");
           runAppCommand("ReadREGX", "MREGX01","","","","");
           runAppCommand("ReadREGX", "MREGX09","","","","");
           runAppCommand("ReadREGX", "MREGX02","","","","");
           return true;
        }


        FUNCTION("ReadStatus", "Read all status registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            runAppCommand("ReadREGX", "MREGX11","","","","");
            runAppCommand("ReadREGX", "MREGX12","","","","");
            runAppCommand("ReadREGX", "MREGX13","","","","");
            runAppCommand("ReadREGX", "MREGX14","","","","");
            runAppCommand("ReadREGX", "MREGX15","","","","");
            runAppCommand("ReadREGX", "MREGX16","","","","");
            runAppCommand("ReadREGX", "MREGX17","","","","");
            runAppCommand("ReadREGX", "MREGX18","","","","");

            UpdateVoltages();
            return true;
        }

        FUNCTION("WriteTX", "Write all TX registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            runAppCommand("WriteREGX", "MREGX03","","","","");
            runAppCommand("WriteREGX", "MREGX04","","","","");
            runAppCommand("WriteREGX", "MREGX05","","","","");
            runAppCommand("WriteREGX", "MREGX06","","","","");
            runAppCommand("WriteREGX", "MREGX07","","","","");
            return true;
        }

        FUNCTION("ReadTX", "Read all TX registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            runAppCommand("ReadREGX", "RMEGX03","","","","");
            runAppCommand("ReadREGX", "RMEGX04","","","","");
            runAppCommand("ReadREGX", "RMEGX05","","","","");
            runAppCommand("ReadREGX", "RMEGX06","","","","");
            runAppCommand("ReadREGX", "RMEGX07","","","","");
            return true;
        }

        FUNCTION("ReadRX", "Read all TX registers"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            runAppCommand("ReadREGX", "MREGX20","","","","");
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

            if(p1.toString() == "MREGX20")
                registers = 1;
            else if(p1.toString() == "MREGX20-MREGX21")
                registers = 2;
            else if(p1.toString() == "MREGX20-MREGX22")
                registers = 3;
            else
                registers = 4;


            gp_DSI3Master->Log(LOG_TYPE_SPI_CMD, QString("GET_CRM_RESP, %1B").arg(registers*4));



            if (gp_UartSsiController->SSIREGX03C.value("[INTERFACE_SEL]") <= 2)     // SSI
            {
                first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x00;
                command.append((char)first_byte).append((char)0x00).append((char)0x00)\
                                    .append((char)0x00).append((char)0x00).append((char)0x00);

                command.append(gp_UartSsiController->CalculateCRC(command));   //CRC

               /* for (i = 0; i < registers; i++)
                {
                    command.append((char)0x00).append((char)0x00).append((char)0x00).append((char)0x00);
                }*/

                cmd = command.toHex();
                regdata = gp_UartSsiController->runAppCommand("SSIWriteRead", QString("data=%1, rx_length=%2").arg(cmd).arg(registers*4),"","","","").toByteArray();

                if (regdata != "")
                {
                    regdata = QByteArray::fromHex(regdata);

                    if (regdata.length() == (registers*4))
                    {
                        switch (registers)
                        {
                        case 4:
                            rdata.clear();
                            rdata.append(regdata.at(15)).append(regdata.at(14)).append(regdata.at(13)).append(regdata.at(12));
                            MREGX23.fromByteArray(rdata);
                        case 3:
                            rdata.clear();
                            rdata.append(regdata.at(11)).append(regdata.at(10)).append(regdata.at(9)).append(regdata.at(8));
                            MREGX22.fromByteArray(rdata);
                        case 2:
                            rdata.clear();
                            rdata.append(regdata.at(7)).append(regdata.at(6)).append(regdata.at(5)).append(regdata.at(4));
                            MREGX21.fromByteArray(rdata);
                        case 1:
                            rdata.clear();
                            rdata.append(regdata.at(3)).append(regdata.at(2)).append(regdata.at(1)).append(regdata.at(0));
                            MREGX20.fromByteArray(rdata);
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
            else            // SPI
            {
                first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x04;
                command.append((char)first_byte).append((char)0x00).append((char)0x00)\
                                    .append((char)0x00).append((char)0x00).append((char)0x00);

                command.append(gp_UartSsiController->CalculateCRC(command));   //CRC

                for (i = 0; i < registers; i++)
                {
                    command.append((char)0x00).append((char)0x00).append((char)0x00).append((char)0x00);
                }

                cmd = command.toHex();
                regdata = gp_UartSsiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","").toByteArray();

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
                            MREGX23.fromByteArray(rdata);
                        case 3:
                            rdata.clear();
                            rdata.append(regdata.at(18)).append(regdata.at(17)).append(regdata.at(16)).append(regdata.at(15));
                            MREGX22.fromByteArray(rdata);
                        case 2:
                            rdata.clear();
                            rdata.append(regdata.at(14)).append(regdata.at(13)).append(regdata.at(12)).append(regdata.at(11));
                            MREGX21.fromByteArray(rdata);
                        case 1:
                            rdata.clear();
                            rdata.append(regdata.at(10)).append(regdata.at(9)).append(regdata.at(8)).append(regdata.at(7));
                            MREGX20.fromByteArray(rdata);
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


        }

        FUNCTION("GetAllPDCMresp", "Perform GET_PDCM_RESP command"
                 ,"<i><br> no parameter"
                 ,"RETURN always true")
        {
            QByteArray command;
            QByteArray regdata;
            QString cmd;
            QString str_data;
            uint first_byte;
            int length;
            int i;

            length = GET_REG("SLOT_LENGTH_ALL").toInt();


            if (gp_UartSsiController->SSIREGX03C.value("[INTERFACE_SEL]") <= 2)     // SSI
            {

            }
            else            // SPI
            {
                gp_DSI3Master->Log(LOG_TYPE_SPI_CMD, QString("GET_ALL_PDCM_RESP, length: %1 bytes").arg(length));

                first_byte = ((uint)gp_DSI3Master->CHANNEL.value() << 5) | 0x07;
                command.append((char)first_byte).append((char)0x00).append((char)0x00)\
                        .append((char)0x00).append((char)0x00).append((char)0x00);
                command.append(gp_UartSsiController->CalculateCRC(command));   //CRC

                for (i = 0; i < (length); i++)
                {
                    command.append((char)0x00);
                }
                cmd = command.toHex();

                str_data = gp_UartSsiController->runAppCommand("SPIWriteRead", QString("data=%1").arg(cmd),"","","","").toString();  // GET_ALL_PDCM_RESP / GET_ALL_BRCS

            }

            if (str_data != "")   //if result is OK
            {
                regdata = QByteArray::fromHex(str_data.toLatin1());

                regdata.remove(0, 7);
                ui->lineEditAllPDCMresp->setText(regdata.toHex());
                return true;
            }
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

                char res = gp_UartSsiController->CalculateUPAECRC(data_crc);

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
            gp_UartSsiController->runAppCommand("WriteREGX", "SSIREGX03C", "", "", "", "");     //write SPI/SSI interface
            runAppCommand("WriteREGX", "MREGX50","","","","");
            runAppCommand("WriteREGX", "MREGX08","","","","");
            runAppCommand("WriteREGX", "MREGX51","","","","");
            runAppCommand("ReadStatus", "","","","","");
            SET_REG("MREGX02[TX_START]", 0);
            SET_REG("MREGX02[BUS_DRIVER_ENA]", 1);
            runAppCommand("WriteConfig", "","","","","");
            runAppCommand("WriteREGX", "MREGX0C","","","","");
            runAppCommand("WriteREGX", "MREGX0E","","","","");
            runAppCommand("WriteREGX", "MREGX0B","","","","");


            if ((gp_DSI3Master->MVar_version == VERSION_RDUSDMX) || (gp_DSI3Master->MVar_version == VERSION_RDUS2DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBEDMX)\
                    || (gp_DSI3Master->MVar_version == VERSION_RDUS3DMX) || (gp_DSI3Master->MVar_version == VERSION_MOBE2DMX))
            {
                if (gp_DSI3Slave->IsAddressSwitches())
                {
                    gp_DSI3Slave->AddressSWITCHES();
                    gp_DSI3Slave->Synchronize();
                    res = gp_DSI3Slave->CheckSlaves();
                }
                else
                {
                    quint32 shiftreg = 0x3F;

                    gp_UartSsiController->SSIREGX040.setValue("[DEMUX_CHA]", shiftreg);
                    gp_UartSsiController->SSIREGX040.setValue("[DEMUX_CHB]", shiftreg);
                    gp_UartSsiController->runAppCommand("WriteREGX", "SSIREGX040", "", "", "", "");


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
        FUNCTION("Demux", ""
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            quint32 shiftreg = 0x3F;

            gp_UartSsiController->SSIREGX040.setValue("[DEMUX_CHA]", shiftreg);
            gp_UartSsiController->SSIREGX040.setValue("[DEMUX_CHB]", shiftreg);
            gp_UartSsiController->runAppCommand("WriteREGX", "SSIREGX040", "", "", "", "");

        }
        FUNCTION("SPIDecoder", ""
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            SPIDecoder();
        }
        FUNCTION("ReadNVMreg00", ""
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            quint32 data;
            gp_UartSsiController->ReadNVMreg(0, &data);
        }
        FUNCTION("ReadNVMreg01", ""
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            quint32 data;
            gp_UartSsiController->ReadNVMreg(1, &data);
        }
        FUNCTION("ReadNVMregs", ""
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            quint32 data;
            bool res;
            res = res & gp_UartSsiController->ReadNVMreg(0, &data);
            EEX00.setValue(0, 31, data);
            res = res & gp_UartSsiController->ReadNVMreg(1, &data);
            EEX01.setValue(0, 31, data);
            res = res & gp_UartSsiController->ReadNVMreg(2, &data);
            EEX02.setValue(0, 31, data);
            res = res & gp_UartSsiController->ReadNVMreg(3, &data);
            EEX03.setValue(0, 31, data);
            res = res & gp_UartSsiController->ReadNVMreg(4, &data);
            EEX04.setValue(0, 31, data);
            res = res & gp_UartSsiController->ReadNVMreg(5, &data);
            EEX05.setValue(0, 31, data);
            res = res & gp_UartSsiController->ReadNVMreg(6, &data);
            EEX06.setValue(0, 31, data);
            res = res & gp_UartSsiController->ReadNVMreg(7, &data);
            EEX07.setValue(0, 31, data);
            res = res & gp_UartSsiController->ReadNVMreg(8, &data);
            EEX08.setValue(0, 31, data);
            res = res & gp_UartSsiController->ReadNVMreg(9, &data);
            EEX09.setValue(0, 31, data);
            res = res & gp_UartSsiController->ReadNVMreg(10, &data);
            EEX0A.setValue(0, 31, data);

        }
        FUNCTION("WriteNVMregs", ""
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            gp_UartSsiController->WriteNVMreg(0, EEX00.value(0, 31));
            gp_UartSsiController->WriteNVMreg(1, EEX01.value(0, 31));
            gp_UartSsiController->WriteNVMreg(2, EEX02.value(0, 31));
            gp_UartSsiController->WriteNVMreg(3, EEX03.value(0, 31));
            gp_UartSsiController->WriteNVMreg(4, EEX04.value(0, 31));
            gp_UartSsiController->WriteNVMreg(5, EEX05.value(0, 31));
            gp_UartSsiController->WriteNVMreg(6, EEX06.value(0, 31));
            gp_UartSsiController->WriteNVMreg(7, EEX07.value(0, 31));
            gp_UartSsiController->WriteNVMreg(8, EEX08.value(0, 31));
            gp_UartSsiController->WriteNVMreg(9, EEX09.value(0, 31));
            gp_UartSsiController->WriteNVMreg(10, EEX0A.value(0, 31));
        }
        FUNCTION("CalcNVMECCCRC", ""
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            QByteArray data;
            uint crc;
            data.clear();
            data.append((char)EEX00.value(8, 15));
            data.append((char)EEX00.value(0, 7));
            data.append((char)EEX01.value(8, 15));
            data.append((char)EEX01.value(0, 7));
            data.append((char)EEX02.value(8, 15));
            data.append((char)EEX02.value(0, 7));
            data.append((char)EEX03.value(8, 15));
            data.append((char)EEX03.value(0, 7));
            data.append((char)EEX04.value(8, 15));
            data.append((char)EEX04.value(0, 7));
            data.append((char)EEX05.value(8, 15));
            data.append((char)EEX05.value(0, 7));
            data.append((char)EEX06.value(8, 15));
            data.append((char)EEX06.value(0, 7));
            data.append((char)EEX07.value(8, 15));
            data.append((char)EEX07.value(0, 7));
            data.append((char)EEX08.value(8, 15));
            data.append((char)EEX08.value(0, 7));
            data.append((char)EEX09.value(8, 15));
            data.append((char)EEX09.value(0, 7));
            data.append((char)EEX0A.value(8, 15));
            data.append((char)EEX0A.value(0, 7));
            crc = CalcEEPROMCRC(data);
            SET_REG("EEX0A[CRC]", crc);
            EEX00.setValue("[ECC]", CalculateECC(EEX00.value(0, 15)));
            EEX01.setValue("[ECC]", CalculateECC(EEX01.value(0, 15)));
            EEX02.setValue("[ECC]", CalculateECC(EEX02.value(0, 15)));
            EEX03.setValue("[ECC]", CalculateECC(EEX03.value(0, 15)));
            EEX04.setValue("[ECC]", CalculateECC(EEX04.value(0, 15)));
            EEX05.setValue("[ECC]", CalculateECC(EEX05.value(0, 15)));
            EEX06.setValue("[ECC]", CalculateECC(EEX06.value(0, 15)));
            EEX07.setValue("[ECC]", CalculateECC(EEX07.value(0, 15)));
            EEX08.setValue("[ECC]", CalculateECC(EEX08.value(0, 15)));
            EEX09.setValue("[ECC]", CalculateECC(EEX09.value(0, 15)));
            EEX0A.setValue("[ECC]", CalculateECC(EEX0A.value(0, 15)));
        }
        FUNCTION("WriteSpreadSpectrum", "Write Spread spectrum registers"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            runAppCommand("WriteREGX", "MREGX3C","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX3D","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX3E","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX3F","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX40","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX41","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX42","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX43","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX44","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX45","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX46","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX47","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX48","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX49","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX4A","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX4B","","","","").toBool();
            runAppCommand("WriteREGX", "MREGX4C","","","","").toBool();
        }
        FUNCTION("ReadSpreadSpectrum", "Read Spread spectrum registers"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            runAppCommand("ReadREGX", "MREGX3C","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX3D","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX3E","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX3F","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX40","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX41","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX42","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX43","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX44","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX45","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX46","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX47","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX48","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX49","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX4A","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX4B","","","","").toBool();
            runAppCommand("ReadREGX", "MREGX4C","","","","").toBool();
        }

        FUNCTION("GetAndShowCurrent", "Get data from shadow register and show"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            QString text;
            QByteArray resp;
            QByteArray merged;

            SET_REG("MREGX02[NUM_OF_PDCM_SLOTS]", 7);
            SET_REG("MREGX02[TX_START]", 0);
            runAppCommand("WriteREGX", "MREGX02","","","","");
            SET_REG("SLOT_LENGTH_ALL", 1351);


            int chann = GET_REG("CHANNEL").toInt();

            if (GET_REG("MREGX0A[CTM_DIV]").toInt() == 0)
            {
                SET_REG("CHANNEL", 0);
                runAppCommand("GetAllPDCMresp", "","","","","");
                text = ui->lineEditAllPDCMresp->text();
                merged = text.toLatin1();
                merged = QByteArray::fromHex(merged);
                merged = merged.remove(0, 7);

                SET_REG("CHANNEL", 1);
                runAppCommand("GetAllPDCMresp", "","","","","");
                text = ui->lineEditAllPDCMresp->text();
                resp = text.toLatin1();
                resp = QByteArray::fromHex(resp);
                resp = resp.remove(0, 7);
                merged.append(resp);
                m_current = merged;
            }
            else
            {
                if (chann == 0)
                {
                    SET_REG("CHANNEL", 1);
                }
                else
                {
                    SET_REG("CHANNEL", 0);
                }

                runAppCommand("GetAllPDCMresp", "","","","","");

                text = ui->lineEditAllPDCMresp->text();
                resp = text.toLatin1();
                resp = QByteArray::fromHex(resp);
                resp = resp.remove(0, 7);
                m_current = resp;
            }

            ShowCurrent();

            SET_REG("CHANNEL", chann);


            return true;
        }
        FUNCTION("SaveCSVCurrent", "Save the current to CSV file"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            QString filename = QFileDialog::getSaveFileName(this, "Save CSV file",QDir::currentPath(), "CSV file(*.csv)");
            QFile file(filename);
            if (file.open(QFile::WriteOnly | QFile::Truncate))
            {
                QTextStream data( &file );
                QStringList strList;
                QString delim = ",";

                data << "x" << delim << "y" << "\n";

                for (int i = 0; i < m_x.size(); i++)
                {
                    data << QString("%1").arg(m_x[i]) << delim << QString("%1").arg(m_y[i]) << "\n";
                }

                file.close();
            }

            return true;
        }
        FUNCTION("SaveBitCSVCurrent", "Save the current to CSV file in bit format"
                 ,"<i><br> no parameter"
                 ,"RETURN true/false")
        {
            QString filename = QFileDialog::getSaveFileName(this, "Save CSV file",QDir::currentPath(), "CSV file(*.csv)");
            QFile file(filename);
            if (file.open(QFile::WriteOnly | QFile::Truncate))
            {
                QTextStream data( &file );
                QStringList strList;
                QString delim = ",";

                //data << "x" << delim << "y" << "\n";

                for (int i = 0; i < m_x.size(); i++)
                {
                    //data << QString("%1").arg(m_x[i]) << delim << QString("%1").arg(m_y[i]) << "\n";
                    data << QString("%1").arg(((m_y_uint[i] >> 7) & 0x01)) << "\n";
                    data << QString("%1").arg(((m_y_uint[i] >> 6) & 0x01)) << "\n";
                    data << QString("%1").arg(((m_y_uint[i] >> 5) & 0x01)) << "\n";
                    data << QString("%1").arg(((m_y_uint[i] >> 4) & 0x01)) << "\n";
                    data << QString("%1").arg(((m_y_uint[i] >> 3) & 0x01)) << "\n";
                    data << QString("%1").arg(((m_y_uint[i] >> 2) & 0x01)) << "\n";
                    data << QString("%1").arg(((m_y_uint[i] >> 1) & 0x01)) << "\n";
                    data << QString("%1").arg(((m_y_uint[i] >> 0) & 0x01)) << "\n";
                }

                file.close();
            }

            return true;
        }

    return false;
}


void RDUMMaster::setupBoard()
{
}


void RDUMMaster::suspend()
{       
}

void RDUMMaster::resume()
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


bool RDUMMaster::GetREGXPointer(QString name, Register **preg)
{
    uint address;
    bool result = false;

    if (name.contains("MREGX"))
    {
        result = true;

        name.remove("MREGX");
        address = name.toUInt(0, 16);

        switch (address)
        {
        case 0:
           *preg = &MREGX00;
            break;
        case 1:
            *preg = &MREGX01;
            break;
        case 2:
            *preg = &MREGX02;
            break;
        case 3:
            *preg = &MREGX03;
            break;
        case 4:
            *preg = &MREGX04;
            break;
        case 5:
            *preg = &MREGX05;
            break;
        case 6:
            *preg = &MREGX06;
            break;
        case 7:
            *preg = &MREGX07;
            break;
        case 8:
            *preg = &MREGX08;
            break;
        case 9:
            *preg = &MREGX09;
            break;
        case 10:
            *preg = &MREGX0A;
            break;
        case 11:
            *preg = &MREGX0B;
            break;
        case 12:
            *preg = &MREGX0C;
            break;
        case 13:
            *preg = &MREGX0D;
            break;
        case 14:
            *preg = &MREGX0E;
            break;
        case 16:
            *preg = &MREGX10;
            break;
        case 17:
            *preg = &MREGX11;
            break;
        case 18:
            *preg = &MREGX12;
            break;
        case 19:
            *preg = &MREGX13;
            break;
        case 20:
            *preg = &MREGX14;
            break;
        case 21:
            *preg = &MREGX15;
            break;
        case 22:
            *preg = &MREGX16;
            break;
        case 23:
            *preg = &MREGX17;
            break;
        case 24:
            *preg = &MREGX18;
            break;
        case 32:
            *preg = &MREGX20;
            break;
        case 33:
            *preg = &MREGX21;
            break;
        case 34:
            *preg = &MREGX22;
            break;
        case 35:
            *preg = &MREGX23;
            break;
        case 60:
            *preg = &MREGX3C;
            break;
        case 61:
            *preg = &MREGX3D;
            break;
        case 62:
            *preg = &MREGX3E;
            break;
        case 63:
            *preg = &MREGX3F;
            break;
        case 64:
            *preg = &MREGX40;
            break;
        case 65:
            *preg = &MREGX41;
            break;
        case 66:
            *preg = &MREGX42;
            break;
        case 67:
            *preg = &MREGX43;
            break;
        case 68:
            *preg = &MREGX44;
            break;
        case 69:
            *preg = &MREGX45;
            break;
        case 70:
            *preg = &MREGX46;
            break;
        case 71:
            *preg = &MREGX47;
            break;
        case 72:
            *preg = &MREGX48;
            break;
        case 73:
            *preg = &MREGX49;
            break;
        case 74:
            *preg = &MREGX4A;
            break;
        case 75:
            *preg = &MREGX4B;
            break;
        case 76:
            *preg = &MREGX4C;
            break;
        case 79:
            *preg = &MREGX4F;
            break;
        case 80:
            *preg = &MREGX50;
            break;
        case 81:
            *preg = &MREGX51;
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


quint16 RDUMMaster::CalcEEPROMCRC(QByteArray data)
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
void RDUMMaster::UpdateVoltages()
{
    ui->labelVSUP->setText(QString("%1 V").arg(GET_REG("MREGX13[VSUP]").toFloat() * 23.69 / 512, 0, 'f', 2));
    ui->labelVDD->setText(QString("%1 V").arg(GET_REG("MREGX13[VDD]").toFloat() * 6.18 / 512, 0, 'f', 2));
    ui->labelVDIG->setText(QString("%1 V").arg(GET_REG("MREGX13[VDRV]").toFloat() * 6.18 / 512, 0, 'f', 2));

    ui->labelVIO->setText(QString("%1 V").arg(GET_REG("MREGX14[VIO]").toFloat() * 6.18 / 512, 0, 'f', 2));
    ui->labelVDATA2->setText(QString("%1 V").arg(GET_REG("MREGX14[VDATA2]").toFloat() * 23.69 / 512, 0, 'f', 2));
    ui->labelVDATA1->setText(QString("%1 V").arg(GET_REG("MREGX14[VDATA1]").toFloat() * 23.69 / 512, 0, 'f', 2));

    ui->labelVDDA_D1->setText(QString("%1 V").arg(GET_REG("MREGX15[VDDA_D1]").toFloat() * 6.18 / 512, 0, 'f', 2));
    ui->labelVDDA_D2->setText(QString("%1 V").arg(GET_REG("MREGX15[VDDA_D2]").toFloat() * 6.18 / 512, 0, 'f', 2));
    ui->labelTEMPERATURE_TJ->setText(QString("%1 C").arg((GET_REG("MREGX15[TEMP]").toFloat() - 249.0) / 0.959, 0, 'f', 2));

}

void RDUMMaster::UpdateSlotDurations()
{

}

//****************************************************************************
// @function name: PerformCOMMAND()
// @description: Perform DSI3PAS COMMAND
//****************************************************************************
bool RDUMMaster::PerformCOMMAND(QByteArray txdata, QByteArray *rxdata, int read_count)
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
                MREGX03.setValue(24, 31, txdata.at(0));
            }
            if (length > 1){
                MREGX03.setValue(16, 23, txdata.at(1));
            }
            if (length > 2){
                MREGX03.setValue(8, 15, txdata.at(2));
            }
            if (length > 3){
                MREGX03.setValue(0, 7, txdata.at(3));
            }
            if (length > 4){
                MREGX04.setValue(24, 31, txdata.at(4));
            }
            if (length > 5){
                MREGX04.setValue(16, 23, txdata.at(5));
            }
            if (length > 6){
                MREGX04.setValue(8, 15, txdata.at(6));
            }
            if (length > 7){
                MREGX04.setValue(0, 7, txdata.at(7));
            }
            if (length > 8){
                MREGX05.setValue(24, 31, txdata.at(8));
            }
            if (length > 9){
                MREGX05.setValue(16, 23, txdata.at(9));
            }
            if (length > 10){
                MREGX05.setValue(8, 15, txdata.at(10));
            }
            if (length > 11){
                MREGX05.setValue(0, 7, txdata.at(11));
            }
            if (length > 12){
                MREGX06.setValue(24, 31, txdata.at(12));
            }
            if (length > 13){
                MREGX06.setValue(16, 23, txdata.at(13));
            }
            if (length > 14){
                MREGX06.setValue(8, 15, txdata.at(14));
            }

            if(read_count == 1)
            {
                SET_REG("MREGX02[TX_START]", 1);
            }
            else if(read_count == -2)
            {
                SET_REG("MREGX02[TX_START]", 0);
            }
            SET_REG("MREGX02[BURST_ENA]", 1);
            SET_REG("MREGX02[BRC_ENA]", 0);
            SET_REG("MREGX02[N_FRAMES]", length);
            SET_REG("MREGX02[NUM_OF_PDCM_SLOTS]", gp_UartSsiController->SSI_SLOT_COUNT.value()-1);
            SET_REG("MREGX09[N_FRAMES_MSB]", 0);

            if (length > 0){
            result = result & runAppCommand("WriteREGX", "MREGX03","","","","").toBool();
            }
            if (length > 4){
            result = result & runAppCommand("WriteREGX", "MREGX04","","","","").toBool();
            }
            if (length > 8){
            result = result & runAppCommand("WriteREGX", "MREGX05","","","","").toBool();
            }
            if (length > 12){
            result = result & runAppCommand("WriteREGX", "MREGX06","","","","").toBool();
            }
            result = result & runAppCommand("WriteREGX", "MREGX09","","","","").toBool();
            result = result & runAppCommand("WriteREGX", "MREGX02","","","","").toBool();
            result = result & runAppCommand("ReadREGX", "MREGX12","","","","").toBool();
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
bool RDUMMaster::PerformCRM(QByteArray txdata, QByteArray *rxdata, int read_count)
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
            MREGX03.setValue(24, 31, txdata.at(0));
            MREGX03.setValue(16, 23, txdata.at(1));
            MREGX03.setValue(8, 15, txdata.at(2));
            MREGX03.setValue(0, 7, txdata.at(3));
            runAppCommand("AddCRC", "MREGX03","","","","");
            runAppCommand("WriteREGX", "MREGX03","","","","").toBool();
            SET_REG("MREGX02[N_FRAMES]", 1);
        }
        if (length >= 8)
        {
            MREGX04.setValue(24, 31, txdata.at(4));
            MREGX04.setValue(16, 23, txdata.at(5));
            MREGX04.setValue(8, 15, txdata.at(6));
            MREGX04.setValue(0, 7, txdata.at(7));
            runAppCommand("AddCRC", "MREGX04","","","","");
            runAppCommand("WriteREGX", "MREGX04","","","","").toBool();
            SET_REG("MREGX02[N_FRAMES]", 2);
        }
        if (length >= 12)
        {
            MREGX05.setValue(24, 31, txdata.at(8));
            MREGX05.setValue(16, 23, txdata.at(9));
            MREGX05.setValue(8, 15, txdata.at(10));
            MREGX05.setValue(0, 7, txdata.at(11));
            runAppCommand("AddCRC", "MREGX05","","","","");
            runAppCommand("WriteREGX", "MREGX05","","","","").toBool();
            SET_REG("MREGX02[N_FRAMES]", 3);
        }

        SET_REG("MREGX02[TX_START]", 1);
        SET_REG("MREGX02[BURST_ENA]", 0);
        SET_REG("MREGX02[BRC_ENA]", 0);
        SET_REG("MREGX09[N_FRAMES_MSB]", 0);

        result = result & runAppCommand("WriteREGX", "MREGX50","","","","").toBool();
        result = result & runAppCommand("WriteREGX", "MREGX51","","","","").toBool();
        result = result & runAppCommand("WriteREGX", "MREGX09","","","","").toBool();
        result = result & runAppCommand("WriteREGX", "MREGX02","","","","").toBool();
        result = result & runAppCommand("ReadREGX", "MREGX12","","","","").toBool();

        if (result)
        {
            SYMBOL_ERR = GET_REG("MREGX12[SYMBOL_ERR]").toInt();
            CRM_RECEIVED = GET_REG("MREGX12[CRM_RCVD]").toInt();
            RX_TO = GET_REG("MREGX12[RX_TO]").toInt();
            FRAME_TO = GET_REG("MREGX12[FRAME_TO]").toInt();

            if ((SYMBOL_ERR == 0) && (RX_TO == 0) && (FRAME_TO == 0) && (CRM_RECEIVED == 1))
            {
                if (length >= 12)
                {
                    result = result & runAppCommand("GetCRMResp", "MREGX20-MREGX22","","","","").toBool();
                }
                else if (length >= 8)
                {
                    result = result & runAppCommand("GetCRMResp", "MREGX20-MREGX21","","","","").toBool();
                }
                else if (length >= 4)
                {
                    result = result & runAppCommand("GetCRMResp", "MREGX20","","","","").toBool();
                }


                if (result)
                {
                    rxdata->clear();

                    if (length >= 4)
                    {
                        rxdata->append(MREGX20.value(24, 31));
                        rxdata->append(MREGX20.value(16, 23));
                        rxdata->append(MREGX20.value(8, 15));
                        rxdata->append(MREGX20.value(0, 7));
                    }
                    if (length >= 8)
                    {
                        rxdata->append(MREGX21.value(24, 31));
                        rxdata->append(MREGX21.value(16, 23));
                        rxdata->append(MREGX21.value(8, 15));
                        rxdata->append(MREGX21.value(0, 7));
                    }
                    if (length >= 12)
                    {
                        rxdata->append(MREGX22.value(24, 31));
                        rxdata->append(MREGX22.value(16, 23));
                        rxdata->append(MREGX22.value(8, 15));
                        rxdata->append(MREGX22.value(0, 7));
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

void RDUMMaster::Ver_setItemText(int index, QString text)
{
    ui->comboBoxVersionM->setItemText(index, text);
}

bool RDUMMaster::GenerateBRCsAndGetData(int slot_count, int BRC_count, int page)
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
        runAppCommand("WriteREGX", "MREGX51","","","","");

        for (brc = 0; brc < BRC_count; brc++)
        {
            SET_REG("MREGX02[N_FRAMES]", 1);
            SET_REG("MREGX02[TIMER]", 0);
            SET_REG("MREGX02[NUM_OF_PDCM_SLOTS]", slot_count - 1);

            SET_REG("MREGX02[BURST_ENA]", 0);
            SET_REG("MREGX02[BRC_ENA]", 1);
            SET_REG("MREGX02[TX_START]", 1);

            SET_REG("MREGX09[N_FRAMES_MSB]", 0);

            gp_DSI3Master->Log(LOG_TYPE_MOBD, "Generate 1 BRC:");

            runAppCommand("WriteREGX", "MREGX09","","","","");
            runAppCommand("WriteREGX", "MREGX02","","","","");

            runAppCommand("ReadREGX", "MREGX12","","","","");

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
        runAppCommand("WriteREGX", "MREGX51","","","","");

        for (brc = 0; brc < (BRC_count / 2); brc++)
        {
            SET_REG("MREGX02[N_FRAMES]", 2);
            SET_REG("MREGX02[TIMER]", 30);
            SET_REG("MREGX02[NUM_OF_PDCM_SLOTS]", slot_count - 1);

            SET_REG("MREGX02[BURST_ENA]", 0);
            SET_REG("MREGX02[BRC_ENA]", 1);
            SET_REG("MREGX02[TX_START]", 1);

            SET_REG("MREGX09[N_FRAMES_MSB]", 0);

            gp_DSI3Master->Log(LOG_TYPE_MOBD, "Generate 2 BRCs:");
            gp_DSI3Master->Log(LOG_TYPE_INFO, "Delay between 2 BRCs defined in TIMER");

            runAppCommand("WriteREGX", "MREGX09","","","","");
            runAppCommand("WriteREGX", "MREGX02","","","","");

            runAppCommand("ReadREGX", "MREGX12","","","","");

            SET_REG("SLOT_LENGTH_ALL", 16);
            SET_REG("SLOT_COUNT", slot_count);

            runAppCommand("GetAllPDCMrespShadow","","","","","");

            //data = QByteArray::fromHex(ui->lineEditAllPDCMrespShadow->text().toLatin1());

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

void RDUMMaster::SPIDecoder()
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

QString RDUMMaster::DecodeUPIDWrite(int addr, uint data)
{
    QString comment;
    QString temp;
    Register *preg;
    Register reg;
    int i;

    if(GetREGXPointer(QString("MREGX%1").arg(addr, 0, 16), &preg))
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

quint16 RDUMMaster::CalculateECC(quint16 data)
{
    quint16 ECCtable[10] = {0x7929,0x8B7B,0x6FDF,0xDFBE,0xC651,0xF587,0xEB06,0xAF25,0x5E4A,0xBC94};
    quint16 output = 0;
    int pos, bit;
    int temp;
    int value;

    for (pos = 0; pos < 10; pos++)
    {
        temp = 0;
        for (bit = 0; bit < 16; bit++)
        {
            if (((ECCtable[pos] >> bit) & 0x01) == 1)
            {
                temp = temp ^ ((data >> bit) & 0x01);
            }
        }
        output = output | (temp << pos);
    }

    return output;
}

void RDUMMaster::slot_REGX50ALLUpdate()
{
    if (ui->checkBox_x50ALL->isChecked())
    {
        SET_REG("MREGX50[31:0]", 0xFFFFFFFF);
    }
    else
    {
        SET_REG("MREGX50[31:0]", 0);
    }

    slot_updateAllControls();
}

void RDUMMaster::slot_REGX51ALLUpdate()
{
    if (ui->checkBox_x51ALL->isChecked())
    {
        SET_REG("MREGX51[31:0]", 0xFFFFFFFF);
    }
    else
    {
        SET_REG("MREGX51[31:0]", 0);
    }

    slot_updateAllControls();
}

void RDUMMaster::ShowCurrent()
{
    QVector <double> x;
    QVector <double> y;
    QVector <double> b0;
    QVector <double> b1;
    QVector <double> div(4);
    int i;
    float timesample;

    div[0] = 1; div[1] = 3; div[2] = 6; div[3] = 9;

    timesample = 0.1 * div[(GET_REG("MREGX0A[CTM_DIV]").toInt()) & 0x03];
    m_timesample = timesample;
    x.resize(m_current.size());
    m_x.resize(m_current.size());
    y.resize(m_current.size());
    b0.resize(m_current.size());
    b1.resize(m_current.size());
    m_y.resize(m_current.size());
    m_y_uint.resize(m_current.size());

    if (GET_REG("MREGX0A[CTM_SEL]").toInt() == 0)
    {
        for (i = 0; i < m_current.size(); i++)
        {
            x[i] = i * timesample;
            y[i] = (m_current.at(i) & 0xFF) * 0.85;
            m_x[i] = x[i];
            m_y[i] = (m_current.at(i) & 0xFF);
            m_y_uint[i] = (m_current.at(i) & 0xFF);
        }
    }
    else if ((GET_REG("MREGX0A[CTM_SEL]").toInt() == 2) || (GET_REG("MREGX0A[CTM_SEL]").toInt() == 3))
    {
        for (i = 0; i < m_current.size(); i++)
        {
            x[i] = i * timesample;
            y[i] = (m_current.at(i) & 0xFC) * 0.85;
            b1[i] = (m_current.at(i) & 0x2) * 40;
            b0[i] = (m_current.at(i) & 0x1) * 40;
            m_x[i] = x[i];
            m_y[i] = (m_current.at(i) & 0xFF);
            m_y_uint[i] = (m_current.at(i) & 0xFF);
        }
    }
    else if (GET_REG("MREGX0A[CTM_SEL]").toInt() == 1)
    {
        for (i = 0; i < m_current.size(); i++)
        {
            x[i] = i * timesample;
            y[i] = (m_current.at(i) & 0xFE) * 0.85;
            b0[i] = (m_current.at(i) & 0x1) * 40;
            m_x[i] = x[i];
            m_y[i] = (m_current.at(i) & 0xFF);
            m_y_uint[i] = (m_current.at(i) & 0xFF);
        }
    }

    if (ui->checkBoxZOOM->isChecked() == false)
    {
        ui->ptCurrent->xAxis->setRange(0, m_current.size() * timesample);
    }
    else
    {
        quint16 start = ui->lineEditZOOM_START->text().toInt();
        quint16 stop = ui->lineEditZOOM_STOP->text().toInt();
        if (start < stop)
        {
            ui->ptCurrent->xAxis->setRange(start, stop);
        }
        else
        {
            ui->ptCurrent->xAxis->setRange(0, m_current.size() * timesample);
        }
    }

    ui->ptCurrent->graph(0)->setData(x, y);
    ui->ptCurrent->graph(1)->setData(x, b0);
    ui->ptCurrent->graph(2)->setData(x, b1);
    ui->ptCurrent->replot();
}
