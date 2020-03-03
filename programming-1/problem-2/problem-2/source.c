#include <stdio.h>
#include <stdlib.h>
#define PATTERN_SIZE 64
#define WORD_SIZE 1024

char** read_words(int number, char** words)
{
	for (int i = 0; i < number; i++)
	{
		scanf("%s", words[i]);
	}

	return words;
}

void free_words(char** words, int number)
{
	for (int i = 0; i < number; i++)
	{
		if (words[i])
		{
			free(words[i]);
		}
		else
		{
			break;
		}
	}

	free(words);
}

char** allocate_words(int number)
{
	char** words = (char**)calloc(number, sizeof(char*));

	if (!words)
	{
		return NULL;
	}

	for (int i = 0; i < number; i++)
	{
		words[i] = (char*)malloc(WORD_SIZE * sizeof(char));

		if (!words[i])
		{
			free_words(words, number);
			return NULL;
		}
	}

	return words;
}

void print_words(char** words, int number)
{
	for (int i = 0; i < number; i++)
	{
		printf("%s\n", words[i]);
	}
}

int main()
{
	char pattern[PATTERN_SIZE];
	int number;
	scanf("%s", pattern);
	scanf("%d", &number);

	char** words = allocate_words(number);
	words = read_words(number, words);

	print_words(words, number);

	free_words(words, number);
	return 0;
}
