#include<stdlib.h>

void swap(void *first, void *second, size_t size)
{
	size_t _size = size;
	char *_first = (char*)first, *_second = (char*)second, _tmp;
	do
	{
		_tmp = *_first;
		*_first++ = *_second;
		*_second++ = _tmp;
	} while (--_size > 0);
}
