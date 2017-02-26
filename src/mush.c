#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <histedit.h>

#include <tokens.h>

char *prompt() {
    return "-> ";
}

int main(int argc, char **argv) {
    const char *line;
    int line_len = 0;

    EditLine *el = el_init(argv[0], stdin, stdout, stderr);
    el_set(el, EL_PROMPT, prompt);
    el_set(el, EL_EDITOR, "vi");

    History *hist = history_init();
    HistEvent he;
    history(hist, &he, H_SETSIZE, 511);
    el_set(el, EL_HIST, history, hist);

    line = el_gets(el, &line_len);
    while (line_len > 0) {
        if (line_len > 1) {
            history(hist, &he, H_ENTER, line);
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
        }
        line = el_gets(el, &line_len);
    }
    history_end(hist);
    el_end(el);
    exit(0);
}
