#include <stdio.h>
#include <strings.h>

int main(int argc, char const *argv[])
{
	FILE* in = fopen("decodeFIXED.txt", "r");
	char arr[1000];
	bzero(arr, 1000);
	char ch;
	int location;
	for (int i = 0; i < 1000; ++i)
	{
		i[arr] = '*';
	}
	// arr[0] = '*';
	char temp;
	while(fread(&temp, 1, 1, in))
	{
		location = 1;
		while(1)
		{
			fread(&ch, 1, 1, in);
			if(ch != '\n')
			{
				if (ch == '0')
				{
					location *= 2;
				}
				else
				{
					location = location*2 + 1;
				}
			}
			else
			{
				arr[location] = temp;
				break;
			}
		}
	}
	FILE* message = fopen("message.txt", "r");
	location = 1;
	while(fread(&ch, 1, 1, message))
	{
		if(ch == '0')
			location *= 2;
		else
			location = location*2 + 1;
		if(arr[location] != '*')
		{
			printf("%c", arr[location]);
			location = 1;
		}
	}
	printf("\n");
	return 0;
}
