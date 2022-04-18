// UDP Client IPv6

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVERIP "::1"  // = 0:0:0:0:0:0:0:1
#define SERVERPORT 9000
#define BUFSIZE 512

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>
#include <WS2tcpip.h>

// 소켓 함수 오류 (강제종료)
void err_quit(const char* msg) { exit(1); }

// 소켓 함수 오류 출력 
void err_display(const char* msg) { printf("오류"); }

int main() {
    int retval;

    // 1. initial winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) return 1;

    //2. create socket
    SOCKET sock = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket() error");

    //3. connect (server IP, port number)
    SOCKADDR_IN6 serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
    int addrlen = sizeof(serveraddr);
    WSAStringToAddress((LPSTR)SERVERIP, AF_INET6, NULL, (SOCKADDR*)&serveraddr, &addrlen);
    serveraddr.sin6_port = htons(SERVERPORT);

    //4. 보내기 위한 준비
    SOCKADDR_IN peeraddr;
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
        printf("[UDP클라이언트]%d 바이트를 보냈습니다.\n", retval);

        //receive
        addrlen = sizeof(peeraddr);
        retval = recvfrom(sock, buf, retval, 0, (SOCKADDR*)&peeraddr, &addrlen);
        if (retval == SOCKET_ERROR) err_display("receive error");
        else if (retval == 0) continue;

        if (memcmp(&peeraddr, &serveraddr, sizeof(peeraddr))) {
            printf("잘못된 데이터");
            continue;
        }

        buf[retval] = '\0';
        printf("[UDP클라이언트]%d 바이트를 받았습니다. \n", retval);
        printf("[받은 데이터]%s\n", buf);
    }

    closesocket(sock);
    WSACleanup();
}