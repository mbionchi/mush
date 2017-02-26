#ifndef _MUSH_TOKENS_H
#define _MUSH_TOKENS_H

#include <sys/types.h>

ssize_t next_token(const char *str, char **tok, char delim);
char **all_tokens(const char *str, char delim);

#endif
