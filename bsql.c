#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>

char *input;
char *prompt = "db> ";

int
main(int argc, char *argv[])
{
  input = readline(prompt);
  if (!input) perror("Couldn't read prompt.");
  
  while(!(strncmp(".exit", input, 5) == 0)) {
    printf("Unrecognized command: %s\n", input);

    input = readline(prompt);
    if (!input) perror("Couldn't read prompt.");
  }

  return EXIT_SUCCESS;
}
