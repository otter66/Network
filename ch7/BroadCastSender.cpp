// UDP Client IPv4

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVERIP "255.255.255.255"
#define SERVERPORT 9000
#define BUFSIZE 512

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>

// 소켓 함수 오류 (강제종료)
void err_quit(const char* msg) { exit(1); }

// 소켓 함수 오류 출력 
void err_display(const char* msg) { printf("오류"); }

int main() {
    int retval;

    // 1. initial winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) return 1;

    // 2. create socket
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket() error");

    // 3. 소켓 옵션 (소켓 기능 확장)
    BOOL bEnable = TRUE;
    retval = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&bEnable, sizeof(bEnable));  //(소켓명, 레벨, 옵션명, 옵션값, 크기)
    if (retval == SOCKET_ERROR) {
        err_quit("set sock error");
    }

    // 4. connect (server IP, port number)
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);   // 255.255.255.255
    serveraddr.sin_port = htons(SERVERPORT);

    // 5. 데이터 보내기 준비 (통신 준비)
    SOCKADDR_IN peeraddr;
    int addrlen;
    char buf[BUFSIZE + 1];
    int len;

    while (1) {
        printf("\n[보낼 데이터]:");
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
        printf("[UDP클라이언트 %d 바이트를 보냈습니다.\n", retval);
    }

    closesocket(sock);
    WSACleanup();
}