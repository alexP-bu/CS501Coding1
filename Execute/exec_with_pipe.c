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

    // ensure that the child processes can inherit our handles!
    sa.bInheritHandle = TRUE;

    // Create a pipe object and share the handle with a child processes 
    if(!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)){
        printf("[!] FAILED TO CREATE PIPE!");
        return 0;
    }
    //set handle info
    if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)){
      printf("ERROR: Could not set handle info");
    }

    // set startupinfo handles
    si.hStdInput = NULL;
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;
    
    //Create the child Processes and wait for it to terminate!
    if(!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)){
        printf("[!] FAILED TO START PROCESS!");
        return 0;
    }else{
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
    }

    char* lpBuffer = (char*)malloc(BUF_SIZE + 1);
    DWORD dwRead = 0;
    if((lpBuffer = NULL)){
        printf("[!] ERROR: MALLOC FAILED!");
    }
    // Finally, print the contents from the pipe!
    while(PeekNamedPipe(hStdOutRead, lpBuffer, BUF_SIZE - 1, &dwRead, NULL, NULL)){
        if(ReadFile(hStdOutRead, lpBuffer, dwRead, NULL, NULL)){
            lpBuffer[dwRead] = '\0';
            printf("%s", lpBuffer);
        }else{
            printf("[!] ERROR: ERROR WITH READFILE: %d", GetLastError());
            break;
        }
    }
    //perform any cleanup necessary!
    free(cmd);
    free(lpBuffer);
    CloseHandle(hStdOutRead);
    CloseHandle(hStdOutWrite); 
    
    return 0;
}