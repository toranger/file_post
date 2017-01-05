#include "Tcpnet.h"
#include <process.h>
#define TEST
#ifdef TEST
#include <iostream>
using namespace std;
#endif
CTcpNet :: CTcpNet() : m_sock(NULL), m_lThreadCount(0), m_bRun(FALSE) {

}
CTcpNet :: ~CTcpNet(){
}
//实现基类接口
BOOL CTcpNet :: InitNet(IKernel *kernel){
	UnInitNet();
	//the inner init for network
	innerInitNet();
	//keep the class of the interface of kernel
	m_pKernel = kernel;
	//create accept thread
	HANDLE handler = (HANDLE)::_beginthreadex(NULL,0,Accept,this,0,NULL);
	if(handler == NULL)
		return FALSE;
	::CloseHandle(handler);
	//create recv thread
	handler = (HANDLE)::_beginthreadex(NULL,0,ReadData,this,0,NULL);
	if(handler == NULL)
		return FALSE;
	::CloseHandle(handler);
	return TRUE;
}
void CTcpNet :: UnInitNet(){
	m_bRun = FALSE;
	while( m_lThreadCount > 0 ){
		//wait the thread out
		::Sleep(1);
	}
	//close socket
	if( m_sock != NULL){
		::closesocket(m_sock);
		m_sock = NULL;
		//delete session
		std::map<SOCKET,STRU_SESSION*>::iterator it = m_mapSession.begin();
		while( it != m_mapSession.end()){
			delete (STRU_SESSION*)(it->second);
			++it;
		}
		m_mapSession.clear();
		//close the lib
		::WSACleanup();
	}
	return ;
}
long CTcpNet :: SendData(STRU_SESSION* pSession, 
	const char* pData, long lDataLen){
		return ::send((SOCKET)(pSession->m_sock),pData,lDataLen,0);
}
//accept the link thread funtion  the __stdcall can be int
unsigned int WINAPI CTcpNet ::  Accept(void* param){
	//get the param
	CTcpNet* p = (CTcpNet*)param;
	p->Accept();
	return 0L;
}
//recv the message thread funtion
unsigned int WINAPI CTcpNet ::  ReadData(void* param){
	CTcpNet* p = (CTcpNet*)param;
	p->ReadData();
	return 0L;
}
BOOL CTcpNet :: innerInitNet(){
	WSADATA wsaData;
	int iResult = 0;

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
	m_sock = ::socket(AF_INET,SOCK_STREAM,0);
	//init addr
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = GetHostIP();
	//bind 
	::bind(m_sock,(const sockaddr*)&addr,sizeof(addr));
	//listen
	::listen(m_sock,5);
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
BOOL CTcpNet :: DelInvalSession(SOCKET sock){
	//before find need lock
	m_lock.Lock();
	std::map<SOCKET,STRU_SESSION*>::iterator it = m_mapSession.find(sock);
	//have two way to erase but often use the return, not the it++
	if( it == m_mapSession.end() ){
		//not found
		m_lock.UnLock();
		return FALSE;
	}else{
		delete (STRU_SESSION*)(it->second); //also delete the derived class
		it = m_mapSession.erase(it); 
	}
	m_lock.UnLock();
	return TRUE;
}

void CTcpNet :: Accept(){
	u_long flag = 100;
	SOCKET client;
	int iErrorCode = 0;
	//利用原子锁多threadcount进行操作
	InterlockedIncrement(&(m_lThreadCount));
	//set the unblock socket to socket
	::ioctlsocket(m_sock,FIONREAD ,&flag);
	//to accept the link
	while(m_bRun){
		client = ::accept(m_sock,NULL,NULL);
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
		//when opt the map need to use lock
		(m_lock).Lock();
		(m_mapSession)[(SOCKET)(session->m_sock)] = session;
		(m_lock).UnLock();
	}
	InterlockedDecrement(&(m_lThreadCount));
	return ;
}
void CTcpNet :: ReadData(){
	char recv_buf[MAX_RECV_BUF] = "";
	timeval time;
	int iRet;
	time.tv_sec = 0;
	time.tv_usec = 10;
	InterlockedIncrement(&(m_lThreadCount));
	while(m_bRun){
		fd_set readset;//every while need to have a new set. because when finish 
							//last set has been changed
		//clear
		FD_ZERO(&readset);
		//add
		std::map<SOCKET,STRU_SESSION*>::iterator it = (m_mapSession).begin();
		while( it != (m_mapSession).end()){
			FD_SET((SOCKET)(it->second->m_sock),&readset);
		}
		//select 
		iRet = ::select(0,&readset,NULL,NULL,&time);
		if( iRet == 0)
			continue;//timeout
		else if( iRet == SOCKET_ERROR )
			break;
		//find the vary socket in set
		it = (m_mapSession).begin();
		while( it != (m_mapSession).end() ){
			if (FD_ISSET(it->second->m_sock,&readset) > 0){
				//the socket in the set to recv data
				iRet = ::recv((SOCKET)(it->second->m_sock),recv_buf,sizeof(recv_buf),0);
				if(iRet == 0 || iRet == SOCKET_ERROR){
					//close and delete the session in map
					DelInvalSession(m_sock);
					
				}else{
					//hand to the class who need 
					m_pKernel->OnRecvData(it->second,recv_buf,iRet,enum_recv_tcp);
				}
			}
		}
	}
	InterlockedDecrement(&(m_lThreadCount));
}
BOOL CTcpNet :: BreakConn(STRU_SESSION* pSession){
	//use the ip and port into the udp sock
	SOCKET i64key = (SOCKET)pSession->m_sock; 
	//find the item in map
	std::map<SOCKET,STRU_SESSION*>::iterator it = m_mapSession.find(i64key);	
	if(it == m_mapSession.end()){
		//dont find in map
		return FALSE;
	}
	//close the client socket in map;
	::closesocket(i64key);
	//free the memery
	delete pSession;
	pSession = NULL;	
	//remove this item from the map
	m_mapSession.erase(it);
	return TRUE;
}
