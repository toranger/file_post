#pragma once
/* keep the write and read positon is not same, so that 
	can do not use the lock. if eeach thread has one container
	the postion don set also need to be locked when io opt*/
#ifndef __CIRCLE_QUE_H__
#define __CIRCLE_QUE_H__
#include<Windows.h>
template<typename T,const long MAX_LEN=10000>
class CircleQue{
public:
	CircleQue() : m_lQueLen(0), m_pQue(NULL), m_lRPos(0), m_lWPos(0), m_bOK(FALSE){
		
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
		if(m_pQue){
			delete m_pQue;
			m_pQue = NULL;
			m_lRPos = m_lWPos;
			m_lQueLen = 0;
			m_bOK = FALSE;
		}
		return ;
	}
	void Clear(){
		if( m_bOK == FALSE)
			return;
		if(m_lRPos == m_lWPos)
			return ;
		for(long i = m_lRPos; i != m_lWPos; i=(i+1)%m_lQueLen){
			delete m_pQue[i];
			m_pQue[i] = NULL;
		}
		return ;
	}
	BOOL Enque(T* node){
		if( m_bOK == FALSE)
			return FALSE;
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
		*node = m_pQue[m_lRPos];
		m_pQue[m_lRPos] = NULL;
		//change the index
		m_lRPos = (m_lRPos + 1) % m_lQueLen; 
		return TRUE;
	}
private:
	T** m_pQue;
	long m_lQueLen;
	long m_lRPos;
	long m_lWPos;
	BOOL m_bOK;
};
#endif//__CIRCLE_QUE_H__