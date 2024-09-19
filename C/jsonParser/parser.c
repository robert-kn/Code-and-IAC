/**
 * This challenge is to build your own JSON parser.
 * 
 * JSON (which stands for JavaScript Object Notation) is a lightweight data-interchange format, which is widely 
 * used for transmitting data over the Internet. It is formally defined by the IETF here: 
 * https://tools.ietf.org/html/std90 or there’s a simpler graphical representation here: 
 * https://www.json.org/json-en.html
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE * open_file(char file_path[]);
int parser(FILE * fp);

#define BUF_SIZE 100

static char delimiters[] = "{}";
static char buffer[BUF_SIZE];

int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        printf("usage: [executable] [file]\n");
        exit(EXIT_FAILURE);
    }
    
    FILE * fp = open_file(argv[1]);
    parser(fp);

    fclose(fp);
    exit(EXIT_SUCCESS);
        
}

FILE * open_file(char file_path[])
{
    FILE * fp = fopen(file_path, "r");

    if(fp == NULL)
    {
        printf("Error: File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    return fp;
}

int parser(FILE * fp)
{
    int count = 0;
    char * pStr = fgets(buffer, BUF_SIZE, fp);

    char * token = strtok(pStr, delimiters);
    while(token)
    {
        count++;
        token = strtok(NULL, delimiters);
    }
    printf("%d\n", count);
    exit(EXIT_SUCCESS);
}