#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unitypes.h>
#include <gtk/gtk.h>
#include <fcntl.h>
#include <inttypes.h>

#include "main.h"
#include "tables.h"

#define CHARLEN 16


GtkWidget *window,  *encode_btn, *decode_btn, *file_encode_btn;
GtkEntry *input, *output, *file, *file_output;
GtkWidget *g_data_input, *g_data_output, *g_file_input, *g_file_output;

void encode_gui()
{
    encode_num(gtk_entry_get_text(GTK_ENTRY(g_data_input)));
    gtk_entry_set_text(GTK_ENTRY(g_data_output), output_char);
}

void decode_gui()
{
    decode_num(gtk_entry_get_text(input));
    gtk_entry_set_text(output, output_char);
}

void file_encode_gui()
{
    GtkFileChooserButton *file_input = GTK_FILE_CHOOSER_BUTTON(g_file_input);
    char *filename = gtk_file_chooser_button_get_title(file_input);

    GtkFileChooser a();


    char* total_result = encode_file(filename);
    gtk_entry_set_text(file_output, total_result);
    free(total_result);
}

int main(int argc, char **argv) {

    GtkBuilder* builder;
    GtkWidget* window;

    gtk_init(&argc, &argv);
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "/home/mingtao/Desktop/未保存的 1.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    g_data_input = GTK_WIDGET(gtk_builder_get_object(builder, "g_data_input"));
    g_data_output = GTK_WIDGET(gtk_builder_get_object(builder, "g_data_output"));
    g_file_input = GTK_WIDGET(gtk_builder_get_object(builder, "g_file_input"));
    g_file_output = GTK_WIDGET(gtk_builder_get_object(builder, "g_file_output"));

    gtk_builder_add_callback_symbol(builder, "encode_gui", encode_gui);
    gtk_builder_add_callback_symbol(builder, "decode_gui", decode_gui);
    gtk_builder_add_callback_symbol(builder, "file_encode_gui", file_encode_gui);

    gtk_builder_connect_signals(builder, NULL);

    g_object_unref(builder);

    gtk_widget_show(window);

    gtk_main();
    /*
      GtkApplication *app;
      int status;

      app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
      g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
      status = g_application_run (G_APPLICATION (app), argc, argv);
      g_object_unref (app);
      */
    //decode_file();

    return 0;
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
    printf("%" PRIu64 "\n", decode(len_encoded));
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
    int file;
    int file_output;

    file_output = open("/home/mingtao/xxx", O_WRONLY | O_CREAT, 0666);

    if ((file = open(file_path, O_RDONLY, 0)) == -1) {
        printf("File opening error.\n");
    }
    ssize_t n;
    uint64_t content;
    uint64_t result;

    uint64_t total_len = 0;
    while ((n = read(file, &content, 8)) > 0) {

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
        result = encode(content2);
        uint64_t result2 = 0;
        for (int i = 0; i < 8; i++) {
            result2 <<= 8;
            result2 += (result >> (i * 8)) & 0xff;
        }

        total_len += n * 8;

        write(file_output, &result2, sizeof(result2));

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

    uint64_t len_encoded = encode(total_len);
    uint8_t len_encoded_byte;
    for (int i = 7; i >= 0; i--) {
        len_encoded_byte = (uint8_t)((len_encoded >> i * 8) & 0xff);
        write(file_output, &len_encoded_byte, sizeof(len_encoded_byte));
    }
    printf("%" PRIu64 "\n", total_len);
    printf("%" PRIu64 "\n", len_encoded);
    printf("len_encoded：\n");
    for (int i = 1; i <= 64; i++) {
        printf("%d", (int)(len_encoded >> (64 - i)) & 1);
    }
    printf("\n");

    close(file);
    close(file_output);
}


void encode_num(char* input) {
    uint64_t bits = char2bit(input);
//    printf("输入数据对应的二进制：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(bits >> (64 - i)) & 1);
//    }
//    printf("\n\n");

    uint64_t result = encode(bits);

    for (int i = 0; i < 16; i++) {
        uint8_t num = (uint8_t)((result >> ((15 - i) * 4)) & 0xf);
        sprintf(&output_char[i], "%x", num);
    }
}

void decode_num(char* input) {
    uint64_t bits = char2bit(input);
//    printf("输入数据对应的二进制：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(bits >> (64 - i)) & 1);
//    }
//    printf("\n\n");

    uint64_t result = decode(bits);

    for (int i = 0; i < 16; i++) {
        uint8_t num = (uint8_t)((result >> ((15 - i) * 4)) & 0xf);
        sprintf(&output_char[i], "%x", num);
    }
}

uint64_t decode(uint64_t bits) {
    k = char2bit("aaaaaaaaaaaaaaaa");
//    printf("密钥对应的二进制：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(k >> (64 - i)) & 1);
//    }
//    printf("\n\n");

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

//        printf("left 对应的二进制：\n");
//        for (int i = 1; i <= 32; i++) {
//            printf("%d", (int)(left >> (32 - i)) & 1);
//        }
//        printf("\n");
//        printf("right 对应的二进制：\n");
//        for (int i = 1; i <= 32; i++) {
//            printf("%d", (int)(right >> (32 - i)) & 1);
//        }
//        printf("\n\n");
    }

    uint64_t output = IP_inverse2();
    return output;
}

uint64_t encode(uint64_t bits) {
    k = char2bit("aaaaaaaaaaaaaaaa");
//    printf("密钥对应的二进制：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(k >> (64 - i)) & 1);
//    }
//    printf("\n\n");


    PC_1();
//    printf("子密钥a对应的二进制：\n");
//    for (int i = 1; i <= 32; i++) {
//        printf("%d", (int)(k_a >> (32 - i)) & 1);
//    }
//    printf("\n");
//    printf("子密钥b对应的二进制：\n");
//    for (int i = 1; i <= 32; i++) {
//        printf("%d", (int)(k_b >> (32 - i)) & 1);
//    }
//    printf("\n\n");

    IP(bits);
//    printf("left 对应的二进制：\n");
//    for (int i = 1; i <= 32; i++) {
//        printf("%d", (int)(left >> (32 - i)) & 1);
//    }
//    printf("\n");
//    printf("right 对应的二进制：\n");
//    for (int i = 1; i <= 32; i++) {
//        printf("%d", (int)(right >> (32 - i)) & 1);
//    }
//    printf("\n\n");

    for (int i = 0; i < 16; i++) {
    //    printf("--------------- 第 %d 轮迭代 -------------\n\n", i);

        uint32_t left1 = right;
        uint32_t right1 = left ^ F(E(right), PC_2(mov[i]));

        left = left1;
        right = right1;

//        printf("left 对应的二进制：\n");
//        for (int i = 1; i <= 32; i++) {
//            printf("%d", (int)(left >> (32 - i)) & 1);
//        }
//        printf("\n");
//        printf("right 对应的二进制：\n");
//        for (int i = 1; i <= 32; i++) {
//            printf("%d", (int)(right >> (32 - i)) & 1);
//        }
//        printf("\n\n");
    }

    uint64_t output = IP_inverse();

//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(output >> (64 - i)) & 1);
//    }

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
//    printf("合并：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(tmp >> (64 - i)) & 1);
//    }
//    printf("\n\n");

    uint64_t output = 0;
    for (int i = 1; i <= 48; i++) {
        output = output << 1;
        uint8_t target_index =  PC_2_table[i - 1];
        output += (tmp >> (56 - target_index)) & 1;
    }
//    printf("查表得到最终k：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(output >> (64 - i)) & 1);
//    }
//    printf("\n\n");

    /**
     * 右移
     */
    k_a = loop_rmov(k_a, num);
    k_b = loop_rmov(k_b, num);
//    printf("子密钥a循环右移 %d 次：\n", num);
//    for (int i = 1; i <= 32; i++) {
//        printf("%d", (int)(k_a >> (32 - i)) & 1);
//    }
//    printf("\n");
//    printf("子密钥b循环右移 %d 次：\n", num);
//    for (int i = 1; i <= 32; i++) {
//        printf("%d", (int)(k_b >> (32 - i)) & 1);
//    }
//    printf("\n\n");
    return output;
}

uint64_t PC_2(int num) {
    /**
     * 先左移
     */
    k_a = loop_lmov(k_a, num);
    k_b = loop_lmov(k_b, num);
//    printf("子密钥a循环左移 %d 次：\n", num);
//    for (int i = 1; i <= 32; i++) {
//        printf("%d", (int)(k_a >> (32 - i)) & 1);
//    }
//    printf("\n");
//    printf("子密钥b循环左移 %d 次：\n", num);
//    for (int i = 1; i <= 32; i++) {
//        printf("%d", (int)(k_b >> (32 - i)) & 1);
//    }
//    printf("\n\n");

    //将两个key合并
    uint64_t tmp = k_a;
    tmp = tmp << 28;
    tmp += k_b;
//    printf("合并：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(tmp >> (64 - i)) & 1);
//    }
//    printf("\n\n");

    uint64_t output = 0;
    for (int i = 1; i <= 48; i++) {
        output = output << 1;
        uint8_t target_index =  PC_2_table[i - 1];
        output += (tmp >> (56 - target_index)) & 1;
    }
//    printf("查表得到最终k：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(output >> (64 - i)) & 1);
//    }
//    printf("\n\n");
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

//    printf("left right 合并：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(input >> (64 - i)) & 1);
//    }
//    printf("\n\n");

    uint64_t output = 0;

    /**
     * 实现查表转换功能
     */
    for (int i = 1; i <= 64; i++) {
        output = output << 1;
        uint8_t target_index =  IP_inverse_table[i - 1];
        output += (input >> (64 - target_index)) & 1;
    }
//    printf("查表转换：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(output >> (64 - i)) & 1);
//    }
//    printf("\n\n");
    return output;
}

uint64_t IP_inverse() {
    uint64_t input = (uint64_t)right;
    input = (input << 32) + left;

//    printf("right left 合并：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(input >> (64 - i)) & 1);
//    }
//    printf("\n\n");

    uint64_t output = 0;

    /**
     * 实现查表转换功能
     */
    for (int i = 1; i <= 64; i++) {
        output = output << 1;
        uint8_t target_index =  IP_inverse_table[i - 1];
        output += (input >> (64 - target_index)) & 1;
    }
//    printf("查表转换：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(output >> (64 - i)) & 1);
//    }
//    printf("\n\n");
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
//    printf("E 运算结果：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(output >> (64 - i)) & 1);
//    }
//    printf("\n\n");
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
    uint64_t xor = input ^ k;

//    printf("E K 异或：\n");
//    for (int i = 1; i <= 64; i++) {
//        printf("%d", (int)(xor >> (64 - i)) & 1);
//    }
//    printf("\n\n");

    //xor = xor << 16;

    uint32_t output = 0;
    for (int i = 0; i < 8; i++) {
        output = output << 4;

        /*
        uint8_t x = 0;
        uint8_t y = 0;

        x += (xor >> 47) & 1;
        x = x << 1;
        x += (xor >> 42) & 1;

        y = (uint8_t)((xor >> 43) & 0xf);

        output += S[i][x][y];
         */
        uint8_t index = (uint8_t)((xor >> 42) & 0x3f);
        output += S2[i][index];

        xor = xor << 6;
    }
//    printf("48 -> 32：\n");
//    for (int i = 1; i <= 32; i++) {
//        printf("%d", (int)(output >> (32 - i)) & 1);
//    }
//    printf("\n\n");

    uint32_t output2 = 0;
    for (int i = 0; i < 32; i++) {
        output2 = output2 << 1;
        output2 += (output >> (32 - P[i])) & 1;
    }

    /*
    printf("\n");
    for (int i = 7; i >= 0; i--) {
        printf("%d ", (output >> (4 * i)) & 15);
    }
     */

    return output2;
}