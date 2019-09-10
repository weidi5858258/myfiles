//
// Created by root on 18-10-29.
//

#include "TransferFile.h"

#define HELLO_WORLD_SERVER_PORT    69
#define LENGTH_OF_LISTEN_QUEUE     20
#define BUFFER_SIZE                512
#define FILE_NAME_MAX_SIZE         512

int transferFile(int argc, char **argv) {
    // set socket's address information
    // 设置一个socket地址结构server_addr,代表服务器internet的地址和端口
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);

    // create a stream socket
    // 创建用于internet的流协议(TCP)socket，用server_socket代表服务器向客户端提供服务的接口
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Create Socket Failed!\n");
        exit(1);
    }

    // 把socket和socket地址结构绑定
    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr))) {
        printf("Server Bind Port: %d Failed!\n", HELLO_WORLD_SERVER_PORT);
        exit(1);
    }

    // server_socket用于监听
    if (listen(server_socket, LENGTH_OF_LISTEN_QUEUE)) {
        printf("Server Listen Failed!\n");
        exit(1);
    }

    // 服务器端一直运行用以持续为客户端提供服务
    while (1) {
        // 定义客户端的socket地址结构client_addr，当收到来自客户端的请求后，调用accept
        // 接受此请求，同时将client端的地址和端口等信息写入client_addr中
        struct sockaddr_in client_addr;
        socklen_t length = sizeof(client_addr);

        // 接受一个从client端到达server端的连接请求,将客户端的信息保存在client_addr中
        // 如果没有连接请求，则一直等待直到有连接请求为止，这是accept函数的特性，可以
        // 用select()来实现超时检测
        // accpet返回一个新的socket,这个socket用来与此次连接到server的client进行通信
        // 这里的new_server_socket代表了这个通信通道
        int new_server_socket = accept(server_socket, (struct sockaddr *) &client_addr, &length);
        if (new_server_socket < 0) {
            printf("Server Accept Failed!\n");
            break;
        }
        char status[4];
        recv(new_server_socket, status, sizeof(status), 0);
        printf("recv status from client: %s\n", status);


        char buffer[BUFFER_SIZE];
        char ack[50];

        bzero(buffer, sizeof(buffer));
        length = recv(new_server_socket, buffer, BUFFER_SIZE, 0);//从客户端接收数据，数据是文件名。
        printf("recv file_name from client: %s\n", buffer);

        fgets(ack, 4096, stdin);
        send(new_server_socket, ack, strlen(ack), 0);


        //sleep(10);
        //	bzero(buffer, sizeof(buffer));




        if (length < 0) {
            printf("Server Recieve Data Failed!\n");
            break;
        }


        char file_name[FILE_NAME_MAX_SIZE + 1];
        bzero(file_name, sizeof(file_name));
        strncpy(file_name, buffer,
                strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));
        //拷贝字符串

        if (!strcmp(status, "get")) {

            FILE *fp = fopen(file_name, "r");
            if (fp == NULL) {
                printf("File:\t%s Not Found!\n", file_name);
            } else {
                bzero(buffer, BUFFER_SIZE);//清空
                int file_block_length = 0;
                while ((file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
                    printf("file_block_length = %d\n", file_block_length);
                    //每次读1024个字节的字符， 就将其发送出去；
                    //发送buffer中的字符串到new_server_socket,实际上就是发送给客户端
                    if (send(new_server_socket, buffer, file_block_length, 0) < 0) {
                        printf("Send File:\t%s Failed!\n", file_name);
                        break;
                    }

                    bzero(buffer, sizeof(buffer));
                }
                fclose(fp);
                printf("File:\t%s Transfer Finished!\n", file_name);
            }
        } else {
            FILE *fp = fopen(file_name, "w");
            if (fp == NULL) {
                printf("File:\t%s Can Not Open To Write!\n", file_name);
                exit(1);
            }

            // 从客户端接收数据到buffer中   
            bzero(buffer, sizeof(buffer));
            int length = 0;
            while (length = recv(new_server_socket, buffer, BUFFER_SIZE, 0)) {
                if (length < 0) {
                    printf("Recieve Data   Failed!\n");
                    break;
                } else
                    printf("%s\n", buffer);
                int write_length = fwrite(buffer, sizeof(char), length, fp);
                if (write_length < length) {
                    printf("File:\t%s Write Failed!\n", file_name);
                    break;
                }
                bzero(buffer, BUFFER_SIZE);
            }
            fclose(fp);//使用fclose()函数就可以把缓冲区内最后剩余的数据输出到内核缓冲区，并释放文件指针和有关的缓冲区。否则最后的buffer将丢失。
            printf("Recieve File:\t %s Finished!\n", file_name);

        }
        close(new_server_socket);
    }

    close(server_socket);

    return 0;
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

int test_start_server(int argc, char **argv) {
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