//
// Created by mingtao on 5/17/17.
//

#ifndef DES_PARSER_MAIN_H
#define DES_PARSER_MAIN_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
/*----------------------------- to show the details-----------------------------------*/
struct _storage {
    char keys[16][19]; // 3 * 6 + 1 (\0)
    char states[20][25];
};
typedef struct _storage storage;
storage sto;

uint64_t K_list[16];
/*------------------------------- core ---------------------------------*/

void des_encrypt(uint64_t* bits);
void des_decrypt(uint64_t*);
uint32_t f(uint32_t R, uint64_t K);



/*------------------------------- interface ---------------------------------*/
void encrypt_num(char *input);
void decrypt_num(char *input);
void decode_file();
char* encode_file(char* file_path);
uint64_t char2bit(char *input);
uint32_t char2bit32(char *input);
void init_key(uint64_t key);
void f_computing(char* R, char* key);
void xor_computing(char* a, char* b);
void e_computing(char* a, char* b);
void sbox_computing(char* input, char* output);
uint64_t E(uint32_t input);
uint32_t S_box(uint64_t input);



uint8_t char2num(char ch);
uint32_t F(uint64_t input, uint64_t k);

uint64_t PC_2(int num);

void IP(uint64_t input);
uint64_t IP_inverse();
uint32_t loop_lmov(uint32_t input, int num);
uint64_t PC_22(int num);
uint32_t loop_rmov(uint32_t input, int num);
uint64_t decode(uint64_t bits);
uint64_t IP_inverse2();

void PC_1();


uint64_t encode(uint64_t);


#endif //DES_PARSER_MAIN_H
