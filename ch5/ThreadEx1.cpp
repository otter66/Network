// Thread 생성, 종료
// 스레드에 사용 될 함수 정의
// createThread() 함수 호출하며 실행

#pragma comment(lib, "ws2_32")

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

struct Point3D {
	int x, y, z;
};

// 스레드 작업 함수 정의
DWORD WINAPI MyThread(LPVOID arg) {
	Point3D* pt = (Point3D*)arg;	// 매개변수 값을 구조체로 형변환

	while (1) {
		printf("running thread() %d: %d, %d, %d\n", GetCurrentThreadId(), pt->x, pt->y, pt->z);
	
		Sleep(1000);
	}

	return 0;
}

int main() {
	// 스레드 생성
	Point3D pt1 = { 10, 20, 30 };
	HANDLE hThread1 = CreateThread(NULL, 0, MyThread, &pt1, 0, NULL);	//보안, 크기, 스레드 함수, 매개변수, 실행 여부, 스레드 id
	if (hThread1 == NULL) return 1;
	CloseHandle(hThread1);	//스레드 종료

	Point3D pt2 = { 40, 50, 60 };
	HANDLE hThread2 = CreateThread(NULL, 0, MyThread, &pt2, 0, NULL);	//보안, 크기, 스레드 함수, 매개변수, 실행 여부, 스레드 id
	if (hThread2 == NULL) return 1;
	CloseHandle(hThread2);	//스레드 종료
	// 메인 -> 스레드1 -> 스레드2 -> 반복

	while (1) {
		printf("running main%d\n", GetCurrentThreadId());
		Sleep(1000);
	}

}