/* The Windows networking API is called Winsock */
/**
 *  The _WIN32_WINNT macro must be defined first so that the proper version of the Windows headers are included. 
 *  winsock2.h, iphlpapi.h, and ws2tcpip.h are the Windows headers we need in order to list network adapters.
 *  We need stdio.h for the printf() function and stdlib.h for memory allocation.
 * 
 *  Next, we include the following pragmas to tell Microsoft Visual C which libraries must be linked with the 
 *  executable.
 * 
 *  If you're compiling with MinGW, these lines will have no effect. You will need to link to these libraries 
 *  explicitly on the command line, for example, gcc win_list.c -o win_list.exe -liphlpapi -lws2_32.
 * 
 *  We then enter the main() function (Whenever we are using Winsock, the first thing we must do is initialize it. 
 * This is done with a call to WSAStartup()) and initialize Winsock 2.2 using the WSAStartup() function which is 
 * called with the requested version, Winsock 2.2 and a WSADATA structure. We check its return value to detect any 
 * errors. The WSAStartup() function returns 0 upon success, and non-zero upon failure.
 * 
 *  The WSADATA structure will be filled in by WSAStartup() with details about the Windows Sockets implementation.
 * 
 *  Next, we allocate memory for the adapters, and we request the adapters' addresses from Windows using the 
 *  GetAdapterAddresses() function.
 * 
 *  The asize variable will store the size of our adapters' address buffer. To begin with, we set it to 20000 and 
 *  allocate 20,000 bytes to adapters using the malloc() function. The malloc() function will return 0 on failure, 
 *  so we test for that and display an error message if allocation failed.
 * 
 *  Next, we call GetAdapterAddresses(). The first parameter, AF_UNSPEC, tells Windows that we want both IPv4 and 
 *  IPv6 addresses. You can pass in AF_INET or AF_INET6 to request only IPv4 or only IPv6 addresses. The second 
 *  parameter, GAA_FLAG_INCLUDE_PREFIX, is required to request a list of addresses. The next parameter is reserved 
 *  and should be passed in as 0 or NULL. Finally, we pass in our buffer, adapters, and a pointer to its size, asize.
 * 
 *  If our buffer is not big enough to store all of the addresses, then GetAdapterAddresses() returns 
 *  ERROR_BUFFER_OVERFLOW and sets asize to the required buffer size. In this case, we free our adapters buffer and 
 *  try the call again with a larger buffer.
 * 
 *  On success, GetAdapterAddresses() returns ERROR_SUCCESS, in which case, we break from the loop and continue. Any
 *  other return value is an error.
 * 
 *  When GetAdapterAddresses() returns successfully, it will have written a linked list into adapters with each 
 *  adapter's address information. Our next step is to loop through this linked list and print information for each 
 *  adapter and address
 * 
 *  We first define a new variable, adapter, which we use to walk through the linked list of adapters. The first 
 *  adapter is at the beginning of adapters, so we initially set adapter to adapters. At the end of each loop, we 
 *  set adapter = adapter->Next; to get the next adapter. The loop aborts when adapter is 0, which means we've 
 *  reached the end of the list.
 * 
 *  We get the adapter name from adapter->FriendlyName, which we then print using printf().
 * 
 *  The first address for each adapter is in adapter->FirstUnicastAddress. We define a second pointer, address, and 
 *  set it to this address. Addresses are also stored as a linked list, so we begin an inner loop that walks through
 *  the addresses.
 * 
 *  The address->Address.lpSockaddr->sa_family variable stores the address family type. If it is set to AF_INET, then
 *  we know this is an IPv4 address. Otherwise, we assume it is an IPv6 address (in which case the family is 
 *  AF_INET6).
 * 
 *  Next, we allocate a buffer, ap, to store the text representation of the address. The getnameinfo() function is 
 *  called to convert the address into a standard notation address. 
 * 
 *  Finally, we can print the address from our buffer, ap, using printf().
 * 
 *  We finish the program by freeing the allocated memory and calling WSACleanup().
 */

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

/**
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

    DWORD asize = 20000;
    PIP_ADAPTER_ADDRESSES adapters; /* check what type PIP_ADAPTER_ADDRESSES returns; an int or a pointer? */

    do
    {
        adapters = (PIP_ADAPTER_ADDRESSES) malloc(asize);

        if(adapters == NULL)
        {
            printf("Couldn't allocate %ld for adapters.\n", asize);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        int r = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);

        if (r == ERROR_BUFFER_OVERFLOW)
        {
            printf("GetAdaptersAddresses wants %ld bytes.\n", asize);
            free(adapters);
        }
        else if(r == ERROR_SUCCESS)
        {
            break;
        }
        else
        {
            printf("Error from GetAdaptersAddresses: %d\n", r);
            free(adapters);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

    } while(!adapters); /* could also be written as adapters == NULL */

    PIP_ADAPTER_ADDRESSES adapter = adapters;

    while(adapter)
    {
        printf("\nAdapter name: %S\n", adapter->FriendlyName);

        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;

        while(address)
        {
            printf("\t%s", address->Address.lpSockaddr->sa_family == AF_INET ? "IPv4" : "IPv6");

            char ap[100];

            getnameinfo(address->Address.lpSockaddr, address->Address.iSockaddrLength, ap, sizeof(ap), 0, 0,NI_NUMERICHOST);

            printf("\t%s\n", ap);

            address = address->Next;
        }

        adapter = adapter->Next;
    }

    free(adapters);
    WSACleanup(); /* When a Winsock program is finished, it should call WSACleanup(). */
    printf("ok.\n");
    exit(EXIT_SUCCESS);
}



