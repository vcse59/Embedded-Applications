#ifndef __DATA_PARSER_INTERFACE_H__
#define __DATA_PARSER_INTERFACE_H__

#include "CommonClasses/CommonDefinitions.h"

namespace PARSER_INTERFACE{
    class DataParserInterface{
        public:
            virtual ~DataParserInterface() {}
            virtual void serialize() = 0;
            virtual COMMON_DEFINITIONS::eSTATUS deserialize() = 0;
            virtual std::string dumpData() = 0;
    };
    typedef std::shared_ptr<PARSER_INTERFACE::DataParserInterface> S_PTR_DATA_PARSER_INTERFACE;
}

#endif