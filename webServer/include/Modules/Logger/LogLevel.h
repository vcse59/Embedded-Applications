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
#include <unordered_map>

namespace LOGGER_SERVICE
{
	enum LOG_OUTPUT
	{
		LOG_OUTPUT_FILE = 0,
		LOG_OUTPUT_CONSOLE = 1,
		LOG_OUTPUT_REMOTE = 2
	};

	enum eLOG_LEVEL_ENUM
	{
	    NONE         =   0,
	    ERROR_LOG    =   NONE + 1,
	    WARNING_LOG  =   ERROR_LOG + 1,
	    INFO_LOG     =   WARNING_LOG + 1,
	    DEBUG_LOG    =   INFO_LOG + 1,
		ALL_LOG		 = 	 DEBUG_LOG + 1
	};

	class LogLevel
	{
	    public:
		std::string getLogLevelString(LOGGER_SERVICE::eLOG_LEVEL_ENUM pLogLevel);
	};
}
#endif /* LogLevel_h */