// ������������ �Ӱ迵���� ������� �ʾ��� �� (���1)

#pragma comment(lib, "ws2_32")

#define MAXCNT 10000000
#include <stdio.h>
#include <windows.h>

int g_count = 0;		// ���� ����
CRITICAL_SECTION cs;	// �Ӱ� ���� ����ü ����

DWORD WINAPI MyThread1(LPVOID arg) {
	for (int i = 0; i < MAXCNT; i++) {
		// 2. �Ӱ� ������ ��
		EnterCriticalSection(&cs);
		g_count += 2;
		// 3. �Ӱ� ���� Ż��
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

	// 1. �Ӱ� ���� �ʱ�ȭ
	InitializeCriticalSection(&cs);

	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);

	WaitForMultipleObjects(2, hThread, true, INFINITE);

	// 4. �Ӱ� ���� ����
	DeleteCriticalSection(&cs);

	printf("g_count = %d\n", g_count);

}