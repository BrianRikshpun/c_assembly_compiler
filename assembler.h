#ifndef ASSEMBLER_H
#define ASSEMBLER_H

void process_am_file(const char* filename);
void parse_elements(const char *opcode, const char *element1, const char *element2, int row, int code_type);

#endif