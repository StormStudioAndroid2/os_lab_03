#include <pthread.h>
#include <stdbool.h>
#include <ctype.h>
#include "linked_list.h"
// лаба 3, вариант 11
// поиск кратчайшего пути в графе поиском в ширину
// граф задается матрицей смежности


char change_gamer(char c) {
    if (c=='x') {
        return '0';
    }
    return 'x';
}
linkedList getFreeCells(char matrix[3][3]) {
    linkedList d;
    init(d);
    for (int i = 0; i<3; ++i) {
        for (int j = 0; j<3; ++j) {
            if (matrix[i][j]=='*') {
                AddList(d,i*3+j);
            }
        }
    }

    return d;
}
int freeCells(char matrix[3][3]) {
    int count = 0;
    for (int i = 0; i < 3; ++i ) {
        for (int j = 0; j < 3; ++j) {
            if (matrix[i][j]=='*') {
                count++;
            }
        }
    }
    return count;
}
char checkGame(char matrix[3][3]) {
    if (matrix[0][0]==matrix[1][1] && matrix[1][1]==matrix[2][2] && matrix[0][0]!='*') {
        return matrix[0][0];
    }
    for (int i = 0; i<3; ++i) {
        if (matrix[i][0]==matrix[i][1] && matrix[i][1]==matrix[i][2] && matrix[i][2]!='*') {
            return matrix[i][0];
        }
        if (matrix[0][i]==matrix[1][i] && matrix[1][i]==matrix[2][i] && matrix[2][i]!='*') {
            return matrix[0][i];
        }

    }
    if (matrix[0][2]==matrix[1][1] && matrix[1][1]==matrix[2][0] && matrix[0][2]!='*') {
        return matrix[0][2];
    }
    return ' ';
}
int cells[9];
typedef struct Data data;

typedef struct Data {
    char optimal_gamer;
    char now_gamer;
    char matrix[3][3];
    linkedList list;
    pthread_mutex_t *thread_count_mutex;
     int *max_thread_count;
    pthread_mutex_t *mutex;
    int cell;
};
int min(int lhs, int rhs) {
    return lhs > rhs ? rhs : lhs;
}

//каждый раз новый поток принимает thread_params
//current_vertex - текущая вершина в обходе
//result - там хранятся подсчитанные расстояния до вершин, или -1 если не подсчитано
//max_thread_count - эта переменная уменьшается при создании нового потока,
//таким образом будет создано столько потоков сколько нужно
//mutex - мьютекс для синхронизации доступа к result
//list - список смежности для графа, будем считать, что граф связный
void *find_optimal(void *args) {
    data* data1 = (data*) args;

   


    pthread_mutex_lock(data1->thread_count_mutex);
    int quantityThreads = min(*(data1->max_thread_count), freeCells(data1->matrix));
    *(data1->max_thread_count) -= quantityThreads;
    pthread_mutex_unlock(data1->thread_count_mutex);

    pthread_t* threads = malloc(sizeof(pthread_t) * quantityThreads);
    data* new_params = malloc(sizeof(data) * quantityThreads);
            pthread_mutex_lock(data1->mutex);

    for (int i = 0; i < quantityThreads-1 && i<getLength(data1->list); ++i) {
        data1->matrix[get(data1->list,0)/3][get(data1->list,0)%3] = data1->now_gamer;
        if (checkGame(data1->matrix)!=' ') {
            if (checkGame(data1->matrix)==data1->optimal_gamer) {
                cells[(data1->cell!=-1 ? data1->cell : get(data1->list,0))]+=10;
            } else {
                cells[(data1->cell!=-1 ? data1->cell : get(data1->list,0))]-=10;  
            }
        }
        new_params[i] = *data1;
        new_params[i].now_gamer = change_gamer(data1->now_gamer);
        new_params[i].cell = (data1->cell!=-1 ? data1->cell : get(data1->list,0));
        new_params[i].list = getFreeCells(data1->matrix);
        pthread_create(&threads[i], NULL, find_optimal, &new_params[i]);
        data1->matrix[get(data1->list,i)/3][get(data1->list,i)%3] = data1->now_gamer;
        DeleteFromList(data1->list,0);

    }
    if (getLength(data1->list)!=0) {
         new_params[quantityThreads-1] = *data1;
        new_params[quantityThreads-1].cell = (data1->cell!=-1 ? data1->cell : get(data1->list,quantityThreads-1));
        new_params[quantityThreads-1].list = getFreeCells(data1->matrix);
        pthread_create(&threads[quantityThreads-1], NULL, find_optimal, &new_params[quantityThreads-1]);
    }
    
   


    for (int i = 0; i < quantityThreads; ++i) {
        pthread_join(threads[i], NULL);
    }
    pthread_mutex_lock(data1->thread_count_mutex);
    data1->max_thread_count += quantityThreads;
    pthread_mutex_unlock(data1->thread_count_mutex);
    free(new_params);
    free(threads);
    DeleteFromList(data1->list,*data1->list.head);    
    pthread_exit(NULL);
}

int parse_count(const char* str) {
    for (int i = 0; str[i] != '\0'; ++i) {
        if (!isdigit(str[i])) {
            return -1;
        }
    }
    return atoi(str);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Not enough arguments\n");
        exit(1);
    }
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Wrong filename\n");
        exit(1);
    }
    int *max_thread_count = malloc(sizeof(int));
    *max_thread_count = parse_count(argv[2]);
    if (*max_thread_count <= 0) {
        printf("Incorrect thread count\n");
        exit(1);
    }
    char matrix[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            char c = fgetc(file);
            if (c=='\n') break;
            matrix[i][j]=c;
        }
    }
    char a;
    fscanf(file, " %c", &a);

    fclose(file);
    
    int n;
    fscanf("%d", &n);
    pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t) * n);
    pthread_mutex_init(mutex, NULL);
    pthread_mutex_t* thread_count_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(thread_count_mutex, NULL);

   
    (*max_thread_count)--;

    data parameters = (data) {
             .max_thread_count = max_thread_count,
             .now_gamer = a,
             .optimal_gamer = a,  
            .list = getFreeCells(matrix),
            .mutex = mutex,
            .cell = -1,
            .thread_count_mutex = thread_count_mutex};

    pthread_t* first_thread = malloc(sizeof(pthread_t));
    pthread_create(first_thread, NULL, find_optimal, &parameters);
    pthread_join(*first_thread, NULL);
    int max = cells[0];
    for (int i = 1; i<9; ++i) {
        if (cells[i]>max) {
            max = cells[i];
        }
    }
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i <3; ++i) {
            for (int j = 0; j<3; ++i) {
                if (i*3+j==k) {
                    printf("%c",a);
                } else {
                    printf("%c",matrix[i][j]);
                }
            }
            printf("\n");
        }
    }
    return 0;
}
