#include "customcontrols_pushbutton.h"
#include "applet.h"
#include "mainapp.h"
namespace CustomControls
{


PushButton::PushButton(QWidget *p): QPushButton(p), UniversalControl()
{
    m_HasCachedName = false;
    setControlType(PUSHBUTTON);
    setMouseTracking(true);
    mp_applet  =0;
    //    /* find applet name*/
    //    bool found = false;
    //    QWidget *pwidget =   this;
    //    for(int i=0;i<10;i++){
    //        if(!pwidget) break;
    //        qDebug()<<i<<pwidget->objectName();
    //        if (pwidget->inherits("Applet"))
    //        {
    //            found = true;
    //            m_appletName = QString(pwidget->metaObject()->className());
    //            break;
    //        }
    //        else pwidget = pwidget->parentWidget();
    //    }
    //    if(!found )
    //        qWarning()<<QString("Control:%1 parent applet not found").arg(this->text());
}

PushButton::~PushButton()
{

}

bool PushButton::editEnabled() const
{
    return isCheckable();
}

void PushButton::setEditEnabled(bool on)
{
    setCheckable(on);
}

void PushButton::setEditValue(double)
{

}

double PushButton::editValue()
{
    return 0.0f;
}

const QString PushButton::name(DragName dragname)
{
    switch(dragname)
    {
        case PARAMETER:
            return m_CachedParameter;
        break;

        case NODENAME:
            return m_function;
        break;

        case FULLPATH:
            return accessibleName();
        break;
    }

    return m_function;
}

bool PushButton::bind(const char *slot, QObject *pobj)
{
    m_binded = true;
    mp_applet = pobj;
    return connect(this, SIGNAL(clicked()), pobj, slot);
}

void PushButton::unbind()
{
    disconnect(this, SIGNAL(clicked()));
}

void PushButton::setDescription(const QString &description)
{
    m_description = description;
}

void PushButton::setAccessibleName(const QString &name)
{
    QRegExp rx("((.*)\\.)?([\\w\\d_]+)\\((.*)\\)");
    if(rx.exactMatch(name))
    {
        m_bindName = name;
        if(rx.captureCount() ==4) {
            m_appletName = rx.cap(2);
            m_function =rx.cap(3);
            m_CachedParameter = rx.cap(4);
            m_parameters.clear();
            foreach(const QString &s,rx.cap(4).split(',')){
                m_parameters.append(s);
            }
        }
    }
    QPushButton::setAccessibleName(name) ;
}

const QString PushButton::function()
{
    return m_function;
}

const QVariantList &PushButton::parameters()
{
    return m_parameters;
}

void PushButton::leaveEvent(QEvent *)
{
    repaint();
}

void PushButton::mouseMoveEvent(QMouseEvent *)
{
    repaint();
}

void PushButton::mousePressEvent(QMouseEvent *pevent)
{
    QPushButton::mousePressEvent(pevent);    
    if (pevent->button() == Qt::LeftButton
            && Qt::ControlModifier == QApplication::keyboardModifiers() && mp_applet)
    {                
        /* assembly MIME string*/
        mp_MimeData =  new QMimeData;

        /*[For Visual Basic  sweeper ]*/

        mp_MimeData->setData(QString("VBScript Init Object")
                             , QString("Set @%1 = CreateObject(\"%1.%3\") ")
                                .arg(PROJECT_NAME)
                                .arg(PROJECT_CLASS).toLatin1() );

        QString tmp = QString("call @%1.run(\"%2.%3\" ")
                                    .arg(PROJECT_NAME)
                                    .arg(mp_applet->objectName())
                                    .arg(m_function);

        foreach(const QVariant &v, m_parameters)
        {
            tmp += ",\"" + v.toString() +"\"";
        }

        tmp += ") ";

        mp_MimeData->setData(QString("VBScript Set Data")
                             , tmp.toLatin1());

        mp_MimeData->setData(QString("VBScript Clean-up")
                             , QString("Set @%1 = Nothing ").arg(PROJECT_NAME).toLatin1());

        /* [ for my sweeper] */

        mp_MimeData->setText(QString("%2.%3 #%1:run").arg(PROJECT_NAME).arg(mp_applet->objectName()).arg(m_function));

        /*** start dragging ***/
        mp_Drag = new QDrag(static_cast<QWidget*>(mp_applet));
        mp_Drag->setMimeData(mp_MimeData);
        mp_Drag->exec(Qt::CopyAction );
//        mp_MimeData->deleteLater();
        mp_Drag->deleteLater();
}
}

void PushButton::paintEvent(QPaintEvent *pev)
{
    QPushButton::paintEvent(pev);
    if (Qt::ControlModifier == QApplication::keyboardModifiers())
    {
        PAINT_MARKER;
    }
}


#include <QToolTip>
bool PushButton::event(QEvent *e)
{
    if(e->type()  == QEvent::ToolTip){

        if(m_appletName.trimmed()!="")
        {
            setToolTip(  QString("<font color=#0000ff>%1.%2<b>(</b><font color=green>(%3)</font><b>)</b></font>"
                                 "<br><font color=grey>%4</font>"
                                 "<br>")
                         .arg(m_appletName)
                         .arg(m_function )
                         .arg(m_CachedParameter )
                         .arg(this->m_description )
                         );
        }
        else{
            setToolTip(  QString("<font color=blue>%1<b>(</b><font color=green>%2</font><b>)</b></font>"
                                 "<br><font color=grey>%3</font>"
                                 "<br>")

                         .arg(m_function )
                         .arg(m_CachedParameter )
                         .arg(this->m_description )
                         );
        }
    }

    return QPushButton::event(e);
}

}
