#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

#include <tokens.h>

#define PROMPT "-> "

int main(int argc, char **argv) {
    char *line = NULL;
    size_t line_len = 0;
    ssize_t n_read = 0;
    printf("%s", PROMPT);
    n_read = getline(&line, &line_len, stdin);
    while (n_read >= 0) {
        line[n_read-1] = '\0';
        char **tokens = all_tokens(line, ' ');
        pid_t child_pid = fork();
        if (child_pid < 0) {
            perror("fork");
            errno = 0;
        } else if (child_pid == 0) {
            if (execv(tokens[0], tokens) != 0) {
                perror("execv");
                exit(1);
            }
        } else {
            int status;
            pid_t wpid;
            wpid = wait(&status);
            while (wpid != child_pid) {
                wpid = wait(&status);
            }
            char **iter;
            for (iter = tokens; *iter != NULL; iter++) {
                free(*iter);
            }
            free(tokens);
        }
        free(line);
        line = NULL;
        line_len = 0;
        printf("%s", PROMPT);
        n_read = getline(&line, &line_len, stdin);
    }
    exit(0);
}
