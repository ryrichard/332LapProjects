/*
Write a program with 2 children process, where one child is created to execute command that
tells you the date and time inUnix.
Use execl(...).
Note, you need to specify the full path of the file name that gives you date and time information.
Announce the successful forking of child process by displaying its PID.
And another child is created to execute a command that shows all files (including hid-den files) in a
directory with information such as permissions, owner, size, and when last modified.
Use execvp(...).
For the command to list directory contents with various options, refer the handout on Unix
filesystem sent to you in the first class.
Announce the successful forking of child process by displaying its PID.
*/
