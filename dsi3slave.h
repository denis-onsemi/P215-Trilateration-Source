#ifndef DSI3SLAVE_H
#define DSI3SLAVE_H

#include "applet.h"

#define MEMORY_VER      (2)

namespace Ui {
    class DSI3Slave;
}

class DSI3Slave : public Applet
{
    Q_OBJECT

public:

    void setupBoard();

    QList<Register *>SREGX[MEMORY_VER];
    QList<Register *>SSCALE;
    QList<Register *>SSCALEv2;
    QList<Variable *>SDBG;
    QList<Variable *>SDBGv2;
    QList<Variable *>SCOMP;
    QList<Variable *>SCOMPv2;

    quint32 m_map_sregs[MEMORY_VER][2][256];
    quint32 m_map_sscale[6][6];

    bool m_slave_connected[2][6];

    bool m_block_update_version;

    int m_PrevVERSION;

    int m_memory;

    Register SWriteREAD;
    Register SReadREAD;
    Register SWriteWRITERWP;
    Register SReadWRITERWP;
    Register SWriteWRITE8;
    Register SReadWRITE8;
    Register SWriteWRITE;
    Register SReadWRITE;
    Register SWriteWRITEP;
    Register SReadWRITEP;
    Register SWriteWRITEBLKP;
    Register SWritePROGRAM;
    Register SReadPROGRAM;
    Register SWriteMEASURE;
    Register SWriteMEASUREold;
    Register SWriteMEASUREadd;
    Register SWriteDISCOVERY;
    Register SWriteSWRST;
    Register SMODES;

    Variable Vardbg_out_sel;
    Variable Vardbg_out_sel_MODE1;
    Variable Vardbg_out_sel_MODE2;
    Variable Vardbg_out_sel_MODE3;
    Variable Vardbg_out_sel_MODE4;
    Variable Vardbg_out_sel_MODE5;
    Variable Varcomp_method;
    Variable Varcomp_method_MODE1;
    Variable Varcomp_method_MODE2;
    Variable Varcomp_method_MODE3;
    Variable Varcomp_method_MODE4;
    Variable Varcomp_method_MODE5;
    Variable VarSID;
    Variable ShadowComp_method;
    Variable piezo_model_shift;

    Variable VarCH1ID1;
    Variable VarCH1ID2;
    Variable VarCH1ID3;
    Variable VarCH1ID4;
    Variable VarCH1ID5;
    Variable VarCH1ID6;
    Variable VarCH2ID1;
    Variable VarCH2ID2;
    Variable VarCH2ID3;
    Variable VarCH2ID4;
    Variable VarCH2ID5;
    Variable VarCH2ID6;

    Variable VarDontUpdateCRC;
    Variable Bitstream2022;
    Variable VarPrevVERSION;
    Variable VarSwitches;

    explicit DSI3Slave(QWidget *parent = 0);
    ~DSI3Slave();
    virtual bool initialize();
    virtual QVariant runAppCommand(const QString &name, const QVariant &p1, const QVariant &p2,const QVariant &p3,const QVariant &p4,const QVariant &p5);
    virtual void suspend();
    virtual void resume();
    virtual bool updateHardware(Direction drirection, const QString &);
    char CalcSlaveCRCfromREG(Register preg);
    bool PerformCRM(Register tx_reg, Register *rx_reg, int read_count);
    bool PerformCOMMAND(Register tx_reg, QByteArray *rxdata, int read_count);
    bool PerformMEASURE(bool brc);
    bool PrepareMEASUREUPID();
    bool PerformDISCOVERY(void);
    bool AddressDISCOVERY(void);
    bool AddressDEMUXES(void);
    bool AddressSWITCHES(void);
    bool PerformWRITE_RWP(bool inreg, int sid, int pdcm_upd, int new_pdcm_r_pg,\
                          int crm_upd, int new_crm_w_pg, int new_crm_w_idx);
    bool WriteSlaveRegs(QString regtype);
    bool WriteModeRegs(void);
    bool ReadSlaveRegs(QString regtype);
    bool ReadModeRegs(void);
    bool CheckSlaves(void);
    void ClearSlaves(void);
    void ClearBitstream2022(void);
    bool WriteDSI3RegsAll(void);
    bool Synchronize(void);
    bool ClearSafetyFlags(bool broadcast);
    void CheckVersionConfig(void);

    bool FirstMEASURE();
    bool LastMEASURE();
    bool ContiMEASURE();
    int Getcomp_method_old(int mode);
    int Getmeas_type_old(int mode);
    void SetPDCMdata(int sid, int page, QString txt);
    bool IsAddressSwitches();

public slots:

    void slot_VersionUpdate(void);

private:
    QList<QCheckBox *>SLAVES_CONNECTED;
    QList<Variable *>CHID_CONNECTED;
    QList<QComboBox *>MEAS_TYPE_LIST;
    QList<QComboBox *>COMP_METHOD_LIST;
    QList<QComboBox *>MEAS_TYPE_LISTv2;
    QList<QComboBox *>MEAS_TYPE_LISTv2BSD;
    QList<QComboBox *>COMP_METHOD_LISTv2;

    void createCustomUi(void);
    Ui::DSI3Slave *ui;
    void CopyAllRegs(int from, int to);
    bool PerformREAD(bool inreg, int sid, int rd_pg, int rd_idx, int *rsp_data);
    bool PerformWRITE_8(bool inreg, int sid, int w_idx, int data);
    bool PerformWRITE(bool inreg, int sid, int data);
    bool PerformWRITE_P(bool inreg, int sid, int data);
    bool PerformWRITE_BLK_P(bool inreg, int sid, int data0, int data1, int data2, \
                            int data3, int data4, int data5);
    bool PerformPROGRAM(bool inreg, int sid);
    bool PerformRWPandWRITE(int sid, int new_crm_w_pg, int new_crm_w_idx, int data);
    bool PerformRWPandWRITEP(int sid, int new_crm_w_pg, int new_crm_w_idx, int data);
    bool PerformSWRST(bool inreg, int sid);
    bool PerformUnlockAndSWRST();
    bool Addressing(void);

    void CopyModes(void);
    void CopyMem(void);

    void CopyAccessNames(int ver);
    void InitRegisterMap(int ver);
    void InitScaleMap();

    void AddCRCModeConfig(void);
    void AddCRCDSI3Config(void);
    void AddCRCNvmCust(void);
    void ChangeConfiguration(void);

    bool ReadPageUsingPDCM(int page);

private slots:
    void slot_UpdateLabels(void);
    void slot_GainGraphUpdate(void);
};

extern DSI3Slave *gp_DSI3Slave;

#endif // DSI3SLAVE_H
