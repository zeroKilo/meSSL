#include "stdafx.h"

typedef SECURITY_STATUS (_stdcall *MyDecrypt)(PCtxtHandle, PSecBufferDesc, ULONG, PULONG);
typedef SECURITY_STATUS (_stdcall *MyEncrypt)(PCtxtHandle, ULONG, PSecBufferDesc, ULONG );

MyDecrypt orgDecrypt = NULL;
MyEncrypt orgEncrypt = NULL;
long count = 0;


SECURITY_STATUS _stdcall DetouredDecrypt(PCtxtHandle phContext, PSecBufferDesc pMessage, ULONG MessageSeqNo, PULONG pfQOP)
{
	SECURITY_STATUS result = orgDecrypt(phContext, pMessage, MessageSeqNo, pfQOP);
	printf("Decrypt called!\n BufferCount=%x\n", pMessage->cBuffers);
	for(int i=0; i<pMessage->cBuffers;i++)
	{			
		void* buff = pMessage->pBuffers[i].pvBuffer;
		unsigned long size = pMessage->pBuffers[i].cbBuffer;
		unsigned long type = pMessage->pBuffers[i].BufferType;
		printf(" Buffer#%d Address:0x%x Size:0x%x Type:0x%x\n", i, buff, size, type);
		if((type != 1 && type != 5) || size == 0)
			continue;
		SaveData(buff, size,"_D", count++);
	}
    return result;
}

SECURITY_STATUS _stdcall DetouredEncrypt(PCtxtHandle phContext, ULONG fQOP, PSecBufferDesc pMessage,  ULONG MessageSeqNo)
{ 
	printf("Encrypt called!\n BufferCount=%x\n", pMessage->cBuffers);
	for(int i=0; i<pMessage->cBuffers;i++)
	{	
		void* buff = pMessage->pBuffers[i].pvBuffer;
		unsigned long size = pMessage->pBuffers[i].cbBuffer;
		unsigned long type = pMessage->pBuffers[i].BufferType;
		printf(" Buffer#%d Address:0x%x Size:0x%x Type:0x%x\n", i, buff, size, type);
		if((type != 1 && type != 5) || size == 0)
			continue;
		SaveData(buff, size,"_E", count++);
	}
    return orgEncrypt(phContext, fQOP, pMessage, MessageSeqNo);
}

void HookSSL()
{
	if (MH_Initialize() != MH_OK)
		printf("Init failed!\n");
	else
		printf("Init succeded!\n");
	if (MH_CreateHookApiEx(L"secur32.dll", "DecryptMessage", DetouredDecrypt, (LPVOID*)&orgDecrypt, true) != MH_OK)
        printf("Decrypthook failed!\n");
	else
		printf("Decrypthook succeded!\n");
	if (MH_CreateHookApiEx(L"secur32.dll", "EncryptMessage", DetouredEncrypt, (LPVOID*)&orgEncrypt, true) != MH_OK)
        printf("Encrypthook failed!\n");
	else
		printf("Encrypthook succeded!\n");
	MH_ApplyQueued();
}

void Main()
{
	OpenConsole();
	printf("meSSL injected!\n");
	HookSSL();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			Main();
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}

