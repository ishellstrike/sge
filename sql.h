#ifndef SQL_H
#define SQL_H
#include "logger.h"
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

class SQL
{
public:
    SQL();
    ~SQL();

    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;

    void commit(const std::string &s);
};

#endif // SQL_H
