#include "LinuxSocket.h"

// ss为服务器端的socket描述符
static int remote_server_sock_fd = -1;

#define DATA_BUFFER 1024

void close_remote_server_sock_fd(int &remote_server_sock_fd) {
    printf("close_remote_server_sock_fd() start remote_server_sock_fd: %d\n",
           remote_server_sock_fd);
    if (remote_server_sock_fd != -1) {
        close(remote_server_sock_fd);
        remote_server_sock_fd = -1;
    }
    printf("close_remote_server_sock_fd() end remote_server_sock_fd: %d\n",
           remote_server_sock_fd);
}

void sig_process(int signo) {
    printf("sig_process() Catch a exit signal.signo: %d\n",
           signo);
    close_remote_server_sock_fd(remote_server_sock_fd);
    exit(EXIT_SUCCESS);
}

// 下面是客户端对服务端发送并接收消息的不同处理方式

// 客户端对服务器端的处理
void process_server_with_read_write(int &remote_server_sock_fd) {
    ssize_t size = 0;
    // 数据的缓冲区
    char buffer[DATA_BUFFER];
    for (;;) {
        // 从套接字中读取数据放到缓冲区buffer中
        // 从屏幕中读取用户输入的内容
        memset(&buffer, '\0', sizeof(buffer));
        size = read(0, buffer, DATA_BUFFER);
        printf("client size: %ld\n", size);
        if (size > 0) {
            // 发给服务器端
            write(remote_server_sock_fd, buffer, size);
        }

        /*write_size = write(remote_server_sock_fd, buffer, read_size);
        if (write_size == -1) {
            printf("client write error. fd = %d\n", remote_server_sock_fd);
            close(remote_server_sock_fd);
            return;
        }
        if (write_size == 0) {
            printf("client没有发送数据\n");
            return;
        }
        printf("client write size: %ld\n", write_size);*/
    }
}

void process_server_with_recv_send(int &remote_server_sock_fd) {
    ssize_t read_size = -1;
    ssize_t write_size = -1;
    ssize_t send_size = -1;
    ssize_t recv_size = -1;
    // 数据的缓冲区
    char buffer[DATA_BUFFER];
    for (;;) {
        // 从套接字中读取数据放到缓冲区buffer中
        // 从屏幕中读取用户输入的内容
        memset(&buffer, 0, sizeof(buffer));
        printf("请输入要发送的内容:\n");
        // 用户从屏幕输入消息
        read_size = read(0, buffer, DATA_BUFFER);
        if (read_size == -1) {
            fprintf(stderr, "read error: %s\n", strerror(errno));
            close_remote_server_sock_fd(remote_server_sock_fd);
            return;
        }
        if (read_size == 0) {
            printf("用户没有输入消息\n");
            continue;
        }
        printf("client read size: %ld\n", read_size);

        // 发送消息给服务器
        send_size = send(remote_server_sock_fd, buffer, read_size, 0);
        if (send_size == -1) {
            fprintf(stderr, "send error: %s\n", strerror(errno));
            close_remote_server_sock_fd(remote_server_sock_fd);
            return;
        }
        if (send_size == 0) {
            printf("client没有发送数据\n");
            return;
        }
        printf("client send size: %ld\n", send_size);

        memset(&buffer, 0, sizeof(buffer));
        // 从服务器读取数据
        recv_size = recv(remote_server_sock_fd, buffer, DATA_BUFFER, 0);
        if (recv_size == -1) {
            fprintf(stderr, "recv error: %s\n", strerror(errno));
            close_remote_server_sock_fd(remote_server_sock_fd);
            return;
        }
        if (recv_size == 0) {
            printf("client没有接收到数据\n");
            /*close(remote_server_sock_fd);
            return;*/
        }
        // 写到标准输出
        // write(1, buffer, recv_size);
        puts(buffer);
    }
}

void process_server_with_readv_writev(int &remote_server_sock_fd) {
    char buffer[30];
    ssize_t read_size = 0;
    ssize_t readv_size = 0;
    struct iovec *v = (struct iovec *) malloc(3 * sizeof(struct iovec));
    if (!v) {
        perror("Not enough memory\n");
        return;
    }
    v[0].iov_base = buffer;
    v[1].iov_base = buffer + 10;
    v[2].iov_base = buffer + 20;
    v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;
    int i = 0;
    for (;;) {
        printf("\n请输入要发送的内容:\n");
        // 用户从屏幕输入消息
        read_size = read(0, v[0].iov_base, 10);
        if (read_size == -1) {
            fprintf(stderr, "read error: %s\n", strerror(errno));
            close_remote_server_sock_fd(remote_server_sock_fd);
            return;
        }
        if (read_size == 0) {
            printf("用户没有输入消息\n");
            continue;
        }
        printf("client read size: %ld\n", read_size);
        writev(remote_server_sock_fd, v, 1);

//        memset(&v, 0, sizeof(v));
//        v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;
        readv_size = readv(remote_server_sock_fd, v, 3);
        if (readv_size == -1) {
            fprintf(stderr, "readv error: %s\n", strerror(errno));
            close_remote_server_sock_fd(remote_server_sock_fd);
            break;
        }
        if (readv_size == 0) {
            printf("client没有接收到数据\n");
            /*close(remote_server_sock_fd);
            return;*/
        }
        // 输出不好,有乱码,还不知道原因
        for (i = 0; i < 3; i++) {
            if (v[i].iov_base != NULL) {
                write(1, v[i].iov_base, v[i].iov_len);
            }
        }
    }
    free(v);
}

void process_server_with_recvmsg_sendmsg(int &remote_server_sock_fd) {
    // 向量的缓冲区
    char buffer[30];
    ssize_t read_size = -1;
    ssize_t recvmsg_size = -1;
    // 消息结构
    struct msghdr msg;
    // 申请3个向量
    struct iovec *v =
            (struct iovec *) malloc(3 * sizeof(struct iovec));
    if (!v) {
        perror("Not enough memory\n");
        return;
    }

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
        printf("\n请输入要发送的内容:\n");
        // 用户从屏幕输入消息
        read_size = read(0, v[0].iov_base, 10);
        if (read_size == -1) {
            fprintf(stderr, "read error: %s\n", strerror(errno));
            close_remote_server_sock_fd(remote_server_sock_fd);
            return;
        }
        if (read_size == 0) {
            printf("用户没有输入消息\n");
            continue;
        }
        printf("client read size: %ld\n", read_size);

        v[0].iov_len = read_size;
        // 发送消息给服务端
        sendmsg(remote_server_sock_fd, &msg, 0);
        printf("client sendd size: %ld\n", read_size);

        /*v[0].iov_len = v[1].iov_len = v[2].iov_len = 10;
        recvmsg_size = recvmsg(remote_server_sock_fd, &msg, 0);
        if (recvmsg_size == -1) {
            fprintf(stderr, "client recvmsg error. fd = %d\n", remote_server_sock_fd, strerror(errno));
            close_remote_server_sock_fd(remote_server_sock_fd);
            break;
        }
        if (recvmsg_size == 0) {
            // 跟客户端断开连接时
            printf("client没有接收到数据\n");
        }
        for (i = 0; i < 3; i++) {
            if (v[i].iov_len > 0) {
                write(1, v[i].iov_base, v[i].iov_len);
            }
        }*/
    }
}

// 下面是使用线程处理读取和发送消息的操作

void *read_thread_work(void *args) {
    printf("read_thread_work tid:  %u\n", pthread_self());

    ssize_t recv_size = -1;
    // 数据的缓冲区
    char buffer[DATA_BUFFER];
    for (;;) {
        memset(&buffer, 0, sizeof(buffer));
        // 从服务器读取数据
        recv_size = recv(remote_server_sock_fd, buffer, DATA_BUFFER, 0);
        if (recv_size == -1) {
            fprintf(stderr, "recv error: %s\n", strerror(errno));
            close_remote_server_sock_fd(remote_server_sock_fd);
            return (int *) EXIT_FAILURE;
        }
        if (recv_size == 0) {
            printf("client没有接收到数据\n");
            /*close(remote_server_sock_fd);
            return;*/
        }
        // 写到标准输出
        // write(1, buffer, recv_size);
        puts(buffer);
    }
}

void *write_thread_work(void *args) {
    printf("write_thread_work tid: %u\n", pthread_self());

    ssize_t read_size = -1;
    ssize_t send_size = -1;
    // 数据的缓冲区
    char buffer[DATA_BUFFER];
    for (;;) {
        // 从套接字中读取数据放到缓冲区buffer中
        // 从屏幕中读取用户输入的内容
        memset(&buffer, 0, sizeof(buffer));
        printf("请输入要发送的内容:\n");
        // 用户从屏幕输入消息
        read_size = read(0, buffer, DATA_BUFFER);
        if (read_size == -1) {
            fprintf(stderr, "read error: %s\n", strerror(errno));
            close_remote_server_sock_fd(remote_server_sock_fd);
            return (int *) EXIT_FAILURE;
        }
        if (read_size == 0) {
            printf("用户没有输入消息\n");
            continue;
        }
        printf("client read size: %ld\n", read_size);

        // 发送消息给服务器
        send_size = send(remote_server_sock_fd, buffer, read_size, 0);
        if (send_size == -1) {
            fprintf(stderr, "send error: %s\n", strerror(errno));
            close_remote_server_sock_fd(remote_server_sock_fd);
            return (int *) EXIT_FAILURE;
        }
        if (send_size == 0) {
            printf("client没有发送数据\n");
            continue;
        }
        printf("client send size: %ld\n", send_size);
    }
}

// 下面是主程序

int test_start_client(char *server_ip) {
    // 服务器端地址结构
    struct sockaddr_in server_addr;
    // 返回值
    int err;

    // 注册信号
    signal(SIGINT, sig_process);
    signal(SIGPIPE, sig_process);

    // 建立一个流式套接字
    remote_server_sock_fd = socket(AF_INET,
                                   SOCK_STREAM,
                                   0);
    if (remote_server_sock_fd == -1) {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("client remote_server_sock_fd: %d\n",
           remote_server_sock_fd);

    // 设置服务器端地址
    // 清零
    /*bzero(reinterpret_cast<char *>(&server_addr),
          sizeof(server_addr));*/
    bzero(&server_addr, sizeof(server_addr));
    // 协议族
    server_addr.sin_family = AF_INET;
    // IP地址为本地任意IP地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 服务器端口
    server_addr.sin_port = htons(PORT);

    inet_aton(server_ip, &server_addr.sin_addr);
    // 将用户输入的字符串类型的IP地址转为整形
    // 下面这种方式连接不上服务端
    /*inet_pton(AF_INET,
              server_ip,
              &server_addr);*/

    // 绑定地址结构到套接字描述符
    err = connect(remote_server_sock_fd,
                  (struct sockaddr *) &server_addr,
                  sizeof(struct sockaddr));
    if (err == -1) {
        fprintf(stderr, "connect error: %s\n", strerror(errno));
        close_remote_server_sock_fd(remote_server_sock_fd);
        exit(EXIT_FAILURE);
    }

    // process_server_with_read_write(remote_server_sock_fd);
    // process_server_with_recv_send(remote_server_sock_fd);
    process_server_with_readv_writev(remote_server_sock_fd);
//    process_server_with_recvmsg_sendmsg(remote_server_sock_fd);

    // 使用两个不同的线程处理读和写的操作
    // 以后也是要用这种方式处理的
    /*pthread_t read_thread;
    pthread_t write_thread;
    err = pthread_create(&read_thread, NULL, read_thread_work, NULL);
    if (err != 0) {
        fprintf(stderr, "pthread_create read_thread error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    err = pthread_create(&write_thread, NULL, write_thread_work, NULL);
    if (err != 0) {
        fprintf(stderr, "pthread_create write_thread error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    err = pthread_join(read_thread, NULL);
    err = pthread_join(write_thread, NULL);*/

    close_remote_server_sock_fd(remote_server_sock_fd);
}

/////////////////////////传输大文件/////////////////////////

#define BUFFER_SIZE 4096

//32系统 可能会截断 一定要自己写 或则atoll有的系统不支持 故
long long ato_ll(const char *p_str) //string --> long long
{

    long long result = 0;
    long long mult = 1;
    unsigned int len = strlen(p_str); // strlen(p_str) unsigned int
    unsigned int i;

    for (i = 0; i < len; ++i) {
        char the_char = p_str[len - (i + 1)];
        long long val;
        if (the_char < '0' || the_char > '9') {
            return 0;
        }
        val = the_char - '0';
        val *= mult;
        result += val;
        mult *= 10;
    }
    return result;
}


struct sockaddr_in server_address;
int filefd;


void *receive(void *s) {
    int thread_order = *(int *) s;

    printf("thread_order = %d\n", thread_order);
    char buf[BUFFER_SIZE];
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket error");
        exit(EXIT_SUCCESS);
    }

    if (connect(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        fprintf(stderr, "thread %d connect error number %d: %s\n", thread_order, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("conncet success,thread id = %d\n", thread_order);
    char head_buf[29] = {0};
    int ret = recv(sockfd, head_buf, sizeof(head_buf) - 1, MSG_WAITALL); //接受每个包的头部
    if (ret < 0) {
        fprintf(stderr, "thread %d recv error number %d: %s\n",
                thread_order, errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    char *cur_ptr = head_buf;
    char *bk = strchr(head_buf, ':');
    if (bk != NULL) {
        *bk = '\0';
    }
    char *size_ptr = bk + 1;

    long long cur = ato_ll(cur_ptr);
    int size = atoi(size_ptr);
    printf("thread %d cur = %lld size = %d\n", thread_order, cur, size);
    while (size) {
        ret = read(sockfd, buf, BUFFER_SIZE);
        if (ret < 0 && errno == EINTR) {
            puts("break by signal");
            continue;
        } else if (ret == 0) {
            break;
        } else if (ret < 0) {
            perror("read");
            exit(1);
        }
        if (pwrite(filefd, buf, ret, cur) < 0) {
            perror("pwrite");
            exit(1);
        }
        cur += ret;
        size -= ret;
    }

    close(sockfd);
    fprintf(stderr, "thread %d finished receiving\n", thread_order);
    free(s);
    pthread_exit((void *) thread_order);
}

int test_start_client(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s server_ip port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    const char *ip = argv[1];
    const int port = atoi(argv[2]);
    printf("sizeof(off_t) = %d\n", sizeof(off_t));
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server_address.sin_addr);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);
    if (connect(sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("connect error");
        exit(EXIT_FAILURE);
    }

    int thread_number = 0;
    int ret = recv(sockfd, &thread_number, sizeof(thread_number), MSG_WAITALL);
    if (ret < 0) {
        perror("recv MSG_WAITALL error");
        exit(EXIT_FAILURE);
    }

    thread_number = ntohl(thread_number); //网络字节序转换成本机字节序
    printf("thread_number = %d\n", thread_number);
    if (thread_number > 500) {
        puts(">>>>500");
        exit(1);
    } //开的线程太多了
    //O_TRUNC 若文件存在 则把文件截断为零
    if ((filefd = open("receive_file.rmvb", O_WRONLY | O_CREAT | O_TRUNC, 0777)) < 0) {
        perror("open error");
        exit(EXIT_FAILURE);
    } else {
        printf("open success\n");
    }
    pthread_t *tid = (pthread_t *) malloc(thread_number * sizeof(pthread_t));
    if (tid == NULL) {
        perror("malloc::");
        exit(1);
    }
    printf("thread_number = %d\n", thread_number);

    for (int i = 0; i < thread_number; ++i) {

        int *thread_id = (int *) malloc(sizeof(int)); //记得在线程中释放
        *thread_id = i;
        pthread_create(&tid[i], NULL, receive, (void *) thread_id);

    }

    for (int i = 0; i < thread_number; ++i) {
        char *ret;
        pthread_join(tid[i], (void **) &ret);
        printf("thread %d finished receiving\n", i);
    }

    close(sockfd);
    close(filefd);
    free(tid);

    gettimeofday(&end, NULL);
    double timeuse = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
    timeuse /= 1000000;
    printf("run time = %f\n", timeuse);

    exit(EXIT_SUCCESS);
}

void LinuxSocket::studyHard() {

    test_start_client("127.0.0.1");

}