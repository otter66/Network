## 스레드 동기화   
- 동기(Synchronous): 요청의 응답을 기다리는 방식   
- 비동기(Asynchronous): 요청의 응답을 기다리는 중 다른 일을 실행하는 방식   

<br>   

- 멀티 스레드를 이용하는 프로그램에서 공유 데이터를 이용할 경우 문제가 발생함. 따라서 스레드를 동기화하기 위한 여러 방법이 있음.   
- <ins>**임계영역(Critical Section)**</ins>: 공유자원에 대해 하나의 스레드만 접근 허용. (한 프로세스에 속한 스레드 간에만 사용 가능)   
    - InitializeCriticalSection(임계영역 포인터)
    - EnterCriticalSection(임계영역 포인터)
    - LeaveCriticalSection(임계영역 포인터)
    - DeleteCriticalSection(임계영역 포인터)   

<br>   

- <ins>**이벤트(Event)**</ins>: 사건 발생을 다른 스레드에 알려 대기중인 스레드 중 하나를 깨워서 작업하도록 함.    
  (이벤트를 비 신호 상태로 생성 → 스레드가 작업을 완료하면 이벤트를 신호 상태로 바꿈)   
    - <ins>CreateEvent(보안, 자동 리셋, 초기신호, 함수이름)</ins>: 이벤트 객체 생성   
    - <ins>SetEvent(object)</ins>: 해당 Event Object의 상태를 신호 상태로 변환.   
    - <ins>ResetEvent(object)</ins>: 해당 Event Object의 상태를 비 신호 상태로 변환.   
    - <ins>WaitForSingleObject() / WaitForMultipleObject()</ins> :  스레드에서 해당 Event, Thread Handel 등의 Object 상태를 불러올 때 사용   

<br>

- 뮤텍스(Mutex): 공유자원에 대해 하나의 스레드만 접근 허용. (서로 다른 프로세스에 속한 스레드 간에도 사용 가능)   
- 세마포어(Semaphore): 자원을 사용할 수 있는 스레드 개수를 제한.    

<br>   

## UDP   
- <ins>비연결 지향(→ connect 함수 미사용)</ins>, 신뢰성 보장X(→ 데이터 오류시 재전송x)
