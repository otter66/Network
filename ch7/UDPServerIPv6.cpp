// UDP Server IPv6

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define SERVERPORT 9000
#define BUFSIZE 512

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>
#include <WS2tcpip.h>   //WSAStringToAddress 함수 등

// 소켓 함수 오류 출력 및 종료
void err_quit(const char* msg) {
    exit(1);    // 강제종료
}

// 소켓 함수 오류 메시지 출력 
void err_display(const char* msg) {
    printf("오류");
}

int main()
{
    int retval; // 서버 연결관리 관련 변수 -> bind, listen, 처리 결과 

    // 1. initial winsock 
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) return 1;

    // 2. create socket
    SOCKET listen_sock = socket(AF_INET6, SOCK_DGRAM, 0);  // socket(주소체계, 소켓타입, 프로토콜)
    if (listen_sock == INVALID_SOCKET) err_quit("socket error");

    // 3. server IP address, port number setting(bind)
    SOCKADDR_IN6 serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));    // 서버 주소 변수를 0으로 초기화
    serveraddr.sin6_family = AF_INET6; //주소 체계 (IPv4, IPv6)
    serveraddr.sin6_addr = in6addr_any;
    serveraddr.sin6_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)); // bind(소켓, 주소, 크기)
    if (retval == SOCKET_ERROR) err_quit("bind error");

    //연결준비
    //--------------------------------------------------------------------------------------

    SOCKADDR_IN6 clientaddr; //연결 클라이언트 소켓
    int addrlen;    //주소 길이
    char buf[BUFSIZE + 1];  //데이터 저장공간

    while (1) {
        addrlen = sizeof(clientaddr);

        retval = recvfrom(listen_sock, buf, BUFSIZE, 0, (SOCKADDR*)&clientaddr, &addrlen);
        //recv(소켓, 읽을 공간, 크기,0);//데이터를 읽어들임
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            continue;
        }

        char ipaddr[50];
        DWORD ipaddrlen = sizeof(ipaddr);
        WSAAddressToString((SOCKADDR*)&clientaddr, sizeof(clientaddr), NULL, ipaddr, &ipaddrlen);
        printf("\nUDP 서버 클라이언트 접속: IP주소 = %s, 포트번호 = %d\n", ipaddr, ntohs(clientaddr.sin6_port));

        buf[retval] = '\0'; //buffer end 표시
        printf("\n[UDP%s:%d] %s", ipaddr, ntohs(clientaddr.sin6_port), buf);

        retval = sendto(listen_sock, buf, retval, 0, (SOCKADDR*)&clientaddr, sizeof(clientaddr));
        if (retval == SOCKET_ERROR) {
            err_display("send error");
            continue;
        }
    }   // accept 마무리
    closesocket(listen_sock);

    WSACleanup();
}