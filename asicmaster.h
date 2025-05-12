#ifndef ASICMASTER_H
#define ASICMASTER_H

#include "applet.h"

#define DSI3MASTER_CH1     (1)
#define DSI3MASTER_CH2     (2)

namespace Ui {
    class ASICMaster;
}

class ASICMaster : public Applet
{
    Q_OBJECT

public:
    Register REGX00;
    Register REGX01;
    Register REGX02;
    Register REGX03;
    Register REGX04;
    Register REGX05;
    Register REGX06;
    Register REGX07;
    Register REGX0A;
    Register REGX0D;
    Register REGX0E;
    Register REGX0F;
    Register REGX10;
    Register REGX11;
    Register REGX12;
    Register REGX13;
    Register REGX14;
    Register REGX15;
    Register REGX16;
    Register REGX20;
    Register REGX21;
    Register REGX22;
    Register REGX23;
    Register REGX50;
    Register REGX51;
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


    explicit ASICMaster(QWidget *parent = 0);
    ~ASICMaster();
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
    Ui::ASICMaster *ui;
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

private slots:
};

extern ASICMaster *gp_ASICMaster;

#endif // ASICMASTER_H
