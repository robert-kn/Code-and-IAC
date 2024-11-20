#include <stdio.h>
#include <time.h>

int main()
{
    time_t timer;
    time(&timer); /* getting the time with the built-in C time() function */
    printf("Local time is: %s\n", ctime(&timer)); /* converts it into a string with the ctime() function. */
    return 0;
}