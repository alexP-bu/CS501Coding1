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
    char* lpBuffer = (char*)malloc(dwFileSize + 1);
    if(lpBuffer == NULL){
        printf("[!] ERROR: MALLOC FAILED!");
        return FALSE;
    }
    DWORD nNumberOfBytesToRead = dwFileSize + 1;
    LPDWORD lpNumberOfBytesRead = NULL;
    LPOVERLAPPED lpOverlapped = NULL;
    if(!ReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped)){
        printf("[!] ERROR: FAILED TO READ FILE!");
    }

    printf(lpBuffer);

    LPCSTR lpApplicationName = NULL;
    // echo "thisiswhoiam" >> outfile.txt
    char* cmd = (char *) malloc(strlen("echo ") + strlen(lpBuffer) + strlen(">>") + strlen("outfile.txt"));
    sprintf(cmd, "echo \"%s\" >> outfile.txt", lpBuffer);
    printf(cmd);
    LPSTR lpCommandLine = cmd;
    LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL;
    LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
    WINBOOL bInheritHandles = TRUE; 
    DWORD dwCreationFlags = CREATE_NO_WINDOW;
    LPVOID lpEnvironment = NULL;
    LPCSTR lpCurrentDirectory = NULL;

    STARTUPINFOA si;
    GetStartupInfoA(&si);
    LPSTARTUPINFOA lpStartupInfo = &si;
    LPPROCESS_INFORMATION lpProcessInformation = NULL; 
    BOOL proc = CreateProcessA(
        lpApplicationName, 
        lpCommandLine, 
        lpProcessAttributes, 
        lpThreadAttributes, 
        bInheritHandles, 
        dwCreationFlags, 
        lpEnvironment, 
        lpCurrentDirectory, 
        lpStartupInfo, 
        lpProcessInformation);

    DWORD dwMilliseconds = INFINITE;
    WaitForSingleObject(lpProcessInformation->hProcess, dwMilliseconds);
}