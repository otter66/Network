// #pragma comment(lib, "ws2_32") //�Ӽ����� ������ ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#define TESTNAME "www.example.com"

//Domain -> IP Address
BOOL GetIPAddr(const char* name, IN_ADDR* addr) {
    
    HOSTENT* ptr = gethostbyname(name);

    if (ptr == NULL || ptr->h_addrtype != AF_INET) {
        printf("����");
        return false;
    }

    memcpy(addr, ptr->h_addr_list, ptr->h_length);
    return true;
}

//IP Address -> Domain
BOOL GetDomainName(IN_ADDR addr, char* name, int namelen) {

    HOSTENT* ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);

    if (ptr == NULL || ptr->h_addrtype != AF_INET) {
        printf("����");
        return false;
    }

    memcpy(name, ptr->h_name, namelen);
    return true;
}

int main()
{
    WSADATA wsa;    //���� �ʱ�ȭ
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }

    printf("������ �̸� ��ȯ �� = %s\n", TESTNAME);


    //domain -> ip
    IN_ADDR addr;
    if (GetIPAddr(TESTNAME, &addr)) {
        printf("IP �ּ� ��ȯ �� = %s\n", inet_ntoa(addr));
    }

    //ip -> domain
    char name[256];
    if (GetDomainName(addr, name, sizeof(name))) {
        printf("������ ��ȭ �� = %s\n", name);
    }
}
