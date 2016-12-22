//the interface of the network to the kernel
#pragma once
#ifndef __INET_H__
#define __INET_H__
#include <WinSock2.h>//it include the redefine of the windows.h
#pragma comment(lib, "ws2_32.lib")//contant the sock lib 
//use the low floor sock and the top appliction use make a map 
//when senddata to the top appliction only need to know the account info
//does not need to know how the socket work. 
struct  STRU_SESSION{
	STRU_SESSION(){
		m_sock = NULL;
		m_dwAccount = 0;
	}
public:
	SOCKET m_sock;
	DWORD  m_dwAccount;
};
class INet{
public:
	//init and uninit
	virtual BOOL InitNet() = 0;
	virtual void UnInitNet() = 0; 
	//write ps:the read(recv)并不是主动功能而write发送数据是主动
	//所以只封装write功能
	virtual long SendData(STRU_SESSION* pSession, 
		const char* pData, long lDataLen) = 0;
};
#endif//__INET_H__
