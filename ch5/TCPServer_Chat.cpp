// TCP Server : 고정(4byte) + 가변(배열 값)
// 클라 -> 서버 -> 클라 -> (1:1)

#pragma comment(lib, "ws2_32")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

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

// 데이터 수신 함수 (사용자 정의 함수)
int recvn(SOCKET s, char* buf, int len, int flags) {
    int received;
    char* ptr = buf;    //읽을 위치
    int left = len;

    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR) return SOCKET_ERROR;
        else if (received == 0) break;

        left -= received;
        ptr += received;
    }

    return (len - left);
}


int main()
{
    int retval; // 서버 연결관리 관련 변수 -> bind, listen, 처리 결과 

    const char* chatid;
    chatid = "서버 아이디>";

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
    int addrlen;    // 주소 길이

    char msg[BUFSIZE + 1];  // 전달 데이터 저장공간
    int len;    // 고정 길이 체크
    char* buf;

    sprintf(msg, "[%s]", chatid);
    buf = msg + strlen(msg);

    while (1) {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);    //accept (서버소켓, 클라이언트 주소, 길이)
        if (client_sock == INVALID_SOCKET) {
            err_display("accept error");
            break;
        }

        printf("\nTCP 서버 클라이언트 접속: IP주소 = %s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));    //접속 클라이언트 정보 출력

        while (1) {
            // 고정길이 수신
            retval = recvn(client_sock, (char*)&len, sizeof(int), 0);   //recv(소켓, 읽을 공간, 크기, 0)
            if (retval == SOCKET_ERROR) {
                err_display("recv error");
                break;
            }
            else if (retval == 0) break;

            // 가변길이 수신
            retval = recvn(client_sock, buf, len, 0);   //recv(소켓, 읽을 공간, 크기, 0)
            if (retval == SOCKET_ERROR) {
                err_display("recv error");
                break;
            }
            else if (retval == 0) break;

            // 받은 데이터 출력
            buf[retval] = '\0'; //buffer end 표시
            printf("%s\n", buf);

            // 보내기 준비
            printf("[%s]", chatid);
            if (fgets(buf, BUFSIZE + 1, stdin) == NULL) break;

            // 입력받은 값 처리
            len = strlen(msg);
            if (msg[len - 1] == '\n') msg[len - 1] = '\0';
            if (strlen(buf) == 0) break;

            // send data (고정, 가변)
            retval = send(client_sock, (char*)&len, sizeof(int), 0);
            if (retval == SOCKET_ERROR) {
                err_display("send");
                break;
            }
            retval = send(client_sock, msg, len, 0);
            if (retval == SOCKET_ERROR) {
                err_display("send");
                break;
            }

        }
        closesocket(client_sock);
        printf("\nTCP 서버 클라이언트 종료: IP주소 = %s, 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
    }   // accept 마무리
    closesocket(listen_sock);

    //WSACleanup();
}
