#include <stdio.h>
#if defined(__APPLE__)
#   include <sys/event.h> // use kqueue
#elif defined(__linux__) || defined(__unix__)
#   include <sys/epoll.h>
#endif

#define BUF_SIZE 100
#define EVENT_SIZE 50

#define MULPLEX_INIT   0x01
#define MULPLEX_CREATE 0x02
#define MULPLEX_WAIT   0x04
#define MULPLEX_CLOSE  0x08
#define MULPLEX_GET    0x16

int epoll(int fd, int flag);
