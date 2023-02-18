/*
Repeat part 3 (by writing a new C program) as per the following procedure:
(a) Read 100 characters from source.txt at a time, and among characters read, replace each character `1` with character `L` and all characters are then written in
destination.txt
(b) Write characters "XYZ" into file destination.txt
(c) Repeat the previous steps until the end of file source.txt. The last read step may not have 100 characters.
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void search_and_replace(char* c, char s, char r)
{
    char* position = strchr(c, s);
    while(position)
    {
        *position = r;
        position = strchr(c,s);
    }
}


int main(int argc, char* argv[])
{
    char xyz[3] = "XYZ";
    char *source = argv[1];
    char *destination = argv[2];
    char c[100];
    int input, output, fileread;

    //open destinationfile
    output = open("DestinationFile.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
    if( output == -1)
        perror("open");

    //open file in read-only mode
    input = open(source, O_RDONLY);
    if(input == -1)
    {
        printf("Error: [%s]\n", strerror(errno));
    }
    else //write into output file 
    {
        //a) replace each character `1` with character `L` and all characters are then written in

        // fileread = read(input, &c, sizeof(c));
        // search_and_replace(c, '1', 'L');
        // printf("%s\n", c);

        while(fileread = read(input, &c, sizeof(c)) != 0)
        {
            search_and_replace(c, '1', 'L');
            write(output, &c, sizeof(c));
            write(output, xyz, sizeof(xyz));
            memset(&c[0], 0, sizeof(c)); //resets c array i.e. 'clears' it
        }
        printf("\n");
    }
    
    close(output);

    return 0;
}