
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
unsigned int hash(const char* str, int table_size) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        printf("char: %c\n", c);
    printf("c: %d\n", c);
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
        printf("hash: %d\n", hash);
    }
    printf("final hash: %d\n", hash);
    printf("table_size: %d\n", table_size);
    printf("index: %d\n", hash % table_size);
    return hash % table_size;
}

void concat_names(char* full_name, char* first_name, char* last_name)
{
    memset(full_name, 0, 102);

    strcpy(full_name, first_name);

    strcat(full_name, " ");
    strcat(full_name, last_name);
}


int main() {

    char full_name[102];
    concat_names(full_name, "daniel", "gabai");
    printf("full name: %s\n", full_name);
    hash("daniel",100000);
    hash("leinad",100000);
    hash("daniel gabai",100000);
    hash("Daniell Baratta",100000);
    return 0;
}