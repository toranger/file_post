#include "ThreadPool.h"

CThreadPool :: CThreadPool(){

}
CThreadPool :: ~CThreadPool(){}
//create threadpool	
bool CThreadPool :: CreateThreadPool(long Minnum,long Maxnum, long Tasknum){

	return TRUE;
}

//destory threadpool
void CThreadPool :: DestoryThreadPool(){}
//push task
bool CThreadPool :: PushTask(ITask* task){
	return TRUE;
}
//the threadproc
UINT WINAPI CThreadPool :: ThreadProc(LPVOID lpParam){


	return 0;
}
//packing the proc
void CThreadPool :: ThreadPool(){}	