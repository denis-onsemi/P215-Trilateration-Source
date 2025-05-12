#ifndef CUSTOMPUSHBUTTON_H
#define CUSTOMPUSHBUTTON_H

#include "universalcontrol.h"
#include <QMimeData>
namespace CustomControls
{

/****************************************************************************
   * @class :
   * PushButton
   ****************************************************************************/
class PushButton: public QPushButton, public UniversalControl
{
    Q_OBJECT
public:
    PushButton(QWidget *p);

    ~PushButton();
    bool editEnabled()const;
    void setEditEnabled(bool on);
    void setEditValue(double );
    double editValue();
    const QString name(DragName dragname=NODENAME);
    bool bind(const char *slot, QObject *pobj);
    void unbind();
    void setDescription(const QString &description);
    void setCaption(const QString & ){}
    const QString  caption(){return QString();}
    void setAccessibleName(const QString &name);
    const QString function();
    const QVariantList& parameters();
protected:
    QVariantList m_parameters;
    QString m_function;

    /* pointers to objects for Drag/Drop functionality*/
    QMimeData *mp_MimeData;
    QDrag *mp_Drag;
    QObject *mp_applet;

    void leaveEvent(QEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *pevent);
    /* paint event used for painting rectangle on CTRL press*/
    void paintEvent(QPaintEvent *pev);
    bool event(QEvent *e);
private:


};
}
#endif
