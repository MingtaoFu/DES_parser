//
// Created by mingtao on 5/19/17.
//

#include "main.h"
char output_char[16];

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
    uint64_t r = char2bit32(input);
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
    print(bits);
}

void decode_file2() {

}


void decode_file() {
    char buf[8];
    FILE *file;
    FILE *file_output;

    long filesize;

    file_output = fopen("/home/mingtao/decoded", "w");

    file = (fopen("/home/mingtao/xxx", "r"));
    if (file == NULL) {
        printf("File opening error.\n");
    }

    fseek(file, 0L, SEEK_END);
    filesize = ftell(file);

    printf("%ld\n", filesize);

    fseek(file, -8L, SEEK_END);
    //fread(buf,  8 , 1,file);
    uint64_t a;
    fread(&a, 8, 1, file);
    printf("len_encoded：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(a >> (64 - i)) & 1);
    }
    printf("\n");
    uint64_t len_encoded = 0;
    for (int i = 0; i < 8; i++) {
        len_encoded <<= 8;
        len_encoded += (a >> (i * 8)) & 0xff;
    }
    printf("len_encoded：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(len_encoded >> (64 - i)) & 1);
    }
    printf("\n");
    //printf("%" PRIu64 "\n", decode(len_encoded));
    uint64_t original_bytes = decode(len_encoded) / 8;
    uint64_t unit_len = original_bytes / 8;
    uint8_t unit_len_extra = (uint8_t)(original_bytes % 8);

    fseek(file, 0L, SEEK_SET);
    uint64_t content;
    uint64_t content_decoded;
    for (uint64_t i = 0; i < unit_len; i++) {
        fread(&content, 8, 1, file);
        uint64_t content2 = 0;
        for (int j = 0; j < 8; j++) {
            content2 <<= 8;
            content2 += (content >> (j * 8)) & 0xff;
        }
        content_decoded = decode(content2);
        uint64_t content_decoded2 = 0;
        for (int j = 0; j < 8; j++) {
            content_decoded2 <<= 8;
            content_decoded2 += (content_decoded >> (j * 8)) & 0xff;
        }
        fwrite(&content_decoded2, 8, 1, file_output);
    }
    uint8_t content_decoded_8;
    fread(&content, 8, 1, file);
    //content_decoded = decode(content);
    uint64_t content2 = 0;
    for (int j = 0; j < 8; j++) {
        content2 <<= 8;
        content2 += (content >> (j * 8)) & 0xff;
    }
    content_decoded = decode(content2);
    uint64_t content_decoded2 = 0;
    for (int j = 0; j < 8; j++) {
        content_decoded2 <<= 8;
        content_decoded2 += (content_decoded >> (j * 8)) & 0xff;
    }
    for (uint8_t i = 0; i < unit_len_extra; i++) {
        content_decoded_8 = (uint8_t)((content_decoded >> ((7-i)*8)) & 0xff);
        fwrite(&content_decoded_8, 1, 1, file_output);
    }

    fclose(file);
    fclose(file_output);
}

char* encode_file(char* file_path) {
    char buf[8];
	FILE *file;

	FILE *file_output;


    file_output = fopen("/home/mingtao/xxx", "w");

    file = fopen("/home/mingtao/xxx", "r");


	/*
    if ((file = open(file_path, O_RDONLY, 0)) == -1) {
        printf("File opening error.\n");
    }
	*/
    ssize_t n;
    uint64_t content;
    uint64_t result;

    uint64_t total_len = 0;
    while ((n = fread(&content, 8, 1, file)) > 0) {

        ssize_t remain = 8 - n;
        /*
        for (int i = 0; i < remain; i++) {
            content = content << 8;
        }
         */

        uint64_t content2 = 0;
        for (int i = 0; i < 8; i++) {
            content2 <<= 8;
            content2 += (content >> (i * 8)) & 0xff;
        }
        des_encrypt(&content2);
        uint64_t result2 = 0;
        for (int i = 0; i < 8; i++) {
            result2 <<= 8;
            result2 += (content2 >> (i * 8)) & 0xff;
        }

        total_len += n * 8;

        fwrite(&result2, sizeof(result2), 1, file_output );

//        for (int i = 0; i < 16; i++) {
//            uint8_t num = (uint8_t)((result >> ((15 - i) * 4)) & 0xf);
//            sprintf(&output_char[i], "%x", num);
//        }

//        char* xx = (char*)malloc(sizeof(char) * total_length);
//        strcpy(xx, total_output);
//        strcat(xx, output_char);
//        free(total_output);
//        total_output = xx;
    }

    des_encrypt(&total_len);
    uint8_t len_encoded_byte;
    for (int i = 7; i >= 0; i--) {
        len_encoded_byte = (uint8_t)((total_len >> i * 8) & 0xff);
        fwrite(&len_encoded_byte, sizeof(len_encoded_byte), 1, file_output);
    }
    //printf("%" PRIu64 "\n", total_len);
    printf("len_encoded：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(total_len >> (64 - i)) & 1);
    }
    printf("\n");

    fclose(file);
    fclose(file_output);
}

void print(uint64_t bits) {
    char output_char[16];
    for (int i = 0; i < 16; i++) {
        uint8_t num = (uint8_t)((bits >> ((15 - i) * 4)) & 0xf);
        sprintf(&output_char[i], "%x", num);
    }
    printf(output_char);
}
