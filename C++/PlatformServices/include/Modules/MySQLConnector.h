#ifndef __MY_SQL_CONNECTOR_H__
#define __MY_SQL_CONNECTOR_H__

#include <string>
#include <map>
#include <mutex>
#include <unordered_set>

/*
   Include directly the different
   headers from cppconn/ and mysql_driver.h + mysql_util.h
   (and mysql_connection.h). This will reduce your build time!
   */
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>

#include "CommonClasses/CommonDefinitions.h"
#include "Interfaces/DataBaseConnectorInterface.h"
#include "Modules/Logger/Logger.h"

namespace DATABASE_SERVICE
{
    class MySQLConnector : public DATABASE_SERVICE::DataBaseConnectorInterface
    {
        public:
            MySQLConnector(LOGGER_SERVICE::S_PTR_LOGGER logger);
            ~MySQLConnector();

            COMMON_DEFINITIONS::eSTATUS initializeDB() override;
            COMMON_DEFINITIONS::eSTATUS executeQuery(std::string tableName,
                                                    std::string queryString,
                                                    COMMON_DEFINITIONS::eQUERY_TYPE qyeryType) override;
            COMMON_DEFINITIONS::eSTATUS loadAccessToken(std::unordered_set<std::string>& accessToken) override;

        private:

            COMMON_DEFINITIONS::eSTATUS connectToDBService();
            COMMON_DEFINITIONS::eSTATUS createDataBase(std::string dbName);
            COMMON_DEFINITIONS::eSTATUS CreateAccessToKenTable();
            COMMON_DEFINITIONS::eSTATUS processDDLQuery(std::string queryString);
            COMMON_DEFINITIONS::eSTATUS processDMLQuery(std::string queryString);
            COMMON_DEFINITIONS::eSTATUS processDQLQuery(std::string queryString);
            COMMON_DEFINITIONS::eSTATUS isDataBaseExists(std::string dbName);
            COMMON_DEFINITIONS::eSTATUS isTableExists(std::string tableName);

            sql::Driver *m_DBdriver = nullptr;
            sql::Connection *m_DBconnection = nullptr;
            sql::PreparedStatement *m_PrepStatement = nullptr;
            std::string m_DbName;
            std::string m_SqlRemoteIP;
            std::string m_TableName;
            unsigned int m_RemotePort;
            std::string m_DbUserName;
            std::string m_DbUserPassword;
            std::string m_AccessTokenTable;
            LOGGER_SERVICE::S_PTR_LOGGER m_logger;
            std::mutex  m_Mutex;
            std::unordered_set<std::string> m_AccessTokenInfo;

            MySQLConnector(const MySQLConnector&) = delete;
            MySQLConnector& operator=(const MySQLConnector&) = delete;
            MySQLConnector(const MySQLConnector&&) = delete;
            MySQLConnector& operator=(const MySQLConnector&&) = delete;
    };
}

#endif