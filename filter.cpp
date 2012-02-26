#include "filter.h"
#include "store.h"

#include <stdio.h>

unsigned char Filter::getByte(BitList list, BitList diffs, int & pos,
                              bool & nybble_diff)
{
    unsigned char ret = 0;

    if (pos+BITS_IN_BYTE > list.size() ||
        pos+BITS_IN_BYTE > diffs.size())
    {
        printf("getByte bounds error %d!\n", pos);
        return 0;
    }
    
    for (int loopc=0; loopc<BITS_IN_BYTE; loopc++)
    {
        ret = ret << 1;
        ret |= list[pos];
        if (diffs[pos])
        {
            nybble_diff = true;
        } 
        pos++;
    }

    return ret;
}

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
            ret += beginHighlight();
        }
        
        int i;
        i = (store->at(column, bitpos)) << 3;
        i |= (store->at(column, bitpos+1)) << 2;
        i |= (store->at(column, bitpos+2)) << 1;
        i |= (store->at(column, bitpos+3)) << 0;

        ret += QString::number(i, 16);
        
        if (was_diff)
        {
            ret += endHighlight();
        }

        bitpos += 4;
    }

    return ret;
}

QString HexFilter::parse(int & bitpos, int column, bool & was_diff)
{
    BitList bits = store->getBits(column, bitpos, size, little_endian);
    BitList diffs = store->getDiffs(bitpos, size, little_endian);
    
    was_diff = false;

    QString ret;

    int listpos = 0;
    
    for (int loopc=0; loopc<size / BITS_IN_BYTE; loopc++)
    {
        bool nybble_diff = false;

        unsigned char val = getByte(bits, diffs, listpos, nybble_diff);

        if (nybble_diff)
        {
            ret += beginHighlight();
            was_diff = true;
        }
        
        QString v = QString::number(val, 16);
        if (v.size() == 1)
        {
            ret += "0";
        }
        
        ret += v;
            
        if (nybble_diff)
        {
            ret += endHighlight();
        }
    }

    bitpos += size;
    return ret;
}



