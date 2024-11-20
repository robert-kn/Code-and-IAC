/**
 * The first part includes winsock.h and ws2tcpip.h on Windows. _WIN32_WINNT must be defined for the Winsock headers 
 * to provide all the functions we need. We also include the #pragma comment(lib,"ws2_32.lib") pragma statement. This
 * tells the Microsoft Visual C compiler to link your program against the Winsock library, ws2_32.lib. If you're using
 * MinGW as your compiler, then #pragma is ignored. In this case, you need to tell the compiler to link in ws2_32.lib
 * on the command line using the -lws2_32 option.
 * 
 * If the program is not compiled on Windows, then the section after #else will compile. This section includes the 
 * various Berkeley socket API headers and other headers we need on these platforms.
 * 
 * In the main() function, we call WSAStartup() on Windows to initialize Winsock. The MAKEWORD macro allows us to 
 * request Winsock version 2.2. If our program is unable to initialize Winsock, it prints an error message and aborts.
 * 
 * When using Berkeley sockets, no special initialization is needed, and the socket API is always ready to use.
 * 
 * Before our program finishes, WSACleanup() is called if we're compiling for Winsock on Windows. This function allows
 * the Windows operating system to do additional cleanup.
 * 
 * Compiling and running this program on Linux or macOS is done with the following command:
 *  
 *      gcc sock_init.c -o sock_init 
 *      ./sock_init
 * 
 * Compiling on Windows using MinGW can be done with the following command:
 * 
 *      gcc sock_init.c -o sock_init.exe -lws2_32
 *      sock_init.exe
 * 
 *  Notice that the -lws2_32 flag is needed with MinGW to tell the compiler to link in the Winsock library, ws2_32.lib.
 */

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif

#include <stdio.h>
#include <stdlib.h>

int main()
{
#if defined(_WIN32)
    WSADATA d;
    if(WSAStartup(MAKEWORD(2, 2), &d))
    {
        fprintf(stderr, "Failed to initialise.\n");
        exit(EXIT_FAILURE);
    }
#endif
    printf("Ready to use socket API\n");

#if defined(_WIN32)
    WSACleanup();
#endif

    exit(EXIT_SUCCESS);
}