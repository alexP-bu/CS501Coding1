#include <windows.h>
#include <stdio.h>

//your solution here!
BOOL PrintFileContents(char* filename){
    if(filename == NULL){
        return FALSE;
    }
    //create file
    LPCSTR lpFileName = filename;
    DWORD dwDesiredAccess = GENERIC_READ; 
    DWORD dwShareMode = FILE_SHARE_READ;
    LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL;
    DWORD dwCreationDisposition = OPEN_EXISTING;
    DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE hTemplateFile = NULL;
    HANDLE hFile = CreateFileA(lpFileName, 
                        dwDesiredAccess, 
                        dwShareMode, 
                        lpSecurityAttributes, 
                        dwCreationDisposition, 
                        dwFlagsAndAttributes, 
                        hTemplateFile);
    if(hFile == INVALID_HANDLE_VALUE){
        printf("[!] ERROR: INVALID FILE HANDLE!");
        return FALSE;
    }

    //read file
    LPDWORD lpFileSizeHigh = NULL;
    DWORD dwFileSize = GetFileSize(hFile, lpFileSizeHigh);
    char* lpBuffer = (char*)malloc(dwFileSize + 1); //we need the extra char to read the null terminator
    if(lpBuffer == NULL){
        printf("[!] ERROR: MALLOC FAILED!");
        return FALSE;
    }
    DWORD nNumberOfBytesToRead = dwFileSize;
    LPDWORD lpNumberOfBytesRead = NULL;
    LPOVERLAPPED lpOverlapped = NULL;
    if(!ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped)){
        printf("[!] ERROR: FAILED TO READ FILE!");
        return FALSE;
    }

    lpBuffer[dwFileSize] = '\0'; // this is why added the +1 earlier!
    printf(lpBuffer);
    
    //clean up
    free(lpBuffer);
    CloseHandle(hFile);
    
    return TRUE;
}