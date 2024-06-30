// Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <iostream>
#include <string>

#define MAX_BUFFER_SIZE     512
#define SHARED_MEMORY_NAME  L"Local\\MyFileMappingObject"

int main()
{
    std::cout << "Client Program to access shared Memory" << std::endl;

    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,
                                    FALSE,
                                    SHARED_MEMORY_NAME);

    if (hMapFile == NULL) {
        std::cerr << "Couldn't open the file mapping obj" << std::endl;
        return -1;
    }

    // Map a view of the file into the address space of the calling process
    LPVOID pBuf = MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        MAX_BUFFER_SIZE
    );

    if (pBuf == NULL) {
        std::cerr << "Could not map view of file." << std::endl;
        CloseHandle(hMapFile);
        return 1;
    }

    char buffer[256];
    std::string lastMessage;

    while (true) {
        // Read the message from the shared memory
        strcpy(buffer, (char*)pBuf);

        if (lastMessage != buffer) {
            std::cout << "Message read from shared memory: " << buffer << std::endl;
            lastMessage = buffer;
        }

        // Break if 'exit' is read from shared memory
        if (lastMessage == "exit") {
            break;
        }

        Sleep(500);  // Wait for half a second before checking again
    }
   

    // Unmap the view and close handle
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}
