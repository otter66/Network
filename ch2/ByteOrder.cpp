// #pragma comment(lib, "ws2_32") //속성에서 설정을 해줌
#include <stdio.h>

#include <WinSock2.h>
#include <Windows.h>

int main()
{
    WSADATA wsa;    //윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }
    
    u_short x1 = 0x1234;
    u_long y1 = 0x12345678;
    u_short x2;
    u_short y2;

    printf("[호스트 -> 네트워크]\n");
    printf("0x%x -> 0x%x\n", x1, x2 = htons(x1));
    printf("0x%x -> 0x%x\n", y1, y2 = htonl(y1));

    printf("\n[네트워크 -> 호스트]\n");
    printf("0x%x -> 0x%x\n", x2, ntohs(x2));
    printf("0x%x -> 0x%x\n", y2, ntohl(y2));

    WSACleanup();   //윈속 종료, 해제, 제거

}
