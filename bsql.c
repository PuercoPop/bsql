#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "bsql.h"

char *input;
char *prompt = "db> ";

#define PATH_MAX 4096
char *history_file = "";

char *
get_history_file()
{
  if(history_file[0] != '\0')
    return history_file;

  const char *home = getenv("HOME");
  if(!home || !*home) {
    perror("$HOME not set");
  }

  size_t history_file_len = snprintf(NULL, 0, "%s/.bsql_history", home);
  history_file = malloc(history_file_len + 1);

  snprintf(history_file, history_file_len + 1, "%s/.bsql_history", home);
  return history_file;
}

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
    write_history(get_history_file());
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
    char *keyword = strtok(input, " ");
    char *id_string = strtok(NULL, " ");

    if(!(strncmp("select", keyword, 6) == 0))
      goto statement_error;

    if (id_string == NULL)
      goto statement_error;

    // Reuse ROW.ID to select 1 row for the time being
    int row_id = atoi(id_string);
    if (row_id < 0)
      goto statement_error;

    ret.row.id = row_id;
    ret.type = STATEMENT_SELECT;
  } else if (strncmp("insert", input, 6) == 0) {
    char *keyword = strtok(input, " ");
    char *id_string = strtok(NULL, " ");
    char *username = strtok(NULL, " ");
    char *email = strtok(NULL, " ");

    if(!(strncmp("insert", keyword, 6) == 0))
      goto statement_error;

    if (id_string == NULL || username == NULL || email == NULL)
      goto statement_error;

    int row_id = atoi(id_string);
    if (row_id < 0)
      goto statement_error;

    ret.row.id = row_id;
    strcpy(ret.row.username, username);
    strcpy(ret.row.email, email);


    ret.type = STATEMENT_INSERT;
  } else {
    ret.type = STATEMENT_NOT_RECOGNIZED;
  }

  return ret;
 statement_error:
  ret.type = STATEMENT_NOT_RECOGNIZED;
  return ret;
}

void
execute_statement(Statement statement) {
  switch (statement.type) {
  case STATEMENT_SELECT:
    printf("IOU a SELECT.\n");
    break;
  case STATEMENT_INSERT:
    printf("IOU an INSERT.\n");
    break;
  case STATEMENT_NOT_RECOGNIZED:
    printf("Unrecognized statement: %s\n", input);
    break;
  }
}

int
main(int argc, char *argv[])
{
  using_history();
  read_history(get_history_file());
  for(;;){
    input = readline(prompt);
    add_history(input);
    if (!input) perror("Couldn't read from the REPL.");

    if(input[0] == '.') {
      Command command = parse_command(input);
      execute_command(command);
    } else {
      Statement statement = parse_statement(input);
      execute_statement(statement);
    }

    free(input);
  }

  return EXIT_SUCCESS;
}
