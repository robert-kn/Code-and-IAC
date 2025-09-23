#include "chap05.h"

#ifndef AI_ALL
#define AI_ALL 0x0100
#endif

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        printf("Usage:\tlookup hostname\n");
        printf("Example:\tlookup example.com\n");         
        exit(0);
    }

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {         
        fprintf(stderr, "Failed to initialize.\n");         
        return EXIT_FAILURE;
    } 
#endif

    printf("Resolving hostname '%s'\n", argv[1]);
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_ALL;
    struct addrinfo *peer_address;
    if (getaddrinfo(argv[1], 0, &hints, &peer_address)) {         
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());         
        return EXIT_FAILURE;
    }

    printf("Remote address is:\n");
    struct addrinfo *address = peer_address;
    do {
        char address_buffer[100];
        getnameinfo(address->ai_addr, address->ai_addrlen, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
        printf("\t%s\n", address_buffer);     
    } while ((address = address->ai_next));

    freeaddrinfo(peer_address);
#if defined(_WIN32)
    WSACleanup();
#endif
    return EXIT_SUCCESS;
}

