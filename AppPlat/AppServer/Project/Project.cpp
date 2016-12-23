#include<iostream>
#include "Tcpnet.h"//the winsock2.h must be front of windows.h
#include "Circle_que.h"
#include "MyDao.h"
#include "Control.h"

class Test{
public:
	Test(int value) : m_iValue(value){

	}
	void Print(){
		cout << m_iValue <<endl;
	}
private:
	int m_iValue;

};
int main(){
	CircleQue<Test> que;
	que.Enque(new Test(1));
	
	return 0;
}