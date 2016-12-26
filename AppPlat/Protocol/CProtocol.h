#pragma once
#ifndef __PRO_H__
#define __PRO_H__
#include <Windows.h>
#define MAX_PASS_LEN    16
#define DEF_PRO_START  (10010)
#define DEF_LOGIN_RQ    (DEF_PRO_START + 1)
#define DEF_LOGIN_RS    (DEF_PRO_START + 2)

#define DEF_UPLOAD_RQ    (DEF_PRO_START + 3)
#define DEF_UPLOAD_RS    (DEF_PRO_START + 4)

#define DEF_DOWNLOAD_RQ    (DEF_PRO_START + 5)
#define DEF_DOWNLOAD_RS    (DEF_PRO_START + 6)

#define DEF_GET_APP_LIST_RQ   (DEF_PRO_START + 7)
#define DEF_GET_APP_LIST_RS  (DEF_PRO_START + 8)

#define DEF_INSTALL_RQ (DEF_PRO_START + 9)
#define DEF_INSTALL_RS (DEF_PRO_START + 10)

#define DEF_UNINSTALL_RQ (DEF_PRO_START + 11)
#define DEF_UNINSTALL_RS (DEF_PRO_START + 12)

#define DEF_LOGOUT_RQ (DEF_PRO_START + 13)//only need to tell server
#define DEF_PRO_END    (10100)
//防止协议类型被修改利用保护继承
struct STRU_PRO_BASE{
public:
	STRU_PRO_BASE(WORD type) : m_wType(type){

	}
	virtual long Serialize(char szBuf[], long szBufLen) = 0;
	virtual BOOL UnSerialize(const char szBuf[], long szBufLen) = 0;
protected:
	WORD m_wType;
};
struct STRU_PRO_LOGIN_RQ : public STRU_PRO_BASE{
public:
	STRU_PRO_LOGIN_RQ();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	WORD  m_wPasswdLen;//according to this len to send and recv
	char  m_pPasswd[MAX_PASS_LEN];
};
struct STRU_PRO_LOGIN_RS : public STRU_PRO_BASE {
	STRU_PRO_LOGIN_RS(): STRU_PRO_BASE(DEF_LOGIN_RS){
		m_i64UserId = 0;
		m_wResult = 0;
	}
public:
	INT64 m_i64UserId;
	WORD  m_wResult;
};


#endif//__PRO_H__