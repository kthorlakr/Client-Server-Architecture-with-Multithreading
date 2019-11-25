#ifndef SOCKET_H
#define SOCKET_H

#include <stdlib.h>

class Socket
{
  public:
    Socket();
    int Client(char *address, char *port);
  protected:
  private:
    /* debug */
    int DEBUG = 0;

};

#endif
