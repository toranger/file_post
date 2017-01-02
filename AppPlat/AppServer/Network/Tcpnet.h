#pragma once
#ifndef __TCP_H__
#define __TCP_H__
#include "INet.h"
#include <map>
#include "Lock.h"
class CTcpNet : public INet{
public: 
	CTcpNet();
	~CTcpNet();
	//实现基类接口
	virtual BOOL InitNet(IKernel* Kernel);
	virtual void UnInitNet(); 
	virtual long SendData(STRU_SESSION* pSession, 
		const char* pData, long lDataLen);
	virtual BOOL BreakConn(STRU_SESSION* pSession);
private:
	//accept the link thread funtion
	static 	unsigned int WINAPI Accept(void* param);
	//静态方法每次调用类的私有成员没有this指针需利用pthis比较繁琐
	void Accept();
	//recv the message thread funtion
	static 	unsigned int WINAPI ReadData(void* param);
	void ReadData();
	//the inner init of the network
	BOOL innerInitNet();
	LONG GetHostIP();
	BOOL DelInvalSession(SOCKET sock);
private:
	SOCKET m_sock;
	long m_lThreadCount;
	BOOL m_bRun;
	std::map<SOCKET,STRU_SESSION*> m_mapSession;
	IKernel* m_pKernel;
	CLock m_lock;
};
#endif//__TCP_H__