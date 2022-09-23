#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PHILOSOPERS_NUM 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define WAITING 3
#define LEFT_PHILOSOPHER (philosopher_number+PHILOSOPERS_NUM-1) % PHILOSOPERS_NUM
#define RIGHT_PHILOSOPHER (philosopher_number+1) % PHILOSOPERS_NUM

pthread_mutex_t mutex;
pthread_cond_t cond_var[PHILOSOPERS_NUM];
pthread_t philosopher[PHILOSOPERS_NUM];

void philosphers(int);
void pickup_forks(int);
void return_forks(int);
void test(int);
int state[PHILOSOPERS_NUM];

void philosphers(int n) {
	int sleep_time;
	
	sleep_time = (rand()%3) + 1;
	printf("Philosopher %d is now THINKING for %d seconds.\n", n, sleep_time);
	sleep(sleep_time);
	    
	pickup_forks(n);
	    
	sleep_time = (rand()%3) + 1;
	printf("Philosopher %d is now EATING.\n", n);
	sleep(sleep_time);
	    
	return_forks(n);
	
	return;
}

void pickup_forks(int philosopher_number) {
	pthread_mutex_lock (&mutex);
	
	state[philosopher_number] = HUNGRY;
	printf("Philosopher %d is now HUNGRY and trying to pick up forks.\n", philosopher_number);
	test(philosopher_number);
	while(state[philosopher_number] != EATING) {
	    pthread_cond_wait(&cond_var[philosopher_number], &mutex);
	}
	
	pthread_mutex_unlock (&mutex);
	
	return;
}

void return_forks(int philosopher_number) {
	pthread_mutex_lock (&mutex);
	
	state[philosopher_number] = THINKING;
	printf("Philosopher %d returns forks and then starts TESTING %d and %d.\n", philosopher_number, LEFT_PHILOSOPHER, RIGHT_PHILOSOPHER);
	test(LEFT_PHILOSOPHER);
	test(RIGHT_PHILOSOPHER);
	
	pthread_mutex_unlock (&mutex);
	
	return;
}

void test(int philosopher_number) { 
	if((state[philosopher_number] == HUNGRY || state[philosopher_number] == WAITING)
	   && state[LEFT_PHILOSOPHER] != EATING && state[RIGHT_PHILOSOPHER] != EATING) {
		state[philosopher_number] = EATING;
	    pthread_cond_signal(&cond_var[philosopher_number]);
	}
	else if(state[philosopher_number] == HUNGRY) {
		state[philosopher_number] = WAITING;
		printf("Philosopher %d can't pick up forks and start waiting.\n", philosopher_number);
	}
	
	return;
}

int main() {
    int i;
    srand(time(NULL));
    
    for(i=0; i < PHILOSOPERS_NUM; i++) {
    	state[i] = THINKING;
      	pthread_cond_init(&cond_var[i], NULL);
    }
    pthread_mutex_init (&mutex, NULL);
    
    for(i=0; i < PHILOSOPERS_NUM; i++) {
      	pthread_create(&philosopher[i], NULL, philosphers, i);
    }
    
    for(i=0; i<PHILOSOPERS_NUM; i++) {
    	pthread_join(philosopher[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
