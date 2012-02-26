#ifndef _FILTER_
#define _FILTER_

#include <QtCore/QString>
#include <QtCore/QMap>

#include "store.h"

class Filter
{
  public:

    Filter(QString n)
    {
        name = n;
    }

    QString nameGet() { return name; }
    
    virtual QString parse(int &, int, bool & w) { w=false; return ""; }
    
  protected:
    
    unsigned char getByte(BitList list, BitList diffs, int & pos,
                          bool & diff);

    QString beginHighlight()
    {
        return "<font color=\"red\"><b>";
    }

    QString endHighlight()
    {
        return "</b></font>";
    }
    
    QString name;
    
};

class BinaryFilter : public Filter
{
  public:

    BinaryFilter(QString n, int s)
        : Filter(n)
    {
        size = s;
    }
    
    virtual QString parse(int & bitpos, int column, bool & was_diff);

  protected:

    int size;
    
};

class HexFilter : public Filter
{
  public:

  HexFilter(QString n, int s, bool b)
        : Filter(n)
    {
        size = s;
        little_endian = b;
    }
    
    virtual QString parse(int & bitpos, int column, bool & was_diff);

  protected:
    
    QString extract_byte(int bitpos, int column, bool & was_diff);
    
    int size;
    bool little_endian;

};



extern QMap<QString, Filter *> filters;

#endif
