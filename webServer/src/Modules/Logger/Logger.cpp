//
//  CLoggerStream.cpp
//  ThreadProgramming
//
//  Created by Vivek Yadav on 11/11/19.
//  Copyright © 2019 Vivek Yadav. All rights reserved.
//

#include <iostream>
#include <ctime>
#include "Modules/Logger/Logger.h"
#include "Modules/Logger/LogLevel.h"

using namespace LOGGER_SERVICE;

LoggerStream::LoggerStream(eLOG_LEVEL_ENUM    pAppLogLevel)
	:std::ostream(&buffer_), buffer_(pAppLogLevel)
{
}

LoggerStream::~LoggerStream()
{
    flush();
}