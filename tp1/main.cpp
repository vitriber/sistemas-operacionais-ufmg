#include <iostream>
#include <string>
#include <vector> 
#include <unistd.h>
#include <pthread.h>

using namespace std;

struct Friend {   
    string name;
    pthread_t thread;
};

pthread_mutex_t oven;
vector<Friend> queue;
pthread_cond_t rules;
vector<string> names = {"Sheldon", "Leonard"};
int it;

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

    cout << p_ptr->name << " quer usar o forno \n" << endl;
    sleep(3);

    pthread_mutex_lock(&oven); 
    cout << p_ptr->name << " começa a esquentar algo \n" << endl;
    sleep(1);
    pthread_mutex_unlock(&oven);       
    
    cout << p_ptr->name << " vai comer \n" << endl;
    sleep(6);        
            
    cout << p_ptr->name << " voltou para o trabalho \n" << endl;        
    sleep(4);         

    j++;
}

void start(){

    Friend leonard;
    leonard.name = "Leonard";           
    pthread_create(&leonard.thread, NULL, friends_func, &leonard);        

    Friend sheldon;
    sheldon.name = "Sheldon";           
    pthread_create(&sheldon.thread, NULL, friends_func, &sheldon);
}

int main( int argc, char *argv[] )  {        

    if( argc != 2 ) {
      perror("Error: Informar, como parâmetro, quantidade de iteração.\n");
      return 0;
   }

    it = atoi(argv[1]);   
    
    int i = 0;

    if (pthread_mutex_init(&oven, NULL) != 0)
    {
        perror("Falha na inicialização do mutex.\n");
        return 0;
    }

    start();    
    
    pthread_exit(NULL);

    return 1;
};