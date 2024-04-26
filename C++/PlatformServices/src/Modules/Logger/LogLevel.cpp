//
//  CLogLevel.cpp
//  ThreadProgramming
//
//  Created by Vivek Yadav on 11/19/19.
//  Copyright © 2019 Vivek Yadav. All rights reserved.
//

#include <stdio.h>
#include "Modules/Logger/LogLevel.h"

using namespace LOGGER_SERVICE;

std::string LOG_LEVEL_STRING[] = {
    "ERROR  ",
    "WARNING",
    "INFO   ",
    "DEBUG  "
};

std::string LogLevel::getLogLevelString(eLOG_LEVEL_ENUM pLogLevel)
{
    return LOG_LEVEL_STRING[pLogLevel];
}