#include "friend.h"

#include <unistd.h>
#include <iostream> 
#include <stdlib.h> 
#include <random>
#include <string.h>

using namespace std; 


void Friend::eat_something() {
    cout << this->name << " começa a esquentar algo" << endl;
    sleep(4);
}

void Friend::eat() {
    cout << this->name << " vai comer" << endl;
    sleep(6);
}

void Friend::back_to_work() {
    cout << this->name << " voltou para o trabalho" << endl;
    sleep(4);;
}