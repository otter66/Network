// #pragma comment(lib, "ws2_32") //�Ӽ����� ������ ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

int main()
{
    WSADATA wsa;    //���� �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }

    const char* ipv4test = "127.0.0.1";
    printf("Ipv4 �ּ� ��ȯ �� = %s\n", ipv4test);

    //inet_addr()
    printf("ipv4 �ּ� ��ȯ �� = 0x%x\n", inet_addr(ipv4test));

    //inet_ntoa()
    IN_ADDR ipv4num;
    ipv4num.s_addr = inet_addr(ipv4test);
    printf("IPv4 �ּ� �纯ȯ = %s\n", inet_ntoa(ipv4num));

    printf("-------------------------------------------------------------------------\n");

    //ipv6 �ּ� ��ȯ
    const char* ipv6test = "0:0:0:0:0:0:0:1";
    printf("ipv6 �ּ� ��ȯ �� = %s\n", ipv6test);

    //WSAStringtoAddress
    SOCKADDR_IN6 ipv6num;
    int addrlen = sizeof(ipv6num);

    WSAStringToAddress((LPSTR)ipv6test, AF_INET6, NULL, (SOCKADDR*)&ipv6num, &addrlen);

    printf("IPv6 �ּ� ��ȯ �� = 0x");
    for (int i = 0; i < 16; i++) {
        printf("%02x", ipv6num.sin6_addr.u.Byte[i]);
    }

    char ipaddr[50];
    DWORD ipaddrlen = sizeof(ipaddr);
    WSAAddressToString((SOCKADDR*)&ipv6num, sizeof(ipv6num), NULL, ipaddr, &ipaddrlen);

    printf("IPv6(�ּ� ��ȯ ��) = %s\n", ipaddr);
}
