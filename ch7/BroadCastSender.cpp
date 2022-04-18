// UDP Client IPv4

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVERIP "255.255.255.255"
#define SERVERPORT 9000
#define BUFSIZE 512

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>

// ���� �Լ� ���� (��������)
void err_quit(const char* msg) { exit(1); }

// ���� �Լ� ���� ��� 
void err_display(const char* msg) { printf("����"); }

int main() {
    int retval;

    // 1. initial winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) return 1;

    // 2. create socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket() error");

    // 3. ���� �ɼ� (���� ��� Ȯ��)
    BOOL bEnable = TRUE;
    retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bEnable, sizeof(bEnable));  //(���ϸ�, ����, �ɼǸ�, �ɼǰ�, ũ��)
    if (retval == SOCKET_ERROR) {
        err_quit("set sock error");
    }

    // 4. connect (server IP, port number)
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);   // 255.255.255.255
    serveraddr.sin_port = htons(SERVERPORT);

    // 5. ������ ������ �غ� (��� �غ�)
    SOCKADDR_IN peeraddr;
    int addrlen;
    char buf[BUFSIZE + 1];
    int len;

    while (1) {
        printf("\n[���� ������]:");
        if (fgets(buf, BUFSIZE + 1, stdin) == NULL) break;
        len = strlen(buf);
        if (buf[len - 1] == '\n') buf[len - 1] = '\0';
        if (strlen(buf) == 0) break;

        //send
        retval = sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
        if (retval == SOCKET_ERROR) {
            err_display("send() error");
            continue;
        }
        printf("[UDPŬ���̾�Ʈ %d ����Ʈ�� ���½��ϴ�.\n", retval);
    }

    closesocket(sock);
    WSACleanup();
}