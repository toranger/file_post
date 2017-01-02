#include "CProtocol.h"

//login rq
long STRU_LOGIN_RQ :: MIN_LEN = 12 ;
STRU_LOGIN_RQ :: STRU_LOGIN_RQ() :STRU_PRO_BASE(DEF_LOGIN_RQ){//the word type in first two bytes
	m_i64UserId = 0;
	m_wPasswdLen = 0;
}
long STRU_LOGIN_RQ :: Serialize(char szBuf[], long szBufLen){
	if( STRU_LOGIN_RQ :: MIN_LEN > szBufLen )
		return -1;
	//type
	*(WORD*)szBuf = m_wType;
	szBuf += sizeof(m_wType);
	//id
	*(INT64*)szBuf = m_i64UserId;
	szBuf += sizeof(m_i64UserId);
	//passwdLen
	*(WORD*)szBuf = m_wPasswdLen;
	szBuf += sizeof(m_wPasswdLen);
	szBufLen -= STRU_LOGIN_RQ :: MIN_LEN;
	if(szBufLen < m_wPasswdLen)
		return -1;
	//passwd
	memcpy(szBuf,m_pPasswd,m_wPasswdLen);
	szBuf += m_wPasswdLen;
	return (STRU_LOGIN_RQ :: MIN_LEN + m_wPasswdLen);
}
BOOL STRU_LOGIN_RQ :: UnSerialize(const char szBuf[], long szBufLen){
	if( STRU_LOGIN_RQ :: MIN_LEN > szBufLen )
		return FALSE;
	m_wType = *(WORD*)szBuf;
	szBuf += sizeof(m_wType);
	m_i64UserId = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserId);
	m_wPasswdLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wPasswdLen);
	szBufLen -= STRU_LOGIN_RQ :: MIN_LEN;
	if(m_wPasswdLen > szBufLen)
		return FALSE;
	memcpy(m_pPasswd,szBuf,m_wPasswdLen);
	szBuf += m_wPasswdLen;
	return TRUE;
}
//the serialize and unserialize in the struct app info
long STRU_APP_INFO :: MIN_LEN = 32 ;
long STRU_APP_INFO :: Serialize(char*& szBuf, long& szBufLen){
	if(szBufLen < 32 )
		return -1L;
	//id
	*(INT64*)szBuf = m_i64UserId;
	szBuf += sizeof(m_i64UserId);
	//key
	*(INT64*)szBuf = m_i64UserKey;
	szBuf += sizeof(m_i64UserKey);
	//sharedtime
	*(DWORD*)szBuf = m_dwSharedTime;
	szBuf += sizeof(m_dwSharedTime);
	//downtimes
	*(DWORD*)szBuf = m_dwDownTimes;
	szBuf += sizeof(m_dwDownTimes);
	//namelen
	*(WORD*)szBuf = m_wNameLen;
	szBuf += sizeof(m_wNameLen);
	//describelen
	*(WORD*)szBuf = m_wDescribeLen;
	szBuf += sizeof(m_wDescribeLen);
	//pathlen
	*(WORD*)szBuf = m_wPathLen;
	szBuf += sizeof(m_wPathLen);
	szBufLen -= STRU_APP_INFO :: MIN_LEN;
	long rest = m_wNameLen + m_wDescribeLen +  m_wPathLen; 
	if(rest > szBufLen)
		return -1L;
	//appname
	memcpy(szBuf,m_pAppName,m_wNameLen);
	szBuf += m_wNameLen;
	//appdescribe
	memcpy(szBuf,m_pAppDescribe,m_wDescribeLen);
	szBuf += m_wDescribeLen;
	//appiconpath
	memcpy(szBuf,m_pAppIconPath,m_wPathLen);
	szBuf += m_wPathLen;
	szBufLen -= rest;
	return STRU_APP_INFO :: MIN_LEN + rest;
}
BOOL STRU_APP_INFO :: UnSerialize(const char*& szBuf, long& szBufLen) {

	if( STRU_APP_INFO :: MIN_LEN > szBufLen )
		return FALSE;
	m_i64UserId = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserId);
	m_i64UserKey = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserKey);
	m_dwSharedTime = *(DWORD*)szBuf;
	szBuf += sizeof(m_dwSharedTime);
	m_dwDownTimes = *(DWORD*)szBuf;
	szBuf += sizeof(m_dwDownTimes);
	m_wNameLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wNameLen);
	m_wDescribeLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wDescribeLen);
	m_wPathLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wPathLen);
	//len
	szBufLen -= STRU_APP_INFO :: MIN_LEN;
	long rest = m_wNameLen + m_wDescribeLen +  m_wPathLen; 
	if(rest > szBufLen)
		return FALSE;
	//appname
	memcpy(m_pAppName,szBuf,m_wNameLen);
	szBuf += m_wNameLen;
	//appdescribe
	memcpy(m_pAppDescribe,szBuf,m_wDescribeLen);
	szBuf += m_wDescribeLen;
	//appiconpath
	memcpy(m_pAppIconPath,szBuf,m_wPathLen);
	szBuf += m_wPathLen;
	return TRUE;
}
//APP_LIST_RS
long STRU_APP_LIST_RS :: MIN_LEN = 12 ;
STRU_APP_LIST_RS :: STRU_APP_LIST_RS() :STRU_PRO_BASE(DEF_GET_APP_LIST_RS){//the word type in first two bytes
	m_i64UserId = 0;
	m_wAppCount = 0;
}
long STRU_APP_LIST_RS :: Serialize(char szBuf[], long szBufLen){
	if( STRU_APP_LIST_RS :: MIN_LEN > szBufLen )
		return -1;
	//type
	*(WORD*)szBuf = m_wType;
	szBuf += sizeof(m_wType);
	//id
	*(INT64*)szBuf = m_i64UserId;
	szBuf += sizeof(m_i64UserId);
	//app count
	*(WORD*)szBuf = m_wAppCount;
	szBuf += sizeof(m_wAppCount);
	szBufLen -= STRU_APP_LIST_RS :: MIN_LEN;

	//put the app info into szBuf
	//because a use b, b use c, 如果a直接操作c依赖性强
	//会产生高耦合,所以在b中创建接口
	for(long i = 0;i<m_wAppCount;++i){
		m_oAppInfo[i].Serialize(szBuf,szBufLen);
	}
	
	return (STRU_APP_LIST_RS :: MIN_LEN + m_wAppCount * 
		sizeof(STRU_APP_INFO));

}
BOOL STRU_APP_LIST_RS :: UnSerialize(const char szBuf[], long szBufLen){
	if( STRU_APP_LIST_RS :: MIN_LEN > szBufLen )
		return FALSE;
	m_wType = *(WORD*)szBuf;
	szBuf += sizeof(m_wType);
	m_i64UserId = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserId);
	m_wAppCount = *(WORD*)szBuf;
	szBuf += sizeof(m_wAppCount);
	szBufLen -= STRU_APP_LIST_RS :: MIN_LEN;
	for(long i = 0;i<m_wAppCount;++i){
		m_oAppInfo[i].UnSerialize(szBuf,szBufLen);
	}
	return TRUE;
}