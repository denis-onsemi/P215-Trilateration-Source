/**
 * @file:exceldata.h   -
 * @description: This is Excel communication class.
 * @project: BENCH OnSemiconductor
 * @date: 2014\11\27 13-51-10
 *
 */


#ifndef EXCEL_H
#define EXCEL_H

#include <QObject>
#include "axobject.h"

#include <QString>
#include "excelenums.h"

#include <QRect>

class Excel : public QObject
{
    Q_OBJECT
public:

    class Frame
    {
      public:

        enum {
            DrawBottom = 1,
            DrawLeft = 2,
            DrawRight = 4,
            DrawTop = 8,
            DrawHorizontal = 16,
            DrawVertical =32
        };


        enum {
            Bottom,
            Left,
            Right,
            Top,
            Horizontal,
            Vertical
        };// line

        enum {
            LineNone ,
            LineContinuous,
            LineDouble
        };//style

        enum {
               WidthHairline,
               WidthMedium ,
               WidthThick ,
               WidthThin
            };// width

        void setDrawLines(quint32 lines){
            m_draw_lines = lines;
        }
        void setStyle(int line ,int style){
            m_styles[line] = style;
        }

        void setWidth(int line, int width){
            m_widths[line] = width;
        }
        int style(int line) const{
            return m_styles[line];
        }
        int width(int line)const{
            return m_widths[line];
        }

        bool drawLine(int line) const{
            return m_draw_lines&(1<<line);
        }


        Frame(quint32 draw_lines){
            setDrawLines(draw_lines);
            for(int i=0;i<6;i++){
                m_styles[i] = LineContinuous;
                m_widths[i] = WidthThin;
            }
        }

    private:
        quint32 m_draw_lines;
        int m_styles[6];
        int m_widths[16];

    };

    class Cell
    {
    public:
        Cell(){m_x=-1;m_y=-1;}
        Cell(const QString &range){
            QRegExp rx("([a-zA-Z]+)([0-9]+)");
            if(rx.exactMatch(range)){
                //m_x =
                m_y = rx.cap(2).toInt()-1;
            }
        }

        Cell(int x, int y)
        {
            if(m_x>=0 && m_y>=0)
            {
                this->m_x = x;
                this->m_y = y;
            }
        }
        int x() const {return m_x;}
        int y() const {return m_y;}
        void setX(int x) {m_x=x;}
        void setY(int y) {m_y=y;}
        QString toRange(bool fixed = false) const
        {
            return Cell_To_Name(*this,fixed);
        }

        bool isEmpty() const
        {
            return x()==-1 && y()==-1;
        }
        int xlRow() const
        {
            return y()+1;
        }
        int xlCol() const
        {
            return x()+1;
        }

    private:
        int m_x;
        int m_y;

    };

    class Rect
    {
    public:
        Rect(){}
        Rect(int x,int y,int width,int height)
        {
            if(width>0 && height >0 && x>=0 && y>=0)
            {
                this->m_p1.setX(x);
                this->m_p1.setY(y);
                this->m_p2.setX(x+width);
                this->m_p2.setY(y+height);
            }
        }
        int x() const  { return m_p1.x();}
        int y()const  { return m_p1.y();}
        int width() const {
            return m_p2.x() - m_p1.x();
        }
        int height() const {
            return m_p2.y() - m_p1.y();
        }
        void setX(int x) {
            m_p1.setX(x);
        }
        void setY(int y) {
            m_p1.setY(y);
        }
        void setWidth(int w) {
            m_p2.setX(w+m_p1.x());
        }
        void setHeight(int h) {
            m_p2.setY(h+m_p1.y());
        }
        QString toRange(bool fixed=false) const {
            return Rect_To_Range(*this,fixed);
        }
        bool isEmpty() const {
            return m_p1.isEmpty() && m_p2.isEmpty();
        }
        Cell p1() const {return m_p1;}
        Cell p2() const {return m_p2;}

        Rect row( int i){
            Rect r = *this;
            if(i<this->height())    {
                r.setY(i+y());
                r.setHeight(1);
            }
            return r;
        }

        Rect column(int i){
            Rect r = *this;
            if(i<this->width())    {
                r.setX(i+x());
                r.setWidth(1);
            }
            return r;
        }

        Cell cell(int x, int y){
            Cell c;
            if(x<width() && y<height()){
                c.setX(x+this->x());
                c.setY(y+this->y());
            }
            return c;
        }

    private:
        Cell m_p1;
        Cell m_p2;
    };

    struct Table
    {
        AxObject::Class range;
        Rect dataRange;
        Rect tableRange;

        int rows_count;  //current text data added to rows
        int height;// rows
        int width; // columns
        QFont font;
    };
    enum ChartType{
        Chart_ScatterLine, Chart_Line
    };


    enum ScaleType{
        Scale_Linear, Scale_Logarithmic
    };


    struct Chart{
        Rect cellDataRange;
        QString title;
        QRect rect;
        ChartType type;
        ScaleType xScaleType;
        ScaleType yScaleType;
        QString xAxis;
        QString yAxis;
        bool legendVisible;
        bool minorGridLines;
        bool majorGridLines;
    };





    explicit Excel(const QString filename, bool use_thread=false
                            ,bool autosave=true, bool displayAlerts = true);
    ~Excel();

    void setErrorSlot(QObject *pobj, const char *slot);
    QString fileName();
    /* opens excel document*/
    bool open();
    void release();
    void clearAbort();
    /* returns if document is opened*/
    bool isOpen() const;
    bool setZoom(int val);
    int zoom();
    /* add sheet with name to opened workbook*/
    bool addSheet(const QString sheetname);    
    /* sets current sheet name*/
    bool setCurrentSheet(const QString sheetname);
    int sheetsCount();
    QStringList sheetsList();
    /* removes sheet sheetname*/
    bool removeSheet(const QString sheetname);
    /* write data to cell*/
    bool write(Excel::Cell cell, const QVariant &data, const QFont &font);
    bool write(qint32 row, qint32 col, const QVariant &data, const QFont &font= QFont()  );
    bool write(AxObject::Class range, qint32 row, qint32 col, const QVariant &data, const QFont &font= QFont()  );
    bool write(const QString &range, const QStringList &l, const QFont &font=QFont());

    //reads range of data
    bool readRange(const QString &range, QVariantList *presult);
    bool writeRange(const QString &range, QVariantList l);

    // shows current cell
    bool cellVisible(int row, int col);

    /* reads data from cell*/
    bool read(qint32 row, qint32 col, QVariant &data);

    bool drawFrame(const QString &range, const Frame &f);
    bool drawFrame(const Rect &rect, const Frame &f);
    /* sets color of cell*/
    bool setColor(qint32 row, qint32 col, const QColor background, const QColor foreground);
    bool setColor(const QString &range, const QColor background, const QColor foreground);
    /* gets color of cell */
    bool color(qint32 row, qint32 col, QColor &background, QColor &foreground);
    /* sets visible workbook*/
    bool setVisible(bool visible);
    bool visible();
    bool valid();
    bool resizeCells(double size);
    /* closes workbook*/
    void close(void);
    /* saves workbook to file*/
    bool save();
    bool saveAs(const QString &);

    QStringList namedRanges();

    bool test();
    static QString Cell_To_Name(const Cell &cell, bool fixed=false);
    static QString Rect_To_Range(const Rect &rect, bool fixed=false);
    static Cell Name_To_Cell(const QString &xl_name);
    static Rect Range_To_Rect(const QString &xl_range);
    static bool Range_Is_Valid(const QString &range);

    Excel::Table CreateTable(const Rect &range, const QStringList &headers, const QStringList &data = QStringList());

    bool AppendRow(Table &t, const QStringList &data);
    bool SetDataToColumn(Excel::Table &t, const QVariantList &data, int column);
    bool SetDataToRow(Excel::Table &t, const QStringList &data,int row);

    int width(const QString &range);
    int height(const QString &range);

    void setUpdatesOn(bool on);
    // *****************charts************************

    // charts
    AxObject::Class CreateChart(const Chart &chart);

    AxObject *object() {return mp_object;}

    bool SetChartData(AxObject::Class chart, const QString &range);

private:
    AxObject *mp_object;
    QString m_filename;
    QString m_sheetname;
    bool m_opened;
    bool m_saved;
    bool m_autosave;

signals:

public slots:

};



#endif // EXCELDATA_H

