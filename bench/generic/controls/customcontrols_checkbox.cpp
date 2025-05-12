#include "customcontrols_checkbox.h"

namespace CustomControls
{


CheckBox::CheckBox(QWidget *p): QCheckBox(p), UniversalControl()
{
  setControlType(CHECKBOX);
  m_HasCachedName = false;

  m_editable = true;
}


CheckBox::~CheckBox()
{
}


bool CheckBox::editEnabled() const
{
  return m_editable;
}


void CheckBox::setEditEnabled(bool on)
{
  m_editable = on;
}


void CheckBox::setEditValue(double v)
{
  setChecked((bool)v);
}


double CheckBox::editValue()
{
  return (double)isChecked();
}


const QString CheckBox::name(DragName dragname)
{
  if ( !m_HasCachedName )
    {
      QString fullpath;
         fullpath = accessibleName();
      extractParameter(fullpath,0,&m_CachedName,&m_CachedParameter);
      m_bindName = fullpath;
      m_HasCachedName =  true;
    }
  switch(dragname)
  {
    case PARAMETER:  return m_CachedParameter;
    case NODENAME: return m_CachedName;
    case FULLPATH: return m_bindName;
    default : break;
  }
  return m_bindName;
}


bool CheckBox::bind(const char *slot, QObject *pobj)
{
  m_binded = true;
  return connect(this, SIGNAL(clicked()), pobj, slot);
}

void CheckBox::unbind()
{
    disconnect(this, SIGNAL(clicked()));
}


void CheckBox::setDescription(const QString &description)
{  
    m_description = description;
}



void CheckBox::leaveEvent(QEvent *)
{
  repaint();
}


void CheckBox::mouseMoveEvent(QMouseEvent *)
{
  repaint();
}


void CheckBox::mousePressEvent(QMouseEvent *pevent)
{
    if(this->editEnabled())
    {
        QCheckBox::mousePressEvent(pevent);
    }
        if (pevent->button() == Qt::LeftButton
                && Qt::ControlModifier == QApplication::keyboardModifiers() )
        {
            startDrag(static_cast<UniversalControl*>(this) , DRAG_SETGET);
            stopDrag();
        }

}

void CheckBox::mouseDoubleClickEvent(QMouseEvent *pev)
{
    emit doubleClick();
    QCheckBox::mouseDoubleClickEvent(pev);
}


void CheckBox::paintEvent(QPaintEvent *pev)
{
  QCheckBox::paintEvent(pev);
  if (Qt::ControlModifier == QApplication::keyboardModifiers())
    {
      PAINT_MARKER;
    }
}

#include <QToolTip>
bool CheckBox::event(QEvent *e)
{
    if(e->type()  == QEvent::ToolTip){
        setToolTip(  QString("<font color=blue>%1</>"
                             "<br><font color=grey>%2</>")
                    .arg(this->accessibleName())
                    .arg(m_description)
                    );
    }

    return QCheckBox::event(e);
}

}

