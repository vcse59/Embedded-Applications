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
    m_SqlRemoteIP       = "192.168.6.230";
    m_DbUserName        = "test";
    m_DbUserPassword    = "Test@143";
    m_TableName         = "UserData";
    m_AccessTokenTable  = "AccessTokenTable";
    m_UserTableName     = "UserTable";
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
    // Connect to MySQL server
    COMMON_DEFINITIONS::eSTATUS status = connectToDBService();

    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS){
        LOGGER(m_logger) << "Connected to MySQL" << std::endl;
    }
    else{
        LOGGER(m_logger) << "Unable to connect to Mysql server on -> " << m_SqlRemoteIP << " at port -> " << m_RemotePort << std::endl;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    // Create database
    status = createDataBase(m_DbName);

    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        LOGGER(m_logger) << "DB " << m_DbName << " Created Successfully" << std::endl;
    }
    else if (status == COMMON_DEFINITIONS::eSTATUS::EXISTS)
    {
        LOGGER(m_logger) << "DB " << m_DbName << " already exists" << std::endl;
    }
    else
    {
        LOGGER(m_logger) << "DB Creation failed" << std::endl;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    
    // Define table name
    std::string tableName = m_TableName;

    // Create sample table
    string createTableQuery = "CREATE TABLE " + tableName + " ("
                                                            "USERID VARCHAR(50),"
                                                            "SESSIONID  VARCHAR(50),"
                                                            "HTTPMETHOD VARCHAR(10),"
                                                            "RESOURCEURL    LONGTEXT,"
                                                            "DATARAW LONGTEXT,"
                                                            "HTTPERROR LONGTEXT,"
                                                            "TIME TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                                                            ")";

    status = createTable(tableName, createTableQuery);
    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        LOGGER(m_logger) << "Table " << tableName << " created successfully " << std::endl;
    }
    else if (status == COMMON_DEFINITIONS::eSTATUS::EXISTS)
    {
        LOGGER(m_logger) << "Table " << tableName << " already exists " << std::endl;
    }
    else{
        LOGGER(m_logger) << "Unable to create table " << tableName << std::endl;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    // Create Access Token table
    tableName = m_AccessTokenTable;
    createTableQuery = "CREATE TABLE " + tableName + " ("
                                                     "USERID INT AUTO_INCREMENT PRIMARY KEY,"
                                                     "SESSIONID  VARCHAR(50),"
                                                     "ACCESSTOKENID VARCHAR(50)"
                                                     ")";
    status = createTable(tableName, createTableQuery);
    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        LOGGER(m_logger) << "Table " << tableName << " created successfully " << std::endl;
    }
    else if (status == COMMON_DEFINITIONS::eSTATUS::EXISTS)
    {
        LOGGER(m_logger) << "Table " << tableName << " already exists " << std::endl;
    }
    else
    {
        LOGGER(m_logger) << "Unable to create table " << tableName << std::endl;
        return COMMON_DEFINITIONS::eSTATUS::ERROR;
    }

    // Create user account table
    tableName = m_UserTableName;
    createTableQuery = "CREATE TABLE " + tableName + " ("
                                                     "USERID    VARCHAR(50) PRIMARY KEY,"
                                                     "USERNAME VARCHAR(50),"
                                                     "PASSWORD VARCHAR(50)"
                                                     ")";
    status = createTable(tableName, createTableQuery);
    if (status == COMMON_DEFINITIONS::eSTATUS::SUCCESS)
    {
        LOGGER(m_logger) << "Table " << tableName << " created successfully " << std::endl;
    }
    else if (status == COMMON_DEFINITIONS::eSTATUS::EXISTS)
    {
        LOGGER(m_logger) << "Table " << tableName << " already exists " << std::endl;
    }
    else
    {
        LOGGER(m_logger) << "Unable to create table " << tableName << std::endl;
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
    COMMON_DEFINITIONS::eSTATUS status = COMMON_DEFINITIONS::eSTATUS::SUCCESS;
    try
    {
        status = isDataBaseExists(dbName);
        
        if (status == COMMON_DEFINITIONS::eSTATUS::ERROR)
            throw sql::SQLException("Error in communicating with DB " + dbName);
        
        if (status == COMMON_DEFINITIONS::eSTATUS::EXISTS)
        {
            m_DbName = dbName;
        }

        if (status == COMMON_DEFINITIONS::eSTATUS::NOT_FOUND){
            sql::SQLString statement = sql::SQLString("CREATE DATABASE ");
            statement += sql::SQLString(dbName);
            m_PrepStatement = m_DBconnection->prepareStatement(statement);
            m_PrepStatement->execute();
            status = COMMON_DEFINITIONS::eSTATUS::SUCCESS;
        }
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

    return status;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::createTable(std::string tableName, std::string createTableQuery)
{
    COMMON_DEFINITIONS::eSTATUS status = COMMON_DEFINITIONS::eSTATUS::SUCCESS;

    status = executeQuery(tableName, createTableQuery, COMMON_DEFINITIONS::eQUERY_TYPE::DATA_DEFINITION);
    return status;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::executeQuery(std::string tableName, 
                                                        std::string queryString,
                                                        COMMON_DEFINITIONS::eQUERY_TYPE queryType)
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    COMMON_DEFINITIONS::eSTATUS status = COMMON_DEFINITIONS::eSTATUS::SUCCESS;
    try
    {
        status = isTableExists(tableName);
        if (status == COMMON_DEFINITIONS::eSTATUS::EXISTS)
        {
            LOGGER(m_logger) << tableName << " exists exists in " << m_DbName << endl;
            return status;
        }

        switch(queryType){
            case    COMMON_DEFINITIONS::eQUERY_TYPE::DATA_DEFINITION:
                status = processDDLQuery(queryString);
                break;
             case    COMMON_DEFINITIONS::eQUERY_TYPE::DATA_MANIPULATION:
                status = processDMLQuery(queryString);
                break;
            case    COMMON_DEFINITIONS::eQUERY_TYPE::DATA_QUERY:
                status = processDQLQuery(queryString);
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

    return status;
}

COMMON_DEFINITIONS::eSTATUS MySQLConnector::processDDLQuery(std::string queryString)
{
    //LOGGER(m_logger) << queryString << std::endl;
    sql::Statement* createStatement = m_DBconnection->createStatement();
    try
    {
        createStatement->execute("USE " + m_DbName);

        bool isSuccess = createStatement->execute(queryString);

        if (isSuccess == true)
            LOGGER(m_logger) << "Query String " << queryString << " is executed successfully" << endl;
    }
    catch (sql::SQLException &e)
    {
        delete createStatement;
        if (e.getErrorCode() == 1050)
            return COMMON_DEFINITIONS::eSTATUS::EXISTS;

        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
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
    COMMON_DEFINITIONS::eSTATUS status = COMMON_DEFINITIONS::eSTATUS::SUCCESS;
    try
    {    
        // Execute the SQL query to check if the database exists
        resultSet = createStatement->executeQuery("SELECT SCHEMA_NAME FROM INFORMATION_SCHEMA.SCHEMATA WHERE SCHEMA_NAME = \"" + dbName + "\"");

        // Check if a row is returned (database exists)
        if (! (resultSet->next()))
            status = COMMON_DEFINITIONS::eSTATUS::NOT_FOUND;
        else
            status = COMMON_DEFINITIONS::eSTATUS::EXISTS;

        delete resultSet;
        delete createStatement;
        return status;
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
        status = COMMON_DEFINITIONS::eSTATUS::ERROR;
        return status;
    }

    if (resultSet != nullptr)
        delete resultSet;
    delete createStatement;
    return status;
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
            return COMMON_DEFINITIONS::eSTATUS::EXISTS;
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
    std::string fetchQuery = "SELECT ACCESSTOKENID from " + m_AccessTokenTable;

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
            accessToken.insert("Bearer " + resultSet->getString("ACCESSTOKENID"));
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
