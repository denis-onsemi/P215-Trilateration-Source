#ifndef CUSTOMCONTROLS_REGISTERBOX_H
#define CUSTOMCONTROLS_REGISTERBOX_H


#include "customcontrols.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <register.h>
#include <QScrollArea>
#include <QLabel>
#include <QWidget>
#include <QGroupBox>

namespace CustomControls
{


//enum {
#define   ShowHeader  1
#define   BankZebra 2
#define   AbsoluteOffset 4
#define   EnableChecked  8
#define   BackwardList  0x10
#define   IgnoreUnderlines 0x20
#define     AddSpacer 0x40

//}RegisterBoxOptions;
typedef QString (*BuddyFunc)(const QString&,const QString &);
//typedef ;
/****************************************************************************
 * @class :
 * RegisterBox
 ****************************************************************************/
class RegisterBoxControl:public QWidget
{
Q_OBJECT
public:

    enum {Bit,Field,Dict};
    RegisterBoxControl(QWidget *p, int type, int max_width);
    virtual ~RegisterBoxControl(){}

    void setStyleSheet(const QString &styleSheet);

    void setAccessibleName(const QString &name);
    QString accessibleName();
    void setCaption(const QString &caption);
    void setValue(int v);
    int width();

    void setBuddy(const QString &buddyName, BuddyFunc buddyFunc);
protected:
    void contextMenuEvent(QContextMenuEvent *){}

private:
    BuddyFunc  mp_BuddyFunc;
    int m_max_symbols;
    int m_type;
    QString m_buddyName;
    QString m_background_color;
    CheckBox    *mp_cb;
    QLabel      *mp_caption;
    LineEdit    *mp_le;
    QLabel      *mp_buddy;
private slots:
    void updateBuddy();
};


class RegisterBox : public QScrollArea
{

public:

    void setWidth(int w);
    void setHeight(int h);


    void setColumnsCount(int n){m_cols = n;}
    RegisterBox(QWidget *parent=0);
    ~RegisterBox();

    /* adds register regname pointer preg*/
    void addRegister( Register &reg);
    void addRegistersList(QList<Register*> regs,BuddyFunc buddy =0);
    void clear();

    QStringList items() const;
    RegisterBoxControl* control(const QString &item) const;
    void setGroupName(const QString &name) {m_group_name = name;}
    QString groupName() const {return m_group_name;}


    void setOptions(unsigned int opt);
    QList<QString> checkedBanks();
    int banksCount(){return m_banks.count();}
    void setChecked(QString n,bool on);
    QList<QGroupBox *> groupBoxes();
    QGroupBox *groupBox(int n);
    QGroupBox *groupBox(const QString &name);

protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
private:

    QMap<QString, QGroupBox*> m_banks;
    QList<QGroupBox *> m_groupBoxList;
    unsigned int m_opt;

    QString m_group_name;
    int m_cols;
    CustomControls::RegisterBoxControl *mp_foundControl;
    QString m_foundText;
   // QList<RegisterBox::Control* > m_controls;


};



}

#endif // CUSTOMCONTROLS_RegisterBox_H
