#pragma once
/*
	the thread pool only use for the udp order transport
*/
#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__
#include <Windows.h>
#include "Lock_que.h"
//任务基类接口
struct ITask{
public:
	virtual ~ITask(){}
	virtual bool RunTask() = 0;
};
class CThreadPool{
public:
	CThreadPool();
	~CThreadPool();
	//create threadpool	
	bool CreateThreadPool(long Minnum,long Maxnum, long Tasknum);

	//destory threadpool
	void DestoryThreadPool();
	//push task
	bool PushTask(ITask* task,long index);
	
public:
	//the threadproc
	static UINT WINAPI ThreadProc(LPVOID lpParam);
	//packing the proc
	void ThreadPool();	
private:
	//task queue
	CLockQue<ITask> m_lockque;	
	//minest thread num
	long m_lMinNum;
	//maxest thread num
	long m_lMaxNum;
	//already running thread num
	long m_lRunNum;
	//already created thread num
	long m_lCreatedNum;
	//notify to run thread signal
	HANDLE m_hSemaphore;
	//notify to thread quit signal
	HANDLE m_hQuitEvent;
};


#endif//__THREAD_POOL_H__