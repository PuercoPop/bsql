/* Commands start with . */
enum command_t {
  COMMAND_EXIT,
  COMMAND_NOT_RECOGNIZED
};
typedef enum command_t command_t;

struct Command {
  command_t type;
};
typedef struct Command Command;

enum statement_t {
  STATEMENT_SELECT,
  STATEMENT_INSERT,
  STATEMENT_NOT_RECOGNIZED
};
typedef enum statement_t statement_t;

struct Statement {
  statement_t type;
};
typedef struct Statement Statement;
