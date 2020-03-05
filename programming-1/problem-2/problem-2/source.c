//#define _CRT_SECURE_NO_WARNINGS
//
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <ctype.h>
//#define PATTERN_SIZE 64
//#define WORD_SIZE 1024
//
//char** read_words(FILE* file, int number, char** words)
//{
//	for (int i = 0; i < number; i++)
//	{
//		fscanf(file, "%s", words[i]);
//	}
//
//	return words;
//}
//
//void free_words_array(char** words, int number)
//{
//	for (int i = 0; i < number; i++)
//	{
//		if (words[i])
//		{
//			free(words[i]);
//		}
//		else
//		{
//			break;
//		}
//	}
//
//	free(words);
//}
//
//char** allocate_words_array(int number)
//{
//	char** words = (char**)calloc(number, sizeof(char*));
//
//	if (!words)
//	{
//		return NULL;
//	}
//
//	for (int i = 0; i < number; i++)
//	{
//		words[i] = (char*)malloc(WORD_SIZE * sizeof(char));
//
//		if (!words[i])
//		{
//			free_words_array(words, number);
//			return NULL;
//		}
//	}
//
//	return words;
//}
//
//int extract_iterations(char* exp_start, char* exp_end)
//{
//	int iterations = 0;
//
//	while (exp_start != exp_end)
//	{
//		iterations *= 10;
//		iterations += *exp_start - '0';
//		exp_start++;
//	}
//
//	return iterations;
//}
//
//char* go_while_bracket_sequence_is_invalid(char *start, char start_bracket, char end_bracket)
//{
//	int brackets_count = 1;
//	char* reader = start;
//
//	while (brackets_count)
//	{
//		if (*reader == start_bracket)
//		{
//			brackets_count++;
//		}
//		else if (*reader == end_bracket)
//		{
//			brackets_count--;
//		}
//		reader++;
//	}
//
//	return reader;
//}
//
//int count_pattern_length(char* pattern_start, char* pattern_end)
//{
//	int length = 0;
//	char *reader = pattern_start, *tmp_reader;
//
//	while (reader != pattern_end)
//	{
//		if (*reader == '[')
//		{
//			tmp_reader = go_while_bracket_sequence_is_invalid(reader + 1, '[', ']');
//			length += extract_iterations(reader + 1, strstr(reader, "*")) * count_pattern_length(strstr(reader, "("), go_while_bracket_sequence_is_invalid(strstr(reader, "(") + 1, '(', ')'));
//			reader++;
//
//			reader = tmp_reader;
//			continue;
//		}
//
//		if (*reader != '\\' && *reader != '(' && *reader != '~' && *reader != ')' && *reader != ']')
//		{
//			length++;
//		}
//
//		reader++;
//	}
//
//	return length;
//}
//
//int match(char* word_start, char* word_end, char* pattern_start, char* pattern_end)
//{
//	if (count_pattern_length(pattern_start, pattern_end) != word_end - word_start)
//	{
//		return 0;
//	}
//
//	char* word_reader = word_start, *pattern_reader = pattern_start, *subpattern_start, *subpattern_end;
//	int iterations, pattern_length, brackets_count;
//
//	while (word_reader != word_end && pattern_reader != pattern_end)
//	{
//		if (*pattern_reader != '[' && *pattern_reader != '~' && *pattern_reader != '\\')
//		{
//			if (*pattern_reader != *word_reader)
//			{
//				return 0;
//			}
//
//			pattern_reader++;
//			word_reader++;
//		}
//		else
//		{
//			switch (*pattern_reader)
//			{
//			case '\\':
//				switch (*(pattern_reader + 1))
//				{
//				case 'd':
//					if (!isdigit(*word_reader))
//					{
//						return 0;
//					}
//					break;
//
//				case 'D':
//					if (!isalpha(*word_reader))
//					{
//						return 0;
//					}
//					break;
//
//				default:
//					break;
//				}
//				break;
//
//			case '~':
//				if (*word_reader == *(pattern_reader + 1))
//				{
//					return 0;
//				}
//				break;
//
//			case '[':
//				iterations = extract_iterations(pattern_reader + 1, strstr(pattern_reader, "*"));
//				subpattern_start = strstr(pattern_reader, "(") + 1;
//				subpattern_end = go_while_bracket_sequence_is_invalid(subpattern_start, '(', ')') - 1;
//
//				pattern_length = count_pattern_length(subpattern_start, subpattern_end);
//
//				for (int i = 0; i < iterations; i++)
//				{
//					if (!match(word_reader, word_reader + pattern_length, subpattern_start, subpattern_end))
//					{
//						return 0;
//					}
//
//					word_reader += pattern_length;
//				}
//
//				pattern_reader++;
//
//				pattern_reader = go_while_bracket_sequence_is_invalid(pattern_reader, '[', ']');
//
//				pattern_reader -= 2;
//				word_reader--;
//				break;
//
//			default:
//				break;
//			}
//
//			word_reader++;
//			pattern_reader += 2;
//		}
//	}
//
//	return 1;
//}
//
////int main()
////{
////	char pattern[PATTERN_SIZE];
////	FILE* input, *output;
////	input = fopen("input.txt", "r");
////
////	if (!input)
////	{
////		printf("Can't open input.txt");
////		return -1;
////	}
////
////	int number, matches_count = 0;
////	fscanf(input, "%s", pattern);
////	fscanf(input, "%d", &number);
////
////	char* pattern_end = pattern + strlen(pattern);
////	char** words = allocate_words_array(number);
////
////	if (!words)
////	{
////		printf("Can't allocate memory for words array");
////		return -1;
////	}
////
////	words = read_words(input, number, words);
////
////	fclose(input);
////
////	output = fopen("output.txt", "w");
////
////	if (!output)
////	{
////		printf("Can't open output.txt");
////		return -1;
////	}
////
////	for (int i = 0; i < number; i++)
////	{
////		if (match(words[i], words[i] + strlen(words[i]), pattern, pattern_end))
////		{
////			fprintf(output, "%d ", i);
////			matches_count++;
////		}
////	}
////
////	if (!matches_count)
////	{
////		fprintf(output, "none");
////	}
////	fclose(output);
////
////	free_words_array(words, number);
////	return 0;
////}
