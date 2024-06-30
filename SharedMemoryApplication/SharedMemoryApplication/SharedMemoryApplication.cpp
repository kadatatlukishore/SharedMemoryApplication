// SharedMemoryApplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <iostream>
#include <string>

#define MAX_BUFFER_SIZE     512
#define SHARED_MEMORY_NAME  L"Local\\MyFileMappingObject"

int main() {
    // Create a file mapping object
    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,
                                        NULL,
                                        PAGE_READWRITE,
                                        0,
                                        MAX_BUFFER_SIZE,
                                        SHARED_MEMORY_NAME);

    if (hMapFile == NULL) {
        std::cerr << "Could not create file mapping object." << std::endl;
        return 1;
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

    std::string input;
    while (true) {
        std::cout << "Enter message to send (type 'exit' to quit): ";
        std::getline(std::cin, input);
        // Write the message to the shared memory
        memcpy(pBuf, input.c_str(), input.size() + 1);

        if (input == "exit") {
            break;
        }
    }

    // Unmap the view and close handles
    UnmapViewOfFile(pBuf);
    CloseHandle(hMapFile);

    return 0;
}