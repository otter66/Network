// TCP Server (IPv4) -> MultiThread

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

// Ŭ���̾�Ʈ�� ������ ��� (recv, send)
DWORD WINAPI ProcessClient(LPVOID arg) {

    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    addrlen = sizeof(clientaddr);

    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen); // ����, �����ּ�, ����

    //Ŭ���̾�Ʈ�� ���
    while (1) {
        retval = recv(client_sock, buf, BUFSIZE, 0);    //recv(����, ���� ����, ũ��, 0)
        if (retval == SOCKET_ERROR) {
            err_display("recv error");
            break;
        }
        else if (retval == 0) break;

        buf[retval] = '\0'; //buffer end ǥ��

        printf("\n[TCP%s:%d] %s", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

        retval = send(client_sock, buf, retval, 0);
        if (retval == SOCKET_ERROR) {
            err_display("send error");
            break;
        }
    }

    closesocket(client_sock);

    printf("\nTCP ���� Ŭ���̾�Ʈ ����: IP�ּ� = %s, ��Ʈ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    return 0;
}

// ������ �۾��� �ʿ��� �Լ� ����
// DWORD WINAPI �Լ���(�Ű�����) ==> �Ű������� ����ȯ

// CreateThread(NULL, 0, �Լ���, �Ű�����, ���࿩��(0=�ٷν���), NULL)
// CREATE_SUSPEND => Resum..�� �� ������ ��ٸ� �� ����
// getpeername(): ���� ip/port ��ȣ üũ�ϴ� �Լ�

int main()
{
    int retval; // ���� ������� ���� ���� -> bind, listen, ó�� ��� 

    // 1. initial winsock 
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) return 1;

    // 2. create socket
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);  // socket(�ּ�ü��, ����Ÿ��, ��������)
    if (listen_sock == INVALID_SOCKET) err_quit("socket error");

    // 3. server IP address, port number setting(bind)
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));    // ���� �ּ� ������ 0���� �ʱ�ȭ
    serveraddr.sin_family = AF_INET; //�ּ� ü�� (IPv4, IPv6)
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY: �ڵ� IP ����
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)); // bind(����, �ּ�, ũ��)
    if (retval == SOCKET_ERROR) err_quit("bind error");

    // 4. ���� ����, ���� ��� ����
    retval = listen(listen_sock, SOMAXCONN);    // listen(����, ���� Ŭ���̾�Ʈ ����)
    if (retval == SOCKET_ERROR) err_quit("listen error");

    //�����غ�
    //--------------------------------------------------------------------------------------

    SOCKET client_sock; // ���� ����
    SOCKADDR_IN clientaddr; //���� Ŭ���̾�Ʈ ����
    int addrlen;    //�ּ� ����
    char buf[BUFSIZE + 1];  //������ �������

    HANDLE hThread;

    while (1) {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);    //accept (��������, Ŭ���̾�Ʈ �ּ�, ����)
        if (client_sock == INVALID_SOCKET) {
            err_display("accept error");
            break;
        }

        printf("\nTCP ���� Ŭ���̾�Ʈ ����: IP�ּ� = %s, ��Ʈ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));    //���� Ŭ���̾�Ʈ ���� ���
        
        //������ ����
        hThread = CreateThread(NULL, 0, ProcessClient, (LPVOID)client_sock, 0, NULL);
        if (hThread == NULL) closesocket(client_sock);
        else CloseHandle(hThread);

    }   // accept ������

    closesocket(listen_sock);
    WSACleanup();
}