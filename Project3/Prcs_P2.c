/*
Copy the contents of source.txt into destination1.txt and destination2.txt
as per the following procedure.
1. Read the first 100 characters from source.txt, and among characters read, replace each
char-acter ’1’ with character ’L’ and all characters are then written in destination1.txt
2. Then the next 50 characters are read from source.txt, and among characters read, replace
each character ’3’ with character ’E’ and all characters are then written in destination2.txt.
3. The previous steps are repeated until the end of file source.txt. The last read may not
have100 or 50 characters.
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

void search_and_replace(char* c, char s, char r)
{
	char* position = strchr(c,s);
	while(position)
	{
		*position = r;
		position = strchr(c,s);
	}
}

int main()
{
	char* source = "source.txt";
	char* des1 = "destination1.txt";
	char* des2 = "destination2.txt";
	char c1[100];
	char c2[50];
	char t = 't';
	int input, output1, output2, err1, err2, fileread;

	output1 = open(des1, O_RDWR | O_TRUNC, 0666);
	if(output1 == -1)
	{
		printf("Error opening %s: [%s]\n", des1, strerror(errno));
		perror("open");
		return -1;
	}

	output2 = open(des2, O_RDWR | O_TRUNC, 0666);
	if(output2 == -1)
	{
		printf("Error opening %s: [%s]\n", des2, strerror(errno));
		perror("open");
		return -1;
	}

	input = open(source, O_RDONLY);
	if(input == -1)
	{
		printf("Error: [%s]\n", strerror(errno));
	}
	else
	{
		while(fileread = read(input, &c1, sizeof(c1)) != 0)
		{
			search_and_replace(c1, '1', 'L');
			write(output1, &c1, sizeof(c1));
			memset(&c1[0], 0, sizeof(c1));
			if(fileread = read(input, &c2, sizeof(c2)) != 0)
			{
				search_and_replace(c2, '3', 'E');
				write(output2, &c2, sizeof(c2));
				memset(&c2[0], 0, sizeof(c2));
			}

			// printf("%s\n", c1);
			// printf("%s\n", c2);
		}

	}

	close(output1);
	close(output2);

	return 0;
}
