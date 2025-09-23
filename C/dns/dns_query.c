#include "chap05.h"

/** msg points to a message's beginning, p is a pointer to the name to print, and end is a pointer to one past the end of message */
const unsigned char *print_name(const unsigned char *msg, const unsigned char *p, const unsigned char *end) {

    if (p + 2 > end) {
        fprintf(stderr, "End of message.\n"); 
        exit(1);
    }

    /** does P point to a name pointer? if so recursively call print_name with the new pointer to the name to print from offset message */
    if ((*p & 0xC0) == 0xC0) {
        const int k = ((*p & 0x3F) << 8) + p[1];
        p += 2;
        printf(" (pointer %d) ", k);
        print_name(msg, msg+k, end);
        return p;

    }
    /** if the name in the answer is not a pointer, we print one label at a time  */ 
    else 
    {
        /** store the length of the current label */
        const int len = *p++;
        /** if adding len + 1 to the pointer moves it beyond end of message then we stop */
        if (p + len + 1 > end) {
            fprintf(stderr, "End of message.\n"); 
            exit(1);
        }
        /** otherwise print the label */
        printf("%.*s", len, p);
        p += len;
        if (*p) {
            printf(".");
            return print_name(msg, p, end);
        } else {
            return p+1;
        }
    }
}

void print_dns_message(const char *message, int msg_length) {
    /* is the message valid? remember that the dns header is 12 bytes long. if a dns message is < 12 bytes then 
    it can safely be ignored. this ensures that we can atleast read the header */
    if (msg_length < 12) {
        fprintf(stderr, "Message is too short to be valid.\n");
        exit(1);
    }
    /* copying the message to apointer of type unsigned char * makes certain calculations easier to work with */
    const unsigned char *msg = (const unsigned char *)message;

    /* if you want to print out the entire raw dns message then uncomment the below block */
    /*
    int i;
    for (i = 0; i < msg_length; ++i) {
        unsigned char r = msg[i];
        printf("%02d:   %02X  %03d  '%c'\n", i, r, r, r);
    }
    printf("\n");
    */

    /* print out the dns message id which is the first 2 bytes */
    printf("ID = %0X %0X\n", msg[0], msg[1]);

    /* retrieve the QR bit which indicates whether this is dns question sent by a client or this is a dns answer sent by a dns resolver */
    const int qr = (msg[2] & 0x80) >> 7;
    printf("QR = %d %s\n", qr, qr ? "response" : "query");

    /* retrieve the opcode where 0 indicates a standard query, 1 indicates a reverse dns query, 3 is server status check */
    const int opcode = (msg[2] & 0x78) >> 3;
    printf("OPCODE = %d ", opcode);
    switch(opcode) {
        case 0: printf("standard\n"); break;
        case 1: printf("reverse\n"); break;
        case 2: printf("status\n"); break;
        default: printf("?\n"); break;
    }

    const int aa = (msg[2] & 0x04) >> 2;
    printf("AA = %d %s\n", aa, aa ? "authoritative" : "");

    const int tc = (msg[2] & 0x02) >> 1;
    printf("TC = %d %s\n", tc, tc ? "message truncated" : "");

    const int rd = (msg[2] & 0x01);
    printf("RD = %d %s\n", rd, rd ? "recursion desired" : "");

    /* RCODE is only set in a dns response where qr bit is set to 1 */
    if (qr) 
    {
        const int rcode = msg[3] & 0x0F;
        printf("RCODE = %d ", rcode);
        switch(rcode) {
            case 0: printf("success\n"); break;
            case 1: printf("format error\n"); break;
            case 2: printf("server failure\n"); break;
            case 3: printf("name error\n"); break;
            case 4: printf("not implemented\n"); break;
            case 5: printf("refused\n"); break;
            default: printf("?\n"); break;
        }
        /* we break out of the function because the dns response is not valid */
        if (rcode != 0) 
            return;
    }

    const int qdcount = (msg[4] << 8) + msg[5];
    const int ancount = (msg[6] << 8) + msg[7];
    const int nscount = (msg[8] << 8) + msg[9];
    const int arcount = (msg[10] << 8) + msg[11];

    printf("QDCOUNT = %d\n", qdcount); /* question count: indicates number of questions in a DNS query; 16 bit value despite the fact that we are only allowed one question */
    printf("ANCOUNT = %d\n", ancount); /* answer count: can have multiple answers to a dns question/query */
    printf("NSCOUNT = %d\n", nscount); /* name server count */
    printf("ARCOUNT = %d\n", arcount); /* additional count */

    /* advance past the dns message header to the question part */
    const unsigned char *p = msg + 12;

    /* set the end variable to one past the end of the message to help us detect whether we're about to read past the end of the message which is to be avoided */
    const unsigned char *end = msg + msg_length;

    /* if there was a question to the name server then we want to print it. no real-world DNS server will accept a message with multiple questions
    however, DNS RFC does clearly define the format to encode multiple questions. For that reason, we make our code loop through each question using a for loop */
    if (qdcount) {
        int i;
        for (i = 0; i < qdcount; ++i) {
            if (p >= end) {
                fprintf(stderr, "End of message.\n"); 
                exit(1);
            }

            printf("Query %2d\n", i + 1);
            printf("  name: ");

            p = print_name(msg, p, end); 
            printf("\n");

            if (p + 4 > end) {
                fprintf(stderr, "End of message.\n"); 
                exit(1);
            }

            const int type = (p[0] << 8) + p[1];
            printf("  type: %d\n", type);
            p += 2;

            const int qclass = (p[0] << 8) + p[1];
            printf(" class: %d\n", qclass);
            p += 2;
        }
    }
    /* marks the end of printing the fields in the question part of the dns message */


    /* in this section we move to print the answer, authority, and additional sections of the message */
    if (ancount || nscount || arcount) {
        int i;
        for (i = 0; i < ancount + nscount + arcount; ++i) {
            if (p >= end) {
                fprintf(stderr, "End of message.\n"); 
                exit(1);
            }

            printf("Answer %2d\n", i + 1);
            printf("  name: ");

            /* printing out the same name we printed in the question part of the dns message */
            p = print_name(msg, p, end); 
            printf("\n");

            /* at this point print_name has advanced the pointer to the first byte of the type field in the answer part of the dns message */
            if (p + 10 > end) {
                fprintf(stderr, "End of message.\n"); 
                exit(1);
            }

            const int type = (p[0] << 8) + p[1];
            printf("  type: %d\n", type);
            p += 2;

            const int qclass = (p[0] << 8) + p[1];
            printf(" class: %d\n", qclass);
            p += 2;

            /* why does he do this? i.e. why are 4 bytes read for the ttl field instead of 2 as indicated by the diagram */
            const unsigned int ttl = (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3];
            printf("   ttl: %u\n", ttl);
            p += 4;

            const int rdlen = (p[0] << 8) + p[1];
            printf(" rdlen: %d\n", rdlen);
            p += 2;

            if (p + rdlen > end) {
                fprintf(stderr, "End of message.\n"); 
                exit(1);
            }

            if (rdlen == 4 && type == 1) {
                /* A Record */
                printf("Address ");
                printf("%d.%d.%d.%d\n", p[0], p[1], p[2], p[3]);

            } else if (rdlen == 16 && type == 28) {
                /* AAAA Record */
                printf("Address ");
                int j;
                for (j = 0; j < rdlen; j+=2) {
                    printf("%02x%02x", p[j], p[j+1]);
                    if (j + 2 < rdlen) printf(":");
                }
                printf("\n");

            } else if (type == 15 && rdlen > 3) {
                /* MX Record */
                const int preference = (p[0] << 8) + p[1];
                printf("  pref: %d\n", preference);
                printf("MX: ");
                print_name(msg, p+2, end); 
                printf("\n");

            } else if (type == 16) {
                /* TXT Record */
                printf("TXT: '%.*s'\n", rdlen-1, p+1);

            } else if (type == 5) {
                /* CNAME Record */
                printf("CNAME: ");
                print_name(msg, p, end); 
                printf("\n");
            }

            p += rdlen;
        }
    }

    if (p != end) {
        printf("There is some unread data left over.\n");
    }

    printf("\n");
}


/* main() function creates a DNS query, sends it to a DNS server, and awaits a response. */

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Usage:\n\tdns_query hostname type\n");
        printf("Example:\n\tdns_query example.com aaaa\n");
        exit(0);
    }

    if (strlen(argv[1]) > 253) {
        fprintf(stderr, "Hostname too long.");
        exit(1);
    }

    unsigned char type;
    if (strcmp(argv[2], "a") == 0) {
        type = 1;
    } else if (strcmp(argv[2], "mx") == 0) {
        type = 15;
    } else if (strcmp(argv[2], "txt") == 0) {
        type = 16;
    } else if (strcmp(argv[2], "aaaa") == 0) {
        type = 28;
    } else if (strcmp(argv[2], "any") == 0) {
        type = 255;
    } else {
        fprintf(stderr, "Unknown type '%s'. Use a, aaaa, txt, mx, or any.", argv[2]);
        exit(1);
    }

#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        fprintf(stderr, "Failed to initialize.\n");
        return 1;
    }
#endif

    printf("Configuring remote address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    struct addrinfo *peer_address;
    if (getaddrinfo("8.8.8.8", "53", &hints, &peer_address)) {
        fprintf(stderr, "getaddrinfo() failed. (%d)\n", GETSOCKETERRNO());
        return EXIT_FAILURE;
    }


    printf("Creating socket...\n");
    SOCKET socket_peer;
    socket_peer = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol);
    if (!ISVALIDSOCKET(socket_peer)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        return EXIT_FAILURE;
    }


    char query[1024] = {0xAB, 0xCD, /* ID */
                        0x01, 0x00, /* Set recursion */
                        0x00, 0x01, /* QDCOUNT */
                        0x00, 0x00, /* ANCOUNT */
                        0x00, 0x00, /* NSCOUNT */
                        0x00, 0x00  /* ARCOUNT */
                    };

    char *p = query + 12;
    char *h = argv[1]; /* grab the hostname passed in the command line when the program was invoked */

    /* We then need to encode the user's desired hostname into the query */
    while(*h) { /* effectively the same as *h != 0 */
        char *len = p;
        p++;
        if (h != argv[1]) 
            ++h;

        while(*h && *h != '.') 
            *p++ = *h++;
        *len = p - len - 1;
    }

    *p++ = 0;
    *p++ = 0x00; *p++ = type; /* QTYPE */
    *p++ = 0x00; *p++ = 0x01; /* QCLASS */

    /* calculate the query size by comparing p to the query beginning */
    const int query_size = p - query;

    /* Now, with the query message formed, and its length known, we can use sendto() to transmit the DNS query to the DNS server. */
    int bytes_sent = sendto(socket_peer, query, query_size, 0, peer_address->ai_addr, peer_address->ai_addrlen);
    printf("Sent %d bytes.\n", bytes_sent);

    /* For debugging purposes, we can also display the query we sent */
    print_dns_message(query, query_size);

    char read[1024];
    int bytes_received = recvfrom(socket_peer, read, 1024, 0, 0, 0);

    printf("Received %d bytes.\n", bytes_received);

    print_dns_message(read, bytes_received);
    printf("\n");


    freeaddrinfo(peer_address);
    CLOSESOCKET(socket_peer);

#if defined(_WIN32)
    WSACleanup();
#endif

    return EXIT_SUCCESS;
}