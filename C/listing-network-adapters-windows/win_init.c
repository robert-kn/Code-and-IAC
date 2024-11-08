/* The Windows networking API is called Winsock */

#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

/**
 * 
 * the pragma directive is used to request special behaviour from the compiler. the set of commands that appear in
 * the pragma directive is different for each compiler. If the directive is not supported, the compiler ignores it.
 * in this case, if the windows compiler supports the comment command (Microsoft Visual C), the pragma directive tells
 * the microsoft visual c compiler to link the executable with the winsock library ws2_32.lib.
 * 
 * If you are using MinGW as your compiler, the pragma is ignored. You need to explicitly tell the compiler to link 
 * in the library by adding the command-line option, -lws2_32. For example, you can compile this program using 
 * MinGW with the following command:
 * 
 * gcc win_init.c -o win_init.exe -lws2_32
 * */

int main()
{
    WSADATA d;

    if(WSAStartup(MAKEWORD(2, 2), &d))
    {
        printf("Failed to initialise");
        exit(EXIT_FAILURE);
    }

    WSACleanup();
    printf("ok. \n");
    exit(EXIT_SUCCESS);
}

/**
 * The WSAStartup() function is called with the requested version, Winsock 2.2 in this case, and a WSADATA structure.
 * The WSADATA structure will be filled in by WSAStartup() with details about the Windows Sockets implementation. 
 * The WSAStartup() function returns 0 upon success, and non-zero upon failure.
 */

