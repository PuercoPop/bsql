#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>

#include "bsql.h"

char *input;
char *prompt = "db> ";


meta_command_t
parse_command(char *cmd)
{
  if ((strncmp(".exit", cmd, 5) == 0)) {
      return META_COMMAND_EXIT;
    }
    return META_COMMAND_UNRECOGNIZED_COMMAND;
}

int
main(int argc, char *argv[])
{
  for(;;){
    input = readline(prompt);
    if (!input) perror("Couldn't read from the REPL.");

    if(input[0] == '.') {
      meta_command_t command = parse_command(input);
        switch(command) {
        case META_COMMAND_EXIT:
          return EXIT_SUCCESS;
          break;
        case META_COMMAND_UNRECOGNIZED_COMMAND:
          break;
        }
    }

    printf("Unrecognized command: %s\n", input);
  }

  return EXIT_SUCCESS;
}
