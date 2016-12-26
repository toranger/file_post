#include "CProtocol.h"


long STRU_PRO_LOGIN_RQ :: MIN_LEN = 12 ;
STRU_PRO_LOGIN_RQ :: STRU_PRO_LOGIN_RQ() :STRU_PRO_BASE(DEF_LOGIN_RQ){//the word type in first two bytes
	m_i64UserId = 0;
	m_wPasswdLen = 0;
}
long STRU_PRO_LOGIN_RQ :: Serialize(char szBuf[], long szBufLen){
	if( STRU_PRO_LOGIN_RQ :: MIN_LEN > szBufLen )
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
	szBufLen -= STRU_PRO_LOGIN_RQ :: MIN_LEN;
	if(szBufLen < m_wPasswdLen)
		return -1;
	//passwd
	memcpy(szBuf,m_pPasswd,m_wPasswdLen);
	szBuf += m_wPasswdLen;
	return (STRU_PRO_LOGIN_RQ :: MIN_LEN + m_wPasswdLen);

}
BOOL STRU_PRO_LOGIN_RQ :: UnSerialize(const char szBuf[], long szBufLen){
	if( STRU_PRO_LOGIN_RQ :: MIN_LEN > szBufLen )
		return FALSE;
	m_wType = *(WORD*)szBuf;
	szBuf += sizeof(m_wType);
	m_i64UserId = *(INT64*)szBuf;
	szBuf += sizeof(m_i64UserId);
	m_wPasswdLen = *(WORD*)szBuf;
	szBuf += sizeof(m_wPasswdLen);
	szBufLen -= STRU_PRO_LOGIN_RQ :: MIN_LEN;
	if(m_wPasswdLen > szBufLen)
		return FALSE;
	memcpy(m_pPasswd,szBuf,m_wPasswdLen);
	szBuf += m_wPasswdLen;
	return TRUE;
}
