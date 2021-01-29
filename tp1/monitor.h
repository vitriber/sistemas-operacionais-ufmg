#pragma once

#include "friend.h"

#include <pthread.h>
#include <vector>

class Monitor {
    public:
        pthread_mutex_t oven;
        vector<Friend> queue;

        void esperar(Friend f);
        bool ready(Friend f);
        void verificar();
        void realizePair(int num);
};