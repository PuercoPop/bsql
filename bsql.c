#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "bsql.h"

static struct sigaction sa;

static char *input;
static char *prompt = "db> ";

static Table *table;

static char *history_file;

void
die(char *msg, ...)
{
        va_list ap;

        va_start(ap, msg);
        vfprintf(stderr, msg, ap);
        va_end(ap);
        exit(1);
}

void
sigint(const int signo)
{
        write_history(history_file);
        exit(0);
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
                write_history(history_file);
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
        size_t row_id;

        switch (statement.type) {
        case STATEMENT_SELECT:
                row_id = statement.row.id;

                if(row_id > table->num_rows) {
                        printf("No such Row\n");
                        break;
                }

                printf("ID: %zu\n", table->rows[row_id].id);
                printf("USERNAME: %s\n", table->rows[row_id].username);
                printf("EMAIL: %s\n", table->rows[row_id].email);
                break;
        case STATEMENT_INSERT:
                if (table->num_rows >= 100)
                        die("Cannot allocate more rows.\n");

                table->num_rows++;

                memcpy(&table->rows[table->num_rows],
                       &statement.row.id,
                       sizeof(size_t));
                memcpy(&table->rows[table->num_rows].username,
                       &statement.row.username,
                       COLUMN_USERNAME_SIZE);
                memcpy(&table->rows[table->num_rows].email,
                       &statement.row.email,
                       COLUMN_EMAIL_SIZE);
                printf("ACK. Current number of Rows: %zu\n", table->num_rows);
                break;
        case STATEMENT_NOT_RECOGNIZED:
                printf("Unrecognized statement: %s\n", input);
                break;
        }
}

void
setup()
{
        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = sigint;
        if(sigaction(SIGINT, &sa, NULL))
                die("Could not setup SIGINT handler.\n");

        const char *home = getenv("HOME");
        if(!home || !*home) {
                die("$HOME not set.\n");
        }

        size_t history_file_len = snprintf(NULL, 0, "%s/.bsql_history", home);
        history_file = malloc(history_file_len);

        snprintf(history_file, history_file_len, "%s/.bsql_history", home);

        table = calloc(1, sizeof(Table));
        if (table  == NULL)
                die("Could not allocate table.\n");
        table->num_rows = 0;

        using_history();
        read_history(history_file);
}

int
main(int argc, char *argv[])
{
        setup();
        for(;;){
                input = readline(prompt);
                add_history(input);
                if (!input) die("Couldn't read from the REPL.\n");

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
