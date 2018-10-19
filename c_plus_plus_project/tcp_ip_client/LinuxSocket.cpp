#include "LinuxSocket.h"

// ss为服务器端的socket描述符
static int remote_server_sock_fd = -1;

void close_remote_server_sock_fd(int &remote_server_sock_fd) {
    if (remote_server_sock_fd != -1) {
        close(remote_server_sock_fd);
        remote_server_sock_fd = -1;
    }
}

void sig_process(int signo) {
    printf("Catch a exit signal.signo: %d\n", signo);
    close_remote_server_sock_fd(remote_server_sock_fd);
    exit(EXIT_SUCCESS);
}

// 服务器端对客户端的处理
void process_conn_client(int ss) {
    ssize_t size = 0;
    // 数据的缓冲区
    char buffer[1024];
    for (;;) {
        // 从套接字中读取数据放到缓冲区buffer中
        // 从屏幕中读取用户输入的内容
        memset(&buffer, '\0', sizeof(buffer));
        size = read(0, buffer, 1024);
        printf("client size: %ld\n", size);
        if (size > 0) {
            // 发给服务器端
            write(ss, buffer, size);
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

#define DATA_BUFFER 1024

void process_conn_client2(int remote_server_sock_fd) {
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

//pthread_mutex_t mutex_lock = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

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

int test_start_client(char *server_ip) {
    // 服务器端地址结构
    struct sockaddr_in server_addr;
    // 返回值
    int err;

    // 注册信号
    signal(SIGINT, sig_process);
    signal(SIGPIPE, sig_process);

    // 建立一个流式套接字
    remote_server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (remote_server_sock_fd == -1) {
        fprintf(stderr, "socket error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("client remote_server_sock_fd: %d\n", remote_server_sock_fd);

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
    // 下面这种方式好像连接不上服务端
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

    //process_conn_client2(remote_server_sock_fd);

    pthread_t read_thread;
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
    err = pthread_join(write_thread, NULL);

    close_remote_server_sock_fd(remote_server_sock_fd);
}

void LinuxSocket::studyHard() {

    test_start_client("127.0.0.1");

}