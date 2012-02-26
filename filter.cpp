#include "filter.h"
#include "store.h"

#include <stdio.h>
#include <math.h>

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
        ret |= list[pos] ? 1 : 0;
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

QString FloatFilter::parse(int & bitpos, int column, bool & was_diff)
{
        // Not currently handled - stuff like NaN
    was_diff = false;
    int size = mantissa+exponent+1;
    BitList bits = store->getBits(column, bitpos, size, little_endian);
    BitList diffs = store->getDiffs(bitpos, size, little_endian);

    QString ret;
    
    for (int loopc=0; loopc<diffs.size(); loopc++)
    {
        if (diffs[loopc])
        {
            was_diff = true;
            ret += beginHighlight();
            break;
        }    
    }

    bool sign = bits[0];
    if (sign)
    {
        ret += "-";
    }
    
    quint64 exp_int = 0;
    for (int loopc=0; loopc<exponent; loopc++)
    {
        exp_int = exp_int << 1;
        exp_int |= bits[loopc+1] ? 0x1 : 0x0;
    }

    quint64 mant_int = 0;
    for (int loopc=0; loopc<mantissa; loopc++)
    {
        mant_int = mant_int << 1;
        mant_int |= bits[loopc+exponent+1] ? 0x1 : 0x0;
    }
    
    int adj_exp = 0;

    int excess = (0x1 << (exponent - 1)) -1;
    
    if (exp_int != 0)
    {
        adj_exp = exp_int - excess;
        mant_int |= (0x1 << mantissa);
    }

    double fresult = 0;
    double adder = 1.0;
    for (int loopc=mantissa;loopc>=0;loopc--)
    {
        if (mant_int & (0x1 << loopc))
        {
            fresult += adder;
        }
        adder /= 2.0;
    }
    fresult *= pow(2, adj_exp);
    ret += QString::number(fresult);

    bitpos += size;

    if (was_diff)
    {
        ret += endHighlight();
    }
    
    return ret;
}



