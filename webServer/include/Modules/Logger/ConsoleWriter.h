#ifndef __CONSOLE_WRITER_H__
#define __CONSOLE_WRITER_H__

#include "Modules/ConsoleMain.h"

namespace LOGGER_SERVICE
{
    class ConsoleWriter : public LOGGER_SERVICE::LogWriterInterface
    {
    public:
        ConsoleWriter() : LOGGER_SERVICE::LogWriterInterface(&std::cout) {}
        ~ConsoleWriter() {}
    };
}

#endif