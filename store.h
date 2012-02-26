#ifndef _STORE_
#define _STORE_

#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>

#include <stdio.h>

#define BITS_IN_BYTE 8  // Never know, could be looking at a VAX!

typedef QList<bool> BitList;

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

    BitList getBits(int column, int start, int len, bool little_endian);
    BitList getDiffs(int start, int len, bool litle_endian);
        
    QList<QString> getNames() { return names; }
    
    bool at(int c,int r)
    {
        if (c > width || r > height || c < 0 || r < 0)
        {
            printf("Store bounds error!\n");
            return false;
        }
        return bits[(c*height)+r];
    }
    
    bool isDiff(int r)
    {
        if (r > height || r < 0)
        {
            printf("Diff bounds error!\n");
            return false;
        }
        return diff[r];
    }

    void lockRead()
    {
        lock.lockForRead();
    }
    
    void lockWrite()
    {
        lock.lockForWrite();
    }

    void unlock()
    {
        lock.unlock();
    }
    
  signals:

    void dataChanged();
    
  protected:

    QReadWriteLock lock;
    
    QList<QString> names;
    QList<QByteArray> qbas;

    int width;
    int height;
    
    bool * bits;
    bool * diff;
};

extern Store * store;

#endif
