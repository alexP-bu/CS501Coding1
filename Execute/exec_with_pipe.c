#include <windows.h>
#include <stdio.h>
#define BUF_SIZE 4096

int main(int argc, char* argv[]){

    if (argc != 3){
        printf("Usage: %s program.exe \"args and args \"", argv[0]);
        return 0;
    }

    // parse args
    char* program = argv[1];
    char* args = argv[2];

    // create buffer for cmdline argument
    char* cmd = (char *)malloc(
        strlen("cmd.exe /c") + 
        strlen(program) + 
        strlen(args) + 
        1 //space between %s
        );
    sprintf(cmd, "cmd.exe /c %s %s", program, args);
    
    // Declare handles for StdOut
    HANDLE hStdOutRead, hStdOutWrite;
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

    // nsure that the child processes can inherit our handles!
    sa.bInheritHandle = TRUE;
    // Create a pipe  object and share the handle with a child processes 
    DWORD nSize = BUF_SIZE;
    if(!CreatePipe(hStdOutRead, hStdOutWrite, &sa, nSize)){
        printf("[!] FAILED TO CREATE PIPE!");
    }

    // set startupinfo handles
    si.hStdInput = NULL;
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;
    
    // Create the child Processes and wait for it to terminate!
    if(!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)){
        printf("[!] FAILED TO START PROCESS!");
    }

    // TODO: perform any cleanup necessary! 
    // The parent processes no longer needs a handle to the child processes, the running thread, or the out file!
    // //your solution here!
    // Finally, print the contents from the pipe!
    //
    return 0;
}