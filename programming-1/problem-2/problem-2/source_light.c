#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define PATTERN_SIZE 64
#define WORD_SIZE 1024

typedef struct subpattern {
	int iterations;
	char* expression;
	int is_kleene_star;
} subpattern ;

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


void print_subpatterns(subpattern* subpatterns, int size)
{
	for (int i = 0; i < size; i++)
	{
		printf("%d %s\n", subpatterns[i].iterations, subpatterns[i].expression);
	}
}

void free_subpatterns(subpattern* subpatterns, int size)
{
	for (int i = 0; i < size; i++)
	{
		free((subpatterns + i)->expression);
	}

	free(subpatterns);
}

subpattern* split_pattern(char* pattern)
{
	int size = substr_count(pattern, "[") * 2 + 1;
	int iterator = 0, subiterator = 0, iterations;
	char* pattern_reader = pattern;

	subpattern* subpatterns = (subpattern*)calloc(size, sizeof(subpattern));
	subpattern* subpatterns_reader = subpatterns;

	if (!subpatterns)
	{
		return NULL;
	}

	for (int i = 0; i < size; i++)
	{
		subpatterns[i].expression = (char*)calloc(PATTERN_SIZE / size + 1, sizeof(char));
	}

	while (*pattern_reader)
	{
		if (*pattern_reader != '[' && *pattern_reader != '(' && *pattern_reader != ')' && *pattern_reader != ']')
		{
			subpatterns_reader->expression[subiterator] = *pattern_reader;
			subiterator++;

			if (!subpatterns_reader->iterations)
			{
				subpatterns_reader->iterations = 1;
			}
		}
		else if (*pattern_reader == '[')
		{
			subpatterns_reader++;
			subiterator = 0;

			subpatterns_reader->iterations = extract_iterations(pattern_reader);
			pattern_reader = strstr(pattern_reader, "(");
		}
		else if (*pattern_reader == ']' && *(pattern_reader + 1) != '[')
		{
			subpatterns_reader++;
			subiterator = 0;
		}

		pattern_reader++;
	}

	//print_subpatterns(subpatterns, size);
	return subpatterns;
}

int match(char* word, subpattern* subpatterns)
{
	subpattern* current_subpattern = subpatterns;
	char* current_expression_reader = current_subpattern->expression;

	char *word_reader = word;

	while (current_subpattern->expression && *word_reader)
	{
		for (int i = 0; i < current_subpattern->iterations; i++)
		{
			current_expression_reader = current_subpattern->expression;

			while (*current_expression_reader)
			{
				if (!word_reader)
				{
					return 0;
				}

				switch (*current_expression_reader)
				{
				case '\\':
					switch (*(current_expression_reader + 1))
					{
					case 'd':
						if (!isdigit(*word_reader))
						{
							return 0;
						}
						word_reader++;
						current_expression_reader += 2;
						break;

					case 'D':
						if (!isalpha(*word_reader))
						{
							return 0;
						}
						word_reader++;
						current_expression_reader += 2;
						break;

					default:
						break;
					}
					break;
				case '~':
					if (*word_reader == *(current_expression_reader + 1))
					{
						return 0;
					}
					word_reader++;
					current_expression_reader += 2;
					break;

				default:
					if (*current_expression_reader != *word_reader)
					{
						return 0;
					}

					current_expression_reader++;
					word_reader++;
					break;
				}
			}
		}

		current_subpattern++;
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

	subpattern* subpatterns = split_pattern(pattern);

	output = fopen("output.txt", "w");

	if (!output)
	{
		printf("Can't open output.txt");
		return -1;
	}

	for (int i = 0; i < number; i++)
	{
		if (match(words[i], subpatterns))
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
	free_subpatterns(subpatterns, substr_count(pattern, "[") * 2 + 1);
	return 0;
}