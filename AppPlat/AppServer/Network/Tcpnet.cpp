#include "Tcpnet.h"
#include <process.h>
#include <stdio.h>
CTcpNet :: CTcpNet() : m_sock(NULL), m_lThreadCount(0), m_bRun(FALSE) {

}
CTcpNet :: ~CTcpNet(){
}
//实现基类接口
BOOL CTcpNet :: InitNet(){
	//the inner init for network
	innerInitNet();
	//create accept thread
	HANDLE handler = (HANDLE)::_beginthreadex(NULL,0,Accept,this,0,NULL);
	if(handler == NULL)
		return FALSE;
	//create recv thread
	handler = (HANDLE)::_beginthreadex(NULL,0,ReadData,this,0,NULL);
	if(handler == NULL)
		return FALSE;
	return TRUE;
}
void CTcpNet :: UnInitNet(){
	//TODO:
}
long CTcpNet :: SendData(STRU_SESSION* pSession, 
	const char* pData, long lDataLen){
		return ::send(pSession->m_sock,pData,lDataLen,0);
}
//accept the link thread funtion  the __stdcall can be int
unsigned __stdcall WINAPI CTcpNet ::  Accept(void* param){
	//get the param
	CTcpNet* p = (CTcpNet*)param;
	u_long flag = 100;
	SOCKET client;
	int iErrorCode = 0;
	//利用原子锁多threadcount进行操作
	InterlockedIncrement(&(p->m_lThreadCount));
	//set the unblock socket to socket
	::ioctlsocket(p->m_sock,FIONREAD ,&flag);
	//to accept the link
	while(p->m_bRun){
		client = ::accept(p->m_sock,NULL,NULL);
		if(client == SOCKET_ERROR){
			//get the error num
			iErrorCode = GetLastError();
			if(iErrorCode == WSAEWOULDBLOCK){
				::Sleep(1);
				continue;
			}else{
				break;	
			}
		}
		//成功接收一个连接
		STRU_SESSION *session = new STRU_SESSION;
		session->m_sock = client;
		//put into the map
		(p->m_mapSession)[session->m_sock] = session;

	}
	InterlockedDecrement(&(p->m_lThreadCount));
	return 0L;
}
//recv the message thread funtion
unsigned __stdcall WINAPI CTcpNet ::  ReadData(void* param){
	CTcpNet* p = (CTcpNet*)param;
	char recv_buf[MAX_RECV_BUF] = "";
	timeval time;
	int iRet;
	time.tv_sec = 0;
	time.tv_usec = 10;
	InterlockedIncrement(&(p->m_lThreadCount));
	while(p->m_bRun){
		fd_set readset;//every while need to have a new set. because when finish 
							//last set has been changed
		//clear
		FD_ZERO(&readset);
		//add
		std::map<SOCKET,STRU_SESSION*>::iterator it = (p->m_mapSession).begin();
		while( it != (p->m_mapSession).end()){
			FD_SET( it->second->m_sock,&readset);
		}
		//select 
		iRet = ::select(0,&readset,NULL,NULL,&time);
		if( iRet == 0)
			continue;//timeout
		else if( iRet == SOCKET_ERROR )
			break;
		//find the vary socket in set
		it = (p->m_mapSession).begin();
		while( it != (p->m_mapSession).end() ){
			if (FD_ISSET(it->second->m_sock,&readset) > 0){
				//the socket in the set to recv data
				iRet = ::recv(it->second->m_sock,recv_buf,sizeof(recv_buf),0);
				if(iRet == 0 || iRet == SOCKET_ERROR){
					//close and delete the session in map
				}else{
					//hand to the class who need 
				}
			}

		}


	}
	InterlockedDecrement(&(p->m_lThreadCount));
	return 0L;
}
BOOL CTcpNet :: innerInitNet(){
	WSADATA wsaData;
	int iResult = 0;

	SOCKET ListenSocket;
	//open the lib 
	iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		printf("WSAStartup() failed with error: %d\n", iResult);
		return 1;
	}
	//check the lib
	if(2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion)){
		return FALSE;
	}
	//create sock
	ListenSocket = ::WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	//init addr
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = GetHostIP();
	//bind 
	::bind(ListenSocket,(const sockaddr*)&addr,sizeof(addr));
	//listen
	::listen(ListenSocket,5);
	return TRUE;

}
LONG CTcpNet :: GetHostIP(){
	LONG iValue = inet_addr("127.0.0.1");
	
	char hostname[100] = "";
	//get the hostname 
	::gethostname(hostname,sizeof(hostname) - 1);	
	//get the host
	struct hostent* p = ::gethostbyname(hostname);
	if( p != NULL ){//the h_length is 4 seem to be the ipv4
		if(p->h_addr_list[0] != NULL && p->h_length == 4){
			//have the addr, alse ipv4 (ps :also can use type)
			iValue = *(long*)(p->h_addr_list[0]);
		}
	}
	//return ipv4
	return iValue;
}
