#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct Row {
        size_t id;
        char username[COLUMN_USERNAME_SIZE];
        char email[COLUMN_EMAIL_SIZE];
} Row;

typedef struct Table {
        size_t num_rows;
        Row rows[100];
} Table;

enum command_t {
        COMMAND_EXIT,
        COMMAND_NOT_RECOGNIZED
};
typedef enum command_t command_t;

typedef struct Command {
        command_t type;
} Command;

enum statement_t {
        STATEMENT_SELECT,
        STATEMENT_INSERT,
        STATEMENT_NOT_RECOGNIZED
};
typedef enum statement_t statement_t;

typedef struct  Statement {
        statement_t type;
        Row row;
} Statement;
