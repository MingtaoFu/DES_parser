#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unitypes.h>
#include <gtk/gtk.h>
#include <fcntl.h>

#include "tables.h"

#define CHARLEN 16

uint64_t char2bit(char *input);
uint8_t char2num(char ch);
uint32_t F(uint64_t input, uint64_t k);

void E2(uint32_t input);
uint64_t PC_2(int num);
uint64_t E(uint32_t input);
uint32_t left = 0;
uint32_t right = 0;
void IP(uint64_t input);
uint64_t IP_inverse();
uint8_t xx[6];
uint32_t loop_lmov(uint32_t input, int num);
uint64_t PC_22(int num);
uint32_t loop_rmov(uint32_t input, int num);
uint64_t decode(uint64_t bits);
uint64_t IP_inverse2();
void encode_num(char* input);

int mov[] = {
        1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

int mov2[] = {
        1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1, 1
};

void PC_1();

char output_char[16];
uint64_t k;
uint32_t k_a;
uint32_t k_b;

uint64_t encode(uint64_t);
char* encode_file(char* file_path);

GtkWidget *window,  *encode_btn, *decode_btn, *file_encode_btn;
GtkEntry *input, *output, *file, *file_output;

static void
print_hello (GtkWidget *widget,
             gpointer   data)
{
    encode_num(gtk_entry_get_text(input));
    gtk_entry_set_text(output, output_char);
}

static void
print_hello2 (GtkWidget *widget,
             gpointer   data)
{
    decode_num(gtk_entry_get_text(input));
    gtk_entry_set_text(output, output_char);
}

static void
print_hello3 (GtkWidget *widget,
             gpointer   data)
{
    char* total_result = encode_file(gtk_entry_get_text(file));
    gtk_entry_set_text(file_output, total_result);
    free(total_result);
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
    GtkGrid *grid, *row1, *row2;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);


    input = gtk_entry_new();
    output = gtk_entry_new();
    file = gtk_entry_new();
    file_output = gtk_entry_new();
    gtk_entry_set_placeholder_text(input, "input");
    gtk_entry_set_placeholder_text(output, "output");
    gtk_entry_set_placeholder_text(file, "file path");
    gtk_entry_set_placeholder_text(file_output, "file output");

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    gtk_grid_attach(grid, input, 0, 0, 80, 40);
    gtk_grid_attach(grid, output, 0, 40, 80, 40);

    gtk_grid_insert_row(grid, 0);
    gtk_grid_insert_row(grid, 40);

    encode_btn = gtk_button_new();
    decode_btn = gtk_button_new();
    file_encode_btn = gtk_button_new();
    gtk_button_set_label(encode_btn, "encode");
    gtk_button_set_label(decode_btn, "decode");
    gtk_button_set_label(file_encode_btn, "file encode");

    gtk_grid_attach(grid, encode_btn, 0, 100, 20, 30);
    gtk_grid_attach(grid, decode_btn, 60, 100, 20, 30);

    g_signal_connect (encode_btn, "clicked", G_CALLBACK (print_hello), NULL);
    g_signal_connect (decode_btn, "clicked", G_CALLBACK (print_hello2), NULL);
    g_signal_connect (file_encode_btn, "clicked", G_CALLBACK (print_hello3), NULL);

    gtk_grid_attach(grid, file, 0, 150, 20, 30);
    gtk_grid_attach(grid, file_output, 0, 190, 20, 30);
    gtk_grid_attach(grid, file_encode_btn, 0, 240, 20, 30);

    gtk_widget_show_all (window);
}


int main(int argc, char **argv) {
    //encode_file();

    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return 0;
}

char* encode_file(char* file_path) {
    char buf[8];
    int file;
    if ((file = open(file_path, O_RDONLY, 0)) == -1) {
        printf("File opening error.\n");
    }
    ssize_t n;
    uint64_t content;
    uint64_t result;
    char* total_output = (char*)malloc(sizeof(char));
    total_output[0] = '\0';
    memset(total_output, 0, 0);
    int total_length = 1;
    while ((n = read(file, buf, 8)) > 0) {
        total_length += 16;

        content = 0;
        for (int i = 0; i < n; i++) {
            content = content << 8;
            content += buf[i];
        }
        ssize_t remain = 8 - n;
        for (int i = 0; i < remain; i++) {
            content = content << 8;
        }
        result = encode(content);


        for (int i = 0; i < 16; i++) {
            uint8_t num = (uint8_t)((result >> ((15 - i) * 4)) & 0xf);
            sprintf(&output_char[i], "%x", num);
        }

        char* xx = (char*)malloc(sizeof(char) * total_length);
        strcpy(xx, total_output);
        strcat(xx, output_char);
        free(total_output);
        total_output = xx;
    }
    return total_output;
}


void encode_num(char* input) {
    uint64_t bits = char2bit(input);
    printf("输入数据对应的二进制：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(bits >> (64 - i)) & 1);
    }
    printf("\n\n");

    uint64_t result = encode(bits);

    for (int i = 0; i < 16; i++) {
        uint8_t num = (uint8_t)((result >> ((15 - i) * 4)) & 0xf);
        sprintf(&output_char[i], "%x", num);
    }
}

void decode_num(char* input) {
    uint64_t bits = char2bit(input);
    printf("输入数据对应的二进制：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(bits >> (64 - i)) & 1);
    }
    printf("\n\n");

    uint64_t result = decode(bits);

    for (int i = 0; i < 16; i++) {
        uint8_t num = (uint8_t)((result >> ((15 - i) * 4)) & 0xf);
        sprintf(&output_char[i], "%x", num);
    }
}

uint64_t decode(uint64_t bits) {
    k = char2bit("aaaaaaaaaaaaaaaa");
    printf("密钥对应的二进制：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(k >> (64 - i)) & 1);
    }
    printf("\n\n");

    IP(bits);

    uint32_t tmp = left;
    left = right;
    right = tmp;

    for (int i = 0; i < 16; i++) {
        printf("--------------- 第 %d 轮迭代 -------------\n\n", i);

        uint32_t right1 = left;
        uint32_t left1 = right ^ F(E(left), PC_22(mov2[i]));

        left = left1;
        right = right1;

        printf("left 对应的二进制：\n");
        for (int i = 1; i <= 32; i++) {
            printf("%d", (int)(left >> (32 - i)) & 1);
        }
        printf("\n");
        printf("right 对应的二进制：\n");
        for (int i = 1; i <= 32; i++) {
            printf("%d", (int)(right >> (32 - i)) & 1);
        }
        printf("\n\n");
    }

    uint64_t output = IP_inverse2();
    return output;
}

uint64_t encode(uint64_t bits) {
    k = char2bit("aaaaaaaaaaaaaaaa");
    printf("密钥对应的二进制：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(k >> (64 - i)) & 1);
    }
    printf("\n\n");


    PC_1();
    printf("子密钥a对应的二进制：\n");
    for (int i = 1; i <= 32; i++) {
        printf("%d", (int)(k_a >> (32 - i)) & 1);
    }
    printf("\n");
    printf("子密钥b对应的二进制：\n");
    for (int i = 1; i <= 32; i++) {
        printf("%d", (int)(k_b >> (32 - i)) & 1);
    }
    printf("\n\n");

    IP(bits);
    printf("left 对应的二进制：\n");
    for (int i = 1; i <= 32; i++) {
        printf("%d", (int)(left >> (32 - i)) & 1);
    }
    printf("\n");
    printf("right 对应的二进制：\n");
    for (int i = 1; i <= 32; i++) {
        printf("%d", (int)(right >> (32 - i)) & 1);
    }
    printf("\n\n");

    for (int i = 0; i < 16; i++) {
        printf("--------------- 第 %d 轮迭代 -------------\n\n", i);

        uint32_t left1 = right;
        uint32_t right1 = left ^ F(E(right), PC_2(mov[i]));

        left = left1;
        right = right1;

        printf("left 对应的二进制：\n");
        for (int i = 1; i <= 32; i++) {
            printf("%d", (int)(left >> (32 - i)) & 1);
        }
        printf("\n");
        printf("right 对应的二进制：\n");
        for (int i = 1; i <= 32; i++) {
            printf("%d", (int)(right >> (32 - i)) & 1);
        }
        printf("\n\n");
    }

    uint64_t output = IP_inverse();

    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(output >> (64 - i)) & 1);
    }

    return output;
}

void PC_1() {
    uint64_t output = 0;
    for (int i = 1; i <= 56; i++) {
        output = output << 1;
        uint8_t target_index =  PC_1_table[i - 1];
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
    printf("合并：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(tmp >> (64 - i)) & 1);
    }
    printf("\n\n");

    uint64_t output = 0;
    for (int i = 1; i <= 48; i++) {
        output = output << 1;
        uint8_t target_index =  PC_2_table[i - 1];
        output += (tmp >> (56 - target_index)) & 1;
    }
    printf("查表得到最终k：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(output >> (64 - i)) & 1);
    }
    printf("\n\n");

    /**
     * 右移
     */
    k_a = loop_rmov(k_a, num);
    k_b = loop_rmov(k_b, num);
    printf("子密钥a循环右移 %d 次：\n", num);
    for (int i = 1; i <= 32; i++) {
        printf("%d", (int)(k_a >> (32 - i)) & 1);
    }
    printf("\n");
    printf("子密钥b循环右移 %d 次：\n", num);
    for (int i = 1; i <= 32; i++) {
        printf("%d", (int)(k_b >> (32 - i)) & 1);
    }
    printf("\n\n");
    return output;
}

uint64_t PC_2(int num) {
    /**
     * 先左移
     */
    k_a = loop_lmov(k_a, num);
    k_b = loop_lmov(k_b, num);
    printf("子密钥a循环左移 %d 次：\n", num);
    for (int i = 1; i <= 32; i++) {
        printf("%d", (int)(k_a >> (32 - i)) & 1);
    }
    printf("\n");
    printf("子密钥b循环左移 %d 次：\n", num);
    for (int i = 1; i <= 32; i++) {
        printf("%d", (int)(k_b >> (32 - i)) & 1);
    }
    printf("\n\n");

    //将两个key合并
    uint64_t tmp = k_a;
    tmp = tmp << 28;
    tmp += k_b;
    printf("合并：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(tmp >> (64 - i)) & 1);
    }
    printf("\n\n");

    uint64_t output = 0;
    for (int i = 1; i <= 48; i++) {
        output = output << 1;
        uint8_t target_index =  PC_2_table[i - 1];
        output += (tmp >> (56 - target_index)) & 1;
    }
    printf("查表得到最终k：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(output >> (64 - i)) & 1);
    }
    printf("\n\n");
    return output;
}

uint32_t loop_rmov(uint32_t input, int num) {
    for (int i = 0; i < num; i++) {
        uint32_t tail = input & 1;

        /*
        printf("tail：\n");
        for (int i = 1; i <= 32; i++) {
            printf("%d", (int)(tail >> (32 - i)) & 1);
        }
        printf("\n\n");
         */

        input = (input >> 1) & 0xfffffff;

        /*
        printf("input：\n");
        for (int i = 1; i <= 32; i++) {
            printf("%d", (int)(input >> (32 - i)) & 1);
        }
        printf("\n\n");
         */

        input += tail << 27;
        /*
        printf("input：\n");
        for (int i = 1; i <= 32; i++) {
            printf("%d", (int)(input >> (32 - i)) & 1);
        }
        printf("\n\n");
         */
    }
    return input;
}

uint32_t loop_lmov(uint32_t input, int num) {
    for (int i = 0; i < num; i++) {
        uint32_t tail = (input >> 27) & 1;

        /*
        printf("tail：\n");
        for (int i = 1; i <= 32; i++) {
            printf("%d", (int)(tail >> (32 - i)) & 1);
        }
        printf("\n\n");
         */

        input = (input << 1) & 0xfffffff;

        /*
        printf("input：\n");
        for (int i = 1; i <= 32; i++) {
            printf("%d", (int)(input >> (32 - i)) & 1);
        }
        printf("\n\n");
         */

        input += tail;
        /*
        printf("input：\n");
        for (int i = 1; i <= 32; i++) {
            printf("%d", (int)(input >> (32 - i)) & 1);
        }
        printf("\n\n");
         */
    }
    return input;
}

uint8_t char2num(char ch) {
    if (ch >= 48 && ch <= 57) {
        // 数字
        return (uint8_t)(ch - 48);
    } else if (ch >= 65 && ch <= 70) {
        // 大写字母
        return (uint8_t)(ch - 55);
    } else if (ch >= 97 && ch <= 102) {
        // 小写字母
        return (uint8_t)(ch - 87);
    } else {
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

    printf("left right 合并：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(input >> (64 - i)) & 1);
    }
    printf("\n\n");

    uint64_t output = 0;

    /**
     * 实现查表转换功能
     */
    for (int i = 1; i <= 64; i++) {
        output = output << 1;
        uint8_t target_index =  IP_inverse_table[i - 1];
        output += (input >> (64 - target_index)) & 1;
    }
    printf("查表转换：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(output >> (64 - i)) & 1);
    }
    printf("\n\n");
    return output;
}

uint64_t IP_inverse() {
    uint64_t input = (uint64_t)right;
    input = (input << 32) + left;

    printf("right left 合并：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(input >> (64 - i)) & 1);
    }
    printf("\n\n");

    uint64_t output = 0;

    /**
     * 实现查表转换功能
     */
    for (int i = 1; i <= 64; i++) {
        output = output << 1;
        uint8_t target_index =  IP_inverse_table[i - 1];
        output += (input >> (64 - target_index)) & 1;
    }
    printf("查表转换：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(output >> (64 - i)) & 1);
    }
    printf("\n\n");
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

uint64_t E(uint32_t input) {
    uint64_t output = 0;
    for (int i = 1; i <= 48; i++) {
        output = output << 1;
        output += (input >> (32 - E_table[i - 1])) & 1;
    }
    printf("E 运算结果：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(output >> (64 - i)) & 1);
    }
    printf("\n\n");
    return output;
}

void E2(uint32_t input) {
    printf("\n");
    for (int i = 0; i < 6; i++) {
        xx[i] = 0;
        for (int j = 0; j < 8; j++) {
            xx[i] = xx[i] << 1;
            int a = (input >> (32 - E_table[i * 8 + j])) & 1;
            printf("%d", a);
            xx[i] += (input >> (32 - E_table[i * 8 + j])) & 1;
        }

        printf("\n");
        printf("%d ", xx[i]);
    }
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
    uint64_t xor = input ^ k;

    printf("E K 异或：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(xor >> (64 - i)) & 1);
    }
    printf("\n\n");

    //xor = xor << 16;

    uint32_t output = 0;
    for (int i = 0; i < 8; i++) {
        output = output << 4;

        uint8_t x = 0;
        uint8_t y = 0;

        x += (xor >> 47) & 1;
        x = x << 1;
        x += (xor >> 42) & 1;

        y = (uint8_t)((xor >> 43) & 0xf);

        output += S[i][x][y];

        xor = xor << 6;
    }
    printf("48 -> 32：\n");
    for (int i = 1; i <= 32; i++) {
        printf("%d", (int)(output >> (32 - i)) & 1);
    }
    printf("\n\n");

    uint32_t output2 = 0;
    for (int i = 1; i <= 32; i++) {
        output2 = output2 << 1;
        output2 += (output >> (32 - P[i - 1])) & 1;
    }

    /*
    printf("\n");
    for (int i = 7; i >= 0; i--) {
        printf("%d ", (output >> (4 * i)) & 15);
    }
     */

    return output2;
}