#include "store.h"

#include <stdio.h>

Store::Store()
{
    width=0;
    height=0;
    bits=0;
    diff=0;
}

void Store::clear()
{
    width=0;
    height=0;
    names.clear();
    qbas.clear();
}

void Store::addArray(QString name,QByteArray data)
{
    names.push_back(name);
    qbas.push_back(data);
}

void Store::makeDiff()
{
    int maxlen=0;
    for (int loopc=0;loopc<qbas.size();loopc++)
    {
        maxlen = (maxlen > qbas[loopc].size()) ? maxlen : qbas[loopc].size();
    }
    
    lockWrite();
    width = qbas.size();
    height = maxlen * BITS_IN_BYTE;
    
    delete[] bits;
    delete[] diff;

    bits = new bool[width * height];
    diff = new bool[height];
    memset(bits, 0, width * height);
    memset(diff, 0, height);
    
    int count = 0;
    for (int loopc=0; loopc<qbas.size(); loopc++)
    {
        count = (loopc * height);
        const unsigned char * ptr = (const unsigned char *)qbas[loopc].constData();
        for (int loopc2 = 0; loopc2 < qbas[loopc].size(); loopc2++)
        {
            unsigned char byte = *ptr;
            for (int loopc3 = 0; loopc3 < 8; loopc3++)
            {
                bits[count] = byte & 0x80;    // Big-bit-endian
                count++;
                byte = byte << 1;
            }
            ptr++;
        }
    }

    for (int loopc=0; loopc<height; loopc++)
    {
        bool initial = at(0,loopc);
        diff[loopc] = false;
        for (int loopc2=0; loopc2<width; loopc2++)
        {
            if (initial != at(loopc2, loopc))
            {
                diff[loopc] = true;
                break;
            }
        }
    }
    
    qbas.clear();
    unlock();
    emit dataChanged();
}
