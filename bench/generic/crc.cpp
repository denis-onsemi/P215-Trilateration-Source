#include "crc.h"


quint8 crc8(quint32 init,quint32 poly, quint8 *data, qint32 len ,CrcDir crc_dir)
{
    int j,i;
    quint8 crc  = init;

    if(crc_dir == CRC_MSB)
    {
        for (i = 0; i < len; i++)
        {
            crc ^= data[i];
            for (j = 0; j < 8; j++)
            {
                if (crc & 0x80)
                    crc = (crc << 1) ^ poly;
                else
                    crc <<= 1;
            }
        }
    }
    else if(crc_dir==CRC_LSB)
    {
        for (i = 0; i < len; i++)
        {
            crc ^= data[i];
            for (j = 0; j < 8; j++)
            {
                if (crc & 0x1)
                    crc = (crc >> 1) ^ poly;
                else
                    crc >>= 1;
            }
        }
    }
    return crc;
}

quint16 crc16(quint32 seed, quint32  poly, quint16 *data, qint32 len,CrcDir crc_dir)
{
    int j,i;
   quint32 crc  = 0;
   if(crc_dir == CRC_MSB)
    {
            //  qDebug()<<"MSB";
            //  qDebug()<<QString::number(crc,2);
            for (i = 0; i < len+1; i++)
    {
        if(i==0){
            crc ^= seed;
        }
        else {crc ^= data[i-1];
        }

        for (j = 0; j < 16; j++)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ poly;
            else
                crc <<= 1;
            // qDebug()<<QString::number(crc,2);
        }
    }
}
else if(crc_dir == CRC_LSB)
{
    //  qDebug()<<"LSB";
    for (i = 0; i < len+1; i++)
    {
        if(i==0){
            crc ^= seed;
        }
        else crc ^= data[i-1];

        for (j = 0; j < 16; j++)
        {
            //  qDebug()<<QString::number(crc,2);
            if (crc & 0x1)
                crc = (crc >> 1) ^ poly;
            else
                crc >>= 1;
        }
    }
}

return crc;
}



