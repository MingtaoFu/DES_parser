//
// Created by mingtao on 5/17/17.
//

#ifndef DES_PARSER_MAIN_H
#define DES_PARSER_MAIN_H

uint64_t char2bit(char *input);
uint8_t char2num(char ch);
uint32_t F(uint64_t input, uint64_t k);

uint64_t PC_2(int num);
uint64_t E(uint32_t input);
uint32_t left = 0;
uint32_t right = 0;
void IP(uint64_t input);
uint64_t IP_inverse();
uint32_t loop_lmov(uint32_t input, int num);
uint64_t PC_22(int num);
uint32_t loop_rmov(uint32_t input, int num);
uint64_t decode(uint64_t bits);
uint64_t IP_inverse2();
void encode_num(char* input);
void decode_file();

void PC_1();

char output_char[16];
uint64_t k;
uint32_t k_a;
uint32_t k_b;

uint64_t encode(uint64_t);
char* encode_file(char* file_path);


#endif //DES_PARSER_MAIN_H
