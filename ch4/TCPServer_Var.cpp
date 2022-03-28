// TCP Server : ���� ����
// �������̴� �޽��� ���� ǥ��
// ��ü �޽��� ���� �� �� ���ڸ� ����

//#pragma comment(lib, "ws2_21")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>

#define SERVERPORT 9000
#define BUFSIZE 512

// ���� �Լ� ���� ��� �� ����
void err_quit(const char* msg) {
    exit(1);    // ��������
}

// ���� �Լ� ���� �޽��� ��� 
void err_display(const char* msg) {
    printf("����");
}

// �Լ� ����
int _recv_head(SOCKET s, char* p) {
    // ���� ���ۿ� �ִ� ������ �� ���ھ� �������ִ� ����
    static int nbytes = 0;  // ���� ����Ʈ
    static char buf[1024];
    static char* ptr;   // �б� ������ ��ġ

    if (nbytes == 0 || nbytes == SOCKET_ERROR) {    // ���� ���� ���ۿ� �о���� �����Ͱ� ���ų� ��� �о ������ִ� ���
        nbytes = recv(s, buf, sizeof(buf), 0);
        if (nbytes == SOCKET_ERROR) return SOCKET_ERROR;
        else if (nbytes == 0) return 0;

        ptr = buf;
    }
    nbytes--;   // ���� ���� --
    *p = *ptr++;    // ������ ++

    return 1;
}

// ��ü �޽����� �б� ���� �Լ�
int recvline(SOCKET s, char* buf, int maxlen) {
    int n;  // ��ü ����
    int nbytes; // _recv_head �Լ��� ���Ϲ��� ��
    char c; // ���� ����
    char* ptr = buf; // ���� ��ġ

    for (n = 1; n < maxlen; n++) {
        nbytes = _recv_head(s, &c);
        if (nbytes == 1) {
            *ptr++ = c;
            if (c == '\n') break;   //���ڰ� �ٹٲ��̸� ��ü ���ڸ� ���� ���̹Ƿ� �ߴ�
        }
        else if (nbytes == 0) {
            *ptr = 0;
            return n - 1;
        }
        else return SOCKET_ERROR;
    }
    *ptr = 0;

    return n;
}

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

    while (1) {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);    //accept (��������, Ŭ���̾�Ʈ �ּ�, ����)
        if (client_sock == INVALID_SOCKET) {
            err_display("accept error");
            break;
        }

        printf("\nTCP ���� Ŭ���̾�Ʈ ����: IP�ּ� = %s, ��Ʈ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));    //���� Ŭ���̾�Ʈ ���� ���

        while (1) {
            retval = recvline(client_sock, buf, BUFSIZE + 1);   // ������ ���� => buf�� �д� �Լ� ȣ��
            if (retval == SOCKET_ERROR) {
                err_display("recv error");
                break;
            }
            else if (retval == 0) break;

            printf("\n[TCP%s:%d] %s", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
        }
        closesocket(client_sock);

        printf("\nTCP ���� Ŭ���̾�Ʈ ����: IP�ּ� = %s, ��Ʈ��ȣ = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    }   // accept ������

    closesocket(listen_sock);
    WSACleanup();
}