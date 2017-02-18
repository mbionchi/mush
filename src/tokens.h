#ifndef _MUSH_TOKENS_H
#define _MUSH_TOKENS_H

#include <sys/types.h>

ssize_t next_token(char *str, char **tok, char delim);
char **all_tokens(char *str, char delim);

#endif
