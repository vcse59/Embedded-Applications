#include <iostream>
#include <chrono>
#include <thread>
#include "Modules/MySQLConnector.h"

using namespace DATABASE_SERVICE;
using namespace std;

MySQLConnector::MySQLConnector(LOGGER_SERVICE::S_PTR_LOGGER logger):m_logger(logger)
{
    m_DbName            = "Test1";
    m_RemotePort        = 3306;
    m_SqlRemoteIP       = "127.0.0.1";
    m_TableName         = "sampleTable";
    m_DbUserName        = "<CONFIGURE_USERNAME>";
    m_DbUserPassword    = "<CONFIGURE_PASSWORD>";
    m_AccessTokenTable  = "AccessTokenTable";
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

COMMON_DEFINITIONS::eSTATUS MySQLConnector::initializeDB()
{
    COMMON_DEFINITIONS::eSTATUS status = connectToDBService();

    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS){
        LOGGER(m_logger) << "Connected to MySQL" << std::endl;
    }
    else{
        LOGGER(m_logger) << LOGGER_SERVICE::eLOG_LEVEL_ENUM::ERROR_LOG << "Not connected" << std::endl;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    status = createDataBase(m_DbName);

    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        LOGGER(m_logger) << "DB Created Successfully" << std::endl;
    }
    else
    {
        LOGGER(m_logger) << LOGGER_SERVICE::eLOG_LEVEL_ENUM::ERROR_LOG << "DB Creation failed" << std::endl;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    
    // Define table name
    std::string tableName = m_TableName;

    // Create table
    string createTableQuery = "CREATE TABLE " + tableName +" ("
                                "id INT AUTO_INCREMENT PRIMARY KEY,"
                                "SESSIONID VARCHAR(50),"
                                "HTTPMETHOD VARCHAR(10),"
                                "RESOURCEURL    LONGTEXT,"
                                "DATARAW LONGTEXT,"
                                "HTTPERROR LONGTEXT"
                              ")";

    status = executeQuery(tableName, createTableQuery, COMMON_DEFINITIONS::eQUERY_TYPE::DATA_DEFINITION);
    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        LOGGER(m_logger) << "Table created successfully" << std::endl;
    }
    else{
        LOGGER(m_logger) << LOGGER_SERVICE::eLOG_LEVEL_ENUM::ERROR_LOG << "Unable to create table" << std::endl;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    status = CreateAccessToKenTable();
    if (status != COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        LOGGER(*m_logger) << "Fail to create access token table";
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::connectToDBService()
{
    try
    {
        /* Create a connection */
        m_DBdriver = get_driver_instance();
        std::string remoteURL;
        remoteURL.append("tcp://");
        remoteURL.append(m_SqlRemoteIP);
        remoteURL.append(":");
        remoteURL.append(std::to_string(m_RemotePort));

        m_DBconnection = m_DBdriver->connect(remoteURL, m_DbUserName, m_DbUserPassword);
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

COMMON_DEFINITIONS::eSTATUS MySQLConnector::CreateAccessToKenTable()
{
    COMMON_DEFINITIONS::eSTATUS status = COMMON_DEFINITIONS::eSTATUS::UNKNOWN;
    // Define table name
    std::string tableName = m_AccessTokenTable;

    // Check if table already exists
    if (isTableExists(tableName) == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        LOGGER(m_logger) << tableName << " already exists in DB : " << m_DbName << std::endl;
        return COMMON_DEFINITIONS::eSTATUS::SUCCESS;
    }

    // Create table
    string createTableQuery = "CREATE TABLE " + tableName +" ("
                                "id INT AUTO_INCREMENT PRIMARY KEY,"
                                "AccessTokenID VARCHAR(50)"
                              ")";

    status = executeQuery(tableName, createTableQuery, COMMON_DEFINITIONS::eQUERY_TYPE::DATA_DEFINITION);
    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        LOGGER(m_logger) << "Table " << tableName << " created successfully" << std::endl;
    }
    else{
        LOGGER(m_logger) << LOGGER_SERVICE::eLOG_LEVEL_ENUM::ERROR_LOG << "Unable to create table " << tableName << std::endl;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

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
    LOGGER(m_logger) << queryString << std::endl;
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
        createStatement->execute("USE " + m_DbName);
        createStatement->executeUpdate(queryString);
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

COMMON_DEFINITIONS::eSTATUS MySQLConnector::loadAccessToken(std::unordered_set<std::string>& accessToken)
{
    std::string fetchQuery = "SELECT AccessTokenID from " + m_AccessTokenTable;

    // Clear the exisiting access Token
    accessToken.clear();

    sql::Statement* createStatement = m_DBconnection->createStatement();
    sql::ResultSet* resultSet = nullptr;
    try
    {    
        // Execute the SQL query to check if the database exists
        resultSet = createStatement->executeQuery(fetchQuery);

        // Population the access token info to unordered_set
        while (resultSet->next())
        {
            accessToken.insert("Bearer " + resultSet->getString("AccessTokenID"));
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
