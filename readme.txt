****
epoll(0, MULPLEX_INIT);

epoll(serv_sock, MULPLEX_CREATE);

epoll(0, MULPLEX_WAIT); // return length of struct size

epoll(clnt_sock, MULPLEX_CREATE);

epoll(fd ,MULPLEX_CLOSE);
