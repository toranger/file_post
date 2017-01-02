#include "CKernel.h"

//define the info colum in user class in dbo
#define DEF_USR_ID ("user_id")
#define DEF_USR_PASS ("user_pass")
#define DEF_LAST_TIME ("last_time")
#define DEF_USR_STAT ("user_stat")

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
	//the 2000 suit for the 2008, but when 2005 get the data have the memery problem
	if( FALSE == m_oDao.OpenDateBase(DEF_DB_NAME,DATEBASE_TYPE_SQL2008,"sa","sa",DEF_DB_IP)){
		return FALSE;
	}
	//the thread pool create
	m_oPool.CreateThreadPool(5, 10, 10000);
	//read the db create the map of the user info
	if( FALSE == ReadUserInfo() ){
		return FALSE;
	}
	return TRUE;
}
//read the info in db of users into the user map
BOOL CKernel :: ReadUserInfo(){
	std::list<TCHAR *> colum;
	std::list<TCHAR *> retList;
	//put the colum name in list
	colum.push_back(DEF_USR_ID);
	colum.push_back(DEF_USR_PASS);
	colum.push_back(DEF_LAST_TIME);
	colum.push_back(DEF_USR_STAT);
	if( FALSE == m_oDao.GetData("select * from USER_INFO",colum,colum.size(),retList) ){
		return FALSE;
	}
	//m_UserMap
	//get info from the colum list to the user map
	std::list<TCHAR*>::iterator it = retList.begin();
	long count = retList.size() / colum.size();
	//the ++i is fast than the i++
	for(long i=0;i<count;++i){
		//get info and create struct
		STRU_USER_INFO* user = new STRU_USER_INFO;
		user->m_i64UserId = _atoi64(*it++);
		user->m_wPasswdLen = strlen(*it);
		memcpy(user->m_pPasswd,*it++,user->m_wPasswdLen);
		user->m_i64LastTime = _atoi64(*it++);
		user->m_wUserStat = atoi(*it++);
		//put struct into map
		m_UserMap[user->m_i64UserId] = user;
	}
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
	STRU_LOGIN_RQ oLoginRq;
	STRU_LOGIN_RS oLoginRs;
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
				pSession->m_dwAccount = oLoginRq.m_i64UserId; 
				//oLoginRs.m_i64UserKey;
				oLoginRs.m_i64UserKey = GetUserVarify(oLoginRq.m_i64UserId);
				it->second->m_i64UserKey = oLoginRs.m_i64UserKey;
			}else{
				oLoginRs.m_wResult = enum_passwd_error;	
			}
		}
	}
	//send response
	DEF_RESPONSE()
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
	//only has the request
	DEF_USER_VARIFY(LOGOUT)
	//the udp link close. because the order use udp  
	//other use the tcp 
	m_pUdpNet->BreakConn(pSession);
	//change the userinfo map 
	it->second->m_i64LastTime = GetTickCount();
	it->second->m_i64UserKey = 0;
	it->second->m_wUserStat = 0;
	//change the user info in db
	//TODO : there can use the circle or lock queue to put operation for db in,
	//then create a new thread to slove the data in queue to put data into db.
	char szBuf[100];
	sprintf(szBuf,"update USER_INFO set last_time = %I64, user_stat = %d where user_id = %I64",
		it->second->m_i64LastTime,it->second->m_wUserStat,it->second->m_i64UserId);
	if(FALSE == m_oDao.EditData(szBuf)){
		return FALSE;
	}
	return TRUE;
}