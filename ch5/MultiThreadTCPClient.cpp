// TCP Client IPv4�� ����

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>

// ���� �Լ� ���� (��������)
void err_quit(const char* msg) { exit(1); }

// ���� �Լ� ���� ��� 
void err_display(const char* msg) { printf("����"); }

// ������ ���� �Լ� (����� ���� �Լ�)
int recvn(SOCKET s, char* buf, int len, int flags) {
    int received;       // ���� ������ �� ó��
    char* ptr = buf;    // ���� ���� ���� ��ġ
    int left = len;     // ���� ���ۿ� ���� ���� ���� ��

    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR) {
            return SOCKET_ERROR;
        }
        else if (received == 0) break;

        left -= received;   // ���� ���� ��
        ptr += received;    // ���� ��
    }

    return(len - left); //���� ���� �� ��ȯ
}

int main() {
    int retval;

    // 1. initial winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) return 1;

    //2. create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket() error");

    //3. connect (server IP, port number)
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect() error");

    //4. ������ ��ſ� ����� ����
    char buf[BUFSIZE + 1];
    int len;
    while (1) {
        printf("\n[���� ������]:");
        if (fgets(buf, BUFSIZE + 1, stdin) == NULL) break;
        len = strlen(buf);
        if (buf[len - 1] == '\n') buf[len - 1] = '\0';
        if (strlen(buf) == 0) break;

        //send
        retval = send(sock, buf, strlen(buf), 0); //�۽� ���۸� ���� ������ ����
        if (retval == SOCKET_ERROR) {
            err_display("send() error");
            break;
        }
        printf("[TCPŬ���̾�Ʈ]%d ����Ʈ�� ���½��ϴ�.\n", retval);

        //receive
        retval = recvn(sock, buf, retval, 0);
        if (retval == SOCKET_ERROR) err_display("receive error");
        else if (retval == 0) break;

        buf[retval] = '\0';
        printf("[TCPŬ���̾�Ʈ]%d ����Ʈ�� �޾ҽ��ϴ�. \n", retval);
        printf("[���� ������]%s\n", buf);
    }

    closesocket(sock);

    WSACleanup();
}