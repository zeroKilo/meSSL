#include "stdafx.h"
void hexdump(void *pAddressIn, long  lSize)
{
 char szBuf[100];
 long lIndent = 1;
 long lOutLen, lIndex, lIndex2, lOutLen2;
 long lRelPos;
 struct { char *pData; unsigned long lSize; } buf;
 unsigned char *pTmp,ucTmp;
 unsigned char *pAddress = (unsigned char *)pAddressIn;

   buf.pData   = (char *)pAddress;
   buf.lSize   = lSize;

   while (buf.lSize > 0)
   {
      pTmp     = (unsigned char *)buf.pData;
      lOutLen  = (int)buf.lSize;
      if (lOutLen > 16)
          lOutLen = 16;

      sprintf(szBuf, " >                            "
                     "                      "
                     "    %08lX", pTmp-pAddress);
      lOutLen2 = lOutLen;

      for(lIndex = 1+lIndent, lIndex2 = 53-15+lIndent, lRelPos = 0;
          lOutLen2;
          lOutLen2--, lIndex += 2, lIndex2++
         )
      {
         ucTmp = *pTmp++;

         sprintf(szBuf + lIndex, "%02X ", (unsigned short)ucTmp);
         if(!isprint(ucTmp))  ucTmp = '.';
         szBuf[lIndex2] = ucTmp;

         if (!(++lRelPos & 3))
         {  lIndex++; szBuf[lIndex+2] = ' '; }
      }

      if (!(lRelPos & 3)) lIndex--;

      szBuf[lIndex  ]   = '<';
      szBuf[lIndex+1]   = ' ';

      printf("%s\n", szBuf);

      buf.pData   += lOutLen;
      buf.lSize   -= lOutLen;
   }
}

MH_STATUS WINAPI MH_CreateHookApiEx(LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, LPVOID *ppOriginal,BOOL bQueue)
{
  HMODULE hModule;
  LPVOID pTarget;
  MH_STATUS err;
  hModule = GetModuleHandleW(pszModule);
  if (hModule == NULL)
    return MH_ERROR_MODULE_NOT_FOUND;
  pTarget = (LPVOID)GetProcAddress(hModule, pszProcName);
  if (pTarget == NULL)
    return MH_ERROR_FUNCTION_NOT_FOUND;
  err = MH_CreateHook(pTarget, pDetour, ppOriginal);
  if (err == MH_OK && bQueue)
    err = MH_QueueEnableHook(pTarget);
  return err;
}

void OpenConsole()
{
			AllocConsole();
			freopen("conin$","r",stdin);
			freopen("conout$","w",stdout);
			freopen("conout$","w",stderr);
			HWND consoleHandle = GetConsoleWindow();
			MoveWindow(consoleHandle,1,1,680,480,1);
			printf("Console initialized.\n");
}

void SaveData(void* buff, unsigned long size, char* ending, long index)
{
	char filename[50];
	int len = sprintf(filename,"dump%08x%s.bin", index, ending);
	FILE *f = fopen(filename, "w");
	fwrite( buff, 1, size, f );
	fclose(f);
}