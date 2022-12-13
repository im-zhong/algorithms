#ifndef __KMP__
#define __KMP__

#include <stddef.h>

void calculate_prefix(const char* pattern, size_t npattern, size_t* prefix);
void kmp(const char* pattern, size_t npattern, const char* text, size_t ntext);
void native_strmatch(const char* text, size_t ntext, const char* pattern,
                     size_t npattern);
#endif // __KMP__
