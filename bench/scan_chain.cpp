
#include "scan_chain.h"
#include "ui_scan_chain.h"
#include <QInputDialog>
#include "project_app.h"

Scanchain *gp_Scanchain;


/****************************************************************************
 *                              REGISTERS
 ****************************************************************************/
#include "register.h"
#include "mainapp.h"


static Register g_ScanChain(0,"SCANCHAIN");

/* register map - configurations from XlS*/
static RegisterMap g_ScanChainConfig;

/* register for remember prev state*/
static Register REM;

static Variable CONFIG("@variable=CONFIG @descr=current configuration");

static QString m_last_xls_file;

static int m_scanchains =0;

/****************************************************************************
 *                              APPLICATION
 ****************************************************************************/
Scanchain::Scanchain(QWidget *parent) :
    Applet(parent)
{
    ui =  new Ui::Scanchain;
    ui->setupUi(this);
    m_scanchains =0;
    setWindowTitle("Scanchain");
}

void Scanchain::setupCustomUi()
{

    switch(m_scanchains)
    {
    case 5:
        ui->suScanchain4->setLayoutType(CustomControls::SuperBox::SHORT);
    case 4:
        ui->suScanchain3->setLayoutType(CustomControls::SuperBox::SHORT);
    case 3:
        ui->suScanchain2->setLayoutType(CustomControls::SuperBox::SHORT);
    case 2:
        ui->suScanchain1->setLayoutType(CustomControls::SuperBox::SHORT);
    case 1:
        ui->suScanchain0->setLayoutType(CustomControls::SuperBox::SHORT);
    default:
        break;
    }

    connect(this->ui->rdLayoutModeFull,SIGNAL(clicked()),this,SLOT(slot_LayoutMode_toggled()));
    connect(this->ui->rdLayoutModeBrief,SIGNAL(clicked()),this,SLOT(slot_LayoutMode_toggled()));
    connect(this->ui->rdLayoutModeCompact,SIGNAL(clicked()),this,SLOT(slot_LayoutMode_toggled()));
    connect(ui->leHighlightText,SIGNAL(textChanged(QString)),this,SLOT(slot_highlight(QString)));
}

void Scanchain::updateStatistics()
{        
    for(int i=0;i<m_scanchains;i++)
    {
        ui->twScanchains->setTabText(i,QString("%1:%2").arg(i).arg(g_ScanChain.sub("num",QString::number(i))->size()));
    }
}

void Scanchain::inflateWithBits(const int num)
{
    Bit *pbit;
    CustomControls::SuperBox *psuperbox = box(num);
    for(int i=0;i<g_ScanChain.size();i++)
    {
        pbit = g_ScanChain.bitAt(i);


        int sub = pbit->extra("sub").toInt();
        const QString bitname = QString("SCANCHAIN[%1]").arg(i);
        const QString color = pbit->extra("color");

        QString caption ;

        if(g_ScanChain.sub(pbit->name())->size()>1 && pbit->name() != "_")
        {
            Register *preg = g_ScanChain.sub(pbit->name());
            int pos=0;
            for(;pos<preg->size();pos++)  {
                if(preg->bitAt(pos) == pbit) break;
            }
            caption = QString("%1[%2]").arg(pbit->name()).arg(pos);
        }
        else caption = pbit->name();



        QString descr = "";




        if(pbit->extra("num").toInt() == num)
        {

            if(ui->cmFilter->currentText()== "ALL"){
                psuperbox->addBit(sub, bitname,caption,color,descr );
            }
            else if(pbit->extra("type") == ui->cmFilter->currentText())
            {
                if(pbit->extra("type") == "ANALOG")
                {
                    psuperbox->addBit(sub, bitname,caption,color,descr );
                }
                else if(pbit->extra("type") == "DIGITAL")
                {
                    psuperbox->addBit(sub,bitname,pbit->description(),color,descr);
                }
            }
        }
    }
}

SuperBox *Scanchain::box(int num)
{
    CustomControls::SuperBox *psuperbox;
    switch(num)
    {
    case 4:
        psuperbox = ui->suScanchain4;
        break;

    case 3:
        psuperbox = ui->suScanchain3;
        break;

    case 2:
        psuperbox = ui->suScanchain2;
        break;

    case 1:
        psuperbox = ui->suScanchain1;
        break;

    case 0:
        psuperbox = ui->suScanchain0;
        break;

    }
    return psuperbox;
}

bool Scanchain::initialize()
{
    setupCustomUi();
    addDataNode(&g_ScanChain);
    addDataNode(&CONFIG);    
    return true;
}

void Scanchain::rebuild()
{    
    unbindNode("SCANCHAIN");
    switch(m_scanchains)
    {
    case 5:
        ui->suScanchain4->clear();
        inflateWithBits(4);
        ui->suScanchain4->reconstruct();
    case 4:
        ui->suScanchain3->clear();
        inflateWithBits(3);
        ui->suScanchain3->reconstruct();
    case 3:
        ui->suScanchain2->clear();
        inflateWithBits(2);
        ui->suScanchain2->reconstruct();
    case 2:
        ui->suScanchain1->clear();
        inflateWithBits(1);
        ui->suScanchain1->reconstruct();
    case 1:
        ui->suScanchain0->clear();
        inflateWithBits(0);
        ui->suScanchain0->reconstruct();
    default:
        break;
    }

    updateStatistics();
    bindCustomControls();
    slot_updateAllControls();
}

bool Scanchain::setConfig(const QString &name)
{
    if( g_ScanChainConfig.contains(name) )
    {
        g_ScanChain = g_ScanChainConfig[name];
        slot_updateAllControls();
        return true;
    }
    return true;
}

bool Scanchain::addConfig(const QString &name, Register *preg)
{
    bool result =false;
    if(!name.isEmpty())
    {
        g_ScanChainConfig[name] = *preg;
        if(ui->cmCONFIG->findText(name) == -1)
            ui->cmCONFIG->addItem(name);
        result=true;
    }
    return result;
}

bool Scanchain::containsConfig(const QString &name)
{
    return g_ScanChainConfig.contains(name);
}


QString Scanchain::currentConfig() const
{
    return ui->cmCONFIG->currentText();
}

Register *Scanchain::scanchain(int num) const
{
    if(num == -1)
    {
        return &g_ScanChain;
    }
    else {
        return g_ScanChain.sub("num",QString::number(num));
    }
}

qint32 Scanchain::size() const
{
    return g_ScanChain.size();
}

void Scanchain::setModesList(const QStringList &modes_list_before,const QStringList &modes_list_after)
{
    ui->cmTestModeBefore->clear();
    ui->cmTestModeBefore->insertItems(0,modes_list_before);
    ui->cmTestModeAfter->clear();
    ui->cmTestModeAfter->insertItems(0,modes_list_after);
}

void Scanchain::setScanchain(Register *preg)
{
    ui->cmCONFIG->clear();
    g_ScanChain.clear();
    g_ScanChain = *preg;
    for(int i=0;i<g_ScanChain.size();i++){
        int num = g_ScanChain.bitAt(i)->extra("num").toInt();
        if((num+1)>m_scanchains) m_scanchains = num+1;
    }
}

const QByteArray Scanchain::saveAppData()
{
    QByteArray result;
    result += "BITS:\n";
    for(qint32 i =0;i<g_ScanChain.size();i++)
    {
        Bit *pbit = g_ScanChain.bitAt(i);
        result +=
                /*0*/ pbit->name()+';'
                /*1*/+pbit->description()+';'
                /*2*/+pbit->extra("type")+';'
                /*3*/+pbit->extra("num")+';'
                /*4*/+pbit->extra("sub")+';'
                /*5*/+QByteArray::number(pbit->value())+';'
                /*6*/+pbit->extra("color")+'\n';
    }
    result += "CFGS:\n";
    foreach(const QString &cfgname, g_ScanChainConfig.keys())
    {
        result += cfgname+';'+g_ScanChainConfig[cfgname].toBitString()+'\n';
    }
    result+="MODES1:\n";
    for(int i=0;i<ui->cmTestModeBefore->count();i++)
    {
        result+=ui->cmTestModeBefore->itemText(i)+"\n";
    }
    result+="MODES2:\n";
    for(int i=0;i<ui->cmTestModeAfter->count();i++)
    {
        result+=ui->cmTestModeAfter->itemText(i)+"\n";
    }
    return result;
}

bool Scanchain::loadAppData(const QByteArray &data)
{
    QList<QByteArray> l_lines = data.split('\n');
    qint32 part=0;
    g_ScanChain.clear();
    ui->cmTestModeBefore->clear();
    ui->cmTestModeAfter->clear();
    foreach(const QByteArray &line, l_lines)
    {
        if(line == "CFGS:") part =1;
        else if(line == "BITS:") part =0;
        else if(line == "MODES1:") part =2;
        else if(line == "MODES2:") part =3;
        else
        {
            switch(part)
            {
            case 0:
            {
                QList<QByteArray> l_entries = line.split(';');
                if(l_entries.size() == 7)
                {
                    Bit *pbit = new Bit;
                    /*0*/pbit->setName(l_entries.at(0));
                    /*1*/pbit->setDescription(l_entries.at(1));
                    /*2*/pbit->setExtra("type",l_entries.at(2));
                    int num = l_entries.at(3).toInt();
                    if((num+1)>m_scanchains) m_scanchains = num+1;
                    /*3*/pbit->setExtra("num",l_entries.at(3));
                    /*4*/pbit->setExtra("sub",l_entries.at(4));
                    /*5*/pbit->setValue(l_entries.at(5).toInt()!=0);
                    /*6*/pbit->setExtra("color",l_entries.at(6));
                    g_ScanChain.addBit(pbit);
                }
            }
                break;

            case 1:
            {
                Register config = g_ScanChain;
                QList<QByteArray> l_entries = line.split(';');
                if(l_entries.size() == 2)
                {
                    config.fromBitString(l_entries.at(1));
                    addConfig(l_entries.at(0),&config);
                }
            }
                break;
            case 2:
                if(line.trimmed() != "")
                    ui->cmTestModeBefore->addItem(line);
                break;

            case 3:
                if(line.trimmed() != "")
                    ui->cmTestModeAfter->addItem(line);
                break;
            }
        }
    }

    rebuild();
    resume();
    return true;
}

bool Scanchain::Write()
{
    bool result;
    if(ui->cbAutoconnectPins->isChecked()){
        BOARD->SaveState();
    }
    result = BOARD->SetModeByName(ui->cmTestModeBefore->currentText());
    result &= BOARD->WriteScanchain(ui->twScanchains->currentIndex());
    if(ui->cbAutoconnectPins->isChecked()){
        LOG(0,"restore pins");
        BOARD->RestoreState();
    }
    return result;
}

bool Scanchain::Read()
{
    bool result;
    if(ui->cbAutoconnectPins->isChecked()){
        BOARD->SaveState();
    }
    result = BOARD->SetModeByName(ui->cmTestModeBefore->currentText());
    result &= BOARD->ReadScanchain(ui->twScanchains->currentIndex());
    slot_updateAllControls();
    if(ui->cbAutoconnectPins->isChecked()){
        LOG(0,"restore pins");
        BOARD->RestoreState();
    }
    return result;
}


QVariant Scanchain::FUNCTION_LIST()
{
    FUNCTION("SetConfig","applies scanchain with name-parameter"
             ,""
             ,"true/false")
    {
        bool result= false;
        if(!p1.isNull())
            result= setConfig(p1.toString());
        return result;
    }

    FUNCTION("SetModeAfter","As parameter use mode name as text","","")
    {
        return SetModeAfter(p1.toString());
    }

    FUNCTION("SetModeBefore","As parameter use mode name as text","","")
    {
        return SetModeBefore(p1.toString());
    }

    FUNCTION("Write","Writes scanchain in current(usual DIGITAL) mode"
             ,""
             ,"true/false")
    {
        return  Write();
    }

    FUNCTION("Read","Reads scanchain in current(usual DIGITAL)"
             ,""
             ,"true/false")
    {
        return Read();
    }

    FUNCTION("AnalogWrite","Writes scanchain in current(usual DIGITAL) and then goes to ANALOG mode."
             ,""
             ,"true/false")
    {
        bool result;
        if(ui->cbAutoconnectPins->isChecked()){
            BOARD->SaveState();
        }
        result = BOARD->SetModeByName(ui->cmTestModeBefore->currentText());
        result &= BOARD->WriteScanchain(ui->twScanchains->currentIndex());
        result &= BOARD->SetModeByName(ui->cmTestModeAfter->currentText());
        if(ui->cbAutoconnectPins->isChecked()){
            LOG(0,"restore pins");
            BOARD->RestoreState();
        }
        return result;
    }
    FUNCTION("AnalogRead","Reads scanchain in current(usual DIGITAL) and then goes to ANALOG mode"
             ,""
             ,"true/false")
    {
        bool result;
        if(ui->cbAutoconnectPins->isChecked()){
            BOARD->SaveState();
        }
        result = BOARD->SetModeByName(ui->cmTestModeBefore->currentText());
        result &= BOARD->ReadScanchain(ui->twScanchains->currentIndex());
        result &= BOARD->SetModeByName(ui->cmTestModeAfter->currentText());
        slot_updateAllControls();
        if(ui->cbAutoconnectPins->isChecked()){
            LOG(0,"restore pins");
            BOARD->RestoreState();
        }
        return result;
    }

    return false;
}

void Scanchain::resume()
{
    setEnabled(SCANCHAIN->size()>0);
}








// SLOTS

void Scanchain::on_pbMs_clicked()
{
    REM = g_ScanChain;
}

void Scanchain::on_pbMr_clicked()
{
    if( REM.size() == g_ScanChain.size())
    {
        g_ScanChain = REM;
    }
    slot_updateAllControls();
}

void Scanchain::slot_LayoutMode_toggled()
{
    for(int i=0;i<m_scanchains;i++)
    {
        if(ui->rdLayoutModeFull->isChecked())
        {
            box(i)->setLayoutType(CustomControls::SuperBox::LARGE);
        }
        else if(ui->rdLayoutModeCompact->isChecked())
        {
            box(i)->setLayoutType(CustomControls::SuperBox::SHORT);
        }
        else if(ui->rdLayoutModeBrief->isChecked())
        {
            box(i)->setLayoutType(CustomControls::SuperBox::BRIEF);
        }
        box(i)->reconstruct();
    }
}

void Scanchain::on_cmFilter_currentIndexChanged(int )
{
    rebuild();
}





bool Scanchain::SetModeBefore(const QString &mode_name)
{
    int index = ui->cmTestModeBefore->findText(mode_name);
    if(index>=0){
        ui->cmTestModeBefore->setCurrentIndex(index);
        return true;
    }
    return false;
}

bool Scanchain::SetModeAfter(const QString &mode_name)
{
    int index = ui->cmTestModeAfter->findText(mode_name);
    if(index>=0){
        ui->cmTestModeAfter->setCurrentIndex(index);
        return true;
    }
    return false;
}

void Scanchain::setCurrentFile(const QString &current_file_name)
{
    m_last_xls_file = current_file_name;
    setWindowTitle(m_last_xls_file);
}


void Scanchain::slot_highlight(const QString &text)
{
    for(int i=0;i<m_scanchains;i++)
        box(i)->clearHighlighiting();

    if(!text.isEmpty())
    {
        for(int i=0;i<m_scanchains;i++)
        {
            box(i)->highlight(SuperBox::BY_TEXT,text);
        }
    }
}

void Scanchain::on_pbFill0_clicked()
{
    g_ScanChain.fill(0);
}

void Scanchain::on_pbXor_clicked()
{

}

void Scanchain::on_pbXor_toggled(bool checked)
{
    if(checked){
        if(REM.size() == g_ScanChain.size())
        {
            for(int j=0;j<g_ScanChain.size();j++)
            {
                if(g_ScanChain.bitAt(j)->value() != REM.bitAt(j)->value()) {

                    for(int i=0;i<m_scanchains;i++)
                    {
                        box(i)->highlight(SuperBox::BY_ACCESSIBLE_NAME,QString("SCANCHAIN[%1]").arg(j));
                    }
                }
            }
        }
    }
    else {
        for(int i=0;i<m_scanchains;i++)
            box(i)->clearHighlighiting();
    }
}


void Scanchain::on_cmCONFIG_activated(const QString &arg1)
{
    setConfig(arg1);
}

void Scanchain::on_pbSaveBack_clicked()
{
    MAINAPP->saveBackToExcel(SCANCHAIN->name());
}

