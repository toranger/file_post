#pragma once
#ifndef __PRO_H__
#define __PRO_H__
#include <Windows.h>
#define MAX_PASS_LEN    (16)
#define DEF_COMMON_MAX_LEN  (100)
#define DEF_GROUP_MAX_COUNT (3)
#define DEF_FILE_NAME_MAX_LEN (20)
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
//#define DEF_PRO_END    (10100)
#define DEF_PRO_COUNT  (100)//use for message map
//use for the file model 
struct STRU_FILE_HEAD{
	STRU_FILE_HEAD(){
		m_i64UserId = 0;
		m_dwFileLen = 0;
		m_wVersion = 0;
		m_wAppNameLen = 0;
	}
	long Serialize(char*& szBuf, long& szBufLen) ;
	BOOL UnSerialize(const char*& szBuf, long& szBufLen) ;
public:
	INT64 m_i64UserId;
	DWORD m_dwFileLen;
	WORD  m_wVersion;
	WORD  m_wAppNameLen;
	char  m_pAppName[DEF_FILE_NAME_MAX_LEN];
};
enum ENUM_RQ_RESULT{
	enum_success,
	enum_unexist,
	enum_passwd_error,
	enum_failed
};
struct STRU_APP_INFO{
	STRU_APP_INFO();
	long Serialize(char*& szBuf, long& szBufLen) ;
	BOOL UnSerialize(const char*& szBuf, long& szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	INT64 m_i64UserKey;
	DWORD m_dwSharedTime;
	DWORD m_dwDownTimes;
	WORD  m_wNameLen;// the real length of the app name to send
	WORD  m_wDescribeLen;
	WORD  m_wPathLen;
	char  m_pAppName[DEF_COMMON_MAX_LEN];
	char  m_pAppDescribe[DEF_COMMON_MAX_LEN];
	char  m_pAppIconPath[DEF_COMMON_MAX_LEN];

};
//防止协议类型被修改利用保护继承 base protocol
struct STRU_PRO_BASE{
public:
	STRU_PRO_BASE(WORD type) : m_wType(type){

	}
	virtual long Serialize(char szBuf[], long szBufLen) = 0;
	virtual BOOL UnSerialize(const char szBuf[], long szBufLen) = 0;
protected:
	WORD m_wType;
};
//login
struct STRU_LOGIN_RQ : public STRU_PRO_BASE{
public:
	STRU_LOGIN_RQ();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	WORD  m_wPasswdLen;//according to this len to send and recv
	char  m_pPasswd[MAX_PASS_LEN];
};
struct STRU_LOGIN_RS : public STRU_PRO_BASE {
	STRU_LOGIN_RS();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	INT64 m_i64UserKey;//the server make this key for user certification
	WORD  m_wResult;
};
//get app list
struct STRU_APP_LIST_RQ : public STRU_PRO_BASE{
public:
	STRU_APP_LIST_RQ();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	INT64 m_i64UserKey;
	DWORD m_dwAppKey;//each app has own appkey. there to tell get this app and next 3 
};
struct STRU_APP_LIST_RS : public STRU_PRO_BASE {
	STRU_APP_LIST_RS();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	WORD  m_wAppCount;//the real count of the m_oAppInfo
	//the udp send max is 1500 bytes in sizeof appinfo struct does not over 400 bytes
	//so there can be 3 group to send
	STRU_APP_INFO m_oAppInfo[DEF_GROUP_MAX_COUNT];
};
//install
struct STRU_INSTALL_RQ : public STRU_PRO_BASE{
public:
	STRU_INSTALL_RQ();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	INT64 m_i64UserKey;
	DWORD m_dwAppKey;//each app has own appkey. there to tell get this app and next 3 
};
//uninstall
struct STRU_UNINSTALL_RQ : public STRU_PRO_BASE{
public:
	STRU_UNINSTALL_RQ();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	INT64 m_i64UserKey;
	DWORD m_dwAppKey;//each app has own appkey. there to tell get this app and next 3 
};
//logout
struct STRU_LOGOUT_RQ : public STRU_PRO_BASE{
public:
	STRU_LOGOUT_RQ();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	INT64 m_i64UserKey;
};
//upload
struct STRU_UPLOAD_RQ : public STRU_PRO_BASE{
public:
	STRU_UPLOAD_RQ();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	INT64 m_i64UserKey;
	STRU_APP_INFO m_oAppInfo;
};
struct STRU_UPLOAD_RS : public STRU_PRO_BASE{
public:
	STRU_UPLOAD_RS();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	STRU_APP_INFO m_oAppInfo;
};
//download 
struct STRU_DOWNLOAD_RQ : public STRU_PRO_BASE{
public:
	STRU_DOWNLOAD_RQ();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	INT64 m_i64UserKey;
	DWORD m_dwAppKey;
};
struct STRU_DOWNLOAD_RS : public STRU_PRO_BASE{
public:
	STRU_DOWNLOAD_RS();
	long Serialize(char szBuf[], long szBufLen) ;
	BOOL UnSerialize(const char szBuf[], long szBufLen) ;
	static long MIN_LEN;//immobilization length
public:
	INT64 m_i64UserId;
	INT64 m_i64UserKey;//app + version create the key
	WORD  m_wResult;
};



#endif//__PRO_H__