/*
 *                  TCP/IP
 *
 *   Chris Thorlaksson Nov 2019.
 *
 *   TCP/IP server 
 *   Listen for a tcp/ip connection on port nr 8080.
 *   Forks a child process to handle each request.
 *   Server accepts multiple connections
 *
 */


#include "../header/mysql.h"
#include "../header/socket.h"

using namespace std;
 
int main(int argc, char *argv[])
{

  /* Create socket object */
  Socket conn;

  switch (fork())
    {
    case -1:
        perror(argv[0]);
        exit(1);
    case 0:         
      /* Initiate Server Process on socket. */
      conn.server();
    }
  /* Exit the Parent Process  */
  exit(0);        
}











