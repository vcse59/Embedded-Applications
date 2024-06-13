#ifndef __FILE_WRITER_H__
#define __FILE_WRITER_H__

#include "Modules/ConsoleMain.h"

namespace LOGGER_SERVICE
{
    class FileWriter : public LOGGER_SERVICE::LogWriterInterface
    {
    public:
        FileWriter(const std::string logFileName = LOG_FILE_NAME)
        {
            fp = new std::ofstream();
            fp->open(LOG_FILE_NAME);
            set(fp);
        }
        ~FileWriter()
        {
            if (fp != nullptr)
            {
                fp->close();
                delete fp;
            }
            fp = nullptr;
        }

    private:
        std::ofstream *fp = nullptr;
    };
}

#endif