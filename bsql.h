/* MetaCommands start with . */
enum meta_command_t {
  META_COMMAND_EXIT,
  META_COMMAND_UNRECOGNIZED_COMMAND
};
typedef enum meta_command_t meta_command_t;

enum statement_t {
  SELECT_STATEMENT,
  INSERT_STATEMENT,
  UNRECOGNIZED_STATEMENT
};
typedef enum statement_t statement_t;

struct Statement {
  statement_t type;
};
typedef struct Statement Statement;
