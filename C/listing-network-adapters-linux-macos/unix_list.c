/* Listing local network addresses is somewhat easier on a Unix-based system, compared to Windows. */

/**
 *  We declare a variable, addresses, which stores the addresses. A call to the getifaddrs() function allocates 
 *  memory and fills in a linked list of addresses. This function returns 0 on success or -1 on failure.
 * 
 *  Next, we use a new pointer, address, to walk through the linked list of addresses. After considering each 
 *  address, we set address = address->ifa_next to get the next address structure. We stop the loop when 
 *  address == 0, which happens at the end of the linked list
 * 
 *  For each address, we identify the address family. We are interested in AF_INET (IPv4 addresses) and AF_INET6 
 *  (IPv6 addresses). The getifaddrs() function can return other types, so we skip those (what other types are there?).
 * 
 *  For each address, we then continue to print its adapter name and its address type, IPv4 or IPv6.
 * 
 *  We then define a buffer, ap, to store the textual address. A call to the getnameinfo() function fills in this 
 *  buffer, which we can then print.
 */

#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    struct ifaddrs * addresses;

    if(getifaddrs(&addresses) == -1)
    {
        printf("getifaddrs call failed\n");
        exit(EXIT_FAILURE);
    }

    struct ifaddrs *address = addresses;

    while(address)
    {
        int family = address->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6)
        {
            printf("interface name\t%s\t", address->ifa_name);
            printf("interface family\t%s\t", family == AF_INET ? "IPv4" : "IPv6");

            char ap[100];

            const int family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);

            getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);

            printf("\t%s\n", ap);
        }
        address = address->ifa_next;
    }

    freeifaddrs(addresses);
    exit(EXIT_SUCCESS);
}