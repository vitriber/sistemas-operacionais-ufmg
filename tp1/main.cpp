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

#define NUM_THREADS  2

using namespace std;

void *init_friend(void *arg);
int ramdomNumber(int top);
void remove(Friend f);
void releaseAll();


int useForno;
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

    // Inicialização da MUTEX
    if (pthread_mutex_init(&forno.oven, NULL) != 0)
    {
        perror("Falha na inicialização do mutex.\n");
        return 0;
    }

    // Criando vetor de structs do Tipo Friends para inicialização das Threads
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

    // Liberando as Threads enquanto elas estiverem na fila
    while(forno.queue.size() > 0){
        releaseAll();
    }       

    pthread_mutex_destroy(&forno.oven);
    pthread_exit(NULL);

    return 1;
}

int ramdomNumber(int top) {
        srand(time(NULL));
        int low = 1;
       return rand()%(top-low+1) + low;
}

void remove(Friend f) { 
    for( std::vector<Friend>::iterator iter = forno.queue.begin(); iter != forno.queue.end(); iter++ )
    {
        Friend fr = *iter;

        if(fr.name == f.name)
        {            
            forno.queue.erase( iter );
            break;
        }        
    }  
}

void releaseAll() {
    for( std::vector<Friend>::iterator iter = friends.begin(); iter != friends.end(); iter++ )
    {
        Friend fr = *iter;
        pthread_cond_signal(&fr.rules);
        pthread_cond_signal(&fr.second_pair);

    }
}

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
