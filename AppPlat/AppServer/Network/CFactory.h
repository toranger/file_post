//the simple factory mode of c plus
//use this factory to build different kind of class which extend inet interface

#pragma once
#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "Tcpnet.h"
#include "Udpnet.h"
enum ENUM_NET_TYPE{
	enum_type_tcp,
	enum_type_udp
};
class CFactoryNet{
public:
	INet* GetInstance(enum ENUM_NET_TYPE type){
		int index = type;	
		INet* tmp = NULL;
		switch(index){
			case enum_type_tcp:
				{
					tmp = new CTcpNet;
				}
				break;
			case enum_type_udp:
				{
					tmp = new CUdpNet;
				}
				break;
		}
		return tmp;
	}
};


#endif//__FACTORY_H__