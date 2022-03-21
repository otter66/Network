# Winsock (windows Socket API 2.2)
##### TCP/IP, IPv6, Bluetooth, IPX 등의 네트워크 프로토콜에 대한 프로그래밍 인터페이스를 제공.   
- 준비: WinSock2.h 파일을 포함, ws2_32.dll을 동적 링크.   
<br>   

1. **<윈속 초기화>**
    - WSADATA structure: 윈도우 소켓버전, 최대 사용할 수 소켓의수 정보 제공   
    - <ins>WSAStartup(윈속 버전, 초기화할 윈속 주소)</ins>를 이용해 초기화 (성공시 0 반환)   
<br>   

2. **<소켓 생성>**   
    - <ins>socket(도메인/주소체계, 소켓 타입, 프로토콜)</ins>을 이용해 소켓 생성   
    - 리소스를 할당, 접근할 수 있는 핸들 값(SOCKET 타입, 32비트 정수)인 socket descriptor를 반환
    - Domain 
        * AF_INET (2):  (UDP, TCP, etc)
        * AF_INET6 (23): (Internetwork Version 6)
        * AF_IRDA (26): IrDA
        * AF_BTH (32): Bluetooth
    - Socket Type
        * SOCK_STREAM: 신뢰성o 데이터 전송시 이용, 연결형 프로토콜(TCP/IP)기반 양방향 통신
        * SOCK_DGRAM: 신뢰성x 데이터 전송시 이용, 비연결형 프로토콜(UDP/IP)기반의 통신
        * SOCK_RAW: IP헤더를 직접 제어하기 위해 사용
<br>   

3. **<<ins>네트워크 통신</ins>>**   
<br>   

4. **<소켓 닫기>**   
<ins>closeSocket(소켓)</ins>를 이용해 소켓을 닫고, 소켓 리소스를 반환

<br>   

5. **<윈속 종료>**   
<ins>WSACleanup()</ins>을 이용해 윈속 사용을 종료, 윈속 리소스를 반환   


<br><br><br><br>   

# Byte Order
- 호스트 바이트 정렬 → 네트워크 바이트 정렬: htons(), htonl(), WSAHtons(), WSAHtonl()   
- 네트워크 바이트 정렬 → 호스트 바이트 정렬: ntohs(), ntohl(), WSANtohs(), WSANtohl()   
<br>   

- Big Endian: 최상위 바이트(MSB, Most Significant Byte)부터 순차 저장 방식. (ex. 네트워크 장비)   
- Little Endian: 최하위 바이트(LST, Least Significant Byte)부터 순차 저장 방식.   
<br>   

-  호스트 바이트 순서(HBO) 
    - CPU별 데이터 저장 방식
    - 대부분장비들이 저장하는 방식
    - Big Endian 기준(모토롤라)
    - Little Endia 기준(인텔)

- 네트워크 바이트 순서 (NBO)
    - 통일된 데이터 송수신 기준을 의미함 
    - 네트워크를 통해 데이터를 전송할 때에는 빅 엔디안이 기준   


<br><br><br><br>   

# IP 주소 변환
- inet_addr(const char*): 문자열 형태의 IPv4 주소 → 32비트 숫자(네트워크 바이트 정렬) 반환 
- inet_ntoa(struct in_addr): 32비트 숫자(네트워크 바이트 정렬) 형태의 IPv4 주소 → 문자열 형태의 주소값 반환
<br>   

- WSAStringToAddress(문자 형식 주소, 주소체계 정보, 프로토콜 프로바이더, 변환된 숫자를 저장할 구조체(SOCKADDR_IN/SOCKADDR_IN6), 주소 구조체 길이): 문자(IPv4/IPv6) 주소 → 숫자 반환   
- WSAAddressToString(구조체 형식 주소, 구조체 크기, 프로토콜 프로바이더, 변환된 문자를 저장할 배열, 주소 값의 크기): 숫자(IPv4/IPv6) 주소 → 문자 반환 


<br><br><br><br>   

# 도메인 이름과 주소 변환
- gethostbyname(도메인 이름): 도메인 이름 → IPv4/IPv6주소(네트워크 바이트 정렬)   
- gethostbyaddr(IP주소, 주소 길이, 주소 체계): IPv4/IPv6주소(네트워크 바이트 정렬) → 도메인 이름   
<br>   

- hostent 구조체
    - h_name: 공식 도메인 이름
    - h_aliases: 별명
    - h_addrtype: 주소 체계
    - h_length: 주소 길이
    - h_addr_list: 네트워크 바이트 정렬 IP주소. IP주소가 하나가 아닌 경우, 포인터를 따라가며 모든 IP주소를 얻을 수 있음.   
특정 호스트에 접속할때는 대부분 첫 번째 IP주소만 이용하므로 h_addr_list[0]을 접근.



