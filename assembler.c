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


char* remove_newline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
    return str;
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


short int element_type(char* str){

    if (str[0] == '@') {
            return 5;

    } else if (isdigit(str[0])) {
        return 1;
    } else {
        return 3;
    }
}

void remove_spaces_and_tabs(char *str) {
    int count = 0;
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ' && str[i] != '\t')
            str[count++] = str[i]; // Here count is incremented
    str[count] = '\0';
}


short get_opcode_index(const char *opcode) {
    for (short i = 0; i < NUM_OPCODES; i++) {
        if (strcmp(opcode, opcodes[i]) == 0) {
            return i;
        }
    }
    printf("Opcode not found: %s\n", opcode);
    return -1;
}

short element_number(const char* str) {
    if (str[0] == '@') {
        // The string starts with '@', so we return the last character as a number.
        return (short)atoi(str + 2);
    }
    else if (isdigit(str[0])) {
        // The string is a number, so we convert it to a short int and return it.
        return (short)atoi(str);
    }
    else {
        // We search for the string in the labels array and return the corresponding row_id.
        for (int i = 0; i < labels.size; i++) {
            if (strcmp(labels.array[i], str) == 0) {
                return (short)labels.row_ids[i];
            }
        }
    }
    // If the string is not found in the labels array, we return -1.
    return -1;
}


void add_code(short int* num){
    if(codes_count == LIST_LENGTH){
        printf("codes array is full! \n");
    }else{
        codes[codes_count] = *num;
        codes_count ++;
        printf("add code line! \n");
    }
    print_bits(codes[codes_count-1]);
}


void add_to_variables(short int num) {
    if(variables_count == LIST_LENGTH){
        printf("variables array is full! \n");
    }else{
        variables[variables_count] = num;
        variables_count ++;
        printf("add variable! \n");
    }
    print_bits(variables[variables_count-1]);
}


void data_parser(char *data) {
    // Split the string at each comma
    char *num_str = strtok(data, ",");
    while (num_str != NULL) {
        // Convert the number string to an integer
        int num = atoi(num_str);
        // Convert the integer to a short int and add it to the variables list
        add_to_variables((short) num);

        // Get the next number string
        num_str = strtok(NULL, ",");
    }
}



void string_parser(char *str) {
    // Remove the quotes from the string
    char* trimmed_str = strtok(str, "\"");

    // Iterate over each character in the string
    for (int i = 0; i < strlen(trimmed_str); i++) {
        // Convert the character to its ASCII value
        short ascii_val = (short) trimmed_str[i];
        // Call the add_string function with the ASCII value
        add_to_variables(ascii_val);
    }
}


short find_row_id_for_label(const char* label) {
    for (int i = 0; i < labels.size; i++) {
        if (strcmp(labels.array[i], label) == 0) {
            return labels.row_ids[i];
        }
    }
    // Return -1 if the label is not found
    return -1;
}


void parse_elements(const char *opcode, const char *element1, const char *element2, int code_type) {

    opcode = remove_newline(opcode);
    element1 = remove_newline(element1);
    element2 = remove_newline(element2);

    printf("new line from the file! \n");
    printf("the parameters of the line are : %s %s %s \n", opcode, element1, element2);


    if(strcmp(opcode, "stop") == 0){
        short int num = 0;
        replace_bits(&num, 0,3,0);
        replace_bits(&num, 4,11,15);
        add_code(&num);
    }
    else{
        short int num = 0;
        short int element1_number = 0;
        short int element2_number = 0;

        short int element1_type, element2_type, opcode_type;
        if(element1 == 0)
            element1_type = 0;
        else
            element1_type = element_type(element1);
        element1_number = element_number(element1);
        if(element2 == 0)
            element2_type = 0;
        else
            element2_type = element_type(element2);
        element2_number = element_number(element2);

        opcode_type = get_opcode_index(opcode);

        replace_bits(&num, 0,1,code_type);
        replace_bits(&num,2,4,element2_type);
        replace_bits(&num,5,8,opcode_type);
        replace_bits(&num,9,11,element1_type);
        add_code(&num);

        if (element1_type == 5 && element2_type == 5){
            short int num3 = 0;
            replace_bits(&num3, 2, 6, element2_number);
            replace_bits(&num3, 7, 11, element1_number);
            add_code(&num3);
        }
        else{
            if(element1_number != 0){
                short int num1 = 0;
                //Label adding
                if(is_all_uppercase(element1)){
                    short int label_row_id = find_row_id_for_label(element1);
                    replace_bits(&num1, 0,1,2);
                    replace_bits(&num1,2,11,label_row_id);
                    add_code(&num1);
                }
                else{
                    replace_bits(&num1, 2, 11, element1_number);
                    add_code(&num1);
                }
            }
            if(element2_number != 0){
                short int num2 = 0;
                replace_bits(&num2, 2, 11, element2_number);
                add_code(&num2);
            }
        }
    }

}

void process_am_file(const char* filename) {
    FILE *file;
    char line[LIST_LENGTH];
    char *temp_line;

    file = fopen(filename, "r");
    if(file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    int row = 0;
    while(fgets(line, sizeof(line), file)) {
        temp_line = line;
        char *word = strtok(temp_line, " ");
        while (word != NULL) {
            // Removing '\n' from the end of the word if it exists
            word[strcspn(word, "\n")] = 0;

            // Labels handling
            if (is_all_uppercase(word) && word[strlen(word) - 1] == ':') {
                word[strlen(word) - 1] = '\0';
                printf("added label %s \n",word);
                add_label(word, row + 100);
                word = strtok(NULL, " ");  // Get the next word
            }


            if (strcmp(word, ".data") == 0) {
                // If the word is ".data", call the data_parser function
                word = strtok(NULL, " ");
                data_parser(word);
                word = strtok(NULL, " ");
            }
            if (strcmp(word, ".string") == 0) {
                // If the word is ".string", call the string_parser function
                word = strtok(NULL, " ");
                string_parser(word);
                word = strtok(NULL, " ");
            }

            if (islower(word[0]) && get_opcode(word) != -1) {
                // Get the elements after the opcode
                char *element1 = strtok(NULL, ",");
                element1 = (element1 != NULL) ? element1 : "0";
                char *element2 = strtok(NULL, ",");
                element2 = (element2 != NULL) ? element2 : "0";

                // Parse the elements
                parse_elements(word, element1, element2,  0);
            }

            if (word == NULL) {
                // There were no more words on the line after the label
                break;
            }


            word = strtok(NULL, " ");
        }
        row++;
    }

    fclose(file);
}