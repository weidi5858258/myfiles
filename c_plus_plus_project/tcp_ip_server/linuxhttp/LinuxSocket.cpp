#include "LinuxSocket.h"

/////////////////////////////实现/////////////////////////////

int test_start_server(void);

void LinuxSocket::studyHard() {
    test_start_server();
}

void sig_process(int signo) {
    printf("Catch a exit signal.signo: %d\n", signo);

    exit(EXIT_SUCCESS);
}

// 关闭套接字文件描述符
void close_local_server_sock_fd(int &local_server_sock_fd) {
    if (local_server_sock_fd != -1) {
        close(local_server_sock_fd);
        local_server_sock_fd = -1;
    }
}

void close_remote_client_sock_fd(int &remote_client_sock_fd) {
    if (remote_client_sock_fd != -1) {
        close(remote_client_sock_fd);
        remote_client_sock_fd = -1;
    }
}

// 下面是处理客户端消息的几种不同方式

// 服务器端对客户端的处理
/***
 处理过程利用3个向量来完成数据的接收和响应工作.
 先申请3个向量,每个向量的大小为10个字符.
 利用一个公共的30个字节大小的缓冲区buffer
 来初始化3个向量的地址缓冲区,将每个向量的向量长度
 设置为10.调用readv()来读取客户端的数据后,
 利用3个缓冲区构建响应信息,最后将响应信息发送给服务器端.

 不要使用下面的方法接收数据和发送数据
 */
static struct iovec *vs = NULL, *vc = NULL;

void process_client_with_readv_writev(int remote_client_sock_fd) {
    char buffer[30];
    char read_buffer[DATA_BUFFER];
    ssize_t readv_size = 0;
    struct iovec *v = (struct iovec *) malloc(3 * sizeof(struct iovec));
    if (!v) {
        perror("Not enough memory\n");
        return;
    }
    vs = v;
    v[0].iov_base = buffer;
    v[1].iov_base = buffer + 10;
    v[2].iov_base = buffer + 20;
    v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;
    int i = 0;
    for (;;) {
        readv_size = readv(remote_client_sock_fd, v, 3);
        if (readv_size == -1) {
            fprintf(stderr, "server recv error. fd = %d\n", remote_client_sock_fd, strerror(errno));
            close_remote_client_sock_fd(remote_client_sock_fd);
            break;
        }
        if (readv_size == 0) {
            // 跟客户端断开连接时
            printf("server没有接收到数据\n");
            close_remote_client_sock_fd(remote_client_sock_fd);
            // 不能少
            break;
        }
        // 读取到的内容还不知道怎么输出,因此暂时不要用这种方法
        // 来读取数据和发送数据
        // 下面的输出方法不好
        for (i = 0; i < 3; i++) {
            if (v[i].iov_len > 0) {
                write(1, v[i].iov_base, v[i].iov_len);
            }
        }

        sprintf((char *) v[0].iov_base, "%d ", readv_size);
        sprintf((char *) v[1].iov_base, "bytes alt");
        sprintf((char *) v[2].iov_base, "ogether\n");
        v[0].iov_len = strlen((const char *) v[0].iov_base);
        v[1].iov_len = strlen((const char *) v[1].iov_base);
        v[2].iov_len = strlen((const char *) v[2].iov_base);
        writev(remote_client_sock_fd, v, 3);
    }
    free(v);
}

// sc为客户端的socket描述符
void process_client_with_read_write(int remote_client_sock_fd) {
    ssize_t size = 0;
    // 数据的缓冲区
    char buffer[DATA_BUFFER];
    for (;;) {
        // 从套接字中读取数据放到缓冲区buffer中
        memset(buffer, 0, sizeof(buffer));
        size = read(remote_client_sock_fd, buffer, DATA_BUFFER);
        // 没有数据
        if (size == 0) {
            return;
        } else {
            // 输出读到的内容,这个函数的作用就是写给谁,这个谁就是“1”
            // “1”代表标准输出
            write(1, buffer, size);
        }
        // 构建响应字符,为接收到客户端字节的数量
        sprintf(buffer, "%d bytes altogether\n", size);
        // 发给客户端
        write(remote_client_sock_fd, buffer, strlen(buffer) + 1);
    }
}

void process_client_with_recvfrom_sendto(int remote_client_sock_fd) {

}

void process_client_with_recvmsg_sendmsg(int remote_client_sock_fd) {
    printf("process_client_with_recvmsg_sendmsg remote_client_sock_fd: %d\n",
           remote_client_sock_fd);

    // 向量的缓冲区
    char buffer[30];
    ssize_t recvmsg_size = 0;
    // 消息结构
    struct msghdr msg;
    // 申请3个向量
    struct iovec *v =
            (struct iovec *) malloc(3 * sizeof(struct iovec));
    if (!v) {
        perror("Not enough memory\n");
        return;
    }
    // 挂接全局变量,便于释放管理
    vs = v;

    // 没有名字域
    msg.msg_name = NULL;
    // 名字域长度
    msg.msg_namelen = 0;
    // 没有控制域
    msg.msg_control = NULL;
    // 控制域长度
    msg.msg_controllen = 0;
    // 挂接向量指针
    msg.msg_iov = v;
    // 接收缓冲区长度为30
    msg.msg_iovlen = 30;
    // 无特殊操作
    msg.msg_flags = 0;
    // 每个向量分配10个字节的空间
    // 0~9
    v[0].iov_base = buffer;
    // 10~19
    v[1].iov_base = buffer + 10;
    // 20~29
    v[2].iov_base = buffer + 20;
    // 初始化长度
    v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;

    int i = 0;
    for (;;) {
        memset(msg.msg_iov[0].iov_base, 0, msg.msg_iov[0].iov_len);
        recvmsg_size = recvmsg(remote_client_sock_fd, &msg, 0);
        if (recvmsg_size == -1) {
            fprintf(stderr, "server recv error. fd = %d\n", remote_client_sock_fd, strerror(errno));
            close_remote_client_sock_fd(remote_client_sock_fd);
            break;
        }
        if (recvmsg_size == 0) {
            // 跟客户端断开连接时
            printf("server没有接收到数据\n");
            close_remote_client_sock_fd(remote_client_sock_fd);
            // 不能少
            break;
        }
        printf("server接收到 %d 个字节\n", recvmsg_size);
        printf("server接收到的内容为:\n");
        /*for (i = 0; i < 3; i++) {
            if (v[i].iov_len > 0) {
                write(1, v[i].iov_base, v[i].iov_len);
            }
        }*/
        //write(1, v[0].iov_base, v[0].iov_len);
        write(1, msg.msg_iov[0].iov_base, strlen((const char *) msg.msg_iov[0].iov_base));
        printf("\n");

        // 给客户端发送数据
        // 构建响应字符,为接收到客户端字节的数量,分别放到3个缓冲区中
        /*sprintf((char *) v[0].iov_base, "%d ", recvmsg_size);
        sprintf((char *) v[1].iov_base, "bytes alt");
        sprintf((char *) v[2].iov_base, "ogether\n");
        v[0].iov_len = strlen((const char *) v[0].iov_base);
        v[1].iov_len = strlen((const char *) v[1].iov_base);
        v[2].iov_len = strlen((const char *) v[2].iov_base);
        sendmsg(remote_client_sock_fd, &msg, 0);*/
    }
}

/***
 这种方式比较好用,且便于理解
 */
void process_client_with_recv_send(int remote_client_sock_fd) {
    ssize_t recv_size = -1;
    ssize_t send_size = -1;

    // 数据的缓冲区
    char buffer[DATA_BUFFER];
    for (;;) {
        // 从套接字中读取数据放到缓冲区buffer中
        memset(buffer, 0, sizeof(buffer));
        recv_size = recv(remote_client_sock_fd, buffer, DATA_BUFFER, 0);
        if (recv_size == -1) {
            fprintf(stderr, "server recv error. fd = %d\n", remote_client_sock_fd, strerror(errno));
            close_remote_client_sock_fd(remote_client_sock_fd);
            return;
        }
        if (recv_size == 0) {
            // 跟客户端断开连接时
            printf("server没有接收到数据\n");
            close_remote_client_sock_fd(remote_client_sock_fd);
            // 不能少
            return;
        }
        // 显示到屏幕
        write(1, buffer, recv_size);

        memset(buffer, 0, sizeof(buffer));
        // 构建响应字符,为接收到客户端字节的数量
        //sprintf(buffer, "服务端已经收到客户端发送过去的 %d 个字节.\n", recv_size);
        sprintf(buffer, "服务端已经收到客户端发送过去的 %d 个字节\n", recv_size);
        // 发给客户端
        send_size = send(remote_client_sock_fd, buffer, strlen(buffer) + 1, 0);
        if (send_size == -1) {
            fprintf(stderr, "server send error. fd = %d\n", remote_client_sock_fd, strerror(errno));
            close_remote_client_sock_fd(remote_client_sock_fd);
            return;
        }
        if (send_size == 0) {
            printf("server没有发送数据\n");
            /*return;*/
        }
        printf("server send size: %ld\n", send_size);
    }
}

void readMsg() {
    char buffer[DATA_BUFFER];
}

void writeMsg() {

}

// 下面是主程序

/***
1.bind error: Address already in use
  服务器设置的端口已被占用
2.accept error: Bad file descriptor
 想法:
 关键函数的参数用全局静态变量代替
 以后忘记网络编程的话,从这里开始看

 */
int test_start_server(void) {
    printf("当前进程id: %d\n", getpid());
    // local_server_sock_fd为服务端的socket描述符
    // remote_client_sock  为客户端的socket描述符
    int local_server_sock_fd = -1, remote_client_sock_fd = -1;
    // 服务端地址结构
    // 客户端地址结构
    // struct sockaddr_in这个地址结构只能用于IPv4协议
    // struct sockaddr   这个地址结构能用于任何协议
    struct sockaddr_in server_addr, client_addr;
    // sockaddr_in6 sockaddr_storage sockaddr_un
    // 返回值
    int err = -1;

    // 注册信号
    // 信号SIGINT通常中是由Ctrl+C终止进程造成的,与Ctrl+C一致.
    signal(SIGINT, sig_process);
    /***
     测试:
     当客户端连接后,退出服务器程序.当标准输入有数据的时候,
     客户端会通过套接字描述符发送数据到服务器端,而服务器已经
     关闭,因此客户端会收到一个SIGPIPE信号.
     */
    signal(SIGPIPE, sig_process);
    signal(SIGIO, sig_process);

    /////////////////////////////socket创建过程/////////////////////////////
    /***
     socket()函数中的__domain参数,使用不同的协议族时,
     bind函数中的第二个参数就需要采用相应的结构
     如AF_INET使用struct sockaddr_in结构
     如AF_UNIX使用struct sockaddr_un结构
     不管采用什么样的结构,调用bind()函数时都需要强制转化成struct sockaddr结构
     */
    // 建立一个流式套接字
    local_server_sock_fd = socket(AF_INET,
                                  SOCK_STREAM,
                                  0);
    if (local_server_sock_fd == -1) {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // 解决在close之后会有一个WAIT_TIME,导致bind失败的问题
    int val = -1;
    err = setsockopt(local_server_sock_fd,
                     SOL_SOCKET,
                     SO_REUSEADDR,
                     (void *) &val,
                     sizeof(int));
    if (err == -1) {
        fprintf(stderr, "setsockopt error: %s\n", strerror(errno));
        close_local_server_sock_fd(local_server_sock_fd);
        exit(EXIT_FAILURE);
    }

    /////////////////////////////bind绑定过程/////////////////////////////
    /***
     struct in_addr {
        // 必须是网络字节序
        uint32_t s_addr;
     };

     bind()函数的第三个参数一律使用sizeof(struct sockaddr)
     */
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    // 设置服务端地址
    // 清零
    /*bzero(reinterpret_cast<char *>(&server_addr), sizeof(server_addr));*/
    bzero(&server_addr, sizeof(server_addr));
    // 设置协议族,意思就是打算采用什么协议族.跟socket()函数中的__domain参数要一致
    server_addr.sin_family = AF_INET;
    // 设置为本地地址
//    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 将字符串的IP地址转化为网络字节序
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.107");
//    server_addr.sin_addr.s_addr = inet_addr("43.82.112.164");
    // 服务器端口(65535).地址结构的端口地址,网络字节序(PORT为主机字节序,需要转化为网络字节序)
    // 我想使用这个端口之前,应不应该判断一下这个端口可不可以使用,有没有被其他进程占用
    server_addr.sin_port = htons(PORT);
    // 将server_addr.sin_zero保留字段置为0
    bzero(&(server_addr.sin_zero), 8);
    // 绑定地址结构到套接字描述符
    err = bind(local_server_sock_fd,
               (struct sockaddr *) &server_addr,
               sizeof(struct sockaddr));
    if (err == -1) {
        fprintf(stderr, "bind error: %s\n", strerror(errno));
        close_local_server_sock_fd(local_server_sock_fd);
        exit(EXIT_FAILURE);
    }

    /////////////////////////////listen监听过程/////////////////////////////
    /***
     listen()函数仅对类型为SOCK_STREAM或者SOCK_SEQPACKET的协议有效.
     BACKLOG表示等待队列的长度.
     */
    // 设置侦听
    err = listen(local_server_sock_fd, BACKLOG);
    if (err == -1) {
        fprintf(stderr, "listen error: %s\n", strerror(errno));
        close_local_server_sock_fd(local_server_sock_fd);
        exit(EXIT_FAILURE);
    }

    /////////////////////////////appept连接过程/////////////////////////////
    /***
     当accept()函数返回时,会将客户端的信息存储在参数client_addr中.
     */
    for (;;) {
        socklen_t addrlen = sizeof(struct sockaddr);
        // 接收客户端连接
        remote_client_sock_fd = accept(local_server_sock_fd,
                                       (struct sockaddr *) &client_addr,
                                       &addrlen);
        if (remote_client_sock_fd == -1) {
            fprintf(stderr, "accept error: %s ss: %d\n", strerror(errno), local_server_sock_fd);
            // 当客户端退出时,这里会报错,但是服务端并没有停止
            exit(EXIT_FAILURE);
        }

        printf("server  local_server_sock_fd: %d\n", local_server_sock_fd);
        printf("server remote_client_sock_fd: %d\n", remote_client_sock_fd);

        struct sockaddr_in guest;
        char serv_ip[20], guest_ip[20];
        socklen_t guest_len = sizeof(guest);
        guest_len = sizeof(client_addr);
        //getsockname(remote_client_sock_fd, (sockaddr *) &guest, &guest_len);
        //getpeername(remote_client_sock_fd, (sockaddr *) &guest, &guest_len);
        getpeername(remote_client_sock_fd, (sockaddr *) &client_addr, &guest_len);
        //inet_ntop(AF_INET, &guest.sin_addr, guest_ip, sizeof(guest_ip));
        inet_ntop(AF_INET, &client_addr.sin_addr, guest_ip, sizeof(guest_ip));
        printf("host: %s %s %s[%d]\n",
               guest_ip,
               inet_ntoa(client_addr.sin_addr),
               inet_ntoa(guest.sin_addr),
               ntohs(guest.sin_port));

        // 把主机信息保存在hostent中
        struct hostent *hptr;
        //if ((hptr = gethostbyaddr((void *) &client_addr.sin_addr, 4, AF_INET)) == NULL) {
        if ((hptr = gethostbyaddr((void *) &client_addr.sin_addr, sizeof(client_addr.sin_addr), AF_INET)) == NULL) {
            printf("h_errno: %d\n", h_errno);
            exit(EXIT_FAILURE);
        }
        if (hptr != NULL) {
            // 正式主机名
            printf("主机名(HostName): %s\n", hptr->h_name);
        }

        // 建立一个新的进程处理到来的连接
        pid_t pid = fork();
        if (pid == -1) {
            fprintf(stderr, "fork error: %s\n", strerror(errno));
            close_remote_client_sock_fd(remote_client_sock_fd);
            continue;
        }

        if (pid == 0) {
            printf("2.父进程新创建的子进程id: %d\n", getpid());
            /***
             子进程关闭local_server_sock处理任务,
             使其回到TIME_WAIT状态值.
             */
            close(local_server_sock_fd);
            // process_client_with_read_write(remote_client_sock_fd);
            // process_client_with_recv_send(remote_client_sock_fd);
            // process_client_with_readv_writev(remote_client_sock_fd);
            process_client_with_recvmsg_sendmsg(remote_client_sock_fd);
        } else {
            printf("1.父进程新创建的子进程id: %d\n", pid);
            /***
             在父进程中关闭连接的套接字描述符,
             只是把remote_client_sock的引用数减少1,
             在子进程中还在使用remote_client_sock.
             */
            close(remote_client_sock_fd);
        }
    }
}



















































