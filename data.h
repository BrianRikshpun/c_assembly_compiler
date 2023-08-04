#ifndef DATA_H
#define DATA_H

#define MAX_LABEL_LENGTH 50
#define LIST_LENGTH 100

typedef struct {
    char** array;
    int* row_ids;
    int size;
    int capacity;
} Labels;

typedef struct {
    char** array;
    int* row_ids;
    int size;
    int capacity;
} Externals;

typedef struct {
    char** array;
    int* row_ids;
    int size;
    int capacity;
} Internals;

extern short codes[];
extern short variables[];
extern Labels labels;
extern Externals externals;
extern Internals internals;
extern char* opcodes[];
extern int num_opcodes;
extern int codes_count;
extern int variables_count;


void allocate_labels();
void allocate_externals();
void allocate_internals();
void free_labels();
void free_externals();
void free_internals();
void add_label(const char* label, int row_id);
void add_external(const char* external);

#endif
