#pragma once

#ifndef __LOCK_H__
#define __LOCK_H__
#include <Windows.h>

class CLock{
public:
	CLock(){
		::InitializeCriticalSection(&m_slock);
	}
	BOOL Lock(){
		::EnterCriticalSection(&m_slock);
		return TRUE;
	}
	BOOL UnLock(){
		::LeaveCriticalSection(&m_slock);
		return TRUE;
	}
	~CLock(){
		::DeleteCriticalSection(&m_slock);
	}
private:
	CRITICAL_SECTION m_slock;
};
class CAutoLock{
public:
	CAutoLock(CLock* lock) : m_pLock(lock){
		m_pLock->Lock();	
	}
	~CAutoLock(){
		m_pLock->UnLock();
	}
private:
	CLock* m_pLock;
};

#endif//__LOCK_H__