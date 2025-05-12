#ifndef MEASUREMENT_REGMAP_H
#define MEASUREMENT_REGMAP_H
#include <QString>
#include "register.h"
#include "customcontrols.h"


static const int colors[10][3] = {{255,0,0},{0,0,180},{0,120,0},{0,0,0},{250,120,0},{202,31,123},{120,120,120},{130,70,0},{51,204,255},{0,230,0}};

static const int percolors[20][3] ={{230, 25, 75},{60, 180, 75},{255, 225, 25},{0, 130, 200},{245, 130, 48},{145, 30, 180},{70, 240, 240},{240, 50, 230},{210, 245, 60},{250, 190, 190},{0, 128, 128},{230, 190, 255},{170, 110, 40},{128, 0, 0},{170, 255, 195},{128, 128, 0},{255, 215, 180},{0, 0, 128},{128, 128, 128},{0, 0, 0}};
/*
static const int percolors[20][3] ={
{230, 25, 75},\ //red
{60, 180, 75},\ //green
{255, 225, 25},\ //yellow
{0, 130, 200},\ //blue
{245, 130, 48},\ //orange
{145, 30, 180},\ //purple
{70, 240, 240},\ //cyan
{240, 50, 230},\ //magenta
{210, 245, 60},\ //lime
{250, 190, 190},\ //pink
{0, 128, 128},\ //teal
{230, 190, 255},\ //lavander
{170, 110, 40},\ //brown
{128, 0, 0},\ //maroon
{170, 255, 195},\ //mint
{128, 128, 0},\ //olive
{255, 215, 180},\ //apricot
{0, 0, 128},\ //navy
{128, 128, 128},\ //grey
{0, 0, 0}}; //black*/

static const char *MEASPERIOD_INIT[] = {"200ms", "500ms", "1s", "2s"};
// COMMAND REGISTERS


static const char *ENA_1_10_INIT={"@register=REG;"
                                            "EN1=1;"
                                            "EN2=1;"
                                            "EN3=1;"
                                            "EN4=1;"
                                            "EN5=1;"
                                            "EN6=1;"
                                            "EN7=1;"
                                            "EN8=1;"
                                            "EN9=1;"
                                            "EN10=1;"
};

static const char *DIAGWORD1_INIT={"@register=REG;"
                                   "time[5:0]#Reverberation Time (6 bits, 1 LSB is 51,2 us);"
                                   "period[15:6]#Reverberation Period (10 bits, 1 LSB is 25 ns);"
};

static const char *DIAGWORD2_INIT={"@register=REG;"
                                   "freq_trim_actual[6:0]#Actual OSC fine trimming code;"
                                   "tx_ov_red#TX overvoltage current reduction OR TX under voltage;"
                                   "die_temp[15:8]#Sensor temperature;"
};

static const char *DIAGWORD0_INIT={"@register=REG @abc=intee;"
                                   "@field=reserved[1:0]   @descr=Reserved for future use     @message=none;"
                                   "@field=CWNL[5:2]       @descr=Continuous wave noise value L, correlator full range/8     @message=CW noise L;"
                                   "@field=CWNH[9:6]       @descr=Continuous wave noise value H, correlator full range/8     @message=CW noise H;"
                                   "@field=SAFETY          @descr=OR of all safety flags                                     @message=Safety flag;"
                                   "@field=TXoverV         @descr=TX overvoltage current reduction OR TX under voltage       @message=TX overvolt;"
                                   "@field=WBN             @descr=Wide band Noise above configured threshold                 @message=WB noise;"
                                   "@field=ModeCRCerr      @descr=Mode CRC error                                             @message=Mode CRC err;"
                                   "@field=ThermalWarning  @descr=Thermal Warning - Temperature threshold reached            @message=Thermal Warn;"
                                   "@field=ThermalShutdown @descr=Thermal Shutdown (only based junction temperature measurement)    @message=Therm Shutdwn;"
};

/*static const char *SWriteREAD_INIT={"@register=SWriteREAD;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "rd_idx[15:8]#Memory index within page to be read;"
                                    "rd_pg[17:16]#Memory page to be read;"
                                    "@field=none[23:18]    @value=0    @descr=Unused;"
                                    "@field=READ[27:24]    @value=9    @readonly=1    @descr=Command code;"
                                    "sid[31:28]#Slave node identifier;"};
*/

static const char *ENA_1_8_INIT={"@register=REG;"
                                            "EN1=1;"
                                            "EN2=1;"
                                            "EN3=1;"
                                            "EN4=1;"
                                            "EN5=1;"
                                            "EN6=1;"
                                            "EN7=1;"
                                            "EN8=1;"
};

#endif // MEASUREMENT_REGMAP_H
