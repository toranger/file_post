#include "FileOpr.h"
CFile :: CFile() : m_pfile(NULL){
	memset(m_pPath,0,sizeof(m_pPath));
}
CFile :: ~CFile(){
	if( m_pfile != NULL){
		CloseFile();
	}
}
BOOL CFile :: OpenFile(const char* pPath, enum ENUM_FILE_OPT opt){
	memset(m_pPath,0,sizeof(m_pPath));	
	strcpy_s(m_pPath,pPath);//strcpy include the /0
	if(m_pPath == NULL){
		return FALSE;
	}
	if( opt & enum_append ){//if have the append at the same time have another ways
		fopen_s(&m_pfile,m_pPath,"ba");	
	}else if( opt == enum_read ){//r
		fopen_s(&m_pfile,m_pPath,"br");	
	}else if( opt == enum_write){//w
		fopen_s(&m_pfile,m_pPath,"bw");	
	}else{//wr
		fopen_s(&m_pfile,m_pPath,"bwr");	
	}
	return TRUE;
}
void CFile :: CloseFile(){
	if( m_pfile ){
		fclose(m_pfile);
		m_pfile = NULL;
	}
}
long CFile :: ReadFile(char* pBuf,long lLen ){
	if( m_pfile == NULL)	
		return -1;
	return fread(pBuf,lLen,1,m_pfile);
} 
long CFile :: WriteFile(const char* pBuf, long lLen){
	if( m_pfile == NULL)
		return -1;
	return fwrite(pBuf,lLen,1,m_pfile);
}
BOOL CFile :: SetPos(DWORD dwPos){
	//the dwpos is the offset to the begin of the file
	if( m_pfile == NULL )
		return FALSE;
	DWORD size = GetFileSize();
	int iRet;
	if(size < dwPos)
		return FALSE;
	iRet = fseek(m_pfile,dwPos,SEEK_SET);
	if( iRet == 0 )
		return TRUE;
	else 
		return FALSE;
}
DWORD CFile :: GetFileSize(){
	//can use the fseek way, but it is not good one, so todo
	//TODO:
	//fseek to the end of file
	//ftell to show the size
	//fseek to the origin postion of the file
	return 0;
}

