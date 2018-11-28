#pragma once

#include "strex.h"
#include<string.h>
#include<stdlib.h>

void strex_init(struct STRINGEX* strex, const char* src) {
	strex->length = 0;
	strex->capacity = 32;
	if (src) {
		strex->length = strlen(src);
		strex->capacity += strex->length;
	}
	strex->str = calloc(strex->capacity, 1);
	if (src) {
		strcpy(strex->str, src);
	}


}

void strex_resize(struct STRINGEX* strex, int new_size) {
	if (new_size > strex->capacity) {
		strex->str = realloc(strex->str, new_size);
	}
}

void strex_append(struct STRINGEX* strex, const char* src);

void strex_push_back(struct STRINGEX* strex, char c);

char strex_pop_back(struct STRINGEX* strex);

void strex_from_file(struct STRINGEX* strex, const char* fname);