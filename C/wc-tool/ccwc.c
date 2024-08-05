#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int count_bytes(FILE * fp);
int count_lines(FILE * fp);
int count_words(FILE * fp);
int count_chars(FILE * fp);

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Error: binary requires commandline option and file to open\n");
        exit(EXIT_FAILURE);
    }
    int count = 0;
    FILE * fp;
    fp = fopen(argv[2], "r");

    if(fp == NULL)
    {
        printf("Error: File cannot be opened\n");
        exit(EXIT_FAILURE);
    }
    
    if(strcmp(argv[1], "-c") == 0)
    {
        count = count_bytes(fp);
        fprintf(stdout, "%d %s\n", count, argv[2]);
    }

    else if(strcmp(argv[1], "-l") == 0)
    {
        count = count_lines(fp);
        fprintf(stdout, "%d %s\n", count, argv[2]);
    }

    else if(strcmp(argv[1], "-w") == 0)
    {
        count = count_words(fp);
        fprintf(stdout, "%d %s\n", count, argv[2]);
    }
    else if(strcmp(argv[1], "-m") == 0)
    {
        count = count_chars(fp);
        fprintf(stdout, "%d %s\n", count, argv[2]);
    }

    fclose(fp);
    return 0;
}

int count_bytes(FILE * fp)
{
    int ch, count = 0;
    while(1)
    {
        ch = getc(fp);
        if(ch == EOF)
            break;
        else
            count++;
    }
    return count;
}

int count_lines(FILE * fp)
{
    int ch, count = 0;
    while(1)
    {
        ch = getc(fp);
        if(ch == EOF)
            break;
        else if(ch == '\n')
            count++;
    }
    return count;
}

int count_words(FILE * fp)
{
    int ch, count, prev_ch;
    count = 0;
    while(1)
    {
        ch = getc(fp);
        if(ch == EOF)
            break;
        else if(isspace(ch) && prev_ch != '\r' && prev_ch != '\n' && prev_ch != '\t' && prev_ch != ' ')
            count++;
        prev_ch = ch;
    }
    return count;
}

int count_chars(FILE * fp)
{
    int ch, count;
    count = 0;
    while(1)
    {
        ch = getc(fp);
        if(ch == EOF)
            break;
        else if(isalnum(ch) || isspace(ch) || ispunct(ch))
            count++;
    }
    return count;
 }


 
        