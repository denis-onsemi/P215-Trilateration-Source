/**
 * @file:mcuprotocol.cpp   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *
 */


#include "controlboard.h"
#include "mainapp.h"
#include <QDebug>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>

// fast connection protocol

#define MCU_REQUEST     ('\x05')
#define MCU_ACK         ('\x06')
#define MCU_RESET       ("\x18\x18\x18\x18\x18\x18")

QMutex mutex;

#include "circularbuffer.h"

CircularBuffer rx_buf(100000);
CircularBuffer  tx_buf(100000);



/****************************************************************************
 * @function name: CONSTRUCTOR
 *
 ****************************************************************************/
ControlBoard::ControlBoard()
    :QThread(0)
{
    m_logMode = false;
    mp_channel = new SerialPortChannel();
    m_finish = false;
    m_timeout = DEFAULT_TIMEOUT;
    m_error =0;
    m_suspend=false;
    m_connected = false;
    m_capture_mode = false;
    m_ain_vref  =3.30f;
    m_request = 0;
    rx_buf.reset();
    tx_buf.reset();
    this->start(QThread::HighPriority);
}


/****************************************************************************
 * @function name: DESTRUCTOR
 *
 ****************************************************************************/
ControlBoard::~ControlBoard()
{
    m_finish = true;
    mp_channel->close();
    terminate();
}

/****************************************************************************
 * @function name: ControlBoard::connect()
 *
 * @param:
 *    const QString & port - port name
 * @description: connects mcu .
 * 1. open channel , 2. controls if there is some board connected
 * @return: ( void )
 ****************************************************************************/
bool ControlBoard::connectDevice(const QString & port,bool reset)
{
    bool result = false;
    if(port == "mockup")
    {
        m_mode = MOCKUP;
        initMockup();
        m_port = port;
        if(!MAINAPP->quietMode())
            QMessageBox::information(0,"","Software is in Mock-up mode. \n "
                                            "Communication is simulated!");
        result =true;
        m_connected = true;
    }
    else if(port == "script")
    {
        m_mode = SCRIPT;
        // TODO open file
        QString script_path;
        while(!QFile::exists(script_path))
        {
            QFileDialog dialog;
            script_path = dialog.getOpenFileName(0, "Script path",
                                                 "",
                                                 "Files (*.py)");
            if(dialog.result() == QDialog::Rejected) break;
        }
        if(!script_path.isEmpty() && QFile::exists(script_path))
        {
            m_script_file = script_path;
            m_port = port;
            result =true;
        }
    }
    else
    {
        m_mode = MICROCONTROLLER;
        mp_channel->setRTS(0);
        if( !port.isEmpty()
                && mp_channel->open(port)
                )
        {
            result = checkDevice();
            if(result)
            {
               // if(reset) resetDevice();
                m_port = port;
                m_connected =true;
            }
            else mp_channel->close();
            tx_buf.reset();
            rx_buf.reset();            
            m_request =0;
            m_wdt.restart();            
        }
    }    
    if(result)
    {
        log_message(LOG_MCU_INFO,"Connected to "%port);
        emit signal_connected();
    }

     return result;
}

bool ControlBoard::setBaudrate(quint32 baudrate)
{
    if(mp_channel)
    {
        m_baudrate = baudrate;
        if(isConnected())
        {
            mutex.lock();
            m_suspend=true;
            mutex.unlock();
            mp_channel->sendData("set baudrate="+QByteArray::number(baudrate)+"\n");
            mp_channel->close();
            mp_channel->open(m_port,baudrate);
            mutex.lock();
            m_suspend=false;
            mutex.unlock();
        }

        return true;
    }
    return false;
}

quint32 ControlBoard::baudrate() const
{
    return m_baudrate;
}

/****************************************************************************
 * @function name: ControlBoard::run()
 *
 * @param:
 *     void
 * @description: thread runner
 * @return: ( void )
 ****************************************************************************/
void ControlBoard::run()
{
    bool reconnect = false;
    //timer for reconnect

    m_wdt.start();
    while( !m_finish )
    {
        if(m_mode == MICROCONTROLLER && isConnected())
        {




            if(m_capture_mode)
            {
                QByteArray rx;
                mp_channel->sendData(m_capture_command.toLatin1() );
                mp_channel->setRTS(1);
                emit signal_stream(1);
                while(m_capture_mode)
                {
                    if( mp_channel->bytesAvailable())
                    {
                        mp_channel->readAll(rx);
                        emit signal_captureData(rx);
                    }
                    msleep(100);
                }
                mp_channel->setRTS(0);
                mp_channel->sendData(QByteArray("\x15\x15\x15",3));
                m_wdt.restart();
                emit signal_stream(0);
                msleep(100);
                mp_channel->readAll(rx);
            }






            //write new data to SerialPort
            if(tx_buf.size()>0)
            {
                mp_channel->sendData(tx_buf.readAll());
                m_wdt.restart();
                emit signal_tx();
            }
            //wait for data on Serial Port

            while(mp_channel->bytesAvailable())
            {
                QByteArray rx;
                emit signal_rx();
                // if result ==  -1 -> connection lost
                if(mp_channel->readAll(rx) != -1)
                {
                    rx_buf.write(rx);
                    m_wdt.restart();
                }
            }

            // if is time to check connection
            if(!m_request && m_wdt.elapsed() > 2000 )
            {
                m_wdt.restart();
                bool connected = isConnected();
                bool check_passed = checkDevice();
                reconnect = !connected || (connected && !check_passed) ;
            }
        }
        else //no connected wait for connection
        {
            msleep(500);
        }

        //need reconnect
        if(reconnect)
        {
            reconnect=false;
            m_wdt.restart();
            int retries=10;
            disconnectDevice(NEED_RECONNECT);
            log_message(LOG_MCU_INFO,"Auto reconnecting...");
            while(retries--)
            {
                if(connectDevice(m_port,false)) {
                    break;
                }
                msleep(1000);
            }

            if(isConnected())
            {
                // [1] TODO check session
                // [2] TODO reconfigure
            }
            else disconnectDevice(DUE_ERROR);
        }

        if(!m_request)
            msleep(1);

        while(m_suspend) {msleep(100);}
    }
    exec();
}




/****************************************************************************
 * @function name: ControlBoard::extractValue()
 *
 * @param:
 *           const QByteArray &data
 *           const QString &name
 *           QByteArray *pvalue
 * @description: extract value from string
 * @return: ( bool)
 ****************************************************************************/
bool ControlBoard::extractText(const QByteArray &data, const QString &name, QByteArray *pvalue)
{
    bool result = false;
    QList<QByteArray> l1,l2;
    if(pvalue && !data.isEmpty())
    {
        //l1 = data.split(' ');
        splitBySpace(data,&l1);
        foreach(const QByteArray &item, l1)
        {
            l2 = item.split('=');
            if(name == l2.at(0))
            {
                *pvalue = l2.at(1);
                result = true;
                break;
            }
        }
    }
    return result;
}

bool ControlBoard::extractByteArray(const QByteArray &data, const QString &name, QByteArray *pvalue)
{
    QByteArray text;
    bool result = extractText(data,name,&text);
    if(pvalue) *pvalue = QByteArray::fromHex(text);
    return result;
}

bool ControlBoard::error()
{
    bool result = m_error;
    m_error = 0;
    return result;
}



/****************************************************************************
 * @function name: ControlBoard::disconnectMcu()
 *
 * @param:
 *           void
 * @description:
 * @return: ( void)
 ****************************************************************************/
void ControlBoard::disconnectDevice(DisconnectReason reason)
{     
    do{
        if(m_capture_mode) stopStream();
        //        if(reason == DUE_ERROR)
        //        {
        //            if(!MAINAPP->quietMode())
        //            {
        //                QMessageBox msg;
        //                msg.setText("Communication errors detected");
        //                msg.setInformativeText("Do you want to continue(Ignore) or stop(Cancel)?");
        //                msg.setStandardButtons(QMessageBox::Ignore | QMessageBox::Cancel);
        //                msg.setDefaultButton(QMessageBox::Cancel);
        //                if(msg.exec() == QMessageBox::Ignore) break;
        //                else reason =USER_COMMAND ;
        //            }
        //        }

        if(reason == DUE_ERROR)
        {
            log_message(LOG_MCU_ERROR,"Disconnect due to errors");
        }
        else if(reason== NEED_RECONNECT)
        {
            log_message(LOG_MCU_INFO,"Reconnect");
        }
        else if(reason==USER_COMMAND){
            log_message(LOG_MCU_INFO,"Disconnect by user");
        }

        if(m_mode== MICROCONTROLLER && mp_channel)
        {
            setBaudrate(DEFAULT_BAUDRATE);
            mp_channel->setRTS(0);
            mp_channel->close();
        }
        emit signal_disconnected();
        m_connected = false;
    }while(0);
}

/****************************************************************************
 * @function name: ControlBoard::availablePorts()
 *
 * @param:
 *           QStringList *plist
 * @description:
 * @return: ( void)
 ****************************************************************************/
void ControlBoard::availablePorts(QStringList *plist)
{
    mp_channel->enumDevices(plist);
    plist->append("mockup");
    plist->append("script");
}

/****************************************************************************
 * @function name: ControlBoard::scanForMcu()
 *
 * @param:
 *           const QStringList &list_in
 *           QStringList *plist_out
 * @description:
 * @return: ( void)
 ****************************************************************************/
void ControlBoard::scanDevices(const QStringList &list_in, QStringList *plist_out)
{
    if(!isConnected())
    {
        foreach(const QString& port, list_in)
        {
            if(port == "script" || port == "mockup" )
            {
                plist_out->append(port);
            }
            else if( mp_channel->open(port) )
            {
                m_mode = MICROCONTROLLER;
                if(checkDevice())
                    plist_out->append(port);
                mp_channel->close();
            }
        }
    }
}

/****************************************************************************
 * @function name: ControlBoard::timeout()
 *
 * @param:
 *          const quint32 &timeout
 * @description:
 * @return: ( void )
 ****************************************************************************/
void ControlBoard::setTimeout(const quint32 &timeout)
{
    m_timeout = timeout;
}


/****************************************************************************
 * @function name: ControlBoard::isConnected()
 *
 * @param:
 *          void
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::isConnected() const
{
    return m_connected;
}

/****************************************************************************
 * @function name: ControlBoard::transferData()
 *
 * @param:
 *          const QByteArray &data_write
 *          QByteArray *pdata_read
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::transferData(const QByteArray &data_write, QByteArray *pdata_read, int timeout, int lines)
{
    bool result = false;


    QByteArray answer;
    if(isConnected() && !m_capture_mode)
    {


        switch( m_mode)
        {
        case (MOCKUP):            
                log_message(LOG_MCU_TX, QString(data_write),timeout);
            transferMockup(data_write, &answer);
            log_message(LOG_MCU_RX,QString(answer));
            result =true;
            break;

        case (SCRIPT):            
                log_message(LOG_MCU_TX, QString(data_write),timeout);
            transferScript(data_write, &answer);
            log_message(LOG_MCU_RX,QString(answer));
            result =true;
            break;

        case(MICROCONTROLLER):
            {
               if(m_request) return false;

                int retry =1;
                rx_buf.reset();
                while(retry-- )
                {
                    if(data_write.count()>0)
                    {
                        QByteArray req = data_write;
                        if( data_write.at(data_write.count()-1) !='\n')
                        {
                            req += "\n";
                        }

                        tx_buf.write(req);
                        m_request = 1;

                        int lines_sent ;
                        if(lines==-1)
                            lines_sent = req.count("\n");
                        else
                                lines_sent = lines;


                        //if timeout parameter is -1 set default
                        if( timeout == -1 )
                            timeout = m_timeout;
                        else  // do not wait just send to port and return
                            if(timeout==0) {
                                result=true;
                                break;}

                        log_message(LOG_MCU_TX, QString(req),timeout);
                        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);

                        QTime tmr;
                        tmr.start();
                        int lines_received=0;
                        bool line_ok=1;

                        while( tmr.elapsed()<timeout )
                        {
                            line_ok = 0;
                            answer = rx_buf.readLine(&line_ok);                            
                            if(line_ok)
                            {
                                lines_received ++;
                                tmr.restart();
                                log_message(LOG_MCU_RX,QString(answer.mid(0,200)));
                                qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
                                if(answer.contains(":err")){
                                    qDebug()<<"Error read Serial:"<<req;
                                    m_error = true;
                                }
                                if( pdata_read)
                                {                                   
                                    *pdata_read->append(answer);
                                }
                                if(lines_received >= lines_sent) break;
                            }
                        }                        

                        if(lines_received >= lines_sent)
                        {
                            result=true;
                            break;
                        }
                        else {
                        }
                    }
                }
            }
        break;

        default :
            break;
        }
        if(!result)
        {

            log_message(LOG_MCU_ERROR, answer);
        }
    }
    m_request = 0;

    return result;
}


/****************************************************************************
 * @function name: ControlBoard::setData()
 *
 * @param:
 *          const QString &variable_name
 *          qint32 variable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
void ControlBoard::transferScript(const QByteArray &data_out, QByteArray *pdata_in)
{
    Q_ASSERT(pdata_in);
    QByteArray tmp = data_out;
    tmp.replace("\n","");
    MAINAPP->runScript(m_script_file,tmp,pdata_in);
}

/****************************************************************************
 * @function name: ControlBoard::splitBySpace()
 *
 * @param:
 *          const QByteArray &data
 *          QList<QByteArray> *plist
 * @description:
 * @return: ( qint32 )
 ****************************************************************************/
qint32 ControlBoard::splitBySpace(const QByteArray &data, QList<QByteArray> *plist)
{
    qint32  len = data.count();
    qint32 i = 0;
    qint32 chains = 0;
    const char *pstr = data.constData();
    QByteArray buf;
    char prev = ' ';
    while (1)
    {
        char symb = pstr[i];
        if (symb == ' ' || len == 0)
        {
            if (prev != ' ')
            {
                if(plist)
                    plist->append(buf);
                buf.clear();
                chains++;
            }
        }
        else
        {
            buf.append(symb);
        }
        if (len == 0)
        {
            break;
        }
        else
        {
            len--;
            i++;
            prev = symb;
        }
    }
    return chains;
}

/****************************************************************************
 * @function name: ControlBoard::setData()
 *
 * @param:
 *          const QString &variable_name
 *          qint32 variable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::setData(const QString &variable_name, qint32 variable_value)
{
    bool result ;
    QByteArray request;
    request =QString("set %1=%2\n").arg(variable_name).arg(variable_value).toLatin1() ;
    result = transferData(request) ;
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::setData()
 *
 * @param:
 *          const QString &variable_name
 *          QByteArray variable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::setData(const QString &variable_name, QByteArray variable_value)
{
    bool result ;
    QByteArray request;
    request = QString("set %1=%2\n").arg(variable_name).arg(QString(variable_value)).toLatin1() ;
    result = transferData(request) ;
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::getData()
 *
 * @param:
 *          const QString &variable_name
 *          qint32 *pvariable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::getData(const QString &variable_name, qint32 *pvariable_value)
{
    bool result;
    QByteArray data;
    QByteArray request = QString("get %1").arg(variable_name).toLatin1();
    QByteArray answer;
    result = transferData(request, &answer);
    result &= extractText(answer, variable_name, &data);
    if(pvariable_value &&!data.isEmpty()) *pvariable_value = data.toInt();
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::getData()
 *
 * @param:
 *          const QString &variable_name
 *          QByteArray *pvariable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::getData(const QString &variable_name, QByteArray *pvariable_value)
{
    bool result ;
    QByteArray data;
    QByteArray request = QString("get %1").arg(variable_name).toLatin1();
    QByteArray answer;
    result = transferData(request, &answer);
    answer.replace('\n',"");    
    result &= extractText(answer, variable_name, &data);
    if(pvariable_value && !data.isEmpty()) *pvariable_value = data;    
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::setDataList()
 *
 * @param:
 *          const QList<Variable> &variables
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::setDataList(const QList<Variable> &variables)
{
    bool result = false;
    QByteArray answer;
    QByteArray request;
    if(variables.count()>0)
    {
        request = "set ";
        foreach(const Variable &v, variables)
        {
            switch(v.type())
            {

            case Variable::INTEGER:
                request  += QString("%1=%2 ").arg(v.name()).arg((int)v.value()).toLatin1();
                break;
            case Variable::REAL:
                request  += QString("%1=%2 ").arg(v.name()).arg((double)v.value()).toLatin1();
                break;
            case Variable::ARRAY:
                request  += QString("%1=%2 ").arg(v.name()).arg(QString(v.array()));
                break;

            case Variable::NONE:
                break;
            default:
                break;
            }

        }
        result = transferData(request,&answer);
        if(result)
        {
            foreach(const Variable &v, variables)
            {
                if ( !answer.contains(v.name().toLatin1()) )
                {
                    result = false;
                    break;
                }
            }
        }
    }
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::getDataList()
 *
 * @param:
 *          QList<Variable> *pvariables
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::getDataList(QList<Variable> *pvariables)
{
    bool result = false;
    QByteArray answer;
    QByteArray request;
    if(pvariables &&pvariables->count()>0)
    {
        request ="get ";
        foreach(const Variable &v, *pvariables)
        {
            request += v.name() +' ';
        }
        result =transferData(request, &answer);
        for(int i=0;i<pvariables->size();i++)
        {
            QByteArray data;
            result &= extractText(answer, (*pvariables)[i].name(), &data);
            switch((*pvariables)[i].type())
            {
            case Variable::INTEGER:
                (*pvariables)[i].setValue(data.toInt());
                break;
            case Variable::REAL:
                (*pvariables)[i].setValue(data.toDouble());
                break;
            case Variable::ARRAY:
                (*pvariables)[i].setValue(data);
                break;
            case Variable::NONE:
                break;
            default:
                break;
            }
        }
    }

    return result;
}

QString ControlBoard::idn()
{
    QByteArray idn;
    QRegExp rx("ON Semiconductor,(.*)");
    if(MCU.transferData("*IDN?",&idn,500,1)){
         idn.replace("\n","");
        if(rx.indexIn(idn)>=0){
            return rx.cap(1);
        }
    }
    return QString("(empty)");
}



/****************************************************************************
 * @function name: ControlBoard::checkDevice()
 *
 * @param:
 *
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::checkDevice()
{
    bool result=false;
    int retry=3;
    if(this->m_mode == MICROCONTROLLER )
    {
        if(mp_channel->isOpened() && !mp_channel->bytesAvailable())
        {
            QByteArray data;
            while(!result && retry--)
            {
                mp_channel->sendData(QByteArray(1,MCU_REQUEST));
                int timeout = 10 ;
                while(timeout--)
                {
                    if( (mp_channel->readData(data,1)==1)
                            && (data == QByteArray(1,MCU_ACK)))
                    {
                        result = true;
                        break;
                    }
                    msleep(10);
                }
            }
        }
    }
    else if(m_mode == SCRIPT || m_mode==MOCKUP)
    {
        result=true;
    }
    return result;
}

/****************************************************************************
 * @function name: ControlBoard::resetDevice()
 * @description: Resets device and wait for responding
 * @return: ( void )
 ****************************************************************************/
void ControlBoard::resetDevice()
{    
    log_message(LOG_MCU_INFO,"Board reset");
    mp_channel->sendData(QByteArray(MCU_RESET, 6));
    QByteArray rx ;
    QTime tmr;
    tmr.start();
    do
    {
        mp_channel->sendData(QByteArray("\x5",1));
        msleep(10);
        mp_channel->readData(rx,1);
    }while(rx != QByteArray("\x6",1) && tmr.elapsed()<10000);
    mp_channel->readAll(rx);
}



/****************************************************************************
 * @function name: ControlBoard::logMode()
 * @param:
 * @description: if  log mode is on
 * @return: ( bool )
 ****************************************************************************/
bool ControlBoard::checkSession()
{
    // TODO
    return true;
}



/****************************************************************************
 * @function name: ControlBoard::clearBuffers()
 *
 * @param:
 *
 * @description:
 * @return: ( void )
 ****************************************************************************/
void ControlBoard::clearBuffers()
{
    //m_data_in.clear();
    rx_buf.reset();
    tx_buf.reset();
    // TODO
}


/****************************************************************************
 * @function name: ControlBoard::initMockup()
 *
 * @param:
 *
 * @description:
 * @return: ( void )
 ****************************************************************************/
void ControlBoard::initMockup()
{
    qsrand(100);
}


/****************************************************************************
 * @function name: ControlBoard::readMockUp()
 *
 * @param:
 *          const QByteArray &data_out
 *          QByteArray *pdata_in
 * @description: read simulation function
 * @return: ( void )
 ***************************************************************************/
void ControlBoard::transferMockup(const QByteArray &data_out, QByteArray *pdata_in)
{
    //set
    if( data_out.contains("set") )
    {
        QList<QByteArray> l = QByteArray(data_out).replace("set ","")
                .replace("\n","").trimmed().split(' ');
        foreach(const QByteArray &ba, l)
        {
            *pdata_in += QByteArray(ba).remove(ba.indexOf('='),ba.size()-1) +":ok ";
        }
    }
    //get
    else
    {
        QList<QByteArray> l = QByteArray(data_out).replace("get ","")
                .replace("\n","").trimmed().split(' ');
        foreach(const QByteArray &ba, l)
        {
            quint32 random = qrand();
            if(ba.contains("DIO"))
            {
                random %= 1;
            }
            else if(ba.contains("AIN"))
            {
                random %= 1023;
            }
            *pdata_in += " "+ ba + "=" + QByteArray::number(random);
        }
        *pdata_in = pdata_in->trimmed();
    }
}


void ControlBoard::log_message( LogMessage message, QString message_text, int timeout)
{
    static QTime time = QTime::currentTime();
    const char LOG_TIME_FORMAT[]="hh:mm:ss.zzz";
    int color;
    int time_diff=time.msecsTo(QTime::currentTime());
    QString text;
    QString unit = "ms";


        if(time_diff>1000) {time_diff/=1000;unit="s";}
        if(message == LOG_MCU_TX)
        {
            color =LOG_COLOR(Qt::blue);

            text = QString("<font color=blue> TX(%1,to=%3):%2 </font>")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(QString(message_text).replace("\n","&lt;cr&gt; "))
                    .arg(timeout);
        }
        else if(message == LOG_MCU_RX)
        {
            color =LOG_COLOR(Qt::green);
            const QRegExp rx("(\\w+\\:err)");
            QStringList err_l;
            int pos =0;
            while ((pos = rx.indexIn(message_text, pos)) != -1) {
                 err_l.append(rx.cap(1));
                 pos += rx.matchedLength();
                LOGS->setErrorCount(LOGS->errorCount()+1);
             }

            // replace errors
            foreach( const QString &err,err_l){
                message_text.replace(err,QString("<font color=red>%1</font>").arg(err));
            }            

            message_text.replace("\n","&lt;cr&gt; ");
            text = QString("<p style=\"color:green\">RX(+%1%2):%3</p>")
                    .arg(time_diff)
                    .arg(unit)
                    .arg(message_text);
        }
        else if(message == LOG_MCU_INFO)
        {
            color = LOG_COLOR(Qt::darkGray);
            text = QString("<font color=black>INF(%1):%2</font>")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(message_text);
        }
        else if(message == LOG_MCU_ERROR)
        {
            color = -1000;
            text = QString("<font color=red>RX_TIMEOUT(+%1%2):%3</font>")
                    .arg(time_diff)
                    .arg(unit)
                    .arg(message_text);             
            LOGS->setErrorCount(LOGS->errorCount()+1);
        }
        emit signal_log(color,text);                


        time = QTime::currentTime();

}


bool ControlBoard::setDIO(const char *dio, quint32 val, bool send_immediately)
{
    static QByteArray cache;
    bool result=false;
    if(!cache.startsWith("set")) cache+= "set ";
    cache += QString("%1=%2 ").arg(dio).arg(val).toLatin1();
    if(send_immediately) {result = transferData(cache);cache.clear();}
    return result;
}



bool ControlBoard::setDIODIR(const char *dio, quint32 val, bool send_immediately)
{
    static QByteArray cache;
    bool result=false;
    if(!cache.startsWith("set")) cache+= "set ";
    cache += QString("%1DIR=%2 ").arg(dio).arg(val);
    if(send_immediately) {
        result = transferData(cache);
        cache.clear();
    }
    return result;
}

bool ControlBoard::getDIO(const char *dio, qint32 *pval)
{
    return getData(dio,pval);
}

void ControlBoard::sendBreak()
{
    if(isConnected()){
         mp_channel->sendData(QByteArray("\x15",1));
    }
}



bool ControlBoard::function(const char *name, const QByteArray &data, quint32 timeout
                            , QByteArray *presult1
                            , QByteArray *presult2
                            , QByteArray *presult3
                            , QByteArray *presult4
                            , QByteArray *presult5)
{
    bool result=false;
    QByteArray request;
    QByteArray answer;
    QByteArray *res[5];
    int count=0;
    do{
        if(presult1 != NULL) {res[count++]=presult1;}
        else break;
        if(presult2 != NULL) {res[count++]=presult2;}
        else break;
        if(presult3 != NULL) {res[count++]=presult3;}
        else break;
        if(presult4 != NULL) {res[count++]=presult4;}
        else break;
        if(presult5 != NULL) {res[count++]=presult5;}
    }
    while(0);
    request = "set " +QByteArray(name) + "=" + data;
    if(transferData(request, &answer, timeout))
    {
        //  to mcu => make
        for(int i=0;i<count;i++)
        {
            request = "get " + QByteArray(name) + "=" +QByteArray::number(i+1);
            answer.clear();
            bool trf_ok = transferData(request, &answer);

            QRegExp rx(QString("%1=(\\w+)").arg(name));
            if( trf_ok && rx.indexIn(answer)>=0)
            {                
                *res[i] = QByteArray::fromHex(rx.cap(1).toLatin1());
                result= true;
            }
            else {result=0; break;}
        }
    }
    return result;
}

double ControlBoard::ainVoltage(const char *ain)
{
    qint32 value;
    getData(ain,&value);    
    return (m_ain_vref/ADC_RESOLUTION)*value;
}


ControlBoard& ControlBoard::operator [](const char *dio)
{
    m_cur_dio = QString(dio);
    if(!m_cur_dio.startsWith("DIO") && !m_cur_dio.startsWith("AIN"))
    {
        qDebug()<<QString("MCU[%1] - fail").arg(dio);
    }
    return *this;
}

void ControlBoard::operator =(const int val)
{
    if(m_cur_dio.startsWith("DIO"))
        setData(m_cur_dio,val);
}

void ControlBoard::operator =(const QByteArray &seq)
{
    if(m_cur_dio.startsWith("DIO"))
    {
        QByteArray data;
        data = "set ";
        foreach(const QChar &c, seq)
        {
            data += m_cur_dio+"="+c +" ";
        }
        transferData(data);
    }    
}



ControlBoard::operator int()
{
    qint32 value=0;
    if(m_cur_dio.startsWith("DIO"))
    {
        getData(m_cur_dio,&value);
    }
    else if(m_cur_dio.startsWith("AIN"))
    {
        getData(m_cur_dio,&value);
    }
    return value;
}


void ControlBoard::startStream(const QString &cmd, Applet *papplet)
{    
    connect(this,SIGNAL(signal_captureData(QByteArray))
            ,papplet,SLOT(slot_capturedData(QByteArray)));
    mp_capture_applet = papplet;
    log_message(LOG_MCU_INFO,"Start stream");
    log_message(LOG_MCU_INFO,"RTS=1");
    log_message(LOG_MCU_INFO,cmd);
    m_capture_command = cmd;
    if(! cmd.contains('\n')) m_capture_command += "\n";
    mutex.lock();
    m_capture_mode = true;
    mutex.unlock();
}

void ControlBoard::stopStream()
{    
    mutex.lock();
    m_capture_mode = false;
    mutex.unlock();
    disconnect(mp_capture_applet,SLOT(slot_capturedData(QByteArray)));
    log_message(LOG_MCU_INFO,"RTS=0");
    log_message(LOG_MCU_INFO,"Stop stream");
}


