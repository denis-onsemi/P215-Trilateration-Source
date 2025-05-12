/**
 * @file:mcuprotocol.cpp   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *
 */


#include "serport.h"
#include "mainapp.h"
#include <QDebug>
#include <QFileDialog>
#include <QProcess>
#include <QMessageBox>
#include "uartuartcontroller.h"

// fast connection protocol

#define MCU_REQUEST      ('\x61')
#define MCU_REQUEST_UPID ('\x80')
#define MCU_ACK          ('\x6e')
#define MCU_RESET        ("\x18\x18\x18\x18\x18\x18")

QMutex mutex;

#include "circularbuffer.h"

CircularBuffer tx_buf(14000);
CircularBuffer rx1_buf(14000);
CircularBuffer rx2_buf(14000);
CircularBuffer rxbridge_buf(1200);
CircularBuffer rxnint_buf(10);


SerPort *gp_SerPort;

/****************************************************************************
 * @function name: CONSTRUCTOR
 *
 ****************************************************************************/
SerPort::SerPort()
    :QThread(0)
{
    //m_logMode = false;
    m_logMode = true;
    mp_channel = new SerialPortChannel();
    m_finish = false;
    m_timeout = DEFAULT_TIMEOUT;
    m_errors_count =0;
    m_suspend=false;
    m_baudrate=923076ul;
    m_connected = false;
    m_escapedchars[0] = 0x10;
    m_escapedreplacement[0] = 0x00;
    m_escapedchars[1] = 0x11;
    m_escapedreplacement[1] = 0x01;
    m_escapedchars[2] = 0x13;
    m_escapedreplacement[2] = 0x02;

    tx_buf.reset();
    rx1_buf.reset();
    rx2_buf.reset();
    rxbridge_buf.reset();
    rxnint_buf.reset();
    this->start(QThread::HighPriority);
}


/****************************************************************************
 * @function name: DESTRUCTOR
 *
 ****************************************************************************/
SerPort::~SerPort()
{
    m_finish = true;
    mp_channel->close();
    terminate();
}

/****************************************************************************
 * @function name: SerPort::connect()
 *
 * @param:
 *    const QString & port - port name
 * @description: connects mcu .
 * 1. open channel , 2. controls if there is some board connected
 * @return: ( void )
 ****************************************************************************/
bool SerPort::connectDevice(const QString & port,bool /*reset*/)
{
    //UNREFERENCED_PARAMETER(reset);
    bool result = false;

    gp_Settings->getHKMC();

    if(port == "mockup")
    {
    }
    else if(port == "no_board")
    {
        m_mode = NOBOARD;
        m_port = port;
        result = true;
        m_connected = true;
        gp_Settings->mVersion = VERSION_NOBOARD;
        MAINWINDOW->updateVisibleWindows();
    }
    else
    {
        m_mode = MICROCONTROLLER;
        mp_channel->setRTS(0);
        if( !port.isEmpty()
                && mp_channel->open((port), m_baudrate)
                )
        {

            if(checkDeviceUPID())
            {
                result = true;
               // if(reset) resetDevice();
                m_port = port;
                m_connected =true;
                gp_Settings->mVersion = VERSION_UPID;
                gp_Settings->mUPIDcontroller_header = "UART-SPI Controller";
                gp_Settings->mUPIDmaster_header = "ASIC Master";
                gp_Settings->mMOBDcontroller_header = "-";
                gp_Settings->mMOBDmaster_header = "-";
                gp_Settings->mRDUMcontroller_header = "-";
                gp_Settings->mRDUMmaster_header = "-";
                log_message(LOG_UART_INFO,"ASIC master connected");
                MAINWINDOW->updateVisibleWindows();
            }
            else if (checkDeviceMOBD())
            {
                result = true;
                m_port = port;
                m_connected =true;
                gp_Settings->mVersion = VERSION_MOBD;
                gp_Settings->mUPIDcontroller_header = "-";
                gp_Settings->mUPIDmaster_header = "-";
                gp_Settings->mMOBDcontroller_header = "UART-UART Controller";
                gp_Settings->mMOBDmaster_header = "DSI3 Master";
                gp_Settings->mRDUMcontroller_header = "-";
                gp_Settings->mRDUMmaster_header = "-";
                log_message(LOG_UART_INFO,"DSI3PAS FPGA master connected");
                MAINWINDOW->updateVisibleWindows();
            }
            else if (checkDeviceRDUM())
            {
                result = true;
                m_port = port;
                m_connected =true;
                gp_Settings->mVersion = VERSION_RDUM;
                gp_Settings->mUPIDcontroller_header = "-";
                gp_Settings->mUPIDmaster_header = "-";
                gp_Settings->mMOBDcontroller_header = "-";
                gp_Settings->mMOBDmaster_header = "-";
                gp_Settings->mRDUMcontroller_header = "UART-SSI Controller";
                gp_Settings->mRDUMmaster_header = "RDUM Master";
                log_message(LOG_UART_INFO,"RDUM master connected");
                MAINWINDOW->updateVisibleWindows();
            }
            else if (checkDeviceRDUM_SPIMASTER())
            {
                result = true;
                m_port = port;
                m_connected =true;
                gp_Settings->mVersion = VERSION_RDUM_SPIMASTER;
                gp_Settings->mUPIDcontroller_header = "-";
                gp_Settings->mUPIDmaster_header = "-";
                gp_Settings->mMOBDcontroller_header = "-";
                gp_Settings->mMOBDmaster_header = "-";
                gp_Settings->mRDUMcontroller_header = "UART-SSI Controller";
                gp_Settings->mRDUMmaster_header = "RDUM Master";
                log_message(LOG_UART_INFO,"RDUM master connected");
                MAINWINDOW->updateVisibleWindows();
            }
            else
            {
                mp_channel->close();
            }
            tx_buf.reset();
            rx1_buf.reset();
            rx2_buf.reset();
            rxbridge_buf.reset();
            rxnint_buf.reset();
            //checkVersion();

        }
    }    
    if(result)
    {
        log_message(LOG_UART_INFO,"Connected to "%port);
        emit signal_connected();
        gp_UartUartController->SwitchUART2UART();
    }

    return  result;
}


/****************************************************************************
 * @function name: SerPort::run()
 *
 * @param:
 *     void
 * @description: thread runner
 * @return: ( void )
 ****************************************************************************/
void SerPort::run()
{
    bool reconnect = false;
    int i;
    //timer for reconnect
    QTime tmr;
    QTime tmr_sleep;
    tmr.start();
    tmr_sleep.start();
    while( !m_finish )
    {
        if(m_mode == MICROCONTROLLER && isConnected())
        {
            //write new data to SerialPort
            if(tx_buf.size()>0)
            {
                mp_channel->sendData(tx_buf.readAll());
                emit signal_tx();
                tmr_sleep.restart();
            }
            //wait for data on Serial Port

            while(mp_channel->bytesAvailable())
            {
                QByteArray rx;
                emit signal_rx();
                if(mp_channel->readAll(rx) != -1)
                {
                    if (gp_Settings->mVersion == VERSION_MOBD)
                    {
                        for (i = 0; i < rx.size(); i++)
                        {
                            switch (rx.at(i)>>4) {
                            case 0:
                                rx1_buf.write(rx.mid(i, 1), 1);
                                break;
                            case 1:
                                rx2_buf.write(rx.mid(i, 1), 1);
                                break;
                            case 2:
                                rx1_buf.write(rx.mid(i, 1), 1);
                                break;
                            case 3:
                                rx2_buf.write(rx.mid(i, 1), 1);
                                break;
                            case 4:
                                rxbridge_buf.write(rx.mid(i, 1), 1);
                                break;
                            case 5:
                                rxbridge_buf.write(rx.mid(i, 1), 1);
                                break;
                            case 7:
                                rxnint_buf.write(rx.mid(i, 1), 1);
                                break;
                            default:
                                break;
                            }
                        }
                    }
                    else
                    {
                        rx1_buf.write(rx);
                    }
                    tmr.restart();
                }
                tmr_sleep.restart();
            }


            if (tmr_sleep.elapsed() > 20)       // prev 50
            {
                this->msleep(20);
            }


            // if is time to check connection
            if(tmr.elapsed() > 2000 )
            {
                tmr.restart();                
                bool connected = isConnected();
                bool check_passed = true;
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
            tmr.restart();
            int retries=10;
            disconnectDevice(NEED_RECONNECT);
            log_message(LOG_UART_INFO,"Auto reconnecting...");
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

        while(m_suspend) {msleep(100);}
    }
    exec();
}



/****************************************************************************
 * @function name: SerPort::disconnectMcu()
 *
 * @param:
 *           void
 * @description:
 * @return: ( void)
 ****************************************************************************/
void SerPort::disconnectDevice(DisconnectReason reason)
{     
    do{
        if(reason == DUE_ERROR)
        {
            log_message(LOG_UART_ERROR,"Disconnect due to errors");
        }
        else if(reason== NEED_RECONNECT)
        {
            log_message(LOG_UART_INFO,"Reconnect");
        }
        else if(reason==USER_COMMAND){
            log_message(LOG_UART_INFO,"Disconnect by user");
        }

        if(m_mode== MICROCONTROLLER && mp_channel)
        {
            mp_channel->setRTS(0);
            mp_channel->close();
        }
        //emit signal_disconnected();
        m_connected = false;
    }while(0);
}

/****************************************************************************
 * @function name: SerPort::availablePorts()
 *
 * @param:
 *           QStringList *plist
 * @description:
 * @return: ( void)
 ****************************************************************************/
void SerPort::availablePorts(QStringList *plist)
{
    mp_channel->enumDevices(plist);
    plist->append("no_board");
}

/****************************************************************************
 * @function name: SerPort::scanForMcu()
 *
 * @param:
 *           const QStringList &list_in
 *           QStringList *plist_out
 * @description:
 * @return: ( void)
 ****************************************************************************/
void SerPort::scanDevices(const QStringList &list_in, QStringList *plist_out)
{
    if(!isConnected())
    {
        foreach(const QString& port, list_in)
        {
            if(port == "no_board" || port == "mockup" )
            {
                plist_out->append(port);
            }
            else if( mp_channel->open((port), m_baudrate))
            {
                m_mode = MICROCONTROLLER;
                if(checkDeviceUPID())
                {
                    //port.append("_M");
                    plist_out->append(port);
                }
                else if(checkDeviceMOBD())
                {
                    //port.append("_U");
                    plist_out->append(port);
                }
                else if(checkDeviceRDUM())
                {
                    //port.append("_U");
                    plist_out->append(port);
                }
                else if(checkDeviceRDUM_SPIMASTER())
                {
                    //port.append("_U");
                    plist_out->append(port);
                }
                mp_channel->close();
            }
        }
    }
}

/****************************************************************************
 * @function name: SerPort::timeout()
 *
 * @param:
 *          const quint32 &timeout
 * @description:
 * @return: ( void )
 ****************************************************************************/
void SerPort::setTimeout(const quint32 &timeout)
{
    m_timeout = timeout;
}


/****************************************************************************
 * @function name: SerPort::isConnected()
 *
 * @param:
 *          void
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool SerPort::isConnected() const
{
    return m_connected;
}

/****************************************************************************
 * @function name: SerPort::transferData()
 *
 * @param:
 *          const QByteArray &data_write
 *          QByteArray *pdata_read
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool SerPort::transferData(QByteArray &data_write, QByteArray *pdata_read,int timeout)
{
    UNREFERENCED_PARAMETER(data_write);
    UNREFERENCED_PARAMETER(pdata_read);
    UNREFERENCED_PARAMETER(timeout);
    bool result = false;
    return result;
}


/*****************************************************************************/
bool SerPort::writereadData(const QByteArray &data_write, QByteArray *pdata_read, bool write, bool read, int channel, int read_length, int timeout)
{
   bool result_read = true;
   bool result_write = true;
   QByteArray answer;
   QByteArray tx_data;

   if(isConnected())
   {
       if(m_mode == MICROCONTROLLER)
       {
           if (write)
           {
               result_write = false;

               if (read)
               {
                    switch (channel) {
                    case RX1_BUF:
                        rx1_buf.reset();
                        break;
                    case RX2_BUF:
                        rx2_buf.reset();
                        break;
                    case RXBRIDGE_BUF:
                        rxbridge_buf.reset();
                        break;
                    case RXNINT_BUF:
                        rxnint_buf.reset();
                        break;
                   }
               }

               if(data_write.length() > 0)
               {
                   tx_data = data_write;

                   if(m_logMode)
                   {
                       //log_message(LOG_UART_TX, tx_data.toHex());
                   }

                   tx_buf.write(tx_data);
                   result_write = true;
               }
           }

           if (read)
           {
               QTime tmr;
               tmr.start();

               result_read = false;

               if( timeout == -1 )          //if timeout parameter is -1 set default
                   timeout = m_timeout;

               while(!result_read && (tmr.elapsed() < timeout))
               {
                   switch (channel) {
                   case RX1_BUF:
                       answer = rx1_buf.readSize(read_length, &result_read);
                       break;
                   case RX2_BUF:
                       answer = rx2_buf.readSize(read_length, &result_read);
                       break;
                   case RXBRIDGE_BUF:
                       answer = rxbridge_buf.readSize(read_length, &result_read);
                       break;
                   case RXNINT_BUF:
                       answer = rxnint_buf.readSize(read_length, &result_read);
                       break;
                  }

                   if(result_read)
                   {
                       if((m_logMode) && (read_length != 0))
                       {
                            //log_message(LOG_UART_RX,QString(answer.toHex()));
                       }

                       if( pdata_read)
                       {
                           *pdata_read= answer;
                       }
                   }
               }
               if (result_read == false)
               {
                    //QByteArray rest = rx1_buf.readAll();
                    //log_message(LOG_UART_ERROR, "RX Data without end char");
                    //log_message(LOG_UART_RX,QString("No end char:") + QString(rest.toHex()));
               }
           }
        }
   }
   return (result_write && result_read);
}

/*****************************************************************************/
bool SerPort::writereadDataUPID(const QByteArray &data_write, QByteArray *pdata_read, bool write, bool read, int timeout)
{
   bool result_read = true;
   bool result_write = true;
   QByteArray answer;
   QByteArray tx_data;

   if(isConnected())
   {
       if(m_mode == MICROCONTROLLER)
       {
           if (write)
           {
               result_write = false;

               if (read)
                   rx1_buf.reset();

               if(data_write.length() > 0)
               {
                   tx_data = data_write;
                   QBAtoTX(tx_data);

                   if(m_logMode)
                   {
                       //log_message(LOG_SPI_TX, tx_data.toHex());
                   }

                   tx_buf.write(tx_data);
                   result_write = true;
               }
           }

           if (read)
           {
               QTime tmr;
               tmr.start();

               result_read = false;

               if( timeout == -1 )          //if timeout parameter is -1 set default
                   timeout = m_timeout;

               while(!result_read && (tmr.elapsed() < timeout))
               {
                   answer = rx1_buf.readResponse(&result_read);
                   if(result_read)
                   {
                       if(m_logMode)
                            //log_message(LOG_SPI_RX,QString(answer.toHex()));

                       if( pdata_read)
                       {
                           QBAfromRX(answer);

                           *pdata_read= answer;
                       }
                   }
               }
               if (result_read == false)
               {
                    QByteArray rest = rx1_buf.readAll();
                    log_message(LOG_UART_ERROR, "RX Data without end char");
                    log_message(LOG_SPI_RX,QString("No end char:") + QString(rest.toHex()));
               }
           }
        }
   }
   return (result_write && result_read);
}

/*****************************************************************************/
bool SerPort::writereadDataRDUM(const QByteArray &data_write, QByteArray *pdata_read, bool write, bool read, int timeout)
{
   bool result_read = true;
   bool result_write = true;
   QByteArray answer;
   QByteArray tx_data;

   if(isConnected())
   {
       if(m_mode == MICROCONTROLLER)
       {
           if (write)
           {
               result_write = false;

               if (read)
                   rx1_buf.reset();

               if(data_write.length() > 0)
               {
                   tx_data = data_write;
                   //QBAtoTX(tx_data);

                   if(m_logMode)
                   {
                       //log_message(LOG_SPI_TX, tx_data.toHex());
                   }

                   tx_buf.write(tx_data);
                   result_write = true;
               }
           }

           if (read)
           {
               QTime tmr;
               tmr.start();

               result_read = false;

               if( timeout == -1 )          //if timeout parameter is -1 set default
                   timeout = m_timeout;

               while(!result_read && (tmr.elapsed() < timeout))
               {
                   answer = rx1_buf.readResponseRDUM(&result_read);
                   if(result_read)
                   {
                       if(m_logMode)
                            //log_message(LOG_SPI_RX,QString(answer.toHex()));

                       if( pdata_read)
                       {
                           //QBAfromRX(answer);

                           *pdata_read= answer;
                       }
                   }
               }
               if (result_read == false)
               {
                    QByteArray rest = rx1_buf.readAll();
                    log_message(LOG_UART_ERROR, "RX Data without end char");
                    log_message(LOG_SPI_RX,QString("No end char:") + QString(rest.toHex()));
               }
           }
        }
   }
   return (result_write && result_read);
}


/****************************************************************************
 * @function name: SerPort::setData()
 *
 * @param:
 *          const QString &variable_name
 *          qint32 variable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
/*bool SerPort::setData(const QString &variable_name, qint32 variable_value)
{
    bool result = true;
    return result;
}*/

/****************************************************************************
 * @function name: SerPort::setData()
 *
 * @param:
 *          const QString &variable_name
 *          QByteArray variable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
/*bool SerPort::setData(const QString &variable_name, QByteArray variable_value)
{
    bool result = true;
    return result;
}*/

/****************************************************************************
 * @function name: SerPort::getData()
 *
 * @param:
 *          const QString &variable_name
 *          qint32 *pvariable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
/*bool SerPort::getData(const QString &variable_name, qint32 *pvariable_value)
{
    bool result = true;
    return result;
}*/

/****************************************************************************
 * @function name: SerPort::getData()
 *
 * @param:
 *          const QString &variable_name
 *          QByteArray *pvariable_value
 * @description:
 * @return: ( bool )
 ****************************************************************************/
/*bool SerPort::getData(const QString &variable_name, QByteArray *pvariable_value)
{
    bool result = true;
    return result;
}*/




/****************************************************************************
 * @function name: SerPort::checkDeviceMOBD()
 *
 * @param:
 *
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool SerPort::checkDeviceMOBD()
{
    QString qsdebug;
    bool result=false;
    int retry=3;
    if(this->m_mode == MICROCONTROLLER )
    {
        log_message(LOG_UART_INFO,"Mode is OK");
        if(mp_channel->isOpened() && !mp_channel->bytesAvailable())
        {
            QByteArray data;
            QByteArray check;
            check[0] = 0x10;  check[1] = 0x08;  check[2] = 0x10;  check[3] = 0x0C;

            while(!result && retry--)
            {
                log_message(LOG_UART_INFO,"Attempt");
                mp_channel->sendData(QByteArray(1,MCU_REQUEST));
                int timeout = 10 ;
                while(timeout--)
                {
                    if (mp_channel->readData(data,1) == 1)
                    {
                        qsdebug = data.toHex();
                        log_message(LOG_UART_INFO, qsdebug);
                        if (data.at(0) == MCU_ACK)
                        {
                            result = true;
                            break;
                        }
                    }
                    else if (mp_channel->readAll(data) > 0)
                    {
                        log_message(LOG_UART_INFO,"ReadAll");
                        qsdebug = data.toHex();
                        log_message(LOG_UART_INFO, qsdebug);
                    }
                    msleep(10);
                }
            }
        }
        else
        {
            log_message(LOG_UART_INFO,"Closed or Bytesavailable");
        }

    }
    else if(m_mode == NOBOARD || m_mode==MOCKUP)
    {
        result=true;
    }
    return result;
}

/****************************************************************************
 * @function name: SerPort::checkDeviceMOBD()
 *
 * @param:
 *
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool SerPort::checkDeviceUPID()
{
    QString qsdebug;
    bool result=false;
    int retry=3;
    if(this->m_mode == MICROCONTROLLER )
    {
        log_message(LOG_UART_INFO,"Mode is OK");
        if(mp_channel->isOpened() && !mp_channel->bytesAvailable())
        {
            QByteArray data;
            QByteArray check;
            check[0] = 0x10;  check[1] = 0x08;  check[2] = 0x10;  check[3] = 0x0C;

            while(!result && retry--)
            {
                log_message(LOG_UART_INFO,"Attempt");
                mp_channel->sendData(QByteArray(1,MCU_REQUEST_UPID));
                int timeout = 10 ;
                while(timeout--)
                {
                    if (mp_channel->readData(data,4) == 4)
                    {
                        if(data == check)
                        {
                            result = true;
                            break;
                        }
                    }
                    msleep(10);
                }
            }
        }
        else
        {
            log_message(LOG_UART_INFO,"Closed or Bytesavailable");
        }

    }
    else if(m_mode == NOBOARD || m_mode==MOCKUP)
    {
        result=true;
    }
    return result;
}

/****************************************************************************
 * @function name: SerPort::checkDeviceMOBD()
 *
 * @param:
 *
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool SerPort::checkDeviceRDUM()
{
    QString qsdebug;
    bool result=false;
    int retry=3;
    if(this->m_mode == MICROCONTROLLER )
    {
        log_message(LOG_UART_INFO,"Mode is OK");
        if(mp_channel->isOpened() && !mp_channel->bytesAvailable())
        {
            QByteArray data;
            QByteArray data_send;
            data_send[0] = 0x01;  data_send[1]=0x00;
            QByteArray check;
            check[0] = 0x01;  check[1] = 0x06;  check[2] = 0x04;

            while(!result && retry--)
            {
                log_message(LOG_UART_INFO,"Attempt");
                mp_channel->sendData(data_send);
                int timeout = 10 ;
                while(timeout--)
                {
                    if (mp_channel->readData(data,3) == 3)
                    {
                        if(data == check)
                        {
                            result = true;
                            break;
                        }
                    }
                    msleep(10);
                }
            }
        }
        else
        {
            log_message(LOG_UART_INFO,"Closed or Bytesavailable");
        }

    }
    else if(m_mode == NOBOARD || m_mode==MOCKUP)
    {
        result = true;
    }
    return result;
}

/****************************************************************************
 * @function name: SerPort::checkDeviceMOBD()
 *
 * @param:
 *
 * @description:
 * @return: ( bool )
 ****************************************************************************/
bool SerPort::checkDeviceRDUM_SPIMASTER()
{
    QString qsdebug;
    bool result=false;
    int retry=3;
    if(this->m_mode == MICROCONTROLLER )
    {
        log_message(LOG_UART_INFO,"Mode is OK");
        if(mp_channel->isOpened() && !mp_channel->bytesAvailable())
        {
            QByteArray data;
            QByteArray data_send;
            data_send[0] = 0x05;
            QByteArray check;
            check[0] = 0x06;

            while(!result && retry--)
            {
                log_message(LOG_UART_INFO,"Attempt");
                mp_channel->sendData(data_send);
                int timeout = 10 ;
                while(timeout--)
                {
                    if (mp_channel->readData(data,1) == 1)
                    {
                        if(data == check)
                        {
                            result = true;
                            break;
                        }
                    }
                    msleep(10);
                }
            }
        }
        else
        {
            log_message(LOG_UART_INFO,"Closed or Bytesavailable");
        }

    }
    else if(m_mode == NOBOARD || m_mode==MOCKUP)
    {
        result = true;
    }
    return result;
}

/****************************************************************************
 * @function name: SerPort::resetDevice()
 * @description: Resets device and wait for responding
 * @return: ( void )
 ****************************************************************************/
void SerPort::resetDevice()
{    

}

/****************************************************************************
 * @function name: SerPort::setLogMode()
 *
 * @param:
 *          bool on
 * @description:
 * @return: ( void )
 ****************************************************************************/
void SerPort::setLogMode(bool on)
{
    m_logMode = on;
}

/****************************************************************************
 * @function name: SerPort::logMode()
 * @param:
 * @description: if  log mode is on
 * @return: ( void )
 ****************************************************************************/

bool SerPort::logMode() const
{
    return m_logMode;
}

/****************************************************************************
 * @function name: SerPort::logMode()
 * @param:
 * @description: if  log mode is on
 * @return: ( bool )
 ****************************************************************************/
bool SerPort::checkSession()
{
    // TODO
    return true;
}


/****************************************************************************
 * @function name: SerPort::clearBuffers()
 *
 * @param:
 *
 * @description:
 * @return: ( void )
 ****************************************************************************/
void SerPort::clearBuffers()
{
    //m_data_in.clear();
    tx_buf.reset();
    rx1_buf.reset();
    rx2_buf.reset();
    rxbridge_buf.reset();
    rxnint_buf.reset();
    // TODO
}


//  typedef enum {LOG_UART_INFO,LOG_UART_ERROR, LOG_UART_TX, LOG_UART_RX, LOG_MOBD, LOG_DSI3_CMD} LogMessage;
void SerPort::log_message( LogMessage message, const QString message_text)
{
    static QTime time = QTime::currentTime();
    const char LOG_TIME_FORMAT[]="hh:mm:ss.zzz";
    int color=-1000;
    int time_diff=time.msecsTo(QTime::currentTime());
    QString text;
    QString unit = "ms";
    if(logMode() == true)
    {
        qDebug() << "signal in logmode";
        if(time_diff>1000) {time_diff/=1000;unit="s";}
        if(message == LOG_UART_TX)
        {
            color =LOG_COLOR(Qt::gray);

            text = QString("UART-UART TX(%1):%2")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(QString(message_text).replace("\n","&lt;cr&gt; "));
        }
        else if(message == LOG_UART_RX)
        {
            color =LOG_COLOR(Qt::gray);
            text = QString("UART-UART RX(+%1%2):%3")
                    .arg(time_diff)
                    .arg(unit)
                    .arg(QString(message_text).replace("\n","&lt;cr&gt; "));
        }
        else if(message == LOG_SPI_TX)
        {
            color =LOG_COLOR(Qt::gray);

            text = QString("UART-UART TX(%1):%2")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(QString(message_text).replace("\n","&lt;cr&gt; "));
        }
        else if(message == LOG_SPI_RX)
        {
            color =LOG_COLOR(Qt::gray);
            text = QString("UART-UART RX(+%1%2):%3")
                    .arg(time_diff)
                    .arg(unit)
                    .arg(QString(message_text).replace("\n","&lt;cr&gt; "));
        }
        else if(message == LOG_UART_INFO)
        {
            color = LOG_COLOR(Qt::darkGray);
            text = QString("INF(%1):%2")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(message_text);
        }
        else if(message == LOG_MOBD)
        {
            color = LOG_COLOR(Qt::green);
            text = QString("MOBD CMD(%1):%2")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(message_text);
        }
        else if(message == LOG_DSI3_CMD)
        {
            color = LOG_COLOR(Qt::blue);
            text = QString("DSI3 CMD(%1):%2")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(message_text);
        }
        else if(message == LOG_UART_ERROR)
        {
            color = -1000;
            text = QString("RX_ERR(+%1%2):%3")
                    .arg(time_diff)
                    .arg(unit)
                    .arg(message_text);
        }
        else if(message == LOG_TIME)
        {
            color = LOG_COLOR(Qt::darkCyan);
            text = QString("TIME (%1):%2")
                    .arg(QTime::currentTime().toString(LOG_TIME_FORMAT))
                    .arg(message_text);
        }
        emit signal_log(color,text);
        qDebug() << text;
        qDebug() << "log_breakpoint";
        time = QTime::currentTime();
    }
    qDebug() << "5";
}

void SerPort::QBAtoTX(QByteArray &data)
{
    int i;
    QByteArray replacement;
    replacement[0] = 0x10;

    for (i = 0; i < m_escapedchars.length(); i++)
    {
        replacement[1] = m_escapedreplacement.at(i);
        data.replace(m_escapedchars.at(i), replacement);
    }
}

void SerPort::QBAfromRX(QByteArray &data)
{
    int i;
    QByteArray newvalue;
    QByteArray replacement;
    replacement[0] = 0x10;

    for (i = (m_escapedchars.length() - 1); i > - 1; i--)
    {
        replacement[1] = m_escapedreplacement.at(i);
        newvalue[0] = m_escapedchars.at(i);
        data.replace(replacement, newvalue);
    }

    replacement[1] = 0x0C;
    if (data.endsWith(replacement))     // remove end char
    {
        data.remove(data.length() - 2, 2);
    }
    replacement[1] = 0x41;
    if (data.endsWith(replacement))     // remove end char
    {
        data.remove(data.length() - 2, 2);
    }
}

void SerPort::QBAfromRXRDUM(QByteArray &data)
{
    int i;
    QByteArray newvalue;
    QByteArray replacement;
    replacement[0] = 0x10;

    for (i = (m_escapedcharsRDUM.length() - 1); i > - 1; i--)
    {
        replacement[1] = m_escapedreplacementRDUM.at(i);
        newvalue[0] = m_escapedcharsRDUM.at(i);
        data.replace(replacement, newvalue);
    }

    if (data.endsWith((char)0x04))     // remove end char
    {
        data.remove(data.length() - 1, 1);
    }
}
