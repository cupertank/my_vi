#ifndef TASK3_C_4_ITERATION_COMMANDS_H
#define TASK3_C_4_ITERATION_COMMANDS_H
struct point {
    int num;
    char* pointer;
};

void render(char *string, struct point *point, char **to_light, int to_light_len);
void add(char** string, char* to_add, struct point* point);
void move(char **string, int to_move, struct point *point, int len);
void insert(char **string, char *to_insert, struct point insert_point, struct point* main_point);
void put(char** string, char* to_put, struct point* point);
void my_remove(char** string, struct point start, struct point end, struct point* point);
void del(char** string, struct point* point, int num);
void movelw(char** string, struct point* point, int num);
void moverw(struct point *point, int num);

void find(char **string, char *to_find, char ***to_light, struct point *main_point);
char * replace(char **string, char *to_replace, char *on_replace, struct point *main_point);
#endif //TASK3_C_4_ITERATION_COMMANDS_H
