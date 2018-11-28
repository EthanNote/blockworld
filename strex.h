#pragma once
struct STRINGEX
{
	char* str;
	int length;
	int capacity;
};


void strex_init(struct STRINGEX* strex, const char* src);

void strex_resize(struct STRINGEX* strex, int new_size);

void strex_append(struct STRINGEX* strex, const char* src);

void strex_push_back(struct STRINGEX* strex, char c);

char strex_pop_back(struct STRINGEX* strex);

void strex_from_file(struct STRINGEX* strex, const char* fname);


