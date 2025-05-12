#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include "applet.h"
#include "qcustomplot.h"
#include "qtablewidget.h"

#define MAX_SLOT_NUMBER     10
#define MAX_BRC_NUMBER      128
#define HISTORY_DEPTH       1000
#define DIAGWORD_SIZE      12
#define SCHEDULER_SIZE      10
#define TW_SUMMARY          0
#define TW_DETAIL           1
#define TW_OVERVIEW         2

#define TOF                 0
#define PEAK                1
#define WIDTH               2
#define PEAK_SNR            3

#define SHIFT_CFAR_THR
#define SHIFT_DIFF

#define MEASURE_SINGLE      0
#define MEASURE_FIRST       1
#define MEASURE_LAST        2
#define MEASURE_CONTI       3

namespace Ui {
    class Measurement;
}
extern double received_data[3][3][2];    //propojeni s triangulaci

class Measurement : public Applet
{
    Q_OBJECT

public:

    Register DIAGWORD2[DIAGWORD_SIZE];
    Register DIAGWORD1[DIAGWORD_SIZE];
    Register DIAGWORD0[DIAGWORD_SIZE];
    Register SCHEDULER_MEASURE[SCHEDULER_SIZE];
    Register SCHEDULER_ENA;
    Register SUMMARYCHANNEL_ENA;
    Variable SUMMARY_STARTMS;
    Variable SUMMARY_STOPMS;
    Variable DETAIL_STARTMS;
    Variable DETAIL_STOPMS;
    Variable OVERVIEW_STARTMS;
    Variable OVERVIEW_STOPMS;
    Variable GRAPH_DETAIL_SEL[2];
    Variable GRAPH_OVERVIEW_SEL[8];
    Variable NCYCLES;
    Variable MEASPERIODMS;
    Variable HISTORY_INDEX;
    Variable UPDATE_MASTER;
    Variable PDCMDATACOUNT;
    Variable CFAR_SW_ENA;
    Variable CFAR_THR_OFF;
    Variable CFAR_SCALE;
    Variable CFAR_DER_THR;
    Variable CFAR_DER_SIZE;
    Variable CFAR_SNR;
    Variable TOF_OFFSET;
    Register APPLY_CFAR;  
    Variable CFAR_STARTMS;      //CFAR start time
    Variable MP_DET_THR_POS;    //Multipeak detection thresholds +/-
    Variable MP_DET_THR_NEG;

    Variable VarPHASE_DIFF_THR;
    Variable VarPHASE_DIFF_12_ENA;
    Variable VarPHASE_DIFF_34_ENA;
    Variable VarPHASE_DIFF_12_INVERT;
    Variable VarPHASE_DIFF_34_INVERT;


    void setupBoard();
    explicit Measurement(QWidget *parent = 0);
    ~Measurement();
    virtual bool initialize();
    virtual QVariant runAppCommand(const QString &name, const QVariant &p1, const QVariant &p2,const QVariant &p3,const QVariant &p4,const QVariant &p5);
    virtual void suspend();
    virtual void resume();
    virtual bool updateHardware(Direction drirection, const QString &);
    void SetBRCReadData(QString data);
    void CalculateCFAR(QVector<double> samples, quint8 der_size, double scale, quint8 offset, int der_thr, \
                           int CFAR_SNR_THRESHOLD, QVector<double> &CFAR_MAG, QVector<double> &ECHO_DET, quint8 MVA_samples, \
                           quint8 SNR_debounce, QVector<double> &DBG1, QVector<double> &DBG2, \
                           QVector<double> &DBG3, QVector<double> &DBG4, QVector<double> &DBG5, QVector<double> &DBG6, QVector<double> &DBG7, \
                           QVector<double> &DBG8, QVector<double> &DBG9, QVector<double> &DBG10, quint32 CFAR_START_SAMPLE);

    quint16 get_diag_position(quint8 meas_type, quint8 selct_compr_res, quint8 meas_duration, quint8 direct, quint8 tank_charge_time, quint8 compr_timer);

    void ShowIQtab(void);
    void Show4DULStab(void);

    void HideBSD(bool hide);
    QVector<double> m_meas_samples[MAX_SLOT_NUMBER];
    QVector<double> m_meas_iq[12];
    QVector<double> m_meas_time;
    QVector<double> m_meas_freq;
    QVector<double> m_meas_tof[MAX_SLOT_NUMBER][4];
    QVector<double> m_short_range[MAX_SLOT_NUMBER];
    QStringList m_meas_slots;
    QStringList m_meas_slots_iq;
    uint m_meas_slot_number;
    int m_graph_to_slot[12];
    bool m_meas_dir[MAX_SLOT_NUMBER];
    bool m_meas_tof_ena;
    int m_meas_counter;
    int m_counter;
    quint32 m_CFAR_START_SAMPLE;
    double m_CFAR_START_MS;
    float m_sampling_period;                //sampling period [ms]
    QVector<QVector<double>> m_TOF_DIFF;    //Diff signal used to calculate TOF from even Multipeak echoes
    QVector<QVector<double>> m_TOF_ECHO_DET;  //Bool signal used to calculate TOF (different CFAR SNR Threshold for TOF detection)
    QVector<QVector<double>> m_TOF_SNR;     //SNR signal used to report peak SNR value during TOF reporting
    QVector<QVector<double>> m_TOF_SAMPLES; //Backup of original input data

    QByteArray m_diagword0[MAX_SLOT_NUMBER];
    QByteArray m_diagword1[MAX_SLOT_NUMBER];
    QByteArray m_diagword2[MAX_SLOT_NUMBER];
    char m_sensorstatus[MAX_BRC_NUMBER][MAX_SLOT_NUMBER];
    int m_pdcm_crc[MAX_BRC_NUMBER][MAX_SLOT_NUMBER];
    int m_kac[MAX_BRC_NUMBER][MAX_SLOT_NUMBER];

    bool m_hide_slot[MAX_SLOT_NUMBER];

    bool m_first_measurement[2];

private:
    QList<QCustomPlot *>GRAPH_DETAIL;
    QList<QTableWidget *>TABLE_DETAIL;
    QList<QCustomPlot *>GRAPH_OVERVIEW;
    QList<QLabel *>LABEL_REVFREQ;
    QList<QLabel *>LABEL_REVFREQ_2;
    QList<QLabel *>LABEL_REVDUR;
    QList<QLabel *>LABEL_REVDUR_2;
    uint m_packets;
    uint m_PERerrors;

    uint m_samplecount1_first102[12];
    uint m_samplecount2_first51[12];
    uint m_samplecount3_second102[12];

    void createCustomUi(void);
    Ui::Measurement *ui;

    bool Measure(int type);
    bool DecodeData(void);
    void ShowData(void);

    void CFARinGUI(void);
    void ToFinGUI(void);
    void CalculateTOF(void);
    void SetSlaveReg(int memory, int page, int index, QString name, int value);
    quint32 GetSlaveReg(int memory, int page, int index, QString name);
    double QVectorMin(QVector<double> vec);
    double QVectorMax(QVector<double> vec);
    double QVectorAvg(QVector<double> vec);
    int TwosComplementToInt(quint8 size, uint input);
    QByteArray Descramble(QByteArray data, int slot);
    void CheckVersionConfig(void);
    QVector<double> Decompress_data(int compression, bool channel_low, QByteArray compressed_data, bool I);
    QVector<double> Decompress_Mode1(QByteArray slot_data);
    QVector<double> Decompress_Mode5(QByteArray slot_data, bool channel_low);
    QVector<double> Decompress_Mode7(QByteArray slot_data, bool channel_low);
    QVector<double> Decompress_Mode9(QByteArray slot_data, bool I);
    QVector<double> Decompress_Mode12(QByteArray slot_data, bool I);
    QVector<double> Decompress_Mode14(QByteArray slot_data);
    QVector<double> GetAndRemoveShortRange(QByteArray *data);

    bool StartContinuousMeasurement(void);
    bool PerformNCycles(void);
    bool StopMeasurement(void);

    void InitGraphsAndTables(void);
    void UpdateSummaryGraphDataAndFormat(void);
    void UpdateSummaryGraphFormat(void);
    void UpdateDetailGraph(void);
    void UpdateOverviewGraph(void);
    void UpdateSummaryTable(void);
    void UpdateIQTable(void);
    void UpdateDetailTable(void);

    void SwitchPersistence(bool on);

    bool SaveGraph(QString arg);
    bool CopyGraph(QString arg);
    bool SaveCSV(QString arg);

    void MEASUREdecode(void);
    void CheckDIAGWORD0(QByteArray data, int slot);
    void CheckKAC(char data, int slot, int brc);
    bool CheckMOBEBRCCRC(int slot, int brc, QByteArray data, QByteArray crc);
    void AddCRCModeConfig();

    void UpdateMasterSettings();

    bool ShowResultsOfIndex(void);
    void ClearHistory(void);
    bool AddToHistory(void);
    void SaveHistory(void);
    void ShowPER(void);
    void CalculatePHASE_DIFF(void);

    void QVectorAppend(QVector<double> *target, QVector<double> source);

    QVector<double> m_meas_sampleshistory[HISTORY_DEPTH][MAX_SLOT_NUMBER];
    QVector<double> m_meas_timehistory[HISTORY_DEPTH];
    QVector<double> m_meas_freqhistory[HISTORY_DEPTH];
    QVector<double> m_meas_tofhistory[HISTORY_DEPTH][MAX_SLOT_NUMBER][4];
    QVector<double> m_short_rangehistory[HISTORY_DEPTH][MAX_SLOT_NUMBER];
    QStringList m_meas_slotshistory[HISTORY_DEPTH];
    uint m_meas_slot_numberhistory[HISTORY_DEPTH];
    int m_graph_to_slothistory[HISTORY_DEPTH][12];
    bool m_meas_tof_enahistory[HISTORY_DEPTH];
    QByteArray m_diagword0history[HISTORY_DEPTH][MAX_SLOT_NUMBER];
    QByteArray m_diagword1history[HISTORY_DEPTH][MAX_SLOT_NUMBER];
    QByteArray m_diagword2history[HISTORY_DEPTH][MAX_SLOT_NUMBER];
    QString m_sensorstatushistory[HISTORY_DEPTH];

    QString m_meas_errorshistory[HISTORY_DEPTH];

    uint m_history_pointer_start;
    uint m_history_pointer_end;

    struct t_MAG_DATA_INFO
    {
        // INPUT information
        quint8 meas_type;           // Measurement Type (0...31)
        quint8 selct_comp_res;      // Magnitude Compression Method (0...4)
        quint8 meas_duration;       // Measurement Duration code (0...63)
        quint8 direct;              // 0...indirect,  1...direct
        quint8 tank_charge_time;    // Tank Capacitor Charge Time (0...7, step (16 * 51.2)us )
        quint8 compr_timer;         // Compression Timer (0...7)

        // OUTPUT information
        quint16 sample_count;       // total number of magnitude samples
        quint16 sample_count_fast;       // actual number of fast samples (51.2us)
        quint16 sample_count_pre_rx;     // actual number of samples before RX phase

        quint8 block_size;          // data block size in bytes (e.g. 3, 4, 2 ... )
        quint8 block_sample_count;  // number of magnitude samples in one block
        quint16 block_count;        // total number of magnitude data blocks

        quint8 block2_size;          // data block size in bytes (e.g. 3, 4, 2 ... )
        quint8 block2_sample_count;  // number of magnitude samples in one block
        quint16 block2_count;        // total number of magnitude data blocks

        quint16 meas_dur_tof;       // meas duration in number of steps of 51.2 us
        quint16 beaming_tof;        // beaming duration in number of steps of 51.2 us
        quint16 mag_data_size;      // magnitude data size in bytes, diag words included diag word #0 at address #0
                                    // diag word #1 at address (mag_data_size-2) little endian (i.e. LSByte first)
        quint16 wbn_count;          // number of 16-bit noise flags
        quint16 wbn_data_size;      // magnitude/IQ data block size for single 16-bit noise flags Diag #0, mag data, Noise flags, mag data, Noise flags, ...
        quint16 wbn_sample_count_2; // Number of samples for 2 WBN flags

    };

    void get_mag_data_info(t_MAG_DATA_INFO *mag_data_info);
    void get_mag_data_info3(t_MAG_DATA_INFO *mag_data_info);
    void get_mag_data_info4(t_MAG_DATA_INFO *mag_data_info);

private slots:
    void slot_UpdateALL(void);
    void slot_MEASTimer_timeout(void);
    void slot_EnableHistoryUpdate(void);
    void slot_PersistenceUpdate(void);
    void slot_TWUpdate(void);
    void slot_updateTable(void);

};

extern Measurement *gp_Measurement;

#endif // MEASUREMENT_H
