/* This is a challenge to build my own compression tool (Huffman encoder/decoder).

Video on understanding cache hierarchies to improve code execution performance
https://www.youtube.com/watch?v=WDIkqP4JbkE

Slides from the talk can be found here
https://www.aristeia.com/TalkNotes/codedive-CPUCachesHandouts.pdf

Things that I do not understand at the moment:
1. What is an optimal prefix code (used for lossless data compression) and why does it matter?
2. What is lossless data compression and how does it work?

Steps involved in compression using Huffman codes:
1. read the text and determine the frequency of each character occuring 
2. build binary tree from the frequencies
3. generate prefix-code table from the tree
4. encode the text using the code table
5. encode the tree - needs to be included in the output file so we can decode it
6. write the encoded tree and text to an output field
*/

#include <stdio.h>
#include <stdlib.h>

FILE * open_file(char file_path[]);

/* step 1: open a file that is fed in as input and read the text to determine the frequency of each character
occuring within the text.

As a debugging aid, log out the table of character frequencies. Some test values to check; there are 333 
occurrences of 'X' and 223000 of 't'.

As an alternative to logging the frequency table, write some unit tests that take in some prepared data and
then check that the counting code returns the correct values. Check your chosen programming language's 
documentation for its unit testing library (consider giving test driven development a go)*/

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("usage: ./ct [FILE]\n");
        exit(EXIT_FAILURE);
    }

    FILE * fp = open_file(argv[1]);
    fclose(fp);
    return 0;
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




/* Step 2: In this step your goal is to use the frequencies that you calculated in step 1 to build the binary 
tree.  There’s a good explanation of how to do this complete with a visualisation here: 

 https://opendsa-server.cs.vt.edu/ODSA/Books/CS3/html/Huffman.html
 
 The examples used for the visualisation would be a good starting point for unit tests.
 */


/* step 3: In this step your goal is to use the tree to generate the prefix-code table. Once again there’s 
an explanation and visualisation at:

https://opendsa-server.cs.vt.edu/ODSA/Books/CS3/html/Huffman.html
*/


/* step 4: write a header section to the output file (you’ll want a command line option to specify the 
filename). The header section will include enough information for your program to be decode the compressed 
file.

One way of doing this is to write out the tree, another option is to write out the character frequency table. 
Don’t forget you’ll need some way of knowing when the header ends and when the compressed data starts.
*/


/* step 5: In this step your goal is to encode the text using the code table and write the compressed content 
of the source text to the output file file, appending it after the header. Don’t forget translate the 
prefixes into bit strings and pack them into bytes to achieve the compression.
*/


/*step 6: In this step your goal is to read in the header of the encoded file and rebuild the prefix table 
ready to decode the text. In essence you’re going to do the reverse of step 4.
*/


/*step 7: In this step you will read in the remainder of the file and decode it using the prefix table 
before writing the decoded text back to the specified output file - the reverse of step 5.

At this point you should be able to compress the file with your tool, compare the file size of the input and 
output see that the output is smaller and then decompress the output file to create an identical copy of the 
input.

If so, congratulations you’ve written a working Huffman encode/decode tool!
*/