/*
 * pid_api.h
 *
 * Author: Alex Lay
 * Created on: Feb 5, 2018
 * Course: Operating Systems
 * Desc: This header file contains the function prototypes
 * 		 for the pid_api specified in problem 3.20.
 */

#ifndef PID_API_H_
#define PID_API_H_

#include <stdio.h>

extern int allocate_map(void);
extern int allocate_pid(void);
extern void release_pid(int pid);
extern int get_pid_value(int pid);
extern void print_pids();

#endif /* PID_API_H_ */
