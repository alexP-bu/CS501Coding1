#include "printfile.h"

int main(int argc, char* argv[]){

    if (argc != 4){
        printf("Usage: %s program.exe \"args and args \" outfile.txt\n", argv[0]);
        return 0;
    }
    //arse args
    char* program = argv[1];
    char* args = argv[2];
    char* outfile = argv[3];

    // create buffer for cmdline argument same as exec with file except without redir and outfile (will be made later)
    char* cmd = (char *)malloc(
        strlen("cmd.exe /c") +
        strlen(program) + 
        strlen(args) + 
        1 //1 space between %s
        );

    sprintf(cmd, "cmd.exe /c %s %s", program, args);

    if(cmd == NULL){
        printf("[!] ERROR: MALLOC FAILED!");
    }

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    // Prevent dead squirrels 
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // Set si.dwFlags...
    // HINT Read this and look for anything that talks about handle inheritance :-)
    //  https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfoa
    si.dwFlags = STARTF_USESTDHANDLES;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;

    //ensure that the child processes can inherit our handles!
    sa.bInheritHandle = TRUE;

    //Create a file  object and share the handle with a child processes 
    // //your solution here!
    HANDLE hFile = CreateFileA(outfile, 
                               GENERIC_WRITE, 
                               0, 
                               &sa, 
                               CREATE_ALWAYS, 
                               FILE_ATTRIBUTE_NORMAL, 
                               NULL);
    if(!hFile){
        printf("[!] ERROR: INVALID FILE HANDLE!");
    }

    // set startupinfo handles
    // //your solution here!
    si.hStdOutput = hFile;
    
    // Create the child Processes and wait for it to terminate!
    // //your solution here!
    if(!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)){
        printf("[!] ERROR: FAILED TO CREATE PROCESS!");
    }

    DWORD dwMS = INFINITE;
    WaitForSingleObject(pi.hThread, dwMS);
    WaitForSingleObject(pi.hProcess, dwMS);

    // perform any cleanup necessary! 
    // The parent processes no longer needs a handle to the child processes, the running thread, or the out file!
    free(cmd);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(hFile);
    // Finally, print the contents of the file!
    if(!PrintFileContents(outfile)){
        printf("[!] ERROR PRINTING FILE CONTENTS");
    }

    return 0;
}