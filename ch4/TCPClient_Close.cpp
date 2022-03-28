// TCP Client (접속 후 종료)
// 반복할 때마다 소켓 생성 -> 연결 -> 전송 -> 종료

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 50

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

    // initial winsock
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) return 1;

    // connect setting (server IP, port number)
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);

    //4. 데이터 통신에 사용할 변수
    char buf[BUFSIZE];
    // 고정 길이 데이터 전송을 위한 배열
    const char* testdata[] = {
        "안녕하세요",
        "안녕 세상",
        "반갑습니다",
        "hello world"
    };

    int len;

    for (int i = 0; i < 4; i++) {
        // create socket
        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) err_quit("socket() error");

        // connect
        retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
        if (retval == SOCKET_ERROR) err_quit("connect() error");

        len = strlen(testdata[i]);
        strncpy(buf, testdata[i], strlen(testdata[i]));

        //send
        retval = send(sock, buf, len, 0); //송신 버퍼를 통해 서버에 보냄
        if (retval == SOCKET_ERROR) {
            err_display("send() error");
            break;
        }
        printf("[TCP클라이언트]%d 바이트를 보냈습니다.\n", retval);

        closesocket(sock);
    }

    WSACleanup();
}

