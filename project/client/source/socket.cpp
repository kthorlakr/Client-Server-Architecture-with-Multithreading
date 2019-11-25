/*
 *                  Socket Class
 *
 *   Chris Thorlaksson Nov 2019.
 *
 *   Socket Class
 *   
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <assert.h>
#include "../header/socket.h"

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


#define MAXDATASIZE         510
#define BUFFSIZ         1000000

using namespace std;


Socket::Socket()
{
  ;
}


int Socket::Client(char *address, char *port)
{

  int sockfd, numbytes;  
  char cmd[BUFFSIZ + 1], rbuf[BUFFSIZ + 1], line[BUFFSIZ +1];
  struct hostent *he;
  struct sockaddr_in their_addr; // connector's address information 
  FILE *in = stdin;
  int end = 0;
  char *chkbuf;
  char HOST[128];
  int PORT;

  memset(HOST, 0, sizeof(HOST));
  strncpy(HOST, address, sizeof(HOST)-1);
  PORT = atoi(port);

  setbuf(stdout, NULL);
  setbuf(stderr, NULL);


  if ((he = gethostbyname(HOST)) == NULL) {  // get the host info 
    perror("gethostbyname");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  their_addr.sin_family = AF_INET;    // host byte order 
  their_addr.sin_port = htons(PORT);  // short, network byte order 
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  memset(their_addr.sin_zero, 0, sizeof(their_addr.sin_zero));

  if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(their_addr)) == -1){
    perror("connect");
    exit(1);
  }

  strcpy(cmd, "Testing");
  strcat(cmd, "\n");

  if (DEBUG) printf("Sending:\n%s\n", cmd);
  send(sockfd, cmd, MAXDATASIZE, 0);

  if ((numbytes = recv(sockfd, cmd, MAXDATASIZE, 0)) < 0){
    perror("recv");
    exit(1);
  }
  cmd[numbytes] = '\0';
  if (DEBUG) printf("Received: %s",cmd);

    close(sockfd);
    if (DEBUG) printf("\nSocket closed. Finished.\n");
    return 0;
}



