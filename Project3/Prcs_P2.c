/*
Copy the contents of source.txt into destination1.txt and destination2.txt
as per the following procedure.
1. Read the first 100 characters from source.txt, and among characters read, replace each
char-acter ’1’ with character ’L’ and all characters are then written in destination1.txt
2. Then the next 50 characters are read from source.txt, and among characters read, replace
each character ’3’ with character ’E’ and all characters are then written in destination2.txt.
3. The previous steps are repeated until the end of file source.txt. The last read may not
have100 or 50 characters.
Once you’re done with successful creation of executables for the above two steps do the following.
Write a C program and call it Parent_Prcs.c. Execute the files as per the following procedure
using execv system call.
[Step 3] Using fork create a child process, say Child 1 that executes Prcs_P1. This will create
two destination files according to Step 1.
[Step 4] After Child 1 finishes its execution, use fork to create another child process, say Child
2 and execute
Prcs_P2 that accomplishes the procedure described in Step 2.
Note that: source.txt is already provided in the previous lab. Use that for task 4.
*/
