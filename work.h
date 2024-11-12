// DO NOT MODIFY THIS FILE
#pragma once

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

//////////////// Functions to be implemented by the student, in main.cpp ////////////////

// will be called by each working thread, as it starts.
void start_monitoring_for_thread(uint32_t threadIdx, pid_t tid, pthread_t handle);

// will be called after the specific working thread is completed.
void stop_monitoring_for_thread(uint32_t threadIdx);

// will be called for each working thread when work_run() is called.
// return the logical core where the thread should reside for optimcal performance.
int32_t get_thread_affinity(uint32_t threadIdx);


//////////////// Functions in the assignment.so library. ////////////////

// intializes the data structures.
extern "C" int work_init(int seed);

// run all the threads for monitoring, this will call the start_monitoring_for_thread function right after the thread starts,
// and call the stop_monitoring_for_thread function right after the thread ends.
extern "C" void work_start_monitoring();

// run all threads for performance measurement, this will call get_thread_affinity() for each working thread and set the affinity. 
extern "C" void work_run();

