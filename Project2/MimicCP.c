/*
The cp command copies the source file specified by the SourceFile parameter to the
destination file specified by the DestinationFile parameter.
Write a C program that mimics the cp command using open() system call to open source.txt
file in read-only mode and copy the contents of it to destination.txt using read() and write()
system calls.
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
    //argv[1] == source
    //argv[2] == destination
    char *source = argv[1];
    char *destination = argv[2];
    char c = 0;
    int input, output, fileread;
    
    //open destinationfile
    output = open(destination, O_CREAT | O_RDWR | O_TRUNC, 0666);
    if(output == -1)
        perror("open");

    //open file in read-only mode
    input = open(source, O_RDONLY);
    if(input == -1)
    {
        printf("Error: [%s]\n", strerror(errno));
    }
    else //write into output file
    {
        while((fileread = read(input, &c, 1)) != 0)
        {
            write(output, &c, 1);
            printf("%c", c);
        }
        printf("\n");
    }

    close(output);

    return 0;
}