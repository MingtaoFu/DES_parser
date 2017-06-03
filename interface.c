//
// Created by mingtao on 5/19/17.
//

#include <netinet/in.h>
#include "main.h"

void print(uint64_t bits);

void f_computing(char* R, char* key) {
    uint32_t result = f(char2bit32(R), char2bit(key));
    for (int i = 0; i < 8; i++) {
		uint8_t num = (uint8_t)((result >> ((7 - i) * 4)) & 0xf);
		sprintf(&R[i], "%x", num);
	}
}

void xor_computing(char* a, char* b) {
    uint32_t result = char2bit32(a) ^ char2bit32(b);
    for (int i = 0; i < 8; i++) {
		uint8_t num = (uint8_t)((result >> ((7 - i) * 4)) & 0xf);
		sprintf(&a[i], "%x", num);
	}
}

void e_computing(char* R, char* output) {
    uint32_t r = char2bit32(R);
    uint64_t result = E(r);
    for (int i = 0; i < 12; i++) {
		uint8_t num = (uint8_t)((result >> ((11 - i) * 4)) & 0xf);
		sprintf(&output[i], "%x", num);
	}
}

void sbox_computing(char* input, char* output) {
    uint64_t r = char2bit(input);
    uint32_t output_num = S_box(r);
    for (int i = 0; i < 8; i++) {
		uint8_t num = (uint8_t)((output_num >> ((7 - i) * 4)) & 0xf);
		sprintf(&output[i], "%x", num);
	}
}

void encrypt_num(char *input) {
    uint64_t bits = char2bit(input);
    des_encrypt(&bits);

	for (int i = 0; i < 16; i++) {
		uint8_t num = (uint8_t)((bits >> ((15 - i) * 4)) & 0xf);
		sprintf(&input[i], "%x", num);
	}
}

void decrypt_num(char *input) {
    uint64_t bits = char2bit(input);
    des_decrypt(&bits);
    for (int i = 0; i < 16; i++) {
		uint8_t num = (uint8_t)((bits >> ((15 - i) * 4)) & 0xf);
		sprintf(&input[i], "%x", num);
	}
}

uint64_t ntohl64(uint64_t host) {
    uint64_t ret = 0;
    uint32_t high,low;
    low = (uint32_t)(host & 0xFFFFFFFF);
    high = (uint32_t)((host >> 32) & 0xFFFFFFFF);
    low = ntohl(low);
    high = ntohl(high);
    ret = low;
    ret <<= 32;
    ret |= high;
    return ret;
}

void decrypt_file(char* in, char* out) {
    remove(out);

    // Define the files and then open them
    FILE *file;
    FILE *file_output;
    file = (fopen(in, "r"));
    file_output = fopen(out, "w");

    long filesize;

    if (file == NULL) {
        printf("File opening error.\n");
    }

    // get file size
    fseek(file, 0, SEEK_END);
    filesize = ftell(file);

    fseek(file, 0, SEEK_SET);

    uint64_t flag1, flag2, flag3, flag;
    fread(&flag1, 1, 8, file);
    fread(&flag2, 1, 8, file);
    fread(&flag3, 1, 8, file);

    if (flag1 == flag2 || flag1 == flag3) {
        flag = flag1;
    } else if (flag2 == flag3){
        flag = flag2;
    } else {
        /**
         * @todo throw error
         */
        printf("error1\n");
        return;
    }

    flag = ntohl64(flag);

    des_decrypt_file(&flag);

    uint64_t len = flag / 8;
    uint64_t remain = 8 - len % 8;

    if (len + 24 + remain != filesize) {
        /**
         * @todo throw error
         */
        printf("error2\n");
        return;
    }

    uint64_t unit_len = len / 8;
    uint8_t unit_len_extra = (uint8_t)(len % 8);

    uint64_t content;
    for (uint64_t i = 0; i < unit_len; i++) {
        fread(&content, 1, 8, file);
        content = ntohl64(content);

        des_decrypt_file(&content);

        content = ntohl64(content);

        fwrite(&content, 1, 8, file_output);
    }
    uint8_t content_decoded_8;
    fread(&content, 1, 8, file);

    content = ntohl64(content);

    des_decrypt_file(&content);

    /**
     * The reason why I not use ntohl64 here is:
     * Following data are written by byte.
     */
    for (uint8_t i = 0; i < unit_len_extra; i++) {
        content_decoded_8 = (uint8_t)((content >> ((7-i)*8)) & 0xff);
        fwrite(&content_decoded_8, 1, 1, file_output);
    }

    fclose(file);
    fclose(file_output);
}


char* encrypt_file(char *file_in, char *file_out) {
    // Define the file handler and then open them
	FILE *file;
	FILE *file_output;

    // remove the output file if it exists
    remove(file_out);

    file = fopen(file_in, "r");
    file_output = fopen(file_out, "w");

    // reserve the space for flag
    fseek(file_output, 24, SEEK_CUR);

    ssize_t n;
    uint64_t content;

    // Record the length of plaintext
    uint64_t total_len = 0;

    while ((n = fread(&content, 1, 8, file)) > 0) {
        if (n < 8) {
            // fill zeros
            total_len += n * 8;
            content = content << (64 - n * 8) >> (64 - n * 8);
        } else {
            total_len += 64;
        }

        // change the order
        content = ntohl64(content);
        des_encrypt_file(&content);

        uint64_t result = ntohl64(content);
        fwrite(&result, sizeof(result), 1, file_output);
    }

    des_encrypt_file(&total_len);
    total_len = ntohl64(total_len);

    // fill the flags
    fseek(file_output, 0, SEEK_SET);
    fwrite(&total_len, 8, 1, file_output);
    fwrite(&total_len, 8, 1, file_output);
    fwrite(&total_len, 8, 1, file_output);

    fclose(file);
    fclose(file_output);
    return NULL;
}
