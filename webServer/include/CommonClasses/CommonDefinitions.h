/**
 * @file CommonDefintions.h
 * @brief Header file for the CommonDefintions class
 *
 * This file contains the declaration of the CommonDefinitions class,
 * which demonstrates defining all the common variables
 *
 * Contact: Vivek Yadav <v.cse59@gmail.com>
 *
 * @author Vivek Yadav
 * @date 2024-06-12
 */

#ifndef __COMMON_DEFINITIONS_H__
#define __COMMON_DEFINITIONS_H__

//Include system header files
#include <cstddef>
#include <iostream>
#include <chrono>
#include <cstring>
#include <memory>

// Pre-Processor Directive
#define MAX_CONNECTIONS  1024
#define DB_TABLE_BUFFER_SIZE 128
#define STRING_SIZE    10 * 1024

#define MAX_BUFFER_SIZE (STRING_SIZE + 1024)

#define MAX_HEADER_SIZE 2048
#define MAX_BODY_SIZE   8192
#define SERVER_LISTENDER_TIMEOUT_IN_MS 1
#define BATCH_SIZE 1000

namespace COMMON_DEFINITIONS
{
    // Status Enumeration
    enum eSTATUS
    {
        UNKNOWN                         = 0,
        SOCKET_CREATED                  = 1,
        SOCKET_CONNECTED                = 2,
        SOCKET_INITIALIZATION_FAILED    = 3,
        SUCCESS                         = 4,
        ERROR                           = 5,
        NOT_FOUND                       = 6,
        EXISTS                          = 7,
        SOCKET_CONNECTION_IN_PROGRESS   = 8
    };

    enum eQUERY_TYPE
    {
        UNKNOWN_QUERY       = 0,
        DATA_QUERY          = 1,
        DATA_MANIPULATION   = 2,
        DATA_DEFINITION     = 3
    };
    
    enum eHTTP_SESSION_STATUS
    {
        UNKNOWN_SESSION     = 0,
        SESSION_ADDED       = 1,
        SESSION_EXISTS      = 2,
        SESSION_EXPIRED     = 3,
        SESSION_DELETED     = 4,
        SESSION_VALID       = 5
    };

    constexpr unsigned int  WEB_SERVER_PORT     = <WEB_SERVER_PORT>; // 9999
    constexpr const char    LOG_SERVER_IP[]     = <LOG_SERVER_IP>;
    constexpr unsigned int  LOG_SERVER_PORT     = <LOG_SERVER_PORT>; //10000;
    constexpr const char    LOG_FILE_NAME[]     = "applicationLog.txt";
    constexpr const char    MYSQL_SERVER_IP[]   = <MYSQL_SERVER_IP>;
    constexpr unsigned int  MYSQL_SERVER_PORT   = <MYSQL_SERVER_PORT>; //3306;
    constexpr const char    MYSQL_USERNAME[]    = <MYSQL_USERNAME>;
    constexpr const char    MYSQL_PASSWORD[]    = <MYSQL_PASSWORD>;
}
#endif