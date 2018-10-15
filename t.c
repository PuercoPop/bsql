#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

/***

The test runner.

When called without arguments it opens each file in the tests/ directory and
executes each script found there. Optionally it accepts PATHS... as
arguments. When passed any number of arguments it instead runs only the scripts
that where passed as arguments.

For each script it sends to the sub-process any line that starts with "> " and
reads every time it encounters a line that starts with "< ". Unless the read
string matches the contents of the line it returns immediately with an error
code. If the script reaches the EOF it exits successfully.

 ***/

static char buf[4096];
static char line[4096];

void die(char *msg, ...);
int read_prompt(int);
int read_line(int);
int run_script(char *, int, int);

void
die(char *msg, ...)
{
	va_list ap;

	va_start(ap, msg);
	vfprintf(stderr, msg, ap);
	va_end(ap);
	exit(EXIT_FAILURE);
}

int
read_prompt(int fd)
{
        char *prompt = "db> ";
        char buffer[5];
        ssize_t len;

        len = read(fd, buffer, 4);

        if (!(len == 4)) {
                die("READ_PROMPT: Read less characters than expected.\n");
        }

        return strncmp(buffer, prompt, 5);
}

int read_line(int fd) {
        int err;
        char next;

        for(int i = 0; i < 4096 - 1; i++) {
                err = read(fd, &next, 1);

                if (err < 0)
                        perror("read_line");

                if (err == 0) {
                        buf[i+1] = '\0';
                        break;
                }

                if (next == '\n') {
                        buf[i+1] = '\0';
                        return i;
                }
                buf[i] = next;
        }
        die("Couldn't read line. Ran out of buffer space.\n");
        return -1;
}

int
run_script(char *path, int in, int out)
{
        FILE *script = fopen(path, "r");

        while(fgets(line, 4096, script) != NULL)
        {
                if (!strncmp("> ", line, 2)) {
                        write(in, &line[2], strlen(&line[2]));
                } else if (!strncmp("< ", line, 2)) {
                        read_line(out);
                        int len = strlen(buf);
                        if (!(len == strlen(&line[2])))
                                die("Assertion violated");

                        if(!(strncmp(&line[2], buf, len)))
                                die("Assertion violated.\n");;
                        printf("EXPECTED OUTPUT.\n");
                } else {
                        die("Script is malformed.\n.");
                }
        }

        fclose(script);
        return 0;
}

int
main(int argc, char *argv[])
{
        pid_t pid;
        int in[2];
        int out[2];

        if (pipe(in) || pipe(out))
                die("Failed to create pipes.\n");

        pid = fork();
        if (pid == -1)
                die("Fork failed.\n");

        if (pid) {
		close(in[0]);
		close(out[1]);

                int child_stdin = in[1];
		int child_stdout = out[0];

                run_script("tests/smoke", child_stdin, child_stdout);
                /***
                    Example of how to communicate with the inferior process.
   		int child_stdin = in[1];
		int child_stdout = out[0];

                read_prompt(child_stdout);
                write(child_stdin, ".\n", 2);

                read_line(child_stdout);
                read_line(child_stdout);
                printf("Read: %s\n", buf);
                ***/

                return EXIT_SUCCESS;

        } else {
                close(in[1]);
		close(out[0]);

		dup2(in[0], STDIN_FILENO);
		dup2(out[1], STDOUT_FILENO);

                close(in[0]);
                close(out[1]);

                char *args[2] = { "", NULL};
                execvp("./bsql", args);
        }
}
