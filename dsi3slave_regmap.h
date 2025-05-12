#ifndef DSI3SLAVER_REGMAP_H
#define DSI3SLAVER_REGMAP_H
#include <QString>
#include "register.h"
#include "customcontrols.h"


static const quint32 mode_start_addr[]={0x30, 0x52, 0x74, 0x96, 0xB8, 0xDA, 0xDA, 0xDA};

static const quint32 meas_type_recalc_OldToNew[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,31,31,21,22,23,24,25,31,26,27,31};
static const quint32 meas_type_recalc_NewToOld[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,23,24,25,26,27,29,30,31,31,31,31};

static const quint32 comp_method_recalc_OldToNew[]={0,0,1,1,2,3,0,5,0,9, 6, 6, 8, 0, 0, 6};
static const quint32 comp_method_recalc_NewToOld[]={8,3,4,5,2,7,15,14,12,9, 0, 0, 0, 0, 0, 0};


static const char *_REGS_INIT[]={
    // 0x00
    "@register=REG @page=0 @adr=00 @type=ReadOnly @purpose=read;"
    "@field=icmfgid[7:0]        @descr=IC Manufactory ID;"
    "@field=ictypeid[15:8]      @descr=IC Type ID;"

    // 0x08
    ,"@register=REG @page=0 @adr=08 @type=ReadOnly @purpose=read;"
    "@field=sensor_id[7:0]       @descr=SENSOR_ID: 0 is not identified, 1-6 is sensor position – sampled from connector;"
    "@field=_[15:8];"

    // 0x10
    ,"@register=REG @page=0 @adr=10 @type=CFAR @purpose=CFAR;"
    "@field=dir_cfar_snr_scale[2:0]        @descr=Direct CFAR SNR Scale;"
    "@field=indir_cfar_snr_scale[5:3]       @descr=Indirect CFAR SNR Scale;"
    "@field=_[15:6];"

    // 0x0A
    ,"@register=REG @page=0 @adr=0A @type=Common @purpose=common;"
    "@field=unlock_id            @descr=UNLOCK_ID: 0 is settings locked, 1 is settings unlocked;"
    "@field=_[7:1];"
    "@field=unlock_nvm           @descr=UNLOCK_NVM: 0 is settings locked, 1 is settings unlocked;"
    "@field=nvm_busy             @readonly=1 @descr=NVM_BUSY: 1 - NVM programming is running;"
    "@field=unlock_sw_rst        @descr=UNLOCK_SW_RST: 0 is settings locked, 1 is settings unlocked;"
    "@field=_[15:11];"

    // 0x0C
    ,"@register=REG @page=0 @adr=0C @type=ReadOnly @purpose=read;"
    "@field=gp_status[15:0]      @descr=General purpose status bits from DSP;"

    // 0x0E
    ,"@register=REG @page=0 @adr=0E @type=Gpcontrol @purpose=gpcontrol;"
    "@field=gp_control[15:0]     @descr=General purpose control bits to DSP;"

    //0x20
    ,"@register=REG @page=0 @adr=20 @type=ReadOnly @purpose=read;"
    "@field=icrev[15:0]                  @descr=ASIC revision identifier;"

    //0x22
    ,"@register=REG @page=0 @adr=22 @type=NVMON @purpose=nvmon;"
    "@field=nvmon1[15:0]                 @descr=NVM memory register;"

    //0x24
    ,"@register=REG @page=0 @adr=24 @type=NVMON @purpose=nvmon;"
    "@field=nvmon2[15:0]                 @descr=NVM memory register;"

    //0x26
    ,"@register=REG @page=0 @adr=26 @type=NVMON @purpose=nvmon;"
    "@field=nvmon3[15:0]                 @descr=NVM memory register;"

    //0x28
    ,"@register=REG @page=0 @adr=28 @type=NVMON @purpose=nvmon;"
    "@field=nvmon4[15:0]                 @descr=NVM memory register;"

    //0x2A
    ,"@register=REG @page=0 @adr=2A @type=NVMON @purpose=nvmon;"
    "@field=nvmon5[15:0]                 @descr=NVM memory register;"

    //0x2C
    ,"@register=REG @page=0 @adr=2C @type=NVMON @purpose=nvmon;"
    "@field=nvmon6[15:0]                 @descr=NVM memory register;"

    //0x2E
    ,"@register=REG @page=0 @adr=2E @type=NVMON @purpose=nvmon;"
    "@field=nvmon7[15:0]                 @descr=NVM memory register;"

    //0x30
    ,"@register=REG @page=0 @adr=30 @type=NVMON @purpose=nvmon;"
    "@field=crc_nvm_on[15:0]             @descr=16 bit CRC  protecting NVM ON Semi area;"


    //-----START MODE CONFIG-----
    //0x40
    ,"@register=REG @page=0 @adr=40 @type=Mode @purpose=mode;"
    "@field=tx_per_l[9:0]                @descr=Actual Center chirp period or AM period channel L, 1LSB is 25ns;"
    "@field=dir_cfar_scale[12:10]        @descr=Scaling of Direct Measurement CFAR Threshold;"
    "@field=indir_cfar_scale[15:13]      @descr=Scaling of Indirect Measurement CFAR Threshold;"

    //0x42
    ,"@register=REG @page=0 @adr=42 @type=Mode @purpose=mode;"
    "@field=tx_per_h[9:0]                @descr=Actual Center chirp period  or AM period channel H;"
    "@field=meas_type[14:10]             @descr=Actual measurement type, total allowed 32 measurement;"
    "@field=dbg_ena                      @descr=Debug output enable;"

    //0x44
    ,"@register=REG @page=0 @adr=44 @type=Mode @purpose=mode;"
    "@field=corr_l_scale_r[4:0]          @descr=Actual Correlator scale factor, listen from right, channel L;"
    "@field=corr_l_scale[9:5]            @descr=Actual Correlator scale factor, listen myself, channel L;"
    "@field=corr_l_scale_l[14:10]        @descr=Actual Correlator scale factor, listen from left, channel L;"
    "@field=_44;"

    //0x46
    ,"@register=REG @page=0 @adr=46 @type=Mode @purpose=mode;"
    "@field=corr_h_scale_r[4:0]          @descr=Actual Correlator scale factor, listen from right, channel H;"
    "@field=corr_h_scale[9:5]            @descr=Actual Correlator scale factor, listen myself, channel H;"
    "@field=corr_h_scale_l[14:10]        @descr=Actual Correlator scale factor, listen from left, channel H;"
    "@field=_46;"

    //0x48
    ,"@register=REG @page=0 @adr=48 @type=Mode @purpose=mode;"
    "@field=tx_current_l[6:0]            @descr=Transmit Current Index  channel L;"
    "@field=tx_current_h[13:7]           @descr=Transmit Current Index  channel H;"
    "@field=dbg_output_sel[15:14]        @descr=Select debug output;"

    //0x4A
    ,"@register=REG @page=0 @adr=4A @type=Mode @purpose=mode;"
    "@field=dyn_gain_offset[6:0]         @descr=Actual Offset of gain curve, 1LSB is 0.38dB;"
    "@field=dbg_output_sel[10:7]         @descr=Select debug output;"
    "@field=dbg_rx_ena                   @descr=Enable (1) or disable (0) debug output;"
    "@field=wb_noise_th[14:12]           @descr=Correlator full range /8, correlator full range is 960 decimal, linear coded from 0 to 120 in 3-bits;"
    "@field=rev_sh_ena                   @descr=Actual Control of short during reverberation;"

    //0x4C
    ,"@register=REG @page=0 @adr=4C @type=Mode @purpose=mode;"
    "@field=gain_lna[1:0]                @descr=Actual Control of LNA gain stage, 0 is 20dB, 1 is 30dB, 2 is 40dB, 3 is 40dB;"
    "@field=tx_damp[7:2]                 @descr=Actual Control of damping resistors;"
    "@field=rev_end_timeout[13:8]        @descr=Actual Reverberation Time-out, 1LSB is 51.2us;"
    "@field=_4C[15:14];"

    //0x4E
    ,"@register=REG @page=0 @adr=4E @type=Mode @purpose=mode;"
    "@field=delta_gain_val0[6:0]         @descr=Actual Delta gain value 0, 1LSB is 0.38dB;"
    "@field=delta_gain_tim0[10:7]        @descr=Actual Delta gain time 0, 1LSB is 409.6us;"
    "@field=init_phase_dur[15:11]        @descr=Duration of initial phase, from reception of valid MEASURE command until end of beaming. 1LSB is 512us;"

    //0x50
    ,"@register=REG @page=0 @adr=50 @type=Mode @purpose=mode;"
    "@field=delta_gain_val1[6:0]         @descr=Actual Delta gain value 1;"
    "@field=delta_gain_tim1[10:7]        @descr=Actual Delta gain time 1;"
    "@field=_50;"
    "@field=tx_meas_gain[13:12]          @descr=Actual Control of SD converter range, 0 is high voltage meas channel for diag modes, 1 is normal meas channel;"
    "@field=rev_sh[15:14]                @descr=Actual Control of Impedance in the short in case rev_sh_ena is set to 1. Rev_sh=0 is 1.4Ohm, 1 is 2 Ohm, 2 is 2,8 Ohm, 3 is 7 Ohm;"

    //0x52
    ,"@register=REG @page=0 @adr=52 @type=Mode @purpose=mode;"
    "@field=delta_gain_val2[6:0]         @descr=Actual Delta gain value 2;"
    "@field=delta_gain_tim2[10:7]        @descr=Actual Delta gain time 2;"
    "@field=comp_method[13:11]           @descr=Actual Selection of DSI3PAS compression mode;"
    "@field=vsup_tx_th[15:14]            @descr=VSUP_TX low threshold setting;"

    //0x54
    ,"@register=REG @page=0 @adr=54 @type=Mode @purpose=mode;"
    "@field=delta_gain_val3[6:0]         @descr=Actual Delta gain value 3;"
    "@field=delta_gain_tim3[10:7]        @descr=Actual Delta gain time 3;"
    "@field=var_gain_enable              @descr=Actual variable gain enable (1) or disable (0);"
    "@field=var_gain_start[15:12]        @descr=Actual Variable gain start, 1LSB is 409.6us;"

    //0x56
    ,"@register=REG @page=0 @adr=56 @type=Mode  @purpose=mode;"
    "@field=delta_gain_val4[6:0]         @descr=Actual Delta gain value 4;"
    "@field=delta_gain_tim4[10:7]        @descr=Actual Delta gain time 4;"
    "@field=_56;"
    "@field=echo_type[15:12]             @descr=Echo data format selection "
                            "<ul>"
                            "<li>0 is Direct Correlator Data in uncompressed format, for Diag Mode ONLY (single sensor ONLY)</li>"
                            "<li>1 is CFAR echo magnitude and CFAR threshold CH H, for Diag mode only</li>"
                            "<li>2 is CFAR echo magnitude and CFAR threshold CH L, for Diag mode only</li>"
                            "<li>3 is CFAR echo magnitude</li>"
                            "<li>4 is CFAR ToF</li>"
                            "<li>Others is CFAR ToF</li>"
                            "</ul>;"

    //0x58
    ,"@register=REG @page=0 @adr=58 @type=Mode @purpose=mode;"
    "@field=meas_duration[6:0]           @descr=Measurement duration, 1LSB is 512us;"
    "@field=comp_method_b3               @descr=Actual selection of DSI3PAS compression mode;"
    "@field=zif_scale[11:8]              @descr=Actual ZIF to Correlator magnitude scale factor;"
    "@field=none[15:12]                  @descr=none;"

    //0x5A
    ,"@register=REG @page=0 @adr=5A @type=Mode @purpose=mode;"
    "@field=dir_cfar_thr_off[7:0]        @descr=Direct CFAR Threshold Offset;"
    "@field=dir_cfar_der_thr[13:8]       @descr=Direct CFAR Derivation Threshold;"
    "@field=dir_cfar_der_size[15:14]     @descr=Direct CFAR Derivation Size;"

    //0x5C
    ,"@register=REG @page=0 @adr=5C @type=Mode @purpose=mode;"
    "@field=indir_cfar_thr_off[7:0]      @descr=Indirect CFAR Threshold Offset;"
    "@field=indir_cfar_der_thr[13:8]     @descr=Indirect CFAR Derivation Threshold;"
    "@field=indir_cfar_der_size[15:14]   @descr=Indirect CFAR Derivation Size;"

    //0x5E
    ,"@register=REG @page=0 @adr=5E @type=Mode @purpose=mode;"
    "@field=autocorrel_suppr_h[2:0]       @descr=autocorrel_suppr setting;"
    "@field=autocorrel_suppr_l[5:3]       @descr=autocorrel_suppr setting;"
    "@field=empty[15:6]  @descr=none;"

    //0x60
    ,"@register=REG  @page=0 @adr=60 @type=Mode @purpose=mode;"
    "@field=crc_mode_config[15:0]        @descr=Actual CRC16 of DSP parameters;"
    //----END CONFIG-----


    // page1 0x00
    ,"@register=REG @page=1 @adr=0  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust1[15:0]               @descr=NVM memory register;"

    // page1 0x02
    ,"@register=REG @page=1 @adr=2  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust2[15:0]               @descr=NVM memory register;"

    // page1 0x04
    ,"@register=REG @page=1 @adr=4  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust3[15:0]               @descr=NVM memory register;"

    // page1 0x06
    ,"@register=REG @page=1 @adr=6  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust4[15:0]               @descr=NVM memory register;"

    // page1 0x08
    ,"@register=REG @page=1 @adr=8  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust5[15:0]               @descr=NVM memory register;"

    // page1 0x0A
    ,"@register=REG @page=1 @adr=A  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust6[15:0]               @descr=NVM memory register;"

    // page1 0x0C
    ,"@register=REG @page=1 @adr=C  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust7[15:0]               @descr=NVM memory register;"

    // page1 0x0E
    ,"@register=REG @page=1 @adr=E  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust8[15:0]               @descr=NVM memory register;"

    // page1 0x10
    ,"@register=REG @page=1 @adr=10  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust9[15:0]               @descr=NVM memory register;"

    // page1 0x12
    ,"@register=REG @page=1 @adr=12  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust10[15:0]              @descr=NVM memory register;"

    // page1 0x14
    ,"@register=REG @page=1 @adr=14  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust11[15:0]              @descr=NVM memory register;"

    // page1 0x16
    ,"@register=REG @page=1 @adr=16  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust12[15:0]              @descr=NVM memory register;"

    // page1 0x18
    ,"@register=REG @page=1 @adr=18  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust13[15:0]              @descr=NVM memory register;"

    // page1 0x1A
    ,"@register=REG @page=1 @adr=1A  @type=NVMCust @purpose=nvmcust;"
    "@field=crc_nvm_cust[15:0]           @descr=CRC NVM memory register;"

    // page1 0x1C
    ,"@register=REG @page=1 @adr=1C @type=Safety @purpose=safety;"
    "@field=sup_ov_3v3                  @descr=Internal analog power supply (3V3) over voltage detection;"
    "@field=sup_ov_1v8                  @descr=Internal digital power supply (1V8) over voltage detection;"
    "@field=dig_reset                   @descr=Internal digital in reset state;"
    "@field=lpm                         @descr=Low Power Mode activated;"
    "@field=ths                         @descr=Thermal Shutdown activated, ths_tx disables beaming;"
    "@field=drvc_faildet                @descr=DRVC pin voltage  unexpected behavior detection. During transmitting DRVC pin shall be high and during echo reception DRVC pin shall be low;"
    "@field=tx_ndmos_faildet            @descr=Internal TX driver high voltage cascode fail. Internal short of output TX driver transistors detection;"
    "@field=tx_curr_faildet             @descr=Internal TX driver current source fail (GS short of LV NOMOS current source). Internal short of low voltage output TX driver current source detection;"
    "@field=tx_no_curr_faildet          @descr=No TX driver current detected. Open circuit from power supply to internal TX driver (open ASIC pins, open transformer pins, etc.) detection;"
    "@field=rxn_inp_faildet             @descr=Receiver input fail detected - pin RXN. Open/short detection on pin RXN;"
    "@field=rxp_inp_faildet             @descr=Receiver input fail detected - pin RXP. Open/short detection on pin RXP;"
    "@field=rx_cmmo_faildet             @descr=Receiver common modes detection. Internal hardware failure inside receiver detection;"
    "@field=dsp_lbist_faildet           @descr=DSP Logic BIST failed;"
    "@field=mode_config_crc             @descr=Mode config CRC;"
    "@field=onsemi_eeprom_crc           @descr=ON Semi part EEPROM CRC failed;"
    "@field=cust_eeprom_crc             @descr=Customer part EEPROM CRC failed;"

    // page1 0x1E
    ,"@register=REG @page=1 @adr=1E @type=Safety @purpose=safety;"
    "@field=eeprom_prog_fail             @descr=EEPROM programing fail or VSUP_RX voltage too low for EEPROM programming;"
    "@field=safety_status_1[15:1]        @descr=Safety status register 1;"

    // page1 0x20
    ,"@register=REG @page=1 @adr=20 @type=DSI3 @purpose=DSI3;"
    "@field=pdcm_size[1:0]              @descr=PDCM_SIZE 3 is 16 bytes;"
    "@field=tx_filter_ena               @descr=1 - Enable additional TX filter to improve emissions of Slave to Master communication;"
    "@field=dsi3pas_ref                 @descr=0 is reference to vbat/2, 1 is reference to GND;"
    "@field=pdcm_start[5:4]             @descr=PDCM start time;"
    "@field=crm_start[7:6]              @descr=CRM_START time;"
    "@field=t_chip                      @descr=T_CHIP: 0 is 3us;"
    "@field=crc_mode                    @descr=CRC in PDCM mode: 0 is 8-bit;"
    "@field=scrambler_ena               @descr=Enable (1) or disable (0) data scrambler;"
    "@field=dsi3pas_vpp                 @descr=DSI3 Vpp signaling level – 0 is 4Vpp, 1 is 2Vpp;"
    "@field=mount_id[14:12]             @descr=Sensor mounting position counted from the leftomost counterclockwise sensor;"
    "@field=vsds                        @descr=VSDS to determine for PDCM_SLOT number"
                            "<ul>"
                            "<li>0 - use Address equal to Sensor_ID (max 6 slots in a PDCM cycle)</li>"
                            "<li>1 - use Dynamic Slot Distribution (max 12 slots)</li>"
                            "</ul>;"

    // page1 0x22
    ,"@register=REG @page=1 @adr=22 @type=DSI3 @purpose=DSI3;"
    "@field=crc_dsi_config[7:0]         @descr=CRC-8 of DSI3PAS MAIN SETTINGS at addr. 0x120;"
    "@field=gain_clamp[15:8]       @value=255     @descr=Gain clamp, set 255;"

    // page1 0x26
    ,"@register=REG @page=1 @adr=26 @type=Common @purpose=Common;"
    "@field=ovt_threshold[7:0]          @descr=Over-temperature threshold, 0 is -55 dgC, 255 is 200 dgC;"
    "@field=die_temp[15:8]              @descr=Die temperature, 0 is -55 dgC, 255 is 200 dgC;"

    // page1 0x2E
    ,"@register=REG @page=1 @adr=2E @type=Trim @purpose=Trim;"
    "@field=freq_trim_factor[7:0]    @descr=trimming factor for the frequency of the sensor local clock  - signed 8-bit value (s8);"
    "@field=freq_trim_actual[15:8]   @descr=Actual trimming value of sensor local clock;"

    // end
    ,0
};

static const char *_REGS_INITv2[]={
    // 0x00
    "@register=REG @page=0 @adr=00 @type=ReadOnly @purpose=read;"
    "@field=icmfgid[7:0]        @descr=IC Manufactory ID;"
    "@field=ictypeid[15:8]      @descr=IC Type ID;"

    // 0x08
    ,"@register=REG @page=0 @adr=08 @type=ReadOnly @purpose=read;"
    "@field=sensor_id[7:0]       @descr=SENSOR_ID: 0 is not identified, 1-6 is sensor position – sampled from connector;"
    "@field=_[15:8];"

    // 0x10
    ,"@register=REG @page=0 @adr=10 @type=CFAR @purpose=CFAR;"
    "@field=dir_cfar_snr_scale[2:0]        @descr=Direct CFAR SNR Scale;"
    "@field=indir_cfar_snr_scale[5:3]       @descr=Indirect CFAR SNR Scale;"
    "@field=_[15:6];"

    // 0x0A
    ,"@register=REG @page=0 @adr=0A @type=Common @purpose=common;"
    "@field=unlock_id            @descr=UNLOCK_ID: 0 is settings locked, 1 is settings unlocked;"
    "@field=_[7:1];"
    "@field=unlock_nvm           @descr=UNLOCK_NVM: 0 is settings locked, 1 is settings unlocked;"
    "@field=nvm_busy             @readonly=1 @descr=NVM_BUSY: 1 - NVM programming is running;"
    "@field=unlock_sw_rst        @descr=UNLOCK_SW_RST: 0 is settings locked, 1 is settings unlocked;"
    "@field=_[15:11];"

    // 0x0C
    ,"@register=REG @page=0 @adr=0C @type=ReadOnly @purpose=read;"
    "@field=gp_status[15:0]      @descr=General purpose status bits from DSP;"

    // 0x0E
    ,"@register=REG @page=0 @adr=0E @type=Gpcontrol @purpose=gpcontrol;"
    "@field=gp_control[15:0]     @descr=General purpose control bits to DSP;"

    //0x20
    ,"@register=REG @page=0 @adr=20 @type=ReadOnly @purpose=read;"
    "@field=icrev[15:0]                  @descr=ASIC revision identifier;"

    //0x22
    ,"@register=REG @page=0 @adr=22 @type=NVMON @purpose=nvmon;"
    "@field=nvmon1[15:0]                 @descr=NVM memory register;"

    //0x24
    ,"@register=REG @page=0 @adr=24 @type=NVMON @purpose=nvmon;"
    "@field=nvmon2[15:0]                 @descr=NVM memory register;"

    //0x26
    ,"@register=REG @page=0 @adr=26 @type=NVMON @purpose=nvmon;"
    "@field=nvmon3[15:0]                 @descr=NVM memory register;"

    //0x28
    ,"@register=REG @page=0 @adr=28 @type=NVMON @purpose=nvmon;"
    "@field=nvmon4[15:0]                 @descr=NVM memory register;"

    //0x2A
    ,"@register=REG @page=0 @adr=2A @type=NVMON @purpose=nvmon;"
    "@field=nvmon5[15:0]                 @descr=NVM memory register;"

    //0x2C
    ,"@register=REG @page=0 @adr=2C @type=NVMON @purpose=nvmon;"
    "@field=nvmon6[15:0]                 @descr=NVM memory register;"

    //0x2E
    ,"@register=REG @page=0 @adr=2E @type=NVMON @purpose=nvmon;"
    "@field=nvmon7[15:0]                 @descr=NVM memory register;"

    //0x30
    ,"@register=REG @page=0 @adr=30 @type=NVMON @purpose=nvmon;"
    "@field=crc_nvm_on[15:0]             @descr=16 bit CRC  protecting NVM ON Semi area;"


    //-----START MODE CONFIG-----
    //0x40
    ,"@register=REG @page=0 @adr=40 @type=Mode @purpose=mode;"
    "@field=tx_per_l[9:0]                @descr=Actual Center chirp period or AM period channel L, 1LSB is 25ns;"
    "@field=scan_near_thr[14:10]         @descr= ;"
    "@field=dyn_gain_offset_6            @descr=Actual Offset of gain curve;"

    //0x42
    ,"@register=REG @page=0 @adr=42 @type=Mode @purpose=mode;"
    "@field=tx_per_h[9:0]                @descr=Actual Center chirp period  or AM period channel H;"
    "@field=dyn_gain_offset_50[15:10]    @descr=Actual Offset of gain curve;"

    //0x44
    ,"@register=REG @page=0 @adr=44 @type=Mode @purpose=mode;"
    "@field=corr_l_scale_r[4:0]          @descr=Actual Correlator scale factor, listen from right, channel L;"
    "@field=corr_l_scale[9:5]            @descr=Actual Correlator scale factor, listen myself, channel L;"
    "@field=corr_l_scale_l[14:10]        @descr=Actual Correlator scale factor, listen from left, channel L;"
    "@field=_44;"

    //0x46
    ,"@register=REG @page=0 @adr=46 @type=Mode @purpose=mode;"
    "@field=corr_h_scale_r[4:0]          @descr=Actual Correlator scale factor, listen from right, channel H;"
    "@field=corr_h_scale[9:5]            @descr=Actual Correlator scale factor, listen myself, channel H;"
    "@field=corr_h_scale_l[14:10]        @descr=Actual Correlator scale factor, listen from left, channel H;"
    "@field=_46;"

    //0x48
    ,"@register=REG @page=0 @adr=48 @type=Mode @purpose=mode;"
    "@field=tx_current_l[6:0]            @descr=Transmit Current Index  channel L;"
    "@field=tx_current_h[13:7]           @descr=Transmit Current Index  channel H;"
    "@field=dbg_rx_ena                   @descr=Enable (1) or disable (0) debug output;"
    "@field=dbg_ena                      @descr=Debug output enable;"

    //0x4A
    ,"@register=REG @page=0 @adr=4A @type=Mode @purpose=mode;"
    "@field=meas_duration[6:0]           @descr=Measurement duration 1LSB is 512us;"
    "@field=meas_type[11:7]              @descr=Actual measurement type, total allowed 32 measurement;"
    "@field=wb_noise_th[14:12]           @descr=Correlator full range /8, correlator full range is 960 decimal, linear coded from 0 to 120 in 3-bits;"
    "@field=rev_sh_ena                   @descr=Actual Control of short during reverberation;"

    //0x4C
    ,"@register=REG @page=0 @adr=4C @type=Mode @purpose=mode;"
    "@field=gain_lna[1:0]                @descr=Actual Control of LNA gain stage, 0 is 20dB, 1 is 30dB, 2 is 40dB, 3 is 40dB;"
    "@field=tx_damp[7:2]                 @descr=Actual Control of damping resistors;"
    "@field=rev_end_timeout[13:8]        @descr=Actual Reverberation Time-out, 1LSB is 51.2us;"
    "@field=_4C[15:14];"

    //0x4E
    ,"@register=REG @page=0 @adr=4E @type=Mode @purpose=mode;"
    "@field=delta_gain_val0[6:0]         @descr=Actual Delta gain value 0, 1LSB is 0.38dB;"
    "@field=delta_gain_tim0[10:7]        @descr=Actual Delta gain time 0, 1LSB is 409.6us;"
    "@field=init_phase_dur[15:11]        @descr=Duration of initial phase, from reception of valid MEASURE command until end of beaming. 1LSB is 512us;"

    //0x50
    ,"@register=REG @page=0 @adr=50 @type=Mode @purpose=mode;"
    "@field=delta_gain_val1[6:0]         @descr=Actual Delta gain value 1;"
    "@field=delta_gain_tim1[10:7]        @descr=Actual Delta gain time 1;"
    "@field=_50;"
    "@field=tx_meas_gain[13:12]          @descr=_;"
    "@field=rev_sh[15:14]                @descr=Actual Control of Impedance in the short in case rev_sh_ena is set to 1. Rev_sh=0 is 1.4Ohm, 1 is 2 Ohm, 2 is 2,8 Ohm, 3 is 7 Ohm;"

    //0x52
    ,"@register=REG @page=0 @adr=52 @type=Mode @purpose=mode;"
    "@field=delta_gain_val2[6:0]         @descr=Actual Delta gain value 2;"
    "@field=delta_gain_tim2[10:7]        @descr=Actual Delta gain time 2;"
    "@field=comp_method[13:11]           @descr=Actual Selection of DSI3PAS compression mode;"
    "@field=vsup_tx_th[15:14]            @descr=VSUP_TX low threshold setting;"

    //0x54
    ,"@register=REG @page=0 @adr=54 @type=Mode @purpose=mode;"
    "@field=delta_gain_val3[6:0]         @descr=Actual Delta gain value 3;"
    "@field=delta_gain_tim3[10:7]        @descr=Actual Delta gain time 3;"
    "@field=var_gain_enable              @descr=Actual variable gain enable (1) or disable (0);"
    "@field=var_gain_start[15:12]        @descr=Actual Variable gain start, 1LSB is 409.6us;"

    //0x56
    ,"@register=REG @page=0 @adr=56 @type=Mode  @purpose=mode;"
    "@field=delta_gain_val4[6:0]         @descr=Actual Delta gain value 4;"
    "@field=delta_gain_tim4[10:7]        @descr=Actual Delta gain time 4;"
    "@field=indir_cfar_scale[13:11]      @descr=Scaling of Indirect Measurement CFAR Threshold;"
    "@field=_56[15:14]                   @descr= ;"

    //0x58
    ,"@register=REG @page=0 @adr=58 @type=Mode @purpose=mode;"
    "@field=dbg_output_sel[5:0]          @descr=Measurement duration, 1LSB is 512us;"
    "@field=_58                          @descr=Actual selection of DSI3PAS compression mode;"
    "@field=zif_scale[10:7]              @descr=Actual ZIF to Correlator magnitude scale factor;"
    "@field=dir_cfar_scale[13:11]        @descr=Scaling of Direct Measurement CFAR Threshold;"
    "@field=comp_method_b3               @descr=Actual Selection of DSI3ON compression mode;"
    "@field=_581                         @descr= ;"

    //0x5A
    ,"@register=REG @page=0 @adr=5A @type=Mode @purpose=mode;"
    "@field=dir_cfar_thr_off[7:0]        @descr=Direct CFAR Threshold Offset;"
    "@field=dir_cfar_der_thr[13:8]       @descr=Direct CFAR Derivation Threshold;"
    "@field=dir_cfar_der_size[15:14]     @descr=Direct CFAR Derivation Size;"

    //0x5C
    ,"@register=REG @page=0 @adr=5C @type=Mode @purpose=mode;"
    "@field=indir_cfar_thr_off[7:0]      @descr=Indirect CFAR Threshold Offset;"
    "@field=indir_cfar_der_thr[13:8]     @descr=Indirect CFAR Derivation Threshold;"
    "@field=indir_cfar_der_size[15:14]   @descr=Indirect CFAR Derivation Size;"

    //0x5E
    ,"@register=REG @page=0 @adr=5E @type=Mode @purpose=mode;"
    "@field=autocorrel_suppr_h[2:0]       @descr=autocorrel_suppr setting;"
    "@field=autocorrel_suppr_l[5:3]       @descr=autocorrel_suppr setting;"
    "@field=empty[15:6]  @descr=none;"

    //0x60
    ,"@register=REG  @page=0 @adr=60 @type=Mode @purpose=mode;"
    "@field=crc_mode_config[15:0]        @descr=Actual CRC16 of DSP parameters;"
    //----END CONFIG-----


    // page1 0x00
    ,"@register=REG @page=1 @adr=0  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust1[15:0]               @descr=NVM memory register;"

    // page1 0x02
    ,"@register=REG @page=1 @adr=2  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust2[15:0]               @descr=NVM memory register;"

    // page1 0x04
    ,"@register=REG @page=1 @adr=4  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust3[15:0]               @descr=NVM memory register;"

    // page1 0x06
    ,"@register=REG @page=1 @adr=6  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust4[15:0]               @descr=NVM memory register;"

    // page1 0x08
    ,"@register=REG @page=1 @adr=8  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust5[15:0]               @descr=NVM memory register;"

    // page1 0x0A
    ,"@register=REG @page=1 @adr=A  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust6[15:0]               @descr=NVM memory register;"

    // page1 0x0C
    ,"@register=REG @page=1 @adr=C  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust7[15:0]               @descr=NVM memory register;"

    // page1 0x0E
    ,"@register=REG @page=1 @adr=E  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust8[15:0]               @descr=NVM memory register;"

    // page1 0x10
    ,"@register=REG @page=1 @adr=10  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust9[15:0]               @descr=NVM memory register;"

    // page1 0x12
    ,"@register=REG @page=1 @adr=12  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust10[15:0]              @descr=NVM memory register;"

    // page1 0x14
    ,"@register=REG @page=1 @adr=14  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust11[15:0]              @descr=NVM memory register;"

    // page1 0x16
    ,"@register=REG @page=1 @adr=16  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust12[15:0]              @descr=NVM memory register;"

    // page1 0x18
    ,"@register=REG @page=1 @adr=18  @type=NVMCust @purpose=nvmcust;"
    "@field=nvmcust13[15:0]              @descr=NVM memory register;"

    // page1 0x1A
    ,"@register=REG @page=1 @adr=1A  @type=NVMCust @purpose=nvmcust;"
    "@field=crc_nvm_cust[15:0]           @descr=CRC NVM memory register;"

    // page1 0x1C
    ,"@register=REG @page=1 @adr=1C @type=Safety @purpose=safety;"
    "@field=sup_ov_3v3                  @descr=Internal analog power supply (3V3) over voltage detection;"
    "@field=sup_ov_1v8                  @descr=Internal digital power supply (1V8) over voltage detection;"
    "@field=dig_reset                   @descr=Internal digital in reset state;"
    "@field=lpm                         @descr=Low Power Mode activated;"
    "@field=ths                         @descr=Thermal Shutdown activated, ths_tx disables beaming;"
    "@field=drvc_faildet                @descr=DRVC pin voltage  unexpected behavior detection. During transmitting DRVC pin shall be high and during echo reception DRVC pin shall be low;"
    "@field=tx_ndmos_faildet            @descr=Internal TX driver high voltage cascode fail. Internal short of output TX driver transistors detection;"
    "@field=tx_curr_faildet             @descr=Internal TX driver current source fail (GS short of LV NOMOS current source). Internal short of low voltage output TX driver current source detection;"
    "@field=tx_no_curr_faildet          @descr=No TX driver current detected. Open circuit from power supply to internal TX driver (open ASIC pins, open transformer pins, etc.) detection;"
    "@field=rxn_inp_faildet             @descr=Receiver input fail detected - pin RXN. Open/short detection on pin RXN;"
    "@field=rxp_inp_faildet             @descr=Receiver input fail detected - pin RXP. Open/short detection on pin RXP;"
    "@field=rx_cmmo_faildet             @descr=Receiver common modes detection. Internal hardware failure inside receiver detection;"
    "@field=dsp_lbist_faildet           @descr=DSP Logic BIST failed;"
    "@field=mode_config_crc             @descr=Mode config CRC;"
    "@field=onsemi_eeprom_crc           @descr=ON Semi part EEPROM CRC failed;"
    "@field=cust_eeprom_crc             @descr=Customer part EEPROM CRC failed;"

    // page1 0x1E
    ,"@register=REG @page=1 @adr=1E @type=Safety @purpose=safety;"
    "@field=eeprom_prog_fail             @descr=EEPROM programing fail or VSUP_RX voltage too low for EEPROM programming;"
    "@field=safety_status_1[15:1]        @descr=Safety status register 1;"

    // page1 0x20
    ,"@register=REG @page=1 @adr=20 @type=DSI3 @purpose=DSI3;"
    "@field=pdcm_size[1:0]              @descr=PDCM_SIZE 3 is 16 bytes;"
    "@field=tx_filter_ena               @descr=1 - Enable additional TX filter to improve emissions of Slave to Master communication;"
    "@field=dsi3pas_ref                 @descr=0 is reference to vbat/2, 1 is reference to GND;"
    "@field=pdcm_start[5:4]             @descr=PDCM start time;"
    "@field=crm_start[7:6]              @descr=CRM_START time;"
    "@field=t_chip                      @descr=T_CHIP: 0 is 3us;"
    "@field=crc_mode                    @descr=CRC in PDCM mode: 0 is 8-bit;"
    "@field=scrambler_ena               @descr=Enable (1) or disable (0) data scrambler;"
    "@field=dsi3pas_vpp                 @descr=DSI3 Vpp signaling level – 0 is 4Vpp, 1 is 2Vpp;"
    "@field=mount_id[14:12]             @descr=Sensor mounting position counted from the leftomost counterclockwise sensor;"
    "@field=vsds                        @descr=VSDS to determine for PDCM_SLOT number"
                            "<ul>"
                            "<li>0 - use Address equal to Sensor_ID (max 6 slots in a PDCM cycle)</li>"
                            "<li>1 - use Dynamic Slot Distribution (max 12 slots)</li>"
                            "</ul>;"

    // page1 0x22
    ,"@register=REG @page=1 @adr=22 @type=DSI3 @purpose=DSI3;"
    "@field=crc_dsi_config[7:0]         @descr=CRC-8 of DSI3PAS MAIN SETTINGS at addr. 0x120;"
    "@field=gain_clamp[15:8]       @value=255     @descr=Gain clamp, set 255;"

    // page1 0x26
    ,"@register=REG @page=1 @adr=26 @type=Common @purpose=Common;"
    "@field=ovt_threshold[7:0]          @descr=Over-temperature threshold, 0 is -55 dgC, 255 is 200 dgC;"
    "@field=die_temp[15:8]              @descr=Die temperature, 0 is -55 dgC, 255 is 200 dgC;"

    // page1 0x2E
    ,"@register=REG @page=1 @adr=2E @type=Trim @purpose=Trim;"
    "@field=freq_trim_factor[7:0]    @descr=trimming factor for the frequency of the sensor local clock  - signed 8-bit value (s8);"
    "@field=freq_trim_actual[15:8]   @descr=Actual trimming value of sensor local clock;"

    // end
    ,0
};


static const char *_SCALE_INIT[]={
    "@register=REG @sid=1;"
    "@field=corr_l_scale_r[4:0]          @descr=Actual Correlator scale factor, listen from right, channel L;"
    "@field=corr_l_scale[9:5]            @descr=Actual Correlator scale factor, listen myself, channel L;"
    "@field=corr_l_scale_l[14:10]        @descr=Actual Correlator scale factor, listen from left, channel L;"
    "@field=corr_h_scale_r[19:15]        @descr=Actual Correlator scale factor, listen from right, channel H;"
    "@field=corr_h_scale[24:20]          @descr=Actual Correlator scale factor, listen myself, channel H;"
    "@field=corr_h_scale_l[29:25]        @descr=Actual Correlator scale factor, listen from left, channel H;"
    "@field=EMPTY[31:30]        @descr=;"

    ,"@register=REG @sid=2;"
    "@field=corr_l_scale_r[4:0]          @descr=Actual Correlator scale factor, listen from right, channel L;"
    "@field=corr_l_scale[9:5]            @descr=Actual Correlator scale factor, listen myself, channel L;"
    "@field=corr_l_scale_l[14:10]        @descr=Actual Correlator scale factor, listen from left, channel L;"
    "@field=corr_h_scale_r[19:15]        @descr=Actual Correlator scale factor, listen from right, channel H;"
    "@field=corr_h_scale[24:20]          @descr=Actual Correlator scale factor, listen myself, channel H;"
    "@field=corr_h_scale_l[29:25]        @descr=Actual Correlator scale factor, listen from left, channel H;"
    "@field=EMPTY[31:30]        @descr=;"

    ,"@register=REG @sid=3;"
    "@field=corr_l_scale_r[4:0]          @descr=Actual Correlator scale factor, listen from right, channel L;"
    "@field=corr_l_scale[9:5]            @descr=Actual Correlator scale factor, listen myself, channel L;"
    "@field=corr_l_scale_l[14:10]        @descr=Actual Correlator scale factor, listen from left, channel L;"
    "@field=corr_h_scale_r[19:15]        @descr=Actual Correlator scale factor, listen from right, channel H;"
    "@field=corr_h_scale[24:20]          @descr=Actual Correlator scale factor, listen myself, channel H;"
    "@field=corr_h_scale_l[29:25]        @descr=Actual Correlator scale factor, listen from left, channel H;"
    "@field=EMPTY[31:30]        @descr=;"

    ,"@register=REG @sid=4;"
    "@field=corr_l_scale_r[4:0]          @descr=Actual Correlator scale factor, listen from right, channel L;"
    "@field=corr_l_scale[9:5]            @descr=Actual Correlator scale factor, listen myself, channel L;"
    "@field=corr_l_scale_l[14:10]        @descr=Actual Correlator scale factor, listen from left, channel L;"
    "@field=corr_h_scale_r[19:15]        @descr=Actual Correlator scale factor, listen from right, channel H;"
    "@field=corr_h_scale[24:20]          @descr=Actual Correlator scale factor, listen myself, channel H;"
    "@field=corr_h_scale_l[29:25]        @descr=Actual Correlator scale factor, listen from left, channel H;"
    "@field=EMPTY[31:30]        @descr=;"

    ,"@register=REG @sid=5;"
    "@field=corr_l_scale_r[4:0]          @descr=Actual Correlator scale factor, listen from right, channel L;"
    "@field=corr_l_scale[9:5]            @descr=Actual Correlator scale factor, listen myself, channel L;"
    "@field=corr_l_scale_l[14:10]        @descr=Actual Correlator scale factor, listen from left, channel L;"
    "@field=corr_h_scale_r[19:15]        @descr=Actual Correlator scale factor, listen from right, channel H;"
    "@field=corr_h_scale[24:20]          @descr=Actual Correlator scale factor, listen myself, channel H;"
    "@field=corr_h_scale_l[29:25]        @descr=Actual Correlator scale factor, listen from left, channel H;"
    "@field=EMPTY[31:30]        @descr=;"

    ,"@register=REG @sid=6;"
    "@field=corr_l_scale_r[4:0]          @descr=Actual Correlator scale factor, listen from right, channel L;"
    "@field=corr_l_scale[9:5]            @descr=Actual Correlator scale factor, listen myself, channel L;"
    "@field=corr_l_scale_l[14:10]        @descr=Actual Correlator scale factor, listen from left, channel L;"
    "@field=corr_h_scale_r[19:15]        @descr=Actual Correlator scale factor, listen from right, channel H;"
    "@field=corr_h_scale[24:20]          @descr=Actual Correlator scale factor, listen myself, channel H;"
    "@field=corr_h_scale_l[29:25]        @descr=Actual Correlator scale factor, listen from left, channel H;"
    "@field=EMPTY[31:30]        @descr=;"

    ,0
};

// COMMAND REGISTERS

static const char *SMODES_INIT={"@register=SMODES;"
                           "MODE0=1#MODE0;"
                           "MODE1#MODE1;"
                           "MODE2#MODE2;"
                           "MODE3#MODE3;"
                           "MODE4#MODE4;"
                           "MODE5#MODE5;"
                          };


static const char *SWriteREAD_INIT={"@register=SWriteREAD;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "rd_idx[15:8]#Memory index within page to be read;"
                                    "rd_pg[17:16]#Memory page to be read;"
                                    "@field=none[23:18]    @value=0    @descr=Unused;"
                                    "@field=READ[27:24]    @value=9    @readonly=1    @descr=Command code;"
                                    "sid[31:28]#Slave node identifier;"};

static const char *SReadREAD_INIT={"@register=SReadREAD;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "rsp_data[23:8]#16 bits data from memory at page rd_pg and index rd_idx;"
                                    "status[28:24]#Sensor status register;"
                                    "sensor_id[31:29]#Slave node identifier read from memory;"};

static const char *SWriteWRITERWP_INIT={"@register=SWriteWRITERWP;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "new_crm_w_idx[15:8]#Value to be written into index of CRM write pointer - word-aligned memory address index from the addressable memory range;"
                                    "new_crm_w_pg[17:16]#Value to be written into page of CRM write pointer;"
                                    "@field=none           @value=0    @descr=Unused;"
                                    "crm_upd#Update mode of CRM write pointer;"
                                    "new_pdcm_r_pg[21:20]#Value to be written into page of PDCM read pointer;"
                                    "pdcm_upd[23:22]#Value to be written into page of CRM write pointer;"
                                    "@field=WRITE_RWP[27:24]    @value=8    @readonly=1    @descr=Command code;"
                                    "sid[31:28]#Slave node identifier;"};

static const char *SReadWRITERWP_INIT={"@register=SReadWRITERWP;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "crm_w_idx[15:8]#value of the pointer after update;"
                                    "crm_w_pg[17:16]#value of the pointer after update;"
                                    "@field=none[19:18]           @value=0    @descr=Unused;"
                                    "pdcm_r_pg[21:20]#value of the pointer after update;"
                                    "@field=none2[23:22]          @value=0    @descr=Unused;"
                                    "status[28:24]#Sensor status register;"
                                    "sensor_id[31:29]#Slave node identifier read from memory;"};

static const char *SWriteWRITE8_INIT={"@register=SWriteWRITE8;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "data[15:8]#8 bits data to be written into memory;"
                                    "w_idx[23:16]#Index of memory - any byte-aligned memory address from the addressable memory range;"
                                    "@field=WRITE8[27:24]    @value=7    @readonly=1    @descr=Command code;"
                                    "sid[31:28]#Slave node identifier;"};

static const char *SReadWRITE8_INIT={"@register=SReadWRITE8;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "data[15:8]#Data from memory at CRM write pointer;"
                                    "crm_w_idx[23:16]#Index of CRM write pointer;"
                                    "status[28:24]#Sensor status register;"
                                    "sensor_id[31:29]#Slave node identifier read from memory;"};

static const char *SWriteWRITE_INIT={"@register=SWriteWRITE;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "data[23:8]#16 bits data to be written into memory;"
                                    "@field=WRITE[27:24]    @value=6    @readonly=1    @descr=Command code;"
                                    "sid[31:28]#Slave node identifier;"};

static const char *SReadWRITE_INIT={"@register=SReadWRITE;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "data[23:8]#Data from memory at CRM write pointer;"
                                    "status[28:24]#Sensor status register;"
                                    "sensor_id[31:29]#Slave node identifier read from memory;"};

static const char *SWriteWRITEP_INIT={"@register=SWriteWRITEP;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "data[23:8]#16 bits data to be written into memory;"
                                    "@field=WRITEP[27:24]    @value=5    @readonly=1    @descr=Command code;"
                                    "sid[31:28]#Slave node identifier;"};

static const char *SReadWRITEP_INIT={"@register=SReadWRITEP;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "data[23:8]#Data from memory at CRM write pointer;"
                                    "status[28:24]#Sensor status register;"
                                    "sensor_id[31:29]#Slave node identifier read from memory;"};

static const char *SWriteWRITEBLKP_INIT={"@register=SWriteWRITEBLKP;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "data5[23:8]#16 bits data to be written into memory;"
                                    "data4[39:24]#16 bits data to be written into memory;"
                                    "data3[55:40]#16 bits data to be written into memory;"
                                    "data2[71:56]#16 bits data to be written into memory;"
                                    "data1[87:72]#16 bits data to be written into memory;"
                                    "data0[103:88]#16 bits data to be written into memory;"
                                    "@field=WRITEBLKP[107:104]    @value=4    @readonly=1    @descr=Command code;"
                                    "sid[111:108]#Slave node identifier;"};

static const char *SWritePROGRAM_INIT={"@register=SWritePROGRAM;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "@field=none2[23:8]       @value=0    @descr=Reserved;"
                                    "@field=PROGRAM[27:24]    @value=10    @readonly=1    @descr=Command code;"
                                    "sid[31:28]#Slave node identifier;"};

static const char *SReadPROGRAM_INIT={"@register=SReadPROGRAM;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "none[23:8]#Reserved;"
                                    "status[28:24]#Sensor status register;"
                                    "sensor_id[31:29]#Slave node identifier read from memory;"};

/*static const char *SWriteMEASURE_INIT={"@register=SWriteMEASURE;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "doppler_tx_side[10:8]#Doppler compensation for TX - side sensor;"
                                    "free1[12:11]#Reserved;"
                                    "doppler_tx_inner[17:13]#Doppler compensation for TX - inner sensor;"
                                    "doppler_tx_outer[22:18]#Doppler compensation for TX - outer sensor;"
                                    "free2#Reserved;"
                                    "doppler_rx_side[26:24]#Doppler compensation for RX - side sensor;"
                                    "free3[28:27]#Reserved;"
                                    "doppler_rx_inner[31:29]#Doppler compensation for RX - inner sensor;"
                                    "free4[33:32]#Reserved;"
                                    "doppler_rx_outer[36:34]#Doppler compensation for RX - outer sensor;"
                                    "free5[39:37]#Reserved;"
                                    "beh_h6[41:40]=3#BEH SID6, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l6[43:42]=3#BEH SID6, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h5[45:44]=3#BEH SID5, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l5[47:46]=3#BEH SID5, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h4[49:48]=3#BEH SID4, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l4[51:50]=3#BEH SID4, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h3[53:52]=3#BEH SID3, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l3[55:54]=3#BEH SID3, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h2[57:56]=3#BEH SID2, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l2[59:58]=3#BEH SID2, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h1[61:60]=3#BEH SID1, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l1[63:62]=3#BEH SID1, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "@field=none[67:64]       @value=0    @descr=Unused;"
                                    "mode[70:68]#Measurement Mode, corresponds with memory;"
                                    "data_mode#0 is normal mode, 1 is diagnostic mode;"
                                    "@field=MEASURE[75:72]    @value=15    @readonly=1    @descr=Command code;"
                                    "sid[79:76]#Slave node identifier;"};*/

static const char *SWriteMEASURE_INIT={"@register=SWriteMEASURE;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "rev_sh_ena#rev_sh_ena;"
                                    "doppler_tx_side[10:9]#Doppler compensation for TX - side sensor;"
                                    "rev_sh[12:11]#rev_sh;"
                                    "free1#Reserved;"
                                    "doppler_tx_inner[17:14]#Doppler compensation for TX - inner sensor;"
                                    "free12#Reserved;"
                                    "doppler_tx_outer[22:19]#Doppler compensation for TX - outer sensor;"
                                    "free2[24:23]#Reserved;"
                                    "doppler_rx_side[26:25]#Doppler compensation for RX - side sensor;"
                                    "free3[29:27]#Reserved;"
                                    "doppler_rx_inner[31:30]#Doppler compensation for RX - inner sensor;"
                                    "free4[34:32]#Reserved;"
                                    "doppler_rx_outer[36:35]#Doppler compensation for RX - outer sensor;"
                                    "free5[39:37]#Reserved;"
                                    "beh_h6[41:40]=3#BEH SID6, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l6[43:42]=3#BEH SID6, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h5[45:44]=3#BEH SID5, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l5[47:46]=3#BEH SID5, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h4[49:48]=3#BEH SID4, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l4[51:50]=3#BEH SID4, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h3[53:52]=3#BEH SID3, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l3[55:54]=3#BEH SID3, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h2[57:56]=3#BEH SID2, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l2[59:58]=3#BEH SID2, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h1[61:60]=3#BEH SID1, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l1[63:62]=3#BEH SID1, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "@field=none[67:64]       @value=0    @descr=Unused;"
                                    "mode[70:68]#Measurement Mode, corresponds with memory;"
                                    "data_mode#0 is normal mode, 1 is diagnostic mode;"
                                    "@field=MEASURE[75:72]    @value=15    @readonly=1    @descr=Command code;"
                                    "sid[79:76]#Slave node identifier;"};

static const char *SWriteMEASUREadd_INIT={"@register=SWriteMEASUREadd;"
                                    "dyn_gain_restart_ena#dyn_gain_restart_ena;"
                                    "bsde1#beam_start_del_ena_1;"
                                    "bsde2#beam_start_del_ena_2;"
                                    "bsde3#beam_start_del_ena_3;"
                                    "bsde4#beam_start_del_ena_4;"
                                    "bsde5#beam_start_del_ena_5;"
                                    "bsde6#beam_start_del_ena_6;"
                                    "beam_start_delay[14:7]#1LSB is 512us;"
};

static const char *SWriteMEASUREold_INIT={"@register=SWriteMEASUREold;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "dyn_gain_restart_ena;"
                                    "beam_start_del_ena[14:9];"
                                    "free4#Reserved;"
                                    "beam_start_del[20:16]#1LSB is 512us;"
                                    "rev_sh[22:21]#rev_sh;"
                                    "rev_sh_ena#rev_sh_ena;"
                                    "doppler_tx_side[25:24]#Doppler compensation for TX - side sensor;"
                                    "doppler_rx_side[27:26]#Doppler compensation for RX - side sensor;"
                                    "doppler_tx_outer[31:28]#Doppler compensation for TX - outer sensor;"
                                    "doppler_rx_outer[33:32]#Doppler compensation for RX - outer sensor;"
                                    "doppler_tx_inner[37:34]#Doppler compensation for TX - inner sensor;"
                                    "doppler_rx_inner[39:38]#Doppler compensation for RX - inner sensor;"
                                    "beh_h6[41:40]=3#BEH SID6, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l6[43:42]=3#BEH SID6, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h5[45:44]=3#BEH SID5, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l5[47:46]=3#BEH SID5, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h4[49:48]=3#BEH SID4, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l4[51:50]=3#BEH SID4, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h3[53:52]=3#BEH SID3, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l3[55:54]=3#BEH SID3, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h2[57:56]=3#BEH SID2, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l2[59:58]=3#BEH SID2, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_h1[61:60]=3#BEH SID1, CH_H, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "beh_l1[63:62]=3#BEH SID1, CH_L, 3 - Low power, 2 - beam and listen own beam, 1 - listen from right, 0 - listen from left;"
                                    "rev_sh_ena2#rev_sh_ena2;"
                                    "rev_sh2[66:65]#rev_sh2;"
                                    "free5#Reserved;"
                                    "mode[70:68]#Measurement Mode, corresponds with memory;"
                                    "data_mode#0 is normal mode, 1 is diagnostic mode;"
                                    "@field=MEASURE[75:72]    @value=15    @readonly=1    @descr=Command code;"
                                    "sid[79:76]#Slave node identifier;"};


static const char *SWriteDISCOVERY_INIT={"@register=SWriteDISCOVERY;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "@field=none[23:8]       @value=0    @descr=Reserved;"
                                    "@field=DISCOVERY[27:24]    @value=3    @readonly=1    @descr=Command code;"
                                    "sid[31:28]#Slave node identifier;"};

static const char *SWriteSWRST_INIT={"@register=SWriteSWRST;"
                                    "@field=crc8[7:0]    @readonly=1      @descr=8 bits CRC security bits;"
                                    "@field=none[23:8]       @value=0    @descr=Reserved;"
                                    "@field=SWRST[27:24]    @value=2    @readonly=1    @descr=Command code;"
                                    "sid[31:28]#Slave node identifier;"};
#endif // DSI3SLAVER_REGMAP_H
