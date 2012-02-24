#ifndef _STORE_
#define _STORE_

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QObject>

#include <stdio.h>

#define BITS_IN_BYTE 8  // Never know, could be looking at a VAX!

class Store : public QObject
{
    Q_OBJECT
        
  public:

    Store();
    void clear();
    void addArray(QString name,QByteArray data);
    void makeDiff();

    int getNumColumns() { return width; }
    int getNumRows() { return height; }
    
    QList<QString> getNames() { return names; }
    
    bool at(int c,int r)
    {
        if (c*r > (width * height))
        {
            printf("Store bounds error!\n");
            return false;
        }
        return bits[(c*height)+r];
    }
    
    bool isDiff(int r)
    {
        if (r > height)
        {
            printf("Diff bounds error!\n");
            return false;
        }
        return diff[r];
    }
    
  signals:

    void dataChanged();
    
  protected:

    QList<QString> names;
    QList<QByteArray> qbas;

    int width;
    int height;
    
    bool * bits;
    bool * diff;
};

extern Store * store;

#endif
