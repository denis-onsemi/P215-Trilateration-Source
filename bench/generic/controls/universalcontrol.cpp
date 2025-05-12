/**
 * @file:customcontrols.cpp   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-52-19
 *
 */

#include "universalcontrol.h"
#include "translator.h"
#include "compiler.h"
#if !TEST
#include "mainapp.h"
#endif


/* pointers to objects for Drag/Drop functionality*/
static QMimeData *gp_MimeData =  NULL;
static QDrag *gp_Drag = NULL;


namespace CustomControls
{




UniversalControl::~UniversalControl()
{

}
/****************************************************************************
* @function name:STATIC  extractParameter()
*
* @param:
*             const QString & name -
*             QString *papplet - applet name which control belongs to
*             QString *pnode -  node is register or variable
*             QString *ppar - ppar is parameter for register
*
* @description: extracts register name from register with bits[]
*
* @return: ( const QString )
****************************************************************************/
bool UniversalControl::extractParameter(const QString & name, QString *papplet,QString *pnode,QString *ppar)
{
    bool result = false;
    QString tmp = QString(name).replace('(',' ').replace(')',' ')
            .replace('[',' ').replace(']',' ').replace('.',' ');
    QStringList l= tmp.split(' ');
    switch(l.size())
    {
    case (1):
        if(pnode) {*pnode = l.at(0);}
        break;
    case (2):
    case (3):
        if(pnode) {*pnode = l.at(0);}
        if(ppar) {*ppar = l.at(1);}
        break;
    case(4):
        if(papplet) {*papplet = l.at(0);}
        if(pnode) {*pnode = l.at(1);}
        if(ppar) {*ppar = l.at(2);}
        break;
    default:
        break;
    }
    return result;
}


/****************************************************************************
* @function name: STATIC startDrag()
*
* @param:
*
*       const QString &namepath
*       QWidget *pwidget
*
* @description: Starts dragging . This function transforms object name
*              passed as pwidget to function to format:
*               Module.Register[field] and sends it to script engine
*              MAINAPP_DRAG_SET/GET - is only for setting registers
*              MAINAPP_DRAG_RUN - is only for buttons or calc functions
*
* @return: ( void )
****************************************************************************/
void UniversalControl::startDrag( UniversalControl *pcontrol, quint32 drag_options)
{
    char tmp[200];
    QString namepath;
    QString access;
#if !TEST
    Applet *papplet;
    papplet = MAINAPP->mainWindow()->activeChild();
    namepath = papplet->name() + "." + pcontrol->name(UniversalControl::FULLPATH);
#else
    // this for testing purposes
#define PROJECT_NAME "TESTMODE"
#define PROJECT_CLASS "TEST"    
    QWidget *papplet = new QWidget;
    namepath = "TESTMODE.";
#endif

    /* assembly MIME string*/
    gp_MimeData =  new QMimeData;

    /*[For Visual Basic  sweeper ]*/

    SPRINTF(tmp, "Set @%s = CreateObject(\"%s.%s\") ", PROJECT_NAME
                ,PROJECT_NAME,PROJECT_CLASS);
    gp_MimeData->setData(QString("VBScript Init Object")
                         , QByteArray(tmp));
    /* DRAG RUN */
    if (drag_options & DRAG_RUN )
    {

        SPRINTF(tmp, "call @%s.run(\"%s\", \"%s\" ) "
                    , PROJECT_NAME
                    , namepath.toLatin1().constData()
                    , pcontrol->name(UniversalControl::PARAMETER).toLatin1().constData()
                    );
        gp_MimeData->setData(QString("VBScript Set Data")
                             , QByteArray(tmp));
    }
    /* DRAG SET/ GET */
    else
    {
        if (drag_options & DRAG_GET )
        {
            SPRINTF(tmp, "# = @%s.get(\"%s\" ) ", PROJECT_NAME, namepath.toLatin1().constData());
            gp_MimeData->setData(QString("VBScript Get Data")
                                 , QByteArray(tmp));     
        }        
        if (drag_options & DRAG_SET )
        {
            SPRINTF(tmp, "call @%s.set(\"%s\", # ) ", PROJECT_NAME, namepath.toLatin1().constData());
            gp_MimeData->setData(QString("VBScript Set Data")
                                 , QByteArray(tmp));
            access+="w";
        }
    }
    SPRINTF(tmp, "Set @%s = Nothing ", PROJECT_NAME);
    gp_MimeData->setData(QString("VBScript Clean-up")
                         , QByteArray(tmp));

    /* [ for my sweeper] */
    QString comment =QString(" #%1:").arg(PROJECT_NAME);
    if(drag_options & DRAG_GET) {
        comment += "get";
        access+="r";
    }
    if(drag_options & DRAG_SET) {
        comment += ",set";
        access+="w";
    }
    if(drag_options & DRAG_RUN) {
        comment += "run";
        access+="x";
    }
    gp_MimeData->setText(namepath+comment);

    // new mode
    gp_MimeData->setData("ControlAccess",access.toLatin1());
    gp_MimeData->setData("ControlPath",namepath.toLatin1());
    gp_MimeData->setData("Control",PROJECT_NAME);
    gp_MimeData->setData("App","Bench");
    gp_MimeData->setData("AppPath",qApp->applicationDirPath().toLatin1());

    /*** start dragging ***/
    gp_Drag = new QDrag(papplet->widget());
    gp_Drag->setMimeData(gp_MimeData);
    gp_Drag->exec(Qt::CopyAction );
}




/****************************************************************************
* @function name:STATIC  stopDrag()
*
* @param:
*             void
* @description: Stops dragging
* @return: ( void)
****************************************************************************/
void UniversalControl::stopDrag()
{
    if (gp_Drag)
    {
        try
        {
            delete gp_Drag;
        }
        catch (...)
        {
            gp_Drag = NULL;
        }
    }
}


/****************************************************************************
* @function name: IsBinded()
*
* @param:
* @description:
* @return: ( bool ) if control is already binded
****************************************************************************/
const bool UniversalControl::isBinded(void) const
{
    return m_binded;
}


/****************************************************************************
* @function name: controlType()
*
* @param:
* @description:
* @return: ( UniversalControl::CustomControlType )
****************************************************************************/
const UniversalControl::CustomControlType UniversalControl::controlType() const
{
    return m_type;
}



/****************************************************************************
* @function name: setControlType()
*
* @param:
*           CustomControlType type
* @description:
* @return: ( void )
****************************************************************************/
void UniversalControl::setControlType(CustomControlType type)
{
    m_type = type;
}


}//UniversalControl
