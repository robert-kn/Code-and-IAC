/**
 *  Detects if the compiler is running on Windows or not and includes the proper headers for the platform it is 
 *  running on.
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


/* Define some macros, which abstract out some of the difference between the Berkeley socket and Winsock APIs */

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET) 
#define CLOSESOCKET(s) closesocket(s) 
#define GETSOCKETERRNO() (WSAGetLastError())

#if !defined(IPV6_V6ONLY)
#define IPV6_V6ONLY 27
#endif

#else
#define ISVALIDSOCKET(s) ((s) >= 0) 
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno) 
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
 * The first thing the main() function will do is initialize Winsock if we are compiling on Windows
 */

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

/** We must now figure out the local address that our web server should bind to:
 * We use getaddrinfo() to fill in a struct addrinfo structure with the needed information. getaddrinfo() takes a 
 * hints parameter, which tells it what we're looking for. In this case, we've zeroed out hints using memset() first. 
 * Then, we set ai_family = AF_INET. AF_INET specifies that we are looking for an IPv4 address. We could use AF_INET6
 * to make our web server listen on an IPv6 address instead.
 * 
 * Next, we set ai_socktype = SOCK_STREAM. This indicates that we're going to be using TCP. SOCK_DGRAM would be used 
 * if we were doing a UDP server instead.
 * 
 * Finally, ai_flags = AI_PASSIVE is set. This is telling getaddrinfo() that we want it to bind to the wildcard 
 * address. That is, we are asking getaddrinfo() to set up the address, so we listen on any available network 
 * interface.
 * 
 * Once hints is set up properly, we declare a pointer to a struct addrinfo structure, which holds the return information 
 * from getaddrinfo(). We then call the getaddrinfo() function. The getaddrinfo() function has many uses, but for our 
 * purpose, it generates an address that's suitable for bind(). To make it generate this, we must pass in the first  
 * parameter as NULL and have the AI_PASSIVE flag set in hints.ai_flags.
 * 
 * The second parameter to getaddrinfo() is the port we listen for connections on. A standard HTTP server would use 
 * port 80. However, only privileged users on Unix-like operating systems can bind to ports 0 through 1023. The 
 * choice of port number here is arbitrary, but we use 8080 to avoid issues. If you are running with superuser 
 * privileges, feel free to change the port number to 80 if you like. Keep in mind that only one program can bind to 
 * a particular port at a time. If you try to use a port that is already in use, then the call to bind() fails. In 
 * this case, just change the port number to something else and try again.
 * 
 * It is common to see programs that don't use getaddrinfo() here. Instead, they fill in a struct addrinfo structure 
 * directly. The advantage to using getaddrinfo() is that it is protocol-independent. Using getaddrinfo() makes it 
 * very easy to convert our program from IPv4 to IPv6. In fact, we only need to change AF_INET to AF_INET6, and our 
 * program will work on IPv6. If we filled in the struct addrinfo structure directly, we would need to make many 
 * tedious changes to convert our program into IPv6.
 */
    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    struct addrinfo *bind_address;
    getaddrinfo(0, "8080", &hints, &bind_address);

/**
 * Now that we've figured out our local address info, we can create the socket. Here, we define socket_listen as a 
 * SOCKET type. Recall that SOCKET is a Winsock type on Windows, and that we have a macro defining it as int on other
 * platforms. We call the socket() function to generate the actual socket. socket() takes three parameters: the 
 * socket family, the socket type, and the socket protocol. The reason we used getaddrinfo() before calling socket() 
 * is that we can now pass in parts of bind_address as the arguments to socket(). Again, this makes it very easy to 
 * change our program's protocol without needing a major rewrite.
 * 
 * It is common to see programs written so that they call socket() first. The problem with this is that it makes the 
 * program more complicated as the socket family, type, and protocol must be entered multiple times. Structuring our 
 * program as we have here is better.
 */
    printf("Creating socket...\n");
    SOCKET socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

/* We should check that the call to socket() was successful. We can check that socket_listen is valid using the 
ISVALIDSOCKET() macro we defined earlier. If the socket is not valid, we print an error message. Our GETSOCKETERRNO() 
macro is used to retrieve the error number in a cross-platform way. */

    if (!ISVALIDSOCKET(socket_listen))
    {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
        exit(EXIT_FAILURE);
    }

    int option = 0;

    if (setsockopt(socket_listen, IPPROTO_IPV6, IPV6_V6ONLY, (void*)&option, sizeof(option)))
    {
        fprintf(stderr, "setsockopt() failed. (%d)\n", GETSOCKETERRNO());
        return 1;
    }

/**
 * After the socket has been created successfully, we can call bind() to associate it with our address from 
 * getaddrinfo(). bind() returns 0 on success and non-zero on failure. If it fails, we print the error number much 
 * like we did for the error handling on socket(). bind() fails if the port we are binding to is already in use. In 
 * that case, either close the program using that port or change your program to use a different port.
 * 
 * After we have bound to bind_address, we can call the freeaddrinfo() function to release the address memory.
 */
    printf("Binding socket to local address...\n");
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        fprintf(stderr, "bind() failed. (%d)\n", GETSOCKETERRNO());
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(bind_address);

/**
 * Once the socket has been created and bound to a local address, we can cause it to start listening for connections 
 * with the listen() function. The second argument to listen(), which is 10 in this case, tells listen() how many 
 * connections it is allowed to queue up. If many clients are connecting to our server all at once, and we aren't 
 * dealing with them fast enough, then the operating system begins to queue up these incoming connections. If 10 
 * connections become queued up, then the operating system will reject new connections until we remove one from the 
 * existing queue.
 * 
 * Error handling for listen() is done the same way as we did for bind() and socket().
 */
    printf("Listening...\n");
    if (listen(socket_listen, 10) < 0)
    {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
        exit(EXIT_FAILURE);
    }

/**
 * After the socket has begun listening for connections, we can accept any incoming connection with the accept() 
 * function. accept() has a few functions. First, when it's called, it will block your program until a new connection
 * is made. In other words, your program will sleep until a connection is made to the listening socket. When the new 
 * connection is made, accept() will create a new socket for it. Your original socket continues to listen for new 
 * connections, but the new socket returned by accept() can be used to send and receive data over the newly
 * established connection. accept() also fills in address info of the client that connected.
 * 
 * Before calling accept(), we must declare a new struct sockaddr_storage variable to store the address info for the 
 * connecting client. The struct sockaddr_storage type is guaranteed to be large enough to hold the largest supported 
 * address on your system. We must also tell accept() the size of the address buffer we're passing in. When accept() 
 * returns, it will have filled in client_address with the connected client's address and client_len with the length 
 * of that address. client_len differs, depending on whether the connection is using IPv4 or IPv6.
 * 
 * We store the return value of accept() in socket_client. We check for errors by detecting if client_socket is a 
 * valid socket. This is done in exactly the same way as we did for socket().
 */
    printf("Waiting for connection...\n");
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    SOCKET socket_client = accept(socket_listen, (struct sockaddr*) &client_address, &client_len);
    if (!ISVALIDSOCKET(socket_client))
    {
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        exit(EXIT_FAILURE);
    }

/**
 * At this point, a TCP connection has been established to a remote client. We can print the client's address to the 
 * console. This step is completely optional, but it is good practice to log network connections somewhere. 
 * 
 * getnameinfo() takes the client's address and address length. The address length is needed because getnameinfo() 
 * can work with both IPv4 and IPv6 addresses. We then pass in an output buffer and buffer length. This is the buffer
 * that getnameinfo() writes its hostname output to. The next two arguments specify a second buffer and its length. 
 * getnameinfo() outputs the service name to this buffer. We don't care about that, so we've passed in 0 for those 
 * two parameters. Finally, we pass in the NI_NUMERICHOST flag, which specifies that we want to see the hostname as 
 * an IP address.
 */

    printf("Client is connected... ");
    char address_buffer[100];
    getnameinfo((struct sockaddr*)&client_address, client_len, address_buffer, sizeof(address_buffer), 0, 0, NI_NUMERICHOST);
    printf("%s\n", address_buffer);

/**
 * As we are programming a web server, we expect the client (for example, a web browser) to send us an HTTP request.
 * We read this request using the recv() function.
 * 
 * We define a request buffer, so that we can store the browser's HTTP request. In this case, we allocate 1,024 bytes
 * to it, which should be enough for this application. recv() is then called with the client's socket, the request 
 * buffer, and the request buffer size. recv() returns the number of bytes that are received. If nothing has been 
 * received yet, recv() blocks until it has something. If the connection is terminated by the client, recv() returns 
 * 0 or -1, depending on the circumstance. We are ignoring that case here for simplicity, but you should always check 
 * that recv() > 0 in production. The last parameter to recv() is for flags. Since we are not doing anything special, 
 * we simply pass in 0.
 * 
 * The request received from our client should follow the proper HTTP protocol. A real web server would need to parse 
 * the request and look at which resource the web browser is requesting. Our web server only has one function—to tell 
 * us what time it is. So, for now, we just ignore the request altogether.
 */

    printf("Reading request...\n");
    char request[1024];
    int bytes_received = recv(socket_client, request, 1024, 0);
    printf("Received %d bytes.\n", bytes_received);

/**
 * If you want to print the browser's request to the console, you can do it as shown below.
 * 
 * Note that we use the printf() format string, "%.*s". This tells printf() that we want to print a specific number 
 * of characters — bytes_received. It is a common mistake to try printing data that's received from recv() directly as 
 * a C string. There is no guarantee that the data received from recv() is null terminated! If you try to print it 
 * with printf(request) or printf("%s", request), you will likely receive a segmentation fault error (or at best it 
 * will print some garbage).
 */

    printf("%.*s", bytes_received, request);

/**
 * Now that the web browser has sent its request, we can send our response back.
 * 
 * To begin with, we set char *response to a standard HTTP response header and the beginning of our message (Local 
 * time is:). This response tells the browser three things — your request is OK; the server will close the connection 
 * when all the data is sent and the data you receive will be plain text.
 * 
 * The HTTP response header ends with a blank line. HTTP requires line endings to take the form of a carriage return 
 * character, followed by a newline character. So, a blank line in our response is \r\n. The part of the string that 
 * comes after the blank line, Local time is:, is treated by the browsers as plain text.
 * 
 * We send the data to the client using the send() function. This function takes the client's socket, a pointer to the
 * data to be sent, and the length of the data to send. The last parameter to send() is flags. We don't need to do 
 * anything special, so we pass in 0.
 * 
 * send() returns the number of bytes sent. You should generally check that the number of bytes sent was as expected, 
 * and you should attempt to send the rest if it's not. We are ignoring that detail here for simplicity. (Also, we are
 * only attempting to send a few bytes; if send() can't handle that, then something is probably very broken, and 
 * resending won't help.)
 */
    printf("Sending response...\n");
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain\r\n\r\n"
        "Local time is: ";
    int bytes_sent = send(socket_client, response, strlen(response), 0);     
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(response));

/**
 * After the HTTP header and the beginning of our message is sent, we can send the actual time. 
 */

    time_t timer;
    time(&timer);
    char *time_msg = ctime(&timer);
    bytes_sent = send(socket_client, time_msg, strlen(time_msg), 0);     
    printf("Sent %d of %d bytes.\n", bytes_sent, (int)strlen(time_msg));

/**
 * We must then close the client connection to indicate to the browser that we've sent all of our data.
 * 
 * If we don't close the connection, the browser will just wait for more data until it times out.
 */

    printf("Closing connection...\n"); 
    CLOSESOCKET(socket_client);

/**
 * At this point, we could call accept() on socket_listen to accept additional connections. That is exactly what a 
 * real server would do. However, as this is just a quick example program, we will instead close the listening socket
 * too and terminate the program.
 */

    printf("Closing listening socket...\n");
    CLOSESOCKET(socket_listen);

#if defined(_WIN32)
    WSACleanup();
#endif

    printf("Finished.\n");

    exit(EXIT_SUCCESS);

/**
 * 
 * That's the complete program. After you compile and run it, you can navigate a web browser to it, and it'll display
 * the current time. When you run the program, it waits for a connection. You can open a web browser and navigate to 
 * http://127.0.0.1:8080 to load the web page. Recall that 127.0.0.1 is the IPv4 loopback address, which connects to 
 * the same machine it's running on. The :8080 part of the URL specifies the port number to connect to. If it were 
 * left out, your browser would default to port 80, which is the standard for HTTP connections.
 */
}