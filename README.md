# OS Scheduler Project

## Overview

This project, part of the Computer Engineering curriculum at Cairo University, focuses on the development of an OS Scheduler. It is designed to evaluate different scheduling algorithms, implement IPC (Inter-Process Communication) techniques, and optimize the use of algorithms and data structures.

## Objectives

- Evaluate different scheduling algorithms.
- Practice the use of IPC techniques.
- Optimize algorithms and data structures for efficiency.

## Environment

- **Platform:** Linux
- **Programming Language:** C

## Components

1. **Process Generator:** Manages process creation, scheduling algorithm selection, and IPC with the scheduler.
2. **Clock Module:** Emulates an integer time clock.
3. **Scheduler:** Core component that tracks processes, decides process execution order, and implements three scheduling algorithms:
   - Non-preemptive Highest Priority First (HPF)
   - Shortest Remaining Time Next (SRTN)
   - Round Robin (RR)
4. **Process Module:** Acts as CPU-bound processes, notifying the scheduler upon completion.

## Input/Output

- Input: `processes.txt` (format defined in the project document).
- Output: `scheduler.log` and `scheduler.perf` files, logging process details and performance metrics.


