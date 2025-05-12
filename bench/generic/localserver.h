/**
 * @file:localconnector.h
 * @description:
 *    This is named pipe connector for controlling app.
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-50-31
 *
 */

#ifndef LOCALCONNECTOR_H
#define LOCALCONNECTOR_H
#include <QLocalServer>

class LocalServer: public QObject
{
  Q_OBJECT

public:
    static void start(){LocalServer::instance();}
  static   LocalServer *instance();
  bool initialize(const QString &pipename);
  const QString & pipeName() const {return m_file_pipe_name;}
  bool restore();
  void shutdown();
  QString genName(const QString &pipename);

private:
  LocalServer();


  /*pointer to local server*/
  QLocalServer *mp_server;
  /* pipe name with full path*/
  QString m_file_pipe_name;
  /* input buffer */
  QByteArray input;
  /* output buffer */
  QByteArray output;
  /* process input .reads from input buffer*/
  QByteArray parse(const QByteArray &data_in);

signals:
  void log(int, const QString &text);

private slots:
  /* read data and execute set/get/run */
  void processConnection();


};

#define LOCALSERVER LocalServer::instance()

#endif // LOCALCONNECTOR_H
