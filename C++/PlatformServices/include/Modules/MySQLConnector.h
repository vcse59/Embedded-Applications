#ifndef __MY_SQL_CONNECTOR_H__
#define __MY_SQL_CONNECTOR_H__

#include <string>
#include <map>

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

namespace DATABASE_SERVICE
{
    class MySQLConnector : public DATABASE_SERVICE::DataBaseConnectorInterface
    {
        public:
            MySQLConnector();
            ~MySQLConnector();

            COMMON_DEFINITIONS::eSTATUS connectToDBService() override;
            COMMON_DEFINITIONS::eSTATUS createDataBase(std::string dbName) override;
            COMMON_DEFINITIONS::eSTATUS createTable(std::string tableName) override;
            COMMON_DEFINITIONS::eSTATUS executeQuery(std::string tableName,
                                                    std::string queryString,
                                                    COMMON_DEFINITIONS::eQUERY_TYPE qyeryType) override;

            // COMMON_DEFINITIONS::eSTATUS dropTable(std::string tableName);

            //COMMON_DEFINITIONS::eSTATUS switchDatabase(std::string dbName);

            //COMMON_DEFINITIONS::eSTATUS insertRecord();
            //COMMON_DEFINITIONS::eSTATUS DeleteRecord();
            //COMMON_DEFINITIONS::eSTATUS UpdateRecord();
            // TODO - Fetch record

        private:

            COMMON_DEFINITIONS::eSTATUS processDDLQuery(std::string queryString);
            COMMON_DEFINITIONS::eSTATUS processDMLQuery(std::string queryString);
            COMMON_DEFINITIONS::eSTATUS processDQLQuery(std::string queryString);
            COMMON_DEFINITIONS::eSTATUS isDataBaseExists(std::string dbName);
            COMMON_DEFINITIONS::eSTATUS isTableExists(std::string tableName);

            sql::Driver *m_DBdriver = NULL;
            sql::Connection *m_DBconnection = NULL;
            sql::PreparedStatement *m_PrepStatement = NULL;
            std::string m_DbName;
    };
}

#endif