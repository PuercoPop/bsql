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

#define COLUMN_USERNAME_SIZE 32
#define COLUMN_EMAIL_SIZE 255

typedef struct {
  size_t id;
  char username[COLUMN_USERNAME_SIZE];
  char email[COLUMN_EMAIL_SIZE];
} Row;

typedef struct {
  void *pages[TABLE_MAX_PAGES];
    size_t num_rows;
} Table;

void serialize_row(Row*, void*);
void deserialize_row(void*, Row*);
