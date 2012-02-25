#include "filter.h"
#include "store.h"

#include <stdio.h>

QString BinaryFilter::parse(int & bitpos, int column, bool & was_diff)
{
    QString ret;
    for (int loopc=0; loopc<size; loopc++)
    {
        int pos = bitpos+loopc;
        
        if (store->isDiff(pos))
        {
            ret += "<font color=\"red\"><b>";
            was_diff = true;
        }
        ret += store->at(column, pos) ? "1" : "0";
        if (store->isDiff(pos))
        {
            ret += "</b></font>";
        }
    }
    bitpos += size;
    return ret;
}

QString HexFilter::extract_byte(int bitpos, int column, bool & was_diff)
{
    QString ret;
    
    for (int loopc=0; loopc<2; loopc++)
    {   
        was_diff = false;
        for (int loopc2=0; loopc2<4; loopc2++)
        {
            if(store->isDiff(bitpos+loopc2))
            {
                was_diff = true;
                break;
            }
        }

        if (was_diff)
        {
            ret += "<font color=\"red\"><b>";
        }
        
        int i;
        i = (store->at(column, bitpos)) << 3;
        i |= (store->at(column, bitpos+1)) << 2;
        i |= (store->at(column, bitpos+2)) << 1;
        i |= (store->at(column, bitpos+3)) << 0;

        ret += QString::number(i, 16);
        
        if (was_diff)
        {
            ret += "</b></font>";
        }

        bitpos += 4;
    }

    return ret;
}

QString HexFilter::parse(int & bitpos, int column, bool & was_diff)
{
    QString ret;
    
    was_diff = false;

    if (little_endian)
    {
        for(int loopc = bitpos + size - 8; loopc>=bitpos; loopc-=8)
        {
            ret += extract_byte(loopc, column, was_diff);
        }
    }
    else
    {
        for(int loopc = bitpos; loopc < bitpos+size; loopc+=8)
        {
            ret += extract_byte(loopc, column, was_diff);
        }
    }
    
    bitpos += size;
    return ret;
}



