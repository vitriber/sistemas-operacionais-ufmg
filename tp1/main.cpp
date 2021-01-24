#include <iostream>
#include <string>
#include <stdio.h>
#include <list>
#include <vector> 
#include <thread>  
#include <sstream>
#include <mutex>
#include <unistd.h>
#include <condition_variable>
#include <algorithm>

using namespace std;

int it;

struct Friend {   
    string name;
    int idx;
    bool isqueue;
    int pair;      
};

// list<string> names = {"Sheldon", "Amy", "Leonard", "Penny", "Howard", "Bernadette", "Stuart", "Kripke"};
list<string> names = {"Sheldon", "Leonard", "Kripke"};

vector<Friend> friends;
vector<Friend> queue;

std::mutex oven;
std::condition_variable rules;

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

void friends_func(Friend f) {      
    
    int j = 0;      
    
    while(j < it) {                            


        cout << "💁‍♂️ - " << f.name << " quer usar o forno \n" << endl;
        f.isqueue = true;
        queue.push_back(f);   
        std::this_thread::sleep_for(std::chrono::seconds(3));

        std::unique_lock<std::mutex> locker(oven);
    
        if (!ready(f)) rules.wait(locker); else rules.notify_one();                                           
        
        cout << "🔥 - " << f.name << " começa a esquentar algo \n" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));

        locker.unlock();   
        
        remove(f);
        
        cout << "🍲 - " << f.name << " vai comer \n" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(8));
        
        cout << "💻 - " << f.name << " voltou para o trabalho \n" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(4));

        j++;
    }
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

    std::vector<std::thread> threads;

    for (std::list<string>::iterator i = names.begin(); i != names.end(); i++)
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
        f.idx = indice;
        f.isqueue = false;
        indice++; 
        friends.push_back(f);
    }

    for (Friend x : friends)            
        threads.push_back(std::thread(friends_func, x));          

    for (auto& th : threads) th.join();

    return 0;
}