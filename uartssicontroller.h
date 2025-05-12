#ifndef UARTSSICONTROLLER_H
#define UARTSSICONTROLLER_H

#include "applet.h"

namespace Ui {
    class UartSsiController;
}

class UartSsiController : public Applet
{
    Q_OBJECT

public:

    Register STATUS;
    Register DEMUX;

    Register SSIREGX000;
    Register SSIREGX004;
    Register SSIREGX008;
    Register SSIREGX00C;
    Register SSIREGX010;
    Register SSIREGX014;
    Register SSIREGX018;
    Register SSIREGX01C;
    Register SSIREGX020;
    Register SSIREGX024;
    Register SSIREGX028;
    Register SSIREGX02C;
    Register SSIREGX030;
    Register SSIREGX034;
    Register SSIREGX038;
    Register SSIREGX03C;
    Register SSIREGX040;
    Register SSIREGX1EC;
    Register SSIREGX1F0;
    Register SSIREGX1F4;
    Register SSIREGX1F8;
    Register SSIREGX1FC;


    Register SSIREGADDR;
    Variable SSIADDR;
    Register SSIREGADDRMEM;
    Register SSIADDRMEM;
    /*Variable BRC_CHANNEL;
    Variable BRC_BRC;
    Variable BRC_SLOTS;
    Variable BRC_LENGTH;
    Variable BRC_PERIOD;
    Variable BRC_RESP_LENGTH;
    Variable BRC2_RESP_LENGTH;
    Variable BRC3_RESP_LENGTH;
    Variable BRC_START;
    Variable REPEAT_TIME;*/

    Variable SSI_CHANNEL;
    Variable SSI_MEAS;
    Variable SSI_READ;
    Variable SSI_BRC_COUNT;
    Variable SSI_SLOT_COUNT;
    Variable SSI_MEAS_BRC_DELAY;
    Variable SSI_BRC_BRC_DELAY;
    Variable SSI_BRC_GET_DELAY;
    Variable SSI_GET_COUNT;
    Variable SSI_GET_BYTES;
    Variable SSI_GET_GET_DELAY;

    Variable SSI_XOR;


    void setupBoard();

    explicit UartSsiController(QWidget *parent = 0);
    ~UartSsiController();
    virtual bool initialize();
    virtual QVariant runAppCommand(const QString &name, const QVariant &p1, const QVariant &p2,const QVariant &p3,const QVariant &p4,const QVariant &p5);
    virtual void suspend();
    virtual void resume();
    virtual bool updateHardware(Direction drirection, const QString &);

    char CalculateCRC(QByteArray data);
    char CalculateUPAECRC(QByteArray data);
    char CalculateUPAECRCwithSEED(char seed, QByteArray data);
    quint16 CalculateUPAECRC16withSEED(quint16 seed, QByteArray data);
    quint16 CalculateCRC16UPAEwithSEEDfromFPGA(quint16 seed, QByteArray data);
    void getTXcommandAndRXlength(QByteArray *txcmd, int *RXlength);
    void Scramble(QByteArray input, QByteArray *output, quint16 SCRAM);

    bool WriteNVMreg(quint32 address, quint32 data);
    bool ReadNVMreg(quint32 address, quint32 *data);

private:
    void createCustomUi(void);
    Ui::UartSsiController *ui;
    bool m_last_command_read;
    bool GetSSIREGXPointer(QString name, Register **preg);

    void QBAtoTXRDUM(QByteArray &data);
    void QBAfromRXRDUM(QByteArray &data);
    int Calculate_scramble(void);

    void xor_init(int seed_id, int crc);

    void xor_tick();

    int next_xor_data(int crc);

    volatile quint16 scrambler;
    volatile quint16 polynom;

    QByteArray m_escapedcharsRDUM;
    QByteArray m_escapedreplacementRDUM;

private slots:
};

extern UartSsiController *gp_UartSsiController;

#endif // UARTSSICONTROLLER_H
