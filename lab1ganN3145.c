#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <string.h>

#define ull unsigned long long


void printDec2Bin(ull dec, int l){
    if(dec > 0){
        printDec2Bin(dec >> 1, l + 1);
        if(l % 8 == 0 && l != 64)
            printf(" ");
        printf("%llu", dec&1);
    }
    else if(l <= 64){
        printDec2Bin(dec, l+1);
        if(l % 8 == 0 && l != 64)
            printf(" ");
        printf("0");
    }
}

void convDec2CharHex(ull dec, unsigned char *bytes){
    for (int i = 0; i < 8; ++i)
        bytes[7 - i] = (dec >> (8 * i)) & 0xff;
}

void printBytes(unsigned char* bytes){
    for(int i = 0; i < 8; ++i)
        printf("%02X", bytes[i]);
    printf("\n");
}

ull convCharHex2Dec(unsigned char* bytes){
    ull n = 0;
    for(int i = 0; i < 8; ++i){
        //printf("i = %d, %02X\n", i, bytes[i]);
        n = n | ((1ull * bytes[i]) << ((7 - i) * 8));
    }
    return n;
}

ull generateRandomULL(){
    ull new_n = (ull)rand() * rand();
    printf("generated random number: %llu\n", new_n);
    return new_n;
}

int main(int argc, char *argv[]) {
    // Проверка запуска с переменной среды, включающей отладочный вывод.
    // Пример запуска с установкой переменной LAB1DEBUG в 1:
    // $ LAB1DEBUG=1 ./lab1abcNXXXXX 123

    char *DEBUG = getenv("LAB1DEBUG");
    if (DEBUG) {
        fprintf(stderr, "Включен вывод отладочных сообщений\n");
    }

    if (argc != 2 && argc != 1) {
        fprintf(stderr, "Usage: %s [число]\n", argv[1]);
        return EXIT_FAILURE;
    }

    //
    // Тут может быть ваш код. В этом файле Вы можете поменять все, что угодно.
    // Главное - чтобы потом все правильно работало ;-)
    //

    srand(time(NULL));

    //Check if input is correct
    bool is_num = true;
    for(int i = 0; argc != 1 && i < (int)strlen(argv[1]); ++i){
        if(isdigit(argv[1][i]) == false){
            is_num = false;
            break;
        }
    }
    if(!is_num)
        goto error_number;

    ull n = (argc == 2) ? (strtoull(argv[1], 0, 10)) : generateRandomULL();
    printDec2Bin(n, 1);
    printf("\n");

    unsigned char bytes[8]; convDec2CharHex(n, bytes);//Save bytes of N to modify them

    //Save byte indexes, which order we should reverse
    int change[8] = {0};
    for(int i = 0; i < 8; ++i){
        bool sim1 = (bytes[i] & (1 << 7)) >> 7 == (bytes[i] & 1);
        bool sim2 = (bytes[i] & (1 << 6)) >> 5 == (bytes[i] & 2);
        if(sim1 && sim2)
            change[i] = 1;
    }

    //Reverse byte order
    for(int i = 0; i < 8; ++i){
        if(!change[i])
            continue;
        //I use double for to simplify code instead of two-pointer method.
        // We don't lose much asymptotic
        for(int j = 7; j > i; --j){
            if(change[i] == change[j] && change[i] == 1){
                unsigned char tmp = bytes[i];
                bytes[i] = bytes[j];
                bytes[j] = tmp;
                change[i] = 0, change[j] = 0;
                break;
            }
        }
    }
    //printBytes(bytes);

    n = convCharHex2Dec(bytes); //Convert new Bytes array to N
    //printf("%llu\n", n);

    printDec2Bin(n, 1); //Print new N
    printf("\n");

    return EXIT_SUCCESS;

    error_number: fprintf(stderr ,"Ошибка: '%s' не является числом\n", argv[1]);
    return EXIT_FAILURE;
}