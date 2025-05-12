/**
 * @file:axconnector.h   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-41-51
 *
 */


#ifndef BENCH_H
#define BENCH_H


#include <QObject>
#include <QVariant>


class Bench;


/* ActiveX connector */
class Bench : public QObject
{
    Q_OBJECT


public:
    Bench(QObject *parent);

    /*ActiveX interface of application */
public slots:
    /* quit from software*/
    void quit(void );
    /* set for setting parameters for objects*/
    bool set(QString command, QVariant data);
    /* run and set has the same functionality only different name*/
    QVariant run(QString command, QVariant p1=QVariant(), QVariant p2=QVariant(), QVariant p3=QVariant(), QVariant p4=QVariant(), QVariant p5=QVariant());
    /* get is for getting parameter from obejcts*/
    QVariant get(QString command);

#ifdef USE_FTDI
#else
    bool open(QString portname);
    bool close();
    QByteArray transfer(QByteArray data);
#endif
    void EnableUi();
    void DisableUi();
    void UpdateUi(QVariant appname=QVariant());
private:

};


#endif // AXCONNECTOR_H
