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
class CMyTask : public ITask{
public:
	CMyTask() : m_pSession(NULL), m_lDataLen(0){

	}
	virtual bool RunTask(){
		
		return TRUE;
	}
public:
	STRU_SESSION* m_pSession;
	char m_pData[MAX_RECV_BUF];	
	long m_lDataLen;
};
class CKernel : public CControl<CKernel>, public IKernel {
public:
	CKernel();
	~CKernel();

	BOOL Init();
	void UnInit();
	BOOL OnRecvData(STRU_SESSION* pSession,
		const char* pData, long lDataLen);
	

private:
	INet* m_pTcpNet;
	INet* m_pUdpNet;
	CMyDao m_oDao;
	CThreadPool m_oPool;
};


#endif//__KERNEL_H__