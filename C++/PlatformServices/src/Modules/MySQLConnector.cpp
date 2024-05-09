#include <iostream>
#include <chrono>
#include <thread>
#include "Modules/MySQLConnector.h"

using namespace DATABASE_SERVICE;
using namespace std;

MySQLConnector::MySQLConnector(LOGGER_SERVICE::S_PTR_LOGGER logger):m_logger(logger)
{
}

MySQLConnector::~MySQLConnector()
{
    if (m_PrepStatement != nullptr)
    {
        delete m_PrepStatement;
    }
    m_PrepStatement = nullptr;
    if (m_DBconnection != nullptr)
    {
        m_DBconnection->close();
        delete m_DBconnection;
    }
    m_DBconnection = nullptr;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::connectToDBService()
{
    try
    {
        /* Create a connection */
        m_DBdriver = get_driver_instance();
        m_DBconnection = m_DBdriver->connect("tcp://192.168.6.26:3306", "test", "Test@143");
    }
    catch(sql::SQLException& e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }
    
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::createDataBase(std::string dbName)
{
    try
    {
        if (isDataBaseExists(dbName) != COMMON_DEFINITIONS::eSTATUS::NOT_FOUND)
        {
            cout << "Database " << dbName << " exists" << endl;
            m_DbName = dbName;
            return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
        }

        sql::SQLString statement = sql::SQLString("CREATE DATABASE ");
        statement += sql::SQLString(dbName);
        m_PrepStatement = m_DBconnection->prepareStatement(statement);
        m_PrepStatement->execute();
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        delete m_PrepStatement;
        m_PrepStatement = nullptr;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    m_DbName = dbName;
    delete m_PrepStatement;
    m_PrepStatement = nullptr;

    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::createTable(std::string tableName)
{
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::executeQuery(std::string tableName, 
                                                        std::string queryString,
                                                        COMMON_DEFINITIONS::eQUERY_TYPE queryType)
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    try
    {
        if (isTableExists(tableName) != COMMON_DEFINITIONS::eSTATUS::NOT_FOUND)
        {
            cout << tableName << " doesn't exists in " << m_DbName << endl;
            return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
        }

        switch(queryType){
            case    COMMON_DEFINITIONS::eQUERY_TYPE::DATA_DEFINITION:
                processDDLQuery(queryString);
                break;
             case    COMMON_DEFINITIONS::eQUERY_TYPE::DATA_MANIPULATION:
                processDMLQuery(queryString);
                break;
            case    COMMON_DEFINITIONS::eQUERY_TYPE::DATA_QUERY:
                processDQLQuery(queryString);
                break;
            case    COMMON_DEFINITIONS::eQUERY_TYPE::UNKNOWN_QUERY:
                throw sql::SQLException("Invalid Query type enumeration");
        }
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::processDDLQuery(std::string queryString)
{
    (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << queryString << std::endl;
    sql::Statement* createStatement = m_DBconnection->createStatement();
    try
    {
        createStatement->execute("USE " + m_DbName);

        bool isSuccess = createStatement->execute(queryString);

        if (isSuccess == true)
            cout << "Query String " << queryString << " is executed successfully" << endl;
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        delete createStatement;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    delete createStatement;
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::processDMLQuery(std::string queryString)
{
    sql::Statement* createStatement = m_DBconnection->createStatement();
    try
    {
        (*m_logger)(LOGGER_SERVICE::eLOG_LEVEL_ENUM::DEBUG_LOG) << "Query string : " << queryString << std::endl;
        createStatement->execute("USE " + m_DbName);

        int affectedRows = createStatement->executeUpdate(queryString);

        if (affectedRows > 0) {
            cout << "Rows inserted: " << affectedRows << endl;
        } else {
            cout << "Error inserting rows." << endl;
        }

    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        delete createStatement;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    delete createStatement;
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::processDQLQuery(std::string queryString)
{
    sql::Statement* createStatement = m_DBconnection->createStatement();
    sql::ResultSet* resultSet = nullptr;
    try
    {
        createStatement->execute("USE " + m_DbName);

        resultSet = createStatement->executeQuery(queryString);

        // Get the number of columns in the result set
        int numColumns = resultSet->getMetaData()->getColumnCount();
        
        // Process the ResultSet
        while (resultSet->next()) {
            // Iterate over each column in the current row
            for (int i = 1; i <= numColumns; ++i) {
                // Retrieve data from the current row and column
                cout << resultSet->getString(i) << "\t"; // You can use appropriate getXXX() methods based on the column data type
            }
            cout << endl; // Move to the next line for the next row
        }

    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        if (resultSet != nullptr)
            delete resultSet;
        delete createStatement;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    if (resultSet != nullptr)
        delete resultSet;
    delete createStatement;
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::isDataBaseExists(std::string dbName)
{
    sql::Statement* createStatement = m_DBconnection->createStatement();
    sql::ResultSet* resultSet = nullptr;
    try
    {    
        // Execute the SQL query to check if the database exists
        resultSet = createStatement->executeQuery("SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = \"" + dbName + "\"");

        // Check if a row is returned (database exists)
        if (! (resultSet->next())) {
            delete resultSet;
            delete createStatement; 
            return COMMON_DEFINITIONS::eSTATUS::NOT_FOUND;
        }
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        if (resultSet != nullptr)
            delete resultSet;
        delete createStatement;        
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    if (resultSet != nullptr)
        delete resultSet;
    delete createStatement;
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::isTableExists(std::string tableName)
{
    sql::Statement* createStatement = m_DBconnection->createStatement();
    sql::ResultSet* resultSet = nullptr;
    try
    {    
        // Execute the SQL query to check if the database exists
        resultSet = createStatement->executeQuery("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_SCHEMA = \"" + tableName + "\"");

        // Check if a row is returned (table exists)
        if (resultSet->next()) {
            delete resultSet;
            delete createStatement; 
            return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
        }else{
            delete resultSet;
            delete createStatement; 
            return COMMON_DEFINITIONS::eSTATUS::NOT_FOUND;
        }
    }
    catch (sql::SQLException &e)
    {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
        if (resultSet != nullptr)
            delete resultSet;
        delete createStatement;        
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    if (resultSet != nullptr)
            delete resultSet;
        delete createStatement;        
    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}
