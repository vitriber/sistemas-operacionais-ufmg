#include <iostream>	
#include <string>	
#include <stdio.h>	
#include <vector> 	 	
#include <sstream>		
#include <unistd.h>		
#include <algorithm>
#include <pthread.h>


using namespace std;

int it;

struct Friend {
    pthread_t thread;  
    string name;
    int idx;
    bool isqueue;
    int pair;      
};

pthread_mutex_t oven;

vector<Friend> friends;
vector<Friend> queue;

pthread_cond_t rules;
vector<string> names = {"Sheldon", "Leonard"};
// vector<string> names = {"Sheldon", "Amy", "Leonard", "Penny", "Howard", "Bernadette", "Stuart", "Kripke"};

pthread_mutex_t oven;
pthread_cond_t rules;

bool ready(Friend f) {    

    vector<string> names;
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

    for (Friend x : queue)
        for (string n : names)         
            if(n == x.name)                           
                return false;                          
    return true;
}

void remove(Friend f) { 
    for( std::vector<Friend>::iterator iter = queue.begin(); iter != queue.end(); iter++ )
    {
        Friend fr = *iter;

        if(fr.name == f.name)
        {            
            queue.erase( iter );
            break;
        }        
    }  
}

void *friends_func(void *arg) {

    Friend *p_ptr = (Friend*)arg; 
    
    int j = 0;      
    

    cout << "💁‍♂️ - " << p_ptr->name << " quer usar o forno \n" << endl;
    p_ptr->isqueue = true;
    queue.push_back(*p_ptr);   
    sleep(3);

    pthread_mutex_lock(&oven);                                          
    
    cout << "🔥 - " << p_ptr->name << " começa a esquentar algo \n" << endl;
    
    sleep(1);
    pthread_mutex_unlock(&oven); 
    
    remove(*p_ptr);
    
    cout << "🍲 - " << p_ptr->name << " vai comer \n" << endl;
    sleep(6);
    
    cout << "💻 - " << p_ptr->name << " voltou para o trabalho \n" << endl;
    sleep(4); 

    j++;
}

void verify(Friend f) {
    sleep(5);

    srand((unsigned int)time(NULL));
    int ramdom = drand48() * 5;

    //pthread_cond_signal(&rules);
}


int main(int argc, char *argv[] )
{
    int indice = 0;

    if( argc != 2 ) {
      cout << "Error: Informar, como parâmetro, quantidade de iteração.\n" << endl;
      return 1;
    }    

    it = atoi(argv[1]);    

    
    if (pthread_mutex_init(&oven, NULL) != 0)
    {
        perror("Falha na inicialização do mutex.\n");
        return 0;
    }

    for (std::vector<string>::iterator i = names.begin(); i != names.end(); i++)
    {
        Friend f;
        f.name = "";
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
        f.idx = indice;
        f.isqueue = false;
        indice++; 
        friends.push_back(f);
    }

    for (Friend x : friends)
        pthread_create(&x.thread, NULL, friends_func, &x);          

    pthread_exit(NULL);

    return 1;
}