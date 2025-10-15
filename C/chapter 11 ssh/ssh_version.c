#include "chap11.h"
#include <stdlib.h>

int main(void)
{
    printf("libssh version: %s\n", ssh_version(0));
    return EXIT_SUCCESS;
}