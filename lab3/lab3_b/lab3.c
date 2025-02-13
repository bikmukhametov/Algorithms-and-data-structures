#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lab3.h"

int scan(int *x){                                       //проверка на целочисленное значение
    int m = 0;
    int x_2 = 0;
    m = scanf("%d", &x_2);
    while(m != 1){
        if(m == 0){
            scanf("%*[^\n]");
            printf("Ошибка! Введите корректное значение: ");
            m = scanf("%d", &x_2);
        }else if(m == EOF){
            return 1;
        }
    }
    scanf("%*c");   ///////////
    *x = x_2;
    return 0;
}

int my_readline(char **stroka){
    char *substring = NULL;
    int len = 0;
    int k = 1;
    while(k != 0){
        substring = (char *) calloc(81, sizeof(char));
        substring[80] = '\0';
        k = scanf("%80[^\n]", substring);
        if(k == -1){
            free(substring);
            *stroka = NULL;
            return 1;
        }else if(k == 1){
            int substring_len = strlen(substring);
            *stroka = (char *) realloc (*stroka, len + substring_len + 1);
            memcpy(*stroka + len, substring, substring_len);
            len += substring_len;
        }else{
            scanf("%*c");
        }
        free(substring);
    }
    if (len > 0){
        (*stroka)[len] = '\0';
    }else{
        *stroka = (char *) realloc (*stroka, sizeof(char) * 1);
        (*stroka)[len] = '\0';
    }
    return 0;
}

int save(Table* table){                              //внесение таблицы в файл
    FILE *fd = NULL;
    Item *cur = NULL;
    fclose(table->fd);
    //fopen_s(&fd, table->fname, "wb");
    fd = fopen(table->fname, "wb");
    if(fd == NULL)
        return 0;
    cur = table->first;
    while(cur != NULL){
        fseek(fd, 0, SEEK_END);
        fwrite(&cur->len_k, sizeof(int), 1, fd);

        fseek(fd, 0, SEEK_END);
        fwrite(cur->key, sizeof(char), cur->len_k, fd);

        fseek(fd, 0, SEEK_END);
        fwrite(&cur->release, sizeof(int), 1, fd);

        fseek(fd, 0, SEEK_END);
        fwrite(&cur->offset, sizeof(int), 1, fd);

        fseek(fd, 0, SEEK_END);
        fwrite(&cur->len, sizeof(int), 1, fd);

        cur = cur->next;
    }
    fclose(fd);
    return 1;
}

int insert(Table *table, char* k, char *str){       //  функция добавления элемента
    Item* item2 = (Item *) calloc (1, sizeof(Item));
    item2->key = k;
    item2->len = strlen(str) + 1;
    item2->len_k = strlen(k) + 1;
    item2->next = NULL;

    int rel = 0;                                    //внесение 'release'
    if(table->first != NULL){
        Item* item_rel = table->first;
        while(item_rel){
            if(strcmp(item_rel->key, k) == 0){
                if(item_rel->release > rel){
                    rel = item_rel->release;
                }
            }
            item_rel = item_rel->next;
        }

    }
    rel += 1;
    item2->release = rel;

    fseek(table->fd, 0, SEEK_END);
    item2->offset = ftell(table->fd);
    if(table->first == NULL){
        table->first = item2;
    }else{
        Item* item = table->first;
        while(item->next != NULL){
            item = item->next;
        }
        item->next = item2;
    }
    fwrite(str, sizeof(char), item2->len, table->fd);
    return 0;
}

int add(Table* table){                                  //добавление элемента в таблицу
    char* k = NULL;
    char* info = NULL;
    printf("Enter key: ");
    int i = my_readline(&k);
    if(i == 1)
        return 1;
    printf("Enter info: ");
    i = my_readline(&info);
    if (i == 1)
        return 1;
    if((info[0] == '\0') || (k[0] == '\0')){
        printf("Ошибка! Пустое значение\n");
        free(k);
        free(info);
        return 0;
    }else{
        insert(table, k, info);
        free(info);
    }
    return 0;
}

void show(Table* table){                                //показ таблицы
    if(table != NULL){
        Item* item = table->first;
        char* info = NULL;
        while(item != NULL){
            info = (char *) calloc (item->len, sizeof(char));
            fseek(table->fd, item->offset, SEEK_SET);
            fread(info, sizeof(char), item->len, table->fd);
            printf("key: %s         ", item->key);
            printf("release: %d     ", item->release);
            printf("info: %s\n", info);
            free(info);
            info = NULL;
            item = item->next;
        }
    }
}

void delet_key_and_release(Table* table, char** k, int r){           //удаление по заданному 'Key' и 'Release'
    if(table != NULL){
        Item * item = table->first;     //текущий
        Item * item2 = NULL;         //предыдущий
        while(item != NULL){
            if((strcmp(item->key, *k) == 0) && (item->release == r)){
                if(table->first == item){
                    table->first = item->next;
                    free(item->key);
                    free(item);
                    item = table->first;
                }else{
                    item2->next = item->next;
                    free(item->key);
                    free(item);
                    item = table->first;
                }
            }else{
                item2 = item;
                item = item->next;
            }
        }
    }
}

void delet_key(Table* table, char** k){           //удаление по заданному 'Key'
    if(table != NULL){
        Item* item = table->first;     //текущий
        Item* item2 = NULL;         //предыдущий
        while(item != NULL){
            if(strcmp(item->key, *k) == 0){
                if(table->first == item){
                    table->first = item->next;
                    free(item->key);
                    free(item);
                    item = table->first;
                }else{
                    item2->next = item->next;
                    free(item->key);
                    free(item);
                    item = table->first;
                }
            }else{
                item2 = item;
                item = item->next;
            }
        }
    }
}

void delet_table(Table* table){                      //чистка памяти (таблицы)
    Item* item2 = NULL;
    if(table != NULL){
        Item* item = table->first;
        while(item != NULL){
            free(item->key);
            item2 = item;
            item = item->next;
            free(item2);
        }
    }
    free(table->fname);
    table->first = NULL;
}

int load (Table *table, char *fname){      //функция считывания из файла
    FILE *fd = NULL;
    char *fdatname;
    int len = strlen(fname);
    Item cur = { 0, NULL, 0, 0, 0, NULL };

    fdatname = strdup(fname);
    table->fname = strdup(fname);
    fdatname = (char *) realloc(fdatname, len + 5);
    strcat(fdatname, ".dat");
    table->fname = (char *)realloc(table->fname, len + 5);
    strcat(table->fname, ".tab");

    fd = fopen(table->fname, "rb");
    if(fd != NULL){
        table->fd = fopen(fdatname, "r+b");
        if (table->fd == NULL){
            free(fdatname);
            return 1;
        }
        while (fread(&(cur.len_k), sizeof(int), 1, fd)){
            cur.key = (char *) calloc(cur.len_k, sizeof(char));
            fread(cur.key, sizeof(char), cur.len_k, fd);
            fread(&(cur.release), sizeof(int), 1, fd);
            fread(&(cur.offset), sizeof(int), 1, fd);
            fread(&(cur.len), sizeof(int), 1, fd);
            Item* item = (Item*) calloc (1, sizeof(Item));
            *item = cur;

            if(table->first == NULL){
                table->first = item;
            }else{
                Item* item2 = table->first;
                while(item2->next != NULL){
                    item2 = item2->next;
                }
                item2->next = item;
            }
        }
        fclose(fd);
    }
    else{
        table->fd = fopen(fdatname, "w+b");
        if (table->fd == NULL){
            free(fdatname);
            return 1;
        }
    }
    free(fdatname);
    return 0;
}

int input_file(Table* table){          //считывание из файла
    char* fname = NULL;
    printf("Enter file name: ");
    int i = my_readline(&fname);
    if((i == 1) || (fname[0] == '\0')){
        free(fname);
        return 1;
    }
    i = load(table, fname);
    if (i == 1){
        printf("The appropriate data file is not exists\n");
    }
    free(fname);
    return i;
}

void find_key(Table* table, char** k){                  //показ элементов таблицы по заданному 'Kлючу'
    if(table != NULL){
        Item* item = table->first;
        char* info = NULL;
        while(item != NULL){
            if(strcmp(item->key, *k) == 0){
                info = (char *) calloc (item->len, sizeof(char));
                fseek(table->fd, item->offset, SEEK_SET);
                fread(info, sizeof(char), item->len, table->fd);
                printf("key: %s         ", item->key);
                printf("release: %d     ", item->release);
                printf("info: %s\n", info);
                free(info);
                info = NULL;
            }
            item = item->next;
        }
    }
}

void find_key_and_release(Table* table, char** k, int r){       //показ элементов таблицы по заданному 'Kлючу' и 'Bерсии'
    if(table != NULL){
        Item* item = table->first;
        char* info = NULL;
        while(item != NULL){
            if((strcmp(item->key, *k) == 0) && (item->release == r)){
                info = (char *) calloc (item->len, sizeof(char));
                fseek(table->fd, item->offset, SEEK_SET);
                fread(info, sizeof(char), item->len, table->fd);
                printf("key: %s         ", item->key);
                printf("release: %d     ", item->release);
                printf("info: %s\n", info);
                free(info);
                info = NULL;
            }
            item = item->next;
        }
    }
}

int dialog(Table* table){
    int i = input_file(table);
    while(i == 0){
        const char *msgs[ ] = {"1. Add                                  -- 1. Добавить",
                               "2. Delete items by 'Key'                -- 2. Удаление по 'Ключу'",
                               "3. Delete items by 'Key' and 'Version'  -- 3. Удалить элементы по 'Ключу' и 'Версии'",
                               "4. Find items by 'Key'                  -- 4. Найти элементы по 'Ключу'",
                               "5. Find items by 'Key' and 'Version'    -- 5. Найти элементы по 'Ключу' и 'Версии'",
                               "6. Show                                 -- 6. Показать",
                               "7. Quit                                 -- 7. Выход"};
        printf("\n");
        for(int ms = 0; ms < 7; ms++){
            printf("%s\n", msgs[ms]);
        }
        int m = 0;
        printf("\n");
        printf("command: ");
        i = scan(&m);
        if(i == 1){
            save(table);
            delet_table(table);
            return 1;
        }
        printf("\n");
        switch(m){
            case 1:
                i = add(table);
                if(i == 1){
                    save(table);
                    delet_table(table);
                    return 1;
                }
                break;
            case 2:
                printf("Enter the key: ");
                char* k2 = NULL;
                i = my_readline(&k2);
                if(i == 1){
                    free(k2);
                    save(table);
                    delet_table(table);
                    return 1;
                }
                delet_key(table, &k2);
                free(k2);
                break;
            case 3:
                printf("Enter the key: ");
                char* k3 = NULL;
                i = my_readline(&k3);
                if(i == 1){
                    free(k3);
                    save(table);
                    delet_table(table);
                    return 1;
                }
                printf("Enter the release: ");
                int r3;
                i = scan(&r3);
                if(i == 1){
                    save(table);
                    delet_table(table);
                    free(k3);
                    return 1;
                }
                delet_key_and_release(table, &k3, r3);
                free(k3);
                break;
            case 4:
                printf("Enter the key: ");
                char* k4 = NULL;
                i = my_readline(&k4);
                printf("\n");
                if(i == 1){
                    free(k4);
                    save(table);
                    delet_table(table);
                    return 1;
                }
                find_key(table, &k4);
                free(k4);
                break;
            case 5:
                printf("Enter the key: ");
                char* k5 = NULL;
                i = my_readline(&k5);
                if(i == 1){
                    free(k5);
                    save(table);
                    delet_table(table);
                    return 1;
                }
                printf("Enter the release: ");
                int r5;
                i = scan(&r5);
                if(i == 1){
                    save(table);
                    delet_table(table);
                    free(k5);
                    return 1;
                }
                printf("\n");
                find_key_and_release(table, &k5, r5);
                free(k5);
                break;
            case 6:
                show(table);
                break;
            case 7:
                save(table);
                delet_table(table);
                i = 1;
                break;
            default:
                printf("Ошибка! Введите число [1 , 7]: ");
                break;
        }
    }
    return 0;
}
