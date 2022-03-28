// TCP Server : 가변 길이
// 가변길이는 메시지 끝을 표현
// 전체 메시지 읽은 후 각 글자를 리턴

//#pragma comment(lib, "ws2_21")

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

// 함수 정의
int _recv_head(SOCKET s, char* p) {
    // 수신 버퍼에 있는 내용을 한 글자씩 리턴해주는 역할
    static int nbytes = 0;  // 읽은 바이트
    static char buf[1024];
    static char* ptr;   // 읽기 시작점 위치

    if (nbytes == 0 || nbytes == SOCKET_ERROR) {    // 소켓 수신 버퍼에 읽어들인 데이터가 없거나 모두 읽어서 비워져있는 경우
        nbytes = recv(s, buf, sizeof(buf), 0);
        if (nbytes == SOCKET_ERROR) return SOCKET_ERROR;
        else if (nbytes == 0) return 0;

        ptr = buf;
    }
    nbytes--;   // 남은 버퍼 --
    *p = *ptr++;    // 포인터 ++

    return 1;
}

// 전체 메시지를 읽기 위한 함수
int recvline(SOCKET s, char* buf, int maxlen) {
    int n;  // 전체 길이
    int nbytes; // _recv_head 함수로 리턴받은 값
    char c; // 읽을 문자
    char* ptr = buf; // 버퍼 위치

    for (n = 1; n < maxlen; n++) {
        nbytes = _recv_head(s, &c);
        if (nbytes == 1) {
            *ptr++ = c;
            if (c == '\n') break;   //문자가 줄바꿈이면 전체 문자를 읽은 것이므로 중단
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

    while (1) {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);    //accept (서버소켓, 클라이언트 주소, 길이)
        if (client_sock == INVALID_SOCKET) {
            err_display("accept error");
            break;
        }

        printf("\nTCP 서버 클라이언트 접속: IP주소 = %s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));    //접속 클라이언트 정보 출력

        while (1) {
            retval = recvline(client_sock, buf, BUFSIZE + 1);   // 데이터 수신 => buf값 읽는 함수 호출
            if (retval == SOCKET_ERROR) {
                err_display("recv error");
                break;
            }
            else if (retval == 0) break;

            printf("\n[TCP%s:%d] %s", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);
        }
        closesocket(client_sock);

        printf("\nTCP 서버 클라이언트 종료: IP주소 = %s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    }   // accept 마무리

    closesocket(listen_sock);
    WSACleanup();
}