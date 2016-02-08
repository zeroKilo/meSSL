void hexdump(void *pAddressIn, long  lSize);
MH_STATUS WINAPI MH_CreateHookApiEx(LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, LPVOID *ppOriginal,BOOL bQueue);
void OpenConsole();
void SaveData(void* buff, unsigned long size, char* ending, long index);