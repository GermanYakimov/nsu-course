#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define PATTERN_SIZE 64
#define WORD_SIZE 1024

typedef struct subexp {
	int iterations;
	char* expression;
} subexp ;

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

int substr_count(char* str, char* substr)
{
	char* tmp = strstr(str, substr);
	int count = 0;

	while (tmp)
	{
		tmp = strstr(tmp + 1, substr);
		count++;
	}

	return count;
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


void print_subpatterns(subexp* subpatterns, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("%d %s\n", subpatterns[i].iterations, subpatterns[i].expression);
	}
}

void free_subexpressions(subexp* subpatterns, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (*((subpatterns + i)->expression))
		{
			free((subpatterns + i)->expression);
		}
	}

	free(subpatterns);
}

subexp* split_pattern(char* pattern)
{
	int size = (substr_count(pattern, "[") + substr_count(pattern, "<"))* 2 + 1;
	int iterator = 0;
	char* pattern_reader = pattern;

	subexp* subexpressions = (subexp*)calloc(size, sizeof(subexp));

	if (!subexpressions)
	{
		return NULL;
	}

	subexp* subexpressions_reader = subexpressions;

	for (int i = 0; i < size; i++)
	{
		subexpressions[i].expression = (char*)calloc(PATTERN_SIZE / size + 1, sizeof(char));

		if (!subexpressions[i].expression)
		{
			free_subexpressions(subexpressions, size);
			return NULL;
		}
	}

	while (*pattern_reader)
	{
		if (*pattern_reader != '[' && *pattern_reader != '(' && *pattern_reader != ')' && *pattern_reader != ']' && *pattern_reader != '<' && *pattern_reader != '>' && *pattern_reader != '*')
		{
			subexpressions_reader->expression[iterator] = *pattern_reader;
			iterator++;

			if (!subexpressions_reader->iterations)
			{
				subexpressions_reader->iterations = 1;
			}
		}
		else if (*pattern_reader == '[')
		{
			subexpressions_reader++;
			iterator = 0;

			subexpressions_reader->iterations = extract_iterations(pattern_reader);
			pattern_reader = strstr(pattern_reader, "(");
		}
		else if (*pattern_reader == ']' && *(pattern_reader + 1) != '[')
		{
			subexpressions_reader++;
			iterator = 0;
		}
		else if (*pattern_reader == '<')
		{
			subexpressions_reader++;
			iterator = 0;

			subexpressions_reader->iterations = INT_MAX;
			//pattern_reader = strstr(pattern_reader, "<");
		}
		else if (*pattern_reader == '>' && *(pattern_reader + 1) != '<')
		{
			subexpressions_reader++;
			iterator = 0;
		}

		pattern_reader++;
	}

	//print_subpatterns(subexpressions, size);
	return subexpressions;
}

int match(char* expression, char** word)
{
	char* exp_reader = expression;
	char** word_reader = word;

	while (*exp_reader && *(*word_reader))
	{
		switch (*exp_reader)
		{
		case '\\':
			switch (*(exp_reader + 1))
			{
			case 'd':
				if (!isdigit(*(*word_reader)))
				{
					return 0;
				}
				(*word_reader)++;
				exp_reader += 2;
				break;

			case 'D':
				if (!isalpha(*(*word_reader)))
				{
					return 0;
				}
				(*word_reader)++;
				exp_reader += 2;
				break;

			default:
				break;
			}
			break;
		case '~':
			if (*(*word_reader) == *(exp_reader + 1))
			{
				return 0;
			}
			(*word_reader)++;
			exp_reader += 2;
			break;

		default:
			if (*exp_reader != *(*word_reader))
			{
				return 0;
			}

			exp_reader++;
			(*word_reader)++;
			break;
		}
	}

	if (*exp_reader && !*(*word_reader))
	{
		return 0;
	}

	return 1;
}

int compare(char* word, subexp* subexpressions)
{
	subexp* current_subexp = subexpressions;

	char* current_exp_reader = current_subexp->expression;

	char *word_reader = word, *word_reader_copy;

	while (current_subexp->expression && *word_reader)
	{
		for (int i = 0; i < current_subexp->iterations; i++)
		{
			word_reader_copy = word_reader;

			if (!match(current_subexp->expression, &word_reader))
			{
				if (current_subexp->iterations < INT_MAX)
				{
					return 0;
				}
				else
				{
					word_reader = word_reader_copy;
					break;
				}
			}
		}

		current_subexp++;
		current_exp_reader = current_subexp->expression;
	}

	if (!(*word_reader) && current_subexp->expression)
	{
		return 0;
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

	subexp* subexpressions = split_pattern(pattern);

	if (!subexpressions)
	{
		printf("Can't allocate memory for subexpressions");
		return -1;
	}

	output = fopen("output.txt", "w");

	if (!output)
	{
		printf("Can't open output.txt");
		return -1;
	}

	for (int i = 0; i < number; i++)
	{
		if (compare(words[i], subexpressions))
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
	free_subexpressions(subexpressions, substr_count(pattern, "[") * 2 + 1);
	return 0;
}