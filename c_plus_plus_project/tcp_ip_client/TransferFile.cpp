//
// Created by root on 18-10-29.
//

#include "TransferFile.h"

#define HELLO_WORLD_SERVER_PORT       69
#define BUFFER_SIZE                   512
#define FILE_NAME_MAX_SIZE            512

int transferFile(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: ./%s ServerIPAddress\n", argv[0]);
        exit(1);
    }

    // 设置一个socket地址结构client_addr, 代表客户机的internet地址和端口
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    back:
    client_addr.sin_family = AF_INET; // internet协议族
    client_addr.sin_addr.s_addr = htons(INADDR_ANY); // INADDR_ANY表示自动获取本机地址
    client_addr.sin_port = htons(0); // auto allocated, 让系统自动分配一个空闲端口

    // 创建用于internet的流协议(TCP)类型socket，用client_socket代表客户端socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Create Socket Failed!\n");
        exit(1);
    }

    // 把客户端的socket和客户端的socket地址结构绑定
    if (bind(client_socket, (struct sockaddr *) &client_addr, sizeof(client_addr))) {
        printf("Client Bind Port Failed!\n");
        exit(1);
    }

    // 设置一个socket地址结构server_addr,代表服务器的internet地址和端口
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;

    // 服务器的IP地址来自程序的参数
    if (inet_aton(argv[1], &server_addr.sin_addr) == 0) {
        printf("Server IP Address Error!\n");
        exit(1);
    }

    server_addr.sin_port = htons(HELLO_WORLD_SERVER_PORT);
    socklen_t server_addr_length = sizeof(server_addr);




    // 向服务器发起连接请求，连接成功后client_socket代表客户端和服务器端的一个socket连接
    if (connect(client_socket, (struct sockaddr *) &server_addr, server_addr_length) < 0) {
        printf("Can Not Connect To %s!\n", argv[1]);
        exit(1);
    } else
        printf("socket 连接成功.\n");

    char status[4];
    printf(" input get ? \t put ?;\n");
    //fgets(status, 4096, stdin);  反正用这个是错误的？原因不详。
    //bzero(status, sizeof(status));
    scanf("%s", status);
    printf("status == %s\n", status);
    send(client_socket, status, sizeof(status), 0);


    char file_name[FILE_NAME_MAX_SIZE + 1];
    bzero(file_name, sizeof(file_name));
    printf("Please Input File Name .\t");
    scanf("%s", file_name);
    printf("输入的文件名为：%s\n", file_name);

    char buffer[BUFFER_SIZE];
    char recvline[BUFFER_SIZE];
    bzero(buffer, sizeof(buffer));
    strncpy(buffer, file_name, strlen(file_name) > BUFFER_SIZE ? BUFFER_SIZE : strlen(file_name));

    // 向服务器发送buffer中的数据，此时buffer中存放的是客户端需要接收(发送)的文件的名字
    send(client_socket, buffer, BUFFER_SIZE, 0);

    recv(client_socket, recvline, 1024, 0);
    printf("recv ack from server: %s\n", recvline);
    FILE *fp;
    if (!strcmp(status, "get")) //下载文件；
    {

        fp = fopen(file_name, "w");
        if (fp == NULL) {
            printf("File:\t%s Can Not Open To Write!\n", file_name);
            exit(1);
        }

        // 从服务器端接收数据到buffer中
        bzero(buffer, sizeof(buffer));
        int length = 0;
        while (length = recv(client_socket, buffer, BUFFER_SIZE, 0)) {
            if (length < 0) {
                printf("Recieve Data From Server %s Failed!\n", argv[1]);
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

        printf("Recieve File:\t %s From Server[%s] Finished!\n", file_name, argv[1]);

    } else //else 上传
    {
        fp = fopen(file_name, "r");
        if (fp == NULL) {
            printf("File:\t%s Not Found!\n", file_name);
        } else {
            bzero(buffer, BUFFER_SIZE);//清空
            int file_block_length = 0;
            while ((file_block_length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
                printf("file_block_length = %d\n", file_block_length);
                //每次读1024个字节的字符， 就将其发送出去；
                //发送buffer中的字符串到 client_socket,实际上就是发送给fuwu端
                if (send(client_socket, buffer, file_block_length, 0) < 0) {
                    printf("Send File:\t%s Failed!\n", file_name);
                    break;
                }

                bzero(buffer, sizeof(buffer));
            }
            //fclose(fp);   //报错 double  free
            printf("File:\t%s Transfer Finished!\n", file_name);
        }
        //goto back;

    }

    // 传输完毕，关闭socket
    fclose(fp);
    close(client_socket);
    goto back;
    return 0;

}