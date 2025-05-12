/**
 * @file:localconnector.cpp
 * @description:
 *    This is named pipe connector for controlling app.
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *
 */
#include "localserver.h"
#include <QMessageBox>
#include <QLocalSocket>
#include "mainapp.h"
#include <QInputDialog>
#include <QVariant>
#include <QTime>

QVariant QString_to_QVariant(const QString &str )
{
    QVariant result;
    char c = str[0].toLatin1();
    /* types f- float, i -integer, s- string */
        switch (c)
        {
            case ( 'f' ):
                if(str.count()>1 && str[1]=='#')
                {
                    QVariantList array;
                    foreach(const QString &s,str.mid(2).split(';')){
                        array.append(s.toDouble());
                    }
                    result = array;
                }else
                    result = str.mid(1).toDouble();
                break;

            case ( 'i' ):
                //array of integer signed 32bit
                if(str.count()>1 && str[1]=='#')
                {
                    QVariantList array;
                    foreach(const QString &s,str.mid(2).split(';')){
                        array.append(s.toInt());
                    }
                    result = array;
                }
                // integer value signed 32 or 64bit
                else{
                    long long tmp  = str.mid(1).toLongLong();
                    if((unsigned long long) tmp &0x80000000)
                        result = tmp;
                    else
                        result = str.mid(1).toInt();
                }
                break;


            case ( 's' ):
                result = str.mid(1);
                break;

            case ('x'):
                result = QByteArray::fromHex(str.mid(1).toLatin1());
                break;
        }
    return result;
}

QString QVariant_to_QString(const QVariant &var)
{
    QString result;
    switch (var.type())
    {

    case QVariant::Double:
        result = 'f' + var.toString().toLatin1();
    break;

    case QVariant::List:
        {
            if(var.toList().size()>0)
            {
                int i=0;
                foreach(const QVariant &v,var.toList())
                {
                    if(i==0 && (v.type()== QVariant::Int || v.type()== QVariant::LongLong || v.type()== QVariant::UInt))
                    {
                        result += "i#";
                    }
                    else if(i==0 && v.type()== QVariant::Double){
                        result +="f#";
                    }
                    result +=v.toString()+";";
                    i++;
                }
                if(result.endsWith(";")) result.chop(1);
            }
            else result = "None";
        }
        break;

    case QVariant::Bool:
        result = 'i' + QString::number(var.toInt()).toLatin1();
        break;
    case QVariant::LongLong:
    case QVariant::UInt:
    case QVariant::Int:
        result = 'i' + var.toString().toLatin1();
        break;

    case QVariant::String:
        result = 's' + var.toString().toLatin1();
        break;
    case QVariant::ByteArray:
        result = 'x' + var.toByteArray().toHex();
        break;

    default:
        result = "None";
        break;
    }
    return result;
}


/****************************************************************************
 * @function name: LocalConnector::LocalConnector()
 *
 * @param:
 *
 * @description:
 * @return: ( void )
 ****************************************************************************/
LocalServer::LocalServer()
{    
    mp_server =0;
    connect(this,SIGNAL(log(int,QString)),gp_Logs,SLOT(slot_addLog(int,QString)));
}

/****************************************************************************
 * @function name: LocalConnector::initialize()
 *
 * @param:const QString &pipename - pipename to connect to
 *
 * @description:
 * @return: ( void )
 ****************************************************************************/
bool LocalServer::initialize(const QString &pipename)
{

    m_file_pipe_name = genName(pipename);

    return restore();
}


/****************************************************************************
 * @function name: Bench::processInput()
 *
 * @param:
 *      void
 * @description: processes string protocol
 * @return: ( void )
 ****************************************************************************/
QByteArray LocalServer::parse(const QByteArray &data_in)
{
    //constants


    QByteArray answer;
    QStringList l = QString(data_in).split('\t');

    // SET
    // this command has always 3 items :
    // (set) <tab> (register/variable) <tab> (value) <cr>
    //
    if (l.at(0) == "set" && l.size()==3 )
    {
        QVariant value_in = QString_to_QVariant(l.at(2));
        Applet::call_from_ui = false;
        bool ok = MAINAPP->set(l.at(1), value_in);
        Applet::call_from_ui = true;
        if(ok)     answer ="ok\n";
        else answer = "err\n";
    }


    // RUN
    // this command has minimum 2 items
    // run <tab> (function) <tab> (paramater1) <tab> (paramater2) <tab> .... <cr>
    //
    else if( l.at(0)== "run" && l.size()>=2)
    {
        QVariant p[5];
        QVariant value_out;
        if(l.size() == 3)
        {
            p[0] = QString_to_QVariant(l.at(2));
        }
        else{
            int i=0;
            foreach(const QString &txt, l.mid(2))
            {
                p[i++] = QString_to_QVariant(txt);
            }            
        }
        Applet::call_from_ui = false;
        bool ok =  MAINAPP->run(l.at(1), &value_out, p[0],p[1],p[2],p[3],p[4]);
        Applet::call_from_ui = true;
        answer = QVariant_to_QString(value_out).toLatin1();
        answer.append(char(0x0A));
        if(!ok ) answer = "err\n";
    }

    // GET
    // this command has always 2 items
    // (get) <tab> (register/variable) <cr>
    //
    else if (l.size() == 2 && l.at(0) == "get")
    {        
        QVariant ret;
        Applet::call_from_ui = false;
        bool ok =  MAINAPP->get(l.at(1), &ret);
        Applet::call_from_ui = true;
        answer = QVariant_to_QString(ret).toLatin1();
        answer.append(char(0x0A));
        if(!ok) answer = "err\n";
    }
    else {
        answer = "err\n";
    }
    return answer;
}





/****************************************************************************
 * @function name: Bench::processConnection()
 *
 * @param:
 *
 *
 * @description:
 * @return: ( void )
 ****************************************************************************/
void LocalServer::processConnection()
{        
    QLocalSocket *clientConnection = mp_server->nextPendingConnection();    

    QTime t;
    t.start();
    while(t.elapsed()<100)
    {
        if(clientConnection->waitForReadyRead(10) && clientConnection->canReadLine()){
            input = clientConnection->readAll();
            break;
        }
    }

    if(!input.isEmpty())
    {
        QList<QByteArray> l = input.replace("\r", "").split('\n');
        QByteArray last_line;
        foreach(const QByteArray &line, l)
        {
            if(last_line == line || line.trimmed().isEmpty()) continue;
            emit log(LOG_COLOR(Qt::cyan), QString(line).replace('\t'," "));
            output += parse(line);
            last_line = line;
        }
    }
    emit log(LOG_COLOR(Qt::darkCyan),output);
    clientConnection->write(output);
    clientConnection->flush();
    //clientConnection->disconnectFromServer();
    input.clear();
    output.clear();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));


}


bool LocalServer::restore()
{
    bool result= false;
    if(!mp_server)
    {
        qDebug()<<"LocalServer:new"<<m_file_pipe_name;
        mp_server = new QLocalServer(this);
        connect(mp_server, SIGNAL(newConnection()), this, SLOT(processConnection()));
        if(!m_file_pipe_name.isEmpty())
        {
            if (mp_server->listen(m_file_pipe_name) )
            {
                    result = true;
                    mp_server->setMaxPendingConnections(1);
            }
            else{
                QMessageBox::critical(0, "Local server failed",
                                      QString("Unable to start interprocess server: \n%1.")
                                      .arg(mp_server->errorString()));
            }
        }
    }
    else if(!mp_server->isListening()){        
        result = mp_server->listen(m_file_pipe_name);        
    }
    return result;
}

void LocalServer::shutdown()
{    
    if(mp_server)
    {
        qDebug()<<"LocalServer:shutdown";
        mp_server->close();
        mp_server->disconnect(this);
        delete mp_server;
        mp_server=0;
    }
}

QString LocalServer::genName(const QString &pipename)
{
    QString new_file_pipe_name =  QString("\\\\.\\pipe\\%1").arg(pipename);
    do
    {
        if(QFile::exists(new_file_pipe_name)
                || !new_file_pipe_name.startsWith("\\\\.\\pipe\\"))
        {
            QMessageBox msg;
            msg.setText("Pipe is already used by another software\nUse another pipename?(Yes)");
            msg.addButton(QMessageBox::Yes);
            msg.addButton(QMessageBox::Close);
            int res = msg.exec();
            if(res== QMessageBox::Yes)
            {
                bool ok;
                QString text = QInputDialog::getText(0, "",
                                                     "Pipename", QLineEdit::Normal,
                                                     "", &ok);
                if(ok && !text.isEmpty())
                    new_file_pipe_name = QString("\\\\.\\pipe\\%1").arg(text);
            }
            else {
                return QString();
            }
        }
        else {
            //found name
            break;
        }
    }while(1);

    return new_file_pipe_name;
}


LocalServer *LocalServer::instance()
{
    static LocalServer *pinstance;
    if(pinstance==0){
        pinstance = new LocalServer();
    }
    return pinstance;
}
