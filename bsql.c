#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>

#include "bsql.h"

char *input;
char *prompt = "db> ";


Command
parse_command(char *input)
{
  Command ret;

  if ((strncmp(".exit", input, 5) == 0)) {
      ret.type = COMMAND_EXIT;
  } else {
    ret.type = COMMAND_NOT_RECOGNIZED;
  }
  return ret;
}

void
execute_command(Command command) {
  switch(command.type) {
  case COMMAND_EXIT:
    exit(EXIT_SUCCESS);
    break;
  case COMMAND_NOT_RECOGNIZED:
    printf("Unrecognized command: %s\n", input);
    break;
  }
}

Statement
parse_statement(char *input)
{
  Statement ret;

  if((strncmp("select", input, 6) == 0)) {
    ret.type = SELECT_STATEMENT;
  } else if (strncmp("insert", input, 6) == 0) {
    ret.type = INSERT_STATEMENT;
  } else {
    ret.type = UNRECOGNIZED_STATEMENT;
  }

  return ret;
}

void
execute_statement(Statement statement) {
  switch (statement.type) {
      case SELECT_STATEMENT:
        printf("IOU a SELECT.\n");
        break;
      case INSERT_STATEMENT:
        printf("IOU an INSERT.\n");
        break;
      case UNRECOGNIZED_STATEMENT:
        printf("Unrecognized statement: %s\n", input);
        break;
      }
}

int
main(int argc, char *argv[])
{
  for(;;){
    input = readline(prompt);
    if (!input) perror("Couldn't read from the REPL.");

    if(input[0] == '.') {
      Command command = parse_command(input);
      execute_command(command);
    } else {
      Statement statement = parse_statement(input);
      execute_statement(statement);
      
        
    }
  }

  return EXIT_SUCCESS;
}
