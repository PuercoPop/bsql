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

enum COMMAND_TYPE {
        COMMAND_EXIT,
        COMMAND_NOT_RECOGNIZED
};
typedef enum COMMAND_TYPE COMMAND_TYPE;

typedef struct Command {
        COMMAND_TYPE type;
} Command;

enum STATEMENT_TYPE {
        STATEMENT_SELECT,
        STATEMENT_INSERT,
        STATEMENT_NOT_RECOGNIZED
};
typedef enum STATEMENT_TYPE STATEMENT_TYPE;

typedef struct  Statement {
        STATEMENT_TYPE type;
        Row row;
} Statement;
