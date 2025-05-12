#ifndef UARTUARTCONTROLLER_H
#define UARTUARTCONTROLLER_H

#include "applet.h"

namespace Ui {
    class UartUartController;
}

class UartUartController : public Applet
{
    Q_OBJECT

public:
    Register MReadNINT;
    Register UREG00;
    Register UREG01;
    Register UREGANY;

    void setupBoard();

    explicit UartUartController(QWidget *parent = 0);
    ~UartUartController();
    virtual bool initialize();
    virtual QVariant runAppCommand(const QString &name, const QVariant &p1, const QVariant &p2,const QVariant &p3,const QVariant &p4,const QVariant &p5);
    virtual void suspend();
    virtual void resume();
    virtual bool updateHardware(Direction drirection, const QString &);
    bool WriteReadUART(bool inLineEdit, QByteArray &data_write, QByteArray *pdata_read, bool write, bool read, int channel, int read_length, int timeout, bool reset);
    bool WriteReadBRIDGE(bool inLineEdit, QByteArray &data_write, QByteArray *pdata_read, bool write, bool read, int read_length, int timeout);
    bool SetBaudrate(void);
    bool SetMDSEL(int type);
    bool GetNINTx(void);
    char CalculateMOBDCRC(QByteArray data);
    char CalculateCRC(QByteArray data);
    char CalculateUPAECRC(QByteArray data);
    char CalculateUPAECRCwithSEED(char seed, QByteArray data);
    quint16 CalculateUPAECRC16withSEED(quint16 seed, QByteArray data);
    quint16 CalculateCRC16UPAEwithSEEDfromFPGA(quint16 seed, QByteArray data);
    void SwitchUART2UART(void);

private:
    void createCustomUi(void);
    Ui::UartUartController *ui;

    void QBAtoTX(QByteArray &data, int channel, bool reset);
    void QBAfromRX(int channel, QByteArray &data);

    bool WriteBridgeReg(QString regname);
    bool ReadBridgeReg(QString regname);
    void CheckCRC(void);

private slots:

};

extern UartUartController *gp_UartUartController;
//#define BOARD gp_UartUartController

#endif // UARTUARTCONTROLLER_H
