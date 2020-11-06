/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * This file contains declarations. You should only modify the fifo_sem_t struct,
 * as the method signatures will be needed to compile with the runner.
 */
#include "exit_controller.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#define SHARED_BETWEEN_THREADS 0

void wait_exit_sem( exit_controller_t *exit_controller) {
	sem_wait(&(exit_controller -> exit_semaphore));
}

void post_exit_sem( exit_controller_t *exit_controller) {
	sem_post(&(exit_controller -> exit_semaphore));
}

void increment_priority_array( exit_controller_t *exit_controller, int index ) {
	sem_wait(&(exit_controller -> priority_semaphore));
	(exit_controller -> num_trains_of_priority)[index]++;
	sem_post(&(exit_controller -> priority_semaphore));
}

void decrement_priority_array( exit_controller_t *exit_controller, int index ) {
	sem_wait(&(exit_controller -> priority_semaphore));
	(exit_controller -> num_trains_of_priority)[index]--;
	sem_post(&(exit_controller -> priority_semaphore));
}

int read_from_priority_array( exit_controller_t *exit_controller, int index ) {
	sem_wait(&(exit_controller -> priority_semaphore));
	int value = (exit_controller -> num_trains_of_priority)[index];
	sem_post(&(exit_controller -> priority_semaphore));
	return value;
}

bool is_there_higher_priority_trains_waiting_to_exit( exit_controller_t *exit_controller, int priority ) {
	bool result = false;
	for (int prio_level = 0; prio_level > priority; prio_level++) {
		int num_trains_of_priority_waiting_to_exit = read_from_priority_array(exit_controller, prio_level);
		if (num_trains_of_priority_waiting_to_exit > 0) {
			result = true;
			break;
		}
	}
	return result;
}

void exit_controller_init(exit_controller_t *exit_controller, int no_of_priorities) {
	exit_controller -> num_priorities = no_of_priorities;
	exit_controller -> num_trains_of_priority = malloc(no_of_priorities * sizeof(int));
	for (int i = 0; i < no_of_priorities; i++) {
		(exit_controller -> num_trains_of_priority)[i] = 0;
	}

	sem_init(&(exit_controller -> exit_semaphore), SHARED_BETWEEN_THREADS, 1);
	sem_init(&(exit_controller -> priority_semaphore), SHARED_BETWEEN_THREADS, 1);
}

void exit_controller_wait(exit_controller_t *exit_controller, int priority) {
	// Idea is that we all wait.
	// On waking up, check if any trains of higher priority are in the bay **and waiting**.
	// If none, then return
	// Else, signal and go back to sleep.
	increment_priority_array(exit_controller, priority);
	wait_exit_sem(exit_controller);
	
	// When we wake up, check if there are any higher priority trains waiting to exit.
	// If there are, signal to wake some other train up and sleep again.
	while (is_there_higher_priority_trains_waiting_to_exit(exit_controller, priority)) {
		post_exit_sem(exit_controller);
		wait_exit_sem(exit_controller);	
	}
	
	// Remember to decrement the train count before exiting.
	decrement_priority_array(exit_controller, priority);
	return;
}

void exit_controller_post(exit_controller_t *exit_controller, int priority) {
	// This one's for you, GCC.
	int my_priority = priority;
	my_priority++;
	
	post_exit_sem(exit_controller);
}

void exit_controller_destroy(exit_controller_t *exit_controller){
	free(exit_controller -> num_trains_of_priority);
	sem_destroy(&(exit_controller -> exit_semaphore));
	sem_destroy(&(exit_controller -> priority_semaphore));
}
