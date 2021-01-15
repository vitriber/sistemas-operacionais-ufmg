#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct {
    const char *name;
    pthread_t thread;
} friend;

pthread_mutex_t forno;

pthread_cond_t enfileirar_membro;
pthread_cond_t regras_precedencia;

void *friends_func(void *arg) {

    friend *p_ptr = NULL;
    p_ptr = (friend*)arg;

    
    printf("Thread %s – value %ld\n", p_ptr->name, pthread_self());
    
}

void main(void)
{
    const char *nameList[] = { "Sheldon", "Leonard", "Howard", "Stuart", "Kripke", "Any", "Bernadette", "Penny" };
    friend** friends = malloc(sizeof(*friends)*5);

    int i;

    for (i = 0; i < 9; i++) {
        friends[i] = malloc(sizeof(*friends[i]));
        friends[i]->name = nameList[i];
        pthread_create( &friends[i]->thread, NULL, friends_func, friends[i]);
    }
 
    // for(i=0; i<2; i++) {
    //     pthread_join(threads[i], NULL);
    // }
};