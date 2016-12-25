#pragma once
#ifndef __FILE_OPR_H__
#define __FILE_OPR_H__
#include "IFile.h"
#include <stdio.h>
class CFile : public IFile{
public:
	CFile();
	~CFile();
public:
	virtual BOOL OpenFile(const char* pPath, enum ENUM_FILE_OPT opt);
	virtual void CloseFile();
	virtual	long ReadFile(char* pBuf,long lLen ); 
	virtual long WriteFile(const char* pBuf, long lLen);
	virtual BOOL SetPos(DWORD dwPos);
	virtual DWORD GetFileSize();


public:
	FILE* m_pfile;
	char  m_pPath[MAX_PATH];
};


#endif//__FILE_OPR_H__