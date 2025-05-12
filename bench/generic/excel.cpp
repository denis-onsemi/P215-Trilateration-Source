/**
 * @file:exceldata.cpp   -
 * @description: This is Excel communication class.
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-51-10
 *
 */


#include "excel.h"
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QRgb>
#include "axobject.h"
#include "excelenums.h"







/****************************************************************************
 * @function name: Constructor
 *
 * @param:
 *          const QString filename  - file name *.xlsx
 * @description:
 ****************************************************************************/
Excel::Excel(const QString filename, bool use_thread, bool autosave, bool displayAlerts) :
    QObject(0)
{
    m_autosave = autosave;
    m_opened = false;
    m_saved = false;
    m_filename = filename;
    mp_object = new AxObject( "Excel.Application",use_thread,false);
    if(mp_object->isValid()){
        mp_object->setProperty( "DisplayAlerts", displayAlerts);
        mp_object->setProperty("DisplayStatusBar",0);
        mp_object->setProperty("EnableEvents",0);
    }

}

Excel::~Excel()
{    
    close();
    mp_object->dynamicCall("Workbooks.Close");
    mp_object->method_run(mp_object->id(),"Quit");
    delete mp_object;
}

void Excel::setErrorSlot(QObject *pobj, const char *slot)
{
    mp_object->setErrorSlot(pobj,slot);
}

QString Excel::fileName()
{
    QVariant v;
    if(mp_object->property("Application.ActiveWorkbook.FullName",&v))
        return v.toString();
    return QString();

}


/****************************************************************************
 * @function name: ExcelData::open()
 *
 * @param:
 *
 *       char mode  - file mode 'r' -read 'w'- write
 * @description: opens excel file for read or write
 * @return: ( bool )  success = true
 ****************************************************************************/
bool Excel::open()
{
    bool result = false;
    QVariant var;
    /* check if opened and file name is empty*/
    if (mp_object != NULL && !m_opened ) {

        /* try open workbooks*/

        if (QFile::exists(m_filename) )
        {
            result = mp_object->dynamicCall("Workbooks.Open", &var, m_filename );
            result = mp_object->method_run(var.toInt(),"Activate");
        }
        else
        {
            result = mp_object->dynamicCall("Workbooks.Add",&var);
            mp_object->method_run(var.toInt(),"Activate");
            removeSheet("Sheet1");
        }
        //result = mp_workbook != 0;
        m_opened = result;
    }

    return result;
}

void Excel::release()
{
    mp_object->release();
}

void Excel::clearAbort()
{
    mp_object->clearAbort();
}


/****************************************************************************
 * @function name: ExcelData::isOpen()
 *
 * @param:
 *             void
 * @description: Check if file is opened
 * @return: ( bool ) is opened = true
 ****************************************************************************/
bool Excel::isOpen() const
{
    return m_opened && mp_object;
}

bool Excel::setZoom(int val)
{
    return mp_object->setProperty("ActiveWindow.Zoom",val);
}

int Excel::zoom()
{
    return 0;//mp_object->property("ActiveWindow.Zoom").toIn;
}


/****************************************************************************
 * @function name: ExcelData::addSheet()
 *
 * @param:
 *
 *       const QString sheetname  - sheet name
 * @description: adds sheet to workbook
 * @return: ( bool ) - success = true
 ****************************************************************************/
bool Excel::addSheet(const QString sheetname)
{
    bool result =false;
    if (m_opened)
    {
        mp_object->clearBag();
        do{
            QVariant var;
            if(!mp_object->dynamicCall("ActiveWorkbook.Worksheets.Add",&var))  break;
            if(!mp_object->setProperty(var.toInt(),"Name", sheetname)) break;
            result = true;
        }while(0);
    }
    return result;
}


/****************************************************************************
 * @function name: ExcelData::setCurrentSheet - ---
 * @param:
 *   const QString sheetname  - sheetn name
 * @description: Sets active sheet
 * @return: ( bool ) - success = true
 ****************************************************************************/
bool Excel::setCurrentSheet(const QString sheetname)
{
    bool result = false;
    if (m_opened)
    {
        mp_object->clearBag();
        result = mp_object->dynamicCall(QString("Sheets(\"%1\").Activate").arg(sheetname));
    }
    return result;
}




/****************************************************************************
 * @function name: ExcelData::removeSheet()
 *
 * @param:
 *
    const QString sheetname
 * @description: removes sheet
 * @return: ( bool ) success = true
 ****************************************************************************/
bool Excel::removeSheet(const QString sheetname)
{
    bool result = false;
    if (m_opened)
    {
        mp_object->clearBag();
        result = mp_object->dynamicCall(QString("Sheets(\"%1\").Delete").arg(sheetname));
    }
    return result;
}


/****************************************************************************
 * @function name: ExcelData::save - ---
 * @description:
 * @return: ( bool ) success = true
 ****************************************************************************/
bool Excel::save(void)
{
    bool ok=0;
    mp_object->clearBag();
    if(!m_saved){
        ok = mp_object->dynamicCall("ActiveWorkbook.SaveAs",0, m_filename);
    }
    else ok =  mp_object->dynamicCall("ActiveWorkbook.Save");
    if(ok ) m_saved =1;
    return ok;
}

bool Excel::saveAs(const QString &)
{
    bool ok ;
    ok = mp_object->dynamicCall("ActiveWorkbook.SaveAs",0, m_filename);
    if(ok) m_saved = 1;
    return m_saved;
}


/****************************************************************************
 * @function name: ExcelData::write - ---
 *
 * @param:
 *
 *    qint32 i
 *    qint32 j
 *    const QVariant data
 * @description: writes data to cell i,j
 * @return: ( bool ) success = true
 ****************************************************************************/

bool Excel::write(Excel::Cell cell, const QVariant &data, const QFont &font )
{
    return write(cell.xlRow(),cell.xlCol(),data,font);
}

bool Excel::write(const QString &range, const QStringList &l, const QFont &font)
{
    mp_object->clearBag();
    bool result = false;
    if (m_opened && Range_Is_Valid(range) )
    {        
        result = mp_object->setProperty( QString("ActiveSheet.Range(\"%1\").FormulaArray").arg(range),l);


        if(result && font != QFont())
        {
            mp_object->setProperty( QString("ActiveSheet.Range(\"%1\").Font.Name").arg(range), font.family());
            if(font.pointSize()>0)
                mp_object->setProperty( QString("ActiveSheet.Range(\"%1\").Font.Size").arg(range), font.pointSize());
            mp_object->setProperty( QString("ActiveSheet.Range(\"%1\").Font.Bold").arg(range), font.bold());
            mp_object->setProperty( QString("ActiveSheet.Range(\"%1\").Font.Italic").arg(range), font.italic());
        }
    }
    return result;
}


bool Excel::write(qint32 row, qint32 col, const QVariant &data, const QFont &font)
{
    mp_object->clearBag();
    bool result = false;
    if (m_opened && row > 0 && col > 0 )
    {
        result = mp_object->setProperty( QString("ActiveSheet.Cells(%1,%2).Value").arg(row).arg(col),data);
        if(result && font != QFont())
        {
            mp_object->setProperty( QString("ActiveSheet.Cells(%1,%2).Font.Name").arg(row).arg(col), font.family());
            if(font.pointSize()>0)
                mp_object->setProperty( QString("ActiveSheet.Cells(%1,%2).Font.Size").arg(row).arg(col), font.pointSize());
            mp_object->setProperty( QString("ActiveSheet.Cells(%1,%2).Font.Bold").arg(row).arg(col), font.bold());
            mp_object->setProperty( QString("ActiveSheet.Cells(%1,%2).Font.Italic").arg(row).arg(col), font.italic());
        }
    }
    return result;
}


bool Excel::write(AxObject::Class range, qint32 row, qint32 col, const QVariant &data, const QFont &font)
{
    mp_object->clearBag();
    bool result = false;
    if (m_opened && row > 0 && col > 0 )
    {
        int retry=5;
        while(!result && retry--)
            result = mp_object->setProperty(range, QString("Cells(%1,%2).Value").arg(row).arg(col),data);

        if(result && font != QFont())
        {
            mp_object->setProperty( range,QString("Cells(%1,%2).Font.Name").arg(row).arg(col), font.family());
            if(font.pointSize()>0)
                mp_object->setProperty(range, QString("Cells(%1,%2).Font.Size").arg(row).arg(col), font.pointSize());
            mp_object->setProperty(range, QString("Cells(%1,%2).Font.Bold").arg(row).arg(col), font.bold());
            mp_object->setProperty(range, QString("Cells(%1,%2).Font.Italic").arg(row).arg(col), font.italic());
        }
    }
    return result;
}

bool Excel::cellVisible(int row, int col)
{
    return mp_object->dynamicCall(QString("ActiveSheet.Cells(%1, %2).Select").arg(row).arg(col));
}



Excel::Table  Excel::CreateTable(const Rect &rect, const QStringList &headers, const QStringList &data)
{        
    mp_object->clearBag();
    Table t;
    mp_object->setProperty("ScreenUpdating",0);
    // table range
    t.tableRange = rect;

    QFont font;
    font.setBold(1);
    font.setPointSize(10);
    font.setFamily("Arial");    
    if(m_opened && !rect.isEmpty())
    {
        if(headers.count()>0)
        {
            Excel::Frame f1(Excel::Frame::DrawBottom
                            |Excel::Frame::DrawLeft
                            |Excel::Frame::DrawRight
                            |Excel::Frame::DrawTop
                            );
            f1.setStyle(Excel::Frame::Bottom, Excel::Frame::LineDouble);
            f1.setStyle(Excel::Frame::Top, Excel::Frame::LineContinuous);
            f1.setStyle(Excel::Frame::Left, Excel::Frame::LineContinuous);
            f1.setStyle(Excel::Frame::Right, Excel::Frame::LineContinuous);

            // table header
            Rect th_r = rect;
            th_r.setHeight(1);
            drawFrame(th_r.toRange(), f1);

            // print headers
            write(th_r.toRange(), headers);
            setColor(th_r.toRange(), Qt::darkGray, Qt::white);
        }
        font.setBold(0);
        Excel::Frame f2(Excel::Frame::DrawBottom
                        |Excel::Frame::DrawLeft
                        |Excel::Frame::DrawRight
                        );
        f2.setStyle(Excel::Frame::Bottom, Excel::Frame::LineContinuous);
        f2.setStyle(Excel::Frame::Left, Excel::Frame::LineContinuous);
        f2.setStyle(Excel::Frame::Right, Excel::Frame::LineContinuous);

        // table data range
        Rect td_r = rect;

        if(headers.count()>0){
          f2.setStyle(Excel::Frame::Top, Excel::Frame::LineContinuous);
          td_r.setY(td_r.y()+1);
        }
        t.dataRange = td_r;// text representation of range area
        drawFrame(td_r.toRange(),f2);


        int rows = data.count()/td_r.width();
        if(data.count()>0 && rows==0) rows=1;
        for(int i=0;i<qMin(rows,td_r.height());i++)
        {            
            write(td_r.row(i).toRange(), data.mid(i*td_r.width(),td_r.width()) ,font);
        }
        t.height =  rect.height();
        t.width = rect.width();
        t.rows_count = 0;
        t.font = QFont();
        t.range = mp_object->queryObject(QString("ActiveSheet.Range(\"%1\")").arg(td_r.toRange()));        
    }

    mp_object->setProperty("ScreenUpdating",1);
    return t;
}

bool Excel::AppendRow(Excel::Table &t, const QStringList &data)
{
    mp_object->clearBag();
    if(!data.isEmpty())
    {
        int i=0;
        int row,column;
        foreach(const QString &txt, data)
        {
            row = i/t.width + 1 + t.rows_count;
            column = i%t.width+1;
            if(!write(t.range, row, column, txt ,t.font)) return false;
            i++;
        }
        t.rows_count = row;
        return true;
    }
    return false;
}

bool Excel::SetDataToColumn(Excel::Table &t, const QVariantList &data, int column)
{
    bool result = false;
    mp_object->clearBag();    
    mp_object->setProperty("ScreenUpdating",0);            
    mp_object->setProperty("Calculation",xlCalculationManual);
    if(!data.isEmpty())
    {        
        VARIANT v;     
        AxObject::QVariantList_to_2D_VARIANT(data,1,data.count(),v);                
        if(mp_object->setPropertyVariant(QString("Range(\"%1\").Value").arg(t.dataRange.column(column).toRange()), v))
        {            
            result = true;
        }
    }    
    mp_object->setProperty("ScreenUpdating",1);        
    mp_object->setProperty("Calculation",xlCalculationAutomatic);
    return result;
}


bool Excel::SetDataToRow(Excel::Table &t, const QStringList &data, int row)
{
    //    mp_excel->clearBag();
    //    if(!data.isEmpty())
    //    {
    //        int column =1;
    //        row = row+1; // translate to escel numbering 1...n
    //        foreach(const QString &txt, data)
    //        {
    //            if(!write(t.range, row, column, txt ,t.font)) return false;
    //            column++;
    //        }
    //        return true;
    //    }
    return false;
}

int Excel::width(const QString &range)
{
    QVariant v;
    if(mp_object->property(QString("ActiveSheet.Range(\"%1\").Width").arg(range),&v))
    {
        return v.toInt();
    }
    return 0;
}

int Excel::height(const QString &range)
{
    QVariant v;
    if(mp_object->property(QString("ActiveSheet.Range(\"%1\").Height").arg(range),&v))
    {
        return v.toInt();
    }
    return 0;
}

void Excel::setUpdatesOn(bool on)
{
    mp_object->setProperty("DisplayStatusBar",0);
    mp_object->setProperty("EnableEvents",0);
    mp_object->setProperty("Calculation",xlCalculationManual);
}


AxObject::Class Excel::CreateChart(const Chart &chart)
{  
    mp_object->clearBag();

    AxObject::Class pObj = 0 ;
    if(!m_opened) return 0;
    QVariant var;
    const int _XLChartType[] ={ xlXYScatterLinesNoMarkers,       xlLine  };

    mp_object->dynamicCall(QString("ActiveSheet.Range(\"%1\").Select").arg(chart.cellDataRange.toRange()));

    if(!chart.rect.isEmpty()){
        mp_object->dynamicCall("ActiveSheet.Shapes.AddChart2",&var ,-1, _XLChartType[chart.type]
                               ,chart.rect.x(),  chart.rect.y(),   chart.rect.width(),   chart.rect.height());
    }
    else {
        mp_object->dynamicCall("ActiveSheet.Shapes.AddChart2",&var ,-1, _XLChartType[chart.type]);
    }

    pObj = var.toInt();
    if(pObj==0) return 0;
    mp_object->setProperty("ScreenUpdating", false);

    mp_object->method_run(pObj,"Select");
    mp_object->setProperty("ActiveChart.ChartType", _XLChartType[chart.type]);

    AxObject::Class  pRange = mp_object->queryObject(QString("ActiveSheet.Range(\"%1\")").arg(chart.cellDataRange.toRange()));
    mp_object->dynamicCall("ActiveChart.SetSourceData",0, AxObjectType(DISPATCH,(quint32)pRange),2);


    mp_object->setProperty("ActiveChart.ChartTitle.Text",chart.title);

    int series_count =0;
    if(mp_object->property("ActiveChart.SeriesCollection.Count",&var))
    {
        series_count = var.toInt();
        for(int i=1;i<series_count+1;i++){
            mp_object->setProperty(QString("ActiveChart.SeriesCollection(%1).Border.Weight").arg(i),xlThin);
        }
    }
    const QColor std_colors[7] = {
        QColor(Qt::blue)
        ,QColor(Qt::magenta)
        ,QColor(24,135,9)
        ,QColor(244,131,0)
        ,QColor(133,153,7)
        ,QColor(137,82,37)
        ,QColor(Qt::black)
    };


    for(int i=0;i<qMin(series_count,7);i++)
    {
        mp_object->setProperty(QString("ActiveChart.SeriesCollection(%1).Format.Line.ForeColor.RGB").arg(i+1), std_colors[i]);
    }


    mp_object->setProperty("ActiveChart.HasTitle", !chart.title.isEmpty());
    mp_object->setProperty("ActiveChart.ChartTitle.Caption", QString("Automated Measurement Output Chart %1").arg(series_count));
    mp_object->setProperty("ActiveChart.ChartTitle.Text", chart.title);
    mp_object->setProperty("ActiveChart.ChartTitle.AutoScaleFont", false);    
    mp_object->setProperty("ActiveChart.ChartTitle.Font.Name", "Arial");
    mp_object->setProperty("ActiveChart.ChartTitle.Font.Size", 12);
    mp_object->setProperty("ActiveChart.ChartTitle.Font.Bold", true);



    mp_object->setProperty("ActiveChart.Axes(2).TickLabels.Font.Name","Arial");
    mp_object->setProperty("ActiveChart.Axes(2).TickLabels.Font.Size",8);
    mp_object->setProperty("ActiveChart.Axes(2).TickLabels.AutoScaleFont",false);
    mp_object->setProperty("ActiveChart.Axes(2).TickLabelPosition",xlLow );
    mp_object->setProperty("ActiveChart.Axes(2).HasMajorGridLines",true);

    mp_object->setProperty("ActiveChart.Axes(2).MajorGridLines.Border.Weight",xlHairLine );
    mp_object->setProperty("ActiveChart.Axes(2).MajorGridLines.Border.ColorIndex",16);
    mp_object->setProperty("ActiveChart.Axes(2).HasMinorGridlines",false);


    //xlValue
    if(!chart.yAxis.isEmpty()){
        mp_object->setProperty("ActiveChart.Axes(2).HasTitle", true);
        mp_object->setProperty("ActiveChart.Axes(2).AxisTitle.Caption", chart.yAxis);
        mp_object->setProperty("ActiveChart.Axes(2).AxisTitle.Font.Name","Arial");
        mp_object->setProperty("ActiveChart.Axes(2).AxisTitle.Font.Size",9);
        mp_object->setProperty("ActiveChart.Axes(2).AxisTitle.Font.Bold",true);
        mp_object->setProperty("ActiveChart.Axes(2).AxisTitle.AutoScaleFont",false);
    }

    //xlCategory
    if(!chart.xAxis.isEmpty()){
        mp_object->setProperty("ActiveChart.Axes(1).HasTitle" , true);
        mp_object->setProperty("ActiveChart.Axes(1).AxisTitle.Caption" , chart.xAxis);
        mp_object->setProperty("ActiveChart.Axes(1).AxisTitle.AutoScaleFont" , false);
        mp_object->setProperty("ActiveChart.Axes(1).AxisTitle.Font.Name" , "Arial");
        mp_object->setProperty("ActiveChart.Axes(1).AxisTitle.Font.Size" , 9);
        mp_object->setProperty("ActiveChart.Axes(1).AxisTitle.Font.Bold" , true);
    }

    const int _XLScaleType[] = {xlLinear,xlLogarithmic};
    mp_object->setProperty("ActiveChart.Axes(1).ScaleType" , _XLScaleType[chart.xScaleType]);

    mp_object->setProperty("ActiveChart.Axes(1).TickLabels.Font.Name" , "Arial");    

    mp_object->setProperty("ActiveChart.Axes(1).TickLabels.Font.Size" , 8);
    mp_object->setProperty("ActiveChart.Axes(1).TickLabels.AutoScaleFont" , 8);

    mp_object->setProperty("ActiveChart.Axes(1).TickLabelPosition" , xlLow);
    mp_object->setProperty("ActiveChart.Axes(1).HasMajorGridlines" , chart.majorGridLines);
    mp_object->setProperty("ActiveChart.Axes(1).MinorGridlines.Border.Weight" , xlHairLine);
    mp_object->setProperty("ActiveChart.Axes(1).MinorGridlines.Border.ColorIndex" , 16);
    mp_object->setProperty("ActiveChart.Axes(1).HasMinorGridlines" , chart.minorGridLines);
    mp_object->setProperty("ActiveChart.Axes(1).MinorGridlines.Border.Weight" , xlHairLine);
    mp_object->setProperty("ActiveChart.Axes(1).MinorGridlines.Border.ColorIndex" , 15);



    QVariant width,left,height;
    mp_object->property("ActiveChart.ChartArea.Width",&width);
    mp_object->property("ActiveChart.ChartArea.Height",&height);
    mp_object->setProperty("ActiveChart.PlotArea.Height", height.toInt() -40);
    mp_object->setProperty("ActiveChart.PlotArea.Interior.ColorIndex",xlAutomatic);    
    mp_object->property("ActiveChart.PlotArea.Left",&left);
    mp_object->setProperty("ActiveChart.PlotArea.Width", width.toInt() -2*left.toInt());

    // legend
    if(chart.legendVisible){
        mp_object->setProperty("ActiveChart.HasLegend",true);
        mp_object->setProperty("ActiveChart.Legend.Font.Name","Arial");
        mp_object->setProperty("ActiveChart.Legend.Font.Size",8);
        mp_object->setProperty("ActiveChart.Legend.IncludeInLayout",false);
        mp_object->setProperty("ActiveChart.Legend.Position",xlLegendPositionRight);
        mp_object->setProperty("ActiveChart.Legend.Format.Fill.Visible",true);
        mp_object->setProperty("ActiveChart.Legend.Format.Fill.ForeColor.ObjectThemeColor",msoThemeColorBackground1);
        mp_object->setProperty("ActiveChart.Legend.Format.Fill.ForeColor.TintAndShade",true);
        mp_object->setProperty("ActiveChart.Legend.Format.Fill.ForeColor.Brightness",0);
        mp_object->setProperty("ActiveChart.Legend.Format.Fill.Transparency",0);
        mp_object->setProperty("ActiveChart.Legend.Interior.PatternColorIndex",2);//white
        mp_object->setProperty("ActiveChart.Legend.Interior.Pattern",xlSolid);//white
        mp_object->setProperty("ActiveChart.Legend.Border.Weight",xlHairLine);
        mp_object->setProperty("ActiveChart.Legend.Border.ColorIndex",1); //Black
        mp_object->setProperty("ActiveChart.Legend.AutoScaleFont",false); //Black
    }

    mp_object->setProperty("ScreenUpdating", true);

    return pObj;
}

bool Excel::SetChartData(AxObject::Class chart, const QString &range)
{
    return false;
}


/*


  LIST:ActiveSheet.Shapes("Chart 7").IncrementLeft -429
  expression .AddChart2(Style,XlChartType,Left,Top,Width,Height,NewLayout)
*/


//bool ExcelData::setBorders(qint32 row, qint32 col, qint32 width, qint32 height, ExcelData::XlBorders borders)
//{
//    return 1;
//}


/****************************************************************************
 * @function name: ExcelData::read - ---
 *
 * @param:
 *
 *      qint32 i
 *      qint32 j
 *      QVariant &data
 * @description: rads data from cell i,j
 * @return: ( bool ) success = true
 ****************************************************************************/
bool Excel::read(qint32 row, qint32 col, QVariant &data)
{
    bool result = false;
    if (m_opened && row > 0 && col > 0 )
    {
        result = mp_object->property(QString("ActiveSheet.Cells( %1, %2).Value").arg(row).arg(col),&data);
    }
    return result;
}

bool Excel::readRange(const QString &range, QVariantList *presult)
{
    QVariant data;
    bool r= mp_object->property(QString("ActiveSheet.Range(\"%1\").Value").arg(range),&data);
    if(presult) *presult = data.toList();
    return r;
}

bool Excel::writeRange(const QString &range, QVariantList l)
{
    // TODO
    return false;
}

bool Excel::drawFrame(const Rect &rect, const Frame &f){    
    return drawFrame(rect.toRange(),f);
}

bool Excel::drawFrame(const QString &range , const Frame &f)
{
    mp_object->clearBag();
    if(m_opened){
        for(int i=0;i<5;i++)
        {
            if(f.drawLine(i))
            {
                const int xlBorders[] = {9,7,10,8,12,11};
                const int xlStyles[] = { -4142,1,-4119};
                const int xlWidth[] = {1, -4138 ,4, 2};

                mp_object->setProperty(QString("ActiveSheet.Range(\"%1\").Borders(%2).LineStyle")
                                       .arg(range).arg(xlBorders[i]), xlStyles[f.style(i)]);
                if( f.style(i) != Frame::LineDouble)
                    mp_object->setProperty(QString("ActiveSheet.Range(\"%1\").Borders(%2).Weight")
                                           .arg(range).arg(xlBorders[i]), xlWidth[f.width(i)]);
            }
        }
        return true;
    }
    return false;
}



/****************************************************************************
 * @function name: ExcelData::setColor()
 *
 * @param:
 *    qint32 i
 *    qint32 j
 *    const QColor background
 *    const QColor foreground
 * @description: Sets color of background ,foreground cell
 * @return: ( bool ) - success = true
 ****************************************************************************/
bool Excel::setColor(qint32 row, qint32 col, const QColor background, const QColor foreground)
{
    bool result = false;
    if (m_opened && row > 0 && col > 0 )
    {
        quint32 color;
        color = (background.red() << 0) | (background.green() << 8) | (background.blue() << 16);
        result = mp_object->setProperty(QString("ActiveSheet.Cells(%1,%2).Interior.Color").arg(row).arg(col), QVariant(color));
        color = (foreground.red() << 0) | (foreground.green() << 8) | (foreground.blue() << 16);
        result = mp_object->setProperty(QString("ActiveSheet.Cells(%1,%2).Font.Color").arg(row).arg(col), QVariant(color));
        result = true;
    }
    return result;
}

bool Excel::setColor(const QString &range, const QColor background, const QColor foreground)
{
    bool result = false;
    if (m_opened && Range_Is_Valid(range) )
    {
        quint32 color;
        color = (background.red() << 0) | (background.green() << 8) | (background.blue() << 16);
        result = mp_object->setProperty(QString("ActiveSheet.Range(\"%1\").Interior.Color").arg(range), QVariant(color));
        color = (foreground.red() << 0) | (foreground.green() << 8) | (foreground.blue() << 16);
        result = mp_object->setProperty(QString("ActiveSheet.Range(\"%1\").Font.Color").arg(range), QVariant(color));
        result = true;
    }
    return result;
}

/****************************************************************************
 * @function name: ExcelData::color()
 *
 * @param:
 *      qint32 i
 *      qint32 j
 *      QColor &background
 *      QColor &foreground
 * @description: gets color of background and foregorund
 * @return: ( bool ) success = true
 ****************************************************************************/
bool Excel::color(qint32 row, qint32 col, QColor &background, QColor &foreground)
{
    bool result = false;
    if (m_opened && row > 0 && col > 0 )
    {
        do{
            QVariant v1, v2;
            if(!mp_object->property( QString("ActiveSheet.Cells(%1,%2).Interior.Color").arg(row).arg(col),&v1)) break;
            background = QColor::fromRgb(v1.toInt());
            if(!mp_object->property( QString("ActiveSheet.Cells(%1,%2).Font.Color").arg(row).arg(col),&v2)) break;
            foreground = QColor::fromRgba(v2.toInt());
            result = true;
        }while(0);
    }
    return result;
}


/****************************************************************************
 * @function name: ExcelData::setVisible()
 * @param:
 *      bool visible
 * @description: sets visibility of workbook
 * @return: ( void )
 ****************************************************************************/
bool Excel::setVisible(bool visible)
{
    if ( isOpen())
    {
        return  mp_object->setProperty("Visible", visible);
    }
    return false;
}

bool Excel::visible()
{
    if ( isOpen())
    {
        QVariant res;
        bool ok = mp_object->property("Visible",&res);
        return  ok && res.toBool();
    }
    return false;
}

bool Excel::valid()
{
    if ( isOpen())
    {
        QVariant tmp;
        return mp_object->property("Visible",&tmp);
    }
    return false;
}

bool Excel::resizeCells(double size)
{
    return mp_object->dynamicCall("Cells.Select") && mp_object->setProperty("Selection.ColumnWidth",size);
}


/****************************************************************************
 * @function name: ExcelData::close()
 *
 * @param:
 *             void
 * @description: closes workbook
 * @return: ( void )
 ****************************************************************************/
void Excel::close()
{   
    if(isOpen()){
        if(m_autosave) save();
        mp_object->dynamicCall("ActiveWorkbook.Close");
        m_opened = false;
    }
}


int Excel::sheetsCount()
{
    QVariant var;
    mp_object->property("ActiveWorkbook.Sheets.Count", &var);
    return var.toInt();
}

QStringList Excel::sheetsList()
{
    QStringList result;
    int sheets_count = sheetsCount();
    if(sheets_count>0)
    {
        for(int i=1;i<=sheets_count;i++)
        {
            QVariant var;
            mp_object->property(QString("ActiveWorkbook.Sheets(%1).Name").arg(i),&var);
            result += var.toString();
        }
    }
    return result;
}


QString Excel::Cell_To_Name(const Excel::Cell &cell,bool fixed)
{
    int index = 26*26*26*26;
    int col = cell.x();
        QString col_abc;
        while(index>1)
        {
            if(col>=index )
            {
                col_abc += QChar('A' +col/index-1);
                col = col % index;
            }
            index /=26;
        }
        col_abc += QChar('A' +col);

    if(fixed){
        return
                QString("$%1$%2").arg(col_abc).arg(1+cell.y());
        }
    return
        QString("%1%2").arg(col_abc).arg(1+cell.y());

}

QString Excel::Rect_To_Range(const Excel::Rect &rect,bool fixed)
{
    Cell c = rect.p2();
    if(c.x()==0)        c.setX(1);
    else c.setX(c.x()-1);
    if(c.y() ==0)        c.setY(1);
    else c.setY(c.y()-1);
    return QString("%1:%2")
            .arg(Cell_To_Name(rect.p1(),fixed))
            .arg(Cell_To_Name(c,fixed));
}

Excel::Cell Excel::Name_To_Cell(const QString &xl_name)
{
    return Cell();
}

Excel::Rect Excel::Range_To_Rect(const QString &xl_range)
{
    return Rect();
}

bool Excel::Range_Is_Valid(const QString &range)
{
    // TODO
    return true;
}


bool Excel::test()
{
    mp_object->blockSignals(1);
    QVariant temp;
    bool result = read(1,1,temp);
    mp_object->blockSignals(0);
    return result;
}
