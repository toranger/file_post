// UploadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AppClient.h"
#include "UploadDlg.h"
#include "afxdialogex.h"


// CUploadDlg 对话框

IMPLEMENT_DYNAMIC(CUploadDlg, CDialog)

CUploadDlg::CUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUploadDlg::IDD, pParent)
{

}

CUploadDlg::~CUploadDlg()
{
}

void CUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUploadDlg, CDialog)
END_MESSAGE_MAP()


// CUploadDlg 消息处理程序
