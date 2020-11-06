CC=gcc
CFLAGS=-g -std=c99 -Wall -Wextra -D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE
LDLIBS=-lpthread

.PHONY: all

all:train_runner_small_test_exit train_runner_big_test_exit

train_runner_small_test_exit: entry_controller.o exit_controller.o train_runner_small_test_exit.o
train_runner_big_test_exit: entry_controller.o exit_controller.o train_runner_big_test_exit.o

.PHONY: clean
clean:
	rm entry_controller.o exit_controller.o train_runner_small_test_exit.o train_runner_big_test_exit.o train_runner_small_test_exit train_runner_big_test_exit




