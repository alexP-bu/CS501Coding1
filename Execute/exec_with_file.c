#include "printfile.h"

int main(int argc, char* argv[]){

    if (argc != 4){
        printf("Usage: %s program.exe \"args and args \" outfile.txt\n", argv[0]);
        return 0;
    }
    // arse args
    char* program = argv[1];
    char* args = argv[2];
    char* outfile = argv[3];

    // create buffer for cmdline argument
    char* cmd = (char*)malloc(strlen(program) + strlen(args) + strlen(outfile));
    sprintf(cmd, "%s %s %s", program, args, outfile);

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    // Prevent dead squirrels 
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // TODO: Set si.dwFlags...
    // HINT Read this and look for anything that talks about handle inheritance :-)
    //  https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfoa
    si.dwFlags = STARTF_USESTDHANDLES;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;

    // TODO: ensure that the child processes can inherit our handles!
    // //your solution here!
    sa.bInheritHandle = TRUE;

    // TODO: Create a file  object and share the handle with a child processes 
    // //your solution here!
    LPCSTR lpFileName = outfile;
    DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
    DWORD dwShareMode = FILE_SHARE_WRITE | FILE_SHARE_READ;
    LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL; 
    DWORD dwCreationDisposition = CREATE_NEW;
    DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    HANDLE hTemplateFile = NULL;
    HANDLE hFile = CreateFileA(
        lpFileName, 
        dwDesiredAccess,
        dwShareMode,
        lpSecurityAttributes,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        hTemplateFile
    );

    si.hStdOutput = hFile;
    // Create the child Processes and wait for it to terminate!
    // //your solution here!
    LPCSTR lpApplicationName = NULL;
    LPSTR lpCommandLine = cmd;
    LPSECURITY_ATTRIBUTES lpProcessAttributes = &sa;
    LPSECURITY_ATTRIBUTES lpThreadAttributes = &sa;
    BOOL bInheritHandles = TRUE;
    DWORD dwCreationFlags = 0;
    LPVOID lpEnvironment = NULL;
    LPCSTR lpCurrentDirectory = NULL;
    LPSTARTUPINFOA lpStartupInfo = &si;
    LPPROCESS_INFORMATION lpProcessInformation = NULL;
    BOOL CreateProcessA(
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
    // TODO: perform any cleanup necessary! 
    // The parent processes no longer needs a handle to the child processes, the running thread, or the out file!
    // //your solution here!
    CloseHandle(lpProcessInformation->hProcess);
    CloseHandle(hFile);
    // Finally, print the contents of the file!
    PrintFileContents(outfile);
    return 0;
}