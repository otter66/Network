// TCP Server IPv6

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVERPORT 9000
#define BUFSIZE 512

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>
#include <WS2tcpip.h>   //WSAStringToAddress �Լ� ��

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
    int retval; // ���� ������� ���� ���� -> bind, listen, ó�� ��� 

    // 1. initial winsock 
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) return 1;

    // 2. create socket
    SOCKET listen_sock = socket(AF_INET6, SOCK_STREAM, 0);  // socket(�ּ�ü��, ����Ÿ��, ��������)
    if (listen_sock == INVALID_SOCKET) err_quit("socket error");

    // 3. server IP address, port number setting(bind)
    SOCKADDR_IN6 serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));    // ���� �ּ� ������ 0���� �ʱ�ȭ
    serveraddr.sin6_family = AF_INET6; //�ּ� ü�� (IPv4, IPv6)
    serveraddr.sin6_addr = in6addr_any;
    serveraddr.sin6_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)); // bind(����, �ּ�, ũ��)
    if (retval == SOCKET_ERROR) err_quit("bind error");

    // 4. ���� ����, ���� ��� ����
    retval = listen(listen_sock, SOMAXCONN);    // listen(����, ���� Ŭ���̾�Ʈ ����)
    if (retval == SOCKET_ERROR) err_quit("listen error");

    //�����غ�
    //--------------------------------------------------------------------------------------

    SOCKET client_sock; // ���� ����
    SOCKADDR_IN6 clientaddr; //���� Ŭ���̾�Ʈ ����
    int addrlen;    //�ּ� ����
    char buf[BUFSIZE + 1];  //������ �������

    while (1) {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);    //accept (��������, Ŭ���̾�Ʈ �ּ�, ����)
        if (client_sock == INVALID_SOCKET) {
            err_display("accept error");
            break;
        }

        // ���� Ŭ���̾�Ʈ ���
        char ipaddr[50];
        DWORD ipaddrlen = sizeof(ipaddr);
        WSAAddressToString((SOCKADDR*)&clientaddr, sizeof(clientaddr), NULL, ipaddr, &ipaddrlen);
        printf("\nTCP ���� Ŭ���̾�Ʈ ����: IP�ּ� = %s, ��Ʈ��ȣ = %d\n", ipaddr, ntohs(clientaddr.sin6_port));

        while (1) {
            retval = recv(client_sock, buf, BUFSIZE, 0);    //recv(����, ���� ����, ũ��, 0)
            if (retval == SOCKET_ERROR) {
                err_display("recv error");
                break;
            }
            else if (retval == 0) break;

            buf[retval] = '\0'; //buffer end ǥ��

            printf("\n[TCP%s:%d] %s", ipaddr, ntohs(clientaddr.sin6_port), buf);

            retval = send(client_sock, buf, retval, 0);
            if (retval == SOCKET_ERROR) {
                err_display("send error");
                break;
            }
        }
        closesocket(client_sock);
        printf("\nTCP ���� Ŭ���̾�Ʈ ����: IP�ּ� = %s, ��Ʈ��ȣ = %d\n", ipaddr, ntohs(clientaddr.sin6_port));
    }   // accept ������
    closesocket(listen_sock);

    WSACleanup();
}