/* Build Your Own wc Tool
 This challenge is to build your own version of the Unix command line tool wc!
 The 'wc' (word count) command counts lines, words, and bytes (or characters) in text files.
 This simplified version will support basic options for counting lines, words, characters, and bytes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdarg.h>

int count_bytes(char * pStr);
int count_lines(char * pStr);
int count_words(char * pStr);
int count_chars(char * pStr);
void read_text(char c);
FILE * open_file(char * file_name, char * mode);
int * process(FILE * fp,...);

char delimiters[] = " \n\r";
static int count = 0;
FILE * fp;
char *buffer;

int main(int argc, char *argv[])
{

    if(argc == 3)
    {
        
        fp = open_file(argv[2], "r");
        int * output = process(fp, argv[1][1]);

        // char *buffer;
        // size_t buffer_size = 100;
        // buffer = (char *) calloc(buffer_size, sizeof(char));
        // char * pStr;

        // pStr = fgets(buffer, buffer_size, fp);

        // while(pStr != NULL)
        // {

        //     switch(argv[1][1])
        //     {

        //         case 'c':
        //             count += count_bytes(pStr);
        //         break;

        //         case 'l':
        //             count += count_lines(pStr); 
        //         break;

        //         case 'w':
        //             count += count_words(pStr); 
        //         break;

        //         case 'm':
        //             count += count_chars(pStr); 
        //         break;

        //         default:
        //             printf("Invalid command line option. Needs to be one of [l | w | m | c]\n");
        //             exit(EXIT_FAILURE);
        //         break;
        //     }
                
        //     pStr = fgets(buffer, buffer_size, fp);
        // } 

        free(buffer);
        buffer = NULL; 

        
        printf("%d %s\n", output[0], argv[2]);
        fclose(fp);
        exit(EXIT_SUCCESS);
    }

    else if(argc == 2)
    {
        if(argv[1][1] == 'l' || argv[1][1] == 'c' || argv[1][1] == 'w' || argv[1][1] == 'm') /* user wants to supply input from stdin */
        {
            read_text(argv[1][1]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            fp = open_file(argv[1], "r");

            fclose(fp);
            exit(EXIT_SUCCESS);

        }

        return 2;
    }
    
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

int * process(FILE * fp, ...)
{
    size_t buffer_size = 100;
    buffer = (char *) calloc(buffer_size, sizeof(char));
    char * pStr;
    static int count_output[3] = {0};
    char * str;
    va_list arg_ptr;
    va_start(arg_ptr, fp);
    str = va_arg(arg_ptr, char *);
    va_end(arg_ptr);

    pStr = fgets(buffer, buffer_size, fp);
    printf("finished successfully %s\n", pStr);
    while(pStr != NULL)
    {
        if(str)
        {
            switch(str[0])
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
        }
        else
        {

        }
        pStr = fgets(buffer, buffer_size, fp);
    }
    
    
    return count_output;
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

void read_text(char c)
{
    char *buffer;
    size_t buffer_size = 100;
    char * pStr;

    buffer = (char *) calloc(buffer_size, sizeof(char));

    if(buffer == NULL)
    {
        printf("Unable to allocate buffer\n");
        exit(EXIT_FAILURE);
    }


    while(true)
    {
        pStr = fgets(buffer, buffer_size, stdin);

        if( pStr != NULL)
        {
            
        }
        else
        {
            printf("Error related to input\n");
            break;
        }    
        
    }
}


 
        