#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>

#include "bsql.h"

char *input;
char *prompt = "db> ";


meta_command_t
parse_command(char *input)
{
  if ((strncmp(".exit", input, 5) == 0)) {
      return META_COMMAND_EXIT;
    }
    return META_COMMAND_UNRECOGNIZED_COMMAND;
}

Statement
parse_statement(char *input)
{
  Statement ret;

  if((strncmp("select", input, 6) == 0)) {
    ret.type = PREPARE_SUCCESS;
  } else {
    ret.type = PREPARE_UNRECOGNIZE_STATEMENT;
  }

  return ret;
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
          printf("Unrecognized command: %s\n", input);
          break;
        }
    } else {
      Statement statement = parse_statement(input);
      switch (statement.type) {
      case PREPARE_SUCCESS:
        printf("Statement successfully recognized\n");
        break;
      case PREPARE_UNRECOGNIZE_STATEMENT:
        printf("Unrecognized statement: %s\n", input);
      }
        
    }
  }

  return EXIT_SUCCESS;
}
