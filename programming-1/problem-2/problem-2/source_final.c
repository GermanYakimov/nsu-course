#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define PATTERN_SIZE 64
#define WORD_SIZE 1024

char** read_words(FILE* file, int number, char** words)
{
	for (int i = 0; i < number; i++)
	{
		fscanf(file, "%s", words[i]);
	}

	return words;
}

void free_words_array(char** words, int number)
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

char** allocate_words_array(int number)
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
			free_words_array(words, number);
			return NULL;
		}
	}

	return words;
}

int extract_iterations(char* exp_start)
{
	int iterations = 0;
	char* exp_reader = exp_start + 1;
	char* exp_end = strstr(exp_reader, "*");

	while (exp_reader != exp_end)
	{
		iterations *= 10;
		iterations += *exp_reader - '0';
		exp_reader++;
	}

	return iterations;
}

int compare_iterations(char** word, char* pattern);

char* compare_kleene_star(char* word, char* pattern)
{
	char* word_reader = word;
	char* pattern_reader = strstr(pattern, "<") + 1;
	char* pattern_end = strstr(pattern, ">");
	char* pattern_start = pattern_reader;

	char* last_position = word_reader;

	for (int i = 0; i < INT_MAX; i++)
	{
		pattern_reader = pattern_start;
		while (*word_reader && pattern_reader != pattern_end)
		{
			switch (*pattern_reader)
			{
			case '\\':
				switch (*(pattern_reader + 1))
				{
				case 'd':
					if (!isdigit(*word_reader))
					{
						return last_position;
					}
					word_reader++;
					pattern_reader += 2;
					break;

				case 'D':
					if (!isalpha(*word_reader))
					{
						return last_position;
					}
					word_reader++;
					pattern_reader += 2;
					break;

				default:
					break;
				}
				break;
			case '[':
				if (!compare_iterations(&word_reader, pattern_reader))
				{
					return last_position;
				}
				pattern_reader = strstr(pattern_reader, "]") + 1;
				break;
			case '~':
				if (*word_reader == *(pattern_reader + 1))
				{
					return last_position;
				}
				word_reader++;
				pattern_reader += 2;
				break;

			default:
				if (*pattern_reader != *word_reader)
				{
					return last_position;
				}

				pattern_reader++;
				word_reader++;
				break;
			}
		}

		if (!(*word_reader))
		{
			return last_position;
		}

		last_position = word_reader;
	}

	return last_position;
}

int compare_iterations(char** word, char* pattern)
{
	int iterations = extract_iterations(pattern);
	char** word_reader = word;
	char* pattern_reader = strstr(pattern, "(") + 1;
	char* pattern_end = strstr(pattern, ")");
	char* pattern_start = pattern_reader;

	for (int i = 0; i < iterations; i++)
	{
		pattern_reader = pattern_start;
		while (**word_reader && pattern_reader != pattern_end)
		{
			switch (*pattern_reader)
			{
			case '\\':
				switch (*(pattern_reader + 1))
				{
				case 'd':
					if (!isdigit(**word_reader))
					{
						return 0;
					}
					(*word_reader)++;
					pattern_reader += 2;
					break;

				case 'D':
					if (!isalpha(**word_reader))
					{
						return 0;
					}
					(*word_reader)++;
					pattern_reader += 2;
					break;

				default:
					break;
				}
				break;
			case '~':
				if (**word_reader == *(pattern_reader + 1))
				{
					return 0;
				}
				(*word_reader)++;
				pattern_reader += 2;
				break;

			case '<':
				*word_reader = compare_kleene_star(*word_reader, pattern_reader);
				pattern_reader = strstr(pattern_reader, "*") + 1;
				break;
			default:
				if (*pattern_reader != **word_reader)
				{
					return 0;
				}

				pattern_reader++;
				(*word_reader)++;
				break;
			}
		}

		if (!(**word_reader))
		{
			return 0;
		}
	}

	return 1;
}



int compare(char* word, char* pattern)
{
	char* word_reader = word, *pattern_reader = pattern;

	while (*word_reader && *pattern_reader)
	{
		switch (*pattern_reader)
		{
		case '\\':
			switch (*(pattern_reader + 1))
			{
			case 'd':
				if (!isdigit(*word_reader))
				{
					return 0;
				}
				word_reader++;
				pattern_reader += 2;
				break;
		
			case 'D':
				if (!isalpha(*word_reader))
				{
					return 0;
				}
				word_reader++;
				pattern_reader += 2;
				break;
		
			default:
				break;
			}
			break;
		case '[':
			if (!compare_iterations(&word_reader, pattern_reader))
			{
				return 0;
			}
			pattern_reader = strstr(pattern_reader, "]") + 1;
			break;

		case '<':
			word_reader = compare_kleene_star(word_reader, pattern_reader);
			pattern_reader = strstr(pattern_reader, ">") + 2;
			break;
		case '~':
			if (*word_reader == *(pattern_reader + 1))
			{
				return 0;
			}
			word_reader++;
			pattern_reader += 2;
			break;
		
		default:
			if (*pattern_reader != *word_reader)
			{
				return 0;
			}
		
			pattern_reader++;
			word_reader++;
			break;
		}
	}

	return 1;
}

int main()
{
	char pattern[PATTERN_SIZE];
	FILE* input, *output;
	input = fopen("input.txt", "r");
	
	if (!input)
	{
		printf("Can't open input.txt");
		return -1;
	}
	
	int number, matches_count = 0;
	fscanf(input, "%s", pattern);
	fscanf(input, "%d", &number);
	
	char** words = allocate_words_array(number);
	
	if (!words)
	{
		printf("Can't allocate memory for words array");
		return -1;
	}
	
	words = read_words(input, number, words);

	fclose(input);

	output = fopen("output.txt", "w");
	
	if (!output)
	{
		printf("Can't open output.txt");
		return -1;
	}

	for (int i = 0; i < number; i++)
	{
		if (compare(words[i], pattern))
		{
			fprintf(output, "%d ", i);
			matches_count++;
		}
	}
	
	if (!matches_count)
	{
		fprintf(output, "none");
	}
	fclose(output);
	
	free_words_array(words, number);

	return 0;
}
