#ifndef _DIFFWINDOW_
#define _DIFFWINDOW_

#include <QtGui/QMainWindow>
#include <QtGui/QTableWidget>
#include <QtGui/QComboBox>

class DiffWindow : public QMainWindow
{
    Q_OBJECT
        
  public:

    DiffWindow();
    
    public slots:

    void updateDisplay();

  protected:

    QComboBox * filter;
    QTableWidget * main_widget;
    
};

#endif
