CC=gcc
CFLAGS=-g -std=c99 -Wall -Wextra -D_POSIX_C_SOURCE=200809L -D_GNU_SOURCE
LDLIBS=-lpthread

.PHONY: clean

train_runner_ex5: entry_controller.o exit_controller.o train_runner_ex5.o
clean:
	rm entry_controller.o exit_controller.o train_runner_ex5.o train_runner_ex5

.PHONY: all
all: train_runner_ex5
