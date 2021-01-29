#include "friend.h"
#include "monitor.h"

#include <iostream>	
#include <string>	
#include <stdio.h>	
#include <vector> 	 	
#include <sstream>		
#include <unistd.h>		
#include <algorithm>
#include <pthread.h>
#include <algorithm>
#include <random>

#define NUM_THREADS  8

using namespace std;

void *init_friend(void *arg);
void *raj_run(void* flag);
void remove(Friend f);
void releaseAll();

int useForno;
bool execution_raj = true;

Monitor forno;
vector<Friend> friends;
vector<string> names = {"Sheldon", "Amy", "Kripke", "Leonard", "Penny", "Howard", "Bernadette", "Stuart"};


int main(int argc, char *argv[] )
{
    if( argc != 2 ) {
      perror("Error: Informar, como parâmetro, quantidade de iteração.\n");
      return 0;
   }

    useForno = atoi(argv[1]); 

    //inicialização thread raj
    pthread_t raj;

    if (pthread_create(&raj, NULL, raj_run, &execution_raj) < 0) {
        perror("Falha na inicialização da thread");
    }   

    // inicialização da mutex
    if (pthread_mutex_init(&forno.oven, NULL) != 0)
    {
        perror("Falha na inicialização do mutex.\n");
        return 0;
    }

    // criando vetor de structs do tipo friends para inicialização das threads
    for (std::vector<string>::iterator i = names.begin(); i != names.end(); i++)
    {
        Friend f;
        f.name = *i;

        if(f.name == "Sheldon" || f.name == "Amy"){
            f.pair = 1;
        }else if(f.name == "Howard" || f.name == "Bernadette"){
            f.pair = 2;
        }else if(f.name == "Leonard" || f.name == "Penny"){
            f.pair = 3;
        }else{
            f.pair = 0;
        }

        pthread_cond_init (&f.rules, NULL);
        pthread_cond_init (&f.second_pair, NULL);

        friends.push_back(f);
    }

    // embaralhar lista friends
    std::shuffle(friends.begin(), friends.end(), std::random_device());

    // Criando as threads de acordo com a lista de Friends
    for (int i = 0; i < NUM_THREADS; i++){
        pthread_create(&friends[i].thread, NULL, init_friend, &friends[i]);
    }

    // liberando as threads enquanto elas estiverem na fila
    while(forno.queue.size() > 0){
        releaseAll();
    }

    execution_raj = false;       

    pthread_mutex_destroy(&forno.oven);
    pthread_exit(NULL);

    return 0;
}


//remove pessoa da fila
void remove(Friend f) { 
    for( std::vector<Friend>::iterator iter = forno.queue.begin(); iter != forno.queue.end(); iter++ )
    {
        Friend fr = *iter;

        fr.isQueue = true;

        if(fr.name == f.name)
        {            
            forno.queue.erase( iter );
            break;
        }        
    }  
}

//libera todas as pessoas
void releaseAll() {
    for( std::vector<Friend>::iterator iter = friends.begin(); iter != friends.end(); iter++ )
    {
        Friend fr = *iter;
        pthread_cond_signal(&fr.rules);
        pthread_cond_signal(&fr.second_pair);

    }
}

void *raj_run(void* flag) {
    bool execution_raj = ((bool*) flag);
    while (execution_raj) {
        sleep(5);
        forno.verificar();
    }
    return NULL;
}

// cria rotina de execucao
void *init_friend(void *arg) {

    Friend *f = (Friend*)arg;
    
    int j = 0;
    
    while(j < useForno) {
        forno.esperar(*f);
 
        if (!forno.ready(*f)) {
            pthread_cond_wait(&f->rules, &forno.oven);
        }

        pthread_mutex_lock(&forno.oven);     
        f->eat_something();                               
        pthread_mutex_unlock(&forno.oven); 
        
        remove(*f);

        f->eat();
        f->back_to_work();
        
        j++;
    }

    return NULL;
}

