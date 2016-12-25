//the interface of the fileopration
#pragma once
#ifndef __IFILE_H__
#define __IFILE_H__
#include <Windows.h>
enum ENUM_FILE_OPT{//use this way can use the bit operation
	enum_read = 1,
	enum_write = 2,
	enum_append =4
};
class IFile{
public:
	virtual BOOL OpenFile(const char* pPath, enum ENUM_FILE_OPT opt) = 0;
	virtual void CloseFile() = 0;
	virtual	long ReadFile(char* pBuf,long lLen ) = 0; 
	virtual long WriteFile(const char* pBuf, long lLen) = 0;
	virtual BOOL SetPos(DWORD dwPos) = 0;
	virtual DWORD GetFileSize() = 0;
};
#endif//__IFILE_H__