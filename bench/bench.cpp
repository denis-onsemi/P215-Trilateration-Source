/**
 * @file:axconnector.cpp   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-41-51
 *
 */


#include <QApplication>
//#include <QAxFactory>
#include "mainapp.h"
#include "bench.h"
#include "project_app.h"



/****************************************************************************
 * @function name: Constructor
 *
 * @param:
 *
 ****************************************************************************/
Bench::Bench(QObject *parent): QObject(parent)
{
    setObjectName("benchboard");    
}


/****************************************************************************
 * @function name: Bench::quit - ---
 * @description:  quit procedure . Closes app immediately
 * @return: ( void )
 ****************************************************************************/
void Bench::quit()
{
    qApp->quit();
}


/****************************************************************************
 * @function name: Bench::set - ---
 *
 * @param:
 *       QString command - command for applet
 *       QVariant data  - data for applet
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool Bench::set(QString command, QVariant data)
{
    Applet::call_from_ui = false;
    bool result =MAINAPP->set(command, data);
    Applet::call_from_ui = true;
    return result;
}


/****************************************************************************
 * @function name: Bench::set - ---
 *
 * @param:
 *         QString command -command for applet
 *        QVariant data - data for applet
 * @description:
 * @return: ( bool )
 ****************************************************************************/
QVariant Bench::run(QString command, QVariant p1, QVariant p2, QVariant p3, QVariant p4, QVariant p5)
{    
    QVariant result;
    Applet::call_from_ui = false;
    MAINAPP->run(command,  &result,p1,p2,p3,p4,p5) ;
    Applet::call_from_ui = true;
    return result;
}

/****************************************************************************
 * @function name: Bench::get - ---
 *
 * @param:
 *         QString command  - command for applet . what to read
 * @description:
 * @return: ( QVariant )
 ****************************************************************************/
QVariant Bench::get(QString command)
{    
    Applet::call_from_ui = false;
    QVariant result;
    MAINAPP->get(command,&result);
    Applet:: call_from_ui = true;
    return result;
}

#ifdef USE_FTDI
#else
bool Bench::open(QString portname)
{
    return MAINAPP->openBoardConnection(portname);
}

bool Bench::close()
{
    if(MAINAPP->isBoardConnected())
    {
        MAINAPP->closeBoardConnection();
        return true;
    }
    return false;
}

QByteArray Bench::transfer(QByteArray data)
{
    QByteArray answer;
    if(MAINAPP->board() && MAINAPP->board()->isConnected())
    {
        MAINAPP->board()->transferData(data, &answer);
    }
    return answer;
}
#endif
void Bench::EnableUi()
{
    Applet::block_ui = 0;
}

void Bench::DisableUi()
{
    Applet::block_ui = 1;
}




void Bench::UpdateUi(QVariant appname)
{
    if(appname.toString().isEmpty())
    {
        for(int i=0;i<MAINAPP->appletsCount();i++){
            MAINAPP->applet(i)->slot_updateAllControls();
        }
    }else{
        for(int i=0;i<MAINAPP->appletsCount();i++){
            if(MAINAPP->applet(i)->name() == appname.toString())
            {
                MAINAPP->applet(i)->slot_updateAllControls();
                break;
            }
        }
    }
}
