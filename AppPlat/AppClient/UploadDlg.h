#pragma once


// CUploadDlg 对话框

class CUploadDlg : public CDialog
{
	DECLARE_DYNAMIC(CUploadDlg)

public:
	CUploadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUploadDlg();

// 对话框数据
	enum { IDD = IDD_UPLOADDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
