## Thread

- 프로세스: CPU를 할당 받아 실행 중인 프로그램   
- 스레드: 프로세스 내에서 실행되는 세부적인 작업 단위

<br>   

- 스레드 생성: <ins>CreateThread(보안(NULL=default), 스레드 할당 스택 크기, 스레드 함수, 매개변수, 실행 여부(0=생성 후 바로 실행), 스레드 id)</ins>. [(링크)](https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread)   
- 스레드 종료: <ins>CloseHandle(스레드)</ins>. [(링크)](https://docs.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle)   

<br>   
<br>   
<br>   

---   

- 스레드 우선 순위 변경: setThreadPriority, getThreadPriority   
- 스레드 우선 순위 클래스   
  - REALTIME_PRIORITY_CLASS : 실시간   
  - HIGH_PRIORITY_CLASS : 높음   
  - ABOVE_NORMAL_PRIORITY_CLASS : 높은 우선순위, 윈도우2000 이상   
  - NORMAL_PRIORITY_CLASS : 보통   
  - BELOW_NORMAL_PRIORITY_CLASS : 낮은 우선순위, 윈도우2000 이상   
  - IDLE_PRIORITY_CLASS : 낮음   
  

<br>   

- 스레드 종료 : WaitForSingleObject(), WaitForMultipleObject()   
- 스레드 일시 정지 및 재시작 : SuspendThread(),ResumThread()   
