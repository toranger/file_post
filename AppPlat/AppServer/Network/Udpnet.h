#pragma once
#ifndef __UDP_H__
#define __UDP_H__
#include "INet.h"
#include <map>
class CUdpNet : public INet{
public:
	CUdpNet();
	~CUdpNet();
public:
	virtual BOOL InitNet(IKernel* kernel);
	virtual void UnInitNet(); 
	//单播和广播
	virtual long SendData(STRU_SESSION* pSession, 
		const char* pData, long lDataLen);
	virtual BOOL BreakConn(STRU_SESSION* pSession);
	//TODO:组播
public:
	BOOL innerInitNet();
	LONG GetHostIP();
	//recv the message thread funtion
	static 	unsigned int WINAPI ReadData(void* param);
	void ReadData();
private:
	SOCKET m_sock;
	long m_lThreadCount;
	BOOL m_bRun;
	std::map<INT64,STRU_SESSION*> m_mapSession;//change the key
	IKernel* m_pKernel;
};



#endif//__UDP_H__