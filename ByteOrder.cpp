// #pragma comment(lib, "ws2_32") //�Ӽ����� ������ ����
#include <stdio.h>

#include <WinSock2.h>
#include <Windows.h>

int main()
{
    WSADATA wsa;    //���� �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }
    
    u_short x1 = 0x1234;
    u_long y1 = 0x12345678;
    u_short x2;
    u_short y2;

    printf("[ȣ��Ʈ -> ��Ʈ��ũ]\n");
    printf("0x%x -> 0x%x\n", x1, x2 = htons(x1));
    printf("0x%x -> 0x%x\n", y1, y2 = htonl(y1));

    printf("\n[��Ʈ��ũ -> ȣ��Ʈ]\n");
    printf("0x%x -> 0x%x\n", x2, ntohs(x2));
    printf("0x%x -> 0x%x\n", y2, ntohl(y2));

    WSACleanup();   //���� ����, ����, ����

}
