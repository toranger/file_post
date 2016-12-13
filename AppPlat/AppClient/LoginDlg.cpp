// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AppClient.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialog)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	ON_STN_CLICKED(IDC_USER, &CLoginDlg::OnStnClickedUser)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


void CLoginDlg::OnStnClickedUser()
{
	// TODO: 在此添加控件通知处理程序代码
}
