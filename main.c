#include "main.h"
#include <stdio.h>

int main(int argc, char **argv) {
    init_key(char2bit(argv[2]));

    if (!strcmp(argv[1], "-en")) {
        encrypt_num(argv[3]);
    } else if (!strcmp(argv[1], "-dn")) {
        decrypt_num(argv[3]);
    } else if (!strcmp(argv[1], "-ef")) {
        encrypt_file(argv[3], argv[4]);
    } else if (!strcmp(argv[1], "-df")) {

    } else {

    }
    return 0;
}
