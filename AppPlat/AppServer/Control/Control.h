#pragma once 
#include <Windows.h>
/* access to build control class to realize the single mode of c++ */
/* 当用统一的control类去实现时getinstance 和 destoryinstance 是初始化和销毁
资源的过程，父类不知道子类需要有什么成员需要初始化和销毁，所以提供子类的初始化
和销毁函数,以及对应类模板*/

//利用了单例和模板模式
//多态分为静态多态和动态多态，动态多态利用指针和虚函数表消耗内存， 
//而模板模式利用静态多态采用模板完成class<T> a; a->getinstance()对应子类的操作 
#ifndef __CONTROL_H__
#define __CONTROL_H__

template<typename T>
class CControl{
protected://防止外界直接创造对象
	CControl();
	~CControl();
	CControl& operator = (CControl& control);
	CControl(CControl& control);
private:
	static CControl *m_pSingleton;
public:
	static CControl* getInstance();
	static void destoryInsstance();
	//setConsoleCtrlHandler 事件处理机制
	//封装控制台事件处理函数
	static BOOL WINAPI HandlerRoutine(DWORD wdCtrlType);
	BOOL OpenControl();
	void CloseControl();
	//提供给子类实现资源的初始化与销毁
	BOOL Init();
	void UnInit();
	 
	

public:
	// the index of whether console quit to notify main thread
	static BOOL m_bQuit;


};

//use the class template
//static number to init
template<typename T>
CControl<T>* CControl<T>::m_pSingleton = NULL;
template<typename T>
BOOL CControl<T>::m_bQuit = FALSE;
template<typename T>
CControl<T>::CControl(){

}
template<typename T>
CControl<T>::~CControl(){

}
template<typename T>
CControl<T>& CControl<T>::operator = (CControl<T>& control){
	return *CControl<T>::m_pSingleton;
}
template<typename T>
CControl<T>::CControl(CControl<T>& control){

}
template<typename T>
CControl<T>* CControl<T>::getInstance(){
	if( CControl<T>::m_pSingleton == NULL){
		//notice here
		CControl<T>::m_pSingleton = new T;
	}
	return CControl<T>::m_pSingleton;
}
template<typename T>
void CControl<T>::destoryInsstance(){
	if( CControl<T>::m_pSingleton != NULL){
		//强制调用子类的析构
		delete (T*)CControl<T>::m_pSingleton;
		CControl::m_pSingleton = NULL;
	}
	return ;
}
template<typename T>
BOOL WINAPI CControl<T>::HandlerRoutine(DWORD wdCtrlType){
	switch(wdCtrlType){
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		{
			CControl<T>::m_bQuit = TRUE;
			while(1){
				Sleep(10);
			}
		}
		break;
	}
	return true;
}
template<typename T>
BOOL CControl<T>::OpenControl(){
	SetConsoleCtrlHandler(CControl<T>::HandlerRoutine,TRUE);

	((T*)CControl<T>::m_pSingleton)->Init();
	return TRUE;
}
template<typename T>
void CControl<T>::CloseControl(){
	((T*)CControl<T>::m_pSingleton)->UnInit();
}


#endif 



