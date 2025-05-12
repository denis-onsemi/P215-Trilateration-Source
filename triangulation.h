#ifndef Triangulation_H
#define Triangulation_H

#include "applet.h"
#include "qcustomplot.h"
#include "qtablewidget.h"


namespace Ui {
class Triangulation;
}

class Triangulation : public Applet
{
    Q_OBJECT

public:
    explicit Triangulation(QWidget *parent = 0);
    ~Triangulation();
    virtual bool initialize();
    virtual QVariant runAppCommand(const QString &name, const QVariant &p1,const QVariant &p2,const QVariant &p3,const QVariant &p4,const QVariant &p5);
    void IsConstructable(double r0, double r1, double r2);
    void setupCustomUI();
    void SaveObstacles();
    void plotObstacles();
    void saveSettings(double S2X, double S2Y, double S3X, double S3Y, double SOS, quint8 proj_sel, bool AdAxRg, int MemTol, int MM_thr, int alg_tol, int prus_tol);


private slots:
    void on_cbAlgChoice_currentIndexChanged(int index);

private:
    Ui::Triangulation *ui;
};

extern Triangulation *gp_Triangulation;

#endif // Triangulation_H



