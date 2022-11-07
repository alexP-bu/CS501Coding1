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
    //si.dwFlags = STARTF_USESTDHANDLES;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;

    // ensure that the child processes can inherit our handles!
    sa.bInheritHandle = TRUE;

    // Create a pipe object and share the handle with a child processes 
    printf("TEST1");
    if(!CreatePipe(hStdOutRead, hStdOutWrite, &sa, BUF_SIZE)){
        printf("[!] FAILED TO CREATE PIPE!");
        return 0;
    }
    printf("TEST2");
    // set startupinfo handles
    si.hStdInput = NULL;
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;
    
    //Create the child Processes and wait for it to terminate!
    if(!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)){
        printf("[!] FAILED TO START PROCESS!");
        return 0;
    }
    char* lpBuffer = (char*)malloc(BUF_SIZE);
    if(lpBuffer = NULL){
        printf("[!] ERROR: MALLOC FAILED!");
    }
    // Finally, print the contents from the pipe!
    while(WaitForSingleObject(pi.hProcess, INFINITE)){
        while(PeekNamedPipe(hStdOutRead, lpBuffer, BUF_SIZE - 1, NULL, NULL, NULL)){
            if(ReadFile(hStdOutRead, lpBuffer, BUF_SIZE - 1, NULL, NULL)){
                lpBuffer[BUF_SIZE] = '\0';
                printf("%s", lpBuffer);
            }
        }
    }
    //perform any cleanup necessary!
    free(cmd);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(hStdOutRead);
    CloseHandle(hStdOutWrite); 
    
    return 0;
}