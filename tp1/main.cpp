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
};

list<string> names = {"Sheldon", "Leonard", "Kripke"};  
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

        std::unique_lock<std::mutex> locker(oven);

        cout << f.name << " quer usar o forno \n" << endl;        
        std::this_thread::sleep_for(std::chrono::seconds(3));
    
        if (!ready(f)) rules.wait(locker); else rules.notify_one();                                   

        locker.unlock();           
        
        cout << f.name << " começa a esquentar algo \n" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        remove(f);
        
        cout << f.name << " vai comer \n" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(6));
        
        cout << f.name << " voltou para o trabalho \n" << endl;
        std::this_thread::sleep_for(std::chrono::seconds(4));

        j++;
    }
}


int main(int argc, char *argv[] )
{
    if( argc != 2 ) {
      cout << "Error: Informar, como parâmetro, quantidade de iteração.\n" << endl;
      return 1;
    }    

    it = atoi(argv[1]);    

    std::vector<std::thread> threads;

    for (std::list<string>::iterator it = names.begin(); it != names.end(); it++)
    {
        Friend f;
        f.name = *it; 
        queue.push_back(f);
    }

    for (Friend x : queue)            
        threads.push_back(std::thread(friends_func, x));            

    for (auto& th : threads) th.join();

    return 0;
}