// 이벤트 객체를 이용해 동기화 (방법2)

#pragma comment(lib, "ws2_32")

#define BUFSIZE 10

#include <stdio.h>
#include <windows.h>

int buf[BUFSIZE];	// 공유 공간

HANDLE hReadEvent; // 이벤트 관련 변수
HANDLE hWriteEvent; 

// 스레드 작업 함수 (쓰기)
DWORD WINAPI WriteThread(LPVOID arg) {
	DWORD retval;

	for (int k = 1; k <= 500; k++) {
		retval = WaitForSingleObject(hReadEvent, INFINITE);	// 읽기 이벤트 신호가 될 때까지 대기
		if (retval != WAIT_OBJECT_0) break;
		
		for (int i = 0; i < BUFSIZE; i++) buf[i] = k;
		
		//이벤트 신호 변경
		SetEvent(hWriteEvent);	// 비신호 -> 신호
	}
	return 0;
}

// 스레드 작업 함수 (읽기)
DWORD WINAPI ReadThread(LPVOID arg) {
	DWORD retval;

	while (1) {
		retval = WaitForSingleObject(hWriteEvent, INFINITE);
		if (retval != WAIT_OBJECT_0) break;

		printf("Thread %4d: ", GetCurrentThreadId());
		for (int i = 0; i < BUFSIZE; i++) {
			printf("%4d", buf[i]);
		}
		printf("\n");
		ZeroMemory(buf, sizeof(buf));

		SetEvent(hReadEvent);	// 비신호 -> 신호
	}
	return 0;
}

int main() {
	// 이벤트 생성 
	hWriteEvent = CreateEvent(NULL, false, false, NULL);	//보안, 자동 리셋, 초기신호(비신호(스레드 종료 상태)), 함수이름
	if (hWriteEvent == NULL) return 1;
	hReadEvent = CreateEvent(NULL, false, true, NULL);		//보안, 자동 리셋, 초기신호(신호(스레드 작업 상태)), 함수이름

	// 스레드 생성
	HANDLE hThread[3];
	hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

	WaitForMultipleObjects(3, hThread, true, INFINITE);

	// 이벤트 마무리
	CloseHandle(hWriteEvent);
	CloseHandle(hReadEvent);
	return 0;
}