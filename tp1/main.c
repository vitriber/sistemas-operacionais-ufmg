#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include<string.h>

typedef struct {
    const char *name;
    pthread_t thread;
} friend;

pthread_mutex_t oven;
pthread_cond_t queue;
pthread_cond_t rules;
int it;

void *friends_func(void *arg) {      

    friend *p_ptr = (friend*)arg; 
    int j = 0;       
    
    //printf("Thread %s – value %ld\n", p_ptr->name, pthread_self());

    while(j < it) {       
         
        printf("%s quer usar o forno\n", p_ptr->name);
        sleep(3);
 
        printf("%s começa a esquentar algo\n", p_ptr->name);
        sleep(1);
 
        pthread_mutex_lock(&oven);         
        printf("%s vai comer\n", p_ptr->name);
        sleep(6);
        pthread_mutex_unlock(&oven);      
                
        printf("%s voltou para o trabalho\n", p_ptr->name);
        sleep(4);         

        j++;
    }
    
}

void main( int argc, char *argv[] )  {        

    if( argc != 2 ) {
      printf("Error: Informar, como parâmetro, quantidade de iteração.\n");
      return;
   }

    it = atoi(argv[1]);   

    //const char *nameList[] = { "Sheldon", "Leonard", "Howard", "Stuart", "Kripke", "Any", "Bernadette", "Penny" };
    const char *nameList[] = { "Sheldon", "Leonard"};
    int sizeList = sizeof nameList / sizeof nameList[0];
    int i = 0;
    
    friend** friends = malloc(sizeof(*friends)*sizeList);
    

    if (pthread_mutex_init(&oven, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return;
    }

    for (i = 0; i < sizeList; i++) {        
        friends[i] = malloc(sizeof(*friends[i]));        
        friends[i]->name = nameList[i];        
        pthread_create(&friends[i]->thread, NULL, friends_func, friends[i]);        
    }
    
    pthread_exit(NULL);
};