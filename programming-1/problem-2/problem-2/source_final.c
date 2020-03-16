#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define PATTERN_SIZE 64
#define WORD_SIZE 1024
#define MAX_PATTERN_SIZE 2048

typedef struct match_result {
	char *pattern_reader;
	char *word_reader;
	int match;
} state;

state match(char* pattern, char* word, char* unexpected_symbols);
int compare(char *word, char *pattern);
int extract_iterations(char *expression);

char **read_words(FILE* file, int number, char **words)
{
	for (int i = 0; i < number; i++)
	{
		fscanf(file, "%s", words[i]);
	}

	return words;
}

void free_words_array(char **words, int number)
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

char *strcut(char* begin, char* end)
{
	int a = end - begin;
	char* result = (char*)calloc(end - begin + 1, sizeof(char));

	if (!result)
	{
		printf("Can't allocate memory for str");
		return NULL;
	}

	int counter = 0;

	while (begin != end)
	{
		result[counter] = *begin;
		begin++;
		counter++;
	}

	return result;
}

char *make_pattern(char *pattern)
{
	char *pattern_reader = pattern;
	char* tmp;
	char *new_pattern = (char*)calloc(MAX_PATTERN_SIZE, sizeof(char));

	if (!new_pattern)
	{
		printf("Can't allocate memory for new pattern");
		return NULL;
	}

	int counter = 0, iterations;

	while (*pattern_reader)
	{
		if (*pattern_reader == '[')
		{
			tmp = strcut(strstr(pattern_reader, "(") + 1, strstr(pattern_reader, ")"));

			if (!tmp)
			{
				return NULL;
			}

			iterations = extract_iterations(pattern_reader);

			for (int i = 0; i < iterations; i++)
			{
				new_pattern = strcat(new_pattern, tmp);
			}

			free(tmp);
			pattern_reader = strstr(pattern_reader, "]") + 1;
			counter = strlen(new_pattern);
		}
		else
		{
			new_pattern[counter] = *pattern_reader;
			pattern_reader++;
			counter++;
		}
	}

	return new_pattern;
}

char **allocate_words_array(int number)
{
	char **words = (char**)calloc(number, sizeof(char*));

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

int extract_iterations(char *expression)
{
	int iterations = 0;
	char *reader = expression + 1, *end = strstr(reader, "*");

	while (reader != end)
	{
		iterations *= 10;
		iterations += *reader - '0';
		reader++;
	}

	return iterations;
}

char *compare_symbol(char symbol, char *pattern)
{
	switch (*pattern)
	{
	case '\\':
		switch (*(pattern + 1))
		{
		case 'd':
			if (!isdigit(symbol))
			{
				return NULL;
			}

			pattern += 2;
			break;

		case 'D':
			if (!isalpha(symbol))
			{
				return NULL;
			}

			pattern += 2;
			break;

		default:
			break;
		}
		break;
	case '~':
		if (symbol == *(pattern + 1))
		{
			return NULL;
		}

		pattern += 2;
		break;

	default:
		if (symbol != *pattern)
		{
			return NULL;
		}

		pattern++;
		break;
	}

	return pattern;
}

char *compare_kleene_star(char *word, char *pattern)
{
	char *word_reader = word, *pattern_reader = strstr(pattern, "<") + 1;
	char *pattern_end = strstr(pattern, ">"), *pattern_start = pattern_reader, *last_position = word_reader;

	state match_result;

	pattern_reader = pattern_start;

	while (*word_reader && pattern_reader != pattern_end)
	{
		match_result = match(pattern_reader, word_reader, "<>");

		if (!match_result.match)
		{
			return last_position;
		}

		pattern_reader = match_result.pattern_reader;
		word_reader = match_result.word_reader;
	}

	if (!(*word_reader))
	{
		return last_position;
	}

	last_position = word_reader;

	return last_position;
}

int compare(char *word, char *pattern)
{
	char* word_reader = word, *pattern_reader = pattern;
	state match_result;

	while (*word_reader && *pattern_reader)
	{
		match_result = match(pattern_reader, word_reader, "");

		if (!match_result.match)
		{
			return 0;
		}

		if (match_result.match == 200)
		{
			return 1;
		}

		word_reader = match_result.word_reader;
		pattern_reader = match_result.pattern_reader;

	}

	if (!(*word_reader) && *pattern_reader)
	{
		return 0;
	}

	return 1;
}

state match(char *pattern, char *word, char* unexpected_symbols)
{
	state result;
	result.match = 0;
	result.pattern_reader = pattern;
	result.word_reader = word;
	char *last_position;

	if (strchr(unexpected_symbols, *pattern))
	{
		return result;
	}

	char *pattern_reader = pattern, *word_reader = word;

	switch (*pattern_reader)
	{
	case '<':
		for (int i = 0; i < INT_MAX; i++)
		{
			if (compare(word_reader, strstr(pattern_reader, ">") + 2))
			{
				result.match = 200;
				result.pattern_reader = pattern_reader;
				result.word_reader = word_reader;

				return result;
			}
			last_position = word_reader;
			word_reader = compare_kleene_star(word_reader, pattern_reader);

			if (word_reader == last_position)
			{
				result.match = 0;
				result.pattern_reader = pattern_reader;
				result.word_reader = word_reader;

				return result;
			}
		}
		pattern_reader = strstr(pattern_reader, ">") + 2;
		break;

	case '\\':
	case '~':
	default:
		pattern_reader = compare_symbol(*word_reader, pattern_reader);

		if (!pattern_reader)
		{
			result.match = 0;
			result.pattern_reader = pattern_reader;
			result.word_reader = word_reader;

			return result;
		}

		word_reader++;
	}

	result.match = 1;
	result.pattern_reader = pattern_reader;
	result.word_reader = word_reader;

	return result;
}

int main()
{
	char pattern[PATTERN_SIZE];
	char* pattern_modified;
	FILE *input, *output;
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

	pattern_modified = make_pattern(pattern);

	if (!pattern_modified)
	{
		return NULL;
	}

	output = fopen("output.txt", "w");

	if (!output)
	{
		printf("Can't open output.txt");
		return -1;
	}

	for (int i = 0; i < number; i++)
	{
		if (compare(words[i], pattern_modified))
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
	free(pattern_modified);

	return 0;
}