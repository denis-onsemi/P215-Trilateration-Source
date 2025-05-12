/**
 * @file:serport.h   -
 * @description:
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *  This class intended for communication with MCU board using simple text protocol.
 *  For detailed information about protocol see: protocol_mcu_bench.docx
 */


#ifndef SERPORT_H
#define SERPORT_H
#include <QThread>
#include <QList>

#include <QVariant>
#include "serialportchannel.h"
#include <QBuffer>

#include <variable.h>
#include <QDateTime>
#include <QTimer>
#include "register.h"
#include "applet.h"

#define DEFAULT_TIMEOUT (800)
#define RX1_BUF         (0)
#define RX2_BUF         (1)
#define RXBRIDGE_BUF    (2)
#define RXNINT_BUF      (3)
#define MDSEL           (4)

/* this class gets data from text in text format paramater = value
 * separated with separarotr CR
 * Protocol gets registers id to table and controls whether data are sent correctly
 * and all answerrs where received from channel.
 * prootocol communicates with channnel in thread .
 * also it has timeout for unreceived registers
 */
class SerPort: public QThread
{
  Q_OBJECT
public:  
  SerPort();
  ~SerPort();
  enum {INPUT = 1, OUTPUT   =  0};
  /* connects to Mcu board*/
  bool connectDevice(const QString & port, bool reset);
  /* disconnect mcu board*/
  typedef enum {USER_COMMAND, DUE_ERROR,NEED_RECONNECT}DisconnectReason;
  void disconnectDevice(DisconnectReason reason);
  /* available ports in system*/
  void availablePorts(QStringList *plist) ;
  /* scans ports from list and finds which has mcu board*/
  void scanDevices(const QStringList &list_in , QStringList *plist_out);
  /* sets timeout*/
  void setTimeout(const quint32 &timeout);    
  /*is connected*/
  bool isConnected(void) const;
  /* resets mcu board*/
  void resetDevice();
  /* sets log mode on/off*/
  void setLogMode(bool on);
  /* true if log mod is on*/
  bool logMode() const;
  /*read session from MCU and compare to my_session*/
  bool checkSession();


  /* clear buffers*/
  void clearBuffers();  
  /* transfers data without each item control*/
  bool transferData(QByteArray &data_write,QByteArray *pdata_read=0, int timeout=-1);
  /* transfers data without each item control*/
  bool writereadData(const QByteArray &data_write, QByteArray *pdata_read=0, bool write = true, bool read = true, int channel = 0, int read_length = 0, int timeout=-1);
  bool writereadDataUPID(const QByteArray &data_write, QByteArray *pdata_read=0, bool write = true, bool read = true, int timeout=-1);
  bool writereadDataRDUM(const QByteArray &data_write, QByteArray *pdata_read=0, bool write = true, bool read = true, int timeout=-1);
  /* sets single data to mcu*/
  //bool setData(const QString &variable_name, qint32 variable_value);
  /* sets single data to mcu*/
  //bool setData(const QString &variable_name, QByteArray variable_value);
  /* gets single data from mcu */
  //bool getData(const QString &variable_name, qint32 *pvariable_value);
  /* gets single data from mcu */
  //bool getData(const QString &variable_name, QByteArray *pvariable_value);


  /* take value from last packet and convert it to integer*/
  //qint32 lastAsInteger(const QString &name);
  /* take value from last packet and convert it to bytearray*/
  //QByteArray lastAsArray(const QString &name);
  /* default timeout setting*/

private:
  // temporary variable
  QString m_cur_dio;
  // command is trigger for streaming start
  QString  m_capture_command;
  Applet *mp_capture_applet;

  bool reconnectDevice();
  /* */
  typedef enum { MICROCONTROLLER, NOBOARD, MOCKUP} Mode;

  typedef enum {LOG_UART_INFO,LOG_UART_ERROR, LOG_UART_TX, LOG_UART_RX, LOG_MOBD, LOG_DSI3_CMD, LOG_TIME, LOG_SPI_TX, LOG_SPI_RX} LogMessage;
  void log_message(LogMessage message , const QString message_text);

  /* check if mcu present on connected channel*/
  bool checkDeviceMOBD();
  bool checkDeviceUPID();
  bool checkDeviceRDUM();
  bool checkDeviceRDUM_SPIMASTER();

  void QBAtoTX(QByteArray &data);

  void QBAfromRX(QByteArray &data);
  void QBAfromRXRDUM(QByteArray &data);

  /* thread runner */
  void run();
  QString m_project;
  /* in logging mode */
  bool m_logMode;
  /* timeout variable */
  quint32 m_timeout;
  /* script file name*/
  QString m_script_file;
  /* current port name */
  QString m_port;
  /* baudrate */
  quint32 m_baudrate;
  /* open mode mockup script, physical microcontroller*/
  Mode m_mode;
  /* counter for errors*/
  qint32 m_errors_count;
  /* flag when time to check */
  bool m_is_time_to_check;
  /* my session number*/
  int m_my_session;
  /* finish flag to terminate thread*/
  bool m_finish;
  bool m_connected;
  bool m_suspend;
  /* channel pointer*/
  SerialPortChannel *mp_channel;

  QByteArray m_escapedchars;
  QByteArray m_escapedreplacement;
  QByteArray m_escapedcharsRDUM;
  QByteArray m_escapedreplacementRDUM;

signals:
  /* signal for logging . Sends signal to LOG window*/
  void signal_log(int ,QString);
  void signal_tx( );
  void signal_rx();
 // void signal_stream(bool);
  void signal_disconnected();
  void signal_connected();
  //void signal_captureData(QByteArray);
};

extern SerPort *gp_SerPort;

#endif // SERPORT_H
