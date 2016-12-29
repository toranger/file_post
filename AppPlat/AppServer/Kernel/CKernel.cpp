#include "CKernel.h"
CKernel :: CKernel(){

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
	//TODO:the thread pool create
	//m_oPool.CreateThreadPool();
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
		
	

	return TRUE;
}