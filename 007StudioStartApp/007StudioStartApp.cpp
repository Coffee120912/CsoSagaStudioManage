// 007StudioStartApp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <CACEInterface.h>
#include <RCF/RCF.hpp>
#include <RCF/RcfServer.hpp>
#include <RCF/Win32NamedPipeEndpoint.hpp>
#include <RCF../../../RCFCsoStudioInterface.hpp>
#include <PackWrite.h>
#include "../Connect/ShareDef.h"
#include "LoginModule.h"
#include <csv2/writer.hpp>
#include <csv2/parameters.hpp>

#pragma comment(lib,"openssl/win32/lib/libcrypto.lib")
#pragma comment(lib,"openssl/win32/lib/libssl.lib")
#pragma comment(lib,"Crypt32.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"wldap32.lib")
#pragma comment(lib,"RcfDll.lib")

class MyClass
{
public:
	MyClass();
	~MyClass();
	int GetNumber();
private:
};

MyClass::MyClass()
{
	printf(__FUNCTION__);

}

MyClass::~MyClass()
{
	printf(__FUNCTION__);
}

int MyClass::GetNumber()
{
	printf(__FUNCTION__);
	return 123;
}

int main()
{
	int TypeFlag = 3 | 1 << 0;
	

	TypeFlag &= ~(1 << 0) & 0xF;


	getchar();
	
}

