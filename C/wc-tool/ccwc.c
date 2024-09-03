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

int count_bytes(FILE * fp);
int count_lines(FILE * fp);
int count_words(FILE * fp);
int count_chars(FILE * fp);
void read_text(char c);

int count = 0;

int main(int argc, char *argv[])
{
    FILE * fp;

    if(argc == 3)
    {
        
        fp = fopen_s(argv[2], "r");

        if(fp == NULL)
        {
            printf("Error: File cannot be opened\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            switch(argv[1][1])
            {
                case 'c':
                    count = count_bytes(fp); /* outputs 342,190 which is correct per wc */
                break;

                case 'l':
                    count = count_lines(fp); /* outputs 7145 which is correct per output of wc */
                break;

                case 'w':
                    count = count_words(fp); /* outputs 58,164 which is correct per output of wc */
                break;

                case 'm':
                    count = count_chars(fp); /* outputs 337,634 instead of 339,292. Why? some characters are clearly not being counted */
                break;

                default:
                    printf("Invalid command line option. Needs to be one of [l | w | m | c]\n");
                    exit(EXIT_FAILURE);
                break;
            }
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
        fp = fopen(argv[1], "r");

        if(fp == NULL)
        {
            printf("Error: File cannot be opened\n");
            exit(EXIT_FAILURE);
        }

        printf("%d ", count_lines(fp));
        fseek(fp, 0, SEEK_SET);
        printf("%d ", count_words(fp));
        fseek(fp, 0, SEEK_SET);
        printf("%d ", count_bytes(fp));
        printf("%s\n", argv[1]);
        fclose(fp);
        exit(EXIT_SUCCESS);
    }
    
}

int count_bytes(FILE * fp)
{
    int ch, count = 0;
    do
    {
        ch = getc(fp);
        if(ch != EOF)
            count++;
    }while(ch != EOF);
    return count;
}

int count_lines(FILE * fp)
{
    int ch, count = 0;
    while(true)
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
    while(true)
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
    while(true)
    {
        ch = getc(fp);
        if(ch == EOF)
            break;
        else if(isascii(ch))
            count++;
    }
    return count;
 }

 void read_text(char c)
 {
    char *buffer;
    size_t buffer_size = 100;
    size_t characters;

    buffer = (char *)malloc(buffer_size * sizeof(char));

    if(buffer == NULL)
    {
        printf("Unable to allocate buffer\n");
        exit(EXIT_FAILURE);
    }

    while(true)
    {
        characters = getline(&buffer, &buffer_size, stdin);

        if(characters == -1)
        {
            printf("Error occured or EOF reached\n");

        }
        else
        {
            switch(c)
            {
                case 'c':
                    count += count_bytes((FILE *) buffer);
                break;

                case 'l':
                    count += count_lines((FILE *) buffer);
                break;

                case 'w':
                    count += count_words((FILE *) buffer);
                break;

                case 'm':
                    count += count_chars((FILE *) buffer);
                break;

                default:
                    printf("Invalid command line option. Needs to be one of [l | w | m | c]\n");
                    exit(EXIT_FAILURE);
                break;
            }
        }
    }
    printf("%d\n", count);
 }


 
        