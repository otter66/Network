// TCP Server(IPv4) : 구조체 메시지 설계
// 클라(입력) -> 서버(연산)

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>

#include <WinSock2.h>

#define SERVERPORT 9000
#define BUFSIZE 512

// 소켓 함수 오류 출력 및 종료
void err_quit(const char* msg) {
    exit(1);    // 강제종료
}

// 소켓 함수 오류 메시지 출력 
void err_display(const char* msg) {
    printf("오류");
}

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

int main()
{
    int retval; // 서버 연결관리 관련 변수 -> bind, listen, 처리 결과 

    // 1. initial winsock 
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa)) return 1;

    // 2. create socket
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);  // socket(주소체계, 소켓타입, 프로토콜)
    if (listen_sock == INVALID_SOCKET) err_quit("socket error");

    // 3. server IP address, port number setting(bind)
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));    // 서버 주소 변수를 0으로 초기화
    serveraddr.sin_family = AF_INET; //주소 체계 (IPv4, IPv6)
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // INADDR_ANY: 자동 IP 설정
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)); // bind(소켓, 주소, 크기)
    if (retval == SOCKET_ERROR) err_quit("bind error");

    // 4. 서버 실행, 리슨 대기 상태
    retval = listen(listen_sock, SOMAXCONN);    // listen(소켓, 연결 클라이언트 개수)
    if (retval == SOCKET_ERROR) err_quit("listen error");

    //연결준비
    //--------------------------------------------------------------------------------------

    SOCKET client_sock; // 연결 소켓
    SOCKADDR_IN clientaddr; //연결 클라이언트 소켓
    int addrlen;    //주소 길이
    char buf[BUFSIZE + 1];  //데이터 저장공간

    //메시지 설계 ==> 구조체
    struct MyData {
        int n1;
        int n2;
    }data;

    int result; //결과

    while (1) {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);    //accept (서버소켓, 클라이언트 주소, 길이)
        if (client_sock == INVALID_SOCKET) {
            err_display("accept error");
            break;
        }

        printf("\nTCP 서버 클라이언트 접속: IP주소 = %s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));    //접속 클라이언트 정보 출력

        while (1) {
            retval = recvn(client_sock, (char*)&data, sizeof(data), 0);    //recv(소켓, 읽을 공간, 크기, 0)
            if (retval == SOCKET_ERROR) {
                err_display("recv error");
                break;
            }
            else if (retval == 0) break;

            buf[retval] = '\0'; //buffer end 표시

            printf("[TCP%s:%d] %d + %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), data.n1, data.n2);

            result = data.n1 + data.n2;

            retval = send(client_sock, (char*)&result, sizeof(result), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send error");
                break;
            }
        }
        closesocket(client_sock);

        printf("\nTCP 서버 클라이언트 종료: IP주소 = %s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    }   // accept 마무리
    closesocket(listen_sock);

    //WSACleanup();
}