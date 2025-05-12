#ifndef CFARSIM_H
#define CFARSIM_H

#include "applet.h"
#include "qcustomplot.h"
#include "qtablewidget.h"

#define MAX_CHANNEL_NUMBER     12
#define CFAR_OUT_MEM_MULTIPLIER 1

#define MEM_SIZE_MULT           5  //sample memory size multiplier
#define DBG1_MEM_MULT           2   //uncomment to enable debug output
#define DBG2_MEM_MULT           3   //en debug output
#define DBG3_MEM_MULT           4   //en debug output
//#define DBG4_MEM_MULT           5   //en debug output
//#define DBG5_MEM_MULT           6   //en debug output
//#define DBG6_MEM_MULT           7   //en debug output
//#define DBG7_MEM_MULT           8   //en debug output
//#define DBG8_MEM_MULT           9   //en debug output
//#define DBG9_MEM_MULT           10  //en debug output
//#define DBG10_MEM_MULT          11  //en debug output

#define PLOT_CNT                8
#define DEBUG_CNT               20

/*  DEBUG OUTPUT SETTING
*   1) Edit MEM_SIZE_MULT to correspond with selected number of debug outputs (minimum 2 - sample memory, CFAR output memory --- maximum 9 (sample, cfar output, debug 1... debug 7)
*   2) Comment/uncomment corresponding DBGx_MEM_MULT definitions to disable/enable debug output
*   Example 1: to enable only customer debug output,
*   set:        #define MEM_SIZE_MULT           5       //(samples, cfar output, debug1...)
*   uncomment:  #define DBG1_MEM_MULT           2 to #define DBG3_MEM_MULT           4   //en debug output
*   comment:    //#define DBG4_MEM_MULT         5 to //#define DBG10_MEM_MULT           11
*
*   Example 2: to enable 10 debug outputs,
*   set:        #define MEM_SIZE_MULT           12      //(samples, cfar output, debug1... debug10)
*   uncomment:  #define DBG1_MEM_MULT           2 to #define DBG10_MEM_MULT          11
*/

#define TOF             0
#define PEAK            1
#define WIDTH           2
#define PEAK_SNR        3
#define REPORT_WIDTH                //comment this line to disable ECHO WIDTH reporting
#define REPORT_PEAK_SNR

#define CMPR_DBG_CNT        3       //compression/decompr. number of debug outputs
#define CMPR_MEM_SIZE_MULT  4       //original data, cfar, undersampling, compression/decompr.

#define SHIFT_CFAR_THR              //comment this line to disable time shift of cfar_threshold signal
#define SHIFT_DIFF                  //comment this line to disable time shift of DIFF signal

#define CFAR_TYPE_MAG       0       //Type of CFAR flag, magnitude CFAR or TOF cfar
#define CFAR_TYPE_TOF       1
#define CFAR_TYPE_ALLDBG    2
#define CFAR_TYPE_NODBG     99

static const int plot_colors[10][3] = {{255,0,0},{0,0,180},{0,120,0},{0,0,0},{250,120,0},{202,31,123},{120,120,120},{130,70,0},{51,204,255},{0,230,0}};

namespace Ui {
    class CFARsim;
}

class CFARsim : public Applet
{
    Q_OBJECT

public:
    Variable CSVLegendLine;
    Variable CFAR_THR_OFF;
    Variable CFAR_SCALE;
    Variable CFAR_DER_THR;
    Variable CFAR_DER_SIZE;
    Variable CFAR_SNR;          //For Magnitude calculation
    Variable CFAR_SNR_TOF;      //For TOF detection
    Variable SUMMARY_STARTMS;
    Variable SUMMARY_STOPMS;
    Variable CFAR_STARTMS;
    Register CMPR_DBG_ENA;
    Variable TOF_REPORTING;
    Variable DER_THR_TOF;
    Variable MP_DET_THR_POS;    //Multipeak detection thresholds +/-
    Variable MP_DET_THR_NEG;
    Variable DER_SIZE_TOF;
    Variable THR_OFF_TOF;
    Variable CFAR_SCALE_TOF;

    Variable DYN_SC_ENA;                                                                                    //Dynamic Scaling - Enable
    Variable DYN_SC_VAL_0,  DYN_SC_VAL_1,   DYN_SC_VAL_2,   DYN_SC_VAL_3,   DYN_SC_VAL_4,   DYN_SC_VAL_5;   //Dynamic Scaling - Value
    Variable DYN_SC_TIME_0, DYN_SC_TIME_1,  DYN_SC_TIME_2,  DYN_SC_TIME_3,  DYN_SC_TIME_4,  DYN_SC_TIME_5;  //Dynamic Scaling - delta Time
    Variable DYN_SC_VAL_0_B,  DYN_SC_VAL_1_B,   DYN_SC_VAL_2_B,   DYN_SC_VAL_3_B,   DYN_SC_VAL_4_B,   DYN_SC_VAL_5_B;   //Dynamic Scaling (channel B) - Value
    Variable DYN_SC_TIME_0_B, DYN_SC_TIME_1_B,  DYN_SC_TIME_2_B,  DYN_SC_TIME_3_B,  DYN_SC_TIME_4_B,  DYN_SC_TIME_5_B;  //Dynamic Scaling (channel B) - delta Time

    QVector<QVector<double>> sim_samples;
    /*Sample memory structure
    *   sim_samples[0 to (sim_channel_count-1)]                     - samples from channels 1,2,3...
    *   sim_samples[sim_channel_count to (sim_channel_count*2-1)]   - CFAR result (echo det signal)
    *   sim_samples[sim_channel_count*2 to (sim_channel_count*3-1)] - CFAR debug signal 1
    *   ...
    */
    QVector<double> sim_time;                           //time steps [ms]
    uint sim_channel_count;                             //number of channels 0-12
    QVector<QString> sim_channel_names;                 //name of channels (1L,2H...6H)
    QVector<double> sim_tof[MAX_CHANNEL_NUMBER][4];     //ToF detection //ToF, Peak, Width, Peak SNR
    float sim_sampling_period;                          //sampling period [ms]
    quint32 sim_CFAR_START_SAMPLE;                      //calculated start of CFAR algorithm from CFAR_STARTMS and sampling period
    QVector<QVector<double>> sim_TOF_DIFF;              //Diff signal used to calculate TOF from even Multipeak echoes
    QVector<QVector<double>> sim_TOF_ECHO_DET;          //Bool signal used to calculate TOF (different CFAR SNR Threshold for TOF detection)
    QVector<QVector<double>> sim_TOF_SNR;               //SNR signal used to report peak SNR value during TOF reporting

    struct t_REG {                                      //Used for undersampling
        int adv_mag;
        quint8 adv_mag_falling_dly;
        quint8 adv_mag_kept;
    };
    QVector<QVector<double>> sim_cmpr_samples;          //Compressed/decompressed data memory structure [debug channel]
    QVector<QString> sim_cmpr_dbg_names;                //name of debug channels (1L,2H...6H)
    QVector<QVector<double>> sim_cmpr_time;             //time steps [ms] for compression/decompr. debugging [debug channel]
    quint8 sim_cmpr_en_ch_cnt;

    explicit CFARsim(QWidget *parent = 0);
    ~CFARsim();
    virtual bool initialize();
    virtual void suspend();
    virtual void resume();
    virtual void setupBoard();
    virtual bool updateHardware(Direction drirection, const QString &);
    virtual QVariant runAppCommand(const QString &name, const QVariant &p1, const QVariant &p2,const QVariant &p3,const QVariant &p4,const QVariant &p5);
    void CalculateCFAR(QVector<double> samples, quint8 der_size, quint8 TOF_der_size, double scale, quint8 offset, int der_thr, int TOF_der_thr, \
                       int CFAR_SNR_THRESHOLD, QVector<double> &CFAR_MAG, QVector<double> &ECHO_DET, QVector<bool> &ECHO_DET_TOF, quint8 MVA_samples, quint8 SNR_debounce, \
                       QVector<double> &DBG1, QVector<double> &DBG2, QVector<double> &DBG3, QVector<double> &DBG4, QVector<double> &DBG5, \
                       QVector<double> &DBG6, QVector<double> &DBG7, QVector<double> &DBG8, QVector<double> &DBG9, QVector<double> &DBG10, quint32 CFAR_START_SAMPLE);
    void CalculateCFARv2(QVector<double> samples, quint8 der_size, quint8 TOF_der_size, double scale, quint8 offset, int der_thr, int TOF_der_thr, \
                       int CFAR_SNR_THRESHOLD, QVector<double> &CFAR_MAG, QVector<double> &ECHO_DET, QVector<double> &ECHO_DET_TOF, quint8 MVA_samples, quint8 SNR_debounce, \
                       QVector<double> &DBG1, QVector<double> &DBG2, QVector<double> &DBG3, QVector<double> &DBG4, QVector<double> &DBG5, \
                       QVector<double> &DBG6, QVector<double> &DBG7, QVector<double> &DBG8, QVector<double> &DBG9, QVector<double> &DBG10, quint32 CFAR_START_SAMPLE);
    void CalculateCFAR_new(QVector<double> samples, quint8 der_size, double scale, quint8 offset, int der_thr, \
                           int CFAR_SNR_THRESHOLD, QVector<double> &CFAR_MAG, QVector<double> &ECHO_DET, quint8 MVA_samples, \
                           quint8 SNR_debounce, QVector<double> &DBG1, QVector<double> &DBG2, \
                           QVector<double> &DBG3, QVector<double> &DBG4, QVector<double> &DBG5, QVector<double> &DBG6, QVector<double> &DBG7, \
                           QVector<double> &DBG8, QVector<double> &DBG9, QVector<double> &DBG10, quint32 CFAR_START_SAMPLE, quint8 CFAR_TYPE);

    bool DynScaleData(int channel, QVector<double> time, QVector<double> &data);
    bool DynScaleCaclulate(QVector<QVector<double> > &scale_tb);
    double LinInterpolate(double x, double x0, double y0, double x1, double y1);

private:
    void createCustomUi(void);
    Ui::CFARsim *ui;
    bool LoadCSV(QVector<QVector<double>> &output, float &sampling_period, quint8 &channel_count, QVector<QString> &channel_name, uint &sample_count);
    bool SetupPlotComboBox(bool remember_state);
    bool SetupComprComboBox(bool remember_state);
    bool LoadFromMeasurement(QVector<QVector<double>> &output, float &sampling_period, quint8 &channel_count, QVector<QString> &channel_name, uint &sample_count);
    void CFAR(void);
    void CFARinGUI(void);
    double QVectorMin(QVector<double> vec);
    double QVectorMax(QVector<double> vec);
    double QVectorAvg(QVector<double> vec);
    void PrepareData(QVector<QVector<double>> samples, float sampling_period, uint channel_count, uint sample_count, QVector<QString> channel_name);
    void PreparePlot(void);
    void CalculateTOF(void);
    void UpdateTable(void);
    bool CopyGraph(void);
    bool SaveGraph(QString arg);
    bool SaveCSV(QString arg);
    QVector<double> Undersampling(QVector<double> samples_in, float &sim_undr_sampling_per);
    void adv_smp(t_REG &reg_s, int mag);
    bool CompressDebug(void);
    QVector<double> ComprDecompr(QVector<double> samples_in);

private slots:
    void slot_PlotData(void);
    void slot_updateTable(void);
    void slot_updateCB(void);
    void slot_updateCFARcontrols(void);
    void slot_updateDynScaling(void);
};

extern CFARsim *gp_CFARsim;

#endif // CFARsim

static const char *ENA_1_4_INIT={"@register=REG;"
                                        "EN1=1#Enable CFAR calculation;"
                                        "EN2=1#Enable Undersampling;"
                                        "EN3=1#Enable Compression/decompression;"
                                        "EN4=1#Enable debug output;"
};

static const char *ENA_1_4x3_INIT={"@register=REG;"
                                        "EN_CFAR1=1#Enable CFAR calculation on debug out. 1;"
                                        "EN_CFAR2=1#Enable CFAR calculation on debug out. 2;"
                                        "EN_CFAR3=1#Enable CFAR calculation on debug out. 3;"
                                        "EN_UNDR1=1#Enable Undersampling on debug out. 1;"
                                        "EN_UNDR2=1#Enable Undersampling on debug out. 2;"
                                        "EN_UNDR3=1#Enable Undersampling on debug out. 3;"
                                        "EN_CMPR1=1#Enable Compression/decompression on debug out. 1;"
                                        "EN_CMPR2=1#Enable Compression/decompression on debug out. 2;"
                                        "EN_CMPR3=1#Enable Compression/decompression on debug out. 3;"
};
