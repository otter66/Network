// �̺�Ʈ ��ü�� �̿��� ����ȭ (���2)

#pragma comment(lib, "ws2_32")

#define BUFSIZE 10

#include <stdio.h>
#include <windows.h>

int buf[BUFSIZE];	// ���� ����

HANDLE hReadEvent; // �̺�Ʈ ���� ����
HANDLE hWriteEvent; 

// ������ �۾� �Լ� (����)
DWORD WINAPI WriteThread(LPVOID arg) {
	DWORD retval;

	for (int k = 1; k <= 500; k++) {
		retval = WaitForSingleObject(hReadEvent, INFINITE);	// �б� �̺�Ʈ ��ȣ�� �� ������ ���
		if (retval != WAIT_OBJECT_0) break;
		
		for (int i = 0; i < BUFSIZE; i++) buf[i] = k;
		
		//�̺�Ʈ ��ȣ ����
		SetEvent(hWriteEvent);	// ���ȣ -> ��ȣ
	}
	return 0;
}

// ������ �۾� �Լ� (�б�)
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

		SetEvent(hReadEvent);	// ���ȣ -> ��ȣ
	}
	return 0;
}

int main() {
	// �̺�Ʈ ���� 
	hWriteEvent = CreateEvent(NULL, false, false, NULL);	//����, �ڵ� ����, �ʱ��ȣ(���ȣ(������ ���� ����)), �Լ��̸�
	if (hWriteEvent == NULL) return 1;
	hReadEvent = CreateEvent(NULL, false, true, NULL);		//����, �ڵ� ����, �ʱ��ȣ(��ȣ(������ �۾� ����)), �Լ��̸�

	// ������ ����
	HANDLE hThread[3];
	hThread[0] = CreateThread(NULL, 0, WriteThread, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);
	hThread[2] = CreateThread(NULL, 0, ReadThread, NULL, 0, NULL);

	WaitForMultipleObjects(3, hThread, true, INFINITE);

	// �̺�Ʈ ������
	CloseHandle(hWriteEvent);
	CloseHandle(hReadEvent);
	return 0;
}