#ifndef CUSTOMCHECKBOX_H
#define CUSTOMCHECKBOX_H

#include "universalcontrol.h"

namespace CustomControls
{

/****************************************************************************
   * @class :
   * CheckBox
   ****************************************************************************/
class CheckBox: public QCheckBox, public UniversalControl
{
    Q_OBJECT

public:
    CheckBox(QWidget *p);


    ~CheckBox();
    bool editEnabled()const;
    void setEditEnabled(bool on);
    void setEditValue(double v);
    double editValue();

    const QString name(DragName dragname=NODENAME);

    bool bind(const char *slot, QObject *pobj);
    void unbind();
    void setDescription(const QString &description);
    const QString  description();

signals:
    void doubleClick();
    void signal_activated();

protected:
    void leaveEvent(QEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *pevent);
    void mouseDoubleClickEvent(QMouseEvent *);
    /* paint event used for painting rectangle on CTRL press*/
    void paintEvent(QPaintEvent *pev);
    bool event(QEvent *e);
private:

    bool m_editable;
};


}


#endif
