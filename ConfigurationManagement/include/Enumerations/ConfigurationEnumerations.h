#ifndef __CONFIGURATION_ENUMERATIONS_H__
#define __CONFIGURATION_ENUMERATIONS_H__

#include <memory>
#include <iostream>

namespace Configuration
{
    enum CONFIGURATION_STATUS
    {
        CONFIGURATION_READ_PARSING_FAILED = 0,
        CONFIGURATION_READ_PARSING_SUCCESS = 1
    };
}

#endif