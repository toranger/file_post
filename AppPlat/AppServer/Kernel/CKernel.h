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
	struct STRU_USER_INFO{
	public:
		INT64 m_i64UserId;
		INT64 m_i64UserKey;//the server make this key for user certification
		WORD  m_wPasswdLen;//according to this len to send and recv
		char  m_pPasswd[MAX_PASS_LEN];
	};
	typedef std::map<INT64,STRU_USER_INFO*> USER_MAP;
	USER_MAP m_UserMap;
private:
	typedef BOOL (CKernel::*MSG_MAP_FUNCTION)(STRU_SESSION* pSession,
		const char* pData, long lDataLe);
	//为每个请求定义对应处理方法
	BOOL OnDealLoginRQ(STRU_SESSION* pSession,
		const char* pData, long lDataLe);
	BOOL OnDealUploadRQ(STRU_SESSION* pSession,
		const char* pData, long lDataLe);
	BOOL OnDealDownloadRQ(STRU_SESSION* pSession,
		const char* pData, long lDataLe);
	BOOL OnDealGetAppListRQ(STRU_SESSION* pSession,
		const char* pData, long lDataLe);
	BOOL OnDealInstallRQ(STRU_SESSION* pSession,
		const char* pData, long lDataLe);
	BOOL OnDealUnInstallRQ(STRU_SESSION* pSession,
		const char* pData, long lDataLe);
	BOOL OnDealLogoutRQ(STRU_SESSION* pSession,
		const char* pData, long lDataLe);
private:
	INet* m_pTcpNet;
	INet* m_pUdpNet;
	CMyDao m_oDao;
	CThreadPool m_oPool;
	MSG_MAP_FUNCTION m_pMessageMap[DEF_PRO_COUNT];//the max count of protocol
};


#endif//__KERNEL_H__