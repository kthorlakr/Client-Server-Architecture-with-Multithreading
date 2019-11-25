/*
 *                  MySql Class
 *
 *   Chris Thorlaksson Nov 2019.
 *
 *   MySql Class
 *   
 *
 */

#include <stdlib.h>
#include <iostream>
#include <cstring>
#include "../header/mysql.h"


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


MysqlConnect::MysqlConnect()
{
 ;
}

/*
 * Chris Thorlaksson
 *
 * insert into MySQL table
 */
void MysqlConnect::Insert(char *table, char *field, char *val)
{

  try {
  
    char insert_text[200];
    stmt = con->createStatement();
    sprintf(insert_text,"insert into %s (%s) values (%s)", table, field, val);
    stmt->execute(insert_text);

  } catch (sql::SQLException &e) {
    cout << "# ERR: SQLException";
    cout << "SQLState3: " << e.getSQLState() << " )" << endl;
  }
  
}



/*
 * Chris Thorlaksson
 *
 * update MySQL table
 */
void MysqlConnect::Update(char *table, char *field, char *val, char *update_field, char *update_val)
{

  try {

    char update_text[200];
    stmt = con->createStatement();                               //Thread1, finished, YES, PID, pid
    sprintf(update_text,"UPDATE %s SET %s = '%s' where %s = %s", table, field, val, update_field, update_val);
    stmt->executeUpdate(update_text);

  } catch (sql::SQLException &e) {
    cout << "# ERR: SQLException";
    cout << "SQLState1: " << e.getSQLState() << " )" << endl;
  }
}


/*
 * Chris Thorlaksson
 *
 * connect to MySQL database
 */
void MysqlConnect::Retrieve(char *table, char *field, char *val)
{

  try {
  
    stmt = con->createStatement();
    char select_text[200];
    sprintf(select_text,"select %s from %s where %s = %s", field, table, field, val);
    res = stmt->executeQuery(select_text);

    while (res->next()) {
      cout << "\t... MySQL replies: ";
      cout << res->getString(field) << endl;

    }
  
  } catch (sql::SQLException &e) {
    cout << "# ERR: SQLException";
    cout << "SQLState1: " << e.getSQLState() << " )" << endl;
  }

} 





/*
 * Chris Thorlaksson
 *
 * connect to MySQL database
 */
void MysqlConnect::Connect()
{

  try {
  
    /* Create a connection */
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", "root", "rootpasswordgiven");
    /* Connect to the MySQL test database */
    con->setSchema("SERVER_CLIENT_TABLES");


  } catch (sql::SQLException &e) {
    cout << "# ERR: SQLException";
    cout << "SQLState2: " << e.getSQLState() << " )" << endl;
  }

}

/*
 * Chris Thorlaksson
 *
 * Check activethreads open in table on MySQL database
 */
void MysqlConnect::SetActiveCount()
{

  try {
  
    stmt = con->createStatement();
    char count_text[200];

    sprintf(count_text,"SELECT COUNT(*) AS _totalActive from %s where active = 'YES' AND finished = 'NO';", this->GetTableName());
        
    res = stmt->executeQuery(count_text);

    while (res->next()) {
      cout << "\t... MySQL replies: ";
      cout << res->getString("_totalActive") << endl;
      std::string count = res->getString("_totalActive");
      const char* temp = count.c_str();
      this->active = atoi(temp);
    }
  
  } catch (sql::SQLException &e) {
    cout << "# ERR: SQLException";
    cout << "SQLState10: " << e.getSQLState() << " )" << endl;
  }
}



/*
 * Chris Thorlaksson
 *
 * Check activethreads open in table on MySQL database
 */
void MysqlConnect::SetTableName(char *tables)
{

  strcpy(this->table, tables);
}



/*
 * Chris Thorlaksson
 *
 * close connection to MySQL server
 */
void MysqlConnect::CloseConnection()
{
  try {
    delete this->res;
    delete this->stmt;
    delete this->con;
  } catch (sql::SQLException &e) {
    cout << "# ERR: SQLException";
    cout << "SQLState12: " << e.getSQLState() << " )" << endl;
  }
}





/*
 * Chris Thorlaksson
 *
 * Get object table name
 */
char* MysqlConnect::GetTableName()
{

  char *string = (char *) malloc(20);
  strcpy(string, this->table);
  return string;


}


/*
 * Chris Thorlaksson
 *
 * Get object table name
 */
int MysqlConnect::GetActiveCount()
{


cout << "These are active... ";
cout << "" << this->active << " )" << endl;
  return this->active;

}
