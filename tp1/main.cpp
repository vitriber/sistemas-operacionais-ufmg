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
    else if(f.name == "Bernadete")
    {
        names.push_back("Sheldon");    
        names.push_back("Leonard");    
        names.push_back("Howard");
        names.push_back("Penny");
        names.push_back("Amy");
    }
    else if(f.name == "Penny")
    {
        names.push_back("Sheldon");
        names.push_back("Leonard");    
        names.push_back("Howard");        
        names.push_back("Amy");
    }
    else if(f.name == "Amy")
    {
        names.push_back("Sheldon");
        names.push_back("Leonard");    
        names.push_back("Howard");
    }
    else if(f.name == "Leonard")
        names.push_back("Howard");    
    else if(f.name == "Stuart")
    {
        names.push_back("Sheldon");    
        names.push_back("Leonard");    
        names.push_back("Howard");    
        names.push_back("Amy");
        names.push_back("Penny");
        names.push_back("Bernadete");
    }
    else if(f.name == "Kripke")
    {
        names.push_back("Sheldon");    
        names.push_back("Leonard");    
        names.push_back("Howard"); 
        names.push_back("Stuart");
        names.push_back("Amy");
        names.push_back("Penny");
        names.push_back("Bernadete");
    }

    cout << "---------------------- \n" << endl;       

    cout << f.name << " preparado? \n" << endl;

    for (Friend x : queue)
        for (string n : names)         
            if(n == x.name)
            {                
                cout << x.name << " superior \n\n" << endl;
                cout << "---------------------- \n" << endl;
                return false;
            }                

    cout << f.name << " está preparado \n\n" << endl;
    cout << "---------------------- \n" << endl;

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

        std::unique_lock<std::mutex> locker(oven); //, std::defer_lock);                                           

        cout << f.name << " quer usar o forno \n" << endl;        
        sleep(3);

        /*if (locker.try_lock()) 
        {
            sleep(1);
            cout << "\nRAJ\n" << endl;                    
            cout << "Nome: " << f.name << "\n" << endl;
            //locker.unlock();     
        }            
        else            
        {
            sleep(1);
            cout << "\nSEM DEADLOCK\n" << endl;
            cout << "Nome: " << f.name << "\n" << endl;
        } */           
    
        if (!ready(f)) 
        {
            rules.wait(locker);            
        }        
        else 
        {
            rules.notify_one();                   
        }

        if (locker.try_lock())  //true can be left out
            std::cout << "lock acquired" << std::endl;
        else
            std::cout << "lock notacquired" << std::endl;

        locker.unlock();           
        
        cout << f.name << " começa a esquentar algo \n" << endl;
        sleep(1); 
        
        remove(f);
        
        cout << f.name << " vai comer \n" << endl;
        sleep(6);                        
        
        cout << f.name << " voltou para o trabalho \n" << endl;
        sleep(4);         

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