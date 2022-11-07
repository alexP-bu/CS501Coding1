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
    char* cmd = (char *)malloc(strlen("cmd.exe /c ") + strlen(program) + strlen(args) + 1); //space between %s
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
    si.dwFlags = STARTF_USESTDHANDLES;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.lpSecurityDescriptor = NULL;

    // ensure that the child processes can inherit our handles!
    sa.bInheritHandle = TRUE;

    // Create a pipe object and share the handle with a child processes 
    if(!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)){
        printf("[!] FAILED TO CREATE PIPE!");
        return 0;
    }
    //set handle info
    if(!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)){
        printf("ERROR: Could not set handle info");
        return 0;
    }

    // set startupinfo handles
    si.hStdInput = NULL;
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;
    
    //Create the child Processes and wait for it to terminate!
    if(!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)){
        printf("[!] FAILED TO START PROCESS!");
        return 0;
    }

    DWORD lpNumberOfBytesRead = 0;
    char* lpBuffer = (char*)malloc(BUF_SIZE);
    // Finally, print the contents from the pipe!
    while(WaitForSingleObject(pi.hProcess, 0)){
        while(PeekNamedPipe(hStdOutRead, lpBuffer, BUF_SIZE - 1, &lpNumberOfBytesRead, NULL, NULL)){
            if(lpNumberOfBytesRead == 0){
                break;
            }
            if(ReadFile(hStdOutRead, lpBuffer, lpNumberOfBytesRead, NULL, NULL)){
                lpBuffer[lpNumberOfBytesRead] = '\0';
                printf("%s", lpBuffer);
            }else{
                printf("[!] ERROR: ERROR WITH READFILE: %d", GetLastError());
                break;
            }
        }
    }
    //perform any cleanup necessary!
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    free(cmd);
    free(lpBuffer);
    CloseHandle(hStdOutRead);
    CloseHandle(hStdOutWrite); 
    return 0;
}