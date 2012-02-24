#ifndef _EDITOR_
#define _EDITOR_

#include <stdio.h>

#include <QtGui/QMainWindow>
#include <QtGui/QTextEdit>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtCore/QThread>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtGui/QToolBar>
#include <QtGui/QStatusBar>

class EditorThread  : public QThread
{
    Q_OBJECT
        
  public:

    EditorThread(QString h, QStringList a, QString p, QStringList s)
    {
        helper=h;
        args=a;
        program=p;
        subs=s;
    }
    
    virtual void run();

  signals:

    void done();
    void progress(const QString &);
    
  protected:
    
    QByteArray getData(int, QString input);

    QString helper;
    QStringList args;
    QString program;
    QStringList subs;
    
};


class Editor : public QMainWindow
{
    Q_OBJECT
        
  public:

    Editor();

    public slots:

    void goPressed();
    
  protected:

    QTextEdit * edit;
    QLineEdit * helper;
    QLineEdit * args;
    QLineEdit * subs;
    QPushButton * go;
    
};

extern Editor * editor;


#endif
