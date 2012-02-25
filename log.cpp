
#include "log.h"

Log::Log()
{
    setWindowTitle("Log");
    log = this;
}

Log::~Log()
{
    log = 0;
}
