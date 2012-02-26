#include <QtGui/QLabel>
#include <QtGui/QToolBar>
#include "diffwindow.h"
#include "store.h"
#include "filter.h"
#include <stdio.h>

DiffWindow::DiffWindow()
{
    connect(store, SIGNAL(dataChanged()), this, SLOT(updateDisplay()),
            Qt::QueuedConnection);
    filter = new QComboBox();
    QMap<QString, Filter *>::const_iterator i = filters.constBegin();
    while (i != filters.constEnd()) {
        filter->insertItem(0, i.key());
        i++;
    }

    QToolBar * tb = new QToolBar();
    addToolBar(tb);
    tb->addWidget(filter);
    main_widget = new QTableWidget;
    setCentralWidget(main_widget);
    setWindowTitle("Diff");
    connect(filter, SIGNAL(currentIndexChanged(int)),
            this, SLOT(updateDisplay()));
    resize(640,480);  // Just to be a bit more sensible
}

void DiffWindow::updateDisplay()
{
    main_widget->clear();
    Filter * f = filters[filter->currentText()];
    if (!f)
    {
        return;
    }

    store->lockRead();
    QStringList headers;

    int real_rows = 0;
    
    main_widget->setColumnCount(store->getNumColumns() * 2);
    main_widget->setRowCount(10000);
    
    int current_display_column = 0;
    for (int loopc=0; loopc<store->getNumColumns(); loopc++)
    {
        headers.push_back("Address");
        headers.push_back(store->getNames()[loopc]);
        int current_row = 0;

        int pos = 0;
        
        while (pos < store->getNumRows())
        {
            QString addr;
            addr = QString::number(pos / BITS_IN_BYTE);
            addr += ".";
            addr += QString::number(pos % BITS_IN_BYTE);
            
            main_widget->setCellWidget(current_row, current_display_column,
                                       new QLabel(addr));
            bool diff = false;
            QString ret = f->parse(pos, loopc, diff);
            QLabel * label = new QLabel(ret);
            label->setTextFormat(Qt::RichText);
            QFont font;
            font.setFixedPitch(true);
            label->setFont(font);
            if (diff)
            {
                label->setAutoFillBackground(true);
                label->setStyleSheet("QLabel { background-color: rgb("
                                     "255,200,200); }");
            } 
            main_widget->setCellWidget(current_row, current_display_column+1,
                                       label);
            current_row++;
            if (current_row > real_rows)
            {
                real_rows = current_row;
            }    
        }
        current_display_column += 2;
    }
    store->unlock();
    main_widget->setRowCount(real_rows);
    main_widget->setHorizontalHeaderLabels(headers);
    main_widget->resizeColumnsToContents();
    main_widget->resizeRowsToContents();
}






