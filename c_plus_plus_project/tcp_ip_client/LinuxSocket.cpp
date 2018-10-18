#include "LinuxSocket.h"

// ss为服务器端的socket描述符
static int ss = 0;

void sig_process(int signo) {
    printf("Catch a exit signal.signo: %d\n", signo);
    if (ss != 0) {
        close(ss);
    }
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
    }
}

void process_conn_client2(int ss) {
    ssize_t size = 0;
    // 数据的缓冲区
    char buffer[1024];
    for (;;) {
        // 从套接字中读取数据放到缓冲区buffer中
        // 从屏幕中读取用户输入的内容
        memset(&buffer, 0, sizeof(buffer));
        size = read(0, buffer, 1024);
        if (size == -1) {
            printf("client read error. fd = %d\n", ss);
            close(ss);
            return;
        }
        if (size == 0) {
            printf("client没有接收到数据\n");
            continue;
        }

        printf("client read size: %ld\n", size);

        // 发送给服务器
        send(ss, buffer, size, 0);
        // 从服务器读取数据
        size = recv(ss, buffer, 1024, 0);
        // 写到标准输出
        write(1, buffer, size);
    }
}

int test_create_client_socket(char *server_ip) {
    // ss为服务器端的socket描述符
    // int ss;
    // 服务器端地址结构
    struct sockaddr_in server_addr;
    // 返回值
    int err;
    signal(SIGINT, sig_process);
    signal(SIGPIPE, sig_process);
    // 建立一个流式套接字
    ss = socket(AF_INET, SOCK_STREAM, 0);
    // 出错
    if (ss == -1) {
        printf("socket error\n");
        return EXIT_FAILURE;
    }

    printf("client ss: %d\n", ss);

    // 设置服务器端地址
    // 清零
    bzero(reinterpret_cast<char *>(&server_addr), sizeof(server_addr));
    // 协议族
    server_addr.sin_family = AF_INET;
    // IP地址为本地任意IP地址
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 服务器端口
    server_addr.sin_port = htons(PORT);

    // inet_aton("127.0.0.1", &server_addr.sin_addr);
    // 将用户输入的字符串类型的IP地址转为整形
    inet_pton(AF_INET,
              server_ip,
              &server_addr);

    // 绑定地址结构到套接字描述符
    connect(ss,
            (struct sockaddr *) &server_addr,
            sizeof(struct sockaddr));

    process_conn_client(ss);

    close(ss);
}

void LinuxSocket::studyHard() {

    test_create_client_socket("127.0.0.1");

}