#pragma once
/* keep the write and read positon is not same, so that 
	can do not use the lock. if eeach thread has one container
	the postion don set also need to be locked when io opt*/
#ifndef __LOCK_QUE_H__
#define __LOCK_QUE_H__
#include<Windows.h>
#include "Lock.h"
template<typename T,const long MAX_LEN=10000>
class CLockQue{
public:
	CLockQue() : m_lQueLen(0), m_pQue(NULL), m_lRPos(0), m_lWPos(0), m_bOK(FALSE){
		
	}
	BOOL Init(long len){
		UnInit();//可重入函数 用于递归调用，多线程
		if( len > MAX_LEN)
			return FALSE;
		m_lQueLen = len + 1;//there have one postion does not work
		m_pQue = new T*[len + 1];
		//to init
		for(long i = 0;i<m_lQueLen;i++){
			m_pQue[i] = NULL;
		}
		m_bOK = TRUE;
		return TRUE;
	}
	void UnInit(){
		m_bOK = FALSE;
		//wait the thread out
		::Sleep(5);

		if(m_pQue){
			delete m_pQue;
			m_pQue = NULL;
			m_lRPos = m_lWPos;
			m_lQueLen = 0;
		}
		return ;
	}
	void Clear(){
		if( m_bOK == FALSE)
			return;
		m_oLock.Lock();
		if(m_lRPos == m_lWPos)
			return ;
		for(long i = m_lRPos; i != m_lWPos; i=(i+1)%m_lQueLen){
			delete m_pQue[i];
			m_pQue[i] = NULL;
		}
		m_oLock.UnLock();
		return ;
	}
	BOOL Enque(T* node){
		if( m_bOK == FALSE)
			return FALSE;
		CAutoLock lock(&m_oLock);
		//get the index
		long index = (m_lWPos + 1) % m_lQueLen;
		if(index != m_lRPos){
			//first put in then change the wpos	
			m_pQue[index] = node;
			m_lWPos = index;
			return TRUE;
		}
		return FALSE;
	}
	BOOL Deque(T** node){
		if(m_lRPos == m_lWPos || m_bOK == FALSE)
			return FALSE;
		CAutoLock lock(&m_oLock);
		*node = m_pQue[m_lRPos];
		m_pQue[m_lRPos] = NULL;
		//change the index
		m_lRPos = (m_lRPos + 1) % m_lQueLen; 
		return TRUE;
	}
	BOOL IsEmpty(){
		if(m_lRPos == m_lWPos)
			return TRUE;
		return FALSE;
	}
	BOOL IsFull(){
		long index = (m_lWPos + 1) % m_lQueLen;
		if(index == m_lRPos){
			return TRUE;
		}
		return FALSE;
	}
private:
	T** m_pQue;
	long m_lQueLen;
	long m_lRPos;
	long m_lWPos;
	BOOL m_bOK;
	CLock m_oLock;

};
#endif//__LOCK_QUE_H__