//
// a simple echo server using epoll in linux
//
// 2009-11-05
// 2013-03-22:修改了几个问题，1是/n格式问题，2是去掉了原代码不小心加上的ET模式;
// 本来只是简单的示意程序，决定还是加上 recv/send时的buffer偏移
// by sparkling
//

/***
第一步：epoll_create()系统调用。此调用返回一个句柄，之后所有的使用都依靠这个句柄来标识。
第二步：epoll_ctl()系统调用。通过此调用向epoll对象中添加、删除、修改感兴趣的事件，返回0标识成功，返回-1表示失败。
第三部：epoll_wait()系统调用。通过此调用收集收集在epoll监控中已经发生的事件。
 */
#include "MyHead.h"

struct myevent_s {
    int fd;

    void (*call_back)(int fd, int events, void *arg);

    int events;
    void *arg;
    int status; // 1: in epoll wait list, 0 not in
    char buff[128]; // recv data buffer
    int len, s_offset;
    long last_active; // last active time
};

// set event
void EventSet(myevent_s *ev, int fd, void (*call_back)(int, int, void *), void *arg) {
    ev->fd = fd;
    ev->call_back = call_back;
    ev->events = 0;
    ev->arg = arg;
    ev->status = 0;
    bzero(ev->buff, sizeof(ev->buff));
    ev->s_offset = 0;
    ev->len = 0;
    ev->last_active = time(NULL);
}

// add/mod an event to epoll
void EventAdd(int epollFd, int events, myevent_s *ev) {
    struct epoll_event epv = {0, {0}};
    int op;
    epv.data.ptr = ev;
    epv.events = ev->events = events;
    if (ev->status == 1) {
        op = EPOLL_CTL_MOD;
    } else {
        op = EPOLL_CTL_ADD;
        ev->status = 1;
    }
    if (epoll_ctl(epollFd, op, ev->fd, &epv) < 0)
        printf("Event Add failed[fd=%d], evnets[%d]\n", ev->fd, events);
    else
        printf("Event Add OK[fd=%d], op=%d, evnets[%0X]\n", ev->fd, op, events);
}

// delete an event from epoll
void EventDel(int epollFd, myevent_s *ev) {
    struct epoll_event epv = {0, {0}};
    if (ev->status != 1) return;
    epv.data.ptr = ev;
    ev->status = 0;
    epoll_ctl(epollFd, EPOLL_CTL_DEL, ev->fd, &epv);
}

int g_epollFd;
myevent_s g_Events[MAX_EVENTS + 1]; // g_Events[MAX_EVENTS] is used by listen fd
void RecvData(int fd, int events, void *arg);

void SendData(int fd, int events, void *arg);

// accept new connections from clients
void AcceptConn(int fd, int events, void *arg) {
    struct sockaddr_in sin;
    socklen_t len = sizeof(struct sockaddr_in);
    int nfd, i;
    // accept
    if ((nfd = accept(fd, (struct sockaddr *) &sin, &len)) == -1) {
        if (errno != EAGAIN && errno != EINTR) {
        }
        printf("%s: accept, %d", __func__, errno);
        return;
    }
    do {
        for (i = 0; i < MAX_EVENTS; i++) {
            if (g_Events[i].status == 0) {
                break;
            }
        }
        if (i == MAX_EVENTS) {
            printf("%s:max connection limit[%d].", __func__, MAX_EVENTS);
            break;
        }
        // set nonblocking
        int iret = 0;
        if ((iret = fcntl(nfd, F_SETFL, O_NONBLOCK)) < 0) {
            printf("%s: fcntl nonblocking failed:%d", __func__, iret);
            break;
        }
        // add a read event for receive data
        EventSet(&g_Events[i], nfd, RecvData, &g_Events[i]);
        EventAdd(g_epollFd, EPOLLIN, &g_Events[i]);
    } while (0);
    printf("new conn[%s:%d][time:%d], pos[%d]\n", inet_ntoa(sin.sin_addr),
           ntohs(sin.sin_port), g_Events[i].last_active, i);
}

// receive data
void RecvData(int fd, int events, void *arg) {
    struct myevent_s *ev = (struct myevent_s *) arg;
    int len;
    // receive data
    len = recv(fd, ev->buff + ev->len, sizeof(ev->buff) - 1 - ev->len, 0);
    EventDel(g_epollFd, ev);
    if (len > 0) {
        ev->len += len;
        ev->buff[len] = '\0';
        printf("C[%d]:%s\n", fd, ev->buff);
        // change to send event
        EventSet(ev, fd, SendData, ev);
        EventAdd(g_epollFd, EPOLLOUT, ev);
    } else if (len == 0) {
        close(ev->fd);
        printf("[fd=%d] pos[%d], closed gracefully.\n", fd, ev - g_Events);
    } else {
        close(ev->fd);
        printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));
    }
}

// send data
void SendData(int fd, int events, void *arg) {
    struct myevent_s *ev = (struct myevent_s *) arg;
    int len;
    // send data
    len = send(fd, ev->buff + ev->s_offset, ev->len - ev->s_offset, 0);
    if (len > 0) {
        printf("send[fd=%d], [%d<->%d]%s\n", fd, len, ev->len, ev->buff);
        ev->s_offset += len;
        if (ev->s_offset == ev->len) {
            // change to receive event
            EventDel(g_epollFd, ev);
            EventSet(ev, fd, RecvData, ev);
            EventAdd(g_epollFd, EPOLLIN, ev);
        }
    } else {
        close(ev->fd);
        EventDel(g_epollFd, ev);
        printf("send[fd=%d] error[%d]\n", fd, errno);
    }
}

void InitListenSocket(int epollFd, short port) {
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(listenFd, F_SETFL, O_NONBLOCK); // set non-blocking
    printf("server listen fd=%d\n", listenFd);
    EventSet(&g_Events[MAX_EVENTS], listenFd, AcceptConn, &g_Events[MAX_EVENTS]);
    // add listen socket
    EventAdd(epollFd, EPOLLIN, &g_Events[MAX_EVENTS]);
    // bind & listen
    sockaddr_in sin;
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);
    bind(listenFd, (const sockaddr *) &sin, sizeof(sin));
    listen(listenFd, 5);
}

int main(int argc, char **argv) {
    unsigned short port = 12345; // default port
    if (argc == 2) {
        port = atoi(argv[1]);
    }
    // create epoll
    g_epollFd = epoll_create(MAX_EVENTS);
    if (g_epollFd <= 0) printf("create epoll failed.%d\n", g_epollFd);
    // create & bind listen socket, and add to epoll, set non-blocking
    InitListenSocket(g_epollFd, port);
    // event loop
    struct epoll_event events[MAX_EVENTS];
    printf("server running:port[%d]\n", port);
    int checkPos = 0;
    while (1) {
        // a simple timeout check here, every time 100, better to use a mini-heap, and add timer event
        long now = time(NULL);
        for (int i = 0; i < 100; i++, checkPos++) // doesn't check listen fd
        {
            if (checkPos == MAX_EVENTS) checkPos = 0; // recycle
            if (g_Events[checkPos].status != 1) continue;
            long duration = now - g_Events[checkPos].last_active;
            if (duration >= 60) // 60s timeout
            {
                close(g_Events[checkPos].fd);
                printf("[fd=%d] timeout[%d--%d].\n", g_Events[checkPos].fd, g_Events[checkPos].last_active, now);
                EventDel(g_epollFd, &g_Events[checkPos]);
            }
        }
        // wait for events to happen
        int fds = epoll_wait(g_epollFd, events, MAX_EVENTS, 1000);
        if (fds < 0) {
            printf("epoll_wait error, exit\n");
            break;
        }
        for (int i = 0; i < fds; i++) {
            myevent_s *ev = (struct myevent_s *) events[i].data.ptr;
            if ((events[i].events & EPOLLIN) && (ev->events & EPOLLIN)) // read event
            {
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
            if ((events[i].events & EPOLLOUT) && (ev->events & EPOLLOUT)) // write event
            {
                ev->call_back(ev->fd, events[i].events, ev->arg);
            }
        }
    }
    // free resource
    return 0;
}
