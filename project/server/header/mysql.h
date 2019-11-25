#ifndef MYSQL_H
#define MYSQL_H

#include <stdlib.h>
#include <time.h>
#include <string>

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

using namespace std;

class MysqlConnect
{
  public:
    MysqlConnect();
    void Connect();
    void Retrieve(char *table, char *field, char *val);
    void CloseConnection();
    void Insert(char *table, char *field, char *val);
    void Update(char *table, char *field, char *val, char *update_field, char *update_val);
    void SetActiveCount();
    int GetActiveCount();
    void SetTableName(char *tables);
    char* GetTableName();
  protected:
  private:
    //std::string tableName = "";
    char table[20];
    int active;
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt;
    sql::ResultSet *res;
    
};

#endif
