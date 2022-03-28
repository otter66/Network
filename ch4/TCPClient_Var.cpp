// TCP Client : ����
// 
//#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 50

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
    char buf[BUFSIZE];
    const char* testdata[] = {
        "�朗��",
        "world",
        "My name is",
        "suyeon.."
    };
    int len;

    for (int i = 0; i < 4; i++) {
        len = strlen(testdata[i]);
        strncpy(buf, testdata[i], len);
        buf[len++] = '\n';  // end line
        
        //send
        retval = send(sock, buf, len, 0); //�۽� ���۸� ���� ������ ����
        if (retval == SOCKET_ERROR) {
            err_display("send() error");
            break;
        }
        printf("[TCPŬ���̾�Ʈ]%d ����Ʈ�� ���½��ϴ�.\n", retval);
    }

    closesocket(sock);
    WSACleanup();
}