#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "zemaphore.h"

pthread_mutex_t agent_zem, tobacco_ready, match_ready, paper_ready, tobacco_zem, match_zem, paper_zem;
pthread_mutex_t mutex;
int is_tobacco, is_match, is_paper;

void *agentA(void *data){
    while(1){
        pthread_mutex_lock(&agent_zem);
        printf("Tobacco and Paper is ready.\n");
        pthread_mutex_unlock(&tobacco_ready);
        pthread_mutex_unlock(&paper_ready);
    }

    return 0;
}

void *agentB(void *data){
    while(1){
        pthread_mutex_lock(&agent_zem);
        printf("Paper and Match is ready.\n");
        pthread_mutex_unlock(&paper_ready);
        pthread_mutex_unlock(&match_ready);
    }
    return 0;
}

void *agentC(void *data){
    while(1){
        pthread_mutex_lock(&agent_zem);
        printf("Tobacco and Match is ready.\n");
        pthread_mutex_unlock(&tobacco_ready);
        pthread_mutex_unlock(&match_ready);
    }

    return 0;
}

void *pusher_tobacco(void *data){
    while(1){
        pthread_mutex_lock(&tobacco_ready);
        pthread_mutex_lock(&mutex);
        if(is_match){
            is_match=0;
            pthread_mutex_unlock(&paper_zem);
        }
        else if(is_paper){
            is_paper=0;
            pthread_mutex_unlock(&match_zem);
        }
        else{
            is_tobacco=1;
        }
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

void *pusher_match(void *data){
    while(1){
        pthread_mutex_lock(&match_ready);
        pthread_mutex_lock(&mutex);
        if(is_tobacco){
            is_tobacco=0;
            pthread_mutex_unlock(&paper_zem);
        }
        else if(is_paper){
            is_paper=0;
            pthread_mutex_unlock(&tobacco_zem);
        }
        else{
            is_match=1;
        }
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

void *pusher_paper(void *data){
    while(1){
        pthread_mutex_lock(&paper_ready);
        pthread_mutex_lock(&mutex);
        if(is_tobacco){
            is_tobacco=0;
            pthread_mutex_unlock(&match_zem);
        }
        else if(is_match){
            is_match=0;
            pthread_mutex_unlock(&tobacco_zem);
        }
        else{
            is_paper=1;
        }
        pthread_mutex_unlock(&mutex);
    }
    return 0;
}

void *smoker_with_tobacco(void *data){
    while(1){
        pthread_mutex_lock(&tobacco_zem);
        printf("Paper and Match is used by smoker with Tobacco.\n\n");
        sleep(1);
        pthread_mutex_unlock(&agent_zem);
    }

    return 0;
}

void *smoker_with_match(void *data){
    while(1){
        pthread_mutex_lock(&match_zem);
        printf("Tobacco and Paper is used by smoker with Match.\n\n");
        sleep(1);
        pthread_mutex_unlock(&agent_zem);
    }

    return 0;
}

void *smoker_with_paper(void *data){
    while(1){
        pthread_mutex_lock(&paper_zem);
        printf("Tobacco and Match is used by smoker with Paper.\n\n");
        sleep(1);
        pthread_mutex_unlock(&agent_zem);
    }

    return 0;
}



int main(int argc, char *argv[])
{

    pthread_t agentA_thread, agentB_thread, agentC_thread,
             smoker_with_tobacco_thread, smoker_with_match_thread, smoker_with_paper_thread,
             pusher_tobacco_thread, pusher_match_thread, pusher_paper_thread;

    

    pthread_mutex_init(&agent_zem,NULL);
    pthread_mutex_init(&tobacco_zem,NULL);
    pthread_mutex_init(&match_zem,NULL);
    pthread_mutex_init(&paper_zem,NULL);
    pthread_mutex_init(&tobacco_ready,NULL);
    pthread_mutex_init(&match_ready,NULL);
    pthread_mutex_init(&paper_ready,NULL);
    pthread_mutex_init(&mutex,NULL);

    pthread_mutex_lock(&tobacco_zem);
    pthread_mutex_lock(&match_zem);
    pthread_mutex_lock(&paper_zem);
    pthread_mutex_lock(&tobacco_ready);
    pthread_mutex_lock(&match_ready);
    pthread_mutex_lock(&paper_ready);

    is_match = is_paper = is_tobacco = 0;

  
    pthread_create(&agentA_thread, NULL, agentA, NULL);
    pthread_create(&agentB_thread, NULL, agentB, NULL);
    pthread_create(&agentC_thread, NULL, agentC, NULL);

    pthread_create(&pusher_tobacco_thread, NULL, pusher_tobacco, NULL);
    pthread_create(&pusher_match_thread, NULL, pusher_match, NULL);
    pthread_create(&pusher_paper_thread, NULL, pusher_paper, NULL);

    pthread_create(&smoker_with_tobacco_thread, NULL, smoker_with_tobacco, NULL);
    pthread_create(&smoker_with_match_thread, NULL, smoker_with_match, NULL);
    pthread_create(&smoker_with_paper_thread, NULL, smoker_with_paper, NULL);


    pthread_join(agentA_thread, NULL);
    pthread_join(agentB_thread, NULL);
    pthread_join(agentC_thread, NULL);

    pthread_join(pusher_tobacco_thread, NULL);
    pthread_join(pusher_match_thread, NULL);
    pthread_join(pusher_paper_thread, NULL);

    pthread_join(smoker_with_tobacco_thread, NULL);
    pthread_join(smoker_with_match_thread, NULL);
    pthread_join(smoker_with_paper_thread, NULL);
  
  
    return 0;
}