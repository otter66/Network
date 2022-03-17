// #pragma comment(lib, "ws2_32") //속성에서 설정을 해줌
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>
#include <Windows.h>

void err_display(char *msg) {
    printf("오류");
}

int main()
{
    WSADATA wsa;    //윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }
    MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);     //메시지, 알림 제목, 버튼 이름

    SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);   //소켓 생성(주소 체계, ,)
    if (tcp_sock == INVALID_SOCKET) { printf("socket 오류"); }
    MessageBox(NULL, "TCP소켓 성공", "알림", MB_OK);

    closesocket(tcp_sock);
    WSACleanup();   //윈속 종료

}
