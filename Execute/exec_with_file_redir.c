#include "printfile.h"

int main(int argc, char* argv[]){

    if (argc != 4){
        printf("Usage: %s program.exe \"args and args \" outfile.txt\n", argv[0]);
        return 0;
    }
    char* program = argv[1];
    char* args = argv[2];
    char* outfile = argv[3];

    //Make cmd line from program, args, and outfile
    char* cmd = (char *)malloc(
        strlen("cmd.exe /c") +
        strlen(program) + 
        strlen(args) + 
        strlen(">") + 
        strlen(outfile) + 
        4 //4 spaces
        );
    sprintf(cmd, "cmd.exe /c %s %s > %s", program, args, outfile);

    if(cmd == NULL){
        printf("[!] MALLOC FAILED!");
    }

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
    BOOL bInheritHandles = FALSE; 
    DWORD dwCreationFlags = 0;
    LPVOID lpEnvironment = NULL;
    LPCSTR lpCurrentDirectory = NULL;
    LPSTARTUPINFOA lpStartupInfo = &si;
    LPPROCESS_INFORMATION lpProcessInformation = &pi;
    if(!CreateProcessA(
        lpApplicationName, 
        lpCommandLine, 
        lpProcessAttributes, 
        lpThreadAttributes, 
        bInheritHandles, 
        dwCreationFlags, 
        lpEnvironment, 
        lpCurrentDirectory, 
        lpStartupInfo, 
        lpProcessInformation)){
            printf("[!] FAILED TO CREATE PROCESS!");
        };

    //Wait for processes to exit 
    DWORD dwMilliseconds = INFINITE;
    WaitForSingleObject(pi.hProcess, dwMilliseconds);

    if(!PrintFileContents(outfile)){
        printf("[!] FAILED TO PRINT FILE CONTENTS!");
    }

    //Cleanup
    free(cmd);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}