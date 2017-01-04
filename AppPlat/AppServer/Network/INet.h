//the interface of the network to the kernel
#pragma once
#ifndef __INET_H__
#define __INET_H__
#include <WinSock2.h>//it include the redefine of the windows.h
#pragma comment(lib, "ws2_32.lib")//contant the sock lib 
//use the low floor sock and the top appliction use make a map 
//when senddata to the top appliction only need to know the account info
//does not need to know how the socket work. 
#define MAX_RECV_BUF 1200
//use for the function of the kernel onrecvdata
enum ENUM_RECV_TYPE{
	enum_recv_tcp,
	enum_recv_udp
};
struct  STRU_SESSION{
	STRU_SESSION(){
		m_sock = NULL;
		m_dwAccount = 0;
	}
public:
	//when use tcp m_sock is listen socket,
	//when use udp m_sock low 2 bytes is port,high 4 bytes is the addr
	INT64 m_sock;
	DWORD  m_dwAccount;
};
//use for tcp transport the file have the file head pointer
//to test whether this is file head
struct STRU_TCP_SESSION : public STRU_SESSION{
	STRU_TCP_SESSION();
public:
	STRU_FILE_HEAD* m_pFileHead;
};
//to use the interface of the ckernel to get the data of recv to 
//the kernel to solve
//如果使用类内包含kernel主类则不满足最少知道原则
class IKernel{
public:
	virtual BOOL OnRecvData(STRU_SESSION* pSession,
		const char* pData, long lDataLen, int iType) = 0;
};
class INet{
public:
	//init and uninit
	virtual BOOL InitNet(IKernel* kernel) = 0;
	virtual void UnInitNet() = 0; 
	//write ps:the read(recv)并不是主动功能而write发送数据是主动
	//提供主动调用去收数据不确定能否收到，毫无意义
	//所以只封装write功能
	virtual long SendData(STRU_SESSION* pSession, 
		const char* pData, long lDataLen) = 0;
	virtual BOOL BreakConn(STRU_SESSION* pSession) = 0;
};
#endif//__INET_H__
