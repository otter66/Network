// ������ ����

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVERPORT 9000
#define BUFSIZE 512

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>


// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg) {
    exit(1);    // ��������
}

// ���� �Լ� ���� �޽��� ��� 
void err_display(const char* msg) {
    printf("����");
}

int main()
{
    int retval; // ���� ������� ���� ���� 

    // 1. initial winsock 
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) return 1;

    // 2. create socket ==> TCP: SOCK_STREAM, UDP: SOCK_DGRAM
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);  // socket(�ּ�ü��, ����Ÿ��, ��������)
    if (sock == INVALID_SOCKET) err_quit("socket error");

    // 3. server IP address, port number setting(bind)
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));    // ���� �ּ� ������ 0���� �ʱ�ȭ
    serveraddr.sin_family = AF_INET; //�ּ� ü�� (IPv4, IPv6)
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY: �ڵ� IP ����
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)); // bind(����, �ּ�, ũ��)
    if (retval == SOCKET_ERROR) err_quit("bind error");

    //�����غ�
    //--------------------------------------------------------------------------------------

    SOCKADDR_IN clientaddr; // ���� Ŭ���̾�Ʈ ����
    int addrlen;    //�ּ� ����
    char buf[BUFSIZE + 1];  // ������ �������

    while (1) {
        addrlen = sizeof(clientaddr);

        retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR*)&clientaddr, &addrlen);    //recv(����, ���� ����, ũ��, 0, ���� IP �ּ�, ����)

        if (retval == SOCKET_ERROR) {
            err_display("recvfrom()");
            continue;
        }

        buf[retval] = '\0'; // buffer end ǥ��

        printf("\n[TCP%s:%d] %s", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
    }   // accept ������

    closesocket(sock);
    WSACleanup();
}