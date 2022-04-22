// 공유변수에서 임계영역을 사용하지 않았을 때 (방법1)

#pragma comment(lib, "ws2_32")

#define MAXCNT 10000000
#include <stdio.h>
#include <windows.h>

int g_count = 0;		// 공유 변수
CRITICAL_SECTION cs;	// 임계 영역 구조체 변수

DWORD WINAPI MyThread1(LPVOID arg) {
	for (int i = 0; i < MAXCNT; i++) {
		// 2. 임계 영역에 들어감
		EnterCriticalSection(&cs);
		g_count += 2;
		// 3. 임계 영역 탈출
		LeaveCriticalSection(&cs);
	}

	return 0;
}

DWORD WINAPI MyThread2(LPVOID arg) {
	for (int i = 0; i < MAXCNT; i++) {
		EnterCriticalSection(&cs);
		g_count -= 2;
		LeaveCriticalSection(&cs);
	}

	return 0;
}

int main() {

	// 1. 임계 영역 초기화
	InitializeCriticalSection(&cs);

	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);

	WaitForMultipleObjects(2, hThread, true, INFINITE);

	// 4. 임계 영역 삭제
	DeleteCriticalSection(&cs);

	printf("g_count = %d\n", g_count);
}
