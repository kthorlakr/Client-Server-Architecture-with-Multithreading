#ifndef SOCKET_H
#define SOCKET_H

#include <stdlib.h>

class Socket
{
  public:
    Socket();
    void termfunc(int i);
    int find_pid();
    void soc_setup(char *cl_ip_add);
    int sendsocket(int send_soc, char *send_buff);
    int server();
  protected:
  private:
    /* sockect */
    int s;
    /* listen socket */
    int ls;
    /* Process ID */
    long pid;

};

#endif
