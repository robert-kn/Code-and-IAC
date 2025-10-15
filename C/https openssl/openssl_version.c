#include <openssl/ssl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    printf("openssl version: %s\n", OpenSSL_version(SSLEAY_VERSION));
    return EXIT_SUCCESS;
}