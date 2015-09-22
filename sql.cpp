#include "sql.h"

SQL::SQL()
{
    try {
        //driver = get_driver_instance();
        //con = driver->connect("tcp://127.0.0.1:3306", "root", "admin");
        //con->setSchema("test");
    }
    catch ( sql::SQLException &e )
    {
        LOG(fatal) << "SQL specific fatal: " << e.getSQLStateCStr();
    }
    catch ( std::exception &e )
    {
        LOG(fatal) << "SQL std fatal: " << e.what();
    }
    catch ( ... )
    {
        LOG(fatal) << "SQL unknown fatal";
    }

    LOG(verbose) << "sql connected";
}

SQL::~SQL()
{
    LOG(verbose) << "sql bye";
    //con->close();
}

void SQL::commit(const std::string &s)
{

}

