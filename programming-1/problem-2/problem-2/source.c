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

int extract_iterations(char* begin, char* end)
{
	int iterations = 0;

	while (begin != end)
	{
		iterations *= 10;
		iterations += *begin - '0';
		begin++;
	}

	return iterations;
}

int count_pattern_length(char* start, char* end)
{
	int length = 0;
	int brackets_count;

	while (start != end)
	{
		if (*start == '[')
		{
			length += extract_iterations(start + 1, strstr(start, "*")) * count_pattern_length(strstr(start, "("), strstr(start, ")"));
			brackets_count = 1;
			start++;

			while (brackets_count)
			{
				if (*start == '[')
				{
					brackets_count++;
				}
				else if (*start == ']')
				{
					brackets_count--;
				}
				start++;
			}

			continue;
		}

		if (*start != '\\' && *start != '(' && *start != '~' && *start != ')')
		{
			length++;
		}
		start++;
	}

	return length;
}


int match(char* word_start, char* word_end, char* pattern_start, char* pattern_end)
{
	if (count_pattern_length(pattern_start, pattern_end) != word_end - word_start)
	{
		return 0;
	}

	char* tmp_word = word_start, *tmp_pattern = pattern_start;
	int iterations, pattern_length, brackets_count;

	while (tmp_word != word_end && tmp_pattern != pattern_end)
	{
		if (*tmp_pattern != '[' && *tmp_pattern != '~' && *tmp_pattern != '\\')
		{
			if (*tmp_pattern != *tmp_word)
			{
				return 0;
			}
			tmp_pattern++;
			tmp_word++;
		}
		else
		{
			switch (*tmp_pattern)
			{
			case '\\':
				switch (*(tmp_pattern + 1))
				{
				case 'd': 
					if (!isdigit(*tmp_word))
					{
						return 0;
					}
					break;

				case 'D':
					if (!((*tmp_word >= 'a' && *tmp_word <= 'z') || (*tmp_word >= 'A' && *tmp_word <= 'Z')))
					{
						return 0;
					}
					break;

				default:
					break;
				}
				break;

			case '~':
				if (*tmp_word == *(tmp_pattern + 1))
				{
					return 0;
				}
				break;

			case '[':
				iterations = extract_iterations(tmp_pattern + 1, strstr(tmp_pattern, "*"));
				tmp_pattern = strstr(tmp_pattern, "(");
				pattern_length = count_pattern_length(tmp_pattern + 1, strstr(tmp_pattern, ")"));

				for (int i = 0; i < iterations; i++)
				{
					if (!(match(tmp_word, tmp_word + pattern_length, tmp_pattern + 1, strstr(tmp_pattern, ")"))))
					{
						return 0;
					}
					tmp_word += pattern_length;
				}
				brackets_count = 1;
				while (brackets_count)
				{
					if (*tmp_pattern == '[')
					{
						brackets_count++;
					}
					else if (*tmp_pattern == ']')
					{
						brackets_count--;
					}
					tmp_pattern++;
				}

				tmp_pattern -= 2;
				tmp_word--;
				break;

			default:
				break;
			}

			tmp_word++;
			tmp_pattern += 2;
		}
	}

	return 1;
}

//void print_word()
//{
//	char word[1024];
//	scanf("%s", word);
//	char* tmp = word;
//
//	while (*tmp)
//	{
//		printf("%c\n", *tmp);
//		tmp++;
//	}
//}

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
	int pattern_length = strlen(pattern);

	//printf("%d", count_pattern_length(pattern, pattern + strlen(pattern)));

	char** words = allocate_words(number);
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
		if (match(words[i], words[i] + strlen(words[i]), pattern, pattern + pattern_length))
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

	free_words(words, number);
	return 0;
}
