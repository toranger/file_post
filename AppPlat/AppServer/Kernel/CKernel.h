#pragma once
#ifndef __KERNEL_H__
#define __KERNEL_H__ 
#include "CFactory.h"
#include "Control.h"
#include "MyDao.h"
#include "FileOpr.h"//when use the interface there need have the factory mode
#include "CProtocol.h"
#include "ThreadPool.h"

#define DEF_DB_NAME ("AppDao")
#define DEF_DB_IP ("localhost")
class KernelToTask{
public:
	virtual BOOL DealData(STRU_SESSION* pSession,
		const char* pData, long lDataLen) = 0;
};
class CMyTask : public ITask{
public:
	CMyTask() : m_pSession(NULL), m_lDataLen(0), m_pKernel(NULL){

	}
	virtual bool RunTask(){
		//TODO:
		if(m_pKernel){
			m_pKernel->DealData(m_pSession,m_pData,m_lDataLen);
		}
		return TRUE;
	}
public:
	STRU_SESSION* m_pSession;
	char m_pData[MAX_RECV_BUF];	
	long m_lDataLen;
	KernelToTask* m_pKernel;
};
class CKernel : public CControl<CKernel>, public IKernel , public KernelToTask{
public:
	CKernel();
	~CKernel();

	BOOL Init();
	void UnInit();
	BOOL OnRecvData(STRU_SESSION* pSession,
		const char* pData, long lDataLen);
	//realize the function of deal the data from the each que
	//the real deal funtion
	BOOL DealData(STRU_SESSION* pSession,
		const char* pData, long lDataLen);

private:
	INet* m_pTcpNet;
	INet* m_pUdpNet;
	CMyDao m_oDao;
	CThreadPool m_oPool;
};


#endif//__KERNEL_H__