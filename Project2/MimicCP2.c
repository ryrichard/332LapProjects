/*
Repeat part 3 (by writing a new C program) as per the following procedure:
(a) Read 100 characters from source.txt at a time, and among characters read, replace
each character `1` with character `L` and all characters are then written in
destination.txt
(b) Write characters "XYZ" into file destination.txt
(c) Repeat the previous steps until the end of file source.txt. The last read step may not
have 100 characters.
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