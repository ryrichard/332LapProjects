/*
1. (a) Extend code snippet 1 to check for read and write access permissions of a given file
(b) Write a C program where open system call creates a new file (say, destination.txt) and
then opens it. (Hint: use the bitwise OR flag)
Marks: 5
*/

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main (int argc, char* argv[])
{
    char* filepath = argv[1];
    int returnval;

    // Check file existence
    returnval = access (filepath, F_OK);
    if (returnval == 0)
        printf ("\n %s exists\n", filepath);
    else
    {
        if (errno == ENOENT)
            printf ("%s does not exist\n", filepath);
        else if (errno == EACCES)
            printf ("%s is not accessible\n", filepath);
        return 0;
    }

    // Check read access
    returnval = access(filepath, R_OK);
    if(returnval == 0)
    {
        prinf("\n %s is readable \n", filepath);
    }
    else if(returnval == -1)
    {
        printf("Error: %s\n", strerror(errno));
    }

    // Check write access
    returnval = access(filepath, W_OK);
    if(returnval == 0)
    {
        prinf("\n %s is writeable \n", filepath);
    }
    else if(returnval == -1)
    {
        printf("Error: %s\n", strerror(errno));
    }

    //Create file destination.txt and open it
    int fd;

    //S_IRUSR - Read permission bit for the owner of the file. On many systems this bit is 0400. S_IREAD is an obsolete synonym provided for BSD compatibility
    //S_IWUSR - Write permission bit for the owner of the file. Usually 0200. S_IWRITE is an obsolete synonym provided for BSD compatibility. 
    //S_IRGRP - Read permission bit for the group owner of the file. Usually 040. 
    //S_IWGRP - Write permission bit for the group owner of the file. Usually 020
    //S_IROTH - Read permission bit for other users. Usually 04. 
    fd = open(argv[1], O_CREAT | O_RDWR, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH)
    if(fd == -1)
    {
        printf("\n open() failed with error [%s]\n," strerror(errno));
        return 1;
    }
    else
    {
        printf("\n Open() successfully\n");
    }
    close(fd);
    
    return 0;
}