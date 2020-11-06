CC=gcc
CFLAGS=-g -std=c99 -Wall -Wextra -D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE
LDLIBS=-lpthread

.PHONY: all

all:train_runner_small_test train_runner_big_test

train_runner_small_test: entry_controller.o exit_controller.o train_runner_small_test.o
train_runner_big_test: entry_controller.o exit_controller.o train_runner_big_test.o

.PHONY: clean
clean:
	rm entry_controller.o exit_controller.o train_runner_small_test.o train_runner_big_test.o train_runner_small_test train_runner_big_test




