// TCP Client : 고정 + 가변
// 메시지 입력 -> 서버에 보내기

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

// 소켓 함수 오류 (강제종료)
void err_quit(const char* msg) { exit(1); }

// 소켓 함수 오류 출력 
void err_display(const char* msg) { printf("오류"); }

// 데이터 수신 함수 (사용자 정의 함수)
int recvn(SOCKET s, char* buf, int len, int flags) {
    int received;       // 수신 데이터 값 처리
    char* ptr = buf;    // 수신 버퍼 시작 위치
    int left = len;     // 수신 버퍼에 아직 읽지 않은 값

    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR) return SOCKET_ERROR;
        else if (received == 0) break;

        left -= received;   // 읽은 남은 값
        ptr += received;    // 읽은 값
    }

    return (len - left); //실제 읽은 값 반환
}


int main() {
    int retval;
    const char* chatid;
    chatid = "클라이언트 이름";

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

    //4. 데이터 통신에 사용할 변수
    char msg[BUFSIZE + 1];  // 고정 길이 데이터 전송을 위한 배열
    int len;    // 고정길이 체크 변수
    char* buf;

    sprintf(msg, "[%s]", chatid);
    buf = msg + strlen(msg);

    while(1) {
        // 보내기 위한 준비
        printf("[%s]", chatid);
        if (fgets(buf, BUFSIZE + 1, stdin) == NULL) break;
        
        len = strlen(msg);
        if (msg[len - 1] == '\n') msg[len - 1] = '\0';
        if (strlen(buf) == 0) break;

        //고정길이 보내기
        retval = send(sock, (char*)&len, sizeof(int), 0); //송신 버퍼를 통해 서버에 보냄
        if (retval == SOCKET_ERROR) {
            err_display("send() error");
            break;
        }

        //가변길이 보내기
        retval = send(sock, buf, len, 0); //송신 버퍼를 통해 서버에 보냄
        if (retval == SOCKET_ERROR) {
            err_display("send() error");
            break;
        }

        // 고정길이 수신
        retval = recvn(sock, (char*)&len, sizeof(int), 0);   //recv(소켓, 읽을 공간, 크기, 0)
        if (retval == SOCKET_ERROR) {
            err_display("recv error");
            break;
        }
        else if (retval == 0) break;

        // 가변길이 수신
        retval = recvn(sock, buf, len, 0);   //recv(소켓, 읽을 공간, 크기, 0)
        if (retval == SOCKET_ERROR) {
            err_display("recv error");
            break;
        }
        else if (retval == 0) break;

        // 받은 데이터 출력
        buf[retval] = '\0';
        printf("%s\n", buf);
    }

    closesocket(sock);

    WSACleanup();
}