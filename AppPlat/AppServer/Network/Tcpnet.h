#pragma once
#ifndef __TCP_H__
#define __TCP_H__
#include <map>
#include "INet.h"
#define MAX_RECV_BUF 1200
class CTcpNet : public INet{
public: CTcpNet();
	~CTcpNet();
	//实现基类接口
	BOOL InitNet();
	void UnInitNet(); 
	long SendData(STRU_SESSION* pSession, 
		const char* pData, long lDataLen);
private:
	//accept the link thread funtion
	static 	unsigned __stdcall WINAPI Accept(void* param);
	//recv the message thread funtion
	static 	unsigned __stdcall WINAPI ReadData(void* param);
	//the inner init of the network
	BOOL innerInitNet();
	LONG GetHostIP();
private:
	SOCKET m_sock;
	long m_lThreadCount;
	BOOL m_bRun;
	std::map<SOCKET,STRU_SESSION*> m_mapSession;
};




#endif//__TCP_H__