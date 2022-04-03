# TCP Server - TCP Client 간의 데이터의 송수신

<br>   

## TCP Server 
- <ins>Bind(접속 수용 목적의 소켓, 소켓 구조체(IP), 주소 구조체 길이)</ins>: 소켓의 IP주소와 포트번호   
- <ins>Listen(접속 수용 목적의 소켓, 접속 가능 클라이언트 개수)</ins>: 소켓의 TCP 상태를 LISTENING 대기상태로 만듦 (다수의 클라이언트 접속 시에는 큐에 저장해 처리)   
- <ins>Accept(서버소켓, 클라이언트 주소, 소켓 주소 구조체 길이)</ins>: 접속한 클라이언트와 통신할 수 있도록 새로운 소켓을 생성   

<br><br>   

## TCP Client
- <ins>Connect(서버와 통신 목적의 소켓, 소켓 구조체(서버 주소), 소켓 구조체 길이)</ins>: 서버에 연결   

<br><br>   

## Data send and receive
- <ins>send(연결된 소켓, 보낼 데이터를 담은 버퍼, 데이터 크기, send함수 동작 변경)</ins>: 오류 없이 전송 성공 시에는 전송된 byte 수 리턴, 오류 시에는 SOCKET_ERROR 반환. [(링크)](https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-send)
- <ins>recv(소켓, 버퍼, 크기, 플래그)</ins>: 오류가 없으면 수신 바이트 수 반환, 정상 접속 종료 시에는 0 리턴. [(링크)](https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-recv)

