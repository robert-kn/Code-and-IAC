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

int count_bytes(char * pStr);
int count_lines(char * pStr);
int count_words(char * pStr);
int count_chars(char * pStr);
void read_text(char c);

static int count = 0;

int main(int argc, char *argv[])
{
    FILE * fp;

    if(argc == 3)
    {
        
        fp = fopen(argv[2], "r");

        if(fp == NULL)
        {
            printf("Error: File cannot be opened\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            char *buffer;
            size_t buffer_size = 100;
            buffer = (char *) calloc(buffer_size, sizeof(char));
            char * pStr;

            pStr = fgets(buffer, buffer_size, fp);

            while(pStr != NULL)
            {

                switch(argv[1][1])
                {

                    case 'c':
                        count += count_bytes(pStr);
                    break;

                    case 'l':
                        count += count_lines(pStr); 
                    break;

                    case 'w':
                        count += count_words(pStr); 
                    break;

                    case 'm':
                        count += count_chars(pStr); 
                    break;

                    default:
                        printf("Invalid command line option. Needs to be one of [l | w | m | c]\n");
                        exit(EXIT_FAILURE);
                    break;
                }
                
                pStr = fgets(buffer, buffer_size, fp);
            } 
            free(buffer);
            buffer = NULL;           
        }
        printf("%d %s\n", count, argv[2]);
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

        return 2;
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
    int prev_ch, count = 0;
    prev_ch = 0;
    while(*pStr)
    {
        if(isspace(*pStr))
        {
            if(isspace(prev_ch))
                continue;
            else
            {
                count++;
            }
        }
                
        prev_ch = *pStr;
        pStr++;
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


 
        