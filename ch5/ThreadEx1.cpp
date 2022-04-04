// Thread ����, ����
// �����忡 ��� �� �Լ� ����
// createThread() �Լ� ȣ���ϸ� ����

#pragma comment(lib, "ws2_32")

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

struct Point3D {
	int x, y, z;
};

// ������ �۾� �Լ� ����
DWORD WINAPI MyThread(LPVOID arg) {
	Point3D* pt = (Point3D*)arg;	// �Ű����� ���� ����ü�� ����ȯ

	while (1) {
		printf("running thread() %d: %d, %d, %d\n", GetCurrentThreadId(), pt->x, pt->y, pt->z);
	
		Sleep(1000);
	}

	return 0;
}

int main() {
	// ������ ����
	Point3D pt1 = { 10, 20, 30 };
	HANDLE hThread1 = CreateThread(NULL, 0, MyThread, &pt1, 0, NULL);	//����, ũ��, ������ �Լ�, �Ű�����, ���� ����, ������ id
	if (hThread1 == NULL) return 1;
	CloseHandle(hThread1);	//������ ����

	Point3D pt2 = { 40, 50, 60 };
	HANDLE hThread2 = CreateThread(NULL, 0, MyThread, &pt2, 0, NULL);	//����, ũ��, ������ �Լ�, �Ű�����, ���� ����, ������ id
	if (hThread2 == NULL) return 1;
	CloseHandle(hThread2);	//������ ����
	// ���� -> ������1 -> ������2 -> �ݺ�

	while (1) {
		printf("running main%d\n", GetCurrentThreadId());
		Sleep(1000);
	}

}