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
    char* cmd = (char *) malloc(strlen(program) + strlen(args) + strlen(">>") + strlen(outfile));
    sprintf(cmd, "%s %s >> %s", program, args, outfile);
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
    DWORD dwCreationFlags = CREATE_NO_WINDOW;
    LPVOID lpEnvironment = NULL;
    LPCSTR lpCurrentDirectory = NULL;
    STARTUPINFOA si;
    GetStartupInfoA(&si);
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
    CloseHandle(pi.hProcess);
    
    PrintFileContents(outfile);
    return 0;
}