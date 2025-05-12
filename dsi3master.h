#ifndef DSI3MASTER_H
#define DSI3MASTER_H

#include "applet.h"

#define DSI3MASTER_CH1     (1)
#define DSI3MASTER_CH2     (2)

namespace Ui {
    class DSI3Master;
}

class DSI3Master : public Applet
{
    Q_OBJECT

public:
    Register MREG00;
    Register MREG01;
    Register MREG02;
    Register MREG03;
    Register MREG04;
    Register MREG05;
    Register MREG06;
    Register MREG07;
    Register MREG08;
    Register MREG09;
    Register MREG10;
    Register MREG11;

    Register MWriteREAD;
    Register MReadREAD;
    Register MWriteWRITE;
    Register MReadWRITE;
    Register MWriteCOMMAND;
    Register MWriteCRM;
    Register MReadCRM;

    Variable MVar_version;
    Variable VarStartOfBRC;
    Variable CHANNEL;

    bool WriteAllConfig(void);

    void setupBoard();
    bool PerformREAD(QStringList regnames);
    bool PerformREAD(void);
    bool PerformWRITE(QStringList regnames);
    bool PerformWRITE(void);
    bool PerformCOMMAND(void);
    bool PerformCOMMAND(QByteArray txdata, QByteArray *rxdata, int read_count);
    bool PerformCRM(QByteArray txdata, QByteArray *rxdata, int read_count);
    bool PerformCOMMANDCRM(void);
    char CalculateMasterCRC(QByteArray data);
    void Ver_setItemText(int index, QString text);

    void Log(int type, QString message);
    bool QuickConfiguration(void);

    explicit DSI3Master(QWidget *parent = 0);
    ~DSI3Master();
    virtual bool initialize();
    virtual QVariant runAppCommand(const QString &name, const QVariant &p1, const QVariant &p2,const QVariant &p3,const QVariant &p4,const QVariant &p5);
    virtual void suspend();
    virtual void resume();
    virtual bool updateHardware(Direction drirection, const QString &);

private:
    void createCustomUi(void);
    Ui::DSI3Master *ui;
    QList<Register *>MREGS;
    bool ReadAllConfig(void);
    bool ReadAllStatus(void);
    bool ReadClockBaudrate(void);
    bool WriteClockBaudrate(void);
    Register *findREG(const QString &regname);

private slots:
    void slot_UpdateLabels(void);
};

extern DSI3Master *gp_DSI3Master;

#endif // DSI3MASTER_H
