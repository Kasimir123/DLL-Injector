#include "../CPPToolLib/CPP Tools/stdafx/stdafx.h"

#include "../CPPToolLib/CPP Tools/proc/proc.h"
#include "../CPPToolLib/CPP Tools/injector/injector.h"

#include <codecvt>


/// <summary>
/// Injects the dll into the specified process
/// </summary>
/// <param name="name">name of process to inject into</param>
/// <param name="path">path to dll to inject</param>
/// <returns>returns whether it was successful or not</returns>
int inject(const std::wstring name, const char* path)
{
    // Gets process id, if process contains the name that was passed
    DWORD procId = proc::GetProcIdByContains(name.c_str());

    // if process id was not found then exit
    if (!procId) return 1;

    // opens a handle to the process
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    // Injects the dll to the process
    injector::LocAInject(path, hProcess);

    // exit
    return 0;
}

/// <summary>
/// Manual Mode, this will open a console
/// </summary>
void manualMode()
{
    // DLL Path
    std::string dllPath;

    // Process name
    std::wstring process;

    // Gets dll path
    std::cout << "Path to DLL: ";
    std::cin >> dllPath;

    // Loop until user wants to quit
    while (true)
    {
        // Get process name
        std::cout << "Process: ";
        std::wcin >> process;

        // If exit was passed then exit
        if (!std::wcscmp(process.c_str(), L"Exit")) break;

        // If proccess was injected print success, else print could not find process and retry
        if (!inject(process.c_str(), dllPath.c_str())) std::cout << "Success" << std::endl;
        else std::cout << "Could not find Process" << std::endl;
    }

    // Exit program
    std::cout << "Exiting" << std::endl;
}


int main(int argc, char *argv[])
{
    // Used to convert char* to wstring
    std::wstring_convert< std::codecvt<wchar_t, char, std::mbstate_t> > converter;

    // If no extra arguments assume exe was clicked on and go to manual mode
    if (argc == 1) manualMode();
    // else check passed arguments
    else {

        // If 2 arguments were not passed then exit
        if (argc != 3) return 1;

        // Inject with first argument as the process and second argument as the dll path
        return inject(converter.from_bytes(argv[1]), argv[2]);
    }

    // exit with success code
    return 0;

}
