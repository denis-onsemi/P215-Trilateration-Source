/**
 * @file:main.cpp   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-59-17
 *
 */



#include <QApplication>
//#include <QAxFactory>
#include "mainapp.h"
#include <QTime>

/* MAIN */

static QtMsgHandler oldMsgHandler;
static QString logFileName;
static void myMessageHandler(QtMsgType type, const char *msg)
{
    QString txt;
    switch (type)
    {
    case QtDebugMsg:
        txt = QString("Debug[%1]: %2").arg(QTime::currentTime().toString()).arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning[%1]: %2").arg(QTime::currentTime().toString()).arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical[%1]: %2").arg(QTime::currentTime().toString()).arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal[%1]: %2").arg(QTime::currentTime().toString()).arg(msg);
        abort();
    }

    if(!QDir().exists("log"))
        QDir().mkdir("log");

    QFile outFile("log\\"+logFileName);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
    outFile.flush();
    oldMsgHandler(type,msg);
}



class Application:public QApplication{
public:
    Application(int &argc, char **argv, int flags=ApplicationFlags):QApplication(argc, argv, flags){
        setQuitOnLastWindowClosed(false);

        /* stylesheet for aplication*/
        QFile file(":/stylesheet/stylesheet") ;
        file.open(QFile::ReadOnly);
        qApp->setStyleSheet(file.readAll());
        file.close();

        logFileName =  QDateTime::currentDateTime().toString("ddMMyyyy.log");
        oldMsgHandler = qInstallMsgHandler(myMessageHandler);
    }

protected:
    virtual bool winEventFilter ( MSG * msg, long * result );
};






bool  Application::winEventFilter( MSG * msg, long * result )
{

     if (WM_POWERBROADCAST == msg->message ){
         switch(msg->wParam)
         {
            case  PBT_APMSUSPEND:
             MAINAPP->suspend();
            break;
         case  PBT_APMRESUMESUSPEND:
             MAINAPP->wakeUp();
             break;
         }
     }
     else if(WM_ENDSESSION ==msg->message){
         qDebug()<<"logout";
     }

// not used
//     else if(WM_WTSSESSION_CHANGE == msg->message){
//         if(msg->wParam == WTS_SESSION_LOCK){
//            qDebug()<<"Lock";
//         }
//         else if(msg->wParam == WTS_SESSION_UNLOCK){
//             qDebug()<<"Unlock";
//         }
//     }

    return QApplication::winEventFilter(msg,result);

}

/****************************************************************************
 * @function name: main()
 *
 * @param:
 *      int argc
 *      char **argv
 * @description:
 * @return: ( int )
 ****************************************************************************/
int main(int argc, char **argv)
{
    int result=0;    
    Application app(argc, argv);

    /* get bench application instance (singletone)*/
    MAINAPP = MainApp::getInstance();

    /* application initilize*/
    if( MAINAPP->initialize())
    {
        /* starts application */
        MAINAPP->start();
        /* qt app execution */
        result = app.exec();
        /* when finish shutdown bench application */
        MAINAPP->shutdown();
    }
    return result;
}

