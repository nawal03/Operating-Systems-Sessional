#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "zemaphore.h"

zem_t agent_zem, tobacco_ready, match_ready, paper_ready, tobacco_zem, match_zem, paper_zem;
zem_t mutex;
int is_tobacco, is_match, is_paper;

void *agentA(void *data){
    while(1){
        zem_down(&agent_zem);
        printf("Tobacco and Paper is ready.\n");
        zem_up(&tobacco_ready);
        zem_up(&paper_ready);
    }

    return 0;
}

void *agentB(void *data){
    while(1){
        zem_down(&agent_zem);
        printf("Paper and Match is ready.\n");
        zem_up(&paper_ready);
        zem_up(&match_ready);
    }

    return 0;
}

void *agentC(void *data){
    while(1){
        zem_down(&agent_zem);
        printf("Tobacco and Match is ready.\n");
        zem_up(&tobacco_ready);
        zem_up(&match_ready);
    }

    return 0;
}

void *pusher_tobacco(void *data){
    while(1){
        zem_down(&tobacco_ready);
        zem_down(&mutex);
        if(is_match){
            is_match=0;
            zem_up(&paper_zem);
        }
        else if(is_paper){
            is_paper=0;
            zem_up(&match_zem);
        }
        else{
            is_tobacco=1;
        }
        zem_up(&mutex);
    }
    return 0;
}

void *pusher_match(void *data){
    while(1){
        zem_down(&match_ready);
        zem_down(&mutex);
        if(is_tobacco){
            is_tobacco=0;
            zem_up(&paper_zem);
        }
        else if(is_paper){
            is_paper=0;
            zem_up(&tobacco_zem);
        }
        else{
            is_match=1;
        }
        zem_up(&mutex);
    }

    return 0;
}

void *pusher_paper(void *data){
    while(1){
        zem_down(&paper_ready);
        zem_down(&mutex);
        if(is_tobacco){
            is_tobacco=0;
            zem_up(&match_zem);
        }
        else if(is_match){
            is_match=0;
            zem_up(&tobacco_zem);
        }
        else{
            is_paper=1;
        }
        zem_up(&mutex);
    }
    return 0;
}

void *smoker_with_tobacco(void *data){
    while(1){
        zem_down(&tobacco_zem);
        printf("Paper and Match is used by smoker with Tobacco.\n\n");
        sleep(1);
        zem_up(&agent_zem);
    }

    return 0;
}

void *smoker_with_match(void *data){
    while(1){
        zem_down(&match_zem);
        printf("Tobacco and Paper is used by smoker with Match.\n\n");
        sleep(1);
        zem_up(&agent_zem);
    }

    return 0;
}

void *smoker_with_paper(void *data){
    while(1){
        zem_down(&paper_zem);
        printf("Tobacco and Match is used by smoker with Paper.\n\n");
        sleep(1);
        zem_up(&agent_zem);
    }

    return 0;
}



int main(int argc, char *argv[])
{

    pthread_t agentA_thread, agentB_thread, agentC_thread,
             smoker_with_tobacco_thread, smoker_with_match_thread, smoker_with_paper_thread,
             pusher_tobacco_thread, pusher_match_thread, pusher_paper_thread;

    

    zem_init(&agent_zem,1);
    zem_init(&tobacco_zem,0);
    zem_init(&match_zem,0);
    zem_init(&paper_zem,0);
    zem_init(&tobacco_ready,0);
    zem_init(&match_ready,0);
    zem_init(&paper_ready,0);
    zem_init(&mutex,1);

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