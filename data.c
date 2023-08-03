#include <stdlib.h>
#include <string.h>
#include "data.h"

short codes[50];
short variables[50];

Labels labels = {NULL, NULL, 0, 0};
Externals externals = {NULL, 0, 0};
Internals internals = {NULL, 0, 0};

char* opcodes[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};
int num_opcodes = sizeof(opcodes) / sizeof(opcodes[0]);

void allocate_labels() {
    labels.array = (char**) malloc(sizeof(char*));
    labels.row_ids = (int*) malloc(sizeof(int));
    labels.capacity = 1;
}

void allocate_externals() {
    externals.array = (char**) malloc(sizeof(char*));
    externals.row_ids = (int*) malloc(sizeof(int));
    externals.capacity = 1;
}

void allocate_internals() {
    internals.array = (char**) malloc(sizeof(char*));
    internals.row_ids = (int*) malloc(sizeof(int));
    internals.capacity = 1;
}

void free_labels() {
    for (int i = 0; i < labels.size; i++) {
        free(labels.array[i]);
    }
    free(labels.array);
    free(labels.row_ids);
}

void free_externals() {
    for (int i = 0; i < externals.size; i++) {
        free(externals.array[i]);
    }
    free(externals.array);
    free(externals.row_ids);
}

void free_internals() {
    for (int i = 0; i < internals.size; i++) {
        free(internals.array[i]);
    }
    free(internals.array);
    free(internals.row_ids);
}

void add_label(const char* label, int row_id) {
    if (labels.size == labels.capacity) {
        labels.capacity *= 2;
        labels.array = (char**) realloc(labels.array, labels.capacity * sizeof(char*));
        labels.row_ids = (int*) realloc(labels.row_ids, labels.capacity * sizeof(int));
    }
    labels.array[labels.size] = (char*) malloc((strlen(label) + 1) * sizeof(char));
    strcpy(labels.array[labels.size], label);
    labels.row_ids[labels.size] = row_id;
    labels.size++;
}

void add_externals(const char* external, int row_id) {
    if (externals.size == externals.capacity) {
        externals.capacity *= 2;
        externals.array = (char**) realloc(labels.array, labels.capacity * sizeof(char*));
        externals.row_ids = (int*) realloc(labels.row_ids, labels.capacity * sizeof(int));
    }
    labels.array[externals.size] = (char*) malloc((strlen(external) + 1) * sizeof(char));
    strcpy(externals.array[labels.size], external);
    externals.row_ids[externals.size] = row_id;
    externals.size++;
}

void add_internals(const char* internal, int row_id) {
    if (internals.size == internals.capacity) {
        internals.capacity *= 2;
        internals.array = (char**) realloc(internals.array, internals.capacity * sizeof(char*));
        internals.row_ids = (int*) realloc(internals.row_ids, internals.capacity * sizeof(int));
    }
    internals.array[internals.size] = (char*) malloc((strlen(internal) + 1) * sizeof(char));
    strcpy(internals.array[internals.size], internal);
    internals.row_ids[internals.size] = row_id;
    internals.size++;
}