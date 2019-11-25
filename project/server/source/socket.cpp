/*
 *                  Socket Class
 *
 *   Chris Thorlaksson Nov 2019.
 *
 *   Socket Class
 *   
 *
 */

#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include "../header/mysql.h"
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

#define PORT 8080
#define MAX_BLOB_SIZE 300000

using namespace std;


Socket::Socket()
{
  ;
}


void Socket::termfunc(int i)
{
  i=0;
  close(s);
  close(ls);
  exit(0);
}

int Socket::find_pid()
{
  int pid_val;
  pid_val=getpid();
  if(pid_val<0)
  {
    exit(-1);
  }
  return(pid_val);
}



void Socket::soc_setup(char *cl_ip_add)
{
  int addrlen,fork_ret;
  /* set the socket */
  struct linger linger;           
  /* pointer service information */
  struct servent *sp;             

  /* socket */
  struct sockaddr_in myaddr_in;   
  /* socket */
  struct sockaddr_in peeraddr_in; 

  addrlen = sizeof(struct sockaddr_in);

  /* Clear values */
  memset ((char *)&myaddr_in  , 0, addrlen);
  memset ((char *)&peeraddr_in, 0, addrlen);

  /* Internet Protocol v4 addresses */
  myaddr_in.sin_family = AF_INET;

  /* listen on address */
  myaddr_in.sin_addr.s_addr = INADDR_ANY;

  /* set the port. */
  myaddr_in.sin_port = htons( PORT );

  /* Open socket to listen on the stream */
  ls = socket (AF_INET, SOCK_STREAM, 0);
  if (ls == -1)
  {
    exit(1);
  }

  /* Bind the listener to the socket */
  if (bind(ls, (struct sockaddr *) &myaddr_in, addrlen) == -1)
  {
     exit(1);
  }

  /* listen on socket. Max 5 connections*/
  if (listen(ls, 5) == -1)
  {
    exit(1);
  }

  /* set proces sin background */
  setpgrp();
  while(1==1)
  {
    /* accept new socket */
    s = accept(ls, (struct sockaddr *)&peeraddr_in, (socklen_t*)&addrlen);
    sprintf(cl_ip_add,"%s", inet_ntoa(peeraddr_in.sin_addr));
  
    if ( s == -1) exit(1);

    fork_ret=fork();

    /* Error on fork */
    if (fork_ret == -1)
    {
      close(s);
      exit(-1);
    }

    /* Child process */
    else if (fork_ret == 0)
    {
      /* close the socket from the parent */
      close(ls);
      /* find new pid id for child  */
      pid=find_pid();

      /* Socket close options */
      linger.l_onoff  =1;
      linger.l_linger =0;
      if (setsockopt(s, SOL_SOCKET, SO_LINGER, &linger, sizeof(linger)) == -1)
      {
        close(s);
        exit(1);
      }
      break;
    }else
    /*  Parent process */
    {
      close(s);
    }
  }
}





int Socket::sendsocket(int send_soc, char *send_buff)
{
  int len=0;
  len=strlen(send_buff);
  send(send_soc, send_buff, len, 0);
  return(0);
}


/********************************************************************
 *                              S E R V E R
 *
 */
int Socket::server()
{
  printf("hello0");  
  char client_ip_addr[20];
  /* response string */
  char out_text[MAX_BLOB_SIZE];
  memset (client_ip_addr , 0, 20);
  
  /* prevent zombie parent process */
  signal(SIGCLD,SIG_IGN);
  pid=find_pid();
  /* Parent waits for new connection */
  soc_setup(client_ip_addr);  
  char field[20] = "PID";
  char update_field[20] = "";
  char update_val[20] = "";
  char val[20] = "";
  char pid[10] = "";
  //find PID
  sprintf(pid, "%d",find_pid());
  
  MysqlConnect sql;
  char table[13] = "insert_table";

  sql.SetTableName(table);

  char *tableName;
  //insert into table
  tableName = sql.GetTableName();
  memset(field, 0, sizeof(field));
  sprintf(field, "%s", "PID");
  sql.Connect();
  sql.Insert(tableName,field,pid);
  //Make it active.
  //Make PID active
  memset(field, 0, sizeof(field));
  memset(val, 0, sizeof(val));
  sprintf(field, "%s", "active");
  sprintf(val, "%s", "YES");
  sprintf(update_field, "%s", "PID");
  sql.SetActiveCount();
  sql.Connect();
  sql.Update(tableName, field, val, update_field, pid);
  //Make PID inactive
  memset(val, 0, sizeof(val));
  sprintf(val, "%s", "NO");
  sql.Connect();
  sql.Update(tableName, field, val, update_field, pid);
  //Make PID finish
  memset(field, 0, sizeof(field));
  sprintf(field, "%s", "finished");
  memset(val, 0, sizeof(val));
  sprintf(val, "%s", "YES");
  sql.Connect();
  sql.Update(tableName, field, val, update_field, pid);

  strcpy(out_text,"Process Complete.");
  /* Send client response on socket */
  sendsocket(s, out_text);

  /* close socket */
  close(s);
  /* close parent server */
  return(0);
}
