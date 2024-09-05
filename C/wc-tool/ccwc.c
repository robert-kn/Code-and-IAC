/* Build Your Own wc Tool
 This challenge is to build your own version of the Unix command line tool wc!
 The 'wc' (word count) command counts lines, words, and bytes (or characters) in text files.
 This simplified version will support basic options for counting lines, words, characters, and bytes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

int count_bytes(char * pStr);
int count_lines(char * pStr);
int count_words(char * pStr);
int count_chars(char * pStr);
void file_count_lines(FILE * fp);
void file_count_words(FILE * fp);
void file_count_bytes(FILE * fp);
FILE * open_file(char * file_name, char * mode);
void read_input(int arg_count,...);
void process(char * pStr, int process, int char_option, ...);
void init_buffer(void);


static char delimiters[] = " \n\r";
static int count = 0;
static int count_output[3] = {0};
static FILE * fp;
static char *buffer;
static size_t buffer_size = 100;

#define FILE_PROCESS 1
#define FILE_NO_PROCESS 0

int main(int argc, char *argv[])
{

    if(argc == 3)
    {
        fp = open_file(argv[2], "r");
        read_input(3, argv[1][1], fp); 

        printf("%d %s\n", count_output[0], argv[2]);
        fclose(fp);
    }

    else if(argc == 2)
    {
        if(argv[1][0] == '-' && isalpha(argv[1][1])) /* user wants to supply input from stdin */
        {
            read_input(2, argv[1][1]);
            printf("%d\n", count_output[0]);
        }
        else
        {
            fp = open_file(argv[1], "r");
            init_buffer();
            file_count_lines(fp);
            fseek(fp, 0, SEEK_SET);
            file_count_words(fp);
            fseek(fp, 0, SEEK_SET);
            file_count_bytes(fp);
            printf("%d %d %d %s\n", count_output[0], count_output[1], count_output[2], argv[1]);
            fclose(fp);
        }
    }
    free(buffer);
    buffer = NULL;
    exit(EXIT_SUCCESS);
}

FILE * open_file(char * file_name, char * mode)
{
    FILE * fp = fopen(file_name, mode);
    if(fp == NULL)
    {
        printf("Error: %s File cannot be opened\n", file_name);
        exit(EXIT_FAILURE);
    }
    return fp;
}

void read_input(int arg_count, ...)
{ 
    init_buffer();
    char * pStr;                                                       
    int char_option = 0;
    va_list arg;
    va_start(arg, arg_count);
    char_option = va_arg(arg, int);

    if(arg_count == 3)
    {
        static FILE * fp = NULL;
        fp = va_arg(arg, FILE *);
    }
    
    va_end(arg);

    if(char_option && arg_count == 3)
    {
        pStr = fgets(buffer, buffer_size, fp);
        process(pStr, FILE_PROCESS, char_option, fp);
        
    }
    else if(char_option && arg_count == 2)
    {
        pStr = fgets(buffer, buffer_size, stdin);
        process(pStr, FILE_NO_PROCESS, char_option);
        
    }
}

void process(char * pStr, int process, int char_option, ...)
{
    if(process)
    {
        static FILE * fp;
        va_list arg;
        va_start(arg, char_option);
        fp = va_arg(arg, FILE *);
        va_end(arg);

    }

   
    while(pStr != NULL)
    {
             
        switch(char_option)
        {

            case 'c':
                count_output[0] += count_bytes(pStr);
            break;

            case 'l':
                count_output[0] += count_lines(pStr); 
            break;

            case 'w':
                count_output[0] += count_words(pStr); 
            break;

            case 'm':
                count_output[0] += count_chars(pStr); 
            break;

            default:
                printf("Invalid command line option. Needs to be one of [l | w | m | c]\n");
                exit(EXIT_FAILURE);
            break;
        }

        if(process)
            pStr = fgets(buffer, buffer_size, fp);
        else
            pStr = fgets(buffer, buffer_size, stdin);
    }
}

void file_count_lines(FILE * fp)
{
    char * pStr = fgets(buffer, buffer_size, fp);
    while(pStr != NULL)
    {
        count_output[0] += count_lines(pStr);
        pStr = fgets(buffer, buffer_size, fp);
    }
}

void file_count_words(FILE * fp)
{
    char * pStr = fgets(buffer, buffer_size, fp);
    while(pStr != NULL)
    {
        count_output[1] += count_words(pStr);
        pStr = fgets(buffer, buffer_size, fp);
    }
}

void file_count_bytes(FILE * fp)
{
    char * pStr = fgets(buffer, buffer_size, fp);
    while(pStr != NULL)
    {
        count_output[2] += count_bytes(pStr);
        pStr = fgets(buffer, buffer_size, fp);
    }
}


int count_bytes(char * pStr)
{
    int count = 0;
    while(*pStr)
    {
        count++;
        pStr++;
    }
    return count;
}

int count_lines(char * pStr)
{
    int count = 0;
    while(*pStr)
    {
        if(*pStr == '\n')
            count++;
        pStr++;
    }
    return count;
}

int count_words(char * pStr)
{
    int count = 0;
    char * token = strtok(pStr, delimiters);

    while(token)
    {
        count++; 
        token = strtok(NULL, delimiters);
    }
    return count;
}

int count_chars(char * pStr)
{
    int count = 0;

    while(*pStr)
    {
        if((*pStr & 0xC0) != 0x80)
            count++; 
        else if(isascii(*pStr))
            count++;
        pStr++;
    }
    return count;
}

void init_buffer(void)
{
    buffer = (char *) calloc(buffer_size, sizeof(char));
}




 
        