//
//  CLogLevel.cpp
//  ThreadProgramming
//
//  Created by Vivek Yadav on 11/19/19.
//  Copyright © 2019 Vivek Yadav. All rights reserved.
//

#include <iostream>
#include "Modules/Logger/LogLevel.h"

using namespace LOGGER_SERVICE;

static std::unordered_map<LOGGER_SERVICE::eLOG_LEVEL_ENUM, std::string> LOG_LEVEL_STRING = {
    {LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG, "DEBUG"}, 
    {LOGGER_SERVICE::eLOG_LEVEL_ENUM::ERROR_LOG, "ERROR"}, 
    {LOGGER_SERVICE::eLOG_LEVEL_ENUM::INFO_LOG, "INFO"}, 
    {LOGGER_SERVICE::eLOG_LEVEL_ENUM::WARNING_LOG, "WARN"},
    {LOGGER_SERVICE::eLOG_LEVEL_ENUM::NONE, "UNKNOWN"},
};

std::string LogLevel::getLogLevelString(eLOG_LEVEL_ENUM pLogLevel)
{
    // Handle the scenario where LOG_LEVEL_STRING map is deallocated
    std::string defaultLogName = "DEBUG";
    if (LOG_LEVEL_STRING.size() == 0)
        return defaultLogName;
    auto it = LOG_LEVEL_STRING.find(pLogLevel);
    if (it == LOG_LEVEL_STRING.end())
        return defaultLogName;
    return it->second;
}