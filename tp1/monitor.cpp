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


using namespace std; 

void Monitor::esperar(Friend f) {
    cout << f.name << " quer usar o forno" << endl;
    queue.push_back(f);
    f.isQueue = true;
        
    sleep(1+ rand()%5);
}

void Monitor::verificar() {
    sleep(5);

    // Verificar quem está na fila e ver se há deadlock
    for (Friend x : queue) {
        if(x.name == "Stuart" || x.name == "Kripke"){

        }
    }

    // Liberar um personagem do Deadlock

}

//Libera proximo par
void Monitor::realizePair(int num){
    for (Friend x : queue) {
        if(x.pair != num){
            pthread_cond_wait(&x.second_pair, &oven);
        }
        else {
            pthread_cond_signal(&x.second_pair);
        }
    }
}


bool Monitor::ready(Friend f) {  

    vector<string> names;

    // Verificando se existe casal na fila
    int pair1 = 0;
    int pair2 = 0;
    int pair3 = 0;

    for (Friend x : queue) {
        if(x.pair == 1){
            pair1++;
        }else if(x.pair == 2){
            pair2++;
        }else if(x.pair == 3){
            pair3++;
        } 
    }

    //Libera a thread casal para usar o forno e enfilera a proxima thread do casal
    if(pair1 == 2) {
        if(f.pair == 1){
            realizePair(1);
            return true;
        }
    }

     if(pair2 == 2) {
        if(f.pair == 2){
            realizePair(2);
            return true;
        }
    }

     if(pair3 == 2) {
        if(f.pair == 3){
            realizePair(2);
            return true;
        }
    }

    // Verifica ordem de prioridades
    if(f.name == "Sheldon")    
        names.push_back("Leonard");
    else if(f.name == "Howard")
        names.push_back("Sheldon");    
    else if(f.name == "Leonard")
        names.push_back("Howard");    
    else if(f.name == "Stuart")
    {
        names.push_back("Sheldon");    
        names.push_back("Leonard");    
        names.push_back("Howard");                            
    }
    else if(f.name == "Kripke")
    {
        names.push_back("Sheldon");    
        names.push_back("Leonard");    
        names.push_back("Howard"); 
        names.push_back("Stuart");        
    }    

    for (Friend x : queue) {
        for (string n : names) {         
            if(n == x.name){                         
                return false;
            }
        } 
    }                      
    return true;
}