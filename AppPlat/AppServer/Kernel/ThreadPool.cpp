#include "ThreadPool.h"
#include <process.h>

CThreadPool :: CThreadPool() : m_lMinNum(0), m_lMaxNum(0), m_lRunNum(0),
	m_lCreatedNum(0), m_hSemaphore(NULL), m_hQuitEvent(NULL){
}
CThreadPool :: ~CThreadPool(){
	DestoryThreadPool();
}
//create threadpool	
bool CThreadPool :: CreateThreadPool(long Minnum,long Maxnum, long Tasknum){
	if(Minnum < 0 || Maxnum < 0 || Tasknum < 0)
		return FALSE;
	//init the queue with this thread pool
	m_lockque.Init(Tasknum);	
	m_lMinNum = Minnum;
	m_lMaxNum = Maxnum;
	m_hQuitEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_hSemaphore = CreateSemaphore(NULL,0,Maxnum,NULL);
	if(!m_hQuitEvent || !m_hSemaphore)
		return FALSE;
	for(int i=0;i<Minnum;i++){
		HANDLE handle = (HANDLE)_beginthreadex(NULL,0,&ThreadProc,this,0,NULL);
		if(handle){
			CloseHandle(handle);
			handle = NULL;
		}else{
			return FALSE;	
		}
	}
	m_lCreatedNum = Minnum;
	return TRUE;
}

//destory threadpool
void CThreadPool :: DestoryThreadPool(){
	SetEvent(m_hQuitEvent);	
	ReleaseSemaphore(m_hSemaphore,m_lCreatedNum,NULL);
	//wait the thread finish
	::Sleep(100);
	CloseHandle(m_hQuitEvent);
	m_hQuitEvent = NULL;
	CloseHandle(m_hSemaphore);
	m_hSemaphore = NULL;
	m_lockque.UnInit();
}
//push task
bool CThreadPool :: PushTask(ITask* task,long index){
	if(task == NULL)
		return FALSE;
	BOOL flag;
	flag = m_lockque.Enque(task);
	if(flag == FALSE)
		return FALSE;
	//is there free thread
	if(m_lRunNum < m_lCreatedNum){
		::ReleaseSemaphore(m_hSemaphore,1,NULL);
	}else{//whether can create
		if(m_lCreatedNum < m_lMaxNum){
			HANDLE handle = (HANDLE)::_beginthreadex(NULL,0,&ThreadProc,this,0,NULL);
			if(handle){
				CloseHandle(handle);
				handle = NULL;
			}
		}
		//the statue of the thread wait become run
		::InterlockedIncrement(&m_lRunNum);
		::ReleaseSemaphore(m_hSemaphore,1,NULL);
	}
	return TRUE;
}
//the threadproc
UINT WINAPI CThreadPool :: ThreadProc(LPVOID lpParam){
	CThreadPool *pThis = (CThreadPool*)lpParam;
	pThis->ThreadPool();
	return 0;
}
//packing the proc
void CThreadPool :: ThreadPool(){
	ITask *task = NULL;
	BOOL flag;
	while(1){
		::WaitForSingleObject(m_hSemaphore,INFINITE);
		if( WAIT_OBJECT_0 == ::WaitForSingleObject(m_hQuitEvent,10)){
			break; //finish the thread
		}
		InterlockedIncrement(&m_lRunNum);
		//get task from the queue
		while(1){
			flag = m_lockque.Deque(&task);
			if(flag == FALSE)
				break;
			task->RunTask();//run task
		}
		InterlockedDecrement(&m_lCreatedNum);
	}
	return;
}	