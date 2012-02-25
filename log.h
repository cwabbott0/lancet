#ifndef _LOG_
#define _LOG_

#include <QtGui/QTextEdit>

class Log : public QTextEdit
{
    Q_OBJECT

public:

        // Just need to make it safe to close the log
        // All actual functionality is in QTextEdit
    Log();
    ~Log();

};

extern Log * log;

#endif
