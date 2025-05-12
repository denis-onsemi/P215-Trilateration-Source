#ifndef SETTINGS_H
#define SETTINGS_H

#include "Applet.h"

namespace Ui 
{
class Settings;
}

class Settings : public Applet 
{
    Q_OBJECT
private:
    Ui::Settings *ui;
    void setupCustomUi();



    QMap<QString,QLineEdit *> m_all_settings;

    void loadSettingsFromfile();
    void saveSettingsToFile();
public:
    explicit Settings(QWidget *parent = 0 );
    ~Settings(){}    
    virtual bool initialize();
     virtual QVariant runAppCommand(const QString &name, const QVariant &p1, const QVariant &p2,const QVariant &p3,const QVariant &p4,const QVariant &p5);
    virtual void suspend(){}
    virtual void resume(){}
    virtual bool updateHardware(Direction direction, const QString &item);

    QString setting(const QString &name);
    
    void getHKMC(void);

    int mVersion;
    bool mHKMC;
    QString mMOBDcontroller_header;
    QString mUPIDcontroller_header;
    QString mRDUMcontroller_header;
    QString mMOBDmaster_header;
    QString mUPIDmaster_header;
    QString mRDUMmaster_header;
    
private slots:
    void slot_le_editingFinished();
};

extern Settings *gp_Settings;
#define SETTINGS  gp_Settings

#endif // SETTINGS_H


