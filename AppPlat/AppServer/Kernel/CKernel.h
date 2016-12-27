#pragma once
#ifndef __KERNEL_H__
#define __KERNEL_H__ 
#include "Control.h"
#include "MyDao.h"
#include "FileOpr.h"//when use the interface there need have the factory mode
#include "CFactory.h"
#include "CProtocol.h"


class CKernel : public CControl<CKernel>{

};


#endif//__KERNEL_H__