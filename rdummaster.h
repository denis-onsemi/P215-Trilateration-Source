#ifndef RDUMMASTER_H
#define RDUMMASTER_H

#include "applet.h"

#define DSI3MASTER_CH1     (1)
#define DSI3MASTER_CH2     (2)

namespace Ui {
    class RDUMMaster;
}

class RDUMMaster : public Applet
{
    Q_OBJECT

public:
    Register MREGX00;
    Register MREGX01;
    Register MREGX02;
    Register MREGX03;
    Register MREGX04;
    Register MREGX05;
    Register MREGX06;
    Register MREGX07;
    Register MREGX08;
    Register MREGX09;
    Register MREGX0A;
    Register MREGX0B;
    Register MREGX0C;
    Register MREGX0D;
    Register MREGX0E;
    Register MREGX10;
    Register MREGX11;
    Register MREGX12;
    Register MREGX13;
    Register MREGX14;
    Register MREGX15;
    Register MREGX16;
    Register MREGX17;
    Register MREGX18;
    Register MREGX20;
    Register MREGX21;
    Register MREGX22;
    Register MREGX23;

    Register MREGX3C;
    Register MREGX3D;
    Register MREGX3E;
    Register MREGX3F;
    Register MREGX40;
    Register MREGX41;
    Register MREGX42;
    Register MREGX43;
    Register MREGX44;
    Register MREGX45;
    Register MREGX46;
    Register MREGX47;
    Register MREGX48;
    Register MREGX49;
    Register MREGX4A;
    Register MREGX4B;
    Register MREGX4C;


    Register MREGX4F;
    Register MREGX50;
    Register MREGX51;

    Register EEX00;
    Register EEX01;
    Register EEX02;
    Register EEX03;
    Register EEX04;
    Register EEX05;
    Register EEX06;
    Register EEX07;
    Register EEX08;
    Register EEX09;
    Register EEX0A;

    Register REGADDR;
    Variable ADDR;
    Variable SLOT_NUMBER;
    Variable SLOT_COUNT;
    Variable SLOT_LENGTH;
    Variable SLOT_LENGTH_ALL;
    Variable ctSID;
    //Variable UPIDTO;
    Variable CLKIN;
    Variable RecCLKIN_FREQ;
    Variable RecCLKIN_FREQ_DET;
    Variable CLKIN_ERR;
    Variable CHECK_CRC;
    Variable CHECK_CRC_SHADOW;
    Variable DATA_LENGTH;
    Variable VarCRC;

    bool WriteAllConfig(void);

    void setupBoard();


    explicit RDUMMaster(QWidget *parent = 0);
    ~RDUMMaster();
    virtual bool initialize();
    virtual QVariant runAppCommand(const QString &name, const QVariant &p1, const QVariant &p2,const QVariant &p3,const QVariant &p4,const QVariant &p5);
    virtual void suspend();
    virtual void resume();
    virtual bool updateHardware(Direction drirection, const QString &);

    bool PerformCOMMAND(QByteArray txdata, QByteArray *rxdata, int read_count);
    bool PerformCRM(QByteArray txdata, QByteArray *rxdata, int read_count);

    void Ver_setItemText(int index, QString text);
    bool GenerateBRCsAndGetData(int slot_count, int BRC_count, int page);


private:
    void createCustomUi(void);
    Ui::RDUMMaster *ui;
    QByteArray m_current;
    float m_timesample;
    QVector<double> m_x;
    QVector<double> m_y;
    QVector<uint> m_y_uint;
    bool GetREGXPointer(QString name, Register **preg);
    void UpdateVoltages();
    void UpdateVoltagesv2();
    void UpdateSlotDurations();
    void CheckForRXErrors();
    quint16 CalcEEPROMCRC(QByteArray data);
    void SPIDecoder();
    QString DecodeUPIDWrite(int addr, uint data);
    quint16 CalculateECC(quint16 data);
    void ShowCurrent(void);


private slots:
    void slot_REGX50ALLUpdate();
    void slot_REGX51ALLUpdate();
};

extern RDUMMaster *gp_RDUMMaster;

#endif // RDUMMASTER_H
