#include <tokens.h>

#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

static const size_t alloc_nmemb = 18;

ssize_t next_token(char *str, char **tok, char delim) {
    ssize_t offset = 0;
    if (str[offset] == '\0') {
        offset = -1;
    } else {
        ssize_t length = 0;
        while (str[offset] == delim) {
            offset++;
        }
        while (str[offset] != delim && str[offset] != '\0') {
            offset++;
            length++;
        }
        offset++;
        *tok = strncpy(malloc((length+1)*sizeof(char)), str+(offset-length-1), length+1);
        (*tok)[length] = '\0';
    }
    return offset;
}

char **all_tokens(char *str, char delim) {
    size_t tokens_n = alloc_nmemb;
    char **tokens = malloc(tokens_n*sizeof(char*));
    char *token;
    int i = 0;
    ssize_t last_offset = next_token(str, &token, delim);
    ssize_t total_offset = last_offset;
    while (last_offset >= 0) {
        if (i >= tokens_n) {
            tokens_n += alloc_nmemb;
            tokens = realloc(tokens, tokens_n*sizeof(char*));
        }
        tokens[i] = token;
        i++;
        last_offset = next_token(str+total_offset, &token, delim);
        total_offset += last_offset;
    }
    if (i >= tokens_n ) {
        tokens = realloc(tokens, (tokens_n+1)*sizeof(char*));
    }
    tokens[i] = NULL;
    return tokens;
}
