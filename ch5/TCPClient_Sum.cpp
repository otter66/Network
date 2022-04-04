// TCP Client(IPv4) : 
// 정수 두 개 입력 -> 서버(연산)

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#define SERVERIP "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE 512

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>

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
        if (received == SOCKET_ERROR) {
            return SOCKET_ERROR;
        }
        else if (received == 0) break;

        left -= received;   // 읽은 남은 값
        ptr += received;    // 읽은 값
    }

    return(len - left); //실제 읽은 값 반환
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

    //4. 데이터 통신에 사용할 변수
    char buf[BUFSIZE + 1];
    int len;

    // 메시지 설계
    struct MyData {
        int n1;
        int n2;
    } data;

    int result;
    int dummy;

    while (1) {
        printf("\n[숫자 두 개 입력]:");
        if (fgets(buf, BUFSIZE + 1, stdin) == NULL) break;

        len = strlen(buf);
        if (buf[len - 1] == '\n') buf[len - 1] = '\0';
        if (strlen(buf) == 0) break;

        if (sscanf_s(buf, "%d%d%d", &data.n1, &data.n2, &dummy) != 2) {
            fprintf(stderr, "오류메시지");
            continue;
        }

        //send
        retval = send(sock, (char*)&data, sizeof(data), 0); //송신 버퍼를 통해 서버에 보냄
        if (retval == SOCKET_ERROR) {
            err_display("send() error");
            break;
        }
        

        //receive
        retval = recvn(sock, (char*)&result, sizeof(result), 0);
        if (retval == SOCKET_ERROR) err_display("receive error");
        else if (retval == 0) break;

        buf[retval] = '\0';
        printf("[결과]%d + %d = %d \n", data.n1, data.n2, result);
    }

    closesocket(sock);

    WSACleanup();
}