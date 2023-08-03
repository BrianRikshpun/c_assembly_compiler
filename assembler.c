#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "data.h"
#include "assembler.h"


void print_bits(short num) {
    int size = sizeof(num) * 8;  // Calculate the number of bits in num
    for(int i = size - 1; i >= 0; i--) {
        printf("%d", (num >> i) & 1);  // Shift num i bits to the right and check the last bit
    }
    printf("\n");
}


int is_all_uppercase(const char *str) {
    for (int i = 0; str[i]; i++) {
        if (!isupper(str[i]) && str[i] != ':') {
            return 0;
        }
    }
    return 1;
}

int get_opcode(const char *opcode) {
    for (int i = 0; i < num_opcodes; i++) {
        if (strcmp(opcode, opcodes[i]) == 0) {
            return i;
        }
    }
    return -1;  // Opcode not found
}

void add_to_labels_or_externals(const char *element, int row) {
    // Check if the element is in the labels array
    for (int i = 0; i < labels.size; i++) {
        if (strcmp(labels.array[i], element) == 0) {
            // The element is a label
            codes[row] = (i & 0x3FF) | (3 << 12);
            return;
        }
    }
    // The element is an label variable
    add_label(element, row);
}


void replace_bits(short int *x, int start, int end, short y) {
    // Create a mask with 1's in the bits to replace
    short mask = ((1 << (end - start + 1)) - 1) << start;

    // Clear the bits to replace in x
    *x &= ~mask;

    // Shift y to the correct position and combine it with x
    *x |= (y << start) & mask;

}


short int element_type(const char* str){

    if (str[0] == '@') {
            return 5;

    } else if (isdigit(str[0])) {
        return 1;
    } else {
        return 3;
    }
}


short get_opcode_index(const char *opcode) {
    int num_opcodes = sizeof(*opcodes) / sizeof(*opcodes[0]);
    for (short i = 0; i < num_opcodes; i++) {
        if (strcmp(opcode, opcodes[i]) == 0) {
            return i;
        }
    }
    printf("Opcode not found: %s\n", opcode);
    return -1;
}
void parse_elements(const char *opcode, const char *element1, const char *element2, int row, int code_type) {


    short int num = 0;

    short int element1_type, element2_type, opcode_type;
    if(element1 == 0)
        element1_type = 0;
    else
        element1_type = element_type(element1);
    if(element2 == 0)
        element2_type = 0;
    else
        element2_type = element_type(element2);

    opcode_type = get_opcode_index(opcode);


    replace_bits(&num, 0,1,code_type);
    replace_bits(&num,2,4,element2_type);
    replace_bits(&num,5,8,opcode_type);
    replace_bits(&num,9,11,element1_type);
    print_bits(num);
    printf("s");



}

void process_am_file(const char* filename) {
    FILE *file;
    char line[100];

    file = fopen(filename, "r");
    if(file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    int row = 0;
    while(fgets(line, sizeof(line), file)) {
        char *word = strtok(line, " ");
        while (word != NULL) {
            if (is_all_uppercase(word) && word[strlen(word) - 1] == ':') {
                printf("added label %s \n",word);
                add_label(word, row + 100);
                // Once a label has been found, get the next word
                word = strtok(NULL, " ");
            }

            // Now 'word' should contain the opcode
            if (islower(word[0]) && get_opcode(word) != -1) {
                // Get the elements after the opcode
                char *element1 = strtok(NULL, ",");
                if (element1 == NULL) {
                    element1 = "0";
                }
                char *element2 = strtok(NULL, ",");
                if (element2 == NULL) {
                    element2 = "0";
                }

                // Parse the elements
                parse_elements(word, element1, element2, row, 0);
            }
            word = strtok(NULL, " ");
        }
        row++;
    }

    fclose(file);
}
