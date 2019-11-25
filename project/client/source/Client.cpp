#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <cstring>
#include <netdb.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include "../header/socket.h"
#define MAXDATASIZE         510
#define BUFFSIZ         1000000

struct sum_runner_struct {
        long long limit;
        long long answer;
};

/*
 * Chris Thorlaksson
 *
 * Establish client connection on individual threads
 */
void* thread_runner(void* arg)
{
  char address[128];
  char port[10];
  strncpy(address, "127.0.0.1", sizeof(address)-1);
  strncpy(port, "8080", sizeof(port)-1);
  Socket conn;
  conn.Client(address, port);

  pthread_exit(0);

}

/*
 * Chris Thorlaksson
 *
 * Initialize threads in main
 *
 */
int main(int argc, char **argv)
{
  Socket conn;

  char address[128];
  char port[10];

  if (argc != 3)
  {
    printf("Usage: <host> <port> Example: 127.0.0.1 8080\n");
    exit(1);
  }
  else
  {
    strncpy(address, argv[1], sizeof(address)-1);
    strncpy(port, argv[2], sizeof(port)-1);
  }

  int num_threads = 30;

  struct sum_runner_struct args[num_threads];
  // Launch thread
  pthread_t tids[num_threads];
  for (int i = 0; i < num_threads; i++) {
    args[i].limit = num_threads+1;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_create(&tids[i], &attr, thread_runner, &args[i]);
  }
  
  // Wait for thread thread to finish
  for (int i = 0; i < num_threads; i++) {
    pthread_join(tids[i], NULL);
  }

}
