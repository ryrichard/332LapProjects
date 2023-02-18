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

int main(int argc, char* argv[])
{
    char c = 0;
    int input, output, fileread;
    //open destinationfile
    if(output = open("DestinationFile.txt", O_CREAT | O_RDWR, 0666)) == -1)
        perror("open");

    //open file in read-only mode
    input = open("source.txt", O_RDONLY);
    if(input == -1)
    {
        printf("error");
    }
    else //write into output file
    {
        do 
        {
            fileread = read(input, &c, 1);
            write(output, &c, 1);
            printf("%c ", t);
        }
        while(fileread != 0);
    }
    close(output);

    return 0;
}