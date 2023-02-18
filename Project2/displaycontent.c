/*
UNIX cat command has three functions with regard to text files: displaying them,
combining copies of them and creating new ones.
Write a C program to implement a command called displaycontent that takes a (text) file
name as argument and display its contents. Report an appropriate message if the file does
not exist or can’t be opened (i.e. the file doesn’t have read permission). You are to use open(),
read(), write() and close() system calls.
NOTE: Name your executable file as displaycontent and execute your program as
./displaycontent file_name
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
    //might have to use perror("open")

    //open() .. https://www.man7.org/linux/man-pages/man2/open.2.html
    int fd;

    fd = open(argv[1], O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
    if(fd == -1)
    {
        printf("Open () failed with error [%s] \n", strerror(errno));
        if(errno == ENOENT)
            printf("File does not exist\n");
        else if (errno == EACCES)
            printf("Requested access denied\n");
    }
    else
    {
        printf("Open \"%s\" sucessfully\n", argv[1]);
    }

    //read() .. https://www.man7.org/linux/man-pages/man2/read.2.html
    int byte_read;
    // char t[1024];
    // byte_read = read(fd, t, sizeof(t) - 1);
    // printf("Test: %s\n", t);
    printf("Reading %s\n\n", argv[1]);
    do
    {
        char t[1];
        byte_read = read(fd, t, 1); //one letter at a time
        if(t[0] != '\n')
            printf("%s", t);
    }
    while(byte_read > 0);
    printf("\n\n");
    if(byte_read == -1)
    {
        printf("Error when reading: [%s]\n", strerror(errno));
    }


    //write() .. https://www.man7.org/linux/man-pages/man2/write.2.html
    char* t = "testing";
    printf("Writing into %s: %s\n", argv[1], t);
    int byte_write = write(fd, t, sizeof(t) - 1);
    if(byte_write == -1)
    {
        printf("Error when writing: [%s]\n", strerror(errno));
    }


    //close() .. https://www.man7.org/linux/man-pages/man2/close.2.html
    printf("Closing %s\n,", argv[1]);
    if(close(fd) == -1)
    {
        printf("Error when closing: [%s]\n", strerror(errno));
    }

    return 0;
}