#include "epoll.h"

int epoll(int fd, int flag)
{
    static int queue;
#if defined(__APPLE__)
    static struct kevent event_list[EVENT_SIZE];
    static struct kevent event;
#elif defined(__linux__) || defined(__unix__)
    static int event_cnt;
    static struct epoll_event ep_events[EVENT_SIZE];
    static struct epoll_event event;
#endif

    switch (flag)
    {
        case MULPLEX_INIT:
#if defined(__APPLE__)
            queue = kqueue();
#elif defined(__linux__) || defined(__unix__)
            queue = epoll_create(EVENT_SIZE);
#endif
			break;

        case MULPLEX_CREATE:
#if defined(__APPLE__)
            EV_SET(&event, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
            kevent(queue, &event, 1, NULL, 0, NULL);
#elif defined(__linux__) || defined(__unix__)
            event.events = EPOLLIN;
            event.data.fd = fd;
            epoll_ctl(queue, EPOLL_CTL_ADD, fd, &event);
#endif
			break;

        case MULPLEX_GET_SIZE:
#if defined(__APPLE__)
            return kevent(queue, NULL, 0, event_list, EVENT_SIZE, NULL);
#elif defined(__linux__) || defined(__unix__)
            return epoll_wait(queue, ep_events, EVENT_SIZE, -1);
#endif

        case MULPLEX_CLOSE:
#if defined(__linux__) || defined(__unix__)
            epoll_ctl(queue, EPOLL_CTL_DEL, ep_events[fd].data.fd, NULL);
#endif
            return 0;

        case MULPLEX_GET:
#if defined(__APPLE__)
            return (int)event_list[fd].ident;
#elif defined(__linux__) || defined(__unix__)
            return ep_events[fd].data.fd;
#endif
    }

    return 1;
}
