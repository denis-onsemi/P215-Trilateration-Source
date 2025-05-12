#include "customcontrols_RegisterBox.h"
#include <QWidget>
#include <QGroupBox>
#include <QScrollBar>
#include <QInputDialog>



namespace CustomControls
{


RegisterBox::RegisterBox(QWidget *parent): QScrollArea(parent)
{

    m_group_name = "bank";
    m_opt=0;
    mp_foundControl=0;
}

RegisterBox::~RegisterBox()
{
    clear();
}




void RegisterBox::addRegister(Register &reg)
{    
    static const char *COLORS[3] = {"rgba(202, 202, 202, 162)"
                                    , "rgba(255, 202, 156, 162)"
                                    ,"rgba(142, 203, 255, 162)"
                                   };


    clear();
    int bank_n=0;
    //widget()->setFixedWidth(width());
    if(!widget()->layout()) {
        widget()->setLayout(new QVBoxLayout(this));
        widget()->layout()->setContentsMargins(0,0,0,0);
    }

    // find the longest name text
    int max_size = 0;
    foreach(const QString &item , reg.toString("@name;").split(";",QString::SkipEmptyParts))
    {
        if(item.count()>max_size)    max_size =item.count();
    }

    QStringList bank_list_by_order ;

    //make list of banks
    for(int i=0;i<reg.size();i++)
    {
        QString bank = reg.bitAt(i)->extra(m_group_name);
        if(!m_banks.contains(bank))
        {
            m_banks[bank] = 0;
            bank_list_by_order.append(bank);
        }
    }

    int col=0;
    int row =0;
    foreach(const QString &bank , bank_list_by_order)
    {
        int type = RegisterBoxControl::Bit;
        int v=0;

        QGroupBox *pwdg = m_banks[bank];
        Register *pcur_bank = reg.sub(m_group_name, bank);
        // add new groupbox
        if( pwdg == 0)
        {

            pwdg = new  QGroupBox(widget());
            m_banks[bank] = pwdg;
            widget()->layout()->addWidget(pwdg);
            pwdg->setProperty(m_group_name.toLatin1().constData(),bank);
            pwdg->setLayout(new QGridLayout());
            pwdg->layout()->setContentsMargins(10,5,10,5);

            // header
            if(m_opt & ShowHeader){
                pwdg->setTitle(QString("%1 (%2)").arg(m_group_name.toUpper()).arg(bank));
                pwdg->layout()->setContentsMargins(0,15,0,5);
            }
            // opt
            QString color;
            if( pcur_bank->size()>0 ) color = pcur_bank->bitAt(0)->extra( "color" );

            QString all_color = reg.extra("color");
            if(color.isEmpty())
            {
                if(m_opt & BankZebra )
                {
                    int ci =0;
                    ci =  bank_n % 2+1;
                    color = COLORS[ci];
                }
                else
                {
                    if(all_color.isEmpty())
                        color = COLORS[0];
                    else color = all_color;
                }
            }

            if(m_opt & EnableChecked){
                pwdg->setCheckable(true);
            }else pwdg->setCheckable(false);

            pwdg->setStyleSheet(QString("QGroupBox{"
                                        "font-weight: bold; "
                                        "color: grey; "
                                        "background-color: qlineargradient(spread:reflect, x1:0.432, y1:0, x2:0.432, y2:0.466318, stop:0.1875 %1, stop:1 rgba(255, 255, 255, 255));"
                                        "border: 2px solid %2; "
                                        "border-radius:5px;}").arg(color).arg(color));


            col =0;
            row=0;
            bank_n++;
        }


        QStringList items_in_bank;
        QStringList l = pcur_bank->toString("@purename;").split(";",QString::SkipEmptyParts);

        if(m_opt & BackwardList)
        {
            for(int i=l.size()-1;i != 0;i--)
            {
                items_in_bank.append(l[i]);
            }
        }else{
            for(int i = 0;i < l.size();i++)
            {
                items_in_bank.append(l[i]);
            }

        }

        foreach(const QString &item , items_in_bank)
        {
            int from= 0;
            int to;
            int offset=0;
            int item_size_in_bank = reg.sub(m_group_name,bank)->sub(item)->size();
            int item_size_all=0;
            int bank_size = reg.sub(m_group_name,bank)->size();

            QString caption = item;
            QString access;

            // get value of current item
            v= reg.sub(m_group_name,bank)->sub(item)->toUInt();

            //
            Bit *pbit0 =0;
            if(item_size_in_bank>0){
                 pbit0 = reg.sub(m_group_name,bank)->sub(item)->bitAt(0);
                from = reg.sub(item)->findBit(pbit0);
            }
            item_size_all = reg.sub(item)->size();
            to = item_size_in_bank-1+from;

            offset = reg.findBit(pbit0);

            if(item_size_in_bank == 1)
            {
                type = RegisterBoxControl::Bit;
                if(m_opt & AbsoluteOffset)
                    caption = QString("%1[%2]").arg(item).arg(offset);
                else
                    caption = QString("%1").arg(item);

                access = QString("%1[%2]").arg(reg.name()).arg(item);
            }
            // if size if more than 1
            else
            {
                type = RegisterBoxControl::Field;
                if(item_size_all != item_size_in_bank){
                    access = QString("%1[%2:%3]").arg(reg.name()).arg(offset).arg(offset+item_size_in_bank-1);
                }
                else{
                    access = QString("%1[%2]").arg(reg.name()).arg(item);
                }

                if(m_opt & AbsoluteOffset)
                    caption = QString("%1[%3:%2]").arg(item).arg(offset).arg(offset+item_size_in_bank-1);
                else caption = QString("%1[%3:%2]").arg(item).arg(from).arg(to);
            }

            //create control
            RegisterBoxControl *pctrl = new RegisterBoxControl(this, type,max_size);
            pctrl->setCaption(caption);
            pctrl->setValue(v);
            pctrl->setAccessibleName(access);

            if(col >= m_cols) {            row++; col=0;            }
            QGridLayout *pgrid = (static_cast<QGridLayout*>(pwdg->layout()));
            pgrid->addWidget(pctrl, row, col,1,1);
            col++;
            pwdg->setMinimumHeight(pgrid->rowCount()*25 +20);
            pwdg->adjustSize();
            widget()->adjustSize();

        }


    }
}

void RegisterBox::addRegistersList(QList<Register*> regs, BuddyFunc buddyFunc)
{
    static const char *COLORS[3] = {"rgba(202, 202, 202, 162)"
                                    , "rgba(255, 202, 156, 162)"
                                    ,"rgba(142, 203, 255, 162)"
                                   };


    clear();
    int bank_n=0;
    //widget()->setFixedWidth(width());
    if(widget()==0) {
        setWidget(new QWidget(this));
        this->setWidgetResizable(1);
        //widget()->setFixedWidth(this->width());
    }
    if( !widget()->layout()) {
        widget()->setLayout(new QVBoxLayout(this));
        widget()->layout()->setContentsMargins(0,0,0,0);
    }
    // find the longest name text
    int max_size = 0;

    foreach(Register *r, regs)
    {
        foreach(const QString &item , r->toString("@name;").split(";",QString::SkipEmptyParts))
        {
            if(item.count()>max_size)    max_size =item.count();
        }
        m_banks[r->name()] = 0;
    }    

    int col=0;
    int row =0;
    foreach( Register *r, regs)
    {
        int type = RegisterBoxControl::Bit;
        int v=0;

        QGroupBox *pwdg = m_banks[r->name()];
        // add new groupbox
        if( pwdg == 0)
        {

            pwdg = new  QGroupBox(widget());
            m_groupBoxList.append(pwdg);
            m_banks[r->name()] = pwdg;
            widget()->layout()->addWidget(pwdg);
            pwdg->setProperty(m_group_name.toLatin1().constData(),r->extra(m_group_name));            
            pwdg->setLayout(new QGridLayout());
            pwdg->layout()->setContentsMargins(10,5,10,5);

            // header
            if(m_opt & ShowHeader){
                pwdg->setTitle(QString("%1 (%2) %3").arg(r->extra(m_group_name)).arg(r->name())
                               // add description
                               .arg(r->extras().contains("caption")?("- "+r->extra("caption")):""));
                pwdg->layout()->setContentsMargins(0,15,0,5);
            }
            // opt
            QString color;
            if( r->size()>0 ) color = r->extra( "color" );

             if(color.isEmpty())
            {
                if(m_opt & BankZebra )
                {
                    int ci =0;
                    ci =  bank_n % 2+1;
                    color = COLORS[ci];
                }
            }

            if(m_opt & EnableChecked){
                pwdg->setCheckable(true);
            }
            else pwdg->setCheckable(false);

            pwdg->setStyleSheet(QString("QGroupBox{"
                                        "font-weight: bold; "
                                        "color: grey; "
                                        "background-color: qlineargradient(spread:reflect, x1:0.432, y1:0, x2:0.432, y2:0.466318, stop:0.1875 %1, stop:1 rgba(255, 255, 255, 255));"
                                        "border: 2px solid %2; "
                                        "border-radius:5px;}").arg(color).arg(color));


            col =0;
            row=0;
            bank_n++;
        }


        QStringList items_in_bank;
        QStringList l = r->toString("@purename;").split(";",QString::SkipEmptyParts);

        if(m_opt & BackwardList)
        {
            for(int i=l.size()-1;i != 0;i--)
            {
                items_in_bank.append(l[i]);
            }
        }else{
            for(int i = 0;i < l.size();i++)
            {
                items_in_bank.append(l[i]);
            }

        }

        int bank_size = r->size();
        foreach(const QString &item , items_in_bank)
        {
            if(m_opt & IgnoreUnderlines && item.startsWith("_")) continue;
            int from= 0;
            int to;
            int offset=0;
            int item_size_in_bank = r->sub(item)->size();
            int item_size_all=0;


            QString caption = item;
            QString access;
            QString buddyName;

            // get value of current item
            v= r->sub(item)->toUInt();

            //
            Bit *pbit0 =0;
            QString itemStyle;
            if(item_size_in_bank>0){
                 pbit0 = r->sub(item)->bitAt(0);                 
                 itemStyle = pbit0->extra("style");
                 buddyName = pbit0->extra("buddy");
                from = r->sub(item)->findBit(pbit0);
            }
            item_size_all = r->sub(item)->size();
            to = item_size_in_bank-1+from;

            offset = r->findBit(pbit0);


            if(item_size_in_bank == 1)
            {
                type = RegisterBoxControl::Bit;
                if(m_opt & AbsoluteOffset)
                    caption = QString("%1[%2]").arg(item).arg(offset);
                else
                    caption = QString("%1").arg(item);

                access = QString("%1[%2]").arg(r->name()).arg(item);
            }
            // if size if more than 1
            else
            {
                type = RegisterBoxControl::Field;
                if(item_size_all != item_size_in_bank){
                    access = QString("%1[%2:%3]").arg(r->name()).arg(offset).arg(offset+item_size_in_bank-1);
                }
                else{
                    access = QString("%1[%2]").arg(r->name()).arg(item);
                }

                if(m_opt & AbsoluteOffset)
                    caption = QString("%1[%3:%2]").arg(item).arg(offset).arg(offset+item_size_in_bank-1);
                else caption = QString("%1[%3:%2]").arg(item).arg(from).arg(to);
            }

            //create control
            RegisterBoxControl *pctrl = new RegisterBoxControl(this, type,max_size);
            pctrl->setCaption(caption);
            pctrl->setValue(v);
            pctrl->setAccessibleName(access);
            pctrl->setStyleSheet(itemStyle);            

            if(!buddyName.isEmpty())
            {
                pctrl->setBuddy(buddyName,buddyFunc);
                connect(r,SIGNAL(changed()),pctrl,SLOT(updateBuddy()));
            }

            if(col >= m_cols)
            {
                row++;
                col=0;
            }
            QGridLayout *pgrid = (static_cast<QGridLayout*>(pwdg->layout()));
            pgrid->addWidget(pctrl, row, col,1,1);
            if(row==0 && (m_opt &AddSpacer) ){
                    QGridLayout *pgrid = (static_cast<QGridLayout*>(pwdg->layout()));
                    pgrid->addItem(new QSpacerItem(1000,0, QSizePolicy::Expanding, QSizePolicy::Minimum),0,col,1,1);
            }
            col++;
            pwdg->setMinimumHeight(pgrid->rowCount()*25 +20);
            pwdg->adjustSize();           
        }
    }    
}


void RegisterBox::clear()
{    
    foreach(const QString &bank, m_banks.keys()) delete m_banks[bank];
    m_banks.clear();
}

RegisterBoxControl::RegisterBoxControl(QWidget *p, int type,int max_width)
    :QWidget(p)

{
    mp_caption =0;
    mp_le =0;
    mp_cb=0;
    mp_buddy =0;

    m_max_symbols = max_width;
    this->setLayout(new QHBoxLayout());
    layout()->setContentsMargins(5,0,5,0);
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    m_type = type;

    switch(type)
    {
    case Bit:
        mp_caption =  new QLabel(this);        
        layout()->addWidget(mp_caption);
        mp_cb =  new CheckBox(this);
        mp_cb->setFixedWidth(17);
        layout()->addWidget(mp_cb);
        break;

    case Field:
        mp_caption =  new QLabel(this);

        layout()->addWidget(mp_caption);
        mp_le = new LineEdit(this);
        mp_le->setFixedWidth(80);
        layout()->addWidget(mp_le);
        break;

    case Dict:
        break;

    default:
        break;
    }    
}

void RegisterBoxControl::setStyleSheet(const QString &styleSheet)
{
        if(mp_caption) mp_caption->setStyleSheet(styleSheet);
}

void  RegisterBoxControl::setAccessibleName(const QString &name)
{
    switch(m_type)
    {
    case Bit:
        mp_cb->setAccessibleName(name);
        break;
    case Field:
        mp_le->setAccessibleName(name);
        break;
    }
}

QString RegisterBoxControl::accessibleName()
{
    switch(m_type)
    {
    case Bit:
        if(mp_cb)
            return mp_cb->accessibleName();
        break;
    case Field:
        if(mp_le)
            return mp_le->accessibleName();
        break;
    }
    return QString();
}

void RegisterBoxControl::setCaption(const QString &caption)
{
    mp_caption->setText(caption);

}

void RegisterBoxControl::setValue(int v)
{
    switch(m_type)
    {
    case Bit:
        mp_cb->setChecked(v);
        break;
    case Field:
        mp_le->setEditValue(v);
        break;
    }
}

int RegisterBoxControl::width()
{
    int w=0;
    if(mp_cb) w+= mp_cb->width();
    if(mp_le) w+= mp_le->width();
    if(mp_caption) w+=mp_caption->width();
    return w;
}

void RegisterBoxControl::setBuddy(const QString &buddyName,BuddyFunc buddyFunc)
{
    if(buddyFunc!=0 && mp_buddy ==0)
    {
        mp_BuddyFunc = buddyFunc;
        m_buddyName = buddyName;
        mp_buddy = new QLabel(this);
        layout()->addWidget(mp_buddy);
    }
}

void RegisterBoxControl::updateBuddy()
{
    switch(m_type)
    {
        case Bit:
            if(mp_cb){
            const QString txt = (mp_BuddyFunc)(m_buddyName,mp_cb->accessibleName());
                mp_buddy->setText(txt);
            }

        break;

        case Field:
        if(mp_le)
        {
            const QString txt = (mp_BuddyFunc)(m_buddyName,mp_le->accessibleName());
            mp_buddy->setText(txt);
        }
        break;

    }
}





    void CustomControls::RegisterBox::setOptions(unsigned int opt)
    {
        m_opt = opt;
    }

    QList<QString> CustomControls::RegisterBox::checkedBanks()
    {
        QList<QString> l;
        foreach(const QString &bank, m_banks.keys()){
            if(m_banks[bank]->isChecked()){
                l.append(bank);
            }
        }
        return l;
    }

    void CustomControls::RegisterBox::setChecked(QString bank, bool on)
    {
        if(m_banks.contains(bank) && m_banks[bank])
            m_banks[bank]->setChecked(on);
    }

    QList<QGroupBox *>RegisterBox::groupBoxes()
    {
        return m_groupBoxList;
    }

    QGroupBox* RegisterBox::groupBox(int n)
    {
        return m_groupBoxList[n];
    }

    QGroupBox *RegisterBox::groupBox(const QString &name)
    {
        return m_banks[name];
    }

    void RegisterBox::resizeEvent(QResizeEvent *)
    {
        widget()->setFixedWidth(size().width()-verticalScrollBar()->width()-5);
    }

    void RegisterBox::keyPressEvent(QKeyEvent *pev)
    {

        if(pev->modifiers() & Qt::ControlModifier && pev->key()== Qt::Key_F){
                bool selected=false;

                m_foundText = QInputDialog::getText(0,"found","",QLineEdit::Normal,m_foundText,&selected);

                for(int i=0;i<m_groupBoxList.count();i++ )
                {
                    QGroupBox *pgrbx = m_groupBoxList[i];
                    QObjectList l = pgrbx->children();
                    foreach( QObject *pobj,l)
                    {
                        if(QString(pobj->metaObject()->className()) == "CustomControls::RegisterBoxControl")
                        {
                            CustomControls::RegisterBoxControl *pctrl =0;
                            pctrl = static_cast<CustomControls::RegisterBoxControl*>(pobj);
                            QRegExp rx(QString(".*\\[%1.*\\]").arg(m_foundText));
                            if(selected && !m_foundText.isEmpty() && rx.exactMatch(pctrl->accessibleName())){
                                pctrl->setStyleSheet("background-color:red");
                                ensureWidgetVisible(pgrbx);
                            }else{
                                pctrl->setStyleSheet("");
                            }
                        }
                    }
                }
        }


        QScrollArea::keyPressEvent(pev);
    }



}

void CustomControls::RegisterBox::setWidth(int w)
{
    widget()->setFixedWidth(w);
}

void CustomControls::RegisterBox::setHeight(int h)
{
    widget()->setFixedHeight(h);
}
