#include "CKernel.h"
CKernel :: CKernel() : m_pTcpNet(NULL), m_pUdpNet(NULL){
	m_pMessageMap[DEF_LOGIN_RQ - DEF_PRO_START] = &CKernel::OnDealUploadRQ;
	m_pMessageMap[DEF_UPLOAD_RQ - DEF_PRO_START] = &CKernel::OnDealUploadRQ;
	m_pMessageMap[DEF_DOWNLOAD_RQ - DEF_PRO_START] = &CKernel::OnDealDownloadRQ;
	m_pMessageMap[DEF_GET_APP_LIST_RQ - DEF_PRO_START] = &CKernel::OnDealGetAppListRQ;
	m_pMessageMap[DEF_INSTALL_RQ - DEF_PRO_START] = &CKernel::OnDealInstallRQ;
	m_pMessageMap[DEF_UNINSTALL_RQ - DEF_PRO_START] = &CKernel::OnDealUnInstallRQ;
	m_pMessageMap[DEF_LOGOUT_RQ - DEF_PRO_START] = &CKernel::OnDealLogoutRQ;
}
CKernel :: ~CKernel(){
	UnInit();
}
BOOL CKernel :: Init(){
	UnInit();
	CFactoryNet factorynet ;
	m_pTcpNet = factorynet.GetInstance(enum_type_tcp);
	m_pTcpNet->InitNet(this);
	m_pUdpNet = factorynet.GetInstance(enum_type_udp);
	m_pUdpNet->InitNet(this);

	m_oDao.OpenDateBase(DEF_DB_NAME,DATEBASE_TYPE_SQL2005,"sa","sa",DEF_DB_IP);//also use 2008
	//the thread pool create
	m_oPool.CreateThreadPool(5, 10, 10000);
	//TODO: read the db create the map of the user info
	//m_UserMap
	return TRUE;
}
void CKernel :: UnInit(){
	//TODO:
}
BOOL CKernel :: OnRecvData(STRU_SESSION* pSession,
		const char* pData, long lDataLen){
	//it will take the tcp or udp thread time to return the recv data to kernel
	//so can create the task, fill the task, then put into the thread of kernel to slove
	//creat task
	CMyTask * task = new CMyTask;
	//fill
	task->m_pSession = pSession;
	memcpy(task->m_pData,pData,lDataLen);
	task->m_lDataLen = lDataLen;
	task->m_pKernel = this;	
	//put into threadpool
	int iTimes = 3;
	while(FALSE == m_oPool.PushTask(task)){
		::Sleep(1);
		if(--iTimes == 0){
			delete task;
			return FALSE;
		}
	}
	return TRUE;
}
	
BOOL CKernel :: DealData(STRU_SESSION* pSession,
		const char* pData, long lDataLen){
	//get the type of the protocol
	WORD type = *(WORD*)lDataLen;
	type -= DEF_PRO_START;
	//使用switch case 不满足程序的开闭性原则所以，每添加一个协议则需要添加case
	//利用消息机制中的消息映射表将type和对应操作函数绑定
	BOOL ret = ( this->*this->m_pMessageMap[type] )(pSession,pData,lDataLen);
	return ret;
}

//the kernel deal with the data
BOOL CKernel :: OnDealLoginRQ(STRU_SESSION* pSession,
	const char* pData, long lDataLen){
	STRU_PRO_LOGIN_RQ oLoginRq;
	STRU_PRO_LOGIN_RS oLoginRs;
	//get the info
	oLoginRq.UnSerialize(pData,lDataLen);
	oLoginRs.m_i64UserId = oLoginRq.m_i64UserId;
	USER_MAP::iterator it = m_UserMap.find(oLoginRq.m_i64UserId);
	//not find person
	if(it == m_UserMap.end()){
		oLoginRs.m_wResult = enum_failed;
	}else{
	//find person check passwd
		if(oLoginRq.m_wPasswdLen != it->second->m_wPasswdLen){
			oLoginRs.m_wResult = enum_passwd_error;	
		}else{
			if(0 == memcmp(oLoginRq.m_pPasswd,it->second->m_pPasswd,oLoginRq.m_wPasswdLen)){
				oLoginRs.m_wResult = enum_success;	
				//TODO: oLoginRs.m_i64UserKey;
			}else{
				oLoginRs.m_wResult = enum_passwd_error;	
			}
		}
	}
	//send response
	char szBuf[MAX_RECV_BUF];
	long lLen;
	lLen = oLoginRs.Serialize(szBuf,MAX_RECV_BUF);
	m_pTcpNet->SendData(pSession,(const char*)szBuf,lLen);
	return TRUE;
}
BOOL CKernel :: OnDealUploadRQ(STRU_SESSION* pSession,
	const char* pData, long lDataLen){

	return TRUE;
}
BOOL CKernel :: OnDealDownloadRQ(STRU_SESSION* pSession,
	const char* pData, long lDataLen){

	return TRUE;
}
BOOL CKernel :: OnDealGetAppListRQ(STRU_SESSION* pSession,
	const char* pData, long lDataLen){

	return TRUE;
}
BOOL CKernel :: OnDealInstallRQ(STRU_SESSION* pSession,
	const char* pData, long lDataLen){

	return TRUE;
}
BOOL CKernel :: OnDealUnInstallRQ(STRU_SESSION* pSession,
	const char* pData, long lDataLen){

	return TRUE;
}
BOOL CKernel :: OnDealLogoutRQ(STRU_SESSION* pSession,
	const char* pData, long lDataLen){

	return TRUE;
}