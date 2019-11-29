#include <pthread.h>
#include <stdbool.h>
#include <ctype.h>
#include "linked_list.h"
#include "stdlib.h"
// лаба 3, вариант 11
// поиск кратчайшего пути в графе поиском в ширину
// граф задается матрицей смежности


char change_gamer(char c) {
    if (c=='x') {
        return '0';
    }
    return 'x';
}
linkedList* list;

int freeCells(char matrix[9]) {
    int count = 0;
    for (int i = 0; i < 9; ++i ) {
            if (matrix[i]=='*') {
                count++;
            }
        
    }
    return count;
}
char checkGame(char* matrix) {
    if (matrix[0]==matrix[1*3+1] && matrix[1*3+1]==matrix[2*3+2] && matrix[0]!='*') {
        return matrix[0];
    }
    for (int i = 0; i<3; ++i) {
        if (matrix[i*3+0]==matrix[i*3+1] && matrix[i*3+1]==matrix[i*3+2] && matrix[i*3+2]!='*') {
            return matrix[i*3+0];
        }
        if (matrix[0*3+i]==matrix[1*3+i] && matrix[1*3+i]==matrix[2*3+i] && matrix[2*3+i]!='*') {
            return matrix[0*3+i];
        }

    }
    if (matrix[0*3+2]==matrix[1*3+1] && matrix[1*3+1]==matrix[2*3+0] && matrix[0*3+2]!='*') {
        return matrix[0*3+2];
    }
    return 0;
}
int cells[9];
typedef struct Parameter Parameter;

typedef struct Parameter {
    char optimal_gamer;
    linkedList* list;
    Data* element;
    pthread_mutex_t *thread_count_mutex;
     int *max_thread_count;
    pthread_mutex_t *mutex_list;
    int cell;
};
int min(int lhs, int rhs) {
    return lhs > rhs ? rhs : lhs;
}


void *find_optimal(void *args) {

    Parameter* parameter = (Parameter*) args;
    char isWin = ' ';
    int isBoolWin = 0;
    Data* d = parameter->element;
 
    for (int i = 0; i<9; ++i) {
        if (d->matrix[i]=='*') {
            d->matrix[i] =  d->nowGamer;
            isWin = checkGame(d->matrix);
             if (isWin==parameter->optimal_gamer) {
                cells[(d->cell!=-1 ? d->cell : i)]+=1;
                isBoolWin = 1;
             }
            if (isWin==change_gamer(parameter->optimal_gamer)) {
                cells[(d->cell!=-1 ? d->cell : i)]-=1;
                isBoolWin = 1;

             }
             d->matrix[i] = '*';
        }
    }
    
  
    if (isBoolWin==0 && freeCells(d->matrix)!=0) { 
        for (int i = 0; i<9; ++i) {
            if (d->matrix[i]=='*') {
                d->matrix[i]=d->nowGamer;
                int k = d->cell!=-1? d->cell:i;
                AddList(d->matrix,k,change_gamer(d->nowGamer));
                d->matrix[i]='*';

            }
        }
      
        pthread_mutex_lock(parameter->thread_count_mutex);
        int quantityThreads = min(*(parameter->max_thread_count), getLength(parameter->list)-1);
        *(parameter->max_thread_count) -= quantityThreads;
        pthread_mutex_unlock(parameter->thread_count_mutex);
        pthread_mutex_lock(parameter->mutex_list);

        pthread_t* threads = malloc(sizeof(pthread_t) * (quantityThreads));
        Parameter* new_params = malloc(sizeof(Parameter) * (quantityThreads));
        for (int i = 0; i<quantityThreads; ++i) {
            new_params[i] = *parameter;
            new_params[i].element = get(0);
            DeleteFromList(0);

        }
                pthread_mutex_unlock(parameter->mutex_list);
        for (int i = 0; i<quantityThreads; ++i) {
            pthread_create(&threads[i], NULL, find_optimal, &new_params[i]);

        }
      
        for (int i = 0; i < quantityThreads; ++i) {
            pthread_join(threads[i], NULL);
                     pthread_mutex_lock(parameter->thread_count_mutex);

             parameter->max_thread_count++;
                     pthread_mutex_unlock(parameter->thread_count_mutex);

        }
       
        free(new_params);
        free(threads);
        free(parameter->element);
      

    }


  
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
    for(int i = 0; i<9; ++i) {
        cells[i] = 0;
    }
    char matrix[9];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            char c = fgetc(file);
            if (c=='\n') {
                j--;
                break;
            };
            matrix[i*3+j]=c;
        }
        fgetc(file);
    }
    char a;
    fscanf(file, " %c", &a);
    
    fclose(file);
    init();
    int n;

    pthread_mutex_t *mutex_list = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mutex_list, NULL);
    pthread_mutex_t* thread_count_mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(thread_count_mutex, NULL);

     
    (*max_thread_count)--;
    int boolNull = 1;
    for (int i = 0; i<9;++i) {
        if (matrix[i]!='*') {
            boolNull = 0;
        }
    }
    if (boolNull==0) {
    Parameter parameters = (Parameter) {
             .max_thread_count = max_thread_count,
             .optimal_gamer = a,  
            .list = list,
            .mutex_list = mutex_list,
            .cell = -1,
            .thread_count_mutex = thread_count_mutex};
  
    AddList(matrix,-1,a);
      while (getLength()!=0) {
            pthread_t* threads = malloc(sizeof(pthread_t));


            Parameter new_params = *(Parameter*)(malloc(sizeof(Parameter)));
            new_params = parameters;
            new_params.element = get(0);
            DeleteFromList(0);
            pthread_create(threads, NULL, find_optimal, &new_params);
            pthread_join(*threads,NULL);
        }
    } else {
        cells[1*3+1] = 1000;
    }
    int max = -30000;
    for (int i = 1; i<9; ++i) {
        if (cells[i]>max  && matrix[i]=='*') {
            max = cells[i];
        }
    }
        for (int k = 0; k < 9; ++k) {
            if (cells[k]==max) {
        for (int i = 0; i<3; ++i) {
        for (int j = 0; j<3; ++j) {
            if (k==i*3+j && cells[k]==max) {
                printf("%c",a);
            } else {
                printf("%c",matrix[i*3+j]);
            }
        }
        printf("\n");
    }
    printf("\n");
    }
    } 
   
    free(max_thread_count);
    free(mutex_list);
    free(thread_count_mutex);
    //     for (int i = 0; i <3; ++i) {
    //         for (int j = 0; j<3; ++i) {
    //             if (i*3+j==k) {
    //                 printf("%c",a);
    //             } else {
    //                 printf("%c",matrix[i][j]);
    //             }
    //         }
    //         printf("\n");
    //     }
    //     }
    // }
    return 0;

}
