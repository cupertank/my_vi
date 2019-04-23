#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>
#include "commands.h"

void my_getline(char** command){
    int len = 1;
    char temp = getchar();
    while(temp != '\n'){
        (*command)[len - 1] = temp;
        len++;
        *command = realloc(*command, len);
        temp = getchar();
    }
    (*command)[len - 1] = '\0';
}

int main() {
    char* string = malloc(1);
    string[0] = '\0';

    struct point main_point = {0, string};

    render(string, &main_point, NULL, 0);

    while(1){
        char* command = malloc(1);
        my_getline(&command);

        if(strstr(command, "exit") == command){
            free(command);
            break;
        } else if (strstr(command, "add") == command){
            add(&string, command + 4, &main_point);
        } else if (strstr(command, "movelw") == command){
            int num = strtol(command + 7, NULL, 10);
            movelw(&string, &main_point, num);
        } else if(strstr(command, "moverw") == command){
            int num = strtol(command + 7, NULL, 10);
            moverw(&main_point, num);
        } else if (strstr(command, "move") == command){
            move(&string, strtol(command + 5, NULL, 10), &main_point, strlen(string));
        } else if (strstr(command, "put") == command){
            put(&string, command + 4, &main_point);
        } else if (strstr(command, "insert") == command){
            char* before_word = strstr(command + 7, " ");
            int insert_pos = strtol(command + 7, &before_word, 10);
            if (insert_pos > strlen(string)){
                insert_pos = strlen(string);
            }
            if (strlen(string) == 0){
                insert_pos = 1;
            }
            struct point insert_point = {insert_pos - 1, string + insert_pos - 1};
            insert(&string, before_word + 1, insert_point, &main_point);
        } else if (strstr(command, "remove") == command){
            int str_len = strlen(string);
            char* before_word = strstr(command + 7, " ");
            int start = strtol(command + 7, &before_word, 10);
            int end = strtol(before_word + 1, NULL, 10);
            if (end < start || start < 1 || end < 1 || start > str_len || end > str_len){
                printf("Буууу, выход за границы");
                continue;
            }
            struct point start_point = {start - 1, string + start - 1};
            struct point end_point = {end - 1, string + end - 1};
            my_remove(&string, start_point, end_point, &main_point);
        } else if (strstr(command, "del") == command){
            if (strlen(command) == 3){
                del(&string, &main_point, 1);
            } else {
                int num = strtol(command + 4, NULL, 10);
                del(&string, &main_point, num);
            }
        } else if (strstr(command, "find") == command){
            char** to_light;
            find(&string, command + 5, &to_light, &main_point);
            render(string, &main_point, to_light, strlen(command + 5));
            free(command);
            free(to_light);
            continue;
        } else if (strstr(command, "replace") == command){
            char* before_word = strstr(command + 8, " ");
            *before_word = '\0';
            char* to_replace = command + 8;
            char* on_replace = before_word + 1;
            char** to_light = malloc(sizeof(char*) * 2);
            to_light[0] = replace(&string, to_replace, on_replace, &main_point);
            to_light[1] = NULL;
            render(string, &main_point, to_light, strlen(on_replace));
            *before_word = ' ';
            free(command);
            free(to_light);
            continue;
        } else if (strstr(command, "upcase") == command){
            *main_point.pointer = (char) toupper((int)*main_point.pointer);
        } else if (strstr(command, "lowcase") == command){
            *main_point.pointer = (char) tolower((int)*main_point.pointer);
        } else if (strstr(command, "load") == command){
            free(string);
            string = malloc(1);
            string[0] = '\0';
            main_point.num = 0; main_point.pointer = string;
            char buffer[100];
            FILE *from_load = fopen(command + 5, "r");
            while (!feof(from_load)){
                fgets(buffer, 100, from_load);
                string = realloc(string, strlen(string) + 100 + 1);
                main_point.pointer = string;
                strcat(string, buffer);
            }
            fclose(from_load);
        } else if (strstr(command, "save") == command){
            FILE* save_point = fopen(command + 5, "w");
            fputs(string, save_point);
            fclose(save_point);
        }
        render(string, &main_point, NULL, 0);
        free(command);
    }
    free(string);
    return 0;
}