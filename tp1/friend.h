#pragma once

#include <string>
#include <pthread.h>

using namespace std; 


class Friend {
    public: 
        int idx;
        int pair; 
        string name;
        
        pthread_t thread; 
        pthread_cond_t rules;  // controlar as regras de precedência no acesso direto ao forno
        pthread_cond_t second_pair; // variável  para  enfileirar  o  segundo  membro  de  um  casal  se  o  outro  já  estiver esperando 

        void eat_something();
        void eat();
        void back_to_work();

        Friend(){
            this->idx = 0;
            this->pair = 0;
            this->name = "";
        };
        
        Friend(string name) {
            this->name = name;
        }
};