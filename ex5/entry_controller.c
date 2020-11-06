/**
 * CS2106 AY 20/21 Semester 1 - Lab 3
 *
 * Your implementation should go in this file.
 */
#include "entry_controller.h"
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>

#define SHARED_BETWEEN_THREADS 0

int take_and_increment_queue_number( entry_controller_t *entry_controller ) {
	sem_wait(&(entry_controller -> queue_number_semaphore));
	int to_return = entry_controller -> queue_number;
	(entry_controller -> queue_number) = (entry_controller -> queue_number) + 1;
	sem_post(&(entry_controller -> queue_number_semaphore));
	
	return to_return;
}

bool is_my_turn_to_enter_bay( entry_controller_t *entry_controller, int queue_number ) {
	sem_wait(&(entry_controller -> next_queue_number_semaphore));
	bool to_return = queue_number == (entry_controller -> next_queue_number);
	sem_post(&(entry_controller -> next_queue_number_semaphore));

	return to_return;
}

void increment_next_queue_number( entry_controller_t *entry_controller ) {
	sem_wait(&(entry_controller -> next_queue_number_semaphore));
	(entry_controller -> next_queue_number) = (entry_controller -> next_queue_number) + 1;
	sem_post(&(entry_controller -> next_queue_number_semaphore));
}

void wait_bays( entry_controller_t *entry_controller ) {
	sem_wait(&(entry_controller -> num_free_bays));
}

void signal_bays( entry_controller_t *entry_controller ) {
	sem_post(&(entry_controller -> num_free_bays));
}

void entry_controller_init( entry_controller_t *entry_controller, int loading_bays ) {
	entry_controller -> is_debug = false;

	entry_controller -> queue_number = 0;
	entry_controller -> next_queue_number = 0;
	sem_init(&(entry_controller -> num_free_bays), SHARED_BETWEEN_THREADS, loading_bays);
	sem_init(&(entry_controller -> queue_number_semaphore), SHARED_BETWEEN_THREADS, 1);
	sem_init(&(entry_controller -> next_queue_number_semaphore), SHARED_BETWEEN_THREADS, 1);
}

void entry_controller_wait( entry_controller_t *entry_controller ) {
	int my_queue_number = take_and_increment_queue_number(entry_controller);
	
	if (entry_controller -> is_debug) {
		printf("  [Thread %ld]: Joined queue to enter bay, queue number = [%d]\n", pthread_self(), my_queue_number);
	}

	wait_bays(entry_controller);
	while (!is_my_turn_to_enter_bay(entry_controller, my_queue_number)) {
		signal_bays(entry_controller);
		wait_bays(entry_controller);
	}
}

void entry_controller_post( entry_controller_t *entry_controller ) {
	increment_next_queue_number(entry_controller);
	if (entry_controller -> is_debug) {
		printf("  [Thread %ld]: Entered loading bay. Next to enter is [%d].\n", pthread_self(), entry_controller -> next_queue_number);
	}
	signal_bays(entry_controller);
	
}

void entry_controller_destroy( entry_controller_t *entry_controller ) {
	sem_destroy(&(entry_controller -> num_free_bays));
	sem_destroy(&(entry_controller -> queue_number_semaphore));
	sem_destroy(&(entry_controller -> next_queue_number_semaphore));
}

