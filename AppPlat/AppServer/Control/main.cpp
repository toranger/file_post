#include<iostream>
#include "Control.h"

using namespace std;

class Test :  public CControl<Test>{
public:
	BOOL Init(){
		cout << "Test" << endl;
		return TRUE;
	}
	void UnInit(){
		cout << "Un test" << endl;
		return ;
	}

};
int main(){
	CControl<Test>* p = CControl<Test>::getInstance();
	p->OpenControl();
	while(CControl<Test>::m_bQuit == FALSE){
		Sleep(1);
	}
	p->CloseControl();
	system("pasue");
	return 0;


}
