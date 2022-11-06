#include "printfile.h"

int main(int argc, char* argv[]){

    if (argc != 4){
        printf("Usage: %s program.exe \"args and args \" outfile.txt\n", argv[0]);
        return 0;
    }
    char* program = argv[1];
    char* args = argv[2];
    char* outfile = argv[3];

    // TODO: Make cmd line from program, args, and outfile
    char* cmd = (char *) malloc(
        strlen("cmd.exe /c ") +
        strlen(program) + 
        strlen(args) + strlen(">") + 
        strlen(outfile) + 
        4 //4 spaces
        );
    sprintf(cmd, "C:\\Windows\\system32\\cmd.exe /c %s %s > %s", program, args, outfile);
    printf(cmd);
    // Values needed for CreateProcessA
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    // Dead squirrels
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    // Dead squirrels 
    ZeroMemory(&pi, sizeof(pi));

    //create the process
    LPCSTR lpApplicationName = NULL;
    LPSTR lpCommandLine = cmd;
    LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL;
    LPSECURITY_ATTRIBUTES lpThreadAttributes = NULL;
    BOOL bInheritHandles = TRUE; 
    DWORD dwCreationFlags = 0;
    LPVOID lpEnvironment = NULL;
    LPCSTR lpCurrentDirectory = NULL;
    LPSTARTUPINFOA lpStartupInfo = &si;
    LPPROCESS_INFORMATION lpProcessInformation = &pi;
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
    // TODO: Wait for processes to exit 
    DWORD dwMilliseconds = INFINITE;
    WaitForSingleObject(lpProcessInformation->hProcess, dwMilliseconds);

    // TODO: Cleanup
    free(cmd);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}