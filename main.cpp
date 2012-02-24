#include <QtGui/QApplication>
#include "editor.h"
#include "store.h"
#include "diffwindow.h"
#include "filter.h"
#include "log.h"

Editor * editor;
Store * store;
Log * log;
QMap<QString, Filter *> filters;

int main(int argc, char ** argv)
{
    QApplication app(argc, argv);
    store = new Store();
    editor = new Editor();
    log = new Log();
    filters["32-bit binary"] = new BinaryFilter("32-bit binary", 32);
    filters["32-bit hex BE"] = new HexFilter("32-bit hex BE", 32, false);
    filters["32-bit hex LE"] = new HexFilter("32-bit hex LE", 32, true);
    editor->show();
    DiffWindow * dw = new DiffWindow();
    dw->show();
    log->show();
    
    app.exec();
}
