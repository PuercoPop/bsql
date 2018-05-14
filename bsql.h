/* Commands start with . */
enum command_t {
  COMMAND_EXIT,
  COMMAND_NOT_RECOGNIZED
};
typedef enum command_t command_t;

typedef struct {
  command_t type;
} Command;

enum statement_t {
  STATEMENT_SELECT,
  STATEMENT_INSERT,
  STATEMENT_NOT_RECOGNIZED
};
typedef enum statement_t statement_t;

typedef struct {
  statement_t type;
} Statement;
