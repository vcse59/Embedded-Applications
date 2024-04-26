//
//  CLogLevel.h
//  ThreadProgramming
//
//  Created by Vivek Yadav on 11/12/19.
//  Copyright © 2019 Vivek Yadav. All rights reserved.
//

#ifndef __LOG_LEVEL_H__
#define __LOG_LEVEL_H__

#include <stdio.h>
#include <string>

namespace LOGGER_SERVICE
{
	enum eLOG_LEVEL_ENUM
	{
	    NONE         =   0,
	    ERROR_LOG    =   1,
	    WARNING_LOG  =   2,
	    INFO_LOG     =   3,
	    DEBUG_LOG    =   4
	};

	class LogLevel
	{
	    public:
		std::string getLogLevelString(LOGGER_SERVICE::eLOG_LEVEL_ENUM pLogLevel);
	};
}
#endif /* LogLevel_h */