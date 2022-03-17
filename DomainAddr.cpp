// #pragma comment(lib, "ws2_32") //속성에서 설정을 해줌
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
        printf("오류");
        return false;
    }

    memcpy(addr, ptr->h_addr_list, ptr->h_length);
    return true;
}

//IP Address -> Domain
BOOL GetDomainName(IN_ADDR addr, char* name, int namelen) {

    HOSTENT* ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);

    if (ptr == NULL || ptr->h_addrtype != AF_INET) {
        printf("오류");
        return false;
    }

    memcpy(name, ptr->h_name, namelen);
    return true;
}

int main()
{
    WSADATA wsa;    //윈속 초기화
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }

    printf("도메인 이름 변환 전 = %s\n", TESTNAME);


    //domain -> ip
    IN_ADDR addr;
    if (GetIPAddr(TESTNAME, &addr)) {
        printf("IP 주소 변환 후 = %s\n", inet_ntoa(addr));
    }

    //ip -> domain
    char name[256];
    if (GetDomainName(addr, name, sizeof(name))) {
        printf("도메인 변화 후 = %s\n", name);
    }
}
