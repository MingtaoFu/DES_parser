//
// Created by mingtao on 5/19/17.
//

#include "main.h"
#include "tables.h"
uint32_t left = 0;
uint32_t right = 0;
uint64_t k;
uint32_t k_a;
uint32_t k_b;

static void ip(uint64_t* input, uint32_t* left, uint32_t* right);
static void ip_inverse(uint64_t* output, uint32_t* left, uint32_t* right);
static void round(uint32_t* a, uint32_t* b, int i);
static void exchange(uint32_t* a, uint32_t* b);

void print_storage(char* container, uint64_t bits) {
    for (int i = 0; i < 8; i++) {
        uint8_t num = (uint8_t)((bits >> ((7 - i) * 8)) & 0xff);
        sprintf(&container[i*3], "%02x ", num);
    }
}
void print_storage2(char* container, uint32_t left, uint32_t right) {
    int i = 0;
    for (; i < 4; i++) {
        uint8_t num = (uint8_t)((left >> ((7 - i) * 8)) & 0xff);
        sprintf(&container[i*3], "%02x ", num);
    }
    for (; i < 8; i++) {
        uint8_t num = (uint8_t)((right >> ((7 - i) * 8)) & 0xff);
        sprintf(&container[i*3], "%02x ", num);
    }
}

void des_encrypt(uint64_t* bits) {
    print_storage(sto.states[0], *bits);

	uint32_t left, right;

	ip(bits, &left, &right);
    print_storage2(sto.states[1], left, right);

	for (int i = 0; i < 15; i++) {
		round(&left, &right, i);
        print_storage2(sto.states[i+2], left, right);
	}

    round(&left, &right, 15);
	exchange(&left, &right);
    print_storage2(sto.states[17], left, right);

	ip_inverse(bits, &left, &right);
    print_storage(sto.states[18], *bits);
    print_storage(sto.states[19], *bits);
}

void des_encrypt_file(uint64_t* bits) {
	uint32_t left, right;

	ip(bits, &left, &right);

	for (int i = 0; i < 15; i++) {
		round(&left, &right, i);
	}

    round(&left, &right, 15);
	exchange(&left, &right);

	ip_inverse(bits, &left, &right);
}

void des_decrypt(uint64_t* bits) {
    print_storage(sto.states[0], *bits);
    uint32_t left, right;
    ip(bits, &left, &right);
    print_storage2(sto.states[1], left, right);
    exchange(&left, &right);
    for (int i = 15; i >= 0; i--) {
        round(&right, &left, i);
        print_storage2(sto.states[17 - i], left, right);
    }
    ip_inverse(bits, &left, &right);
    print_storage(sto.states[18], *bits);
    print_storage(sto.states[19], *bits);
}

uint64_t decode(uint64_t bits) {
    k = char2bit("aaaaaaaaaaaaaaaa");
    PC_1();
    IP(bits);

    uint32_t tmp = left;
    left = right;
    right = tmp;

    for (int i = 0; i < 16; i++) {
    //    printf("--------------- 第 %d 轮迭代 -------------\n\n", i);

        uint32_t right1 = left;
        uint32_t left1 = right ^ F(E(left), PC_22(mov2[i]));

        left = left1;
        right = right1;
    }

    uint64_t output = IP_inverse2();
    return output;
}

static void ip(uint64_t* input, uint32_t* left, uint32_t* right) {
    /**
     * @todo 可见密钥那儿不需要合并
     */
    for (int i = 1; i <= 32; i++) {
        *left <<= 1;
        int target_index = IP_table[i - 1];
        *left += (*input >> (64 - target_index)) & 1;
    }

    for (int i = 33; i <= 64; i++) {
        *right <<= 1;
        int target_index = IP_table[i - 1];
        *right += (*input >> (64 - target_index)) & 1;
    }
}

static void ip_inverse(uint64_t* output, uint32_t* left, uint32_t* right) {
    *output = 0;
    uint64_t input = (uint64_t)(*left);
    input = (input << 32) + *right;

    /**
     * 实现查表转换功能
     */
    for (int i = 1; i <= 64; i++) {
        *output <<= 1;
        uint8_t target_index =  IP_inverse_table[i - 1];
        *output += (input >> (64 - target_index)) & 1;
    }
}



uint64_t encode(uint64_t bits) {
    k = char2bit("aaaaaaaaaaaaaaaa");
    PC_1();
    IP(bits);
    for (int i = 0; i < 16; i++) {
    //    printf("--------------- 第 %d 轮迭代 -------------\n\n", i);

        uint32_t left1 = right;
        uint32_t right1 = left ^ F(E(right), PC_2(mov[i]));

        left = left1;
        right = right1;
    }

    uint64_t output = IP_inverse();
    return output;
}

void PC_1() {
    uint64_t output = 0;
    for (int i = 0; i < 56; i++) {
        output = output << 1;
        uint8_t target_index =  PC_1_table[i];
        output += (k >> (64 - target_index)) & 1;
    }
    k_a = (uint32_t)((output >> 28) & 0xfffffff);
    k_b = (uint32_t)(output & 0xfffffff);
}

uint64_t PC_22(int num) {
    //将两个key合并
    uint64_t tmp = k_a;
    tmp = tmp << 28;
    tmp += k_b;
    uint64_t output = 0;
    for (int i = 1; i <= 48; i++) {
        output = output << 1;
        uint8_t target_index =  PC_2_table[i - 1];
        output += (tmp >> (56 - target_index)) & 1;
    }
    /**
     * 右移
     */
    k_a = loop_rmov(k_a, num);
    k_b = loop_rmov(k_b, num);
    return output;
}

uint64_t PC_2(int num) {
    /**
     * 先左移
     */
    k_a = loop_lmov(k_a, num);
    k_b = loop_lmov(k_b, num);
    //将两个key合并
    uint64_t tmp = k_a;
    tmp = tmp << 28;
    tmp += k_b;
    uint64_t output = 0;
    for (int i = 1; i <= 48; i++) {
        output = output << 1;
        uint8_t target_index =  PC_2_table[i - 1];
        output += (tmp >> (56 - target_index)) & 1;
    }
    return output;
}

uint32_t loop_rmov(uint32_t input, int num) {
    for (int i = 0; i < num; i++) {
        uint32_t tail = input & 1;
        input = (input >> 1) & 0xfffffff;
        input += tail << 27;
    }
    return input;
}

uint32_t loop_lmov(uint32_t input, int num) {
    for (int i = 0; i < num; i++) {
        uint32_t tail = (input >> 27) & 1;
        input = (input << 1) & 0xfffffff;
        input += tail;
    }
    return input;
}

uint8_t char2num(char ch) {
    if (ch >= 48 && ch <= 57) {
        // 数字
        return (uint8_t)(ch - 48);
    } else if (ch >= 97 && ch <= 102) {
        // 小写字母
        return (uint8_t)(ch - 87);
    } else if (ch >= 65 && ch <= 70) {
        // 大写字母
        return (uint8_t)(ch - 55);
    }  else {
        printf("输入有错误\n");
        return (uint8_t)-1;
    }
}


void IP(uint64_t input) {
    /**
     * 实现查表转换功能
     */
    for (int i = 1; i <= 32; i++) {
        left = left << 1;
        int target_index = IP_table[i - 1];
        left += (input >> (64 - target_index)) & 1;
    }

    for (int i = 33; i <= 64; i++) {
        right = right << 1;
        int target_index = IP_table[i - 1];
        right += (input >> (64 - target_index)) & 1;
    }
}

uint64_t IP_inverse2() {
    uint64_t input = (uint64_t)left;
    input = (input << 32) + right;
    uint64_t output = 0;

    /**
     * 实现查表转换功能
     */
    for (int i = 1; i <= 64; i++) {
        output = output << 1;
        uint8_t target_index =  IP_inverse_table[i - 1];
        output += (input >> (64 - target_index)) & 1;
    }
    return output;
}

uint64_t IP_inverse() {
    uint64_t input = (uint64_t)right;
    input = (input << 32) + left;
    uint64_t output = 0;

    /**
     * 实现查表转换功能
     */
    for (int i = 1; i <= 64; i++) {
        output = output << 1;
        uint8_t target_index =  IP_inverse_table[i - 1];
        output += (input >> (64 - target_index)) & 1;
    }
    return output;
}

uint64_t char2bit(char *input) {
    if (strlen(input) != CHARLEN) {
        printf("输入长度有误\n");
    }

    uint64_t output = 0;
    uint64_t bit;
    for (int i = 1; i <= CHARLEN; i++) {
        uint8_t num = char2num(input[i - 1]);
        for (int j = 0; j < 4; j++) {
            int mov = ((CHARLEN - i) * 4 + j);
            bit = (uint64_t)(num % 2) << mov;
            num /= 2;
            output += bit;
        }
    }
    return output;
}

uint32_t char2bit32(char *input) {
    if (strlen(input) != 8) {
        printf("输入长度有误\n");
    }

    uint32_t output = 0;
    uint32_t bit;
    for (int i = 1; i <= 8; i++) {
        uint8_t num = char2num(input[i - 1]);
        for (int j = 0; j < 4; j++) {
            int mov = ((8 - i) * 4 + j);
            bit = (uint32_t)(num % 2) << mov;
            num /= 2;
            output += bit;
        }
    }
    return output;
}

uint64_t E(uint32_t input) {
    uint64_t output = 0;
    for (int i = 1; i <= 48; i++) {
        output = output << 1;
        output += (input >> (32 - E_table[i - 1])) & 1;
    }
    return output;
}

/**
 * 测试通过
 *
 * uint8_t a[6] = {0, 0, 0, 0, 0, 0};
 * uint8_t b[6] = {0, 0, 0, 0, 0, 0};
 * 结果为每个表的第一个数
 *
 * uint8_t a[6] = {0, 0, 0, 0, 0, 0};
 * uint8_t b[6] = {255, 255, 255, 255, 255, 255};
 * 结果为每个表的最末一个数
 *
 * 48 Bits, 48 Bits
 * @param input
 * @param k
 */
uint32_t F(uint64_t input, uint64_t k) {
    //uint8_t xor[6];
    uint64_t xor_result = input ^ k;

    uint32_t output = 0;
    for (int i = 0; i < 8; i++) {
        output = output << 4;
        uint8_t index = (uint8_t)((xor_result >> 42) & 0x3f);
        output += S2[i][index];

        xor_result = xor_result << 6;
    }
    uint32_t output2 = 0;
    for (int i = 0; i < 32; i++) {
        output2 = output2 << 1;
        output2 += (output >> (32 - P[i])) & 1;
    }
    return output2;
}

/**
 * a, b 代表 left/right 中的某一个，
 * 没有写 left/right 是因为在加/解密时代表的不同
 * @param a
 * @param b
 * @param i 代表轮数
 */
static void round(uint32_t* a, uint32_t* b, int i) {
    uint32_t a1 = *b;
    uint32_t b1 = *a ^ f(*b, K_list[i]);

    *a = a1;
    *b = b1;
}

static void pc_1(uint64_t* key, uint32_t* k_a, uint32_t* k_b) {
    /**
     * @todo 直接用的以前代码
     */
    uint64_t output = 0;
    for (int i = 0; i < 56; i++) {
        output = output << 1;
        uint8_t target_index =  PC_1_table[i];
        output += (*key >> (64 - target_index)) & 1;
    }
    *k_a = (uint32_t)((output >> 28) & 0xfffffff);
    *k_b = (uint32_t)(output & 0xfffffff);
}

static uint64_t pc_2(uint32_t k_a, uint32_t k_b) {
    uint64_t tmp = k_a;
    tmp = tmp << 28;
    tmp += k_b;
    uint64_t output = 0;
    for (int i = 1; i <= 48; i++) {
        output = output << 1;
        uint8_t target_index =  PC_2_table[i - 1];
        output += (tmp >> (56 - target_index)) & 1;
    }
    return output;
}

void init_key(uint64_t key) {
    uint32_t k_a, k_b;

    pc_1(&key, &k_a, &k_b);

    for (int i = 0; i < 16; i++) {
        k_a = loop_lmov(k_a, mov[i]);
        k_b = loop_lmov(k_b, mov[i]);
        K_list[i] = pc_2(k_a, k_b);

        // 存储到storage中
        for (int j = 0; j < 6; j++) {
            sprintf(&sto.keys[i][j*3], "%02x ", (uint8_t)((K_list[i] >> ((5 - j) * 8)) & 0xff));
            printf("%02x ", (uint8_t)((K_list[i] >> ((5 - j) * 8)) & 0xff));
        }
        printf("\n");
    }
}

static void exchange(uint32_t* a, uint32_t* b) {
    /**
     * @todo 检查直接修改指针是否可行
     */
    uint32_t tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * @param R. 32bits input
 * @param K
 */
uint32_t f(uint32_t R, uint64_t K) {
    uint64_t xor_result = E(R) ^ K;

    /**
     * 下面是直接用的以前的
     */
    uint32_t output = S_box(xor_result);
    uint32_t output2 = 0;
    for (int i = 0; i < 32; i++) {
        output2 = output2 << 1;
        output2 += (output >> (32 - P[i])) & 1;
    }
    return output2;
}

uint32_t S_box(uint64_t input) {
    uint32_t output = 0;
    for (int i = 0; i < 8; i++) {
        output = output << 4;
        uint8_t index = (uint8_t)((input >> 42) & 0x3f);
        output += S2[i][index];

        input <<= 6;
    }
    return output;
}

void encrypt2(uint64_t* bits) {
    uint32_t left, right;
    ip(bits, &left, &right);
    for (int i = 0; i < 16; i++) {
        round(&left, &right, i);
    }
    exchange(&left, &right);
    ip_inverse(bits, &left, &right);
}
