#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"

const int WEIGHT = 80;
const int HEIGHT = 25;

int max(int x, int y){ return x < y ? y : x;}
int min(int x, int y){ return x < y ? x : y;}

void render(char *string, struct point *point, char **to_light, int to_light_len) {
    system("clear");
    char* begin = string;
    int len = strlen(string);
    int i = (point->num / (WEIGHT * HEIGHT)) * (WEIGHT * HEIGHT);
    begin += i;
    int num_light = 0;
    while(to_light != NULL && *to_light != NULL && *to_light < begin){
        num_light++;
        to_light++;
    }
    if (to_light != NULL && *to_light != NULL && *to_light == begin){
        printf("\x1b[42m");
    }
    int stop_pos = HEIGHT * WEIGHT + i;
    while (i < stop_pos && i < len){
        if (to_light != NULL && *to_light != NULL && begin - *to_light == to_light_len){
            if (begin != *(to_light + 1))
                printf("\x1b[0m");
            to_light++;
            num_light++;
        }
        if (point->num - 1 == i && point->num % WEIGHT != 0){
            printf(">");
        } else if (point->num + 1 == i && point->num % WEIGHT != 79){
            printf("<");
        } else {
            printf("%c", *begin);
        }
        ++i;
        ++begin;
        if (to_light != NULL && *to_light != NULL && *to_light == begin){
            printf("\x1b[42m");
        }
        if (i % WEIGHT == 0){
            printf("\n");
        }
    }
    printf("\x1b[40m");
    if (point->num + 1 == len && i % WEIGHT != 79){
        printf("<");
    }
    printf("\n");
}

void add(char** string, char* to_add, struct point* point){
    *string = realloc(*string, strlen(*string) + strlen(to_add) + 1);
    point->pointer = *string + point->num;
    strcat(*string, to_add);
}

void move(char **string, int to_move, struct point *point, int len) {
    if (len < 1){
        point->num = 0;
        point->pointer = *string;
        return;
    }
    int temp = 0;
    if (to_move > 0){
        temp =  min(to_move, len - point->num - 1);
    } else {
        temp = max(to_move, -point->num);
    }
    point->num += temp;
    point->pointer += temp;
}

void insert(char **string, char *to_insert, struct point insert_point, struct point* main_point){
    int str_len = strlen(*string);
    int ins_len = strlen(to_insert);
    if (insert_point.num + ins_len > str_len){
        *string = realloc(*string, insert_point.num + ins_len + 1);
        insert_point.pointer = *string + insert_point.num;
        main_point->pointer = *string + main_point->num;
    }
    memcpy(insert_point.pointer, to_insert, ins_len);
    //(*string)[ins_len + insert_point.num] = '\0';
}

void put(char** string, char* to_put, struct point* point){
    int str_len = strlen(*string);
    int put_len = strlen(to_put);

    if (str_len < 1){
        add(string, to_put, point);
        return;
    }

    *string = realloc(*string, str_len + put_len + 1);
    point->pointer = *string + point->num;
    memcpy(point->pointer + put_len, point->pointer, str_len - point->num + 1);
    memcpy(point->pointer, to_put, put_len);
    move(string, put_len, point, str_len);
}

void my_remove(char** string, struct point start, struct point end, struct point* point){
    int str_len = strlen(*string);
    memcpy(start.pointer, end.pointer + 1, str_len - end.num);
    *string = realloc(*string, str_len - (end.num - start.num));
    point->pointer = *string + point->num;
    if (point->num >= start.num){
        move(string, -(end.num - start.num + 1), point, strlen(*string));
    }
}

void del(char** string, struct point* point, int num){
    if (point->num == 0){
        printf("Бууу, слева никого нет");
        return;
    }
    num = min(num, point->num + 2);
    struct point start, end;
    if (point->num - num >= 0){
        start.num = point->num - num;
        start.pointer = point->pointer - num;
    } else {
        start.num = 0;
        start.pointer = *string;
    }

    end.num = point->num - 1;
    end.pointer = point->pointer - 1;
    my_remove(string, start, end, point);
}

void movelw(char** string, struct point* point, int num){
    // "privet kek shmek ses"
    while(point->pointer != *string && *(point->pointer) != ' '){
        --point->pointer;
        --point->num;
    }
    if(point->pointer == *string){ return; }
    int i = 0;
    while(i != num && point->pointer != *string){
        --point->pointer;
        --point->num;
        if(*(point->pointer) == ' '){
            ++i;
        }
    }
    if (point->pointer != *string){
        point->pointer++;
        point->num++;
    }
}

void moverw(struct point *point, int num) {
    while(*(point->pointer) != '\0' && *(point->pointer) != ' '){
        ++point->pointer;
        ++point->num;
    }
    if(*(point->pointer) == '\0'){ return;}
    int i = 0;
    while(i != num && *(point->pointer) != '\0'){
        ++point->pointer;
        ++point->num;
        if(*(point->pointer) == ' '){
            ++i;
        }
    }
    --point->num;
    --point->pointer;
}

void find(char **string, char *to_find, char ***to_light, struct point *main_point) {
    *to_light = malloc(sizeof(char*));
    int len_light = 1;
    int len_find = strlen(to_find);
    char* pointer = strstr(*string, to_find);
    if (pointer != NULL){
        main_point->num = pointer - *string;
        main_point->pointer = pointer;
    }
    while (pointer != NULL){
        (*to_light)[len_light - 1] = pointer;
        len_light++;
        *to_light = realloc(*to_light, sizeof(char*) * len_light);
        pointer = strstr(pointer + len_find, to_find);
    }
    (*to_light)[len_light - 1] = NULL;
}

char * replace(char **string, char *to_replace, char *on_replace, struct point *main_point) {
    int len_to = strlen(to_replace);
    char* find_pointer = strstr(*string, to_replace);
    int ans_pos;
    if (find_pointer == NULL){
        ans_pos = -1;
    } else {
        ans_pos = find_pointer - *string;
    }
    if (strcmp(to_replace, on_replace) == 0){
        find_pointer = NULL;
    }
    while (find_pointer != NULL){
        main_point->pointer = find_pointer + len_to;
        main_point->num = find_pointer - *string + len_to;
        del(string, main_point, len_to);
        put(string, on_replace, main_point);
        find_pointer = strstr(main_point->pointer, to_replace);
    }
    if (ans_pos != -1){
        main_point->num = ans_pos;
        main_point->pointer = *string + ans_pos;
        return *string + ans_pos;
    }
    return NULL;
}