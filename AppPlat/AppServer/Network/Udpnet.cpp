#include "Udpnet.h"
#include <process.h>
CUdpNet:: CUdpNet() : m_sock(NULL), m_lThreadCount(0), m_bRun(FALSE), m_pKernel(NULL){
	
}
CUdpNet:: ~CUdpNet(){

}
BOOL CUdpNet:: InitNet(IKernel* kernel){
	UnInitNet();
	//do not have the accept thread
	innerInitNet();
	m_pKernel = kernel;
	//create recv thread
	HANDLE handler = (HANDLE)::_beginthreadex(NULL,0,ReadData,this,0,NULL);
	if(handler == NULL)
		return FALSE;
	::CloseHandle(handler);
	return TRUE;
}
//recv the message thread funtion
unsigned int WINAPI CUdpNet :: ReadData(void* param){
	CUdpNet* p = (CUdpNet*)param;
	p->ReadData();
	return 0L;
}
void CUdpNet :: ReadData(){
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
		FD_SET(m_sock,&readset);//only the sock
		//select 
		iRet = ::select(0,&readset,NULL,NULL,&time);
		if( iRet == 0)
			continue;//timeout
		else if( iRet == SOCKET_ERROR )
			break;
		SOCKADDR_IN clientAddr;
		int addr_len = sizeof(clientAddr);
		if( FD_ISSET(m_sock,&readset) > 0 ){//get 
			iRet = ::recvfrom(m_sock,recv_buf,sizeof(recv_buf),0,(sockaddr*)(&clientAddr),
				&addr_len);
			if( iRet > 0 ){//recv the buf
				//according to the INT64 to find
				INT64 key64 = ( ((INT64)(clientAddr.sin_addr.S_un.S_addr)) << 32 ) +
					clientAddr.sin_port;
				std::map<INT64,STRU_SESSION*>::iterator it =  m_mapSession.find(key64);
				STRU_SESSION* tmp = it->second;
				if( it != m_mapSession.end() ){//find the session 

				}else{//not find to create new one and insert into map
					tmp = new STRU_SESSION;
					tmp->m_sock = key64;
					m_mapSession[key64] = tmp;
				}
				//to notify the kernel class to use
				if(m_pKernel){
					 m_pKernel->OnRecvData(tmp,recv_buf,iRet,enum_recv_udp);
				}

			}//end of recv
		}//end of get the sock from set
	}//end of the run
	InterlockedDecrement(&(m_lThreadCount));
}
void CUdpNet:: UnInitNet(){
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
		std::map<INT64,STRU_SESSION*>::iterator it = m_mapSession.begin();
		while( it != m_mapSession.end()){
			delete (STRU_SESSION*)(it->second);
			++it;
		}
		m_mapSession.clear();
		//close the lib
		::WSACleanup();
	}
} 
long CUdpNet:: SendData(STRU_SESSION* pSession, 
		const char* pData, long lDataLen){
	SOCKADDR_IN sockadd;
	sockadd.sin_family = AF_INET;
	sockadd.sin_port = (WORD)(pSession->m_sock);//low two bytes
	sockadd.sin_addr.S_un.S_addr = (DWORD)((pSession->m_sock) >> 32);//high 4 bytes
	return ::sendto(m_sock,pData,lDataLen,0,(sockaddr*)&sockadd,
		sizeof(sockadd));
}
BOOL CUdpNet :: innerInitNet(){
	WSADATA wsaData;
	int iResult = 0;
	//open the lib 
	iResult = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		return 1;
	}
	//check the lib
	if(2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wVersion)){
		return FALSE;
	}
	//create sock
	m_sock = ::socket(AF_INET,SOCK_DGRAM,0);
	//init addr
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(54321);
	addr.sin_addr.S_un.S_addr = GetHostIP();
	//bind 
	::bind(m_sock,(const sockaddr*)&addr,sizeof(addr));
	//set boardcast
	BOOL flag = TRUE;
	setsockopt(m_sock,SOL_SOCKET,SO_BROADCAST,(char*)&flag,sizeof(flag));
	return TRUE;

}
LONG CUdpNet :: GetHostIP(){
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
	
BOOL CUdpNet :: BreakConn(STRU_SESSION* pSession){
	//use the ip and port into the udp sock
	INT64 i64key = pSession->m_sock; 
	//find the item in map
	std::map<INT64,STRU_SESSION*>::iterator it = m_mapSession.find(i64key);	
	if(it == m_mapSession.end()){
		//dont find in map
		return FALSE;
	}
	//free the memery
	delete pSession;
	pSession = NULL;	
	//remove this item from the map
	m_mapSession.erase(it);
	return TRUE;
}
