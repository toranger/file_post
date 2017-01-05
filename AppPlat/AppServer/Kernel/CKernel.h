#pragma once
#ifndef __KERNEL_H__
#define __KERNEL_H__ 
#include "CFactory.h"
#include "Control.h"
#include "MyDao.h"
#include "FileOpr.h"//when use the interface there need have the factory mode
#include "CProtocol.h"
#include "ThreadPool.h"
#include "Circle_que.h"

#define DEF_DB_NAME ("AppDao")
#define DEF_DB_IP ("localhost,1040")
//the num of the thread in thread pool
#define DEF_THREAD_POOL_MAX    (10)
#define DEF_THREAD_QUE_LEN      (1000)

#define DEF_USER_VARIFY(TYPE) \
	STRU_##TYPE##_RQ oLoginRq; \
	if( 0 == oLoginRq.UnSerialize(pData,lDataLen)){ \
		return FALSE; \
	} \
	USER_MAP::iterator it = m_UserMap.find(oLoginRq.m_i64UserId); \
	if(it == m_UserMap.end()){ \
		return FALSE; \
	}else if(it->second->m_i64UserKey != oLoginRq.m_i64UserKey){ \
		return FALSE; \
	} 
#define DEF_RESPONSE() \
	char szBuf[MAX_RECV_BUF]; \
	long lLen; \
	lLen = oLoginRs.Serialize(szBuf,MAX_RECV_BUF); \
	m_pTcpNet->SendData(pSession,(const char*)szBuf,lLen); \

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
		return true;
	}
public:
	STRU_SESSION* m_pSession;
	char m_pData[MAX_RECV_BUF];	
	long m_lDataLen;
	KernelToTask* m_pKernel;
};
//FileTask does not extend the Itask because tcp send file data
//use the other theads with their own queue, to slove the data
class CFileTask{
public:
	CFileTask(){
		m_i64UserId = 0;
		m_i64FileKey = 0;
		m_dwFileLen = 0;
		m_pFileContent = NULL;	
	}
public:
	INT64 m_i64UserId;
	INT64 m_i64FileKey;
	DWORD m_dwFileLen;
	char* m_pFileContent;
};
class CKernel : public CControl<CKernel>, public IKernel , public KernelToTask{
public:
	CKernel();
	~CKernel();

	BOOL Init();
	void UnInit();
	BOOL OnRecvData(STRU_SESSION* pSession,
		const char* pData, long lDataLen, int iType);
	//realize the function of deal the data from the each que
	//the real deal funtion
	BOOL DealData(STRU_SESSION* pSession,
		const char* pData, long lDataLen);
private:
	struct STRU_USER_INFO{
	public:
		INT64 m_i64UserId;
		INT64 m_i64UserKey;//the server make this key for user certification
		INT64 m_i64LastTime;
		WORD  m_wUserStat;
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
	//read the info in db of users into the user map
	BOOL ReadUserInfo();
	//the userid + login time create the varify number
	//because of often login so use inline function
	inline INT64 GetUserVarify(INT64 user_id){
		DWORD time = ::GetTickCount();			
		INT64 i64VarifyNum = (((INT64)time) << 32) +( ((user_id % time) << 32) >> 32 ); 
		return i64VarifyNum;
	}

	inline INT64 GetFileKey(INT64 user_id, WORD version, WORD filename_len,
		const char* filename){
		INT64 i64Key = user_id + HIBYTE(version)/LOBYTE(version)
			+ filename_len;
		return i64Key;
	}
	inline long GetPostQueNum(INT64 file_key){
		//according to the num of the thread num in thread pool
		return file_key % m_lThreadCount;
	}
private:
	//the thread function to tcp file transport
	static unsigned int WINAPI OnFileTran(void* param);
	//@param index : the index of the which queue
	void OnFileTran(long index);
	//the file transport thread param
	struct STRU_TRAN_FILE_PARAM{
		STRU_TRAN_FILE_PARAM(){
			m_pKernel = NULL;
			m_lIndex = 0;
		}
	public:
		CKernel* m_pKernel;
		long m_lIndex;//which queue
	};
private:
	INet* m_pTcpNet;
	INet* m_pUdpNet;
	CMyDao m_oDao;
	CThreadPool m_oPool;//only use for the udp slove the order data
	MSG_MAP_FUNCTION m_pMessageMap[DEF_PRO_COUNT];//the max count of protocol
	//each file thread has own task queue
	CircleQue<CFileTask>* m_pFileTaskQue;//according to thread count
	long m_lThreadCount;
	STRU_TRAN_FILE_PARAM* m_pTranFileParam;//have the thread count
	
};


#endif//__KERNEL_H__