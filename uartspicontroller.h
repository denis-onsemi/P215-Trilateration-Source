#ifndef UARTSPICONTROLLER_H
#define UARTSPICONTROLLER_H

#include "applet.h"

namespace Ui {
    class UartSpiController;
}

class UartSpiController : public Applet
{
    Q_OBJECT

public:

    Register STATUS;
    Register DEMUX;
    Register U2SREGX00;
    Register U2SREGX01;
    Register U2SREGX02;
    Register U2SREGX03;
    Register U2SREGADDR;
    Variable U2SADDR;
    Variable BRC_CHANNEL;
    Variable BRC_BRC;
    Variable BRC_SLOTS;
    Variable BRC_LENGTH;
    Variable BRC_PERIOD;
    Variable BRC_RESP_LENGTH;
    Variable BRC2_RESP_LENGTH;
    Variable BRC3_RESP_LENGTH;
    Variable BRC_START;
    Variable REPEAT_TIME;


    void setupBoard();

    explicit UartSpiController(QWidget *parent = 0);
    ~UartSpiController();
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


private:
    void createCustomUi(void);
    Ui::UartSpiController *ui;
    bool m_last_command_read;
    bool GetU2SREGXPointer(QString name, Register **preg);


private slots:
};

extern UartSpiController *gp_UartSpiController;

#endif // UARTSPICONTROLLER_H
